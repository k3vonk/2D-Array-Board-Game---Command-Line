#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define BOARD_SIZE 7
#define AREA 4

enum value{ false, true };

enum class{Elf, Human, Ogre, Wizard};
enum terrain{Level_Ground, Hill, City}; //Terrain to choose from

struct player{
	char name[40];
	enum class type;
	int life_points;
	int smartness, strength, magic_skills, luck, dexterity;
	int action;
	struct slot *place;
};

struct slot{
	int row;
	int column;
	enum terrain type;
	enum value log;
	struct slot *left;
	struct slot *right;
	struct slot *up;
	struct slot *down;
};

int play();
void name(struct player players[], int n);
void class_choice(struct player p[], int n);
void capabilities(struct player p[], int n);
void createBoard(struct player p[], struct slot **board,struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight, int n);
void action(struct player p[], struct slot **board, int n);
void terrain_mod(struct player p[],int currPlayer);
void stats(struct player p[], int n);

int main(struct player p[])
{
	srand(time(NULL));
	int num, i;
	num = play();
	struct player players[num]; //A struct array that contains players information
	name(players, num); //Calls function to store names
	class_choice(players, num);
	capabilities(players, num); //Setting a players capabilities

	int row, column;
	struct slot *upLeft;
	struct slot *upRight;
	struct slot *downLeft;
	struct slot *downRight;
	//The board is represented as a pointer of pointer to slots
	//This allocates in memory the space for the pointers to each row of the board
	struct slot ** board = malloc(BOARD_SIZE * sizeof(struct slot *));

	createBoard(players,board,&upLeft, &upRight, &downLeft, &downRight, num);
	stats(players, num);
	
	
	action(players,board, num);	
	stats(players, num);	
	
	
	
	
	
}

int play()
{
	int n; //Stores the value of players
	printf("\nEnter the number of players(min 2,max 6): ");
	scanf("%d", &n);
	
	//Error checking
	while(n < 2 || n > 6)
	{
		printf("\nInvalid input, please try again!\n");
		printf("Enter the number of players(max 6): ");
		scanf("%d", &n);
	}
	
	return n; //Returns number of players to main
}

/*Function to ask user to choose names
 *Input: 
 *			p[] = struct array of players
 *			n   = number of players
 */
void name(struct player p[], int n)
{
	//Loop through players asking and storing their name into the array
	for(size_t i = 0; i < n; i++){
		printf("Enter the name of player %lu: ", i + 1);
		scanf("%s", p[i].name);
	}
}


/*A function to allow user to choose player classes
 *Input:
 *			p[] = Player data array
 *			n   = number of players
 */
void class_choice(struct player p[], int n)
{
	int p_class; //A value that is used to determine a player's class
	
	//A loop that asks user to pick a class
	for(size_t i = 0; i < n; i++)
	{
		//Class choice
		printf("\nEnter %s's class:\n", p[i].name);
		printf("(0)Elf  (1)Human  (2)Ogre  (3)Wizard\n");
		scanf("%d", &p_class);
		
		//Error Checking for class choice
		while(p_class < 0 || p_class > 3){
			printf("\nInvalid input, please try again!\n");
			printf("Enter %s's class:\n", p[i].name);
			printf("(0)Elf  (1)Human  (2)Ogre  (3)Wizard\n");
			scanf("%d", &p_class);
		}
		
		//Allocating the players their classes by storing an enum value to the .type
		switch(p_class){
			case(0):
				p[i].type = Elf; 
				break;
			case(1):
				p[i].type = Human;
				break;
			case(2):
				p[i].type = Ogre;
				break;
			case(3):
				p[i].type = Wizard;
				break;
			default:
				break;
		}
	}
}

/*Setting players capabilities based on class
 *Input:
 *				p[] = Player data array
 *				n   = Number of players
 */
