#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h" 

#define MAX_CHARNAME	200
#define N_PLAYER		3
#define MAX_DIE         6 

#define PLAYERSTATUS_LIVE	0
#define PLAYERSTATUS_DIE	1
#define PLAYERSTATUS_END	2

//���� ���� 
int player_position[N_PLAYER];
char player_name[N_PLAYER][MAX_CHARNAME];
int player_coin[N_PLAYER];
int player_status[N_PLAYER];
char player_statusString[3][MAX_CHARNAME] = { "LIVE","DEAD","END" };

//���� �����Ҷ� ����� �Լ� 
void opening(void)
{
    printf("***************************************************************************\n");
    printf("                        ^o^ game start ^o^                                 \n");
    printf("***************************************************************************\n");

}

//�������� �ֻ��� ������(1~6) 
int rolldie()
{
    return rand() % MAX_DIE + 1;
}

//�ļյ� ���� ���� Ȯ�� & player �ڸ� ���� �Լ� 
void printPlayerPosition(int player)
{
    int i;
    for (i = 0; i < N_BOARD; i++)
    {
        printf("|");
        if (i == player_position[player])
            printf("%c", player_name[player][0]); // ù��° ���� ���
        else
        {
            if (board_getBoardStatus(i) == BOARDSTATUS_NOK)
                printf("X");
            else
                printf(" ");
        }

    }
    printf("|\n");
}

//player�� ���� ��� �Լ� 
void printPlayerStatus()
{
    int i;
    printf("----------player status----------\n");
    for (i = 0; i < N_PLAYER; i++)
    {
        printf("%s : pos %i, coin %i, status : %s\n", player_name[i], player_position[i], player_coin[i], player_statusString[player_status[i]]);

    }
    printf("---------------------------------\n");
}

//die�� ���¸� ��Ÿ���� �Լ� 
void checkDie()
{
    int i;
    for (i = 0; i < N_PLAYER; i++) {
        //�÷��̾ ���������� �����ߴµ� �� ������������ ������ ��� �÷��̾� ���´� ������ �ƴ϶� ����
        if (player_status[i] == PLAYERSTATUS_END)
            continue;

        if (board_getBoardStatus(player_position[i]) == BOARDSTATUS_NOK)// ���尡 �ļյ� ��� = ������� ������� die 
            player_status[i] = PLAYERSTATUS_DIE;
    }

}


