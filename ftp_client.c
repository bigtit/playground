#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

char* server_name = "192.168.1.109";
int port = 21;
char* username = "";
char* password = "";

int main(){
  int control_sock, data_sock;
  struct hostent* hp;
  struct sockaddr_in server;
  struct sockaddr_in serverd; // addr for server data
  memset(&server, 0, sizeof(server));

  char read_buf[256];
  int read_len = 256;
  char send_buf[256];
  char* dir_name = "/";

  control_sock = socket(AF_NETLINK, SOCK_STREAM, 0);
  hp = gethostbyname(server_name);
  memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  connect(control_sock, (struct sockaddr*)&server, sizeof(server));
  read(control_sock, read_buf, read_len); // read welcome msg
  sprintf(send_buf, "USER %s\r\n", username);
  write(control_sock, send_buf, strlen(send_buf)); // write username
  read(control_sock, read_buf, read_len);
  sprintf(send_buf, "PASS %s\r\n", password);
  write(control_sock, send_buf, strlen(send_buf)); // write pass
  read(control_sock, read_buf, read_len);
  sprintf(send_buf, "PASV\r\n"); // tell server to use passive mode
  write(control_sock, send_buf, strlen(send_buf)); // write pass
  read(control_sock, read_buf, read_len);
  // passive mode means using port given by server to connect to data channel
  // set up data_sock using read_buf
  data_sock = socket(AF_NETLINK, SOCK_STREAM, 0);
  memcpy(&serverd, &server, sizeof(server));
  serverd.sin_port = (read_buf[strlen(read_buf)-2]<<8) + read_buf[strlen(read_buf)-1];

  connect(data_sock, (struct sockaddr*)&serverd, sizeof(server));
  sprintf(send_buf, "CWD %s\r\n", dir_name); // tell server to use passive mode
  write(control_sock, send_buf, strlen(send_buf)); // write pass
  read(control_sock, read_buf, read_len);

  return 0;
}
