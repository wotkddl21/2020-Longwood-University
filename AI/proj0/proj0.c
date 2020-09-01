#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct _Tree{
	struct _Tree* child[10000];
	struct _Tree* parent;
	int childnum;
	int generation;
	char list[100];
}Tree;

Tree* treehead;
char start[101];
char end[101];
char filename[101];
int strnum=0;
int diff=0;


Tree* queue[100000000];
int queuenum=0;

char check[100];

char close[1000000][100];
int closenum=0;

char total_list[1000000][100];
int total_num=0;


int init(void){
	int i=0;
	printf("Input start word ( 100 character limits ):");
	scanf("%s",start);
	strnum = strlen(start);
	for(i=0;i<strnum;i++){
		if(start[i]>='A'&& start[i]<='Z'){
			start[i] = start[i]-'A'+'a';
		}
	}
	printf("Input end word ( 100 character limits ):");
	scanf("%s",end);
	printf("Input file name ( 100 character limits ):");
	scanf("%s",filename);
	if(strnum!=strlen(end)){
		printf("Length of start and end are different\n");
		return -1;
	}
	if(strcmp(end,start)==0){
		printf("start : %s\nend : %s\n",start,end);
		return -1;
	}
	return 0;
}
int main(void){
	int initiate=0;
	FILE* fp;
	FILE* fp2;
	char cur[100];
	int i=0;
	int capital=0;
	Tree* initial;
	Tree* curTree;
	Tree* compare;
	char route[1000][100];
	int routenum=0;
	int impossible=0;


	initial = (Tree*)malloc(sizeof(Tree));
	treehead = (Tree*)malloc(sizeof(Tree));
	treehead->child[0] = initial;
	treehead->childnum = -1;
	initial->parent = treehead;
	//
	initial->generation=1;
	//
	fp2 = fopen("result.txt","w");
	initiate = init();
	if(initiate==-1){
		return 0;
	}
	fp = fopen(filename,"r");
	if(fp==NULL){
		printf("%s is an invalid filename\n",filename);
		return 0;
	}
	while(EOF!=fscanf(fp,"%s",check)){
		if(strlen(check)==strnum){
			capital=0;
			for(i=0;i<strnum;i++){
				if(check[i]>='A'&& check[i]<='Z'){
					check[i]=check[i]-'A'+'a';
				}
			}
			if(capital==0){
				strcpy(total_list[total_num],check);
				total_num++;
			}
		}
		else if(strlen(check)==strnum+1){
			if(check[strnum]=='.'){
				check[strnum] = '\0';
				strcpy(total_list[total_num],check);
				total_num++;
			}
		}
	}
	for(i=0;i<total_num;i++){
		if(strcmp(start,total_list[i])==0){
			impossible =1;
		}
	}
	if(impossible ==0){
		printf("start word is not in the dictionary\n");
		return 0;
	}
	impossible =0;
	for(i=0;i<total_num;i++){
		if(strcmp(end,total_list[i])==0){
			impossible =1;
		}
	}
	if(impossible ==0){
		printf("end word is not in the dictionary\n");
		return 0;
	}

	strcpy(initial->list,start);
	strcpy(close[closenum],start);
	closenum++;
	queue[queuenum]=initial;
	queuenum++;
	int j=0;
	int flag=0;
	int flag2=0;
	int success=0;
	while(queuenum!=0){
		if(success==1){
			break;
		}
		curTree = queue[0];
		for(i=0;i<=queuenum-1;i++){
			queue[i]=queue[i+1];
		}
		queuenum--;
		//dequeue
		compare = curTree;
		routenum=0;
		while(1){
			if(compare->childnum==-1){
				break;
			}
			else{
				strcpy(route[routenum],compare->list);
				routenum++;
				compare = compare->parent;
			}
		}
		// find child for curTree
		for(j=0;j<total_num;j++){
			flag=0;
			for(int k=0;k<strnum;k++){
				if(total_list[j][k]==curTree->list[k]){
					flag++;
				}
			}
			if(flag==strnum-1){
				flag2=0;
				for(int k=0;k<routenum;k++){
					if(strcmp(route[k],total_list[j])==0){
						flag2=1;
					}
				} //except the word in their parent
				if(flag2==0){// insert
					Tree* newnode;
					newnode = (Tree*)malloc(sizeof(Tree));
					strcpy(newnode->list,total_list[j]);
					newnode->generation = curTree->generation+1;
					curTree->child[curTree->childnum] = newnode;
					curTree->childnum++;
					newnode->parent = curTree;
					queue[queuenum]=newnode;
					queuenum++;
					strcpy(route[routenum],total_list[j]);
					closenum++;
					if(strcmp(total_list[j],end)==0){
						success=1;
						break;
					}
				}
			}
			if(queuenum>=10000){
				if(queuenum%50000==0){
					printf("wait... it is finding from %d number of queue\n",queuenum);
				}
				if(queuenum%100000==0){
					printf("current generation : %d\n",curTree->generation);
				}
			}
			if(queuenum>=99999999){
				printf("No ladder... too much queuenum....\n");
				break;
			}
		}
	}
	fclose(fp2);
	fclose(fp);
	char result[100][10];
	int resultnum=0;
	if(queuenum==0){
		printf("No ladder exists\n");
		return 0;
	}
	while(1){
		if(curTree->childnum==-1){
			break;
		}
		strcpy(result[resultnum],curTree->list);
		resultnum++;
		curTree= curTree->parent;
	}
	//curTree : treehead
	printf("start : %s\n",start);
	for(i=resultnum-2;i>=0;i--){
		printf("%s\n",result[i]);
	}
	printf("end : %s\n",end);
	return 0;
}
