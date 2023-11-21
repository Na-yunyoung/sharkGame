#include <stdio.h>
#include <stdlib.h>
#include "board.h" 

#define N_BOARD		15
#define N_COINPOS	12
#define MAX_COIN	4


static int board_status[N_BOARD];
static int board_coin[N_BOARD];

static int board_sharkPosition;

int board_initBoard() //���� �ʱ�ȭ
{
	int i;
	for (i=0;i<N_BOARD;i++)
	{
		board_status[i] == BOARDSTATUS_OK;
		board_coin[i] = 0;
	}
	for (i=0;i<N_COINPOS;i++)
	{
		int flag = 0;
		while(flag == 0)
		{
			int allocpos = rand()%N_BOARD;
			if (board_coin[allocpos] == 0)
			{
				board_coin[allocpos] = rand() %MAX_COIN+1;
				flag = 1;
			}
		}
	}
 } 

int board_printBoardStatus()  // ��ü ������ �������
{
	int i;
	
	printf("----------------------------------- BOARD STATUS --------------------------------------------------\n");
	for( i=0;i<N_BOARD;i++)
	{
		printf("|");
		if (board_status[i] == BOARDSTATUS_NOK)
			printf("X");
		else
			printf("O"); 
	}
	printf("|\n");
	printf("----------------------------------------------------------------------------------------------------\n");
} 
int board_getBoardCoin(int pos) //�������� ���� 
{
	int coin = board_coin[pos];
	board_coin[pos] = 0;
	return board_coin[pos];
}
int board_getBoardStatus(int pos) //Ư�� ĭ�� �ļ� ���� ��� 
{
	return board_status[pos];
}

//int board_getSharkPosition(); //����� ��ġ ��� 

//int board_stepShark(); //��� �������� 