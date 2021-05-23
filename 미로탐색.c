
/*
 * 자료구조 과제 2
 * 스택/큐
 *
 * 2020-04-03
 * 만든이: 20175154 김태현
 *
 * version 1.1
 * 저장된 미로를 찾아가는 프로그램. 두 가지 스택을 사용하여 하나는 갈림길의 위치를, 하나는 지금까지의 경로를 저장한다.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100
#define TRUE 1
#define FALSE 0

//미로의 가로세로 길이(칸의 개수)
#define WIDTH 9
#define HEIGHT 12

//벽은 열림, 또는 닫힘 값을 갖는다.
#define OPEN 1
#define CLOSE 0

#define PAST 2

//열린 벽의 방향
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

typedef int bool;
typedef int IsOpen;

//경로의 길이를 저장하는 변수
int path_length = 0;

//스택의 사용 횟수를 저장하는 변수
int how_many_using_stack_count = 0;

/*
 * 가장 빠른 길에서의 위치를 저장할 구조체
 */
typedef struct{
    int x;
    int y;
}Coord;

/*
 * 스택 요소로 저장될 구조체
 */
typedef struct{
    int x;
    int y;
    int open_direction_list[4];
    //그 지점까지 계산된 길의 길이
    int path_length;
}Element;

/*
 *  스택 구조체
 */
typedef struct{
    Element element[MAX_STACK_SIZE];
    int top;
}Stack, *PStack;

/*
 * 스택을 동적으로 할당해여 그 포인터를 반환하는 함수
 */
PStack CreateStack(){
    PStack stack = malloc(sizeof(Stack));
    stack->top = -1;
    return stack;
}

/*
 * 스택 연산 함수들
 */
