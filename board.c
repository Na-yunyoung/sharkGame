#include <stdio.h>
#include <stdlib.h>
#include "board.h" 

#define N_BOARD		15
#define N_COINPOS	12
#define MAX_COIN	4

#define MAX_SHARKSTEP	6
#define SHARK_INITPOS	-4

//변수 선언 
static int board_status[N_BOARD];
static int board_coin[N_BOARD];
static int shark_Position;

int board_initBoard() //보드 초기화
{
	int i;
	for (i=0;i<N_BOARD;i++)
	{
		board_status[i] = BOARDSTATUS_OK;
		board_coin[i] = 0;
	}
	
	shark_Position = SHARK_INITPOS;
	for (i=0;i<N_COINPOS;i++)
	{
		int flag = 0;
		while(flag == 0)
		{
			int allocPos = rand()%N_BOARD;
			if (board_coin[allocPos] == 0)
			{
				board_coin[allocPos] = rand()%MAX_COIN+1;
				flag = 1;
			}
		}
	}
 } 

int board_printBoardStatus()  // 전체 보드의 상태출력
{
	int i;
	
	printf("----------------------------------- BOARD STATUS --------------------------------------------------\n");
	for( i=0;i<N_BOARD;i++)
	{
		printf("|");
		if (board_status[i] == BOARDSTATUS_NOK) //파손된 경우 = 상어가 지나간 자리 
			printf("X");
		else
			printf("O"); 
	}
	printf("|\n");
	printf("----------------------------------------------------------------------------------------------------\n");
} 
int board_getBoardCoin(int pos) //동전습득 명령 
{
	int coin = board_coin[pos];
	board_coin[pos] = 0;
	return coin;
}
int board_getBoardStatus(int pos) //특정 칸의 파손 여부 출력 
{
	return board_status[pos];
}

int board_stepShark()//상어 전진명령
{
	int step = rand()%MAX_SHARKSTEP + 1;
	int i;
	for (i= shark_Position+1;i<=shark_Position+step;i++)
	{
		if(i>=0 && i< N_BOARD)
			board_status[i] = BOARDSTATUS_NOK;
	}
	
	shark_Position += step;
	
	return shark_Position;
	
}
