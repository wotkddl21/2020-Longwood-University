#include<iostream>
#include<time.h>
#include<unistd.h>
typedef struct _pawn {
	int x;
	int y;
	int valid[14];
	int wall;
	int generation;
	/*
	10
	1  2  3
	11 4  P  6 12
	7  8  9
	13
	*/
}Pawn;
typedef struct _minmax{
	int wall; // 0 for movement 1 for wall
	int m_col_wall[9][8];
	int m_row_wall[8][9];
	Pawn a;
	Pawn b;
	struct _minmax* parent;
	struct _minmax* child;
	int generation;
}Minmax;

Minmax* MinHead;

using namespace std;
Pawn A;
Pawn B;
Pawn current;
Pawn Queue[1000000];
int queuenum;

int turn = 0;
int col_wall[9][8];
int row_wall[8][9];
void ai_helper();
void ai_min(Minmax*,int);
void ai_max(Minmax*,int);
int find_shortest_path(int, int, int);
void find_valid_movement(int, int, int, int*);
void show_how_to_build_wall();
int check_wall(int s_x, int s_y, int d_x, int d_y, int set) {
	int flag = 0;
	int i = 0;
	int temp = 0;
	if (s_x >= d_x) {
		temp = d_x;
		d_x = s_x;
		s_x = temp;
	}
	if (s_y >= d_y) {
		temp = d_y;
		d_y = s_y;
		s_y = temp;
	}
	if (s_x == d_x) {
		if (s_y - d_y == -1) { // row wall
			flag = 2;
		}
		else {
			return -1;
		}
	}
	else if (s_y == d_y) {
		if (s_x - d_x == -1) { // col wall
			flag = 1;
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
	if (set) {
		if (flag == 2) {  //check row
			if (s_x<0 || s_x>8) {
				return 10;
			}
			if (s_y<0 || s_y>8) {
				return 10;
			}
			if (d_y<1 || d_y>9) {
				return 10;
			}

			if (row_wall[s_x][s_y] == 1 || row_wall[d_x][d_y] == 1) {
				return 20;
			}
			//check cross
			if (col_wall[s_x][s_y] == 1 && col_wall[s_x + 1][s_y] == 1) {
				return 30;
			}
			row_wall[s_x][s_y] = row_wall[d_x][d_y] = set;
		}
		else if (flag == 1) {  //check col
			if (s_y<0 || s_y>8) {
				return 10;
			}
			if (s_x<0 || s_x >8) {
				return 10;
			}
			if (d_x<1 || d_x >9) {
				return 10;
			}
			if (col_wall[s_x][s_y] == 1 || col_wall[d_x][d_y] == 1) {
				return 20;
			}
			if (row_wall[s_x][s_y] == 1 && row_wall[s_x][s_y + 1] == 1) {
				return 30;
			}
			col_wall[s_x][s_y] = col_wall[d_x][d_y] = set;

		}
	}
	else {
		if (flag == 2) {  //check row
			if (s_x<0 || s_x>8) {
				return 10;
			}
			if (s_y<0 || s_y>8) {
				return 10;
			}
			if (d_y<1 || d_y>9) {
				return 10;
			}

			row_wall[s_x][s_y] = row_wall[d_x][d_y] = set;
		}
		else if (flag == 1) {  //check col
			if (s_y<0 || s_y>8) {
				return 10;
			}
			if (s_x<0 || s_x >8) {
				return 10;
			}
			if (d_x<1 || d_x >9) {
				return 10;
			}
			col_wall[s_x][s_y] = col_wall[d_x][d_y] = set;
		}

	}
	return flag;
};
void print_valid_wall(void) {
	int row = 0;
	int col = 0;
	int s_x;
	int s_y;
	int d_x;
	int d_y;
	int flag = 0; // flag==0 >> invalid  , flag==1 >> valid
	int flag2 = 0;
	int flag3 = 0;
	if (turn) {
		if (A.wall == 0) {
			printf("You already use all of your walls\n");
			return;
		}
		else {
			A.wall--;
		}
	}
	else {
		if (B.wall == 0) {
			printf("You already use all of your walls\n");
			return;
		}
		else {
			B.wall--;
		}
	}
	while (1) {
		printf("Input (s_x,s_y) , (d_x,d_y)\n");
		printf("s_x :");
		scanf("%d", &s_x);
		printf("s_y :");
		scanf("%d", &s_y);
		printf("d_x :");
		scanf("%d", &d_x);
		printf("d_y :");
		scanf("%d", &d_y);
		flag = check_wall(s_x, s_y, d_x, d_y, 1);
		if (flag == 10) {
			printf("Invalid range\n");
		}
		else if (flag == 20) {
			printf("There is already a wall\n");
		}
		else if (flag == 30) {
			printf("There will be a cross\n");
		}
		else if (flag == -1) {
			printf("Invalid wall construction try again\n");
		}
		else {
			if (turn) {
				flag3 = find_shortest_path(B.x, B.y, 0);
				flag2 = find_shortest_path(A.x, A.y, 1);

			}
			else {
				flag3 = find_shortest_path(B.x, B.y, 0);
				flag2 = find_shortest_path(A.x, A.y, 1);
			}
			if (flag2 == -1) {
				printf("You can't block opponent's way\n");
				flag = check_wall(s_x, s_y, d_x, d_y, 0);
			}
			else if (flag3 == -1) {
				printf("You can't block opponent's way\n");
				flag = check_wall(s_x, s_y, d_x, d_y, 0);
			}
			if (flag != 0) {
				break;
			}
		}
	}
	printf("B : %d\n", flag3);
	printf("A : %d\n", flag2);
};

void print_star(int x, int y) {
	if (A.x == x && A.y == y) {
		printf("\e[33m");
		printf("\e[5m");
		printf("A ");
		printf("\e[0m");

	}
	else if (B.x == x && B.y == y) {
		printf("\e[36m");
		printf("\e[5m");
		printf("B ");
		printf("\e[0m");
	}
	else {
		printf("* ");
	}

};

void print_col_wall(int x, int y) {
	if (col_wall[x][y] == 1) {
		printf("| ");
	}
	else {
		printf("  ");
	}
};

void print_row_wall(int x, int y) {
	if (y>0 && y<8) {
		if (row_wall[x][y] == 1) {
			printf("---'");
		}
		else {
			printf("   '");
		}
	}
	else if (y == 8) {
		if (row_wall[x][y] == 1) {
			printf("--'");
		}
		else {
			printf("  '");
		}
	}
	else if (y == 0) {
		if (row_wall[x][y] == 1) {
			printf("--'");
		}
		else {
			printf("  '");
		}

	}
};

void print_grid() {
	int i = 0;
	int j = 0;
	// col [9][8]  // row[8][9]
	printf("***********\t***********\n");
	printf("**  number \tof walls **\n");
	printf("**  ");
	printf("\e[33m");
	printf("A : %d  ", A.wall);
	printf("\e[36m");
	printf("\tB : %d ", B.wall);
	printf("\e[0m");
	printf("  **\n");
	printf("**         \t         **\n");
	printf("***********\t***********\n");

	printf(" ");
	for (i = 0; i <= 7; i++) {
		printf(" %d .", i);
	}
	printf(" 8\n");
	for (i = 0; i<8; i++) {
		printf("%d ", i);
		for (j = 0; j<8; j++) {
			print_star(i, j);
			print_col_wall(i, j);
		}
		print_star(i, 8);
		printf("\n");
		printf(" '");
		for (j = 0; j<9; j++) {
			print_row_wall(i, j);
		}
		printf("\n");
	}
	printf("%d ", 8);
	for (i = 0; i<8; i++) {
		print_star(8, i);
		print_col_wall(8, i);
	}
	print_star(8, 8);
	printf("\n");
	printf("  ");
	for (i = 0; i<7; i++) {
		printf("  ' ");
	}
	printf("  '");
	printf("\n");

}
void show_how_to_build_wall() {
	int i, j;
	printf("********* Coordinate ***********\n");
	printf("  ");
	for (i = 0; i <= 7; i++) {
		printf("  %d  .", i);
	}
	printf(" 8\n");
	for (i = 0; i<8; i++) {
		printf("%d ", i);
		for (j = 0; j<8; j++) {
			printf("(%d,%d)", i, j);
			if (col_wall[i][j] != 1) {
				printf("|");
			}
			else {
				printf(" ");
			}
		}
		printf("(%d,%d)\n", i, 8);
		printf(" ");
		for (j = 0; j <= 8; j++) {
			printf(" ");
			if (row_wall[i][j] != 1) {
				printf("-----");
			}
			else {
				printf("     ");
			}
		}
		printf("\n");

	}


	printf("To build wall[a][b]\n");
	printf("  (a,b)|    <- col_wall[a][b] (0<=b<=7)\n");
	printf("  -----     <- row_wall[a][b] (0<=a<=7)\n");



}
int check_winner() {
	if (A.x == 8) {
		print_grid();
		printf("A is a winner\n");
		return 1;
	}
	else if (B.x == 0) {
		print_grid();
		printf("B is a winner\n");
		return 2;
	}
	else {
		return 0;
	}
};
int get_command(int input) {
	char command;
	int ret = 0;
	if (input) {
		while (1) {
			printf("Player A's turn... press P/p to move pawn or press W/w to build wall: ");
			scanf(" %c", &command);
			if (command == 'P' || command == 'p') {
				ret = 1;
				break;
			}
			else if (command == 'W' || command == 'w') {
				if (A.wall == 0) {
					printf("You use all of your walls!\n");
				}
				else {
					ret = 2;
					break;
				}
			}
			else {
				printf("\nInvalid input...\n");
			}
		}
	}
	else {
		while (1) {
			printf("Player B's turn... press P/p to move pawn or press W/w to build wall: ");
			scanf(" %c", &command);
			if (command == 'P' || command == 'p') {
				ret = 1;
				break;
			}
			else if (command == 'W' || command == 'w') {
				if (B.wall == 0) {
					printf("You use all of your walls!\n");
				}
				else {
					ret = 2;
					break;
				}
			}
			else {
				printf("\nInvalid input...\n");
			}
		}
	}
	return ret;
}
void move_pawn(int command, int input) {
	if (input == 1) {//for A
		switch (command) {
		case 1: A.x--; A.y--;
			break;
		case 2:A.x--;
			break;
		case 3: A.x--; A.y++;
			break;
		case 4:A.y--;
			break;
		case 6:A.y++;
			break;
		case 7:A.y--; A.x++;
			break;
		case 8:A.x++;
			break;
		case 9:A.x++; A.y++;
			break;
		case 10:A.x--; A.x--;
			break;
		case 11:A.y--; A.y--;
			break;
		case 12:A.y++; A.y++;
			break;
		case 13:if(A.x !=7){A.x++;} A.x++;
			break;
		default:
			break;
		}
	}
	else if (input == 0) {//for B
		switch (command) {
		case 1: B.x--; B.y--;
			break;
		case 2:B.x--;
			break;
		case 3: B.x--; B.y++;
			break;
		case 4:B.y--;
			break;
		case 6:B.y++;
			break;
		case 7:B.y--; B.x++;
			break;
		case 8:B.x++;
			break;
		case 9:B.x++; B.y++;
			break;
		case 10:B.x--; B.x--;
			break;
		case 11:B.y--; B.y--;
			break;
		case 12:B.y++; B.y++;
			break;
		case 13:B.x++; B.x++;
			break;
		default:
			break;
		}
	}
}
int find_shortest_path(int x, int y, int input) {
	//check in grid
	//check there is a wall
	//check there is a pawn
	int i = 0;
	int j = 0;
	for (i = 0; i<queuenum; i++) {
		Queue[i].x = 0;
		Queue[i].y = 0;
		for (j = 0; j<14; j++) {
			Queue[i].valid[j] = 0;
		}
	}
	queuenum = 0;
	int grid[9][9] = { 0, };
	int length = 0;
	for (i = 0; i<9; i++) {
		for (j = 0; j<9; j++) {
			grid[i][j] = 0;
		}
	}
	//initializing the queue
	Pawn temp;
	if (input) {
		// priority : 13
		//            7==9==8
		//            11==12
		//            4==6==8
		//            2
		//            1==3
		//            10

		grid[x][y] = 1;
		find_valid_movement(x, y, input, Queue[queuenum].valid);// insert root
		Queue[0].x = x;
		Queue[0].y = y;
		Queue[0].generation = 1;
		queuenum++;
		//dequeue
		while (1) {
			temp = Queue[0];
			length++;
			grid[temp.x][temp.y] = 1;
			x = temp.x;
			y = temp.y;
			for (i = 0; i<queuenum - 1; i++) {
				Queue[i] = Queue[i + 1];
			}
			queuenum--;
			if (temp.x == 8) {
				break;
			}
			if (temp.valid[13] == 1) {
				if (x + 2 <= 8) {
					if (grid[x + 2][y] != 1) {
						grid[x + 2][y] = 1;
						find_valid_movement(x + 2, y, input, Queue[queuenum].valid);
						Queue[queuenum].x = x + 2;
						Queue[queuenum].y = y;
						queuenum++;
						Queue[queuenum].generation = temp.generation + 1;
					}
				}
			}
			if (temp.valid[7] == 1) {
				if (grid[x + 1][y - 1] != 1) {
					grid[x + 1][y - 1] = 0;
					find_valid_movement(x + 1, y - 1, input, Queue[queuenum].valid);
					Queue[queuenum].x = x + 1;
					Queue[queuenum].y = y - 1;
					queuenum++;
					Queue[queuenum].generation = temp.generation + 1;
				}
			}
			if (temp.valid[9] == 1) {
				if (grid[x + 1][y + 1] == 0) {
					grid[x + 1][y + 1] = 1;
					find_valid_movement(x + 1, y + 1, input, Queue[queuenum].valid);
					Queue[queuenum].x = x + 1;
					Queue[queuenum].y = y + 1;
					queuenum++;
					Queue[queuenum].generation = temp.generation + 1;
				}
			}
			if (temp.valid[8] == 1) {
				if (grid[x + 1][y] == 0) {
					grid[x + 1][y] = 1;
					find_valid_movement(x + 1, y, input, Queue[queuenum].valid);
					Queue[queuenum].x = x + 1;
					Queue[queuenum].y = y;
					queuenum++;
					Queue[queuenum].generation = temp.generation + 1;
				}
			}
			if (temp.valid[12] == 1) {
				if (y + 2 <= 8) {
					if (grid[x][y + 2] != 1) {
						grid[x][y + 2] = 1;
						find_valid_movement(x, y + 2, input, Queue[queuenum].valid);
						Queue[queuenum].x = x;
						Queue[queuenum].y = y + 2;
						queuenum++;
						Queue[queuenum].generation = temp.generation + 1;
					}
				}
			}
			if (temp.valid[11] == 1) {
				if (y - 2>0) {
					if (grid[x][y - 2] != 1) {
						find_valid_movement(x, y - 2, input, Queue[queuenum].valid);
						Queue[queuenum].x = x;
						Queue[queuenum].y = y - 2;
						grid[x][y - 2] = 1;
						queuenum++;
						Queue[queuenum].generation = temp.generation + 1;
					}
				}
			}
			if (temp.valid[4] == 1) {
				if (grid[x][y - 1] == 0) {
					grid[x][y - 1] = 1;
					find_valid_movement(x, y - 1, input, Queue[queuenum].valid);
					Queue[queuenum].x = x;
					Queue[queuenum].y = y - 1;
					queuenum++;
					Queue[queuenum].generation = temp.generation + 1;
				}
			}
			if (temp.valid[6] == 1) {
				if (grid[x][y + 1] == 0) {
					grid[x][y + 1] = 1;
					find_valid_movement(x, y + 1, input, Queue[queuenum].valid);
					Queue[queuenum].x = x;
					Queue[queuenum].y = y + 1;
					queuenum++;
					Queue[queuenum].generation = temp.generation + 1;
				}
			}
			if (temp.valid[2] == 1) {
				if (grid[x - 1][y] == 0) {
					grid[x - 1][y] = 1;
					find_valid_movement(x - 1, y, input, Queue[queuenum].valid);
					Queue[queuenum].x = x - 1;
					Queue[queuenum].y = y;
					queuenum++;
					Queue[queuenum].generation = temp.generation + 1;
				}
			}
			if (temp.valid[1] == 1) {
				if (grid[x - 1][y - 1] == 0) {
					grid[x - 1][y - 1] = 1;
					find_valid_movement(x - 1, y - 1, input, Queue[queuenum].valid);
					Queue[queuenum].x = x - 1;
					Queue[queuenum].y = y - 1;
					queuenum++;
					Queue[queuenum].generation = temp.generation + 1;
				}
			}
			if (temp.valid[3] == 1) {
				if (grid[x - 1][y + 1] == 0) {
					grid[x - 1][y + 1] = 1;
					find_valid_movement(x - 1, y + 1, input, Queue[queuenum].valid);
					Queue[queuenum].x = x - 1;
					Queue[queuenum].y = y + 1;
					queuenum++;
					Queue[queuenum].generation = temp.generation + 1;
				}
			}
			if (temp.valid[10] == 1) {
				if (x - 2>0) {
					if (grid[x - 2][y] == 0) {
						grid[x - 2][y] = 1;
						find_valid_movement(x - 2, y, input, Queue[queuenum].valid);
						Queue[queuenum].x = x - 2;
						Queue[queuenum].y = y;
						queuenum++;
						Queue[queuenum].generation = temp.generation + 1;
					}
				}
			}
			length = temp.generation;
			if (queuenum == 0) {
				length = -1;
				break;
			}
		}
	}
	else {
		//priority : 10
		//           1==3==2
		//           11==12
		//           4==6
		//           8
		//           7==9
		//           13
		grid[x][y] = 1;
		find_valid_movement(x, y, input, Queue[queuenum].valid);// insert root
		queuenum = 1;
		Queue[0].x = x;
		Queue[0].y = y;
		Queue[0].generation = 0;
		int q = 0;
		//dequeue
		int checkpoint[8][8] = { 0 };
		while (1) {
			temp = Queue[0];
			length++;
			grid[temp.x][temp.y] = 1;
			x = temp.x;
			y = temp.y;
			for (i = 0; i<queuenum - 1; i++) {
				Queue[i] = Queue[i + 1];
			}
			queuenum--;
			if (temp.x == 0) {
				break;
			}
			if (temp.valid[10] == 1) {
				if (x - 2>0) {
					if (grid[x - 2][y] != 1) {
						grid[x - 2][y] = 1;
						find_valid_movement(x - 2, y, input, Queue[queuenum].valid);
						Queue[queuenum].x = x - 2;
						Queue[queuenum].y = y;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
			}
			if (temp.valid[1] == 1) {
				if (grid[x - 1][y - 1] != 1) {
					grid[x - 1][y - 1] = 0;
					find_valid_movement(x - 1, y - 1, input, Queue[queuenum].valid);
					Queue[queuenum].x = x - 1;
					Queue[queuenum].y = y - 1;
					Queue[queuenum].generation = temp.generation + 1;
					queuenum++;
				}
			}
			if (temp.valid[3] == 1) {
				if (grid[x - 1][y + 1] == 0) {
					grid[x - 1][y + 1] = 1;
					find_valid_movement(x - 1, y + 1, input, Queue[queuenum].valid);
					Queue[queuenum].x = x - 1;
					Queue[queuenum].y = y + 1;
					Queue[queuenum].generation = temp.generation + 1;
					queuenum++;
				}
			}
			if (temp.valid[2] == 1) {
				if (grid[x - 1][y] == 0) {
					grid[x - 1][y] = 1;
					find_valid_movement(x - 1, y, input, Queue[queuenum].valid);
					Queue[queuenum].x = x - 1;
					Queue[queuenum].y = y;
					Queue[queuenum].generation = temp.generation + 1;
					queuenum++;
				}
			}
			if (temp.valid[12] == 1) {
				if (y + 2 <= 8) {
					if (grid[x][y + 2] != 1) {
						grid[x][y + 2] = 1;
						find_valid_movement(x, y + 2, input, Queue[queuenum].valid);
						Queue[queuenum].x = x;
						Queue[queuenum].y = y + 2;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}

			}
			if (temp.valid[11] == 1) {
				if (y - 2>0) {
					if (grid[x][y - 2] != 1) {
						find_valid_movement(x, y - 2, input, Queue[queuenum].valid);
						Queue[queuenum].x = x;
						Queue[queuenum].y = y - 2;
						grid[x][y - 2] = 1;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
			}
			if (temp.valid[4] == 1) {
				if (grid[x][y - 1] == 0) {
					grid[x][y - 1] = 1;
					find_valid_movement(x, y - 1, input, Queue[queuenum].valid);
					Queue[queuenum].x = x;
					Queue[queuenum].y = y - 1;
					Queue[queuenum].generation = temp.generation + 1;
					queuenum++;
				}
			}
			if (temp.valid[6] == 1) {
				if (grid[x][y + 1] == 0) {
					grid[x][y + 1] = 1;
					find_valid_movement(x, y + 1, input, Queue[queuenum].valid);
					Queue[queuenum].x = x;
					Queue[queuenum].y = y + 1;
					Queue[queuenum].generation = temp.generation + 1;
					queuenum++;
				}
			}
			if (temp.valid[8] == 1) {
				if (grid[x + 1][y] == 0) {
					grid[x + 1][y] = 1;
					find_valid_movement(x + 1, y, input, Queue[queuenum].valid);
					Queue[queuenum].x = x + 1;
					Queue[queuenum].y = y;
					Queue[queuenum].generation = temp.generation + 1;
					queuenum++;
				}
			}
			if (temp.valid[7] == 1) {
				if (grid[x + 1][y - 1] == 0) {
					grid[x + 1][y - 1] = 1;
					find_valid_movement(x + 1, y - 1, input, Queue[queuenum].valid);
					Queue[queuenum].x = x + 1;
					Queue[queuenum].y = y - 1;
					Queue[queuenum].generation = temp.generation + 1;
					queuenum++;
				}
			}
			if (temp.valid[9] == 1) {
				if (grid[x + 1][y + 1] == 0) {
					grid[x + 1][y + 1] = 1;
					find_valid_movement(x + 1, y + 1, input, Queue[queuenum].valid);
					Queue[queuenum].x = x + 1;
					Queue[queuenum].y = y + 1;
					Queue[queuenum].generation = temp.generation + 1;
					queuenum++;
				}
			}
			if (temp.valid[13] == 1) {
				if (x + 2 <= 8) {
					if (grid[x + 2][y] == 0) {
						grid[x + 2][y] = 1;
						find_valid_movement(x + 2, y, input, Queue[queuenum].valid);
						Queue[queuenum].x = x + 2;
						Queue[queuenum].generation = temp.generation + 1;
						Queue[queuenum].y = y;
					}
				}
			}
			length = temp.generation;
			if (queuenum == 0) {
				length = -1;
				break;
			}
		}
	}
	return length;
}
void find_valid_movement(int x, int y, int input, int * valid) {
	int avail[14] = { 0 };
	if (input) { // for A
				 /*     10
				 1  2  3
				 11 4  p  6 12
				 7  8  9
				 13
				 */
		if (x == 0) {  /* 4 p 6
					   7 8 9
					   */
			avail[4] = avail[6] = avail[8] = 1;
			if (y == 0) {
				avail[4] = 0;
				if (row_wall[0][0] == 1) {
					avail[8] = 0;
				}
				if (col_wall[0][0] == 1) {
					avail[6] = 0;
				}
				if (B.x == 1 && B.y == y) {
					avail[8] = 0;
					avail[13] = 1;
				}
				if (row_wall[1][0] == 1) {
					if (avail[13] != 0) {
						avail[9] = 1;
					}
				}
			}
			else if (y == 8) {
				avail[6] = 0;
				if (row_wall[0][8] == 1) {
					avail[8] = 0;
				}
				if (col_wall[0][7] == 1) {
					avail[4] = 0;
				}
				if (B.x == 1 && B.y == y) {
					avail[8] = 0;
					avail[13] = 1;
				}
				if (row_wall[1][0] == 1) {
					if (avail[13] != 0) {
						avail[7] = 1;
					}
				}

			}
			else { // A.x==0 , A.y>=1 && A.y<=7
				   /* 4 p 6
				   8
				   */
				if (col_wall[x][y - 1] == 1) { // left wall
					avail[4] = 0;
				}
				if (col_wall[x][y] == 1) { // right wall
					avail[6] = 0;
				}
				if (row_wall[x][y] == 1) {//  bottom wall
					avail[8] = 0;
				}
				if ((B.x == (x)+1) && (B.y == y)) {  // A
													 // B
					avail[8] = 0;
					avail[13] = 1;
					if (row_wall[B.x][B.y] == 1) {
						avail[13] = 0;
						avail[7] = avail[9] = 1;
					}
					if (col_wall[B.x][B.y] == 1) {  //   A
													// 7 B|
													//   13
						avail[9] = 0;
					}
					if (col_wall[B.x][B.y - 1] == 1) {
						avail[7] = 0;
					}
				}
			}
		}// A.x ==0 clear
		 //A.x >=1  start
		else if (y == 0) { // A.x>=1
			avail[2] = avail[6] = avail[8] = 1;
			if (row_wall[x][y] == 1) {
				avail[8] = 0;
			}
			if (row_wall[x - 1][y] == 1) {
				avail[2] = 0;
			}
			if (col_wall[x][y] == 1) {
				avail[6] = 0;
			}
			if (B.x == x - 1 && B.y == y) { // B
											// A
				avail[2] = 0;
				avail[10] = 1;
				if (B.x>0 && row_wall[B.x - 1][B.y] == 1) {
					avail[10] = 0;
					avail[3] = 1;
				}
				if (col_wall[B.x][B.y] == 1) {
					avail[3] = 0;
				}
			}
			if (B.x == x + 1 && B.y == y) { //A
											//B
				avail[8] = 0;
				avail[13] = 1;
				if (B.x <= 7 && row_wall[B.x][B.y] == 1) {
					avail[13] = 0;
					avail[9] = 1;
				}
				if (col_wall[B.x][B.y] == 1) {
					avail[9] = 1;
				}
			}
			if (B.x == x && B.y == y + 1) { // A B
				avail[6] = 0;
				avail[12] = 1;
				if (col_wall[B.x][B.y] == 1) {
					avail[12] = 0;
					avail[3] = 1;
					avail[9] = 1;
				}
				//B.x !=8  since if A.x ==8  then game set
				if (row_wall[B.x][B.y] == 1) {
					avail[9] = 0;
				}
				if (row_wall[B.x - 1][B.y] == 1) {
					avail[3] = 0;
				}
			}
		}
		else if (y == 8) {
			avail[2] = avail[4] = avail[8] = 1;
			if (row_wall[x][y] == 1) {
				avail[8] = 0;
			}
			if (row_wall[x - 1][y] == 1) {
				avail[2] = 0;
			}
			if (col_wall[x][y - 1] == 1) {
				avail[4] = 0;
			}
			if (B.x == x - 1 && B.y == y) { // B
											// A
				avail[2] = 0;
				avail[10] = 1;
				if (B.x>0 && row_wall[B.x - 1][B.y] == 1) {
					avail[10] = 0;
					avail[1] = 1;
				}
				if (col_wall[B.x][B.y] == 1) {
					avail[1] = 0;
				}
			}
			if (B.x == x + 1 && B.y == y) { //A
											//B
				avail[8] = 0;
				avail[13] = 1;
				if (B.x <= 7 && row_wall[B.x][B.y] == 1) {
					avail[13] = 0;
					avail[7] = 1;
				}
				if (col_wall[B.x][B.y] == 1) {
					avail[7] = 0;
				}
			}
			if (B.x == x && B.y == y - 1) { // A B
				avail[4] = 0;
				avail[11] = 1;
				if (col_wall[B.x][B.y] == 1) {
					avail[11] = 0;
					avail[1] = 1;
					avail[7] = 1;
				}
				//B.x !=8  since if A.x ==8  then game set
				if (row_wall[B.x][B.y] == 1) {
					avail[7] = 0;
				}
				if (row_wall[B.x - 1][B.y] == 1) {
					avail[1] = 0;
				}
			}

		}
		// A.x >=1  A.y >=1 && <=7
		else {
			avail[2] = avail[4] = avail[6] = avail[8] = 1;

			if (row_wall[x - 1][y] == 1) {
				avail[2] = 0;
			}
			if (row_wall[x][y] == 1) {
				avail[8] = 0;
			}
			if (col_wall[x][y - 1] == 1) {
				avail[4] = 0;
			}
			if (col_wall[x][y] == 1) {
				avail[6] = 0;
			}
			/// first wall clear
			//check for b
			if (B.x == x + 1 && y == B.y) {  // A
											 //  B
				avail[8] = 0;
				avail[13] = 1;
			}
			if (B.x == x - 1 && y == B.y) { // B
											// A
				avail[2] = 0;
				avail[10] = 1;
			}
			if (B.x == x && B.y == y + 1) { // A  B
				avail[6] = 0;
				avail[12] = 1;
			}
			if (B.x == x && B.y == y - 1) { // B  A
				avail[4] = 0;
				avail[11] = 1;
			}
			// b clear
			// check one more wall
			if (x >= 2 && x <= 6) {                        //
				if (row_wall[x - 2][y] == 1) {    //     1  B  3
												  //        A
												  //     7
					if (avail[10] == 1) {
						avail[1] = avail[3] = 1;
						if (col_wall[B.x][B.y] == 1) {
							avail[3] = 0;
						}
						if (col_wall[B.x][B.y - 1] == 1) {
							avail[1] = 0;
						}
					}
					avail[10] = 0;

				}
				if (row_wall[x + 1][y] == 1) {
					if (avail[13] == 1) {
						avail[7] = avail[9] = 1;
						if (col_wall[B.x][B.y] == 1) {
							avail[9] = 0;
						}
						if (col_wall[B.x][B.y - 1] == 1) {
							avail[7] = 0;
						}
					}
					avail[13] = 0;
				}
				if (y >= 2 && y <= 6) {
					if (col_wall[x][y - 2] == 1) {
						if (avail[11] == 1) {
							avail[1] = avail[7] = 1;
							if (row_wall[B.x][B.y] == 1) {
								avail[7] = 0;
							}
							if (row_wall[B.x - 1][B.y] == 1) {
								avail[1] = 0;
							}
						}
						avail[11] = 0;
					}
					if (col_wall[x][y + 1] == 1) {
						if (avail[12] == 1) {
							avail[3] = avail[9] = 1;
							if (row_wall[B.x][B.y] == 1) {
								avail[9] = 0;
							}
							if (row_wall[B.x - 1][B.y] == 1) {
								avail[3] = 0;
							}
						}
						avail[12] = 0;
					}
				}
			}
		}
	}
	else {
		if (x == 8) {
			avail[2] = avail[4] = avail[6] = 1;
			if (y == 0) {
				avail[4] = 0;
				if (row_wall[x - 1][y] == 1) {
					avail[2] = 0;
				}
				if (col_wall[x][y] == 1) {
					avail[6] = 0;
				}
				if (A.x == 7 && y == A.y) {
					avail[2] = 0;
					avail[10] = 1;
				}
				if (row_wall[x - 2][y] == 1) {
					if (avail[10] == 1) {
						avail[3] = 1;
					}
				}
			}
			else if (y == 8) {
				avail[6] = 0;
				if (row_wall[x - 1][y] == 1) {
					avail[2] = 0;
				}
				if (col_wall[x][y - 1] == 1) {
					avail[4] = 0;
				}
				if (A.x == 7 && y == A.y) {
					avail[2] = 0;
					avail[10] = 1;
				}
				if (row_wall[x - 2][y] == 1) {
					if (avail[10] == 1) {
						avail[1] = 1;
					}
				}
			}
			else { // B.x==8 B.y>=1 && <=7
				if (col_wall[x][y - 1] == 1) {
					avail[4] = 0;
				}
				if (col_wall[A.x][A.y] == 1) {
					avail[6] = 0;
				}
				if (row_wall[x - 1][y] == 1) {
					avail[2] = 0;
				}
				if (x - 1 == A.x && (y == A.y)) {
					avail[2] = 0;
					avail[10] = 1;
					if (row_wall[A.x - 1][A.y] == 1) {
						avail[10] = 0;
						avail[1] = avail[3] = 1;
					}
					if (col_wall[A.x][A.y] == 1) {
						avail[3] = 0;
					}
					if (col_wall[A.x][A.y - 1] == 1) {
						avail[1] = 0;
					}
				}
			}
		} // B.x ==8 clear
		  //B.x <=7 start
		else if (y == 0) {
			avail[2] = avail[6] = avail[8] = 1;
			if (row_wall[x][y] == 1) {
				avail[8] = 0;
			}
			if (row_wall[x - 1][y] == 1) {
				avail[2] = 0;
			}
			if (col_wall[x][y] == 1) {
				avail[6] = 0;
			}
			if (x == A.x + 1 && y == A.y) { // A
											// B
				avail[2] = 0;
				avail[10] = 1;
				if (A.x>0 && row_wall[A.x - 1][A.y] == 1) {
					avail[10] = 0;
					avail[3] = 1;
				}
				if (col_wall[A.x][A.y] == 1) {
					avail[3] = 0;
				}
			}
			if (x == A.x - 1 && y == A.y) { //B
											//A
				avail[8] = 0;
				avail[13] = 1;
				if (A.x<7 && row_wall[A.x][y] == 1) {
					avail[13] = 0;
					avail[9] = 1;
				}
				if (col_wall[A.x][A.y] == 1) {
					avail[9] = 0;
				}
			}
			if (x == A.x&& y + 1 == A.y) {  // B  A
				avail[6] = 0;
				avail[12] = 1;
				if (col_wall[A.x][A.y] == 1) {
					avail[12] = 0;
					avail[3] = avail[9] = 1;
				}
				if (row_wall[A.x][A.y] == 1) {
					avail[9] = 0;
				}
				if (row_wall[A.x - 1][A.y] == 1) {
					avail[3] = 0;
				}

			}
		}
		else if (y == 8) {
			avail[2] = avail[4] = avail[8] = 1;
			if (row_wall[x][y] == 1) {
				avail[8] = 0;
			}
			if (row_wall[x - 1][y] == 1) {
				avail[2] = 0;
			}
			if (col_wall[x][y - 1] == 1) {
				avail[4] = 0;
			}
			if (x == A.x + 1 && y == A.y) { // A
											// B
				avail[2] = 0;
				avail[10] = 1;
				if (A.x>0 && row_wall[A.x - 1][A.y] == 1) {
					avail[10] = 0;
					avail[1] = 1;
				}
				if (col_wall[A.x][A.y] == 1) {
					avail[1] = 0;
				}
			}
			if (x == A.x - 1 && y == A.y) { //B
											//A
				avail[8] = 0;
				avail[13] = 1;
				if (A.x<7 && row_wall[A.x][y] == 1) {
					avail[13] = 0;
					avail[7] = 1;
				}
				if (col_wall[A.x][A.y] == 1) {
					avail[7] = 0;
				}
			}
			if (x == A.x&& y + 1 == A.y) {  // B  A
				avail[6] = 0;
				avail[12] = 1;
				if (col_wall[A.x][A.y - 1] == 1) {
					avail[12] = 0;
					avail[1] = avail[7] = 1;
				}
				if (row_wall[A.x][A.y] == 1) {
					avail[7] = 0;
				}
				if (row_wall[A.x - 1][A.y] == 1) {
					avail[1] = 0;
				}

			}
		}
		//B.x >=1  B.y>=1 && <=7
		else {
			avail[2] = avail[4] = avail[6] = avail[8] = 1;
			if (row_wall[x - 1][y] == 1) {
				avail[2] = 0;
			}
			if (row_wall[x][y] == 1) {
				avail[8] = 0;
			}
			if (col_wall[x][y - 1] == 1) {
				avail[4] = 0;
			}
			if (col_wall[x][y] == 1) {
				avail[6] = 0;
			}
			//first wall clear
			//check for A
			if (x == A.x - 1 && y == A.y) { //B
											//A
				avail[8] = 0;
				avail[13] = 1;
			}
			if (x == A.x + 1 && y == A.y) {
				avail[2] = 0;
				avail[10] = 1;
			}
			if (x == A.x && y == A.y - 1) { // B  A
				avail[6] = 0;
				avail[12] = 1;
			}
			if (x == A.x && y == A.y + 1) {
				avail[4] = 0;
				avail[11] = 1;
			}
			//clear for check a
			//check one more wall
			if (x >= 2 && x <= 6) {
				if (row_wall[x - 2][y] == 1) {
					if (avail[10] == 1) {
						avail[1] = avail[3] = 1;
						if (col_wall[A.x][A.y] == 1) {
							avail[3] = 0;
						}
						if (col_wall[A.x][A.y - 1] == 1) {
							avail[1] = 0;
						}
					}
					avail[10] = 0;
				}
				if (row_wall[x + 1][y] == 1) {
					if (avail[13] == 1) {
						avail[7] = avail[9] = 1;
						if (col_wall[A.x][A.y] == 1) {
							avail[9] = 0;
						}
						if (col_wall[A.x][A.y - 1] == 1) {
							avail[7] = 0;
						}
					}
					avail[13] = 0;
				}
				if (y >= 2 && y <= 6) {
					if (col_wall[x][y - 2] == 1) {
						if (avail[11] == 1) {
							avail[1] = avail[7] = 1;
							if (row_wall[A.x][A.y] == 1) {
								avail[7] = 0;
							}
							if (row_wall[A.x - 1][A.y] == 1) {
								avail[1] = 0;
							}
						}

						avail[11] = 0;
					}
					if (col_wall[x][y + 1] == 1) {
						if (avail[12] == 1) {
							avail[3] = avail[9] = 1;
							if (row_wall[A.x][A.y] == 1) {
								avail[9] = 0;
							}
							if (row_wall[A.x - 1][A.y] == 1) {
								avail[3] = 0;
							}
						}
						avail[12] = 0;
					}
				}
			}
		}
	}

	int i = 0;
	for (i = 0; i<14; i++) {
		valid[i] = avail[i];
	}
}
int print_valid_movement(int input) {
	int command;
	int avail[14] = { 0 };
	/////print//////////
	int i = 0;
	int j = 0;
	if (input) {
		find_valid_movement(A.x, A.y, 1, A.valid);
		for (i = 0; i<14; i++) {
			avail[i] = A.valid[i];
		}
	}
	else {
		find_valid_movement(B.x, B.y, 0, B.valid);
		for (i = 0; i<14; i++) {
			avail[i] = B.valid[i];
		}
	}
	printf("\n");
	printf("**********************\n");
	printf("********  ");
	if (avail[10] == 1) {
		printf("10  ********\n");
	}
	else {
		printf("  **********\n");
	}
	printf("**    ");
	for (i = 1; i <= 3; i++) {
		if (avail[i] == 1) {
			printf("%d ", i);
		}
		else {
			printf("  ");
		}
		printf("  ");
	}
	printf("  **\n");
	if (avail[11] == 1) {
		printf("11  ");
	}
	else {
		printf("**  ");
	}
	if (turn == 1) {
		if (avail[4] == 1)
			printf("  4   A   ");
		else {
			printf("      A   ");
		}
	}
	else {
		if (avail[4] == 1)
			printf("  4   B   ");
		else {
			printf("      B   ");
		}

	}
	if (avail[6] == 1) {
		printf("6   ");
	}
	else {
		printf("    ");
	}
	if (avail[12] == 1) {
		printf("12**\n");
	}
	else {
		printf("  **\n");
	}
	printf("**    ");
	for (i = 7; i <= 9; i++) {
		if (avail[i] == 1) {
			printf("%d   ", i);
		}
		else {
			printf("    ");
		}
	}
	printf("  **\n********  ");
	if (avail[13] == 1) {
		printf("13  ********\n");
	}
	else {
		printf("    ********\n");
	}
	printf("**********************\n");
	i = 0;
	while (1) {
		printf("Choose where you want to go between ");
		for (i = 0; i<14; i++) {
			if (avail[i] == 1) {
				printf("%d ", i);
			}
		}
		scanf("%d", &command);
		if (command<0 || command>13) {
			printf("Invalid way!!\n");
		}
		else if (avail[command] == 0) {
			printf("Not in the available list!!\n");
		}
		else {
			break;
		}
	}
	return command;
}
void AI_find_valid_movement(Pawn temp_A, Pawn temp_B, int x, int y, int input, int * valid, int*** ai_row_wall, int***ai_col_wall, int cur) {
	int avail[14] = { 0 };
	if (input && x>=0 && x<=7 && y>=0 && y<=7) { // for temp_A
				 /*     10
				 1  2  3
				 11 4  p  6 12
				 7  8  9
				 13
				 */
		if (x == 0) {  /* 4 p 6
					   7 8 9
					   */
			avail[4] = avail[6] = avail[8] = 1;
			if (y == 0) {
				avail[4] = 0;
				if (ai_row_wall[cur][0][0] == 1) {
					avail[8] = 0;
				}
				if (ai_col_wall[cur][0][0] == 1) {
					avail[6] = 0;
				}
				if (temp_B.x == 1 && temp_B.y == y) {
					avail[8] = 0;
					avail[13] = 1;
				}
				if (ai_row_wall[cur][1][0] == 1) {
					if (avail[13] != 0) {
						avail[9] = 1;
					}
				}
			}
			else if (y == 8) {
				avail[6] = 0;
				if (ai_row_wall[cur][0][8] == 1) {
					avail[8] = 0;
				}
				if (ai_col_wall[cur][0][7] == 1) {
					avail[4] = 0;
				}
				if (temp_B.x == 1 && temp_B.y == y) {
					avail[8] = 0;
					avail[13] = 1;
				}
				if (ai_row_wall[cur][1][0] == 1) {
					if (avail[13] != 0) {
						avail[7] = 1;
					}
				}

			}
			else { // temp_A.x==0 , temp_A.y>=1 && temp_A.y<=7
				   /* 4 p 6
				   8
				   */
				if (ai_col_wall[cur][x][y - 1] == 1) { // left wall[*cur]
					avail[4] = 0;
				}
				if (ai_col_wall[cur][x][y] == 1) { // right wall[*cur]
					avail[6] = 0;
				}
				if (ai_row_wall[cur][x][y] == 1) {//  bottom wall[*cur]
					avail[8] = 0;
				}
				if ((temp_B.x == (x)+1) && (temp_B.y == y)) {  // temp_A
															   // temp_B
					avail[8] = 0;
					avail[13] = 1;
					if (ai_row_wall[cur][temp_B.x][temp_B.y] == 1) {
						avail[13] = 0;
						avail[7] = avail[9] = 1;
					}
					if (ai_col_wall[cur][temp_B.x][temp_B.y] == 1) {  //   temp_A
																	   // 7 temp_B|
																	   //   13
						avail[9] = 0;
					}
					if (ai_col_wall[cur][temp_B.x][temp_B.y - 1] == 1) {
						avail[7] = 0;
					}
				}
			}
		}// temp_A.x ==0 clear
		 //temp_A.x >=1  start
		else if (y == 0 && x<=7) { // temp_A.x>=1
			avail[2] = avail[6] = avail[8] = 1;
			if (ai_row_wall[cur][x][y] == 1) {
				avail[8] = 0;
			}
			if (ai_row_wall[cur][x - 1][y] == 1) {
				avail[2] = 0;
			}
			if (ai_col_wall[cur][x][y] == 1) {
				avail[6] = 0;
			}
			if (temp_B.x == x - 1 && temp_B.y == y) { // temp_B
													  // temp_A
				avail[2] = 0;
				avail[10] = 1;
				if (temp_B.x>0 && ai_row_wall[cur][temp_B.x - 1][temp_B.y] == 1) {
					avail[10] = 0;
					avail[3] = 1;
				}
				if (ai_col_wall[cur][temp_B.x][temp_B.y] == 1) {
					avail[3] = 0;
				}
			}
			if (temp_B.x == x + 1 && temp_B.y == y) { //temp_A
													  //temp_B
				avail[8] = 0;
				avail[13] = 1;
				if (temp_B.x <= 7 && ai_row_wall[cur][temp_B.x][temp_B.y] == 1) {
					avail[13] = 0;
					avail[9] = 1;
				}
				if (ai_col_wall[cur][temp_B.x][temp_B.y] == 1) {
					avail[9] = 1;
				}
			}
			if (temp_B.x == x && temp_B.y == y + 1) { // temp_A temp_B
				avail[6] = 0;
				avail[12] = 1;
				if (ai_col_wall[cur][temp_B.x][temp_B.y] == 1) {
					avail[12] = 0;
					avail[3] = 1;
					avail[9] = 1;
				}
				//temp_B.x !=8  since if temp_A.x ==8  then game set
				if (ai_row_wall[cur][temp_B.x][temp_B.y] == 1) {
					avail[9] = 0;
				}
				if (ai_row_wall[cur][temp_B.x - 1][temp_B.y] == 1) {
					avail[3] = 0;
				}
			}
		}
		else if (y == 8 && x<=7) {
			avail[2] = avail[4] = avail[8] = 1;
			if (ai_row_wall[cur][x][y] == 1) {
				avail[8] = 0;
			}
			if (ai_row_wall[cur][x - 1][y] == 1) {
				avail[2] = 0;
			}
			if (ai_col_wall[cur][x][y - 1] == 1) {
				avail[4] = 0;
			}
			if (temp_B.x == x - 1 && temp_B.y == y) { // temp_B
													  // temp_A
				avail[2] = 0;
				avail[10] = 1;
				if (temp_B.x>0 && ai_row_wall[cur][temp_B.x - 1][temp_B.y] == 1) {
					avail[10] = 0;
					avail[1] = 1;
				}
				if (ai_col_wall[cur][temp_B.x][temp_B.y] == 1) {
					avail[1] = 0;
				}
			}
			if (temp_B.x == x + 1 && temp_B.y == y) { //temp_A
													  //temp_B
				avail[8] = 0;
				avail[13] = 1;
				if (temp_B.x <= 7 && ai_row_wall[cur][temp_B.x][temp_B.y] == 1) {
					avail[13] = 0;
					avail[7] = 1;
				}
				if (ai_col_wall[cur][temp_B.x][temp_B.y] == 1) {
					avail[7] = 0;
				}
			}
			if (temp_B.x == x && temp_B.y == y - 1) { // temp_A temp_B
				avail[4] = 0;
				avail[11] = 1;
				if (ai_col_wall[cur][temp_B.x][temp_B.y] == 1) {
					avail[11] = 0;
					avail[1] = 1;
					avail[7] = 1;
				}
				//temp_B.x !=8  since if temp_A.x ==8  then game set
				if (ai_row_wall[cur][temp_B.x][temp_B.y] == 1) {
					avail[7] = 0;
				}
				if (ai_row_wall[cur][temp_B.x - 1][temp_B.y] == 1) {
					avail[1] = 0;
				}
			}

		}
		// temp_A.x >=1  temp_A.y >=1 && <=7
		else if(x<=7) {
			avail[2] = avail[4] = avail[6] = avail[8] = 1;

			if (ai_row_wall[cur][x - 1][y] == 1) {
				avail[2] = 0;
			}
			if (ai_row_wall[cur][x][y] == 1) {
				avail[8] = 0;
			}
			if (ai_col_wall[cur][x][y - 1] == 1) {
				avail[4] = 0;
			}
			if (ai_col_wall[cur][x][y] == 1) {
				avail[6] = 0;
			}
			/// first wall[*cur] clear
			//check for b
			if (temp_B.x == x + 1 && y == temp_B.y) {  // temp_A
													   //  temp_B
				avail[8] = 0;
				avail[13] = 1;
			}
			if (temp_B.x == x - 1 && y == temp_B.y) { // temp_B
													  // temp_A
				avail[2] = 0;
				avail[10] = 1;
			}
			if (temp_B.x == x && temp_B.y == y + 1) { // temp_A  temp_B
				avail[6] = 0;
				avail[12] = 1;
			}
			if (temp_B.x == x && temp_B.y == y - 1) { // temp_B  temp_A
				avail[4] = 0;
				avail[11] = 1;
			}
			// b clear
			// check one more wall[*cur]
			if (x >= 2 && x <= 6) {                        //
				if (ai_row_wall[cur][x - 2][y] == 1) {    //     1  temp_B  3
														   //        temp_A
														   //     7
					if (avail[10] == 1) {
						avail[1] = avail[3] = 1;
						if (ai_col_wall[cur][temp_B.x][temp_B.y] == 1) {
							avail[3] = 0;
						}
						if (ai_col_wall[cur][temp_B.x][temp_B.y - 1] == 1) {
							avail[1] = 0;
						}
					}
					avail[10] = 0;

				}
				if (ai_row_wall[cur][x + 1][y] == 1) {
					if (avail[13] == 1) {
						avail[7] = avail[9] = 1;
						if (ai_col_wall[cur][temp_B.x][temp_B.y] == 1) {
							avail[9] = 0;
						}
						if (ai_col_wall[cur][temp_B.x][temp_B.y - 1] == 1) {
							avail[7] = 0;
						}
					}
					avail[13] = 0;
				}
				if (y >= 2 && y <= 6) {
					if (ai_col_wall[cur][x][y - 2] == 1) {
						if (avail[11] == 1) {
							avail[1] = avail[7] = 1;
							if (ai_row_wall[cur][temp_B.x][temp_B.y] == 1) {
								avail[7] = 0;
							}
							if (ai_row_wall[cur][temp_B.x - 1][temp_B.y] == 1) {
								avail[1] = 0;
							}
						}
						avail[11] = 0;
					}
					if (ai_col_wall[cur][x][y + 1] == 1) {
						if (avail[12] == 1) {
							avail[3] = avail[9] = 1;
							if (ai_row_wall[cur][temp_B.x][temp_B.y] == 1) {
								avail[9] = 0;
							}
							if (ai_row_wall[cur][temp_B.x - 1][temp_B.y] == 1) {
								avail[3] = 0;
							}
						}
						avail[12] = 0;
					}
				}
			}
		}
	}
//	else if (x>=0 && x<=7 && y>=0 && y<=7) { // 
	else{
		if (x == 8) {
			avail[2] = avail[4] = avail[6] = 1;
			if (y == 0) {
				avail[4] = 0;
				if (ai_row_wall[cur][x - 1][y] == 1) {
					avail[2] = 0;
				}
				if (ai_col_wall[cur][x][y] == 1) {
					avail[6] = 0;
				}
				if (temp_A.x == 7 && y == temp_A.y) {
					avail[2] = 0;
					avail[10] = 1;
				}
				if (ai_row_wall[cur][x - 2][y] == 1) {
					if (avail[10] == 1) {
						avail[3] = 1;
					}
				}
			}
			else if (y == 8) {
				avail[6] = 0;
				if (ai_row_wall[cur][x - 1][y] == 1) {
					avail[2] = 0;
				}
				if (ai_col_wall[cur][x][y - 1] == 1) {
					avail[4] = 0;
				}
				if (temp_A.x == 7 && y == temp_A.y) {
					avail[2] = 0;
					avail[10] = 1;
				}
				if (ai_row_wall[cur][x - 2][y] == 1) {
					if (avail[10] == 1) {
						avail[1] = 1;
					}
				}
			}
			else { // temp_B.x==8 temp_B.y>=1 && <=7
				if (ai_col_wall[cur][x][y - 1] == 1) {
					avail[4] = 0;
				}
				if (ai_col_wall[cur][temp_A.x][temp_A.y] == 1) {
					avail[6] = 0;
				}
				if (ai_row_wall[cur][x - 1][y] == 1) {
					avail[2] = 0;
				}
				if (x - 1 == temp_A.x && (y == temp_A.y)) {
					avail[2] = 0;
					avail[10] = 1;
					if (ai_row_wall[cur][temp_A.x - 1][temp_A.y] == 1) {
						avail[10] = 0;
						avail[1] = avail[3] = 1;
					}
					if (ai_col_wall[cur][temp_A.x][temp_A.y] == 1) {
						avail[3] = 0;
					}
					if (ai_col_wall[cur][temp_A.x][temp_A.y - 1] == 1) {
						avail[1] = 0;
					}
				}
			}
		} // temp_B.x ==8 clear
		  //temp_B.x <=7 start
		else if (y == 0&& x>0) {
			avail[2] = avail[6] = avail[8] = 1;
			if (ai_row_wall[cur][x][y] == 1) {
				avail[8] = 0;
			}
			if (ai_row_wall[cur][x - 1][y] == 1) {
				avail[2] = 0;
			}
			if (ai_col_wall[cur][x][y] == 1) {
				avail[6] = 0;
			}
			if (x == temp_A.x + 1 && y == temp_A.y) { // temp_A
													  // temp_B
				avail[2] = 0;
				avail[10] = 1;
				if (temp_A.x>0 && ai_row_wall[cur][temp_A.x - 1][temp_A.y] == 1) {
					avail[10] = 0;
					avail[3] = 1;
				}
				if (ai_col_wall[cur][temp_A.x][temp_A.y] == 1) {
					avail[3] = 0;
				}
			}
			if (x == temp_A.x - 1 && y == temp_A.y) { //temp_B
													  //temp_A
				avail[8] = 0;
				avail[13] = 1;
				if (temp_A.x<7 && ai_row_wall[cur][temp_A.x][y] == 1) {
					avail[13] = 0;
					avail[9] = 1;
				}
				if (ai_col_wall[cur][temp_A.x][temp_A.y] == 1) {
					avail[9] = 0;
				}
			}
			if (x == temp_A.x&& y + 1 == temp_A.y) {  // temp_B  temp_A
				avail[6] = 0;
				avail[12] = 1;
				if (ai_col_wall[cur][temp_A.x][temp_A.y] == 1) {
					avail[12] = 0;
					avail[3] = avail[9] = 1;
				}
				if (ai_row_wall[cur][temp_A.x][temp_A.y] == 1) {
					avail[9] = 0;
				}
				if (ai_row_wall[cur][temp_A.x - 1][temp_A.y] == 1) {
					avail[3] = 0;
				}

			}
		}
		else if (y == 8&& x>0) {
			avail[2] = avail[4] = avail[8] = 1;
			if (ai_row_wall[cur][x][y] == 1) {
				avail[8] = 0;
			}
			if (ai_row_wall[cur][x - 1][y] == 1) {
				avail[2] = 0;
			}
			if (ai_col_wall[cur][x][y - 1] == 1) {
				avail[4] = 0;
			}
			if (x == temp_A.x + 1 && y == temp_A.y) { // temp_A
													  // temp_B
				avail[2] = 0;
				avail[10] = 1;
				if (temp_A.x>0 && ai_row_wall[cur][temp_A.x - 1][temp_A.y] == 1) {
					avail[10] = 0;
					avail[1] = 1;
				}
				if (ai_col_wall[cur][temp_A.x][temp_A.y] == 1) {
					avail[1] = 0;
				}
			}
			if (x == temp_A.x - 1 && y == temp_A.y) { //temp_B
													  //temp_A
				avail[8] = 0;
				avail[13] = 1;
				if (temp_A.x<7 && ai_row_wall[cur][temp_A.x][y] == 1) {
					avail[13] = 0;
					avail[7] = 1;
				}
				if (ai_col_wall[cur][temp_A.x][temp_A.y] == 1) {
					avail[7] = 0;
				}
			}
			if (x == temp_A.x&& y + 1 == temp_A.y) {  // temp_B  temp_A
				avail[6] = 0;
				avail[12] = 1;
				if (ai_col_wall[cur][temp_A.x][temp_A.y - 1] == 1) {
					avail[12] = 0;
					avail[1] = avail[7] = 1;
				}
				if (ai_row_wall[cur][temp_A.x][temp_A.y] == 1) {
					avail[7] = 0;
				}
				if (ai_row_wall[cur][temp_A.x - 1][temp_A.y] == 1) {
					avail[1] = 0;
				}

			}
		}
		//temp_B.x >=1  temp_B.y>=1 && <=7
		else if(x>=1) {
			avail[2] = avail[4] = avail[6] = avail[8] = 1;
			if (ai_row_wall[cur][x - 1][y] == 1) {
				avail[2] = 0;
			}
			if (ai_row_wall[cur][x][y] == 1) {
				avail[8] = 0;
			}
			if (ai_col_wall[cur][x][y - 1] == 1) {
				avail[4] = 0;
			}
			if (ai_col_wall[cur][x][y] == 1) {
				avail[6] = 0;
			}
			//first wall[*cur] clear
			//check for temp_A
			if (x == temp_A.x - 1 && y == temp_A.y) { //temp_B
													  //temp_A
				avail[8] = 0;
				avail[13] = 1;
			}
			if (x == temp_A.x + 1 && y == temp_A.y) {
				avail[2] = 0;
				avail[10] = 1;
			}
			if (x == temp_A.x && y == temp_A.y - 1) { // temp_B  temp_A
				avail[6] = 0;
				avail[12] = 1;
			}
			if (x == temp_A.x && y == temp_A.y + 1) {
				avail[4] = 0;
				avail[11] = 1;
			}
			//clear for check a
			//check one more wall[*cur]
			if (x >= 2 && x <= 6) {
				
				if (ai_row_wall[cur][x - 2][y] == 1) {
					if (avail[10] == 1) {
						avail[1] = avail[3] = 1;
						if (ai_col_wall[cur][temp_A.x][temp_A.y] == 1) {
							avail[3] = 0;
						}
						if (ai_col_wall[cur][temp_A.x][temp_A.y - 1] == 1) {
							avail[1] = 0;
						}
					}
					avail[10] = 0;
				}
				if (ai_row_wall[cur][x + 1][y] == 1) {
					if (avail[13] == 1) {
						avail[7] = avail[9] = 1;
						if (ai_col_wall[cur][temp_A.x][temp_A.y] == 1) {
							avail[9] = 0;
						}
						if (ai_col_wall[cur][temp_A.x][temp_A.y - 1] == 1) {
							avail[7] = 0;
						}
					}
					avail[13] = 0;
				}
				if (y >= 2 && y <= 6) {
					if (ai_col_wall[cur][x][y - 2] == 1) {
						if (avail[11] == 1) {
							avail[1] = avail[7] = 1;
							if (ai_row_wall[cur][temp_A.x][temp_A.y] == 1) {
								avail[7] = 0;
							}
							if (ai_row_wall[cur][temp_A.x - 1][temp_A.y] == 1) {
								avail[1] = 0;
							}
						}
						avail[11] = 0;
					}
					if (ai_col_wall[cur][x][y + 1] == 1) {
						if (avail[12] == 1) {
							avail[3] = avail[9] = 1;
							if (ai_row_wall[cur][temp_A.x][temp_A.y] == 1) {
								avail[9] = 0;
							}
							if (ai_row_wall[cur][temp_A.x - 1][temp_A.y] == 1) {
								avail[3] = 0;
							}
						}
						avail[12] = 0;
					}
					
				}
			}
		}
	}

	int i = 0;
	for (i = 0; i<14; i++) {
		valid[i] = avail[i];
	}
}
int AI_find_shortest_path(int x, int y, int input, int*** ai_col_wall, int***ai_row_wall, int cur) {
	//check in grid
	//check there is a wall
	//check there is a pawn
	int i = 0;
	int j = 0;
	for (i = 0; i<queuenum; i++) {
		Queue[i].x = 0;
		Queue[i].y = 0;
		for (j = 0; j<14; j++) {
			Queue[i].valid[j] = 0;
		}
	}
	queuenum = 0;
	double grid[9][9] = { 0.0 };
	int length = 0;
	for (i = 0; i<9; i++) {
		for (j = 0; j<9; j++) {
			grid[i][j] = 0;
		}
	}
	//initializing the queue
	Pawn temp;
	if (input) {
		// priority : 13
		//            7==9==8
		//            11==12
		//            4==6==8
		//            2
		//            1==3
		//            10
		if (x >= 0 && y >= 0) {
			grid[x][y] = 1;
		}
		AI_find_valid_movement(A, B, x, y, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);// insert root
		Queue[0].x = x;
		Queue[0].y = y;
		Queue[0].generation = 1;
		queuenum++;
		//dequeue
		while (1) {
			temp = Queue[0];
			queuenum--;
			if (temp.x >= 0 && temp.y >= 0) {
				length++;
				grid[temp.x][temp.y] = 1;
				x = temp.x;
				y = temp.y;
				for (i = 0; i < queuenum ; i++) {
					Queue[i] = Queue[i + 1];
				}
				
				if (temp.x == 8) {
					break;
				}
				if (temp.valid[13] == 1) {
					if (x + 2 <= 8) {
						if (grid[x + 2][y] != 1) {
							grid[x + 2][y] = 1;
							AI_find_valid_movement(A, B, x + 2, y, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
							Queue[queuenum].x = x + 2;
							Queue[queuenum].y = y;
							Queue[queuenum].generation = temp.generation + 1;
							queuenum++;
						}
					}
				}
				if (temp.valid[7] == 1) {
					if (grid[x + 1][y - 1] != 1) {
						grid[x + 1][y - 1] = 0;
						AI_find_valid_movement(A, B, x + 1, y - 1, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x + 1;
						Queue[queuenum].y = y - 1;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[9] == 1) {
					if (grid[x + 1][y + 1] == 0) {
						grid[x + 1][y + 1] = 1;
						AI_find_valid_movement(A, B, x + 1, y + 1, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x + 1;
						Queue[queuenum].y = y + 1;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[8] == 1) {
					if (grid[x + 1][y] == 0) {
						grid[x + 1][y] = 1;
						AI_find_valid_movement(A, B, x + 1, y, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x + 1;
						Queue[queuenum].y = y;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[12] == 1) {
					if (y + 2 <= 8) {
						if (grid[x][y + 2] != 1) {
							grid[x][y + 2] = 1;
							AI_find_valid_movement(A, B, x, y + 2, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
							Queue[queuenum].x = x;
							Queue[queuenum].y = y + 2;
							Queue[queuenum].generation = temp.generation + 1;
							queuenum++;
						}
					}
				}
				if (temp.valid[11] == 1) {
					if (y - 2 > 0) {
						if (grid[x][y - 2] != 1) {
							AI_find_valid_movement(A, B, x, y - 2, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
							Queue[queuenum].x = x;
							Queue[queuenum].y = y - 2;
							grid[x][y - 2] = 1;
							Queue[queuenum].generation = temp.generation + 1;
							queuenum++;
						}
					}
				}
				if (temp.valid[4] == 1) {
					if (grid[x][y - 1] == 0) {
						grid[x][y - 1] = 1;
						AI_find_valid_movement(A, B, x, y - 1, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x;
						Queue[queuenum].y = y - 1;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[6] == 1) {
					if (grid[x][y + 1] == 0) {
						grid[x][y + 1] = 1.0;
						AI_find_valid_movement(A, B, x, y + 1, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x;
						Queue[queuenum].y = y + 1;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[2] == 1) {
					if (grid[x - 1][y] == 0) {
						grid[x - 1][y] = 1.0;
						AI_find_valid_movement(A, B, x - 1, y, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x - 1;
						Queue[queuenum].y = y;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[1] == 1) {
					if (grid[x - 1][y - 1] == 0) {
						grid[x - 1][y - 1] = 1.0;
						AI_find_valid_movement(A, B, x - 1, y - 1, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x - 1;
						Queue[queuenum].y = y - 1;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[3] == 1) {
					if (grid[x - 1][y + 1] == 0) {
						grid[x - 1][y + 1] = 1.0;
						AI_find_valid_movement(A, B, x - 1, y + 1, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x - 1;
						Queue[queuenum].y = y + 1;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[10] == 1) {
					if (x - 2 > 0) {
						if (grid[x - 2][y] == 0) {
							grid[x - 2][y] = 1.0;
							AI_find_valid_movement(A, B, x - 2, y, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
							Queue[queuenum].x = x - 2;
							Queue[queuenum].y = y;
							Queue[queuenum].generation = temp.generation + 1;
							queuenum++;
						}
					}
				}
				length = temp.generation;
			}
			
			if (queuenum == 0) {
				length = -1;
				break;
			}
			
		}
	}
	else {
		//priority : 10
		//           1==3==2
		//           11==12
		//           4==6
		//           8
		//           7==9
		//           13
		if (x >= 0 && y >= 0) {
			grid[x][y] = 1.0;
		}
		queuenum=0;
		AI_find_valid_movement(A, B, x, y, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
		queuenum = 1;
		Queue[0].x = x;
		Queue[0].y = y;
		Queue[0].generation = 0;
		int q = 0;
		//dequeue
		int checkpoint[8][8] = { 0 };
		while (1) {
			temp = Queue[0];
			queuenum--;
			if (x >= 0 && y >= 0) {

				length++;
				grid[temp.x][temp.y] = 1.0;
				x = temp.x;
				y = temp.y;
				for (i = 0; i < queuenum ; i++) {
					Queue[i] = Queue[i + 1];
				}
				for (i = 0; i < 8; i++) {
					for (j = 0; j < 8; j++) {
						if (checkpoint[i][j] == 0) {
							if (i == x && j == y) {
								checkpoint[i][j] = temp.generation;
							}
						}
					}
				}
				

				if (temp.x == 0) {
					break;
				}
				if (temp.valid[10] == 1) {
					if (x - 2 > 0) {
						if (grid[x - 2][y] != 1) {
							grid[x - 2][y] = 1.0;
							AI_find_valid_movement(A, B, x - 2, y, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
							Queue[queuenum].x = x - 2;
							Queue[queuenum].y = y;
							Queue[queuenum].generation = temp.generation + 1;
							queuenum++;
						}
					}
				}
				if (temp.valid[1] == 1) {
					if (grid[x - 1][y - 1] != 1) {
						grid[x - 1][y - 1] = 1.0;
						AI_find_valid_movement(A, B, x - 1, y + 1, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x - 1;
						Queue[queuenum].y = y - 1;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[3] == 1) {
					if (grid[x - 1][y + 1] == 0) {
						grid[x - 1][y + 1] = 1.0;
						AI_find_valid_movement(A, B, x - 1, y + 1, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x - 1;
						Queue[queuenum].y = y + 1;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[2] == 1) {
					if (grid[x - 1][y] == 0) {
						grid[x - 1][y] = 1.0;
						AI_find_valid_movement(A, B, x - 1, y, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x - 1;
						Queue[queuenum].y = y;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[12] == 1) {
					if (y + 2 <= 8) {
						if (grid[x][y + 2] != 1) {
							grid[x][y + 2] = 1.0;
							AI_find_valid_movement(A, B, x, y + 2, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
							Queue[queuenum].x = x;
							Queue[queuenum].y = y + 2;
							Queue[queuenum].generation = temp.generation + 1;
							queuenum++;
						}
					}

				}
				if (temp.valid[11] == 1) {
					if (y - 2 > 0) {
						if (grid[x][y - 2] != 1) {
							AI_find_valid_movement(A, B, x, y - 2, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
							Queue[queuenum].x = x;
							Queue[queuenum].y = y - 2;
							grid[x][y - 2] = 1.0;
							Queue[queuenum].generation = temp.generation + 1;
							queuenum++;
						}
					}
				}
				if (temp.valid[4] == 1) {
					if (grid[x][y - 1] == 0) {
						grid[x][y - 1] = 1.0;
						AI_find_valid_movement(A, B, x, y - 1, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x;
						Queue[queuenum].y = y - 1;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[6] == 1) {
					if (grid[x][y + 1] == 0) {
						grid[x][y + 1] = 1.0;
						AI_find_valid_movement(A, B, x, y + 1, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x;
						Queue[queuenum].y = y + 1;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[8] == 1) {
					if (grid[x + 1][y] == 0) {
						grid[x + 1][y] = 1.0;
						AI_find_valid_movement(A, B, x + 1, y, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x + 1;
						Queue[queuenum].y = y;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[7] == 1) {
					if (grid[x + 1][y - 1] == 0) {
						grid[x + 1][y - 1] = 1.0;
						AI_find_valid_movement(A, B, x + 1, y - 1, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x + 1;
						Queue[queuenum].y = y - 1;
						Queue[queuenum].generation = temp.generation + 1;
						queuenum++;
					}
				}
				if (temp.valid[9] == 1) {
					if (grid[x + 1][y + 1] == 0) {
						grid[x + 1][y + 1] = 1.0;
						AI_find_valid_movement(A, B, x + 1, y + 1, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
						Queue[queuenum].x = x + 1;
						Queue[queuenum].y = y + 1;
						Queue[queuenum].generation = temp.generation + 1;
					}
				}
				if (temp.valid[13] == 1) {
					if (x + 2 <= 8) {
						if (grid[x + 2][y] == 0) {
							grid[x + 2][y] = 1.0;
							AI_find_valid_movement(A, B, x + 2, y, input, Queue[queuenum].valid, ai_row_wall, ai_col_wall, cur);
							Queue[queuenum].x = x + 2;
							Queue[queuenum].generation = temp.generation + 1;
							Queue[queuenum].y = y;
							queuenum++;
						}
					}
				}
				length = temp.generation + 1;	
			}
			if (queuenum == 0) {
				break;
			}
		}
	}
	return length;
}
void ai_helper() {
	Minmax* newnode;
	Minmax* cur;
	free(MinHead);
	int flag=-11;
	newnode = (Minmax*)malloc(sizeof(Minmax));
	MinHead = newnode;
	newnode->parent = MinHead;
	newnode->a = A;
	newnode->b = B;
	newnode->b.wall = B.wall;
	newnode->a.wall = A.wall;
	find_valid_movement(B.x,B.y,0,B.valid);
	for(int i=0;i<14;i++){
		newnode->a.valid[i]=0;
		newnode->b.valid[i]=0;
		if(A.valid[i]==1){
			newnode->a.valid[i]=1;
		}
		if(B.valid[i]==1){
			newnode->b.valid[i]=1;
		}
	}
	newnode->generation =1;
	int i=0;
	int j=0;
	for(i=0;i<9;i++){
		for(j=0;j<8;j++){
			newnode->m_col_wall[i][j] = col_wall[i][j];
		}
	}
	for(i=0;i<8;i++){
		for(j=0;j<9;j++){
			newnode->m_row_wall[i][j] = row_wall[i][j];
		}
	}
	ai_max(newnode,flag);
	if(newnode->wall==0){
		// move
		B.x = newnode->b.x;
		B.y = newnode->b.y;
	}
	else{
		B.wall--;
		for(i=0;i<9;i++){
			for(j=0;j<8;j++){
				col_wall[i][j] = newnode->m_col_wall[i][j];
				row_wall[j][i] = newnode->m_row_wall[j][i];
			}

		}
	}


}
void ai_min(Minmax* minmax,int flag){
	if(flag!=-11){
		return ;
	}
	Pawn temp_A = minmax->a;
	Pawn temp_B = minmax->b;
	for(int i=0;i<14;i++){
		temp_A.valid[i]=0;
		temp_B.valid[i]=0;
		if(minmax->a.valid[i]==1){
			temp_A.valid[i]=1;
		}
		if(minmax->b.valid[i]==1){
			temp_B.valid[i]=1;
		}
	}
	int*** ai_col_wall;
	int*** ai_row_wall;
	int i=0;
	int j=0;
	int k=0;
	int short_a;
	int short_b;
	ai_col_wall = (int***)malloc(sizeof(int**)*10000);
	ai_row_wall = (int***)malloc(sizeof(int**)*10000);
	for(i=0;i<10000;i++){
		ai_col_wall[i] = (int**)malloc(sizeof(int*)*9);
		ai_row_wall[i] = (int**)malloc(sizeof(int*)*8);
	}
	for(i=0;i<10000;i++){
		for(j=0;j<9;j++){
			ai_col_wall[i][j] = (int*)malloc(sizeof(int)*8);
		}
		for(j=0;j<8;j++){
			ai_row_wall[i][j] = (int*)malloc(sizeof(int)*9);
		}
	}
	for(i=0;i<8;i++){
		for(j=0;j<9;j++){
			ai_col_wall[0][j][i] = col_wall[j][i];
			ai_row_wall[0][i][j] = row_wall[i][j];
		}
	}
	// find shortest path of A in current A position
	int temp=100000;
	int shortest=100000;
	int current_short_a =0;
	int current_short_b=0;
	int move_a =0;
	int wall_col_i=0;
	int wall_col_j=0;
	int wall_row_i=0;
	int wall_row_j=0;
	int wall_col=-1;
	for(i=0;i<14;i++){
		if(temp_A.valid[i]==1){
			switch(i){
				case 1: temp = AI_find_shortest_path(temp_A.x-1,temp_A.y-1,1,ai_col_wall,ai_row_wall,0);break;
				case 2: temp = AI_find_shortest_path(temp_A.x-1,temp_A.y,1,ai_col_wall,ai_row_wall,0);break;
				case 3: temp = AI_find_shortest_path(temp_A.x-1,temp_A.y+1,1,ai_col_wall,ai_row_wall,0);break;
				case 4: temp = AI_find_shortest_path(temp_A.x,temp_A.y-1,1,ai_col_wall,ai_row_wall,0);break;
				case 6: temp = AI_find_shortest_path(temp_A.x,temp_A.y+1,1,ai_col_wall,ai_row_wall,0);break;
				case 7: temp = AI_find_shortest_path(temp_A.x+1,temp_A.y-1,1,ai_col_wall,ai_row_wall,0);break;
				case 8: temp = AI_find_shortest_path(temp_A.x+1,temp_A.y,1,ai_col_wall,ai_row_wall,0);break;
				case 9: temp = AI_find_shortest_path(temp_A.x+1,temp_A.y+1,1,ai_col_wall,ai_row_wall,0);break;
				case 10: temp = AI_find_shortest_path(temp_A.x-2,temp_A.y,1,ai_col_wall,ai_row_wall,0);break;
				case 11: temp = AI_find_shortest_path(temp_A.x,temp_A.y-2,1,ai_col_wall,ai_row_wall,0);break;
				case 12: temp = AI_find_shortest_path(temp_A.x,temp_A.y+2,1,ai_col_wall,ai_row_wall,0);break;
				case 13: temp = AI_find_shortest_path(temp_A.x+2,temp_A.y,1,ai_col_wall,ai_row_wall,0);break;
				default : break;
			}
		}
		if(shortest > temp){
			shortest = temp;
			move_a = i;
		}
	}
	current_short_a = shortest;
		int valid=0;
		int t=0;
		int col_valid=0;
		shortest=100000;
	if(temp_A.wall!=0){
		for(i=0;i<8;i++){ // make a col_wall
			for(j=0;j<8;j++){
				if(col_wall[i][j]==0 && col_wall[i+1][j]==0){
					if(row_wall[i][j]==0||row_wall[i][j+1]==0){
						valid++;	
						for(t=0;t<9;t++){
							for(k=0;k<8;k++){
								ai_col_wall[valid][t][k]  = col_wall[t][k];
							}
						}
						for(t=0;t<8;t++){
							for(k=0;k<9;k++){
								ai_row_wall[valid][t][k]  = row_wall[t][k];
							}
						}
						ai_col_wall[valid][i][j] =1;
						ai_col_wall[valid][i+1][j] =1;
						temp = AI_find_shortest_path(temp_B.x,temp_B.y,0,ai_col_wall,ai_row_wall,valid);
					}
				}
				if(shortest>temp){
					shortest = temp;
					col_valid = valid;
				}
			}
		}
		int shortest2=100000;
		int row_valid=0;
		for(i=0;i<8;i++){ // make a col_wall
			for(j=0;j<8;j++){
				if(row_wall[i][j]==0 && row_wall[i][j+1]==0){
					if(col_wall[i][j]==0||row_wall[i+1][j]==0){
						valid++;	
						for(t=0;t<9;t++){
							for(k=0;k<8;k++){
								ai_col_wall[valid][t][k]  = col_wall[t][k];
							}
						}
						for(t=0;t<8;t++){
							for(k=0;k<9;k++){
									ai_row_wall[valid][t][k]  = row_wall[t][k];
							}
						}
						ai_row_wall[valid][i][j] =1;
						ai_row_wall[valid][i][j+1] =1;
						temp = AI_find_shortest_path(temp_B.x,temp_B.y,0,ai_col_wall,ai_row_wall,valid);
					}
				}
				if(shortest2>temp){
					shortest2 = temp;
					row_valid = valid;
				}
			}
		}
		if(shortest<shortest2){ // use col wall
			wall_col =1;
		}
		else{
			shortest = shortest2;
			wall_col=0;
		}
		current_short_b = shortest;

		short_b = AI_find_shortest_path(temp_B.x,temp_B.y,0,ai_col_wall,ai_row_wall,0);
		short_a = AI_find_shortest_path(temp_A.x,temp_A.y,1,ai_col_wall,ai_row_wall,0);
		if(short_b-current_short_a >= current_short_b-short_a){ // move a
			for(i=0;i<9;i++){
				for(j=0;j<8;j++){
					minmax->m_col_wall[i][j] = col_wall[i][j];
					minmax->m_row_wall[j][i] = row_wall[j][i];
				}
			}
			switch(move_a){
				case 1: minmax->a.x--; minmax->a.y--; break;
				case 2: minmax->a.x--; minmax->a.y; break;
				case 3: minmax->a.x--; minmax->a.y++; break;
				case 4: minmax->a.x; minmax->a.y--; break;
				case 6: minmax->a.x; minmax->a.y++; break;
				case 7: minmax->a.x++; minmax->a.y--; break;
				case 8: minmax->a.x++; minmax->a.y; break;
				case 9: minmax->a.x++; minmax->a.y++; break;
				case 10: minmax->a.x--;minmax->a.x--; minmax->a.y; break;
				case 11: minmax->a.x; minmax->a.y--;minmax->a.y--; break;
				case 12: minmax->a.x; minmax->a.y++;minmax->a.y++; break;
				case 13: minmax->a.x++;minmax->a.x++; minmax->a.y; break;
			}
		}
		else{//make a wall
			minmax->wall=1;
			for(i=0;i<9;i++){
				for(j=0;j<8;j++){
					if(wall_col ==1){
						minmax->m_col_wall[i][j] = ai_col_wall[col_valid][i][j];
						minmax->m_row_wall[j][i] = ai_row_wall[col_valid][j][i];
					}
					else{
						minmax->m_col_wall[i][j] = ai_col_wall[row_valid][i][j];
						minmax->m_row_wall[j][i] = ai_row_wall[row_valid][j][i];
					}
				}
			}
		}
	}
	else{
		for(i=0;i<9;i++){
			for(j=0;j<8;j++){
				minmax->m_col_wall[i][j] = col_wall[i][j];
				minmax->m_row_wall[j][i] = row_wall[j][i];
			}
		}
		switch(move_a){
			case 1: minmax->a.x--; minmax->a.y--; break;
			case 2: minmax->a.x--; minmax->a.y; break;
			case 3: minmax->a.x--; minmax->a.y++; break;
			case 4: minmax->a.x; minmax->a.y--; break;
			case 6: minmax->a.x; minmax->a.y++; break;
			case 7: minmax->a.x++; minmax->a.y--; break;
			case 8: minmax->a.x++; minmax->a.y; break;
			case 9: minmax->a.x++; minmax->a.y++; break;
			case 10: minmax->a.x--;minmax->a.x--; minmax->a.y; break;
			case 11: minmax->a.x; minmax->a.y--;minmax->a.y--; break;
			case 12: minmax->a.x; minmax->a.y++;minmax->a.y++; break;
			case 13: minmax->a.x++;minmax->a.x++; minmax->a.y; break;
		}
	}
	Minmax* newnode;
	newnode = (Minmax*)malloc(sizeof(Minmax));
	newnode->a = minmax->a;
	if(newnode->a.x==8&& flag==-11){
		printf("Hmm... I think you could win\n");
		flag=100;
	}
	newnode->b = minmax->b;
	newnode->parent = minmax;
	minmax->child = newnode;
	newnode->generation = minmax->generation+1;
	for(i=0;i<8;i++){
		for(j=0;j<9;j++){
			newnode->m_col_wall[j][i] = minmax->m_col_wall[j][i];
			newnode->m_row_wall[i][j] = minmax->m_row_wall[i][j];
		}
	}
	for (i = 0; i<8; i++) {
		for (j = 0; j<9; j++) {
			newnode->m_col_wall[j][i] = minmax->m_col_wall[j][i];
			newnode->m_row_wall[i][j] = minmax->m_row_wall[i][j];
		}
	}
	for (i = 0; i < 10000; i++) {
		for (j = 0; j < 8; j++) {
			free(ai_col_wall[i][j]);
			free(ai_row_wall[i][j]);
		}
	}
	for (i = 0; i < 10000; i++) {
		free(ai_col_wall[i]);
		free(ai_row_wall[i]);
	}
	free(ai_col_wall);
	free(ai_row_wall);
	if(newnode->generation >=26){
		return ;
	}
	else{
		ai_max(newnode,flag);
	}
}
void ai_max(Minmax* minmax,int flag){
	if(flag!=-11){
		return ;
	}
	Pawn temp_A = minmax->a;
	Pawn temp_B = minmax->b;
	for(int i=0;i<14;i++){
		if(minmax->a.valid[i]==1){
			temp_A.valid[i]=1;
		}
		if(minmax->b.valid[i]==1){
			temp_B.valid[i]=1;
		}
	}
	int *** ai_col_wall;
	int *** ai_row_wall;
	int i=0;
	int j=0;
	int k=0;
	int short_a;
	int short_b;
	ai_col_wall=(int***)malloc(sizeof(int**)*10000);
	ai_row_wall=(int***)malloc(sizeof(int**)*10000);
	for(i=0;i<10000;i++){
		ai_col_wall[i] = (int**)malloc(sizeof(int*)*9);
		ai_row_wall[i] = (int**)malloc(sizeof(int*)*8);
	}
	for(i=0;i<10000;i++){
		for(j=0;j<9;j++){
			ai_col_wall[i][j]= (int*)malloc(sizeof(int)*8);
		}
		for(j=0;j<8;j++){
			ai_row_wall[i][j] =(int*)malloc(sizeof(int)*9);
		}
	}
	for (int k = 0; k < 10000; k++) {
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 9; j++) {
				ai_col_wall[k][j][i] = 0;
				ai_row_wall[k][i][j] = 0;

			}
		}
	}
	for(i=0;i<8;i++){
		for(j=0;j<9;j++){
			ai_col_wall[0][j][i] = minmax->m_col_wall[j][i];
			ai_row_wall[0][i][j] = minmax->m_row_wall[i][j];
		}
	}
	
//	short_b = AI_find_shortest_path(temp_B.x,temp_B.y,0,ai_col_wall,ai_row_wall,0);
	short_a = AI_find_shortest_path(temp_A.x,temp_A.y,1,ai_col_wall,ai_row_wall,0);
	int temp=10000000;
	int shortest = 100000;
	int shortest2 = 0;
	int current_short_a=0;
	int current_short_b=0;
	int move_b=0;
	int wall_col_i=0;
	int wall_col_j=0;
	int wall_row_i=0;
	int wall_row_j=0;
	int wall_col=-1;
	for(i=0;i<14;i++){
		if(temp_B.valid[i]==1){
			switch(i){
				case 1: temp = AI_find_shortest_path(temp_B.x-1,temp_B.y-1,0,ai_col_wall,ai_row_wall,0);
						break;
				case 2: temp = AI_find_shortest_path(temp_B.x-1,temp_B.y,0,ai_col_wall,ai_row_wall,0);
						break;
				case 3: temp = AI_find_shortest_path(temp_B.x-1,temp_B.y+1,0,ai_col_wall,ai_row_wall,0);
						break;
				case 4: temp = AI_find_shortest_path(temp_B.x,temp_B.y-1,0,ai_col_wall,ai_row_wall,0);
						break;
				case 6: temp = AI_find_shortest_path(temp_B.x,temp_B.y+1,0,ai_col_wall,ai_row_wall,0);
						break;
				case 7: temp = AI_find_shortest_path(temp_B.x+1,temp_B.y-1,0,ai_col_wall,ai_row_wall,0);
						break;
				case 8: temp = AI_find_shortest_path(temp_B.x+1,temp_B.y,0,ai_col_wall,ai_row_wall,0);
						break;
				case 9: temp = AI_find_shortest_path(temp_B.x+1,temp_B.y+1,0,ai_col_wall,ai_row_wall,0);
						break;
				case 10: temp = AI_find_shortest_path(temp_B.x-2,temp_B.y,0,ai_col_wall,ai_row_wall,0);
						 break;
				case 11: temp = AI_find_shortest_path(temp_B.x,temp_B.y-2,0,ai_col_wall,ai_row_wall,0);
						 break;
				case 12: temp = AI_find_shortest_path(temp_B.x,temp_B.y+2,0,ai_col_wall,ai_row_wall,0);
						 break;
				case 13: temp = AI_find_shortest_path(temp_B.x+2,temp_B.y,0,ai_col_wall,ai_row_wall,0);
						 break;
				default: break;
			}
		}
		if(shortest>temp){
			shortest = temp;
			move_b = i;
			//printf("move_b %d\n", move_b);
		}
	}
	current_short_b = shortest;
	int valid=0;
	int t=0;
	int col_valid=0;
	int row_valid=0;

	int check_B=0;
	shortest =0;
	int shortest_3 = 0;
	int temp2 = 0;
	if(temp_B.wall>0){
		for(i=0;i<8;i++){ // check col wall
			for(j=0;j<8;j++){
				if(col_wall[i][j]==0 && col_wall[i+1][j]==0){
					if(row_wall[i][j]==0 || row_wall[i][j+1]==0){
						valid++;
						for(t=0;t<9;t++){
							for(k=0;k<8;k++){
								ai_col_wall[valid][t][k] = col_wall[t][k];
							}
						}
						for(t=0;t<8;t++){
							for(k=0;k<9;k++){
								ai_row_wall[valid][t][k] = row_wall[t][k];
							}
						}
						ai_col_wall[valid][i][j] = 1;
						ai_col_wall[valid][i+1][j] =1;
						temp = AI_find_shortest_path(temp_A.x,temp_A.y,1,ai_col_wall,ai_row_wall,valid);
						temp2 = AI_find_shortest_path(temp_B.x,temp_B.y,0,ai_col_wall,ai_row_wall,valid);
						//temp = temp-temp2;
						//printf("temp in col : %d %d\n", temp,valid);
						if (shortest<temp && temp<1000) {
							shortest = temp;
							shortest_3 = temp2;
							col_valid = valid;
							//printf("col_valid  for : %d\n", col_valid);
						}
						if(temp2==-1){
							shortest_3 = -1;
						}
					}
				}
				
			}
		}
		//printf("col_wall %d\n",col_valid);
	//	for (i = 0; i < 8; i++) {
	//		for (j = 0; j < 9; j++) {
	//		//	printf("%d   ", ai_col_wall[col_valid][j][i]);
	//		}
	//		printf("\n");
	//	}
	//	printf("\n");
		int shortest_4 = 0;
		for(i=0;i<8;i++){ // make a row_wall
			for(j=0;j<8;j++){
				if(row_wall[i][j]==0 && row_wall[i][j+1]==0){
					if(col_wall[i][j]==0||row_wall[i+1][j]==0){
						valid++;	
						for(t=0;t<9;t++){
							for(k=0;k<8;k++){
								ai_col_wall[valid][t][k]  = col_wall[t][k];
							}
						}
						for(t=0;t<8;t++){
							for(k=0;k<9;k++){
									ai_row_wall[valid][t][k]  = row_wall[t][k];
							}
						}
						ai_row_wall[valid][i][j] =1;
						ai_row_wall[valid][i][j+1] =1;
						temp = AI_find_shortest_path(temp_A.x,temp_A.y,1,ai_col_wall,ai_row_wall,valid);
						temp2 = AI_find_shortest_path(temp_B.x,temp_B.y,0,ai_col_wall,ai_row_wall,valid);
						//temp = temp - temp2;
					}
					if(shortest2<temp && temp<1000){
						shortest2 = temp;
						shortest_4 = temp2;
						row_valid = valid;
					}
					if(temp2==-1){
						shortest_4= temp2;
					}
				}
			}
		}
	//	for (i = 0; i < 8; i++) {
	//		for (j = 0; j < 9; j++) {
	//			printf("%d   ", ai_row_wall[row_valid][i][j]);
	//		}
	//		printf("\n");
	//	}
	//	printf("\n");
		if(shortest>shortest2){ // use col wall
			wall_col =1;
		}
		else{
			shortest = shortest2;
			wall_col=0;
		}
		current_short_a = shortest;
		if((short_a-current_short_b >=  current_short_a) || short_a==-1||shortest_3==-1||shortest_4==-1){ // move a
			//printf("move\n", move_b);
			minmax->wall = 0;
			for(i=0;i<9;i++){
				for(j=0;j<8;j++){
					minmax->m_col_wall[i][j] = col_wall[i][j];
					minmax->m_row_wall[j][i] = row_wall[j][i];
				}
			}
			switch(move_b){
				case 1: minmax->b.x--; minmax->b.y--; break;
				case 2: minmax->b.x--; minmax->b.y; break;
				case 3: minmax->b.x--; minmax->b.y++; break;
				case 4: minmax->b.x; minmax->b.y--; break;
				case 6: minmax->b.x; minmax->b.y++; break;
				case 7: minmax->b.x++; minmax->b.y--; break;
				case 8: minmax->b.x++; minmax->b.y; break;
				case 9: minmax->b.x++; minmax->b.y++; break;
				case 10: minmax->b.x--;minmax->b.x--; minmax->b.y; break;
				case 11: minmax->b.x; minmax->b.y--;minmax->b.y--; break;
				case 12: minmax->b.x; minmax->b.y++;minmax->b.y++; break;
				case 13: minmax->b.x++;minmax->b.x++; minmax->b.y; break;
			}
		}
		else{//make a wall
			minmax->wall=1;
			//printf("wall_col : %d\n", wall_col);
			for(i=0;i<9;i++){
				for(j=0;j<8;j++){
					if(wall_col ==1){
						minmax->m_col_wall[i][j] = ai_col_wall[col_valid][i][j];
						minmax->m_row_wall[j][i] = ai_row_wall[col_valid][j][i];
						
					}
					else{
						minmax->m_col_wall[i][j] = ai_col_wall[row_valid][i][j];
						minmax->m_row_wall[j][i] = ai_row_wall[row_valid][j][i];
					}
				}
			}
		}
	}
	else{ // just move
		minmax->wall = 0;
		for(i=0;i<9;i++){
			for(j=0;j<8;j++){
				minmax->m_col_wall[i][j] = col_wall[i][j];
				minmax->m_row_wall[j][i] = row_wall[j][i];
			}
		}
		switch(move_b){
			case 1: minmax->b.x--; minmax->b.y--; break;
			case 2: minmax->b.x--; minmax->b.y; break;
			case 3: minmax->b.x--; minmax->b.y++; break;
			case 4: minmax->b.x; minmax->b.y--; break;
			case 6: minmax->b.x; minmax->b.y++; break;
			case 7: minmax->b.x++; minmax->b.y--; break;
			case 8: minmax->b.x++; minmax->b.y; break;
			case 9: minmax->b.x++; minmax->b.y++; break;
			case 10: minmax->b.x--;minmax->b.x--; minmax->b.y; break;
			case 11: minmax->b.x; minmax->b.y--;minmax->b.y--; break;
			case 12: minmax->b.x; minmax->b.y++;minmax->b.y++; break;
			case 13: minmax->b.x++;minmax->b.x++; minmax->b.y; break;
		}
	}
	Minmax* newnode;
	newnode = (Minmax*)malloc(sizeof(Minmax));
	newnode->a = minmax->a;
	newnode->b = minmax->b;
	if(flag==-11&& newnode->b.x == 0){
		printf("I think I would win\n");
		flag = 100;
	}
	newnode->parent = minmax;
	minmax ->child = newnode;
	newnode->generation = minmax->generation+1;
	for(i=0;i<8;i++){
		for(j=0;j<9;j++){
			newnode->m_col_wall[j][i] = minmax->m_col_wall[j][i];
			newnode->m_row_wall[i][j] = minmax->m_row_wall[i][j];
		}
	}
	for (i = 0; i < 10000; i++) {
		for (j = 0; j < 8; j++) {
				free(ai_col_wall[i][j]);
				free(ai_row_wall[i][j]);
		}
	}
	for (i = 0; i < 10000; i++) {
		free(ai_col_wall[i]);
		free(ai_row_wall[i]);
	}
	free(ai_col_wall);
	free(ai_row_wall);
	ai_min(newnode,flag);
}



int main(void) {
	A.x = 0;
	B.x = 8;
	A.y = 4;
	B.y = 4;
	A.wall = 10;
	B.wall = 10;
	int i = 0;
	int j = 0;
	int winner = 0;
	int command = 0;
	while (winner == 0) {
		turn = 1;
		print_grid();
		command = get_command(1);
		if (command == 1) {
			command = print_valid_movement(1);
			move_pawn(command, 1);
		}
		else if (command == 2) {
			show_how_to_build_wall();
			print_valid_wall();
		}
		winner = check_winner();
		if (winner != 0) {
			break;
		}
		print_grid();
		turn = 0;
		ai_helper();

		winner = check_winner();
		if (winner != 0) {
			break;
		}
	}
	return 0;
}

