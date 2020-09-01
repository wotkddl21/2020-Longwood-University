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
  while(cin >> p.x and cin >> p.y){
    int n = (23*noice.getColor(p.x,p.y,0.1,1))+232;
    cout << p.x << " " << p.y << " " << n << endl;
  }
  exit(0);
}

int main(){
  vector<Tube> tubes;

  vector<Point> points;
  for(int i =0; i < 100; ++i){
    for(int k= 0; k < 10; ++k){
      points.push_back({i,k});
    }
  }

//  random_shuffle(points.begin(),points.end());

  for(int i = 0; i < 16; ++i){
    tubes.push_back(Tube());
    int pid = fork();
    if(pid != 0){
      tubes[tubes.size()-1].openStreams();
      Point p = points[points.size()-1];
      points.pop_back();
      fprintf(tubes[tubes.size()-1].to,"%d %d\n",p.x,p.y);
    }else{
      tubes[tubes.size()-1].makeStdIO();
      childProgram();
    }
  }

  while(tubes.size() > 0 and waitOnEvents(tubes)){
    for(int i = tubes.size()-1; i >= 0; --i){
      if(tubes[i].hasEvent()){
        int answer;
        Point q;
        fscanf(tubes[i].from,"%d %d %d",&q.x,&q.y,&answer);
        if(points.size() > 0){
          Point p = points[points.size()-1];
          points.pop_back();
          fprintf(tubes[i].to,"%d %d\n",p.x,p.y);
        }else{
          fputc(EOF,tubes[i].to);
          tubes[i].closeStreams();
          tubes.erase(tubes.begin()+i);
        }
        printf("%d,%d --> %d\n",q.x,q.y,answer);
      }
    }
  }
  for(int i =0; i < 16; ++i){
    int rv;
    wait(&rv);
  }

  return 0;
}
