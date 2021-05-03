/*
 * 자료구조 과제 4
 * 정렬
 *
 * 2020-04-28
 * 만든이: 20175154 김태현
 *
 * version 0.5
 *
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0

//데이터 개수와 그 출력 결과의 간격.
#define STUDENT_COUNT 50000
#define PRINT_GAP 1000

//중복된 값의 탐색을 활성화 시키는 매크로.
#define DUPLICATE_VALUE_CHECK

//각 정렬 방법을 활성화 시키는 매크로.
#define QUICK_SORT_C
#define SELECTION_SORT
#define QUICK_SORT_RECURSIVE
#define QUICK_SORT_STACK
#define QUICK_SORT_SHUFFLE
#define HEAP_SORT

#define SWAP(a, b, type) do { \
    type t; \
    t = a;  \
    a = b;     \
    b = t;  \
} while (0)

typedef int bool;

clock_t execute_time;

//시간을 측정하는 함수
void TimeCheckStart(){
    execute_time = clock();
}
void TimeCheckEnd(){
    clock_t end = clock();
    printf("excute time is %f\n", (double)(end - execute_time) / CLOCKS_PER_SEC);
    execute_time = clock();
}

//학생 데이터는 이 레코드 형식을 따라 저장된다.
typedef struct{
    int student_ID;
    char name[10];
    int phone_number[11];
}student_data;


/*
 * 스택 구현
 */
typedef struct{
    int left;
    int right;
}Element;

typedef struct{
    Element *element;
    int top;
    int size;
}Stack, *Pstack;

Pstack CreateStack(){
    Pstack stack = malloc(sizeof(Stack));
    stack->top = 0;
    stack->size = 0;
    return stack;
}

bool IsStackEmpty(Pstack stack){
    if (stack->top == 0){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

bool IsStackFull(Pstack stack){
    if (stack->size == stack->top){
        return TRUE;
    }
    else {
        return FALSE;
    }
}

/*
 * 스택에 넣는 함수.
 * 맨 처음 스택에 1 만큼의 크기를 할당하고
 * 이후 할당된 공간을 모두 채웠다면 전에 할당한 공간의 두 배를 다시 할당한다.
 */
void Push(Pstack stack, Element data){
    if (IsStackEmpty(stack) && stack->size == 0){
        stack->element = malloc(sizeof(Element) * 1);
        stack->size = 1;
    }
    else if (IsStackFull(stack)){
        stack->element = realloc(stack->element, sizeof(Element) * ((stack->top)*2));
        stack->size = (stack->top) * 2;
    }
    stack->element[stack->top++] = data;
}

Element Pop(Pstack stack){
    if (IsStackEmpty(stack)){
        printf("오류: 스택이 비었습니다. \n");
        exit(10);
    }
    return stack->element[--stack->top];
}

void DestructStack(Pstack stack){
    free(stack->element);
    free(stack);
}

Element CreateElement(int left, int right){
    Element temp;
    temp.left = left;
    temp.right = right;
    return temp;
}

void PrintStackSize(Pstack stack){
    printf("stack size is %d\n", stack->size);
}

/*
 * 중복값의 존재를 일일이 검사하는 학번 제조 함수
 * 너무 느린 것을 확인했다.
 */
void RandomizeStudentID_OneByOne(int size, student_data *studentData){
    for (int i = 0; i < size;) {
        bool is_same_ID_exist = FALSE;
        srand((unsigned int) clock());
        int year = rand() % (21 + 1 - 13) + 13;
        studentData[i].student_ID = 20000000;
        studentData[i].student_ID += year * 10000;

        srand((unsigned int) clock());
        int rand_4_digit = rand() % 10000;
        studentData[i].student_ID += rand_4_digit;

        for (int j = 0; j < i; ++j) {
            if (studentData[i].student_ID / 10000 == studentData[j].student_ID / 10000) {
                if (studentData[i].student_ID % 10000 == studentData[j].student_ID % 10000){
                    is_same_ID_exist = TRUE;
                    break;
                }
            }
        }
        if (is_same_ID_exist == FALSE){
            i++;
        }
    }
}

/*
 * 배열을 섞는 함수.
 */
void ShuffleArray(int size, int *number_table){
    srand((unsigned int) clock());
    int randIndex;

    for (int i = 0; i < size-1; ++i) {
        //i번째 요소와 i+1 ~ size범위에 있는 요소를 바꾼다.
        randIndex = rand() % (size - i) + i;
        SWAP(number_table[i], number_table[randIndex], int);
    }
}

/*
 * 난수 테이블을 만들고 그 테이블 앞에서부터 하나씩 빼서 학번에 넣는 함수.
 */
void RandomizeStudentID_Table(int size, student_data *studentData){

    //난수 테이블을 만든다.
    //이 난수들은 학번 뒤 4자리에 들어갈 것.
    int number_table[10000];
    for (int i = 0; i < 10000; ++i) {
        number_table[i] = i;
    }

    //각 학년 학생 수를 결정한다.
    int student_count[9] = {0,};
    int current_grade;
    for (int i = 0; i < size; ++i) {
        current_grade = rand() % 9;
        student_count[current_grade]++;
    }

    //각 학년에 생성한 난수 테이블을 섞고 해당 학년 학생 수 만큼 대입한다.
    int student_data_index = 0;
    for (int student_grade = 0; student_grade < 9; ++student_grade) {
        ShuffleArray(10000, number_table);
        for (int i = 0; i < student_count[student_grade]; ++i) {
            studentData[student_data_index].student_ID = 20000000;
            studentData[student_data_index].student_ID += (student_grade + 13) * 10000;
            studentData[student_data_index++].student_ID += number_table[i];
        }
    }

}

/*
 * 무작위 이름을 생성하는 함수.
 */
void RandomizeName(int size, student_data studentData[]){
    srand((unsigned int) clock());
    char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < 10; ++j) {
            studentData[i].name[j] = alphabet[rand() % 26];
        }
    }
}

