/*
 * 자료구조 과제 4
 * 연결리스트
 *
 * 20175154 김태현
 *
 * 2021-05-02
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_STUDENT_COUNT 10000

/* 각각의 색인은 다음의 개수를 가진다.
 * 학년의 개수, 알파벳의 개수와 같다. */
#define GRADE_COUNT 9
#define ALPHABET_COUNT 26

#define MAX_NAME_LENGTH 10
#define MIN_NAME_LENGTH 2

/* 특정 학생 데이터에 해당하는 색인을 특정하기 위한 수.
 * 실제 데이터에서 이 값을 빼면 색인의 번호가 나온다. */
#define START_GRADE 13
#define ASCII_INT_DIFFERENCE 65

/* 학생 데이터가 출력되는 간격. */
#define NODE_PRINT_GAP 100

typedef struct{
    int ID;
    char *name;
    int phone_number[11];
}Student;

typedef struct node{
    Student *student;
    struct node *next_ID_node;
    struct node *next_name_node;
}Node;

/* index_node는 해당 색인 범위의 첫 노드를 가리킨다.
 * before_index_node는 해당 색인 범위에는 포함되지 않지만
 * 가장 가까운 바로 앞 노드를 가리킨다. */
typedef struct{
    struct node *index_node;
    struct node *before_index_node;
}Index;

/* 연결 리스트를 생성하는 함수.
 * 헤드를 생성하고 반환한다. */
Node *CreateHead(){
    Node *head = malloc(sizeof(Node));
    head->next_ID_node = head;
    head->next_name_node = head;
    head->student = NULL;
    return head;
}

Student *CreateStudent(){
    Student *new_student = malloc(sizeof(Student));
    new_student->name = NULL;
    return new_student;
}

/* 학생의 정보들을 저장하는 함수 */
Student *CreateID(Student *student, int ID) {
    student->ID = ID;
    return student;
}
Student *CreateName(Student *student){
    srand((unsigned int) clock());
    char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    int name_len = rand() % (MAX_NAME_LENGTH + 1 - MIN_NAME_LENGTH) + MIN_NAME_LENGTH;
    student->name = malloc(sizeof(char) * name_len + 1);
    int i;
    for (i = 0; i < name_len; ++i) {
        student->name[i] = alphabet[rand() % 26];
    }
    //이름 문자열 마지막에 널 삽입.
    student->name[i] = '\0';
    return student;
}
Student *CreatePhoneNum(Student *student){
    srand((unsigned int) clock());
    student->phone_number[0] = 0;
    student->phone_number[1] = 1;
    student->phone_number[2] = 0;
    for (int i = 3; i < 11; ++i) {
        student->phone_number[i] = rand() % 10;
    }
    return student;
}

/* 새 노드를 생성하는 함수.
 * 학생 데이터로부터 노드를 만든다.*/
Node *CreateNode(Student *student){
    Node *new_node = malloc(sizeof(Node));
    new_node->student = student;
    return new_node;
}

/* 색인 배열을 만들어 반환하는 함수들.
 * 색인은 각 학년에 하나 씩 존재한다. */
Index *CreateIndex(Node *head, int index_count){
    Index *index_of_llist = malloc(sizeof(Index) * index_count);
    for (int i = 0; i < index_count; ++i) {
        index_of_llist[i].before_index_node = head;
        index_of_llist[i].index_node = NULL;
    }
    return index_of_llist;
}

/* 동적으로 할당한 데이터들을 해제하는 함수들 */
void DestructName(char *name){
    if (name == NULL){
        return;
    }
    free(name);
}
void DestructStudent(Student *student){
    if (student == NULL){
        return;
    }
    DestructName(student->name);
    free(student);
}
void DestructNode(Node *node){
    if (node == NULL){
        return;
    }
    DestructStudent(node->student);
    free(node);
}
void DestructIndex(Index *index_of_llist){
    free(index_of_llist);
}

/* 한 노드에 대하여 그 노드에 해당하는 인덱스 항목 번호를 반환하는 함수 */
int GetIDIndex(Node *node, int *max_index){
    *max_index = GRADE_COUNT;
    int index_number = (node->student->ID / 10000 % 100) - START_GRADE;
    return index_number;
}
int GetNameIndex(Node *node, int *max_index){
    *max_index = ALPHABET_COUNT;
    int index_number = node->student->name[0] - ASCII_INT_DIFFERENCE;
    return index_number;
}

/* 새로 추가될 노드를 확인하고 색인을 업데이트하는 함수
 * 헤드와 색인 배열, 새 노드 그리고 새 노드로부터 해당하는 index를 반환하는 함수를 매개변수로 갖는다. */