void capabilities(struct player p[], int n)
{
	//Loops through the players and setting each players capabilities
	for(size_t i = 0; i < n; i++)
	{
		//Depending on the .type value a player will get a set of capabilities
		switch(p[i].type){
			case(0): //Elf capabilities
				p[i].life_points = 100;
				p[i].smartness = 70 + rand() % 31; 
				p[i].strength = 1 + rand() % 50;
				p[i].magic_skills = 51 + rand() % 29;
				p[i].luck = 60 + rand() % 41;
				p[i].dexterity = 1 + rand() % 100;
				break;
			case(1): //Human capabilities
				do{
					p[i].life_points = 100;
					p[i].smartness = 1 + rand() % 100; 
					p[i].strength = 1 + rand() % 100;
					p[i].magic_skills = 1 + rand() % 100;
					p[i].luck = 1 + rand() % 100;
					p[i].dexterity = 1 + rand() % 100;
				}while((p[i].smartness + p[i].strength + p[i].magic_skills + p[i].luck + p[i].dexterity) > 299); //Condition that his capabilities excluding life points needs to be < 300
				break;
			case(2): //Ogre capabilities
				do{
					p[i].life_points = 100;
					p[i].smartness = rand() % 21;
					p[i].strength = 80 + rand() % 21;
					p[i].magic_skills = 0;
					p[i].luck = rand() % 51;
					p[i].dexterity = 80 + rand() % 21;
				}while((p[i].luck + p[i].smartness > 50)); //Conditions that ogre's luck and smartness must be less than or equal to 50
				break;
			case(3): //Wizard capabilities
				p[i].life_points = 100;
				p[i].smartness = 90 + rand() % 11; 
				p[i].strength = 1 + rand() % 20;
				p[i].magic_skills = 80 + rand() % 21;
				p[i].luck = 50 + rand() % 51;
				p[i].dexterity = 1 + rand() % 100;
				break;
			default:
				break;
		}
	}
}


