void board_display(char *board);
void new_game(char *board);
void continue_game(FILE *ap, char *board, int *turn);
void save_game(FILE *ap, char *board, int *turn);

int pawn_rule(char *board, char player, int i_pos, int f_pos);
int tower_rule(char *board, int inital_col, int dest_col, int inital_row, int dest_row);
int knight_rule(int i_pos, int f_pos);
int bishop_rule(char *board, int i_pos, int f_pos);
int king_rule(int i_pos, int f_pos);

void check();
int check_mate();

int check_move(char *board, char *piece, char *ini_pos, int i_pos, char *fin_pos, int f_pos, int player);
int check_rules(char *board, char *piece, int i_pos, int f_pos);
void move_piece(char *board, char *piece, int ini_pos, int fin_pos);