void UpdateIndex(Node *head, Index *index_of_llist, Node *new_node, int (*GetIndexFunc)(Node *, int *), int (*CompareFunc)(Node *, Node *)){
    int max_index;
    int index_number = GetIndexFunc(new_node, &max_index);
    /* index가 가리키는 노드보다 새 노드가 작으면 index 노드는 새 노드를 가리킴. */
    if (index_of_llist[index_number].index_node == NULL || CompareFunc(index_of_llist[index_number].index_node, new_node) > 0){
        index_of_llist[index_number].index_node = new_node;
    }
    for (int i = index_number + 1; i < max_index; i++) {
        /* index_number 보다 큰 index에 대하여
         * before 노드가 헤드를 가리키거나
         * before 노드 ID가 새로 추가하려는 노드 ID보다 작은 index들은
         * before 노드를 새로 추가하는 노드로 업데이트 한다.
         * 이는 해당 index 범위의 첫 노드의 전 노드가
         * 더 이상 전 노드가 아니고 새 노드임을 뜻한다. */
        if (index_of_llist[i].before_index_node == head || CompareFunc(index_of_llist[i].before_index_node, new_node) < 0){
            index_of_llist[i].before_index_node = new_node;
        }
    }
}

/* 노드를 어떤 방식으로 비교할 것인지 결정하는 함수 */
int CompareID(Node *node1, Node *node2){
    if (node1->student->ID > node2->student->ID){
        return 1;
    }
    else if (node1->student->ID < node2->student->ID){
        return -1;
    }
    else{
        return 0;
    }
}
int CompareName(Node *node1, Node *node2){
    return strcmp(node1->student->name, node2->student->name);
}

/* 해당 학번이 연결 리스트에 존재하는지 확인하는 함수.
 * 존재하지 않거나 존재하면 그 앞 주소를, 노드가 아예 없다면 NULL 반환.
 * 유무를 판단하려면 단지 반환된 노드 바로 뒤 노드를 확인하기만 하면 된다. */
Node *SearchNode(Node *head, Index *index_of_llist, Node *new_node, int (*GetIndexFunc)(Node * , int *), int (*CompareFunc)(Node *, Node *), Node *(*LinearSearch)(Node *, Index *, Node *, int (*CompareFunc)(Node *, Node *))){

    int max_index;
    int index_number = GetIndexFunc(new_node, &max_index);

    /* 노드에 해당하는 index_node가 존재하지 않을 때 or */
    /* 노드에 해당하는 index_node가 존재하지만 노드보다 클 때 */
    if (index_of_llist[index_number].index_node == NULL || CompareFunc(index_of_llist[index_number].index_node, new_node) > 0){
        return index_of_llist[index_number].before_index_node;
    }

        /* 노드에 해당하는 index_node가 존재하고 노드보다 작거나 같을 때 */
    else{
        return LinearSearch(head, &index_of_llist[index_number], new_node, CompareFunc);
    }
}

/* 노드에 해당하는 색인 하나를 받고 그 색인 범위를 순차탐색하는 함수들 */
Node *LinearSearchID(Node *head, Index *index_of_ID, Node* new_node, int (*CompareFunc)(Node *, Node *)){
    Node *current_node = index_of_ID->index_node;
    Node *previous_node = index_of_ID->before_index_node;
    //다음 index 바로 전까지 순차 탐색.
    while (CompareFunc(current_node, new_node) < 0){
        previous_node = current_node;
        current_node = current_node->next_ID_node;
        if (current_node == head || current_node == (index_of_ID + 1)->index_node){
            break;
        }
    }
    return previous_node;
}
Node *LinearSearchName(Node *head, Index *index_of_name, Node* new_node, int (*CompareFunc)(Node *, Node *)){
    Node *current_node = index_of_name->index_node;
    Node *previous_node = index_of_name->before_index_node;
    //다음 index 바로 전까지 순차 탐색.
    while (CompareFunc(current_node, new_node) < 0){
        previous_node = current_node;
        current_node = current_node->next_name_node;
        if (current_node == head || current_node == (index_of_name + 1)->index_node){
            break;
        }
    }
    return previous_node;
}

