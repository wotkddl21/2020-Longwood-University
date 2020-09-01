#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _word{
	char* word;
	int num;
}wordlist;

int check_word(char* input){//return 0 if not word, and 1 if word.
	int i=0;
	int len = strlen(input);
	int wordnum=0;
	//punctuation : except a~z, A~Z, -, _, ', \",
	//start with ' or "
	if(input[0]=='\''){
		for(i=1;i<len;i++){
			input[i-1] = input[i];
		}
		input[len-1]='\0';
		len = strlen(input);
		if(input[len-1]=='\''&& len>=3){
			input[len-1]='\0';
			return 1;
		}
		else if(input[len-1]=='\'' && len<3){
			return 0;
		}
	}
	else if(input[0]=='\"'){
		if(input[len-1]=='\"'&& len>=3){
			for(i=1;i<len;i++){
				input[i-1] = input[i];
			}
			input[len-2]='\0';
			return 1;
		}
		else if(input[len-1]=='\"' && len<3){
			return 0;
		}

	}
	for(i=0;i<len;i++){
		if(input[i]>='A'&&input[i]<='Z'){
			input[i]+= 'a'-'A';
			wordnum++;
		}
		else if(input[i]>='a'&&input[i]<='z'){
			wordnum++;
		}
	}
	//lowercase
	
	if(input[len-1]=='\''){ //commodities'
		input[len-1]='\0';
		return 1;
	}
	else if(input[len-1]=='\"'){//commodities"
		input[len-1]='\0';
		return 1;
	}
	if(wordnum==0){
		return 0;
	}
	else{
		return 1;
	}
	
}


int main(int argc,char* argv[]){
	char filename[100];
	wordlist* word_list=NULL;
	int wordnum=0;
	int longest=0;
	char temp[1000];
	FILE* fp=NULL;
	int i=0;
	if(argc!=2){
		printf("./proj3 <filename>\n");
		return 0;
	}
	strcpy(filename,argv[1]);
	fp = fopen(filename,"r");
	if(fp==NULL){
		printf("File open error!\n");
		return 0;
	}
	while(fscanf(fp,"%s",temp)!=EOF){
		if(longest<=strlen(temp)){
				longest = strlen(temp);
		}
		wordnum++;
	}//get number of words and the longest length of words
	fseek(fp,0L,SEEK_SET);
	printf("wordnum : %d\n",wordnum);
	word_list = (wordlist*)malloc(sizeof(wordlist)*wordnum);
	for(i=0;i<wordnum;i++){
		word_list[i].word = (char*)malloc(sizeof(char)*longest);
	}
	int cur=0;
	int j=0;
	int k=0;
	int same=0;
	for(i=0;i<wordnum;i++){
		fscanf(fp,"%s",temp);
		same=0;
		if(strlen(temp)>0){
			if(check_word(temp)){
				for(j=0;j<cur;j++){
		
					if(strcmp(temp,word_list[j].word)==0){
						word_list[j].num++;
						same=1;
						break;
					}//if this word is in the word_list, then increase the num
				}
				if(same==0){
					strcpy(word_list[cur].word,temp);
					word_list[cur].num++;
					cur++;
				}//else add this word to the word_list
			}
		}
	}
	fclose(fp);
	printf("The word list\n");
	for(i=0;i<cur;i++){
		printf("%.2f%\t",(float)word_list[i].num/(float)wordnum*100.0);
		printf("%s\n",word_list[i].word);
	}

	for(i=0;i<wordnum;i++){
		free(word_list[i].word);
	}
	free(word_list);
				

}
