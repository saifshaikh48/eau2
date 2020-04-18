#include "message.h"
#include "network_ifc.h"
#include "network_ip.h"


int main (int argc, char* argv[])
{


  NetworkIP*  nip = new NetworkIP();

  // char* val = new char[9];
  // val[0] = '1';
  // val[1] = '2';
  // val[2] = '7';
  // val[3] = '.';
  // val[4] = '0';
  // val[5] = '.';
  // val[6] = '0';
  // val[7] = '.';
  // val[8] = '1';

  int port = atoi(argv[1]);
  char* address = argv[2];
  int idx = atoi(argv[3]);
  nip->client_init(idx, port, address, 8000, 4);
  // nip->client_init(1, port, address, 8000, 2);


  return 0;

}
