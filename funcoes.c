#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//verificar check e check-mate
//castling (torre e rei)
//peao mudar quando chega à ultima linha
//en passant

void board_display(char *board){
	printf("\n A    B    C    D    E    F    G    H");
	printf("\n-----------------------------------------\n");
	for (int y = 0; y < 64; y+=8)
	{
		printf("|");
		for (int x = 0; x < 8; x++)
		{
			printf(" %s |", &board[(x+y)*3]);
		}
		printf(" %d\n-----------------------------------------\n", (64-y)/8);
	}
}

void new_game(char *board){

	for (int i = 0; i < 64; i++)
	{
		if (i==0 || i==7)
		{
			strcpy(&board[i*3], "br");
		}else if (i==1 || i==6)
		{
			strcpy(&board[i*3], "bk");
		}else if (i==2 || i==5)
		{
			strcpy(&board[i*3], "bb");
		}else if (i==3)
		{
			strcpy(&board[i*3], "bq");
		}else if (i==4)
		{
			strcpy(&board[i*3], "bK");
		}else if (i<16 && i>7)
		{
			strcpy(&board[i*3], "bp");
		}else if (i==56 || i==63)
		{
			strcpy(&board[i*3], "wr");
		}else if (i==57 || i==62)
		{
			strcpy(&board[i*3], "wk");
		}else if (i==58 || i==61)
		{
			strcpy(&board[i*3], "wb");
		}else if (i==59)
		{
			strcpy(&board[i*3], "wq");
		}else if (i==60)
		{
			strcpy(&board[i*3], "wK");
		}else if (i<56 && i>47)
		{
			strcpy(&board[i*3], "wp");
		}else{
			strcpy(&board[i*3], "  ");
		}
	}
	board_display(board);
}

void continue_game(FILE *ap, char *board, int *turn){
	char line[25];
	int pos=0;
	ap = fopen("save.csv", "r");
	while (fgets(line, 25, ap) != NULL && pos < 63)
	{
		for (int i = 0; i < 8; i++)
		{
			if (i == 0)
			{
				memcpy(&board[(pos+i)*3], strtok(line, ",\n"),3);
			}else{
				memcpy(&board[(pos+i)*3], strtok(NULL, ",\n"),3);
			}
		}
		pos+=8;
	}
	*turn = atoi(line);
	fclose(ap);
	board_display(board);
}

void save_game(FILE *ap, char *board, int *turn){
	ap = fopen("save.csv", "w");
	for (int y = 0; y < 64; y+=8)
	{
		for (int x = 0; x < 8; x++)
		{
			if (x==7)
			{
				fprintf(ap, "%s", &board[(x+y)*3]);
			}else{
				fprintf(ap, "%s,", &board[(x+y)*3]);
			}
		}
		fprintf(ap, "\n");
	}
	fprintf(ap, "%d\n", *turn);
	fclose(ap);
	printf("Saved with success\n");
}

int pawn_rule(char *board, char player, int i_pos, int f_pos){
	
	int flag = 0;
	if (player == 'w')
	{
		if (i_pos > 47 && i_pos < 56) //first move
		{
			if ((f_pos == (i_pos-16) || f_pos == (i_pos-8)) && strcmp(&board[f_pos*3], "  ") == 0)
			{
				flag = 1;
			}
		}else{ //default move
			if (f_pos == (i_pos-8) && strcmp(&board[f_pos*3], "  ") == 0)
			{
				flag = 1;
			}
		}
		for (int i = i_pos-8; i < f_pos-1; i-=8) //verifying if there is a piece in it's way
		{
			if (strcmp(&board[i*3], "  ") != 0)
			{
				return 0;
			}
		}
		if ((f_pos == (i_pos-7) && strcmp(&board[f_pos*3], "  ") != 0) || (f_pos == (i_pos+9) && strcmp(&board[f_pos*3], "  ") != 0)) //eating another piece
		{
			return  1;
		}
	}else{ // same thing but for the black pieces
		if (i_pos > 7 && i_pos < 16) 
		{
			if ((f_pos == (i_pos+16) || f_pos == (i_pos+8)) && strcmp(&board[f_pos*3], "  ") == 0)
			{
				flag = 1;
			}
		}else{ 
			if (f_pos == (i_pos+8) && strcmp(&board[f_pos*3], "  ") == 0)
			{
				flag = 1;
			}
		}
		for (int i = i_pos+8; i < f_pos+1; i+=8)
		{
			if (strcmp(&board[i*3], "  ") != 0)
			{
				return 0;
			}
		}
		if ((f_pos == (i_pos+9) && strcmp(&board[f_pos*3], "  ") != 0) || (f_pos == (i_pos+7) && strcmp(&board[f_pos*3], "  ") != 0)) 
		{
			return 1;
		}
	}
	return flag;
}

int rook_rule(char *board, int inital_col, int dest_col, int inital_row, int dest_row){

	if ((inital_col != dest_col) && (inital_row != dest_row)) //first condition for vertical movement and second one for horizontal
	{
		return 0;
	}else if ((inital_col == dest_col) && inital_row>dest_row) //verifying if there is a piece in it's way (4 times)
	{
		for (int i = inital_row-1; i > dest_row; i--) 
		{
			if (strcmp(&board[(i*8+inital_col)*3], "  ") != 0)
			{
				return 0;
			}
		}
	}else if ((inital_col == dest_col) && inital_row<dest_row)
	{
		for (int i = inital_row+1; i < dest_row; i++)
		{
			if (strcmp(&board[(i*8+inital_col)*3], "  ") != 0)
			{
				return 0;
			}
		}
	}else if ((inital_row == dest_row) && inital_col>dest_col)
	{
		for (int i = inital_col-1; i > dest_col; i--)
		{
			if (strcmp(&board[(inital_row*8+i)*3], "  ") != 0)
			{
				return 0;
			}
		}
	}else if ((inital_row == dest_row) && inital_col<dest_col)
	{
		for (int i = inital_col+1; i < dest_col; i++)
		{
			if (strcmp(&board[(inital_row*8+i)*3], "  ") != 0)
			{
				return 0;
			}
		}
	}
	
	return 1;
}

