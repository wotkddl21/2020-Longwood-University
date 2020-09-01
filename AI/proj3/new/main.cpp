#include <iostream>
#include<cstring>
#include<stdlib.h>
#include <vector>
#include<sstream>
#include<math.h>
using namespace std;
enum{z=0,x=1,c=2,v=3,b=4,n=5,m=6,a=7,s=8,d=9,f=10,g=11,h=12,j=13,k=14,l=15,q=16,w=17,e=18,r=19,t=20,y=21,u=22,i=23,o=24,p=25};

typedef struct _point{
  double x;
  double y;
}Point;
Point point[26];

typedef struct _candi{
  string word;
  double score;
}Candidate;

typedef struct _word{
	char word[300];
	double num;
}unigram;

typedef struct _biword{
  char previous[300];
  char current[300];
  double num;
}bigram;

void sort_candi(vector<Candidate>& input){
  int i=0;
  int j=0;
  Candidate temp;
  for(i=input.size()-1;i>0;i--){
    for(j=0;j<i;j++){
      temp = input.at(j);
      if(temp.score<=input.at(j+1).score){
        input.at(j) = input.at(j+1);
        input.at(j+1) = temp;
      }
    }
  }
}


int converter(char input){
  int ret=0;
  switch(input){
    case 'z': ret = 0; break; case 'x': ret = 1; break; case 'c': ret = 2; break; case 'v': ret = 3; break; case 'b': ret = 4; break;case 'n': ret = 5; break; case 'm': ret = 6; break;
    case 'a': ret = 7; break; case 's': ret = 8; break; case 'd': ret = 9; break; case 'f': ret = 10; break; case 'g': ret = 11; break;
    case 'h': ret = 12; break; case 'j': ret = 13; break; case 'k': ret = 14; break; case 'l': ret = 15; break;
    case 'q': ret = 16; break; case 'w': ret = 17; break; case 'e': ret = 18; break; case 'r': ret = 19; break;
    case 't': ret = 20; break; case 'y': ret = 21; break; case 'u': ret = 22; break; case 'i': ret = 23; break;
    case 'o': ret = 24; break; case 'p': ret = 25; break;
  }
  return ret;
}


double get_distance(int a,int b){
  double x;
  double y;
  if(a!=b){
    x = fabs(point[a].x-point[b].x)+1;
    y = fabs(point[a].y-point[b].y)+1;
    return 1/sqrt((x*x+y*y));
  }
  else{
    return 0;
  }
}
vector<unigram> unigram_list; vector<bigram> bigram_list;



vector<Candidate> find_sub(string input){
  int i=0; int length = input.length();
  int diff=0; double score[2]={0}; int diff_point[2];
  vector<Candidate> ret;
  vector<string> candidate;
  vector <double> point;
  for(i=0;i<unigram_list.size();i++){
    if(strlen(unigram_list.at(i).word)== length){
      string s(unigram_list.at(i).word);
      double d = unigram_list.at(i).num;
      candidate.push_back(s);
      point.push_back(d);
    }
  }
  for(i=0;i<candidate.size();i++){
    diff=0;
    double prob = point.at(i);
    for(int j=0;j<length;j++){
      if(input[j]!=candidate[i][j]){
        if(diff<2){
        score[diff]=get_distance(converter((char)(input[j])),converter((char)candidate[i][j]));
        }
        diff++;
      }
    }
    if(diff==0){
      Candidate c;
      c.word.assign(candidate[i]);
      c.score = prob;
      ret.push_back(c);
    }
    else if(diff<=2){
      Candidate c;
      c.word.assign(candidate[i]);
      c.score = (score[0]+score[1])*prob;
      ret.push_back(c);
    }
  }
  return ret;
}