/*
 * 무작위 전화번호를 생성하는 함수.
 */
void RandomizePhoneNumber(int size, student_data *studentData){
    srand((unsigned int) clock());
    for (int i = 0; i < size; ++i) {
        studentData[i].phone_number[0] = 0;
        studentData[i].phone_number[1] = 1;
        studentData[i].phone_number[2] = 0;
        for (int j = 3; j < 11; ++j) {
            studentData[i].phone_number[j] = rand() % 10;
        }
    }
}

/*
 * qsort 함수를 위한 비교 함수.
 */
int Compare(const void *a, const void *b){
    student_data student1 = *(student_data *)a;
    student_data student2 = *(student_data *)b;
    return strcmp(student1.name, student2.name);
}

/*
 * 두 학생 데이터를 바꾸는 함수.
 */
void SwapStudentData(student_data *a, student_data *b){
    student_data temp;
    SWAP(a->student_ID, b->student_ID, int);
    memcpy(temp.name, a->name, sizeof(char) * 10);
    memcpy(a->name, b->name, sizeof(char) * 10);
    memcpy(b->name, temp.name, sizeof(char) * 10);
    memcpy(temp.phone_number, a->phone_number, sizeof(int)*11);
    memcpy(a->phone_number, b->phone_number, sizeof(int)*11);
    memcpy(b->phone_number, temp.phone_number, sizeof(int)*11);
}

/*
 * 선택 정렬 함수.
 */
void SelectionSort(int size, student_data studentData[]){
    //최솟값 인덱스
    int min_index;
    for (int i = 0; i < size-1; ++i) {
        min_index = i;
        for (int j = i+1; j < size; ++j) {
            //이름 비교
            if (strcmp(studentData[j].name, studentData[min_index].name) < 0) {
                min_index = j;
            }
        }
        //범위 내 가장 작은 데이터 스왑
        SwapStudentData(&studentData[i], &studentData[min_index]);
    }
}

