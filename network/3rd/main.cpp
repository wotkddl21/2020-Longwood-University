#include<iostream>
#include<thread>
#include<vector>

using namespace std;

int treasure =1000;
int payment=0;

void fetch(){
	int amt=0;
	while(treasure>0){
		amt = min(treasure,13);
		treasure-=13;
		payment+=13;
	}
}



int main(){
	vector<thread*> divers;
	int i=0;
	for(i=0;i<10000;i++){
		divers.push_back(new thread(fetch));
	}
	for(thread* t:divers){
		t->join();
	}
	cout<<payment<<endl;
	return 0;
}