int knight_rule(int i_pos, int f_pos){

	if (f_pos > i_pos)
	{
		if ((f_pos == (i_pos+8+2)) || (f_pos == (i_pos+8-2)) || (f_pos == (i_pos+1+16)) || (f_pos == (i_pos-1+16)))
		{
			return 1;
		}
	}else if (f_pos < i_pos)
	{
		if ((f_pos == (i_pos-8+2)) || (f_pos == (i_pos-8-2)) || (f_pos == (i_pos+1-16)) || (f_pos == (i_pos-1-16)))
		{
			return 1;
		}
	}

	return 0;
}

int bishop_rule(char *board, int i_pos, int f_pos){	

	if ((abs(i_pos-f_pos)%9!=0) && (abs(i_pos-f_pos)%7 != 0))
	{
		return 0;
	}
	if ((abs(i_pos-f_pos)%9==0) && i_pos>f_pos) //verifying if there is a piece in it's way (4 times)
	{
		for (int i = i_pos-9; i > f_pos; i-=9) 
		{
			if (strcmp(&board[i*3], "  ") != 0)
			{
				return 0;
			}
		}
	}else if ((abs(i_pos-f_pos)%9==0) && i_pos<f_pos)
	{
		for (int i = i_pos+9; i < f_pos; i+=9)
		{
			if (strcmp(&board[i*3], "  ") != 0)
			{
				return 0;
			}
		}
	}else if ((abs(i_pos-f_pos)%7 == 0) && i_pos>f_pos)
	{
		for (int i = i_pos-7; i > f_pos; i-=7)
		{
			if (strcmp(&board[i*3], "  ") != 0)
			{
				return 0;
			}
		}
	}else if ( (abs(i_pos-f_pos)%7 == 0) && i_pos<f_pos)
	{
		for (int i = i_pos+7; i < f_pos; i+=7)
		{
			if (strcmp(&board[i*3], "  ") != 0)
			{
				return 0;
			}
		}
	}
	
	return 1;
}

int king_rule(int i_pos, int f_pos){

	if ((f_pos == i_pos+1) || (f_pos == i_pos-1) || (f_pos == i_pos+8) || (f_pos == i_pos-8) || (f_pos == i_pos+9) || (f_pos == i_pos-9) || (f_pos == i_pos+7) || (f_pos == i_pos-7))
	{
		return 1;
	}
	
	return 0;
}

int check_rules(char *board, char *piece, int i_pos, int f_pos){
	int flag = 0;
	int inital_col= i_pos%8;
	int dest_col = f_pos%8;
	int inital_row = abs(i_pos-inital_col)/8;
	int dest_row = abs(f_pos-dest_col)/8;

	if (piece[1] == 'p')
	{
		flag = pawn_rule(board, piece[0], i_pos, f_pos);
	}else if (piece[1] == 'r')
	{
		flag = rook_rule(board, inital_col, dest_col, inital_row, dest_row);
	}else if (piece[1] == 'k')
	{
		flag = knight_rule(i_pos, f_pos);
	}else if (piece[1] == 'b')
	{
		flag = bishop_rule(board, i_pos, f_pos);
	}else if (piece[1] == 'q') // rule for the queen is a junction of the rook and bishop rules
	{
		if (rook_rule(board, inital_col, dest_col, inital_row, dest_row) || bishop_rule(board, i_pos, f_pos))
		{
			flag = 1;
		}
	}else if (piece[1] == 'K')
	{
		flag = king_rule(i_pos, f_pos);
	}
	
	return flag;
}

int check_move(char *board, char *piece, char *ini_pos, int i_pos, char *fin_pos, int f_pos ,int player){ 

	char pieces_name[25] = {"bbwpbpwrbrwkbkwqbqwKbKwb"};
	if(strstr(pieces_name, piece) == NULL){
		printf("The name of the piece is wrong.\n");
		return 0;
	}else if ((player == 2 && piece[0]=='w') || (player == 1 && piece[0] == 'b'))
	{
		printf("Wrong player.\n");
		return 0;
	
	}else if (('A'>ini_pos[0] || 'H'<ini_pos[0]) || (atoi(&ini_pos[1]) >8 || atoi(&ini_pos[1]) <1) || strcmp(&board[i_pos*3], piece) != 0)
	{
		printf("The initial position is wrong.\n");
		return 0;
	}else if(('A'>fin_pos[0] || 'H'<fin_pos[0]) || (atoi(&fin_pos[1]) >8 || atoi(&fin_pos[1]) <1) || (player == 1 && board[f_pos*3]=='w') || (player == 2 && board[f_pos*3] =='b')){
		printf("The final position is wrong.\n");
		return 0;
	}else if (i_pos == f_pos)
	{
		printf("The initial position can't be the same as the final one\n");
		return 0;
	}
	
	
	return 1;
}

void move_piece(char *board, char *piece, int ini_pos, int fin_pos){
		
	strcpy(&board[fin_pos*3], piece);
	strcpy(&board[ini_pos*3], "  ");
}