bool IsStackEmpty(PStack stack){
    if (stack->top == -1){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

bool IsStackFull(PStack stack){
    if (stack->top == MAX_STACK_SIZE-1){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

void Push(PStack stack, Element data){
    if (IsStackFull(stack)){
        printf("오류: 스택이 모두 찼습니다.");
        exit(8);
    }
    stack->element[++stack->top] = data;
}

Element Pop(PStack stack){
    if (IsStackEmpty(stack)){
        printf("오류: 스택이 비었습니다.");
        exit(9);
    }
    return stack->element[stack->top--];
}

Element Peek(PStack stack){
    if (IsStackEmpty(stack)){
        printf("오류: 스택이 비어있습니다. ");
        exit(7);
    }
    return stack->element[stack->top];
}

//스택이 해당 위치를 갖고 있는지 체크하는 함수. 이미 갔던 길인지 체크하는데 사용한다.
bool IsStackHasCoord(PStack stack, int x, int y){
    for (int i = 0; i <= stack->top; ++i) {
        if ((stack->element[i].x == x) && (stack->element[i].y == y)){
            return TRUE;
        }
    }
    return FALSE;
}


/*
 * 가로벽과 세로벽을 정의한다.
 */
IsOpen horizontal[HEIGHT + 1][WIDTH];
IsOpen vertical[HEIGHT][WIDTH + 1];

/*
 * 가로벽을 만드는 함수. 매개변수로 0번 벽부터 8번 벽까지의 상태 값을 받는다.
 */
void CreateMazeHorizontal(int row, IsOpen a, IsOpen b, IsOpen c, IsOpen d, IsOpen e, IsOpen f, IsOpen g, IsOpen h, IsOpen i){
    horizontal[row][0] = a;
    horizontal[row][1] = b;
    horizontal[row][2] = c;
    horizontal[row][3] = d;
    horizontal[row][4] = e;
    horizontal[row][5] = f;
    horizontal[row][6] = g;
    horizontal[row][7] = h;
    horizontal[row][8] = i;
}
/*
 * 세로벽을 만드는 함수. 매개변수로 0번 벽부터 9번 벽까지의 상태 값을 받는다.
 */
void CreateMazeVertical(int row, IsOpen a, IsOpen b, IsOpen c, IsOpen d, IsOpen e, IsOpen f, IsOpen g, IsOpen h, IsOpen i, IsOpen j){
    vertical[row][0] = a;
    vertical[row][1] = b;
    vertical[row][2] = c;
    vertical[row][3] = d;
    vertical[row][4] = e;
    vertical[row][5] = f;
    vertical[row][6] = g;
    vertical[row][7] = h;
    vertical[row][8] = i;
    vertical[row][9] = j;
}

/*
 * 전체 미로를 구현하는 함수. 가로벽과 세로벽을 번갈아가며 정의한다.
 */
void CreateMaze(){
    CreateMazeHorizontal(0, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE);
    CreateMazeVertical(0, CLOSE, OPEN, OPEN, OPEN, CLOSE, OPEN, OPEN, OPEN, CLOSE, CLOSE);
    CreateMazeHorizontal(1, OPEN, CLOSE, CLOSE, OPEN, CLOSE, CLOSE, OPEN, OPEN, OPEN);
    CreateMazeVertical(1, CLOSE, OPEN, OPEN, CLOSE, CLOSE, OPEN, OPEN, CLOSE, CLOSE, CLOSE);
    CreateMazeHorizontal(2, CLOSE, CLOSE, OPEN, OPEN, OPEN, CLOSE, OPEN, OPEN, OPEN);
    CreateMazeVertical(2, CLOSE, OPEN, OPEN, CLOSE, OPEN, CLOSE, OPEN, CLOSE, CLOSE, CLOSE);
    CreateMazeHorizontal(3, OPEN, CLOSE, OPEN, CLOSE, CLOSE, OPEN, CLOSE, OPEN, OPEN);
    CreateMazeVertical(3, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, OPEN, OPEN, CLOSE);
    CreateMazeHorizontal(4, OPEN, OPEN, CLOSE, OPEN, OPEN, CLOSE, OPEN, CLOSE, OPEN);
    CreateMazeVertical(4, CLOSE, CLOSE, CLOSE, OPEN, CLOSE, OPEN, OPEN, CLOSE, OPEN, CLOSE);
    CreateMazeHorizontal(5, OPEN, OPEN, OPEN, OPEN, CLOSE, CLOSE, CLOSE, OPEN, OPEN);
    CreateMazeVertical(5, CLOSE, OPEN, CLOSE, CLOSE, OPEN, OPEN, CLOSE, OPEN, CLOSE, CLOSE);
    CreateMazeHorizontal(6, OPEN, CLOSE, OPEN, OPEN, CLOSE, CLOSE, CLOSE, CLOSE, OPEN);
    CreateMazeVertical(6, CLOSE, OPEN, OPEN, CLOSE, CLOSE, OPEN, OPEN, OPEN, OPEN, CLOSE);
    CreateMazeHorizontal(7, CLOSE, CLOSE, CLOSE, OPEN, OPEN, CLOSE, CLOSE, OPEN, OPEN);
    CreateMazeVertical(7, CLOSE, CLOSE, OPEN, OPEN, CLOSE, CLOSE, OPEN, OPEN, CLOSE, CLOSE);
    CreateMazeHorizontal(8, OPEN, OPEN, CLOSE, CLOSE, OPEN, OPEN, CLOSE, CLOSE, CLOSE);
    CreateMazeVertical(8, CLOSE, CLOSE, OPEN, OPEN, OPEN, CLOSE, OPEN, OPEN, OPEN, CLOSE);
    CreateMazeHorizontal(9, OPEN, CLOSE, CLOSE, OPEN, CLOSE, CLOSE, OPEN, CLOSE, OPEN);
    CreateMazeVertical(9, CLOSE, OPEN, OPEN, CLOSE, OPEN, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE);
    CreateMazeHorizontal(10, OPEN, CLOSE, OPEN, OPEN, OPEN, OPEN, CLOSE, OPEN, OPEN);
    CreateMazeVertical(10, CLOSE, OPEN, CLOSE, CLOSE, CLOSE, CLOSE, OPEN, OPEN, CLOSE, CLOSE);
    CreateMazeHorizontal(11, CLOSE, OPEN, OPEN, OPEN, OPEN, OPEN, CLOSE, CLOSE, OPEN);
    CreateMazeVertical(11, CLOSE, OPEN, CLOSE, OPEN, CLOSE, OPEN, CLOSE, OPEN, OPEN, CLOSE);
    CreateMazeHorizontal(12, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE, CLOSE);
}

/*
 * 한 칸의 x좌표와 y좌표를 매개변수로 받아 그 칸에 뚫린 벽 방향을 direction_list 배열에 저장하고 그 개수를 반환하는 함수.
 */
int WhatDirectionOfOpenWall(int x, int y, int direction_list[], bool* is_direction_exist, PStack previous_step_stack){
    int open_wall_count = 0;

    //만약 길이 없어서 다시 돌아온 경우라면 스택에서 꺼낸 요소의 열린 방향을 수정하지 않고 사용한다.
    //이미 간 길은 닫힘으로 표시되어있다.
    if (!(*is_direction_exist)){
        for (int i = 0; i < 4; ++i) {
            if (direction_list[i] == OPEN){
                open_wall_count++;
            }
        }
        return open_wall_count;
    }
    for (int i = 0; i < 4; ++i) {
        direction_list[i] = CLOSE;
    }

    //만약 지금까지의 경로 스택에 목적지 좌표가 존재한다면
    //이는 왔던 길로 다시 돌아온 것.
    //따라서 그 좌표로는 가지 않도록 해야한다.
    //up
    if ((horizontal[y][x] == OPEN) && (!IsStackHasCoord(previous_step_stack, x, y-1))){
        direction_list[UP] = OPEN;
        open_wall_count++;
    }
    //down
    if ((horizontal[y+1][x] == OPEN) && (!IsStackHasCoord(previous_step_stack, x, y+1))){
        direction_list[DOWN] = OPEN;
        open_wall_count++;
    }
    //left
    if ((vertical[y][x] == OPEN) && (!IsStackHasCoord(previous_step_stack, x-1, y))){
        direction_list[LEFT] = OPEN;
        open_wall_count++;
    }
    //right
    if ((vertical[y][x+1] == OPEN) && (!IsStackHasCoord(previous_step_stack, x+1, y))){
        direction_list[RIGHT] = OPEN;
        open_wall_count++;
    }
    return open_wall_count;
}

/*
 * 스택에 위치의 x좌표와 y좌표, 그리고 열린 방향 배열을 저장하기 위해서는 이들을 Element 구조체로 묶어야 한다.
 * 이를 묶어서 반환하는 함수.
 */
void MakeElement(Element* element, int x, int y, int open_direction[]){
    element->x = x;
    element->y = y;
    //방향을 저장. 방향은 네 방향 밖에 없다.
    for (int i = 0; i < 4; ++i) {
        element->open_direction_list[i] = open_direction[i];
    }
    //해당 지점까지의 길이를 저장한다.
    element->path_length = path_length;
}

/*
 * 주어진 방향에 따라 현재 x, y 값을 바꾸는 함수.
 * 즉 해당 방향으로 이동하는 함수이다.
 */
void MoveDirection(int direction, int *x, int *y, bool *is_direction_exist, PStack previous_step_stack){


    //up
    if (direction == UP){
        (*y)--;
        *is_direction_exist = TRUE;
    }
        //down
    else if (direction == DOWN){
        (*y)++;
        *is_direction_exist = TRUE;
    }
        //left
    else if (direction == LEFT){
        (*x)--;
        *is_direction_exist = TRUE;
    }
        //right
    else if (direction == RIGHT){
        (*x)++;
        *is_direction_exist = TRUE;
    }
    else{
        printf("direction error!");
        exit(4);
    }
    //경로의 길이 ++
    path_length++;
    //경로 스택에 해당 위치 저장
    Element temp;
    temp.x = *x;
    temp.y = *y;
    Push(previous_step_stack, temp);
}

/*
 * 미로의 시작점과 도착점을 좌표로 받고 길을 찾아 길의 개수를 반환하는 함수
 */
int FindPath(Coord min_road_arr[], int start_x, int start_y, int end_x, int end_y){
    PStack stack = CreateStack();

    //지나온 경로를 저장할 스택
    PStack previous_step_stack = CreateStack();

    //길의 최소값을 저장하는 변수
    int min_length=0;

    //해당 위치가 일반적인 진행인지,
    //아니면 막다른 길에 다다라서 스택에서 pop해 온 위치인지 나타내는 변수
    //TRUE라면 일반적인 진행을 뜻한다.
    bool is_direction_exist = TRUE;

    //현재 칸의 좌표
    int x, y;
    x = start_x;
    y = start_y;

    //시작점을 경로 스택에 push.
    Element init_coord;
    init_coord.x = x;
    init_coord.y = y;
    Push(previous_step_stack, init_coord);

    //열린 방향을 저장할 배열
    int open_direction[4];

    //스택에 넣을 임시 구조체. 갈림길 위치와 열린 방향으로 구성됨.
    Element cross = {0, };

    //갈 수 있는 방향의 개수
    int direction_count = 0;

    //반환할 미로 경로의 개수
    int path_count = 0;

    //스택이 빌 때까지 모든 길을 검색
    while (TRUE) {
        while ((x != end_x) || (y != end_y)) {

            //현재 위치에서 열 린 곳이 몇개인지, 어디인지 탐색.
            direction_count = WhatDirectionOfOpenWall(x, y, open_direction, &is_direction_exist, previous_step_stack);

            //열린 곳이 지금까지 지나온 길을 제외하고 없다면
            //스택에서 꺼내어 되돌아가야함.
            if (direction_count == 0) {
                //되돌아갈 갈림길이 더 이상 존재하지 않을 때
                if (IsStackEmpty(stack)) {
                    free(stack);
                    free(previous_step_stack);
                    printf("스택 사용 횟수: %d\n", how_many_using_stack_count);
                    printf("모두 %d개의 길을 찾았습니다. \n", path_count);
                    printf("가장 짧은 길을 표시합니다. \n");
                    return min_length;
                }

                cross = Pop(stack);
                x = cross.x;
                y = cross.y;
                how_many_using_stack_count++;
                printf("POP(%d, %d)\n", x, y);
                for (int i = 0; i < 4; ++i) {
                    open_direction[i] = cross.open_direction_list[i];
                }
                //길의 길이를 해당 지점부터 다시 세야한다.
                path_length = cross.path_length;

                is_direction_exist = FALSE;

                //길 스택에 저장된 위치들을 다시 돌아간 갈림길이 맨 위로 올라올 때까지 Pop
                while ((Peek(previous_step_stack).x != x) || (Peek(previous_step_stack).y != y)) {
                    Pop(previous_step_stack);
                }
            }
                //열린 곳이 지나온 곳 제외 한 곳이라면
            else if (direction_count == 1) {
                for (int i = 0; i < 4; ++i) {
                    if (open_direction[i] == OPEN) {
                        //그 곳으로 이동
                        MoveDirection(i, &x, &y, &is_direction_exist, previous_step_stack);
                        break;
                    }
                }
            }
                //열린 곳이 두 곳 이상이라면
                //이 위치와 방향을 스택에 넣고 그 중 한 방향을 임의로 따라간다.
                //간 방향은 막힘 처리 되어 스택에 저장된다.
            else if (direction_count > 1) {
                MakeElement(&cross, x, y, open_direction);

                for (int i = 0; i < 4; ++i) {
                    if (cross.open_direction_list[i] == OPEN) {
                        //한 열린 곳을 가고
                        MoveDirection(i, &x, &y, &is_direction_exist, previous_step_stack);
                        //그 방향을 열린 방향 리스트에서 제거
                        cross.open_direction_list[i] = CLOSE;
                        break;
                    }
                }
                //다른 갈림길이 존재하는 그 위치를 스택에 다시 넣는다.
                Push(stack, cross);
                how_many_using_stack_count++;
                printf("PUSH(%d, %d)\n", cross.x, cross.y);
            }
        }

        //현재 위치의 좌표가 도착 위치와 좌표와 같다면
        if ((x == end_x) && (y == end_y)) {
            printf("도착!\n");
            path_count++;
            //최소 거리가 0(처음 도착)이거나 저장된 최소거리보다 더 작은 거리가 확인되었을 경우
            if ((min_length > path_length) || (min_length == 0)) {
                min_length = path_length;
                //최단거리 경로를 저장
                for (int i = 0; i <= previous_step_stack->top; ++i) {
                    min_road_arr[i].x = previous_step_stack->element[i].x;
                    min_road_arr[i].y = previous_step_stack->element[i].y;
                }
            }
            //스택에 갈림길이 남아있지 않다면 모두 탐색한 것이다.
            if (IsStackEmpty(stack)) {
                free(stack);
                free(previous_step_stack);
                printf("스택 사용 횟수: %d\n", how_many_using_stack_count);
                printf("모두 %d개의 길을 찾았습니다. \n", path_count);
                printf("가장 짧은 길을 표시합니다. \n");
                return min_length;
            }
                //스택에 갈림길이 남아있으면 다시 되돌아가서 탐색해야 한다.
            else {
                Element temp;
                temp = Pop(stack);
                x = temp.x;
                y = temp.y;
                for (int i = 0; i < 4; ++i) {
                    open_direction[i] = temp.open_direction_list[i];
                }
                //해당 위치부터 경로의 길이를 다시 센다.
                path_length = temp.path_length;
                is_direction_exist = FALSE;

                //지나온 경로 스택에서 해당 위치까지 pop한다. 그 위치부터 다시 기록할 것.
                while ((Peek(previous_step_stack).x != x) || (Peek(previous_step_stack).y != y)) {
                    Pop(previous_step_stack);
                }
            }
        }
    }

}

//배열에 해당 x, y값이 존재하는지 여부를 반환하는 함수. 출력을 위해 사용함.
bool IsArrHasXY(int arr_size, Coord arr[], int x, int y){
    for (int i = 0; i < arr_size + 1; ++i) {
        if ((arr[i].x == x) && (arr[i].y == y)){
            return TRUE;
        }
    }
    return FALSE;
}

//미로를 출력하는 함수
void PrintMaze(int min_path_size, Coord min_path[], int start_x, int start_y, int end_x, int end_y){
    for (int i = 0; i < HEIGHT; ++i) {
        //가로벽
        for (int j = 0; j < WIDTH; ++j) {
            if (horizontal[i][j] == OPEN){
                printf("    ");
            }
            else if (horizontal[i][j] == CLOSE){
                printf(" ── ");
            }
        }
        printf("\n");
        //세로벽
        for (int j = 0; j < WIDTH+1; ++j) {
            if (vertical[i][j] == OPEN){
                printf("  ");
            }
            else if (vertical[i][j] == CLOSE){
                printf("│ ");
            }
            if ((start_x == j) && (start_y == i)){
                printf("S ");
            }
            else if ((end_x == j) && (end_y == i)){
                printf("E ");
            }
                //최단거리 경로 배열에 해당 좌표가 저장되어 있을 경우, 이를 표시해준다.
            else if (IsArrHasXY(min_path_size, min_path, j, i)){
                printf("* ");
            }
            else{
                printf("  ");
            }
        }
        printf("\n");
    }
    //마지막 가로벽.
    for (int j = 0; j < WIDTH; ++j) {
        if (horizontal[HEIGHT][j] == OPEN){
            printf("    ");
        }
        else if (horizontal[HEIGHT][j] == CLOSE){
            printf(" ── ");
        }
    }
    printf("\n");
}

int main(){
    //최단거리 경로를 저장할 구조체 배열.
    Coord min_path[MAX_STACK_SIZE];
    //미로 생성.
    CreateMaze();
    //최단거리 값을 저장할 변수.
    int min_path_length;
    //시작점과 도착점
    int start_x = 0; int start_y = 0;
    int end_x = 8; int end_y = 11;
    //미로찾기 시작.
    min_path_length = FindPath(min_path, start_x, start_y, end_x, end_y);
    //최단거리가 0(초기값)이라는 것은 한 번도 도착지점에 도달하지 못한 것.
    if (min_path_length != 0) {
        printf("거리: %d\n", min_path_length);
    }
    else{
        printf("가능한 경로가 존재하지 않습니다!\n");
    }
    PrintMaze(min_path_length, min_path, start_x, start_y, end_x, end_y);

    return 0;
}