int main(int argc, char* argv[]) {
    //���� ���� 
	int pos = 0;
    int i;
    int turn = 0;

    srand((unsigned)time(NULL)); //random

    //0.opening
    opening();

    //1. �ʱ�ȭ & �÷��̾� �̸� ���� 
    board_initBoard();

    //1-2.
    for (i = 0; i < N_PLAYER; i++)
    {
        player_position[i] = 0;
        player_coin[i] = 0;
        player_status[i] = PLAYERSTATUS_LIVE;// ó���� ��� live�� ���·� ����� 
        //player_name �Է� 
        printf("player %d's name : ", i + 1);
        scanf("%s", player_name[i]);
    }

    //2. �ݺ��� (�÷��̾� ��)
    do { 
        int step;
        int coinResult;
        char c;
		
		//end or die�� ��� �ݺ����� ���� �ʰ� continue 
        if (player_status[turn] != PLAYERSTATUS_LIVE)
        {
            turn = (turn + 1) % N_PLAYER;
            continue;
        }

        //2-1. �÷��̾� �������
        board_printBoardStatus();
        for (i = 0; i < N_PLAYER; i++)
        {
            printPlayerPosition(i);
        }
        printPlayerStatus();

        //2-2. �ֻ��� ������
        printf("%s turn!! ", player_name[turn]);
        printf("Press any key to roll a die!\n");
        scanf("%c", &c);
        getchar();
        step = rolldie();

        //2-3. �̵� & ��� 
        player_position[turn] += step;
        if (player_position[turn] >= N_BOARD)
        {
            player_position[turn] = N_BOARD - 1;
        }
        if (player_position[turn] == N_BOARD - 1)
        {
            player_status[turn] = PLAYERSTATUS_END;
        }
        printf("dice result : %d, %d's player moved to %d\n", step, turn + 1, player_position[turn]);


        //2-4. ���� �ݱ�
        pos = player_position[turn]; //player�� �� ��ġ���� ���� ���θԱ� 
        coinResult = board_getBoardCoin(pos);
		player_coin[turn] += coinResult;
		
		//�ش� ��ġ���� ������ ������� lucky ��� 
        if (coinResult > 0)
            printf("   -> Lucky!!! %d's player got %d coins \n", turn + 1, coinResult);
            
        //�ش� ��ġ�� ������ ���ų�(���� ���� 0���� ����) �ջ���� ������ �ֿ��� ������ 0���� ���� Unlucky ���
        else
            printf("   -> Unlucky... there is no coin\n");


        //2-5. ������
        turn = (turn + 1) % N_PLAYER; //wrap around �ٽ� ���ư��� 
        
        //2-6. if (����: ��� �÷�� �ѹ��� ����) 
        if (turn == 0)
        {
            //��� ����
            int shark_pos = board_stepShark();
            printf("shark move to %i\n", shark_pos);
            checkDie(); 
        }
		 
        //������ ����Ǵ� ���� 
        int stop_cnt = 0;
        for (i = 0; i < N_PLAYER; i++) {
            if (player_status[i] == PLAYERSTATUS_LIVE)// live ������ ��� 
                continue;
            else
                stop_cnt++;
        }

        if (stop_cnt == N_PLAYER) //end or die�� ��찡 player ����ŭ ���� ��� �ݺ��� ���������� 
            break;
    } while (1);

    //���� ���� ���� ��� 
    printf("\n\n##############################FINAL RESULT##############################\n");
    board_printBoardStatus();//���� ���� ��� 
    for (i = 0; i < N_PLAYER; i++)
    {
        printPlayerPosition(i);
    }
    printPlayerStatus();//player ���� ��� 
    
    //player ���� ���
	for (i=0;i<N_PLAYER;i++)
	{
		if(player_status[i] == PLAYERSTATUS_DIE )
			printf("%s in pos %d has died!! (COIN : %d)\n",player_name[i],player_position[i],player_coin[i]);
		else if(player_status[i] == PLAYERSTATUS_END)
			printf("%s in pos %d has END!! (COIN : %d)\n",player_name[i],player_position[i],player_coin[i]);
	} 
    printf("##############################FINAL RESULT##############################\n\n");

    //3. ���� (���� ���, ��� ��) 
    //��� ��¿� �ʿ��� ����
    int all_die = 0, max = 0;
    int winner_cnt = 0;
    
    //all player die�� ��� all die ��� -> ���� ����. 
    for (i = 0; i < N_PLAYER; i++) {
        if (player_status[i] == PLAYERSTATUS_DIE) {
            all_die++;
        }
    }

    //��� �÷��̾ �׾����� ���� Ȯ��
    if (all_die == N_PLAYER) {
        printf("ALL DIE!\n");
    }
    //���� ���
    else {
        //�÷��̾� ������ �ϳ��� ���ϸ鼭(����ִ� ��쿡��) ���� ���� ���� ���� ���� 
        for (i=0;i<N_PLAYER;i++)
        {
            if (player_status[i] == PLAYERSTATUS_END)//end�� player�� ��� 
                if (player_coin[i] > max)
                { 
                    max = player_coin[i];
                }
        }
        
        //live�� player �� max ������ ������ �ִ� winner count 
        int i;
        for (i = 0; i < N_PLAYER; i++)
            if (player_coin[i] == max && player_status[i] == PLAYERSTATUS_END)
                winner_cnt++;

        //winner�� ���� �Ѹ� �̻��� ��� multi winner ��� 
         if (winner_cnt > 1)
            printf("There are multi winners!\n");

        printf("-------------------- Game end --------------------\n");
        for (i = 0; i < N_PLAYER; i++) {
        	//���� winner 
            if (player_coin[i] == max && player_status[i] == PLAYERSTATUS_END) {
                printf("Congratulation!  WINNER is %s!\n", player_name[i]);
            }
        }
        printf("--------------------------------------------------\n");
    }
    system("PAUSE");
    return 0;
}
