#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

int main(int argc, char *argv[]){

    if(argc != 1){

        printf("Please rewrite like the following: './chess'\n");
        return -1;
    }

	char *board = (char *)malloc(64*(3*sizeof(char))); //64 positions with 3 chars each position with '/0' in the end
	//layout of the board and the positions
	/* 0  1  2  3  4  5  6  7
	   8  9  10 11 12 13 14 15
	   16 17 18 19 20 21 22 23
	   24 25 26 27 28 29 30 31
	   32 33 34 35 36 37 38 39
	   40 41 42 43 44 45 46 47
	   48 49 50 51 52 53 54 55
	   56 57 58 59 60 61 62 63
	*/

	int choice, *turn = (int *)malloc(sizeof(int));
	*turn = 0;
	char check_file, move[15];
    FILE *ap;
	ap = fopen("save.csv", "r");

	//Choice of a new game or continue a previous one
    printf("1 - New Game\n");
    if (fgets(&check_file, 1, ap) != NULL)
    {
        printf("2 - Continue Game\n");
    }
	fclose(ap);
	printf("Choice: ");
	do
	{
		scanf("%d", &choice);
    	if (choice == 1)
		{
			new_game(board);
			break;
		}else if (choice == 2)
		{
			continue_game(ap, board, turn);
			break;
		}else{
			printf("Please enter a valid choice: ");
		}
	}while(1);
	
	//The game itself
	int player;

	if (*turn%2 == 0)
	{
		player = 1;
	}else{
		player = 2;
	}
	
	while (1)
	{	
		printf("Turn: %d\n", *turn+1);
		printf("Player %d: ", player);
		scanf("%s", move);
		if (strcmp(move, "exit") == 0)
		{
			printf("Exited with success\n");
			break;
		}else if(strcmp(move, "save") == 0){
			save_game(ap, board, turn);
			break;
		}
		char piece[3], ini_pos[3], fin_pos[3];

		memcpy(piece, strtok(move, "-"), sizeof piece);
		memcpy(ini_pos, strtok(NULL, "-"), sizeof ini_pos);
		memcpy(fin_pos, strtok(NULL, "-"), sizeof fin_pos);

		int i_pos = 64-(atoi(&ini_pos[1])*8) + (ini_pos[0]-65);
		int f_pos = 64-(atoi(&fin_pos[1])*8) + (fin_pos[0]-65);

		if(check_move(board, piece, ini_pos, i_pos, fin_pos, f_pos, player) != 1){
			printf("\nSuggestion: exit | save | 'piece'-'initial position'-'final position'\n\n");
		}else if (check_rules(board, piece, i_pos, f_pos) != 1){
			printf("Wrong '%s' movement.\n", piece);			
		}else{
			check();
			move_piece(board, piece, i_pos, f_pos);
			board_display(board);
			if (player == 2)
			{
				player = 1;
			}else{
				player = 2;
			}
			(*turn)++;
		}

		if (*turn%10 == 0)
		{
			printf("Do you want to save? (yes or no)\n");
			scanf("%s", move);
			if (strcmp(move, "yes") == 0)
			{
				save_game(ap, board, turn);
			}
		}
		
	}

	free(turn);
	free(board);
    return 0;
}