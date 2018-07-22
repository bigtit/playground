#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

static int create_and_bind (char *port, int socktype) {
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int s, sfd;

  memset (&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
  hints.ai_socktype = socktype; /* We want a TCP socket */
  hints.ai_flags = AI_PASSIVE;     /* All interfaces */

  s = getaddrinfo (NULL, port, &hints, &result);
  if (s != 0) {
    fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
    return -1;
  }
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1) continue;

    s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
    if (s == 0) break;
    close (sfd);
  }
  if (rp == NULL) {
    fprintf (stderr, "Could not bind\n");
    return -1;
  }
  freeaddrinfo (result);
  return sfd;
}

static int make_socket_non_blocking (int sfd) {
  int flags, s;
  flags = fcntl (sfd, F_GETFL, 0);
  if (flags == -1) {
    perror ("fcntl");
    return -1;
  }
  flags |= O_NONBLOCK;
  s = fcntl (sfd, F_SETFL, flags);
  if (s == -1) {
    perror ("fcntl");
    return -1;
  }
  return 0;
}

#define MAXEVENTS 64

static const char reply[] =
"HTTP/1.0 200 OK\r\n"
"Content-type: text/html\r\n"
"Connection: close\r\n"
"Content-Length: 82\r\n"
"\r\n"
"<html>\n"
"<head>\n"
"<title>performance test</title>\n"
"</head>\n"
"<body>\n"
"test\n"
"</body>\n"
"</html>";

int main (int argc, char *argv[]) {
  int sfd, s;
  int efd;
  struct epoll_event event;
  struct epoll_event *events;

  if (argc != 3) {
    fprintf (stderr, "Usage: %s [tcp port] [udp port]\n", argv[0]);
    exit (EXIT_FAILURE);
  }

  sfd = create_and_bind (argv[1], SOCK_STREAM);
  if (sfd == -1) abort ();

  s = make_socket_non_blocking (sfd);
  if (s == -1) abort ();

  s = listen (sfd, SOMAXCONN);
  if (s == -1) {
    perror ("listen");
    abort ();
  }

  efd = epoll_create1 (0);
  if (efd == -1) {
    perror ("epoll_create");
    abort ();
  }

  event.data.fd = sfd;
  event.events = EPOLLIN | EPOLLET;
  s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);
  if (s == -1) {
    perror ("epoll_ctl");
    abort ();
  }

  // udp server setup
  int udp_sock, udp_clientlen;
  struct sockaddr_in udp_clientaddr;
  struct hostent* udp_hostp;
  char* udp_hostaddrp;

  udp_sock = create_and_bind(argv[2], SOCK_DGRAM);
  s = make_socket_non_blocking(udp_sock);

  event.data.fd = udp_sock;
  event.events = EPOLLIN | EPOLLET;
  s = epoll_ctl (efd, EPOLL_CTL_ADD, udp_sock, &event);
  // end udp server setup


  /* Buffer where events are returned */
  events = calloc (MAXEVENTS, sizeof event);

  /* The event loop */
  while (1) {
    int n, i;

    n = epoll_wait (efd, events, MAXEVENTS, -1);
    for (i = 0; i < n; i++) {
      if ((events[i].events & EPOLLERR) ||
          (events[i].events & EPOLLHUP) ||
          (!(events[i].events & EPOLLIN))) {
        /* An error has occured on this fd, or the socket is not
         * ready for reading (why were we notified then?) */
        fprintf (stderr, "epoll error. events=%u\n", events[i].events);
        close (events[i].data.fd);
        continue;
      }

      else if (sfd == events[i].data.fd) {
        /* We have a notification on the listening socket, which
         * means one or more incoming connections. */
        while (1) {
          struct sockaddr in_addr;
          socklen_t in_len;
          int infd;

          in_len = sizeof in_addr;
          infd = accept (sfd, &in_addr, &in_len);
          if (infd == -1) {
            printf("errno=%d, EAGAIN=%d, EWOULDBLOCK=%d\n", errno, EAGAIN, EWOULDBLOCK);
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
              /* We have processed all incoming
               * connections. */
              printf ("processed all incoming connections.\n");
              break;
            } else {
              perror ("accept");
              break;
            }
          }
          /* Make the incoming socket non-blocking and add it to the
           * list of fds to monitor. */
          s = make_socket_non_blocking (infd);
          if (s == -1) abort ();

          event.data.fd = infd;
          event.events = EPOLLIN | EPOLLET;
          printf("set events %u, infd=%d\n", event.events, infd);
          s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);
          if (s == -1) {
            perror ("epoll_ctl");
            abort ();
          }
        }
      } else if (udp_sock == events[i].data.fd) {
        int count;
        char buf[512];
        count = recvfrom(udp_sock, buf, 512, 0, (struct sockaddr*)&udp_clientaddr, (socklen_t*)&udp_clientlen);
        udp_hostp = gethostbyaddr((const char*)&udp_clientaddr.sin_addr.s_addr, sizeof(udp_clientaddr.sin_addr.s_addr), AF_INET);
        udp_hostaddrp = inet_ntoa(udp_clientaddr.sin_addr);
        printf("server received datagram from %s (%s)\n", udp_hostp->h_name, udp_hostaddrp);
        printf("server received %ld / %d bytes: %s\n", strlen(buf), count, buf);
        count = sendto(udp_sock, buf, strlen(buf), 0, (struct sockaddr*)&udp_clientaddr, udp_clientlen);
      } else {
        /* We have data on the fd waiting to be read. Read and
         * display it. We must read whatever data is available
         * completely, as we are running in edge-triggered mode
         * and won't get a notification again for the same
         * data. */
        int done = 0;

        while (1) {
          ssize_t count;
          char buf[512];

          count = read (events[i].data.fd, buf, sizeof buf);
          if (count == -1) {
            /* If errno == EAGAIN, that means we have read all
             * data. So go back to the main loop. */
            if (errno != EAGAIN) {
              perror ("read");
              done = 1;
            }
            break;
          }
          else if (count == 0) {
            /* End of file. The remote has closed the
             * connection. */
            done = 1;
            break;
          }

          /* Write the reply to connection */
          s = write (events[i].data.fd, reply, sizeof(reply));
          if (s == -1) {
            perror ("write");
            abort ();
          }
        }

        if (done) {
          printf ("Closed connection on descriptor %d\n", events[i].data.fd);
          /* Closing the descriptor will make epoll remove it
           * from the set of descriptors monitored. */
          close (events[i].data.fd);
        }
      }
    }
  }

  free (events);

  close (sfd);

  return EXIT_SUCCESS;
}
