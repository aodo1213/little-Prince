#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

#define WIDTH 120
#define HEIGHT 30

void item(int itemx[], int itemy[], int itemban[]);
void monster(int mopx[], int mopy[]);
void hole(int holex[], int holey[]);
void water(int waterx[], int watery[], int water_exists[], int isSaltWater[]);
void colorset(int backcolor, int textcolor);
void gotoxy(int x, int y);
void clearScreen();
void displayWaterLevel(int waterLevel);
void displayMessage(const char* message, int duration);
bool isCoordinateOccupied(int x, int y, int arrX[], int arrY[], int size);

void colorset(int backcolor, int textcolor) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, (backcolor << 4) + textcolor);
}

void gotoxy(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen() {
    system("cls");
}
void displayscore(int score) {
    colorset(0, 15);
    gotoxy(0, 2);
    printf("점수:%d", score);
}
void displayHP(int HP) {
    colorset(4, 15);
    gotoxy(0, 1);
    printf("HP=%d", HP);
}
void displayWaterLevel(int waterLevel) {
    colorset(1, 15); // 파란색 배경에 흰색 글자
    gotoxy(0, 0);
    printf("수분: %d  ", waterLevel); // 수분 표시 (빈 공간 추가하여 이전 값 지우기)
}

void displayMessage(const char* message, int duration) {
    colorset(1, 15); // 파란색 배경에 흰색 글자
    gotoxy(10, 0); // 수분 표시 옆에 메시지 표시
    printf("%s", message);
    Sleep(duration);
    gotoxy(0, 1); // 메시지 지우기
    printf("                                "); // 이전 메시지 지우기 위한 공백 출력
}

bool isCoordinateOccupied(int x, int y, int arrX[], int arrY[], int size) {
    for (int i = 0; i < size; i++) {
        if (arrX[i] == x && arrY[i] == y) {
            return true;
        }
    }
    return false;
}

int main() {
    setlocale(LC_ALL, "ko_KR.UTF-8");
    int x = 1;
    int y = 15;
    int ch;
    int waterLevel = 400; // 초기 수분값 400
    int HP = 100;//초기 hp 100
    int score = 0;
    int i, j;

    int mopx[4];
    int mopy[4];
    int mopban[5] = { 1,1,1,1,1 };
    int holex[4];
    int holey[4];
    int waterx[8];
    int watery[8];
    int itemx[14];
    int itemy[14];
    int itemban[14] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }; // 아이템 존재 여부 초기화
    int water_exists[8] = { 1, 1, 1, 1, 1, 1, 1, 1 }; 
    int isSaltWater[8] = { 0, 0, 0, 0, 0, 0, 1, 1 };    

    srand((unsigned int)time(NULL));

    for (i = 0; i < 4; i++) {
        do {
            mopx[i] = rand() % 70 + 40; // 몬스터 생성 x좌표
            mopy[i] = rand() % 16 + 2;  // 몬스터 생성 y좌표
        } while (isCoordinateOccupied(mopx[i], mopy[i], mopx, mopy, i));
    }
    for (j = 0; j < 4; j++) {
        do {
            holex[j] = rand() % 70 + 40; // 구덩이 생성 x좌표
            holey[j] = rand() % 16 + 2;  // 구덩이 생성 y좌표
        } while (isCoordinateOccupied(holex[j], holey[j], mopx, mopy, 4) || isCoordinateOccupied(holex[j], holey[j], holex, holey, j));
    }
    for (i = 0; i < 8; i++) {
        do {
            waterx[i] = rand() % 70 + 40; // 물 생성 x좌표
            watery[i] = rand() % 16 + 2;  // 물 생성 y좌표
        } while (isCoordinateOccupied(waterx[i], watery[i], mopx, mopy, 4) || isCoordinateOccupied(waterx[i], watery[i], holex, holey, 4) || isCoordinateOccupied(waterx[i], watery[i], waterx, watery, i));
    }
    for (i = 0; i < 14; i++) {
        do {
            itemx[i] = rand() % 70 + 30;  // 아이템 생성 x좌표
            itemy[i] = rand() % 25 + 1;  // 아이템 생성 y좌표
        } while (isCoordinateOccupied(itemx[i], itemy[i], mopx, mopy, 4) || isCoordinateOccupied(itemx[i], itemy[i], holex, holey, 4) || isCoordinateOccupied(itemx[i], itemy[i], waterx, watery, 8) || isCoordinateOccupied(itemx[i], itemy[i], itemx, itemy, i));
    }

    while (1) {
        colorset(14, 0);
        clearScreen();
        if (itemban[7] == 1) {
            gotoxy(x, y);
            printf("옷"); // 어린왕자 캐릭터
        }
        else if (itemban[7] == 0) {
            gotoxy(x, y);
            printf("옷");        
            gotoxy(x, y+1);
            printf("mmm");//낙타
        
        }
        displayWaterLevel(waterLevel); // 현재 수분 표시
        displayHP(HP);
        displayscore(score);
        monster(mopx, mopy,mopban);
        hole(holex, holey);
        water(waterx, watery, water_exists, isSaltWater);
        item(itemx, itemy, itemban);
        if (y == 1 || y == 29 || x == 0 || x == 118) {
            gotoxy(100, 0);
            printf("이 길은 막혀있다.");
        }

        if ((x == holex[0] && y == holey[0]) || (x == holex[1] && y == holey[1]) || (x == holex[2] && y == holey[2])) {
            if (itemban[2] == 0) {
                gotoxy(x, y - 1);
                printf("로프로 구멍을 탈출했다!");
                Sleep(1000);
                x = x + 10;
         }
            x = x - 10; // 뒤로 열 칸
            gotoxy(x, y);
            printf("옷");
        }

        // Check if the character meets the water or salt water
        for (i = 0; i < 8; i++) {
            if (x == waterx[i] && y == watery[i] && water_exists[i]) {
                water_exists[i] = 0; // Mark this water position as removed
                if (isSaltWater[i]) {
                    waterLevel -= 20; // 소금물은 수분 20 감소
                    if (waterLevel < 0) waterLevel = 0; // 수분 최소값은 0
                    displayMessage("으악 소금물이다! 수분-10", 1000);
                }
                else {
                    waterLevel += 60; // 물은 수분 60 증가
                    if (waterLevel > 400) waterLevel = 400; // 수분 최대값은 400
                    displayMessage("시원한 물이다! 수분+30", 1000);
                }
                gotoxy(x, y);
                printf(" "); // Clear the water
            }
        }
        for (i = 0; i < 4; i++) {
            if (x == mopx[i] && y == mopy[i]&&itemban[3]==1) {
                gotoxy(x, y - 1);
                printf("크윽 몬스터녀석 강하군!");
                Sleep(1000);
                HP -= 40;
            }
            else if (x==mopx[i]&&y==mopy[i]&&itemban[3] == 0) {
                mopban[i] = 0;
                gotoxy(x, y - 1);
                printf("벌레퇴치약 사용! 몬스터를 쓰러트렸다 점수 +100");
                Sleep(1000);
                score += 100;
            }
        }
        for (i = 0; i < 2; i++) {
            if (x == itemx[i] && y == itemy[i]&&itemban[i]) {
                itemban[i] = 0;
                gotoxy(x, y - 1);
                printf("사막의 보물을 얻었다! 점수+100");
                Sleep(1000);
                score += 100;              
            }
        }
        if (x == itemx[2] && y == itemy[2] && itemban[2]) {
            itemban[2] = 0;
            gotoxy(x, y - 1);
            printf("로프를 얻었다! 이것만 있으면 구멍따윈 무섭지 않다구~");
            Sleep(1000);
        }
        if (x == itemx[3] && y == itemy[3] && itemban[3]) {
            itemban[3] = 0;
            gotoxy(x, y - 1);
            printf("벌레퇴치제를 얻었다! 학살을 시작하지");
            Sleep(1000);
        }
        if (x == itemx[4] && y == itemy[4] && itemban[4]) {
            itemban[4] = 0;
            gotoxy(x, y - 1);
            printf("쿨링 스카프를 얻었다! 체력+10");
            Sleep(1000);
            HP += 10;
        }
        if (x == itemx[5] && y == itemy[5]&&itemban[5]) {
            itemban[5] = 0;
            gotoxy(x, y - 1);
            printf("모래보호고글을 얻었다! 모래폭풍속에서도 잘 보이겠지?");
            Sleep(1000);
        }
        if (x == itemx[6] && y==itemy[6] && itemban[6]) {
            itemban[6] = 0;
            gotoxy(x, y - 1);
            printf("낙타똥을 얻었다! 장미가 좋아하겠는걸?");
            Sleep(1000);
        }
        if (x == itemx[7] && y == itemy[7] && itemban[7]) {
            itemban[7] = 0;
            gotoxy(x, y - 1);
            printf("낙타를 얻었다! 한번 타볼까?");
            Sleep(1000);
        }
        
        if (x == itemx[8] && y == itemy[8] && itemban[8]) {
                itemban[8] = 0;
                gotoxy(x, y - 1);
                printf("쓰레기잖아.... HP-10");
                Sleep(1000);
                HP -= 10;
            }
        
        if (x == itemx[9] && y == itemy[9] && itemban[9]) {
            itemban[9] = 0;
            gotoxy(x, y - 1);
            printf("모래폭풍을 만났다;; 눈이 안보여 ㅠㅠ");
            Sleep(1000);
        }
        if (x == itemx[10] && y == itemy[10] && itemban[10]) {
            itemban[10] = 0;
            gotoxy(x, y - 1);
            printf("오아시스를 발견했다! 수분을 최대치로!");
            Sleep(1000);
            waterLevel = 400;
        }
        if (x == itemx[11] && y == itemy[11]&&itemban[11]) {
            itemban[11] = 0;
            gotoxy(x, y -1);
            printf("이런 신기루떄문에 깃발을 착각했어!");
            Sleep(1000);    
            
            
        }
        if (x == itemx[12] && y == itemy[12]&&itemban[12]) {
            itemban[12] = 0;
            gotoxy(x, y - 1);
            printf("우와 깃발을 찾았다!");
            Sleep(1000);
          
            
        }
        if (x == itemx[13] && y == itemy[13]&&itemban[13]) {
            itemban[13] = 0;         
            system("cls");
            
                printf("                ,==$$$=!                \n");
                printf("              ~$#: ~; ~#=*              \n");
                printf("             $:=   ~;   $-=~            \n");
                printf("            @.!-   ~;    * *~           \n");
                printf("           @  =          :  *:          \n");
                printf("          ;. $:.$!    :$,~#  *.         \n");
                printf("         ~* =.:~-~.  .~-~-=@  $         \n");
                printf("         =  @ :   -=!    ::@  :-        \n");
                printf("        .:  !-;     -    ~ @   $        \n");
                printf("        $,   $#   -#:    :#    !        \n");
                printf("        #     *     !    *     ,~       \n");
                printf("        ~     !   ;!,    $      @       \n");
                printf("       ,,     !,         !      @       \n");
                printf("       @.     :;         !      *,      \n");
                printf("       @-----  !        -! ------,      \n");
                printf("        ,=  .  @@@@@@@@#@- .  -$,       \n");
                printf("         =     ,$       :     -$        \n");
                printf("         =     ,$       :     -$        \n");
                printf("         =  ,   :       ,  -  -$        \n");
                printf("         =.;*              !*,-$        \n");
                printf("         =#                  ==$        \n");
                printf("        ,;                    .$        \n");
                printf("        $#$$.     $$$$$$$$  $$$@        \n");
                printf("       .~                      .=       \n");
                printf("       @.                       =.      \n");
                printf("      -#$$$$$$$$*        .$$$$$$$;      \n");
                printf("      #                          =.     \n");
                printf("     :*                           :     \n");
                printf("    .$.......               ......=.    \n");
                printf("    *::::::::               :::::::!*   \n");
                printf("    :              :=;             -*   \n");
                printf("    :              ! $             -*   \n");
                printf("    @#@@@~   $@@   ! $  !@@@@~     -*   \n");
                printf("    :              . $             -*   \n");
                printf("    :                $             -*   \n");
                printf("    :              ~ $             -*   \n");
                printf("  @***=***$#****   ! $  ****@=***=***@  \n");
                printf("  @   -   ~$   *   ! $  *   $-   ,   @  \n");
                printf("  @   -   ~$   *   ! $  *   $-   ,   @  \n");
                printf("  @   -   ~$   *   !    *   $-   ,   @  \n");
                printf("~~@   -   ~$   *   ! ;  *   $-   ,   @~~\n");
                printf("  @   -   ~$   $:::$:#::$   $-   ,   @  \n");
                printf("  @   -   ~$   =--------=   $-   ,   @  \n");
                printf("~~@~~~;~~~!$~~~=~~~     *~~~#;~~~;~~:@~~\n");
                printf("                                        \n");
                int ch;
                gotoxy(60, 0);
                printf("나는 스핑크스다.");
                gotoxy(60, 1);
                printf("지금부터 너에게 문제를 줄것이다.");
                gotoxy(60, 2);
                printf("맞춘다면 어마어마한 상을 틀리면 벌을 주마");
                gotoxy(60, 3);
                printf("이 보물찾기 게임의 주제는 무엇일까?(숫자만입력하시오)");
                gotoxy(60, 4);
                printf("1.어린공주");
                gotoxy(60, 5);
                printf("2.큰왕자");
                gotoxy(60, 6);
                printf("3.어린왕자");
                gotoxy(60, 7);
                printf("4.큰공주");
                gotoxy(60, 8);
                printf("답:");
                scanf_s("%d", &ch);
                if (ch == 3) {
                    gotoxy(60, 4);
                    printf("정답이다 어린왕자여 하하핳 점수 500점을 주마");
                    Sleep(2000);
                    score += 500;
                }
                else {
                    gotoxy(60, 4);
                    printf("정신이 나갔구나 이걸 틀리다니 벌로 점수 -100을 내리마");
                    Sleep(2000);
                    score -= 100;
                }

             
           
        }
        for (i = 0; i < 14; i++) {
            if (x == itemx[i] && y == itemy[i] && itemban[i]) {
                itemban[i] = 0;
                gotoxy(x, y);
                printf(" ");
            }
        }
        ch = _getch();
        if (ch == 224&&itemban[7]==1) {
            ch = _getch();
            switch (ch) {
            case 72: // 위 방향키
                y = (y > 1) ? y - 1 : 1;
                waterLevel-=2; // 수분 감소
                break;
            case 80: // 아래 방향키
                y = (y < HEIGHT - 1) ? y + 1 : 29;
                waterLevel-=2; // 수분 감소
                break;
            case 75: // 왼쪽 방향키
                x = (x > 0) ? x - 1 : 0;
                waterLevel-=2; // 수분 감소
                break;
            case 77: // 오른쪽 방향키
                x = (x < WIDTH - 2) ? x + 1 : 118;
                waterLevel-=2; // 수분 감소
                break;
            }
        }
        else if (ch == 224 && itemban[7] == 0) {
            ch = _getch();
            switch (ch) {
            case 72: // 위 방향키
                y = (y > 1) ? y - 1 : 1;
                waterLevel -= 1; // 수분 감소
                break;
            case 80: // 아래 방향키
                y = (y < HEIGHT - 1) ? y + 1 : 29;
                waterLevel -= 1; // 수분 감소
                break;
            case 75: // 왼쪽 방향키
                x = (x > 0) ? x - 1 : 0;
                waterLevel -= 1; // 수분 감소
                break;
            case 77: // 오른쪽 방향키
                x = (x < WIDTH - 2) ? x + 1 : 118;
                waterLevel -= 1; // 수분 감소
                break;
            }
        }
        
        else if (ch == 27) { // ESC 키로 종료
            break;
        }

        if (waterLevel <= 0) {
            gotoxy(50, 15);
            printf("탈수 상태에 빠졌습니다! 게임 오버\n");
            break;
        }
    }
    return 0;
}

