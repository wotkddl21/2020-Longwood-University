#ifndef TUBE__H
#define TUBE__H
#include <unistd.h>
#include <cstdio>
#include <vector>
#include <poll.h>

using namespace std;


struct Tube{
  int outgoing[2];
  int incoming[2];

  FILE* to;
  FILE* from;

  Tube(){
    pipe(outgoing);
    pipe(incoming);
  }

  bool hasEvent(){
    struct pollfd pfd;
    pfd.fd = outgoing[0];
    pfd.events = POLLIN;
    return poll(&pfd,1,0);
  }

  void openStreams(){
    close(incoming[0]);
    close(outgoing[1]);
    to   = fdopen(incoming[1], "w");
    setlinebuf(to);
    from = fdopen(outgoing[0], "r");
  }

  void closeStreams(){
    fclose(to);
    fclose(from);
  }
 void makeStdOut(){
    dup2(outgoing[1], STDOUT_FILENO);
    close(outgoing[0]);
    close(outgoing[1]);
  }

  void makeStdIn(){
    dup2(incoming[0], STDIN_FILENO);
    close(incoming[0]);
    close(incoming[1]);
  }

  void makeStdIO(){
    makeStdIn();
    makeStdOut();
  }

};

bool waitOnEvents(vector<Tube> tubes){
  vector<struct pollfd> helper;
  for(Tube t: tubes){
    struct pollfd pfd;
    pfd.fd = t.outgoing[0];
    pfd.events = POLLIN;
    helper.push_back(pfd);
  }
  return poll(helper.data(),helper.size(),-1) > 0;
}


#endif