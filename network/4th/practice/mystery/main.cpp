#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <sys/wait.h>
#include <cstdlib>
#include "Tube.h"
#include <vector>
#include <algorithm>
#include "Noise.h"

using namespace std;

struct Point{
  int x;
  int y;
};

void childProgram(){
  Point p = {0,0};
  Noise noice;
  noice.seed = 12345;
  while(cin >> p.x and cin >> p.y){//while there are input p.x and p.y
    int n = (23*noice.getColor(p.x,p.y,0.1,1))+232;//get color with the seed and point p.
    cout << p.x << " " << p.y << " " << n << endl;//because the "makeStdIo()" has been run before this function, it also write the data to  outgoing[0] though it writes data to cout.
  }
  exit(0);//terminate this process.
}

int main(){
  vector<Tube> tubes;

  vector<Point> points;
  for(int i =0; i < 100; ++i){
    for(int k= 0; k < 10; ++k){
      points.push_back({i,k});
    }
  }//get point from (0,0)~(99,9)

  random_shuffle(points.begin(),points.end());//shuffle the point randomly

  for(int i = 0; i < 16; ++i){
    tubes.push_back(Tube());//construct new Tube and then store it into vector "tubes".
    int pid = fork();//make child process
    if(pid != 0){//if this process is parent
      tubes[tubes.size()-1].openStreams();// make FILE* to in the last element of vector tubes be incoming[1] with write
	  									  // make FILE* from in the last element of vector tubes be outgoing[0] with read
	  									  // with linebuffer.
      Point p = points[points.size()-1];//get (x,y) from last element of vector points.
      points.pop_back();//then remove the last element. those 2 lines act like pop in stack.
      fprintf(tubes[tubes.size()-1].to,"%d %d\n",p.x,p.y);//write the p.x and p.y to the stream which we made above.
    }else{// if it is child process
      tubes[tubes.size()-1].makeStdIO();//set the outgoing[1] as stdout, incoming[0] as stdin
      childProgram();//run the childProgram() which write p.x,p.y and the color. %d %d %d
    }
  }

  while(tubes.size() > 0 and waitOnEvents(tubes)){//if there is at least one tube and there is any data that can be read from tubes' outgoing[0]
    for(int i = tubes.size()-1; i >= 0; --i){
      if(tubes[i].hasEvent()){//from the last element in tubes,
        int answer;
        Point q;
        fscanf(tubes[i].from,"%d %d %d",&q.x,&q.y,&answer);//read the data from "from",which is stdin
        if(points.size() > 0){
          Point p = points[points.size()-1];
          points.pop_back();
          fprintf(tubes[i].to,"%d %d\n",p.x,p.y);//if there is ant point, write the p.x and p.y to "to" whichi is stdout
        }else{
          fputc(EOF,tubes[i].to);//if there is no point, close the "to" socket.
          tubes[i].closeStreams();
          tubes.erase(tubes.begin()+i);
        }
        printf("%d,%d --> %d\n",q.x,q.y,answer);//write the q.x q.y answer which are from stdin to stdout.
      }
    }
  }
  for(int i =0; i < 16; ++i){
    int rv;
    wait(&rv);//since we fork 16 times, we have to set 16 wait points.
  }

  return 0;
}