/*
 * 퀵 정렬을 위한 분할 함수1. 배열을 피벗을 기준으로 분리한다.
 * 피벗은 배열의 맨 앞 요소로 한다.
 */
int DivideToPartition(int first, int last, student_data studentData[]){
    int low, high, pivot;
    low = first; high = last+1;
    pivot = first;
    do {
        do {
            low++;
        }while (strcmp(studentData[low].name, studentData[pivot].name) < 0 && low <= last);
        do {
            high--;
        }while (strcmp(studentData[pivot].name, studentData[high].name) < 0 && high >= first);
        if (low<high){
            SwapStudentData(&studentData[low], &studentData[high]);
        }
    }while (low < high);
    SwapStudentData(&studentData[first], &studentData[high]);
    return high;
}

/*
 * 퀵 정렬을 위한 분할 함수2
 * 피벗은 배열의 중간 요소로 한다.
 */
int DivideToPartitionMiddlePivot(int first, int last, student_data studentData[]){
    //피벗이 될 중간 요소를 맨 앞 값과 바꿈.
    SWAP(studentData[first], studentData[(last + first) / 2], student_data);
    int low, high, pivot;
    low = first; high = last+1;
    pivot = first;
    do {
        do {
            low++;
        }while (strcmp(studentData[low].name, studentData[pivot].name) < 0 && low <= last);
        do {
            high--;
        }while (strcmp(studentData[pivot].name, studentData[high].name) < 0 && high >= first);
        if (low<high){
            SwapStudentData(&studentData[low], &studentData[high]);
        }
    }while (low < high);
    SwapStudentData(&studentData[first], &studentData[high]);
    return high;
}

/*
 * 퀵 정렬 함수1.
 * 재귀를 이용하여 구현하였다.
 */

void QuickSortRecursive(int first, int last, student_data *studentData){
    if (first<last){
        int q = DivideToPartitionMiddlePivot(first, last, studentData);
        QuickSortRecursive(first, q - 1, studentData);
        QuickSortRecursive(q + 1, last, studentData);
    }
}

/*
 * 스택을 사용하는 퀵정렬
 */
void QuickSortStack(int size, student_data studentData[]){
    Pstack stack = CreateStack();
    //첫 인덱스 값과 마지막 인덱스 값을 push.
    Push(stack, CreateElement(0, size-1));

    while (!IsStackEmpty(stack)){
        //범위 하나를 pop
        Element temp = Pop(stack);

        //해당 범위가 1 이상이라면
        if (temp.left < temp.right){
            //해당 범위를 피벗을 기준으로 나누고
            int q = DivideToPartitionMiddlePivot(temp.left, temp.right, studentData);
            //나눈 범위를 다시 push.
            Push(stack, CreateElement(temp.left, q-1));
            Push(stack, CreateElement(q+1, temp.right));
        }
    }
    //스택의 최종 크기를 출력하고 동적할당 해제
    PrintStackSize(stack);
    DestructStack(stack);
}

/*
 * 입력된 데이터를 섞은 뒤 정렬하는 퀵 정렬 함수.
 */
void QuickSortAfterShuffle(int size, student_data studentData[]){
    srand((unsigned int) clock());
    int randIndex;
    for (int i = 0; i < size-1; ++i) {
        //i번째 요소와 i+1 ~ size범위에 있는 요소를 바꾼다.
        randIndex = rand() % (size - i) + i;
        SWAP(studentData[i], studentData[randIndex], student_data);
    }

    Pstack stack = CreateStack();
    //첫 인덱스 값과 마지막 인덱스 값을 push.
    Push(stack, CreateElement(0, size-1));

    while (!IsStackEmpty(stack)){
        //범위 하나를 pop
        Element temp = Pop(stack);

        //해당 범위가 1 이상이라면
        if (temp.left < temp.right){
            //해당 범위를 피벗을 기준으로 나누고
            int q = DivideToPartition(temp.left, temp.right, studentData);
            //나눈 범위를 다시 push.
            Push(stack, CreateElement(temp.left, q-1));
            Push(stack, CreateElement(q+1, temp.right));
        }
    }
    //스택의 최종 크기를 출력하고 동적할당 해제
    PrintStackSize(stack);
    DestructStack(stack);
}