/* 연결리스트에 새 노드를 ID에 맞추어 올바른 자리에 넣는 함수. */
void InsertNodeByID(Node *head, Index *index_of_ID, Node *new_node){
    Node *before_node = SearchNode(head, index_of_ID, new_node, GetIDIndex, CompareID, LinearSearchID);
    new_node->next_ID_node = before_node->next_ID_node;
    before_node->next_ID_node = new_node;
    UpdateIndex(head, index_of_ID, new_node, GetIDIndex, CompareID);
}
void InsertNodeByName(Node *head, Index *index_of_name, Node *new_node){
    Node *before_node = SearchNode(head, index_of_name, new_node, GetNameIndex, CompareName, LinearSearchName);
    new_node->next_name_node = before_node->next_name_node;
    before_node->next_name_node = new_node;
    UpdateIndex(head, index_of_name, new_node, GetNameIndex, CompareName);
}

/* 랜덤한 학번을 생성하고 그 학번이 연결리스트에 존재하는지 확인 */
int CreateRandomID(Node *head, Index *index){
    srand((unsigned int) clock());
    Node *before;
    Node *temp_node = CreateNode(CreateStudent());
    int random_ID = 0;
    do{
        random_ID = 20000000;
        random_ID += (rand() % (21 + 1 - 13) + 13) * 10000;
        random_ID += rand() % 10000;
        temp_node->student->ID = random_ID;
        //생성한 학번이 존재하는지 확인한다.
        before = SearchNode(head, index, temp_node, GetIDIndex, CompareID, LinearSearchID);
        if (before->next_ID_node->student == NULL){
            break;
        }
    }while (before != head && before->next_ID_node->student->ID == random_ID);
    DestructNode(temp_node);
    return random_ID;
}

/* 출력 함수 */
void PrintPhoneNumber(Node *node){
    for (int i = 0; i < 11; ++i) {
        printf("%d", node->student->phone_number[i]);
    }
    printf("\n");
}
void DisplayByID(Node *head){
    Node *current_node = head->next_ID_node;
    printf("학번 순으로 출력합니다. \n");
    for (int i = 0; current_node != head; i++){
        if (i % NODE_PRINT_GAP == 0){
            printf("%d - ", current_node->student->ID);
            printf("%s - ", current_node->student->name);
            PrintPhoneNumber(current_node);
        }
        current_node = current_node->next_ID_node;
    }
}
void DisplayByName(Node *head){
    Node *current_node = head->next_name_node;
    printf("이름 순으로 출력합니다. \n");
    for (int i = 0; current_node != head; i++){
        if (i % NODE_PRINT_GAP == 0) {
            printf("%d - ", current_node->student->ID);
            printf("%s - ", current_node->student->name);
            PrintPhoneNumber(current_node);
        }
        current_node = current_node->next_name_node;
    }
}

int ChoosePrintMethod(){
    printf("(1) 학번 순\n");
    printf("(2) 이름 순\n");
    printf("메뉴 선택(0: 종료) : ");
    int choice;
    scanf("%d", &choice);
    getchar();
    
    return choice;
}

/* 학생 전체의 데이터와 그 연결리스트를 생성하는 함수. */
Node *CreateLList(Node *head, Index *index_of_ID, Index *index_of_name) {
    for (int i = 0; i < TOTAL_STUDENT_COUNT; ++i) {
        Student *student = CreateStudent();
        CreateID(student, CreateRandomID(head, index_of_ID));
        CreateName(student);
        CreatePhoneNum(student);
        Node *new_node = CreateNode(student);
        InsertNodeByName(head, index_of_name, new_node);
        InsertNodeByID(head, index_of_ID, new_node);
    }
    return head;
}

/* 연결리스트 전체를 해제하는 함수. */
void DestructLList(Node *head){
    Node *current_node = head->next_ID_node;
    Node *next_node = head->next_ID_node->next_ID_node;
    while (current_node != head){
        DestructNode(current_node);
        current_node = next_node;
        next_node = next_node->next_ID_node;
    }
    DestructNode(head);
}

#define PROGRAM_END 0
#define PRINT_ID_ORDER 1
#define PRINT_NAME_ORDER 2

int main(){

    Node *head = CreateHead();
    Index *index_of_ID = CreateIndex(head, GRADE_COUNT);
    Index *index_of_name = CreateIndex(head, ALPHABET_COUNT);

    head = CreateLList(head, index_of_ID, index_of_name);

    while (1) {
        int choice = ChoosePrintMethod();
        if (choice == PRINT_ID_ORDER) {
            DisplayByID(head);
        } else if (choice == PRINT_NAME_ORDER) {
            DisplayByName(head);
        } else if (choice == PROGRAM_END) {
            break;
        } else{
            printf("잘못된 입력입니다. \n");
        }
    }

    DestructIndex(index_of_ID);
    DestructIndex(index_of_name);
    DestructLList(head);
    return 0;
}