void monster(int mopx[], int mopy[], int mopban[]) {
    int i;

    for (i = 0; i < 2; i++) {
        if (mopban[i]) {
            colorset(14, 4);
            gotoxy(mopx[i], mopy[i]);
            printf("*"); // 지네
        }
    }
    for (i = 2; i < 4; i++) {
        if (mopban[i]) {
            colorset(14, 0);
            gotoxy(mopx[i], mopy[i]);
            printf("*"); // 전갈
        }
    }
}

void hole(int holex[], int holey[]) {
    int j;
    for (j = 0; j < 3; j++) {
        gotoxy(holex[j], holey[j]);
        printf("o");
    }
}

void water(int waterx[], int watery[], int water_exists[], int isSaltWater[]) {
    int i;
    for (i = 0; i < 8; i++) {
        if (water_exists[i]) {
            colorset(14, 1); // 물과 소금물 모두 파란색으로 표시
            gotoxy(waterx[i], watery[i]);
            printf("i"); // 물 + 소금물
        }
    }
}
void item(int itemx[], int itemy[], int itemban[]) {
    setlocale(LC_ALL, "ko_KR.UTF-8");
    int i;
    for (i = 0; i < 11; i++) {//로프1개, 낙타1개, 살충제1개, 오아시스깃발1개, 스핑크스깃발1개, 진짜깃발1개, 가짜깃발1개, 모래보호고글1개, 쿨링스카프1개,  
        if (itemban[i]) {          //모래폭풍1개, 보물2개, 낙타똥1개
            colorset(14, 3);        //아이템은 하늘색
            gotoxy(itemx[i], itemy[i]);
            printf("x");
        }
    }
    for (i = 10; i < 14; i++) {
        if (itemban[i]) {
            colorset(14, 7);
            gotoxy(itemx[i], itemy[i]);
            printf("\u25B6");
        }
    }
}
