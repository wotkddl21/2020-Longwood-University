#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>
char** fl_list(const char* path){
	char** ret;
	DIR* dir=NULL;
	int num=0;
	int i=0;
	struct dirent* entry = NULL;
	dir = opendir(path);
	if(dir==NULL){
		printf("Invalid path\n");
		return NULL;
	}
	entry = readdir(dir);
	while(entry!=NULL){
		num++;
		entry = readdir(dir);
	}
	closedir(dir);

	ret = (char**)malloc(sizeof(char*)*(num+1));
	for(i=0;i<num+1;i++){
		ret[i] = (char*)malloc(sizeof(char)*256);
	}
	for(i=0;i<40;i++){
		ret[num][i]='*';
	}
	
	dir = opendir(path);
	for(i=0;i<num;i++){
		entry = readdir(dir);
		sprintf(ret[i],"%s/%s",path,entry->d_name);
//		strcpy(ret[i],entry->d_name);
	}
	closedir(dir);
	return ret;
}

char** fl_list_filter(const char*path,const char* prefix,const char*suffix){

	char** ret;
	DIR* dir=NULL;
	int num=0;
	int i=0;
	int j=0;
	int plength = strlen(prefix);//length of prefix
	int slength = strlen(suffix);//length of suffix
	int templength=0; // length of entry->d_name
	struct dirent* entry = NULL;
	dir = opendir(path);
	if(dir==NULL){
		printf("Invalid path\n");
		return NULL;
	}
	entry = readdir(dir);
	while(entry!=NULL){
		num++;
		entry = readdir(dir);
	}
	closedir(dir);
	ret = (char**)malloc(sizeof(char*)*(num+1));
	for(i=0;i<num+1;i++){
		ret[i] = (char*)malloc(sizeof(char)*256);
	}
	for(i=0;i<40;i++){
		ret[num][i]='*';
	}
	dir = opendir(path);
	int pflag=0;
	int sflag=0;
	int correct=0;
	for(i=0;i<num;i++){
		pflag=0;
		sflag=0;
		entry = readdir(dir);
		templength = strlen(entry->d_name);
		if(templength>=plength+slength){
			for(j=0;j<plength;j++){
				if(prefix[j] != entry->d_name[j]){
					pflag=1;
				}
			}
			int t=0;
			if(pflag==0){
				for(j=templength-slength;j<templength;j++){
					if(suffix[t]!=entry->d_name[j]){
						sflag=1;
					}
					t++;
				}
				if(sflag==0||suffix[0]=='*'){
					sprintf(ret[correct],"%s/%s",path,entry->d_name);
//					strcpy(ret[correct],entry->d_name);
					correct++;
				}
			}
		}
	}
	for(i=correct;i<num;i++){
		for(j=0;j<40;j++){
			ret[i][j]='*';
		}
	}

	return ret;
}


int fl_count(char** files){
	int num=0;
	int i=0;
	int j=0;
	int flag=0;
	while(1){
		flag=0;
		for(j=0;j<40;j++){
			if(files[num][j]=='*'){
				flag++;
			}
		}
		if(flag==40){
			break;
		}
		if(files[num][0]=='\0'){
			num--;
		}
		i++;
		num++;
	}
	return num;
}

	


int fl_isDir(const char* name){
	struct stat st;
	stat(name,&st);
	return S_ISDIR(st.st_mode);
}

int fl_isExe(const char* name){
	struct stat st;
	stat(name,&st);
	if((st.st_mode & S_IEXEC)!=0 && S_ISDIR(st.st_mode)!=1){
		return 1;
	}
	return 0;
}
int fl_isSym(const char* name){
	struct stat st;
	stat(name,&st);
	return S_ISLNK(st.st_mode);
}


unsigned int fl_size(const char* name){
	struct stat st;
	stat(name,&st);
	return st.st_size;
}
unsigned int fl_size_r(const char* name){
	int flag=0;
	struct stat st;
	DIR* dir=NULL;
	unsigned int ret=0;
	unsigned int check=0;
	struct dirent* entry=NULL;
	char temp[1000];

	stat(name,&st);
	flag = fl_isDir(name);
	if(flag){//dir
		dir = opendir(name);
		if(dir==NULL){
			printf("Invalid path\n");
		}
		entry = readdir(dir);
		while(entry!=NULL){
			if(strcmp(name,entry->d_name)!=0 && strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
				sprintf(temp,"%s/%s",name,entry->d_name);
				check = fl_size_r(temp);
				ret+=check;
			}
			entry = readdir(dir);
		}
		closedir(dir);
		return ret;
	}
	else{
		return st.st_size;
	}

}
unsigned int fl_date(const char* name){
	struct stat st;
	stat(name,&st);
	return st.st_mtime;
}

void fl_sort_bySize(char** files){
	unsigned int* size;
	struct stat st;
	int i,j;
	int temp=0;
	char tstr[256]="";
	int num=0;
	int flag=0;
	i=0;

	while(1){
		for(j=0;j<40;j++){
			if(files[i][j]=='*'){
				flag++;
			}
		}
		if(flag==40){
			break;
		}
		i++;
	}
	num=i;
	size = (unsigned int*)malloc(sizeof(unsigned int)*num);
	for(i=0;i<num;i++){
		stat(files[i],&st);
		if(fl_isDir(files[i])){
			size[i]=fl_size_r(files[i]);
		}
		else{
			size[i] = st.st_size;
		}
	}
	for(i=0;i<num-1;i++){
		for(j=0;j<num-i-1;j++){
			if(size[j]>=size[j+1]){ // swap
				temp = size[j];
				size[j]= size[j+1];
				size[j+1] = temp;
				strcpy(tstr,files[j]);
				strcpy(files[j],files[j+1]);
				strcpy(files[j+1],tstr);
			}
		}
	}
}

