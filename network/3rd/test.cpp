#include<iostream>
#include<thread>
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


void func(){
	cout<<5<<endl;
}
void fund(int me){
	cout<<me*2<<endl;
}

int main(){
	thread thd(func);
	thread the(fund,4);
	thd.join();
	the.join();
	cout<<"done"<<endl;

	thread diver(fetch);
	diver.join();
	cout<<payment<<endl;
	return 0;
}
