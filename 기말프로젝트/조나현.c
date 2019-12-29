/*
print_start > move_control > move_mario (print_star, print_map) >  print_end
*/

#include <stdio.h>
#include <Windows.h> //gotoxy ,SetConsoleTextAttribute
#include <conio.h>   //getch
#include <stdlib.h> 
#include<mmsystem.h>  //음악 넣기
#pragma comment(lib, "winmm.lib" )
#include <process.h>

void gotoxy(int x, int y);          // 커서의 위치 제어
void print_start();                 // 게임 시작 화면 출력
void print_end(int clear);          // 게임 종료 화면 출력
void print_map(int map[][150]);     // 맵 화면 출력
void move_control(int map[][150]);  // 키 입력에 따른 제어
void move_mario(int map[][150]);    // 마리오의 이동
void print_star();                  // 별 코인 점수 출력
void star_out(int map[][150]);      // 별 따면 별 사라짐 + count 증가
void jump(int map[][150]);          // 제자리 블록까지 점프 up or down

int inx = 0, iny = 8;               // 전역변수 inx, iny : 마리오의 시작위치
int count = 0;                      // 전역변수 별 딴 갯수

//커서 숨기는 함수
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;
void setcursortype(CURSOR_TYPE c)
{
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c)
	{
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
//맵 그림 디파인값 설정
#define SPACE 0
#define BLOCK 1
#define START 2
#define END 3
#define STAR 4
#define JUMP_UP_MAX 2
#define JUMP_DOWN_MAX 9

//가로150*세로 10칸 배열 생성
int map[10][150] =
{
 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0 },
 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,0,0,1,1,0,0,0,0,0,0 },
 { 0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,1,1,1,1,1,1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
 { 0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,1,1,1,1,4,0,0,0,4,4,4,4,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,4,4,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0 },
 { 0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,4,4,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0,4,4,0,0,0,0,0,4,4,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,4,4,4,4,4,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,4,0,0,0,0 },
 { 0,0,0,0,4,1,1,1,1,4,0,0,0,0,0,0,0,4,1,1,4,0,0,0,4,1,1,4,4,0,0,4,1,1,4,0,0,0,0,4,4,4,0,0,4,4,0,0,0,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,4,0,0,1,1,4,0,0,0,0,4,4,0,0,0,1,1,1,1,1,4,0,0,0,1,1,1,0,0,0,0,0,4,4,0,0,1,1,1,1,0,0,0,0,4,4,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,4,1,1,4,0,0,0,0,4,1,1,0,0,0,0,0 },
 { 0,0,0,4,0,0,0,0,0,0,0,4,0,0,0,0,4,0,0,0,0,4,4,0,0,0,0,0,0,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,4,4,0,0,0,0,0,4,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,4,0,0,0,4,0,0,0,0,0,4,0,0,0,0,0,0,4,4,4,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,4,0,0 },
 { 2,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,3 }
};

int main(void)
{
	setcursortype(NOCURSOR);   //커서 삭제
	print_start();
	return 0;
}

void print_start()
{
	int i, x1, y1, x2, y2, choice;
	x1 = 25;
	y1 = 6;
	x2 = 45;
	y2 = 13;
	PlaySound(L"start.wav", NULL, SND_ASYNC);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	gotoxy(x1, y1 + 0); printf("■■■    ■    ■   ■■■   ■■■   ■■■");
	gotoxy(x1, y1 + 1); printf("■        ■    ■   ■  ■   ■       ■  ■");
	gotoxy(x1, y1 + 2); printf("■■■    ■    ■   ■■■   ■■■   ■■■");
	gotoxy(x1, y1 + 3); printf("    ■    ■    ■   ■       ■       ■ ■ ");
	gotoxy(x1, y1 + 4); printf("■■■      ■■     ■       ■■■   ■  ■");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
	gotoxy(x2, y2 + 0); printf("  ■  ■       ■     ■■■   ■■■     ■   ");
	gotoxy(x2, y2 + 1); printf("■  ■  ■   ■  ■   ■  ■     ■     ■  ■ ");
	gotoxy(x2, y2 + 2); printf("■      ■   ■■■   ■■■     ■    ■    ■");
	gotoxy(x2, y2 + 3); printf("■      ■   ■  ■   ■ ■      ■     ■  ■ ");
	gotoxy(x2, y2 + 4); printf("■      ■   ■  ■   ■  ■   ■■■     ■   ");
	//박스칸
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	gotoxy(19, 21);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

	for (i = 0; i < 5; i++)
	{
		gotoxy(17, 22 + i);
		printf("■");
	}

	for (i = 0; i < 5; i++)
	{
		gotoxy(97, 22 + i);
		printf("■");
	}

	gotoxy(38, 24);
	printf("[ 1 ] 게임 시작        [ any key ] 게임 종료      ");

	gotoxy(19, 27);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

	choice = _getch();

	if (choice == '1')
	{
		system("cls");
		move_control(map);
	}
	else
	{
		exit(0);
	}
	system("cls");
}

