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

int server_tcp_handshake(int listen_socket){
  int client_socket;
  socklen_t sock_size;
  struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);
  //accept() the client connection
  client_socket = accept(listen_socket, (struct sockaddr *)&client_address, &sock_size);
  if (client_socket < 0) {
    //printf("accept() error: %s\n", strerror(errno));
    exit(1);
  }
  //printf("Server accepted from client @ socket: %d\n", client_socket);
  return client_socket;
}

int client_tcp_handshake(char * server_address) {
  struct addrinfo *hints, *results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;//TCP
  //getaddrinfo
  int getaddrinfo_result = getaddrinfo(server_address, PORT, hints, &results);
  if (getaddrinfo_result != 0) {
    //printf("getaddrinfo() error: %s\n", gai_strerror(getaddrinfo_result));
    exit(1);
  }
  //printf("Client getaddrinfo successful\n");
  int serverd;//store the socket descriptor here
  //create the socket
  serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  if (serverd < 0) {
    //printf("socket() error: %s\n", strerror(errno));
    exit(1);
  }
  //printf("Client socket created: %d\n", serverd);
  //connect() to the server
  int connect_result = connect(serverd, results->ai_addr, results->ai_addrlen);
  if (connect_result < 0) {
    //printf("connect() error: %s\n", strerror(errno));
    exit(1);
  }
  //printf("Client connected to server %s:10101\n", server_address);
  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);
  return serverd;
}
