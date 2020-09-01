how to run??
gcc proj0.c
then execute ./a.out

ex)
gcc proj0.c
./a.out
Input start word ( 100 character limits ): go  // insert the start word for word ladder
Input end word ( 100 character limits ): on // insert the end word for word ladder
Input file name( 100 character limits ):words.txt // insert the filename of word dictionary There is a local dictionary "words.txt"
//If you don't have no dictionary, then just insert words.txt

All of letter in start word and end word are modified as lower letter

ex) if start word : STart  then it is modified as start

If one of start word and end word is not in dictionary, then the program ends.

if there is a word ladder, then the path would be showed up.
otherwise, "No ladder" will outcome.

The words used in this program are extracted from the input dictionary file.
Every upper letter becomes lower letter and '.' becomes blank.
ex) Cat >> cat ,  dog. >> dog

/////////////////// The above program feature is constructed in code from line 33 to 55 ( function  "initiate") and from 90 to 130


I used the breadth first search with my struct.
I made a Tree-structure with doublely linked list.
my struct :   
typedef struct _Tree{
	struct _Tree* child[10000];
	struct _Tree* parent;
	int childnum;
	int generation;
	char list[100];
}Tree;

Every parent can have at most 10000 child. Parent and its children are doublely linked.
Each Tree node has generation which means its level of Tree.
Each Tree node has list which stores the word for word ladder.

The first Tree node has start word and its generation is 1.
The headTreenode's childnum = -1 // to check it is head or not.
The pseudo code is below.


//find end word by breadth first searching
////////////////////////start of rough code //////////////////////////
while(!queue.empty()){
	dequeue(treenode);
	queuenum--;
	for word in every possible from treenode{
		if(word == end word){
			success =1;
			break; // success
		}
		if( word does not appear in every ancestor node){
			make new tree node 
			newtreenode->generation = treenode->generation+1;
			newtreenode->list <= word
			newtreenode->parent = treenode;
			treenode->child[treenode->childnum]=newtreenode;
			treenode->childnum++;
			enqueue(newtreenode);
			queuenum++;
		}
		if(queuenum>=100000000){
			break;
		}
	}
}

if(success==0){// too many word ladder.... >> no ladder exists
	printf("No ladder\n");
}
else if(success==1){// find the "shortest" word ladder
	while(1){
		if(curTree->childnum==-1){ // curTree : headTreenode
			break;
		}
		strcpy(result[resultnum],curTree->list);
		resultnum++;
		curTree=curTree->parent;
	} // find the path of word ladder from the end word to start word.
	printf("start : %s\n",start);
	for(i=resultnum-2;i>=0;i--){
		printf("%s\n", result[i]);
	}
	printf("end : %s\n",end);
}

///////////////////end of rough code/////////////////////////////////

If you want to see the original code, check my code from line 145 to 228




Proof why this program can get "shortest" word ladder.

This program uses the breadth first search to find the word ladder.

From the parent node, it finds every possible word which has same length of start word 
and differs only one character from start word and  never appears in its ancestor node.
And it keeps going on until it finds the end word.

Since the child node's word has never appeared in its ancestor node, if this program finds "end" word, then it is the first discovery!!
That means the program finds the "shortest" path from every possible word ladder.
But if there are several shortest path, then this program will show you the path in alphabetic order in most case.
In some cases, which seldomly happens, inverse alphabetic order.

test case
1. start : go, end : on
expected : go > io > in > on
result  :  go > gn > on

2. start : back, end : fire
expected : back > bank > bane > bare > fare > fire
result : back > bark > bare > fare > fire

3. start: dog, end : cat
expected : dog > cog > cag > cat
result : dog > cog > cag > cat

4. start : dogedoms , end : bateleur
expected : No ladder
result : No ladder exists

5. start : cup, end : bat
expected : cup > cut > cat > bar
result : cup > cap > bap > bat

6. start: hat, end : six
expected: hat > ham > him > hit > sit > six
result : hat > hax > sax > six

7. start: word end : size
expected : word > wore > wire > sire > size
result : word > sord > sore > sire > size

8. start : bend end : malt
expected : bend > mend > mead > meat > melt > malt
result : bend > band > bald > balt > malt

why I set the maximum queuenum to be 100,000,000 ?
I tried some examples.
The number of words in childnode was at most 100.
And this number should get smaller as the level of tree grows.
But if the length of path gets longer, the queuenum grows up exponentially.
So, for convenience, I set the maximum queuenum.
And if queuenum excess the maximum, then the program is terminated and print "No ladder... too much queuenum....\n"
If the queuenum < maximum and the program checked every possible path but didn't find the valid word ladder, it also returns "No ladder exists\n"