void print_end(int clear)
{
	int i, x, y;
	x = 22;
	y = 10;
	//게임 성공시 clear=0 반환 실패시 1 반환
	if (clear == 0)
	{
		gotoxy(10, 7);
		printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣");
		for (i = 0; i < 15; i++)
		{
			gotoxy(10, 8 + i);
			printf("▣");
		}
		for (i = 0; i < 15; i++)
		{
			gotoxy(104, 8 + i);
			printf("▣");
		}
		gotoxy(x, y + 0);
		printf("+---------------------------------------------------------------------+");
		gotoxy(x, y + 1);
		printf("|                     G A M E           C L E A R !                   |");
		gotoxy(x, y + 2);
		printf("+---------------------------------------------------------------------+");
		gotoxy(x, y + 4);
		printf("                           Congratulation!!                            ");
		gotoxy(x, y + 7);
		printf("                          Your Score is   %d                  ",count*10);
		gotoxy(x, y + 10);
		printf("                    Play again. You will get more score :)             ");
		gotoxy(10, y + 13);
		printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣");
		PlaySound(L"success.wav", NULL, SND_SYNC);
		exit(0);
	}

	if (clear == 1)
	{
		gotoxy(10, 7);
		printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣");
		for (i = 0; i < 15; i++)
		{
			gotoxy(10, 8 + i);
			printf("▣");
		}
		for (i = 0; i < 15; i++)
		{
			gotoxy(104, 8 + i);
			printf("▣");
		}
		gotoxy(x, y + 0);
		printf("+---------------------------------------------------------------------+");
		gotoxy(x, y + 1);
		printf("|                     G A M E           O V E R !                     |");
		gotoxy(x, y + 2);
		printf("+---------------------------------------------------------------------+");
		gotoxy(x, y + 4);
		printf("                             What a shame!!                            ");
		gotoxy(x, y + 7);
		printf("                          Your Score is   %d                  ",count*10);
		gotoxy(x, y + 10);
		printf("                     Play again. You can do it :)                      ");
		gotoxy(10, y + 13);
		printf("▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣");
		PlaySound(L"fail.wav", NULL, SND_SYNC);
		exit(0);
	}
}

void print_map(int map[][150])
{
	int i, j, x, y;
	x = 19;
	y = 13;
	//한 화면에 가로 30 세로 10 출력
	for (i = 0; i < 10; i++)
	{
		gotoxy(x, y + i);
		for (j = inx; j < inx + 40; j++)
		{
			if ((i == iny && j == inx) && map[i][j] != BLOCK)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
				printf("♠");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			else if (map[9][inx] == SPACE && iny == 9)
			{
				system("cls"); 
				print_end(1);
			}
			else if (map[9][inx] == END && iny == 8)
			{
				system("cls");
				print_end(0);
			}
			else if ((map[iny][inx + 1] == END) && inx == 8)
			{
				system("cls");
				print_end(0);
			}
			else if (map[i][j] == BLOCK)
				printf("■");
			else if (map[i][j] == SPACE)
				printf("  ");
			else if (map[i][j] == STAR)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
				printf("☆");;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			else if (map[i][j] == START)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				printf("※");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			else if (map[i][j] == END)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				printf("※");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			star_out(map);
		}
		printf("\n");
	}
}

void move_control(int map[][150])
{
	char key;
	int count;
	do
	{
		while (!_kbhit())
		{
			system("cls");
			move_mario(map);
		}
		key = _getch();
		switch (key)
		{
		case 32:
			//스페이스
			jump(map);
			break;
		case 75:
			//<-
			if (map[iny][inx - 1] != BLOCK)   //벽 못 뚫고 지나가게
			{
				inx--;
				if (map[iny + 1][inx] != BLOCK && (map[iny + 1][inx] != START)) //낙하
				{
					for (count = 0; count < JUMP_DOWN_MAX; count++)
					{
						if (map[iny + 1][inx] == SPACE || map[iny + 1][inx] == STAR)
							iny++;
					}
				}
			}
			move_mario(map);
			break;
		case 77:
			//->
			if (map[iny][inx + 1] != BLOCK)
			{
				inx++;
				if (map[iny + 1][inx] != BLOCK && (map[iny + 1][inx] != END))
				{
					for (count = 0; count < JUMP_DOWN_MAX; count++)
					{
						if (map[iny + 1][inx] == SPACE || map[iny + 1][inx] == STAR)
							iny++;
					}
				}
			}
			move_mario(map);
			break;
		default:
			break;
		}
	} while (key != 27); //esc
	printf("\n");
}

void move_mario(int map[][150])
{
	do
	{
		system("cls");
		print_map(map);
		print_star();

		if (iny > 9)
			iny = 9;
		else if (iny < 0)
			iny = 0;
		if (inx > 149)
			inx = 149;
		else if (inx < 1)
			inx = 1;
		Sleep(100); // 시간을 지연시키는 함수
	} while (!_kbhit());
}

void print_star()
{
	gotoxy(80, 5);
	printf("Your score is %d",count*10);
}

void star_out(int map[][150])
{
	if (map[iny][inx] == STAR)
	{
		map[iny][inx] = SPACE;
		count++;
		PlaySound(L"coin.wav", NULL, SND_ASYNC);
	}
}

void jump(int map[][150])
{
	PlaySound(L"jump.wav", NULL, SND_ASYNC);
	int jcount;

		for (jcount = 0; jcount < JUMP_UP_MAX; jcount++)
		{
			if (map[iny - 1][inx] != BLOCK)
			{
				iny--;
				star_out(map);
			}
		}
		move_mario(map);
		Sleep(100);
}

void gotoxy(int x, int y)
{
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}