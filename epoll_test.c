#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define MAXEVENTS 64

int main(){
  int msock, addrlen, nsock;
  int max_sd, sd;
  struct sockaddr_in addr;
  char buf[1024];
  char* msg = "EPOLL TEST V1.0\n";

  // epoll vars
  struct epoll_event ev, evs[MAXEVENTS];
  int epfd = epoll_create(1);

  msock = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(8888);

  // epoll vars
  ev.data.fd = msock;
  ev.events = EPOLLIN|EPOLLET;
  epoll_ctl(epfd, EPOLL_CTL_ADD, msock, &ev);

  bind(msock, (struct sockaddr*)&addr, sizeof(addr));
  puts("listening on port 8888");
  listen(msock, 0); // set status
  addrlen = sizeof(addr);
  puts("waiting for connections...");

  for(;;){
    int nfds = epoll_wait(epfd, evs, MAXEVENTS, -1);
    for(int i=0; i<nfds; ++i){
      if(evs[i].data.fd==msock){ // connections come
        nsock = accept(msock, (struct sockaddr*)&addr, &addrlen);
        printf("new connection: sockfd: %d, ip: %s, port: %d\n",
            nsock, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        send(nsock, msg, strlen(msg), 0);
        puts("message sent ok");

        ev.data.fd = nsock;
        ev.events = EPOLLIN|EPOLLET;
        epoll_ctl(epfd, EPOLL_CTL_ADD, nsock, &ev);
      }
      else if(evs[i].events&EPOLLIN){ // readable
        sd = evs[i].data.fd;
        if(sd<0) continue;
        int n = read(sd, buf, 1024);
        if(!n){
          getpeername(sd, (struct sockaddr*)&addr, &addrlen);
          printf("host disconnected, ip: %s, port: %d\n",
            inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
          close(sd);
          evs[i].data.fd = -1;
        }
        else{
          buf[n] = 0;
          send(sd, buf, strlen(buf), 0);
          // set writable epoll
          //ev.data.fd = sd;
          //ev.events = EPOLLOUT|EPOLLET;
        }
      }
      //else if(evs[i].events&EPOLLOUT){ // writable
      //  sd = evs[i].data.fd;
      //  write(sd, buf, strlen(buf));
      //  ev.data.fd = sd;
      //  ev.events = EPOLLIN|EPOLLET;
      //  epoll_ctl(epfd, EPOLL_CTL_ADD, sd, &ev);
      //}
    }
  }

  return 0;
}