void createBoard(struct player p[],struct slot **board,struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight, int n){

	
	for(int i =0; i< BOARD_SIZE; i++){
		//This allocates in memory the space for the slots in each row of the board
		board[i] = malloc(BOARD_SIZE * sizeof(struct slot));

		//For each slot it sets up the row and column number
		for(int j=0;j < BOARD_SIZE; j++){
			board[i][j].row = i;
			board[i][j].column = j;
			board[i][j].type = rand()%3; //assigning terrain
		}
	}

	//It sets up the adjacent slots for the slots that are
	//not at the borders. These slots have 4 adjacent elements
	for(int i =1; i< BOARD_SIZE-1; i++){
		for(int j=1;j < BOARD_SIZE-1; j++){
			board[i][j].up = &board[i-1][j];
			board[i][j].right = &board[i][j+1];
			board[i][j].down = &board[i+1][j];
			board[i][j].left = &board[i][j-1];
		}
	}

	//It sets up the adjacent slots for the slots that are
	//in the first and the last row, except the slots at the edges.
	//
	for(int j = 1; j < BOARD_SIZE -1; j++){
		//It sets up the adjacent slots for the slots that are in the first row.
		//These slots don't have an adjacent element on top of them
		// because they are on the first row of the board
		board[0][j].right = &board[0][j+1];
		board[0][j].left = &board[0][j-1];
		board[0][j].down = &board[1][j];

		//It sets up the adjacent slots for the slots that are in the last row.
		//These slots don't have an adjacent element on down them
		// because they are on the last row of the board
		board[BOARD_SIZE - 1][j].right = &board[BOARD_SIZE - 1][j+1];
		board[BOARD_SIZE -1][j].left = &board[BOARD_SIZE - 1][j-1];
		board[BOARD_SIZE - 1][j].up = &board[BOARD_SIZE - 2][j];
	}

	//It sets up the adjacent slots for the slots that are
	//in the first and the last column, except the slots at the edges.
	//
	for(int i = 1; i < BOARD_SIZE -1; i++){
		//It sets up the adjacent slots for the slots that are in the first column.
		//These slots don't have an adjacent element on the left
		// because they are on the first column of the board
		board[i][0].right = &board[i][1];
		board[i][0].up = &board[i-1][0];
		board[i][0].down = &board[i+1][0];

		//It sets up the adjacent slots for the slots that are in the last column.
		//These slots don't have an adjacent element on the right
		// because they are on the last column of the board
		board[i][BOARD_SIZE-1].left = &board[i][BOARD_SIZE-2];
		board[i][BOARD_SIZE -1].up = &board[i -1][BOARD_SIZE-1];
		board[i][BOARD_SIZE -1].down = &board[i+1][BOARD_SIZE -1];
	}


	//It sets up the adjacent slots for the slot at position (0,0).
	//This only has only 2 adjacent slots: right and down
	board[0][0].right = &board[0][1];
	board[0][0].down = &board[1][0];

	//It sets up the adjacent slots for the slot at position (0,BOARD_SIZE -1).
	//This only has only 2 adjacent slots: left and down
	board[0][BOARD_SIZE-1].left = &board[0][BOARD_SIZE-2];
	board[0][BOARD_SIZE -1].down = &board[1][BOARD_SIZE -1];

	//It sets up the adjacent slots for the slot at position (BOARD_SIZE -1 ,0).
	//This only has only 2 adjacent slots: up and right
	board[BOARD_SIZE -1][0].right = &board[BOARD_SIZE -1][1];
	board[BOARD_SIZE -1][0].up = &board[BOARD_SIZE -2][0];

	//It sets up the adjacent slots for the slot at position (BOARD_SIZE -1 ,BOARD_SIZE-1).
	//This only has only 2 adjacent slots: up and left
	board[BOARD_SIZE - 1][BOARD_SIZE-1].up = &board[BOARD_SIZE-2][BOARD_SIZE-1];
	board[BOARD_SIZE - 1][BOARD_SIZE -1].left = &board[BOARD_SIZE -1][BOARD_SIZE -2];

	//assigns a pointer to slot at position (0, 0)
	*upLeft = &board[0][0];
	//assigns pointer of pointer to slot at position (0, BOARD_SIZE -1)
	*upRight = &board[0][BOARD_SIZE -1];
	//assigns pointer of pointer to slot at position ( BOARD_SIZE -1,)
	*downLeft = &board[BOARD_SIZE -1][0];
	//assigns pointer of pointer to slot at position (BOARD_SIZE -1, BOARD_SIZE -1)
	
	*downRight = &board[BOARD_SIZE -1][BOARD_SIZE -1];
	
	for(int i = 0; i < n; i++)
	{
		p[i].place = &board[rand()%7][rand()%7];
		//printf("[%d,%d]", p[i].place->row, p[i].place->column);
	}
	
	for(int i = 0; i < BOARD_SIZE; i++)
	{
		for(int j = 0; j < BOARD_SIZE; j++)
		{
			printf("[%d,%d]", board[i][j].row, board[i][j].column);
		}
		printf("\n");
	}
	
}

