#include "message.h"
#include "network_ifc.h"
#include "network_ip.h"


int main (int argc, char* argv[])
{
  NetworkIP*  nip = new NetworkIP();

  nip->server_init(0, 8000, 4);

  printf("clients registered\n");
  // while (true) {

  // }
  return 0;

}
