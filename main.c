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

//변수 선언 
int player_position[N_PLAYER];
char player_name[N_PLAYER][MAX_CHARNAME];
int player_coin[N_PLAYER];
int player_status[N_PLAYER];
char player_statusString[3][MAX_CHARNAME] = { "LIVE","DEAD","END" };

//게임 시작할때 출력할 함수 
void opening(void)
{
    printf("***************************************************************************\n");
    printf("                        ^o^ game start ^o^                                 \n");
    printf("***************************************************************************\n");

}

//랜덤으로 주사위 굴리기(1~6) 
int rolldie()
{
    return rand() % MAX_DIE + 1;
}

//파손된 보드 상태 확인 & player 자리 지정 함수 
void printPlayerPosition(int player)
{
    int i;
    for (i = 0; i < N_BOARD; i++)
    {
        printf("|");
        if (i == player_position[player])
            printf("%c", player_name[player][0]); // 첫번째 글자 출력
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

//player의 상태 출력 함수 
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

//die인 상태를 나타내는 함수 
void checkDie()
{
    int i;
    for (i = 0; i < N_PLAYER; i++) {
        //플레이어가 도착지점에 도착했는데 상어가 도착지점까지 도착한 경우 플레이어 상태는 죽음이 아니라 도착
        if (player_status[i] == PLAYERSTATUS_END)
            continue;

        if (board_getBoardStatus(player_position[i]) == BOARDSTATUS_NOK)// 보드가 파손된 경우 = 상어한테 잡힌경우 die 
            player_status[i] = PLAYERSTATUS_DIE;
    }

}


int main(int argc, char* argv[]) {
    //변수 선언 
	int pos = 0;
    int i;
    int turn = 0;

    srand((unsigned)time(NULL)); //random

    //0.opening
    opening();

    //1. 초기화 & 플레이어 이름 결정 
    board_initBoard();

    //1-2.
    for (i = 0; i < N_PLAYER; i++)
    {
        player_position[i] = 0;
        player_coin[i] = 0;
        player_status[i] = PLAYERSTATUS_LIVE;// 처음에 모두 live인 상태로 만들기 
        //player_name 입력 
        printf("player %d's name : ", i + 1);
        scanf("%s", player_name[i]);
    }

    //2. 반복문 (플레이어 턴)
    do { 
        int step;
        int coinResult;
        char c;
		
		//end or die인 경우 반복문을 돌지 않고 continue 
        if (player_status[turn] != PLAYERSTATUS_LIVE)
        {
            turn = (turn + 1) % N_PLAYER;
            continue;
        }

        //2-1. 플레이어 상태출력
        board_printBoardStatus();
        for (i = 0; i < N_PLAYER; i++)
        {
            printPlayerPosition(i);
        }
        printPlayerStatus();

        //2-2. 주사위 던지기
        printf("%s turn!! ", player_name[turn]);
        printf("Press any key to roll a die!\n");
        scanf("%c", &c);
        getchar();
        step = rolldie();

        //2-3. 이동 & 출력 
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


        //2-4. 동전 줍기
        pos = player_position[turn]; //player가 간 위치에서 랜덤 코인먹기 
        coinResult = board_getBoardCoin(pos);
		player_coin[turn] += coinResult;
		
		//해당 위치에서 코인을 먹을경우 lucky 출력 
        if (coinResult > 0)
            printf("   -> Lucky!!! %d's player got %d coins \n", turn + 1, coinResult);
            
        //해당 위치에 코인이 없거나(랜덤 값이 0으로 설정) 앞사람이 코인을 주워서 코인이 0개인 경우는 Unlucky 출력
        else
            printf("   -> Unlucky... there is no coin\n");


        //2-5. 다음턴
        turn = (turn + 1) % N_PLAYER; //wrap around 다시 돌아가는 
        
        //2-6. if (조건: 모든 플레어가 한번씩 돌기) 
        if (turn == 0)
        {
            //상어 동작
            int shark_pos = board_stepShark();
            printf("shark move to %i\n", shark_pos);
            checkDie(); 
        }
		 
        //게임이 종료되는 조건 
        int stop_cnt = 0;
        for (i = 0; i < N_PLAYER; i++) {
            if (player_status[i] == PLAYERSTATUS_LIVE)// live 상태인 경우 
                continue;
            else
                stop_cnt++;
        }

        if (stop_cnt == N_PLAYER) //end or die인 경우가 player 수만큼 됐을 경우 반복문 빠져나오기 
            break;
    } while (1);

    //최종 보드 상태 출력 
    printf("\n\n##############################FINAL RESULT##############################\n");
    board_printBoardStatus();//보드 상태 출력 
    for (i = 0; i < N_PLAYER; i++)
    {
        printPlayerPosition(i);
    }
    printPlayerStatus();//player 상태 출력 
    
    //player 최종 결과
	for (i=0;i<N_PLAYER;i++)
	{
		if(player_status[i] == PLAYERSTATUS_DIE )
			printf("%s in pos %d has died!! (COIN : %d)\n",player_name[i],player_position[i],player_coin[i]);
		else if(player_status[i] == PLAYERSTATUS_END)
			printf("%s in pos %d has END!! (COIN : %d)\n",player_name[i],player_position[i],player_coin[i]);
	} 
    printf("##############################FINAL RESULT##############################\n\n");

    //3. 정리 (승자 계산, 출력 등) 
    //결과 출력에 필요한 변수
    int all_die = 0, max = 0;
    int winner_cnt = 0;
    
    //all player die인 경우 all die 출력 -> 승자 없음. 
    for (i = 0; i < N_PLAYER; i++) {
        if (player_status[i] == PLAYERSTATUS_DIE) {
            all_die++;
        }
    }

    //모든 플레이어가 죽었는지 조건 확인
    if (all_die == N_PLAYER) {
        printf("ALL DIE!\n");
    }
    //승자 계산
    else {
        //플레이어 코인을 하나씩 비교하면서(살아있는 경우에만) 가장 많은 코인 개수 저장 
        for (i=0;i<N_PLAYER;i++)
        {
            if (player_status[i] == PLAYERSTATUS_END)//end인 player인 경우 
                if (player_coin[i] > max)
                { 
                    max = player_coin[i];
                }
        }
        
        //live인 player 중 max 코인을 가지고 있는 winner count 
        int i;
        for (i = 0; i < N_PLAYER; i++)
            if (player_coin[i] == max && player_status[i] == PLAYERSTATUS_END)
                winner_cnt++;

        //winner의 수가 한명 이상인 경우 multi winner 출력 
         if (winner_cnt > 1)
            printf("There are multi winners!\n");

        printf("-------------------- Game end --------------------\n");
        for (i = 0; i < N_PLAYER; i++) {
        	//최종 winner 
            if (player_coin[i] == max && player_status[i] == PLAYERSTATUS_END) {
                printf("Congratulation!  WINNER is %s!\n", player_name[i]);
            }
        }
        printf("--------------------------------------------------\n");
    }
    system("PAUSE");
    return 0;
}