void fl_sort_byDate(char** files){
	unsigned int* time;
	struct stat st;
	int i,j;
	int temp=0;
	char tstr[256]="";
	int num=0;
	int flag=0;
	i=0;
	while(1){
		for(j=0;j<40;j++){
			if(files[i][j]=='*'){
				flag++;
			}
		}
		if(flag==40){
			break;
		}
		i++;
	}
	num=i;
	time = (unsigned int*)malloc(sizeof(unsigned int)*num);
	for(i=0;i<num;i++){
		stat(files[i],&st);
		time[i] = st.st_mtime;
	}
	for(i=0;i<num-1;i++){
		for(j=0;j<num-i-1;j++){
			if(time[j]>=time[j+1]){ // swap
				temp = time[j];
				time[j]= time[j+1];
				time[j+1] = temp;
				strcpy(tstr,files[j]);
				strcpy(files[j],files[j+1]);
				strcpy(files[j+1],tstr);
			}
		}
	}

}
void fl_delete(char** files){
	int i=0;
	int j=0;
	int flag=0;
	while(1){
		for(j=0;j<40;j++){
			if(files[i][j]=='*'){
				flag++;
			}
		}
		if(flag==40){
			break;
		}
		i++;
	}
	for(j=0;j<i;j++){
		free(files[j]);
	}
	free(files);
}

void fl_formatDate(const char* filename,char* str){
	struct stat st;
	struct tm* time;
	stat(filename,&st);
	time = localtime(&st.st_mtime);
	switch(time->tm_mon+1){
		case 1:
			str[0] = 'J';
			str[1] = 'a';
			str[2] = 'n';
			break;
		case 2:
			str[0] = 'F';
			str[1] = 'e';
			str[2] = 'b';
			break;
		case 3:
			str[0] = 'M';
			str[1] = 'a';
			str[2] = 'r';
			break;
		case 4:
			str[0] = 'A';
			str[1] = 'p';
			str[2] = 'r';
			break;
		case 5:
			str[0] = 'M';
			str[1] = 'a';
			str[2] = 'y';
			break;
		case 6:
			str[0] = 'J';
			str[1] = 'u';
			str[2] = 'n';
			break;
		case 7:
			str[0] = 'J';
			str[1] = 'u';
			str[2] = 'l';
			break;
		case 8:
			str[0] = 'A';
			str[1] = 'u';
			str[2] = 'g';
			break;
		case 9:
			str[0] = 'S';
			str[1] = 'e';
			str[2] = 'p';
			break;
		case 10:
			str[0] = 'O';
			str[1] = 'c';
			str[2] = 't';
			break;
		case 11:
			str[0] = 'N';
			str[1] = 'o';
			str[2] = 'v';
			break;
		case 12:
			str[0] = 'D';
			str[1] = 'e';
			str[2] = 'c';
			break;
	}
	//month
	
	str[3]=str[4]=' ';
	if(time->tm_mday >=30){
		str[5]='3';
	}
	else if(time->tm_mday >=20){
		str[5] = '2';
	}
	else if(time->tm_mday >=10){
		str[5] = '1';
	}
	else{
		str[5] ='0';
	}

	str[6] = '0'+time->tm_mday%10;
	str[7] = ':';
	//date
	
	if(time->tm_hour >=50){
		str[8] = '5';
	}
	else if(time->tm_hour >=40){
		str[8] = '4';
	}
	else if(time->tm_hour >=30){
		str[8] = '3';
	}
	else if(time->tm_hour >=20){
		str[8] = '2';
	}
	else if(time->tm_hour >=10){
		str[8] = '1';
	}
	else if(time->tm_hour >=0){
		str[8] = '0';
	}

	str[9] = '0'+time->tm_hour%10;
	str[10]='\0';

}

void fl_readableSize(unsigned int size, char* str){
	char last = 'B';
	float digit=0;
	int i=0;
	if(size >0 && size<1000){ // byte
		last = 'B';
		digit = (float)size;
	}
	else if(size >=1000 && size <1000*1000){//KB
		last = 'K';
		//digit = (float)size/1024;
		digit = (float)size/1000;
	}
	else if(size>=1000*1000 && size<1000*1000*1000){//MB
		last = 'M';
		//digit =(float)size/(1024*1024);
		digit =(float)size/(1000*1000);
	}
	else if(size>=1000*1000*1000){//GB
		last = 'G';
//		digit = (float)size/(1024*1024*1024);
		digit = (float)size/(1000*1000*1000);
	}
	int temp=0;
	int scale = 100;
	for(i=0;i<3;i++){
		temp = digit/scale;
		temp%=10;
		scale/=10;
		if(temp==0 && i==0){
			str[i] = ' ';
		}
		else if(temp==0 && str[0]==' '&& i!=2){
			str[i] = ' ';
		}
		else{
			str[i] = '0'+temp;
		}
	}
	str[3] = '.';
	if(digit-(int)digit==0){
		str[4] = '0';
	}
	else{
		str[4] = '0'+(digit - (int)digit)*10;
	}
	str[5] = last;
}


