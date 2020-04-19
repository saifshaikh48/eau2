#pragma once
#include "../../util/serializer.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>

class Message {
public:
	char type;
	unsigned src;
	unsigned dest;
	const char* payload;
	size_t payload_size;

	Message(unsigned sender, unsigned receiver, char t, const char* serialized_data, size_t size_of_serialized) {
		type = t;
		src = sender;
		dest = receiver;
		payload = serialized_data;
		payload_size = size_of_serialized;
		//printf("THIS IS MESSAGE SIZE OF SERIALIZED: %zu\n", payload_size);
	}

	Message() {
		type = 'M';
		src = 0;
		dest = 0;
		payload = " ";
		payload_size = 0;
	}

	virtual ~Message() {}

	//should add serialization and deserialization
 	const char* serialize() {
		//printf("In Message Serialize\n");
		Serialize* serial = new Serialize();
		serial->serialize(type);
		serial->serialize(src);
		serial->serialize(dest);

//TODO: IMPORTANT THIS IS NOT WORKING
		serial->serialize(payload_size);
		serial->serialize(payload_size,payload);
		return serial->buffer;
	}

	size_t sizeof_serialized() {
		return (2 * sizeof(unsigned)) + sizeof(char)
		+ sizeof(size_t) + payload_size * sizeof(char) * 2;
	}

	virtual unsigned target() { return dest; }

	static Message* deserialize(const char* msg) {
		size_t buff_size = 0;

		char type = Deserialize::deserialize_char(msg);
		buff_size += sizeof(type);

		unsigned src = Deserialize::deserialize_unsigned(msg + buff_size);
		buff_size += sizeof(src);

		unsigned dest = Deserialize::deserialize_unsigned(msg + buff_size);
		buff_size += sizeof(dest);

		size_t p_size = Deserialize::deserialize_size_t(msg + buff_size);
		buff_size += sizeof(p_size);

    // printf("%c\n", type);
    // printf("%u\n", src);
    // printf("%u\n", dest);
    // printf("%zu\n", p_size);


		char* payload = Deserialize::deserialize(msg + buff_size, p_size);
		buff_size += sizeof(payload);


    // printf("%s\n", payload);


		Message* message = new Message(src, dest, type, payload, p_size);

		return message;

	}
};

class Register : public Message {
public:
		char type;
    sockaddr_in client;
    size_t port;

		Register(unsigned idx, unsigned p) {
			type = 'R';
			struct sockaddr_in cl;
			cl.sin_addr.s_addr = idx;
			client = cl;
			port = p;

			// inet_aton("127.0.0.1", &client.sin_addr);

			src = idx;
			dest = 0;
		}



		virtual size_t sizeof_serialized() {
  		return sizeof(unsigned) + (sizeof(size_t));
  	}

		unsigned sender() {
			return client.sin_addr.s_addr;
		}



		const char* serialize() {
			// printf("In register Serialize\n");

			Serialize* serial = new Serialize();

			serial->serialize(port);
			serial->serialize(client.sin_addr.s_addr);

			return serial->buffer;

		}


		static Register* deserialize(const char* msg) {

			size_t buff_size = 0;

			size_t port = Deserialize::deserialize_size_t(msg + buff_size);
			buff_size += sizeof(size_t);

			unsigned idx = Deserialize::deserialize_unsigned(msg + buff_size);
			buff_size += sizeof(unsigned);


			Register* reg = new Register(idx, port);

			return reg;
		}
};



class Directory : public Message {
public:
	 char type;
	 size_t size_dir;
   size_t * ports;  // owned
   const char ** addresses;  // owned; strings owned


	 Directory(size_t dirsz, size_t* p, const char** addrs) {
		 type = 'D';
		 size_dir = dirsz;
		 ports = p;
		 addresses = addrs;

		 src = 0;
		 dest = 0;
	 }

	 virtual size_t sizeof_serialized() {
 		return sizeof(size_t) + size_dir * ((sizeof(size_t) + 7));
 	}

	  const char* serialize() {

		 Serialize* serial = new Serialize();
		 serial->serialize(size_dir);

		 for (size_t i = 0; i < size_dir; i++) {
			 serial->serialize(ports[i]);
		 }
		 for (size_t i = 0; i < size_dir; i++) {
			 // printf("size of address: %zu\n", strlen(addresses[i]));
			 // printf("address: %s\n", addresses[i]);

			 serial->serialize(7, addresses[i]);
		 }

		 return serial->buffer;

	 }

	 static Directory* deserialize(const char* message) {
		 size_t buff_size = 0;
		 // printf("deserializing directory\n");

		 size_t size_dir = Deserialize::deserialize_size_t(message + buff_size);
		 buff_size = sizeof(size_t);

		 // printf("deserialized size dir: %zu\n", size_dir);

		 size_t* ports = new size_t[size_dir];
		 for (size_t i = 0; i < size_dir; i++) {
			 ports[i] = Deserialize::deserialize_size_t(message + buff_size);
			 // printf("deserialized port %zu: %zu\n", i, ports[i]);

			 buff_size += sizeof(size_t);
		 }

		 const char** addresses = new const char*[size_dir];
		 for (size_t i = 0; i < size_dir; i++) {
			 // addresses[i] = Deserialize::deserialize(message + buff_size, 7);
			 //std::string addr = "127.0.0." + std::to_string(i);
			 //addresses[i] = addr.c_str();
			 addresses[i] = "127.0.0.1";
			 // printf("deserialized address %zu: %s\n", i, addresses[i]);

			 buff_size += 7;
		 }


		 Directory* dir = new Directory(size_dir, ports, addresses);

		 return dir;
	 }

};