void action(struct player p[],struct slot **board, int n){
	//A loop that asks each user what course of action they will take
	for(size_t i = 0; i < n; i ++){
		printf("\n%s choose an action:\n", p[i].name);
		printf("(1)Move to an adacent slot - 1 step\n(2)Attack\n(3)Quit the game\n");
		scanf("%d", &p[i].action);
	
			//Error checking
			while(p[i].action < 1 || p[i].action > 3)
			{
				printf("Invalid input, please try again!\n");
				printf("\n%s choose an action:\n", p[i].name);
				printf("(1)Move to an adacent slot - 1 step\n(2)Attack\n(3)Quit the game\n");
				scanf("%d", &p[i].action);
			}
			
		int move[AREA];	
		int point = -1;
		for(size_t i = 0; i < AREA; i++)
		{
			move[i] = 1; //[0] = UP, [1] = DOWN, [2] = LEFT, [3] = RIGHT
		}		
		
			
		switch(p[i].action){
			case(1):
				printf("[%d,%d] Location of %s", p[i].place->row, p[i].place->column, p[i].name);
				
				if(p[i].place->row == 0 && (p[i].place->column != 0 && p[i].place->column !=6))
				{
					printf("\n%s can move:\n", p[i].name);
					printf("DOWN[%d,%d]\nLEFT[%d,%d]\nRIGHT[%d,%d]\n", p[i].place->down->row,p[i].place->down->column,p[i].place->left->row,p[i].place->left->column,p[i].place->right->row,p[i].place->right->column);
					move[0] = 0;
				}
				else if(p[i].place->row == 6 && (p[i].place->column != 0 && p[i].place->column !=6))
				{
					printf("\n%s can move:\n", p[i].name);
					printf("UP[%d,%d]\nLEFT[%d,%d]\nRIGHT[%d,%d]\n", p[i].place->up->row, p[i].place->up->column,p[i].place->left->row,p[i].place->left->column,p[i].place->right->row,p[i].place->right->column);
					move[1] = 0;
				}
				else if(p[i].place->column == 0 && (p[i].place->row != 0 && p[i].place->row !=6))
				{
					printf("\n%s can move:\n", p[i].name);
					printf("UP[%d,%d]\nDOWN[%d,%d]\nRIGHT[%d,%d]\n", p[i].place->up->row, p[i].place->up->column,p[i].place->down->row,p[i].place->down->column,p[i].place->right->row,p[i].place->right->column);
					move[2] = 0;
				}
				else if(p[i].place->column == 6 && (p[i].place->row != 0 && p[i].place->row !=6))
				{
					printf("\n%s can move:\n", p[i].name);
					printf("UP[%d,%d]\nDOWN[%d,%d]\nLEFT[%d,%d]\n", p[i].place->up->row, p[i].place->up->column,p[i].place->down->row,p[i].place->down->column,p[i].place->left->row,p[i].place->left->column);
					move[3] = 0;
				}
				else if(p[i].place->row == 0 && p[i].place->column == 0)
				{
					printf("\n%s can move:\n", p[i].name);
					printf("DOWN[%d,%d]\nRIGHT[%d,%d]\n",p[i].place->down->row,p[i].place->down->column,p[i].place->right->row,p[i].place->right->column);
					move[0] = 0;
					move[2] = 0;
				}
				else if(p[i].place->row == 0 && p[i].place->column == 6)
				{
					printf("\n%s can move:\n", p[i].name);
					printf("DOWN[%d,%d]\nLEFT[%d,%d]\n", p[i].place->down->row,p[i].place->down->column,p[i].place->left->row,p[i].place->left->column);
					move[0] = 0;
					move[3] = 0;
				}
				else if(p[i].place->row == 6 && p[i].place->column == 0)
				{
					printf("\n%s can move:\n", p[i].name);
					printf("UP[%d,%d]\nRIGHT[%d,%d]\n", p[i].place->up->row, p[i].place->up->column,p[i].place->right->row,p[i].place->right->column);
					move[1] = 0;
					move[2] = 0;
				}
				else if(p[i].place->row == 6 && p[i].place->column == 6)
				{
					printf("\n%s can move:\n", p[i].name);
					printf("UP[%d,%d]\nLEFT[%d,%d]\n", p[i].place->up->row, p[i].place->up->column,p[i].place->left->row,p[i].place->left->column);
					move[1] = 0;
					move[3] = 0;
				}
				else 
				{
					printf("\n%s can move:\n", p[i].name);
					printf("UP[%d,%d]\nDOWN[%d,%d]\nLEFT[%d,%d]\nRIGHT[%d,%d]\n", p[i].place->up->row, p[i].place->up->column,p[i].place->down->row,p[i].place->down->column,p[i].place->left->row,p[i].place->left->column,p[i].place->right->row,p[i].place->right->column);
				}
				
				
				do{
					
					printf("Enter your choice of movement:\n");
					printf("0.Up 1.Down 2.Left 3.Right\n");
					scanf("%d", &point);
					
					if(move[point] == 0){
						printf("Invalid action, retry\n");
					}
					
				}while(move[point] != 1);
				
				if(point == 0){
					p[i].place = p[i].place->up;
				}
				else if(point == 1){
					p[i].place = p[i].place->down;
				}
				else if(point == 2){
					p[i].place = p[i].place ->left;
				}
				else if(point = 3){
					p[i].place = p[i].place -> right;
				}
				terrain_mod(p,i);
				printf("[%d,%d] New place", p[i].place->row, p[i].place->column);
			break;
			case (2): printf("Attack Mode");/************************************************************************************************************************/
			
			
			break;
			case (3): printf("\n%s Wants to quit\n", p[i].name);
					p[i].life_points = 0;	
						
			break; 
			
		}
	}
}

