#include<iostream>
#include<sstream>
extern "C"{
	#include<curl/curl.h>
}
using namespace std;
size_t write( void* ptr, size_t size, size_t memb, string *stream){
	size_t s_size = size*memb;
	stream->assign((char*)ptr,(char*)ptr+s_size);
	return s_size;



}
string* s;
int main(int argc,char*argv[]){
	if(argc!=2){
		cout<<"./a.out <url>\n";
		return 0;
	}
	CURL *curl;
	CURLcode cc;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl,CURLOPT_URL,argv[1]);
	curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,0);
	curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,0);
	
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,s);
//	curl_easy_setopt(curl,CURLOPT_WRITEHEADER,stderr);
//	curl_easy_setopt(curl,CURLOPT_WRITEDATA,stdout);
	cc=curl_easy_perform(curl);

	if(CURLE_OK!=cc){
		cerr<<"cURL error: "<<curl_easy_strerror(cc)<<endl;
	}
	else{
		double statd;
		long statl;
		char* statstring=NULL;
		if(CURLE_OK==curl_easy_getinfo(curl,CURLINFO_HTTP_CODE,&statl)){
			cout<<"Response code: "<<statl<<endl;
		}
		if(CURLE_OK==curl_easy_getinfo(curl,CURLINFO_CONTENT_TYPE,&statstring)){
			cout<<"Content type: "<<statstring<<endl;
		}
		if(CURLE_OK==curl_easy_getinfo(curl,CURLINFO_SIZE_DOWNLOAD,&statd)){
			cout<<"Download size: "<<statd<<endl;
		}
	}
	stringstream ss;
	ss.str(*s);
	int depth;
	while(ss>>depth){
		cout<<"depth: "<<depth<<endl;
	}
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return 0;
}
