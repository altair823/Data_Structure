/*
 * 자료구조 과제 3
 * 스택/큐
 *
 * 2020-04-04
 * 만든이: 20175154 김태현
 *
 *
 * 정해진 시간마다 큐에 알파벳을 넣고 받은 입력 만큼 큐에서 꺼내는 프로그램
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_QUEUE_SIZE 21

#define TRUE 1
#define FALSE 0

typedef int bool;

//시간을 기록할 변수
time_t last_time = 0;

/*
 * 큐 요소
 */
typedef char Element;

/*
 * 큐 구조체
 */
typedef struct{
    Element element[MAX_QUEUE_SIZE];
    int front;
    int rear;
}Queue, *PQueue;

/*
 * 큐를 동적할당으로 생성하여 그 포인터를 반환.
 */
PQueue CreateQueue(){
    PQueue queue = malloc(sizeof(Queue));
    queue->rear = 0;
    queue->front = 0;
    return queue;
}

/*
 * 큐와 관련된 연산들
 */
bool IsQueueEmpty(PQueue queue){
    if (queue->front == queue->rear){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

bool IsQueueFull(PQueue queue){
    //큐의 한 자리를 버리는 대신 rear가 front 바로 뒤로 오면 다 찬 것으로 여김.
    if ((queue->rear + 1) % MAX_QUEUE_SIZE == queue->front){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

bool Enqueue(PQueue queue, Element data){
    if (IsQueueFull(queue)){
        printf("오류: 큐가 모두 찼습니다! \n");
        return FALSE;
    }
    else{
        queue->rear = (queue->rear+1) % MAX_QUEUE_SIZE;
        queue->element[queue->rear] = data;
        return TRUE;
    }
}

Element Dequeue(PQueue queue){
    if (IsQueueEmpty(queue)){
        return '\0';
    }
    else{
        queue->front = (queue->front+1) % MAX_QUEUE_SIZE;
        return queue->element[queue->front];
    }
}

/*
 * 현재 큐의 내용과 그 개수를 출력하는 함수.
 */
void StateOfQueue(PQueue queue){
    int count = 0;
    printf("QUEUE = ");
    int pos = (queue->front + 1) % MAX_QUEUE_SIZE;
    while (pos != (queue->rear + 1) % MAX_QUEUE_SIZE){
        printf("%c", queue->element[pos]);
        pos = (pos + 1) % MAX_QUEUE_SIZE;
        count++;
    }
    printf("(%d)\n", count);
}

/*
 * 바로 전에 기록한 시간으로부터 얼마나 지났는지 정수로 반환하고 현재 시간을 기록하는 함수.
 */
int TimeCheck(){
    time_t current_time;
    int time_difference;

    if (last_time == 0){
        time(&last_time);
        return 0;
    }
    else{
        time_difference = (int)(time(&current_time) - last_time);
        last_time = current_time;
        return time_difference;
    }
}

/*
 * 프로그램 시작, 또는 방금 전 입력으로부터 지난 시간(초)만큼
 * 큐에 알파벳을 차례대로 넣는 함수.
 * 입력 받은 개수만큼 알파벳을 큐에서 빼내어 출력한다.
 */
void QueueHandler(){
    PQueue queue = CreateQueue();
    int time_difference;

    //큐에 차례대로 들어갈 알파벳들.
    char alphabet[26] ={'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    int alphabet_index = 0;

    //큐에서 뺀 것을 저장할 결과 배열.
    char result[26];
    int result_index = 0;

    //시작시간 체크.
    TimeCheck();
    printf("시스템이 시작됩니다.\n");

    while (TRUE){
        int input;
        printf(">>> ");
        scanf("%d", &input);

        //0이 입력되면 큐의 정보를 출력.
        //단지 dequeue하지 않을 뿐, enqueue는 해야함.
        if (input == 0){
            time_difference = TimeCheck();
            for (int i = 0; i < time_difference; ++i) {
                printf("(SYSTEM) ADDQUEUE(%c)  F=%d R=%d\n", alphabet[alphabet_index % 26], queue->front, queue->rear);
                Enqueue(queue, alphabet[(alphabet_index++) % 26]);
            }
            StateOfQueue(queue);
        }
        else{
            time_difference = TimeCheck();
            //addqueue
            for (int i = 0; i < time_difference; ++i) {
                printf("(SYSTEM) ADDQUEUE(%c)  F=%d R=%d\n", alphabet[alphabet_index % 26], queue->front, queue->rear);
                Enqueue(queue, alphabet[(alphabet_index++) % 26]);
            }

            //deletequeue
            for (int i = 0; i < input; ++i) {
                result[result_index] = Dequeue(queue);
                printf("DELETEQUEUE() ");
                if (result[result_index] != '\0') {
                    printf("= %c, F=%d R=%d\n", result[result_index], queue->front, queue->rear);
                    result_index++;
                } else{
                    printf("FAIL. QueueEmpty\n");
                }
            }

            //deletequeue 한 결과 출력
            printf("RESULT = ");
            for (int i = 0; i < result_index; ++i) {
                printf("%c", result[i]);
            }
            result_index = 0;

            printf("\n");

            StateOfQueue(queue);
        }
    }

};


int main(){
    QueueHandler();
    return 0;
}