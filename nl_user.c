#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <string.h>

#define MAX_PAYLOAD 1024 // max size of a msg
#define NETLINK_TEST 17

int main(int argc, char** argv){
  struct sockaddr_nl dest_addr;
  struct nlmsghdr* nlh = 0;
  struct iovec iov;
  int sock_fd = -1;
  struct msghdr msg;

  sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TEST);
  if(-1==sock_fd){
    perror("create socket error");
    return -1;
  }

  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.nl_family = AF_NETLINK;
  dest_addr.nl_pid = 0;
  dest_addr.nl_groups = 0;

  bind(sock_fd, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
  nlh = (struct nlmsghdr*)malloc(NLMSG_SPACE(MAX_PAYLOAD));
  memset(nlh, 0, MAX_PAYLOAD);
  nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
  nlh->nlmsg_pid = 0;
  nlh->nlmsg_type = NLMSG_NOOP;
  nlh->nlmsg_flags = 0;

  strcpy(NLMSG_DATA(nlh), argv[1]);

  memset(&iov, 0, sizeof(iov));
  iov.iov_base = (void*)nlh;
  iov.iov_len = nlh->nlmsg_len;
  memset(&msg, 0, sizeof(msg));
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;

  sendmsg(sock_fd, &msg, 0);

  close(sock_fd);
  free(nlh);

  return 0;
}

