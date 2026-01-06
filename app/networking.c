#include "networking.h"

int server_setup() {
  //setup structs for getaddrinfo
  struct addrinfo *hints, *results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;//TCP socket
  hints->ai_flags = AI_PASSIVE;
  //getaddrinfo
  int getaddrinfo_result = getaddrinfo(NULL, PORT, hints, &results);
  if (getaddrinfo_result != 0) {
    //printf("getaddrinfo() error: %s\n", gai_strerror(getaddrinfo_result));
    exit(1);
  }
  //printf("Server getaddrinfo successful\n");
  //create the socket
  int clientd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);//store the socket descriptor here
  if (clientd < 0) {
    //printf("socket() error: %s\n", strerror(errno));
  	exit(1);
  }
  //printf("Server socket created: %d\n", clientd);
  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  if (sockOpt < 0) {
    //printf("setsockopt() error: %s\n", strerror(errno));
    exit(1);
  }
  //printf("Server setsockopt successful\n");
  //bind the socket to address and port
  int bind_result = bind(clientd, results->ai_addr, results->ai_addrlen);
  if (bind_result < 0) {
    //printf("bind() error: %s\n", strerror(errno));
    exit(1);
  }
  //printf("Server bind successful on port \n");
  //set socket to listen state
  int listen_result = listen(clientd, 10);
  if (listen_result < 0) {
    //printf("listen() error: %s\n", strerror(errno));
    exit(1);
  }
  //printf("Server listening for client...\n");
  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);
  return clientd;
}