/*A function that depending on the players location it will increase or decrease their stats
 *Input:
 *				p[] 	= Player data array
 *				s[] 	= Slots data array
 *				player 	= The current player
 */
void terrain_mod(struct player p[],int currPlayer)
{
	
	//Depending on current players location type we modify their capabilities
	switch(p[currPlayer].place->type){
		case(0): //Level Ground slot no capabilities needed to be modified
			break;
		case(1): //Modify player capabilities, when on Hill slot
			if(p[currPlayer].dexterity < 50){
				p[currPlayer].strength -= 10;
			}
			else if(p[currPlayer].dexterity >= 60){
				p[currPlayer].strength += 10;
			}
			break;
		case(2): //Modify player capabilities, when on City slot
			if(p[currPlayer].smartness > 60){
				p[currPlayer].magic_skills += 10;
			}
			else if(p[currPlayer].smartness <= 50){
				p[currPlayer].dexterity -= 10;
			}
			break;
		default:
				break;
	}
	
	//Conditions that ensures that capabilties don't exceed 100 or fall below 0
	if(p[currPlayer].strength < 0){
		p[currPlayer].strength = 0;
	}
	else if(p[currPlayer].strength > 100){
		p[currPlayer].strength = 100;
	}
	
	if(p[currPlayer].magic_skills > 100){
		p[currPlayer].magic_skills = 100;
	}
	
	if(p[currPlayer].dexterity < 0){
		p[currPlayer].dexterity = 0;
	}
}

/*A function that prints out players stats
 *Input:
 *				p[] = Player data array
 *				n   = Number of players
 */
void stats(struct player p[], int n){
	
	printf("\n%-10s  Class \t%-40s\n", "Player Name","---------------Capabilities---------------\tLocation");
	
	//A loop that prints players stats
	for(size_t i = 0; i < n; i++)
	{
		//A switch that depends on what class players picked
		switch(p[i].type){
			case(0):
				printf("%d.%-10s %-8s\tSm[%3d]  St[%3d]  MS[%3d]  L[%3d]  D[%3d]\t [%d,%d]\n",i+1,p[i].name, "Elf",p[i].smartness, p[i].strength,p[i].magic_skills, p[i].luck, p[i].dexterity, p[i].place->row, p[i].place->column);
				printf("(%d)\n", p[i].life_points);
				break;
			case(1):
				printf("%d.%-10s %-8s\tSm[%3d]  St[%3d]  MS[%3d]  L[%3d]  D[%3d]\t [%d,%d]\n",i+1,p[i].name, "Human",p[i].smartness, p[i].strength,p[i].magic_skills, p[i].luck, p[i].dexterity, p[i].place->row, p[i].place->column);
				printf("(%d)\n", p[i].life_points);
				break;
			case(2):
				printf("%d.%-10s %-8s\tSm[%3d]  St[%3d]  MS[%3d]  L[%3d]  D[%3d]\t [%d,%d]\n",i+1,p[i].name, "Ogre",p[i].smartness, p[i].strength,p[i].magic_skills, p[i].luck, p[i].dexterity, p[i].place->row, p[i].place->column);
				printf("(%d)\n", p[i].life_points);
				break;
			case(3):
				printf("%d.%-10s %-8s\tSm[%3d]  St[%3d]  MS[%3d]  L[%3d]  D[%3d]\t [%d,%d]\n",i+1,p[i].name, "Wizard",p[i].smartness, p[i].strength,p[i].magic_skills, p[i].luck, p[i].dexterity, p[i].place->row, p[i].place->column);
				printf("(%d)\n", p[i].life_points);
				break;
			default:
			break;
		}
	}	
}

void attack(struct player p[], int n){
	
	
	//p[i].place->row, p[i].place->column
	
	
}
