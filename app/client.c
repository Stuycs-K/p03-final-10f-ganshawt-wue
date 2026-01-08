#include "networking.h"

void sigint_handler(int sig) {
  //printf("Client exit");
  exit(0);
}

void clientLogic(int server_socket){
  char buffer[BUFFER_SIZE];
  while ()  {
    printf("Waiting for 4 players...");
  }
  // if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
  //   //printf("\nClient exit\n");
  //   close(server_socket);
  //   kill(getpid(), EOF);
  //   exit(0);
  // }
  // buffer[strcspn(buffer, "\n")] = '\0';
  // //Send input to server
  // int bytes_written = write(server_socket, buffer, strlen(buffer));
  // if (bytes_written < 0) {
  //   //printf("write() error: %s\n", strerror(errno));
  //   return;
  // }
  // //printf("Client sent string to server: %s\n", buffer);
  // int bytes_read = read(server_socket, buffer, sizeof(buffer) - 1);
  // if (bytes_read < 0) {
  //   //printf("read() error: %s\n", strerror(errno));
  //   exit(0);
  // }
  // if (bytes_read == 0) {
  //   //printf("Server disconnected\n");
  //   close(server_socket);
  //   //exit(0);
  // }
  // buffer[bytes_read] = '\0';
}

int main(int argc, char *argv[] ) {
  signal(SIGINT, sigint_handler);
  static char IP[256] = "127.0.0.1";
  if(argc > 1)  {
    strncpy(IP, argv[1], sizeof(IP) - 1);
  }
  else  {
    FILE *fp = fopen(".server_ip", "r");
    if (fp) {
      if (fscanf(fp, "%255s", IP) == 1) {
        //printf("Connecting to server at: %s\n", IP);
      }
      else  {
        strcpy(IP, "127.0.0.1");
      }
      fclose(fp);
    }
    else  {
      //printf("No server ip found, using 127.0.0.1\n");
    }
  }
  int server_socket = client_tcp_handshake(IP);
  //printf("Client connected.\n");
  while (1) {
    clientLogic(server_socket);
  }
  close(server_socket);
  return 0;
}