vector<Candidate> find_insert(string input){// input.length()+1 = candidate.length
  int i=0; int length = input.length()+1; int t=0; int j=0; int k=0;
  int diff=0; double score; double tscore;
  char temp[1000];
  vector<Candidate> ret;
  vector<string> candidate;
  vector<double> point;
  for(i=0;i<unigram_list.size();i++){
    if(strlen(unigram_list.at(i).word)== length){
      string s(unigram_list.at(i).word);
      double d = unigram_list.at(i).num;
      point.push_back(d);
      candidate.push_back(s);
    }
  }
  i=0;
  for(string s:candidate){
    double prob = point.at(i);
    i++;
    for(t=0;t<length;t++){
      k=0; diff=0;
      for(j=0;j<t;j++){
        temp[k] = s[j];
        k++;
      }
      for(j=t+1;j<length;j++){
        temp[k] = s[j];
        k++;
      }
      temp[k]='\0';
      for(j=0;j<length-1;j++){
        if(temp[j]!=input.at(j)){
          diff++;
          break;
        }
      }
      if(diff==0){
        Candidate c;
        c.word.assign(s);
        if(t==0){
          score = get_distance(converter((char)s[t]),converter((char)s[t+1]));
        }
        else if(t==length-1){
          score = get_distance(converter((char)s[t]),converter((char)s[t-1]));  
        }
        else{
          score = get_distance(converter((char)s[t]),converter((char)s[t+1]));
          tscore = get_distance(converter((char)s[t]),converter((char)s[t-1]));
        }
        c.score = score* prob;
        ret.push_back(c);
        break;
      }
    }
  }
  return ret;
}

