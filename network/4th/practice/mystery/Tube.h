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

  Tube(){ // constructor
    pipe(outgoing);// data can be written to outgoing[1], read from outgoing[0]
    pipe(incoming);// data can be written to incoming[1], read from incoming[0]
  }

  bool hasEvent(){
    struct pollfd pfd;
    pfd.fd = outgoing[0];
    pfd.events = POLLIN;// data can be read from outgoing[0] now on
    return poll(&pfd,1,0);//nfds : 1  so the number of fd is 1. And timeout: 0... so it doesn't wait. it checks the condition of fd without any block.
  }// if there is any data that can be read from outgoing[0], then return true.

  void openStreams(){
    close(incoming[0]);//data can't be read from incoming[0] now on
    close(outgoing[1]);//data can't be written to outgoing[1] now on
    to   = fdopen(incoming[1], "w");//to write data to incoming[1], open fd as to.
    setlinebuf(to); // set the buffer of to as linebuffer
    from = fdopen(outgoing[0], "r");//to read data from outgoing[0], open fd as from.
  }
  //so read data from outgoing[1] and write data to incoming[0] with linebuffer

  void closeStreams(){
    fclose(to);
    fclose(from);
  }//close fd
 void makeStdOut(){
    dup2(outgoing[1], STDOUT_FILENO);// from now on, the data written to stdout will be written to outgoing[1],which is kind of "redirection"
    close(outgoing[0]);//since we make stdout point outging[1], if we want to read data, we are supposed to write the data to stdin
    close(outgoing[1]);//we let the stdout point outgoing[1], we don't need the old pointer.
  }//set stdout point to outgoing[1]

  void makeStdIn(){
    dup2(incoming[0], STDIN_FILENO);//from now on, the data supposed to be read from stdin will be read from incoming[0]
    close(incoming[0]);//because stdin points incoming[0] , we don't need the old pointer
    close(incoming[1]);// the data was supposed to be written to incoming[1] and read from incoming[0], but we made stdin point incoming[0], we can read data from stdin.
					   // So if we want to read data from incoming[0], then we have to write data to stdout,which means we don't need incoming[1] anymore.
  }//set stdin point to incoming[0]

  void makeStdIO(){// from now on, we can write data ,which is supposed to be writen to stdout, to outgoing[1] and read data ,which is supposed to be read from stdin, from incoming[0]
    makeStdIn();
    makeStdOut();
  }

};

bool waitOnEvents(vector<Tube> tubes){
  vector<struct pollfd> helper;
  for(Tube t: tubes){//for every Tube in tubes
    struct pollfd pfd;
    pfd.fd = t.outgoing[0];//get outgoing[0] into fd
    pfd.events = POLLIN;//data can be read from outgoing[0]
    helper.push_back(pfd);//store pfd into helper
  }
  return poll(helper.data(),helper.size(),-1) > 0; // while waiting there is any event, if there is any data that can be read from any outgoing[0] of tube in helper, return true. this is because the poll() returns the number of possible pfd.
}


#endif
