#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

int main(){
  int fd, size;
  struct sockaddr_un un;
  memset(&un, 0, sizeof(un));
  strcpy(un.sun_path, "./foo.socket");
  fd = socket(AF_UNIX, SOCK_STREAM, 0);
  size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
  bind(fd, (struct sockaddr*)&un, size);
  printf("UNIX domain socket bould\n");
  return 0;
}