/*
 * 힙인지 판단하고 아니라면 힙으로 바꾸는 함수.
 */
void MakeHeap(int root, int size, student_data *list){
    int root_index = root;
    int child = root_index * 2 + 1;
    while (child < size){
        //두 자식 노드 간 대소 비교.
        if (strcmp(list[child].name, list[child+1].name) < 0 && child < size - 1){
            child++;
        }
        //둘 중 더 큰 자식 노드와 부모 노드 비교.
        //부모 노드가 더 크다면 힙 완성
        if (strcmp(list[root_index].name, list[child].name) > 0){
            break;
        }
        //자식 노드가 더 크다면 부모와 바꿈.
        else{
            SwapStudentData(&list[(child-1)/2], &list[child]);
            root_index = child;
            child = root_index * 2 + 1;
        }
    }
}

/*
 * 힙 정렬 함수.
 */
void HeapSort(int size, student_data studentData[]){
    //모든 데이터를 힙 트리로 만든다.
    for (int i = size/2 - 1; i >= 0; --i) {
        MakeHeap(i, size, studentData);
    }
    //가장 큰 값을 맨 뒤 값과 바꾸고 다시 힙 트리로 만듦.
    for (int i = size-1; i > 0; --i) {
        SwapStudentData(&studentData[0], &studentData[i]);
        MakeHeap(0, i, studentData);
    }
}

//학생들의 데이터를 출력하는 함수
void PrintStudentData(int size, student_data studentData[]){
    for (int i = 0; i < size; i = i + PRINT_GAP) {
        printf("%d - %s - ", studentData[i].student_ID, studentData[i].name);
        for (int j = 0; j < 11; ++j) {
            printf("%d", studentData[i].phone_number[j]);
        }
        printf("\n");
    }
}

//학생들 데이터에서 중복된 학번의 존재 유무를 밝히는 함수.
//많은 시간이 걸림.
bool AreDuplicateValuesExist(int size, student_data *studentData){
    for (int i = 0; i < size-1; ++i) {
        for (int j = i+1; j < size; ++j) {
            if (studentData[i].student_ID == studentData[j].student_ID){
                printf("%d - ", studentData[i].student_ID);
                printf("%d\n", studentData[j].student_ID);
                printf("there are same values!\n");
                return TRUE;
            }
        }
    }
    printf("all values are different\n");
    return FALSE;
}

/*
 * 배열이 올바르게 정렬 되어있는지 확인하는 함수.
 */
bool IsArraySorted(int size, student_data arr[]){
    for (int i = 0; i < size-1; ++i) {
        if (strcmp(arr[i].name, arr[i+1].name) >= 0){
            printf("the array isn't sorted!\n");
            return FALSE;
        }
    }
    printf("the array is sorted.\n");
    return TRUE;
}

student_data raw_data[STUDENT_COUNT] = {0, };
student_data student_data_sort1[STUDENT_COUNT] = {0, };
student_data student_data_sort2[STUDENT_COUNT] = {0, };
student_data student_data_sort3[STUDENT_COUNT] = {0, };
student_data student_data_sort4[STUDENT_COUNT] = {0, };
student_data student_data_sort5[STUDENT_COUNT] = {0, };
student_data student_data_sort6[STUDENT_COUNT] = {0, };


