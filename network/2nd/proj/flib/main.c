#include"flib.h"
#include<dirent.h>
#include<unistd.h>
#include<string.h>
void printType(const char* filename,char* ret){
	int flag=0;
	flag = fl_isDir(filename);
	if(flag){
		strcpy(ret,"[dir]");
	}
	else{
		flag= fl_isExe(filename);
		if(flag){
			strcpy(ret,"[exe]");

		}
	}
}

void printname(const char* filename){
	char temp[1000]="";
	int i=0;
	int num=0;
	strcpy(temp,filename);
	for(i=0;i<strlen(filename);i++){
		if(filename[i]=='/'){
			i++;
			num = i;
			break;
		}
	}
	for(i=0;i<strlen(filename)-num;i++){
		temp[i] = temp[i+num];
	}
	temp[strlen(filename)-num]='\0';
	printf("%s\n",temp);


}
int main(int argc,char* argv[]){
	extern char*optarg;
	int m=0;
	char n;
	char** temp;
	char path[100];
	char readable[100];
	char date[100];
	char type[6]="";
	char prefix[100];
	char suffix[100];
	int num=0;
	m = getopt(argc,argv,"sdf");
	if(m!='s' && m!='d'&& m!='f'){
		printf("Option : \n-s sort by size\n-d sort by date\n-f prefix suffix list_filter with prefix and suffix\n");
		printf("Input option (s or d or anything for default) :");
		scanf("%c",&n);
		getchar();
	}
	if(n=='s'||m=='s'){
		printf("sort by size\n");
	}
	else if(n=='d'||m=='d'){
		printf("sort by date\n");
	}
	else if(n=='f'||m=='f'){
		printf("Input prefix:");
		scanf("%s",prefix);
		printf("Input suffix (it could be *):");
		scanf("%s",suffix);
	}
	else{
		printf("Default option\n");
	}
	printf("Input path:");
	scanf("%s",path);
	if(n=='f'||m=='f'){
		temp = fl_list_filter(path,prefix,suffix);
	}
	else{
		temp = fl_list(path);
	}
	if(temp==NULL){
		return 0;
	}
	num = fl_count(temp);
	if(n=='s'||m=='s'){
		fl_sort_bySize(temp);
	}
	else if(n=='d'||m=='d'){
		fl_sort_byDate(temp);
	}
	for(int i=0;i<num;i++){
		if(fl_isDir(temp[i])){
			fl_readableSize(fl_size_r(temp[i]),readable);
		}
		else{
			fl_readableSize(fl_size(temp[i]),readable);
		}
		fl_formatDate(temp[i],date);
		strcpy(type,"");
		printType(temp[i],type);
		printf("%s\t",readable);
		printf("%s\t",date);
		printf("%s\t",type);
		printname(temp[i]);
		//printf("%s\n",temp[i]);
	}
	printf("\n%d entries\n",num);
	fl_delete(temp);
	return 0;
}