vector<Candidate> find_del(string input){//input.length()-1 = candidate.length
  int i=0; int length = input.length()-1; int t=0; int k=0; int j=0;
  char temp[1000]; int diff=0; double score=0; double tscore=0;
  vector<Candidate> ret;
  vector<string> candidate;
  vector <double>point;
  for(i=0;i<unigram_list.size();i++){
    if(strlen(unigram_list.at(i).word)== length){
      string s(unigram_list.at(i).word);
      double d = unigram_list.at(i).num;
      point.push_back(d);
      candidate.push_back(s);
    }
  }
  i=0;
  for(string s:candidate){
    double prob = point.at(i);
    i++;
    for(t=0;t<length;t++){
      k=0; diff=0;
      for(j=0;j<t;j++){
        temp[k] = input[j];
        k++;
      }
      for(j=t+1;j<length;j++){
        temp[k] = input[j];
        k++;
      }
      temp[k]='\0';
      for(j=0;j<length-1;j++){
        if(temp[j]!=s.at(j)){
          diff++;
          break;
        }
      }
      if(diff==0){
        Candidate c;
        c.word.assign(s);
        if(t==0){
          score = get_distance(converter((char)input[t]),converter((char)input[t+1]));
        }
        else if(t==length-1){
          score = get_distance(converter((char)input[t]),converter((char)input[t+1]));
        }
        else{
          score = get_distance(converter((char)input[t]),converter((char)input[t+1]));
          tscore = get_distance(converter((char)input[t]),converter((char)input[t-1]));
          if(tscore>score){
            score = tscore;
          }
        }
        c.score = prob * score;
        ret.push_back(c);
        break;
      }
    }  
  }
  return ret;
}

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
  else if(input[len-1]=='.' || input[len-1]==','){//you.
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

vector<string> str_token(const string& data,char delimiter=' '){
  vector<string> ret;
  int i=0;
  string token;
  stringstream ss(data);
  while(getline(ss,token,delimiter)){
    for(i=0;i<token.length();i++){
      for(int j=0;j<token.length();j++){
       if(token.at(i)>='A'&&token.at(i)<='Z'){
         token.at(i)+='a'-'A';
       }
      }
    }
    ret.push_back(token);
  }
  return ret;
}



int main(int argc,char* argv[]){
	char filename[100];
  char temp[1000]; char temp2[1000];
  string stemp; string stemp2;
  //string input;
	double wordnum=0; double bigram_num=0;
	FILE* fp=NULL;
	double i=0; bool flag=true; int j=0;
	if(argc!=2){
		cout<<"./proj3 <filename>\n";
		return 0;
	}
  for(j=0;j<7;j++){
    point[j].x=i;
    point[j].y=0;
  }
  for(j=7;j<16;j++){
    point[j].x = i-7;
    point[j].y = 1;
  }
  for(j=16;j<26;j++){
    point[j].x = i-16;
    point[j].y = 2;
  }

	strcpy(filename,argv[1]);
	fp = fopen(filename,"r");
	if(fp==NULL){
		cout<<"File open error!\n";
		return 0;
	}
  fseek(fp,2L,SEEK_SET);
  while(1){
    if(fscanf(fp,"%s",temp)==EOF){
      break;
    }
   
    if(check_word(temp)){
      wordnum++;
      flag=true;
      for(i=0;i<unigram_list.size();i++){
        if(strcmp(temp,unigram_list.at(i).word)==0){//same word exists.
      
          flag=false;
          unigram_list.at(i).num++;
         
          break;
        }
      }
      
      if(flag){
        
        unigram new_word;
        //new_word = (unigram*)malloc(sizeof(unigram));
        //new_word->word = (char*)malloc(sizeof(char)*strlen(temp));
      
        strcpy(new_word.word,temp);
      
        new_word.num=1;
        unigram_list.push_back(new_word);
    
      }
    }
  }
  fseek(fp,0L,SEEK_SET);
  bool biflag = false;
  while(1){
 
    flag = true;
    if(biflag){ // read one additional word
      strcpy(temp,temp2);
      if(fscanf(fp,"%s",temp2)==EOF){
        break;
      }
    }
    else{// read two word
      if(fscanf(fp,"%s",temp)==EOF || fscanf(fp,"%s",temp2)==EOF){
        break;
      }
    }
    if(check_word(temp)!=0 && check_word(temp2)!=0){
      bigram_num++;
      biflag = true;
      for(i=0;i<bigram_list.size();i++){
        if(strcmp(temp,bigram_list.at(i).previous)==0 && strcmp(temp2,bigram_list.at(i).current)==0){
        bigram_list.at(i).num++;
        flag = false;
        break;
        }
      }
      if(flag){
        bigram new_bigram;
        //new_bigram = (bigram*)malloc(sizeof(bigram));
        //new_bigram->previous = (char*)malloc(sizeof(char)*strlen(temp));
        //new_bigram->current = (char*)malloc(sizeof(char)*strlen(temp2));
        strcpy(new_bigram.previous,temp);
        strcpy(new_bigram.current,temp2);
        new_bigram.num=1;
        bigram_list.push_back(new_bigram);
      }
    }
    else{
      biflag=false;
    } 
  }
  for(i=0;i<unigram_list.size();i++){
    //cout<<word_list.at(i)->num/wordnum<<"\t\t\t"<<word_list.at(i)->word<<endl;
    unigram_list.at(i).num /= wordnum;
  }
  for(i=0;i<bigram_list.size();i++){
    //cout<<bigram_list.at(i)->previous<<endl;
    bigram_list.at(i).num /= bigram_num;
  }
  //get unigram probability and bigram probability
  i=0;
  //start user input
  vector<vector<Candidate>> candidate;
  vector<vector<string>> complete;
  vector<vector<string>> input_list;
  int num=0;
  for( ; ;){
    cout<<"Insert your sentence: (insert q or Q to terminate)";
    string input;
    vector<string> token;
    getline(cin,input,'\n');
    if(input.length()==0){
      getline(cin,input,'\n');
    }
    
    
    vector<string> comp;
    if((input[0]=='q' && input.length()==1)||(input[0]=='Q' && input.length()==1)){
      //input_list.push_back(token);
      break;
    }
    cout<<"input length : "<<input.length()<<endl;
    token = str_token(input,' ');
    input_list.push_back(token);
    
    for(string s : token){
      char* c = new char[s.length()+1];
      strcpy(c,s.c_str());
      if(check_word(c)!=0){
         //candidate.push_back(temp);
        cout<<s<<"\tscore : ";
        j=0;
        vector<Candidate> cand0 ;
        cand0 = find_sub(s);
        vector<Candidate> cand1 ;
        if(s.length()>1){ 
          cand1= find_insert(s);
        }
        vector<Candidate> cand_1;
        if(s.length()>1){
          cand_1 = find_del(s);
        }
        for( Candidate c: cand1){
          cand0.push_back(c);
        }
        for( Candidate c: cand_1){
          cand0.push_back(c);
        }      
        candidate.push_back(cand0);
        sort_candi(candidate.at(i));
        printf("\t%0.8lf\n",candidate.at(i).at(0).score);
        
        for(Candidate C : candidate.at(i)){
          cout<<j<<"\t";
          printf("%0.8lf",C.score);
          cout<<"\t"<<C.word<<endl;
          j++;
        }
        if(j>0){
          cout<<"Choose between 0 ~ "<<j-1<<" (if you don't want to change, insert -1) :"<<endl;
          cin>>num;
          while(1){
            if(num<-1 || num>=j){
              cout<<"Choose between 0 ~ "<<j-1<<" (if you don't want to change, insert -1) "<<endl;
              cin>>num;
            }else{
              break;
            }
          }

          if(num==-1){
            cout<<"You choose "<<s<<endl;
            comp.push_back(s);
          }
          else{
            string choose(candidate.at(i).at(num).word);
            cout<<"You choose "<<choose<<endl;
            comp.push_back(choose);
          }
          
        }
        i++;
      }
      else{

      }
    }
    cout<<"corrected sentence :";
    for(string s: comp){
      cout<<s<<" ";
    }
    cout<<endl;
    complete.push_back(comp);
  }
  vector<unigram> uni_input; int uni_num;
  vector<unigram> inter_input; int inter_num;
  vector<bigram> bi_input;int bi_num;
  for(vector<string> t:input_list){
    for(string s: t){
      strcpy(temp,s.c_str());
      if(check_word(temp)){
        uni_num++;
        flag=true;
        for(i=0;i<uni_input.size();i++){
          if(strcmp(temp,uni_input.at(i).word)==0){//same word exists.
            flag=false;
            uni_input.at(i).num++;
            break;
          }
        }
      
        if(flag){
          unigram new_word;
          //new_word = (unigram*)malloc(sizeof(unigram));
          //new_word->word = (char*)malloc(sizeof(char)*strlen(temp));
          strcpy(new_word.word,temp);
          new_word.num=1;
          uni_input.push_back(new_word);
        }
      }
    }
  }

  for(vector<string> t:input_list){
    for(i=0;i<t.size()-1;i++){
      string s1;
      s1 = t.at(i);
      string s2;
      s2 = t.at(i+1);
      strcpy(temp,s1.c_str());  // temp for previous
      strcpy(temp2,s2.c_str()); // temp2 for current
      if(check_word(temp)!=0 && check_word(temp2)!=0){
        bi_num++;
        biflag = true;
        for(double k=0;k<bi_input.size();k++){
          if(strcmp(temp,bi_input.at(k).previous)==0 && strcmp(temp2,bi_input.at(k).current)==0){
          bi_input.at(k).num++;
          flag = false;
          break;
          }
        }
        if(flag){
          bigram new_bigram;
          //new_bigram = (bigram*)malloc(sizeof(bigram));
          //new_bigram->previous = (char*)malloc(sizeof(char)*strlen(temp));
          //new_bigram->current = (char*)malloc(sizeof(char)*strlen(temp2));
          strcpy(new_bigram.previous,temp);
          strcpy(new_bigram.current,temp2);
          new_bigram.num=1;
          bi_input.push_back(new_bigram);
        }
      }
    }
  }
  cout<<"Unigram of each word of input"<<endl;
  for(unigram u: uni_input){
    cout<<u.num/uni_num<<"\t"<<u.word<<endl;
  }
  cout<<endl;
  cout<<"Bigram of each word pair of input"<<endl;
  for(bigram b: bi_input){
    cout<<b.num/bi_num<<"\t"<<b.previous<<"\t"<<b.current<<endl;
  }
  cout<<endl;

  return 0;
}