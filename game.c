#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#define NUM_THREADS 3
//Name: Akib Hasan Aryan
//UCID: 30141456
//Defines structure for thread id
typedef struct _thread_data_t
{
	int tid;
} thread_data_t;
//defines struct to be shared among threads
struct shared{
	char board[3][3];	//2D board of character arrays
	int turn;		//turn variable
};

void create_board(char board[3][3])	//Function to create board
{
	for (int i=0; i<3; i++)	
	{
		for (int j=0; j < 3; j++){
			board[i][j] = '.';	//Initally makes board with all spaces having .
		}
	}
}
struct shared share;				//Initialises shared struct
pthread_mutex_t lock_shared_data;		//Mutex
void display_board(char board[3][3])		//Function to display board at a given time
{
	for (int i=0;i<3; i++)		
	{
		for(int j=0; j<3; j++){
			printf("%c",board[i][j]);//Prints out each character of a row
		}
		printf("\n");			//prints out new line at end of each row
	}
	printf("\n");				//Prints out new line at end of each board
}
bool check_winnerY(char board[3][3])		//Function to check if winner is O
{	//Sees if rows contain all O

	if('O' == board[0][0] == board[1][1] == board[2][2]){	
		return true;
	}
	
	else if('O' == board[0][0] == board[1][0] == board[2][0]){
		return true;
	}
	else if('O' == board[0][1] == board[1][1] == board[2][1]){
		return true;
	}

	else if('O' == board[0][2] == board[1][2] == board[2][2]){
		return true;
	}
	//Sees if columns contain all O
	else if('O' == board[0][0] == board[0][1] == board[0][2]){
		return true;
	}
	else if('O' == board[1][0] == board[1][1] == board[1][2]){
		return true;
	}
	else if('O' == board[2][0] == board[2][1] == board[2][2]){
		return true;
	}
	else if('O' == board[0][2] == board[1][1] == board[2][0]){
		return true;
	}
	else{
		return false;
	}
}

bool check_winnerX(char board[3][3]){//Checks if winner is X in the same way as above

	if('X' == board[0][0] == board[1][1] == board[2][2]){
		return true;
	}
	else if('X' == board[0][0] == board[1][0] == board[2][0]){
		return true;
	}
	else if('X' == board[0][1] == board[1][1] == board[2][1]){
		return true;
	}
	else if('X' == board[0][2] == board[1][2] == board[2][2]){
		return true;
	}
	else if('X' == board[0][0] == board[0][1] == board[0][2]){
		return true;
	}
	else if('X' == board[1][0] == board[1][1] == board[1][2]){
		return true;
	}
	else if('X' == board[2][0] == board[2][1] == board[2][2]){
		return true;
	}
	else if('X' == board[0][2] == board[1][1] == board[2][0]){
		return true;
	}
	else{
		return false;
	}
}
//Checks if board is full
bool board_full(char board[3][3]){
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if('.' == board[i][j]){
				return false;
			}
		}
	}
	return true;
}
//ALlows player thread to decide where to place a move
void decideMove(char c, char b[3][3]){
	while(1){
		int num1 = rand()%3;	//Generates random column number
		int num2 = rand()%3;	//Generates random row number
		if(b[num2][num1] == '.'){//Checks if the random spot is empty
			b[num2][num1] = c;//Places symbol if it is
			break;
		}
	}
}
//Function to see who is the winner
void win_announce(char b[3][3]){
	if(check_winnerX(b)){
		printf("Player one wins!");
	}
	else if(check_winnerY(b)){
		printf("Player two wins!");
	}
	else if(board_full(b)){
		printf("It's a draw!");
	}
}
//Player thread function
void *play(void *arg){
	//struct thread_data_t *data = (struct thread_data_t *) malloc(sizeof(int));
	thread_data_t *data = (thread_data_t *)arg;
	//thread_data_t lis = data;
	while(1){
		while(share.turn != (data->tid));
		char c;
		if((data->tid) == 1){
			char c = 'X';
			decideMove('X',share.board);
		}else{
			char c = 'O';
			decideMove('O',share.board);
		}
		//decideMove(c,share.board);
		share.turn = 0;
		

		//pthread_exit(NULL);

	}

}

int main(int argc, char **argv)
{	
	bool game_on = true;
	pthread_t thr[2];
	int i, rc;
	/*Create a thread_data_t argument array */
	thread_data_t thr_data[NUM_THREADS];
	
	/*initialise shared data */
	create_board(share.board);
	share.turn = 0;
	display_board(share.board);
	/*initialise pthread mutex protecting "Shared data" */
	pthread_mutex_init(&lock_shared_data, NULL);

	/*create threads*/

	for(i=0; i<NUM_THREADS; ++i)
	{
		thr_data[i].tid = i+1;
		if((rc = pthread_create(&thr[i], NULL, play, &thr_data[i])))
		{
			fprintf(stderr, "error: pthread_create rc:%d\n", rc);
			return EXIT_FAILURE;
		}
	}
	int counter = 1;
	while(1){
	while (share.turn != 0);
	display_board(share.board);
	if(check_winnerX(share.board) || check_winnerY(share.board) || board_full(share.board)){
		game_on=false;
		pthread_cancel(thr[0]);
		pthread_cancel(thr[1]);
		win_announce(share.board);
		break;
	}
	else{
		share.turn = counter;
		counter = counter+1;
		if(counter>2){
			counter=1;
		}
	}
	}
	return 1;
}

