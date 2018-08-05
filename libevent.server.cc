#include <memory>
#include <cstdint>
#include <iostream>
#include <cstring>

#include <arpa/inet.h>
#include <signal.h>

#include <evhttp.h>
#include <event2/listener.h>

#define MAX_LINE_LENGTH 10240
#define MAX_OUTPUT_BUFFER 102400


int main(int argc, char** argv) {
  if(argc!=3) {
    std::cout << argv[0] << " [http port] [echo port]\n";
    return -1;
  }
  unsigned short hport = std::stoi(argv[1]), eport = std::stoi(argv[2]);
  auto ebm = event_base_new();
  auto hs = evhttp_new(ebm);
  evhttp_set_allowed_methods(hs, EVHTTP_REQ_GET);

  auto home = [](struct evhttp_request* req, void* p) {
    struct evbuffer* buf = evbuffer_new();
    if(!buf) return;
    evbuffer_add_printf(buf, "success");
    // evbuffer_add_printf(buf, "requested: %s\n", evhttp_request_uri(req));
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
  };
  auto notfound = [](struct evhttp_request* req, void* p) {
    evhttp_send_error(req, HTTP_NOTFOUND, "not found");
  };
  // evhttp_set_cb(hs, "/index", home, 0);
  evhttp_set_gencb(hs, home, 0);

  // start server
  if(evhttp_bind_socket(hs, "0.0.0.0", hport)) perror("cannot bind to localhost");













  // now we want add a tcp server to eb
  auto accept_cb = [](struct evconnlistener* ls, evutil_socket_t fd, struct sockaddr* addr, int socklen, void* arg) {
    // read_cb
    auto read_cb = [](struct bufferevent* be, void*) {
      auto input = bufferevent_get_input(be);
      auto output = bufferevent_get_output(be);
      // receive data until end
      for(;;) {
        size_t eol_len = 0;
        auto eol = evbuffer_search_eol(input, NULL, &eol_len, EVBUFFER_EOL_LF);
        if(eol.pos<0) {
          auto readable = evbuffer_get_length(input);
          if(readable>MAX_LINE_LENGTH) {
            std::cout << "input too long\n";
            bufferevent_free(be);
          }
          break;
        } else if(eol.pos>MAX_LINE_LENGTH) {
          std::cout << "line too long\n";
          bufferevent_free(be);
          break;
        } else {
          char req[MAX_LINE_LENGTH+1];
          auto req_len = evbuffer_remove(input, req, eol.pos+1);
          char resp[MAX_LINE_LENGTH+1];
          auto resp_len = req_len;
          memcpy(resp, req, req_len);

          evbuffer_add(output, resp, resp_len);
          if(evbuffer_get_length(output)>MAX_OUTPUT_BUFFER) {
            std::cout << "output buffer too long\n";
            bufferevent_free(be);
            break;
          }
        }
      }
    };
    // event_cb
    auto event_cb = [](struct bufferevent* be, short ev, void*) {
      if(ev & BEV_EVENT_ERROR) perror("error bufferevent");
      if(ev & (BEV_EVENT_EOF|BEV_EVENT_ERROR)) bufferevent_free(be);
    };
    char a[INET_ADDRSTRLEN];
    auto sin = (struct sockaddr_in*)addr;
    inet_ntop(AF_INET, &sin->sin_addr, a, INET_ADDRSTRLEN);
    std::cout << "accept connection from: " << a << std::endl;

    auto eb = evconnlistener_get_base(ls);
    auto be = bufferevent_socket_new(eb, fd, BEV_OPT_CLOSE_ON_FREE);
    // set_tcp_no_delay(fd);
    bufferevent_setcb(be, read_cb, NULL, event_cb, NULL);
    bufferevent_enable(be, EV_READ|EV_WRITE);
  };
  auto accept_error_cb = [](struct evconnlistener* ls, void* arg) {
    auto eb = evconnlistener_get_base(ls);
    auto err = EVUTIL_SOCKET_ERROR();
    std::cerr << "error: " << evutil_socket_error_to_string(err) << std::endl;
    event_base_loopexit(eb, NULL);
  };

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(eport);
  auto listener = evconnlistener_new_bind(ebm, accept_cb, nullptr, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
                                          (struct sockaddr*)&sin, sizeof(sin));
  if(!listener) perror("cannot create tcp socket");
  evconnlistener_set_error_cb(listener, accept_error_cb);














  // signal processing
  auto sig_cb = [](evutil_socket_t fd, short what, void* arg) {
    struct event_base* eb = (struct event_base*)arg;
    std::cout << "sigint captured\n";
    event_base_loopexit(eb, NULL);
  };
  auto evstop = evsignal_new(ebm, SIGINT, sig_cb, ebm);
  evsignal_add(evstop, NULL);







  // main loop
  event_base_dispatch(ebm);

  // exit
  std::cout << "done\n";
  evhttp_free(hs);
  event_base_free(ebm);
  return 0;
}
