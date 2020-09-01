How to run this program
Since it uses the ANSI escape code for linux shell, it only works at linux.
g++ proj1.cpp
./a.out

And enjoy!


I made quridor game.

Plan for representing all valid states and actions

There is a structure Pawn.
typedef struct _pawn{
	int x;
	int y;
	int valid[14];
}Pawn;
It has int x, int y for position and int* valid for valid movement.

	10
      1  2  3
  11  4  P  6  12
      7  8  9
        13
Pawn A, Pawn B are variable for player.
These are global variable.

int col_wall[9][8];

int row_wall[8][9];

if there is a wall from (a,b+1) to (a+1,b+1) then col_wall[a][b]=1; else col_wall[a][b]=0;
if there is a wall from (a+1,b) to (a+1,b+1) then row_wall[a][b]=1; else row_wall[a][b]=0;


At first, read what user wants to do from keyboard. Moving pawn or constructing a wall with function int get_command(int)
(from 339 to 377)

For moving pawn, this program will run function "print_valid_movement" (from 1297 to 1404)
It will print valid movement indo stdout.
And then it runs "find_valid_movement(int x,int y,int*valid)" (from 749 to 1296).
This function will find every possible movement for (x,y) and then change Pawn.valid .
And then come back to "print_valid_movement",
print out the valid movement for Pawn.
Then get the direction from user, and return the command and be terminated.
This return value is stored at 'command'.
And then this program will run "move_pawn"(int input,char command) (from 378 to 439)
In this function, the x,y in Pawn will be updated.


For constructing wall, this program will run "print_valid_wall()" (from 168 to 228)
It gets two coordinates for wall.
One for start point the other for destination point.
And then it calls "check_wall()" (from 32 to 167) to check this wall can be constructed.
In check_wall, this function checks whether the two points are valid (in the grid or not) and there is already a wall and it can make cross wall like "+".
And return flag.
come back to "print_valid_wall", then it runs "find_shortest_path"(from 440 to 748) twice.
One for A, the other for B.
This "find_shortest_path" will find the shortest path from (x,y) to the goal line.
Since goal lines for A and B are different, this function gets int input for denoting A or B.
For A, it will find the shortest path from (x,y) to x==8.
For B, it will find that from (x,y) to x==0
If it can't find any path then return -1.
That there is no path for A or B means A or B will be isolated if the wall is constructed.
So, in print_valid_wall, it rejects the construction of wall.

After getting actions from user and updating the state, it will show the updated state.
And the turn alternates.
if A.x == 8 then A wins.
if B.x == 0 then B wins.
else then game keeps going on.


///////////////////////////////////////////////
///////////////////////////////////////////////

///////////////////Updated/////////////////////

I added a new structure

typedef struct _minmax{
	int wall;
	int m_col_wall[9][8];
	int m_row_wall[8][9];
	Pawn a;
	Pawn b;
	struct _minmax* parent;
	struct _minmax* child;
	int generation;
}Minmax;
Since I made my state variables global, I had to consider that CHANGING WHOLE PROGRAM or ADDING NEW STRUCTURE.
I chose latter.
My heuristic function is "The length of fastest way of Player's pawn - The length of fastest way of AI's pawn".
I made 3 additional functions.
ai_helper();
ai_min(Minmax*,int);
ai_max(Minmax*,int);

ai_helper() will be run right after user's turn.
It generates the first minmax node with current state.
And then run ai_max().
ai_max() will generate possible maximum childnode.
It makes 2 different node.
The first is moving AI's pawn, which means the length of fastest way from its position.
The other is constructing wall to detract player's pawn, which means the construction that makes player's way largest.
And then it compares the value and then chooses the maximum value.
If moving pawn has maximum value, then it will move the pawn.
And in this ai_max function, it will change current minmax node as maximum node and make a new childnode for minnode.
And then this ai_max will run ai_min.
ai_min function will make best decision of player's pawn.
In this function, the heuristic function will be negative. And it will run ai_max recursively.
After searching 26 nodes deep, it will be terminated.
If by searching, one of this function find end state, then every function will be ternimated.
And there would be a message from commandline who gonna win.