int main(){


    printf("Randomizing student data\n");
    TimeCheckStart();
    RandomizeStudentID_Table(STUDENT_COUNT, raw_data);
    TimeCheckEnd();
    RandomizeName(STUDENT_COUNT, raw_data);
    RandomizePhoneNumber(STUDENT_COUNT, raw_data);
    printf("Printing Raw Data\n");
    PrintStudentData(STUDENT_COUNT, raw_data);
    printf("-----------------\n");

#ifdef DUPLICATE_VALUE_CHECK
    AreDuplicateValuesExist(STUDENT_COUNT, raw_data);
    IsArraySorted(STUDENT_COUNT, raw_data);
    printf("-----------------\n");
#endif

#ifdef QUICK_SORT_C
    memcpy(student_data_sort1, raw_data, sizeof(student_data) * STUDENT_COUNT);
    printf("Quick Sort in stdlib.h\n");
    TimeCheckStart();
    qsort(student_data_sort1, STUDENT_COUNT, sizeof(student_data), Compare);
    PrintStudentData(STUDENT_COUNT, student_data_sort1);
    TimeCheckEnd();
    IsArraySorted(STUDENT_COUNT, student_data_sort1);
    printf("-----------------\n");
#endif

#ifdef SELECTION_SORT
    memcpy(student_data_sort2, raw_data, sizeof(student_data) * STUDENT_COUNT);
    printf("Selection Sort\n");
    TimeCheckStart();
    SelectionSort(STUDENT_COUNT, student_data_sort2);
    PrintStudentData(STUDENT_COUNT, student_data_sort2);
    TimeCheckEnd();
    IsArraySorted(STUDENT_COUNT, student_data_sort2);
    printf("-----------------\n");
#endif

#ifdef QUICK_SORT_RECURSIVE
    memcpy(student_data_sort3, raw_data, sizeof(student_data) * STUDENT_COUNT);
    printf("Quick Sort with Recursion\n");
    TimeCheckStart();
    QuickSortRecursive(0, STUDENT_COUNT-1, student_data_sort3);
    PrintStudentData(STUDENT_COUNT, student_data_sort3);
    TimeCheckEnd();
    IsArraySorted(STUDENT_COUNT, student_data_sort3);
    printf("-----------------\n");
#endif

#ifdef QUICK_SORT_STACK
    memcpy(student_data_sort4, raw_data, sizeof(student_data) * STUDENT_COUNT);
    printf("Quick Sort with Stack\n");
    TimeCheckStart();
    QuickSortStack(STUDENT_COUNT, student_data_sort4);
    PrintStudentData(STUDENT_COUNT, student_data_sort4);
    TimeCheckEnd();
    IsArraySorted(STUDENT_COUNT, student_data_sort4);
    printf("-----------------\n");
#endif

#ifdef QUICK_SORT_SHUFFLE
    //일반적인 퀵 정렬에서의 최악의 결과를 안정적으로 해결하는지 알아보기 위해
    //qsort로 정렬한 배열을 입력으로 주었다.
    memcpy(student_data_sort5, student_data_sort1, sizeof(student_data) * STUDENT_COUNT);
    printf("Quick Sort with Shuffling\n");
    TimeCheckStart();
    QuickSortAfterShuffle(STUDENT_COUNT, student_data_sort5);
    PrintStudentData(STUDENT_COUNT, student_data_sort5);
    TimeCheckEnd();
    IsArraySorted(STUDENT_COUNT, student_data_sort5);
    printf("-----------------\n");
#endif

#ifdef HEAP_SORT
    memcpy(student_data_sort6, raw_data, sizeof(student_data) * STUDENT_COUNT);
    printf("Heap Sort\n");
    TimeCheckStart();
    HeapSort(STUDENT_COUNT, student_data_sort6);
    PrintStudentData(STUDENT_COUNT, student_data_sort6);
    TimeCheckEnd();
    IsArraySorted(STUDENT_COUNT, student_data_sort6);
#endif
    return 0;
}