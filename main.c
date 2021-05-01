/*
 * 자료구조 과제 4
 * 연결리스트
 *
 * 20175154 김태현
 *
 * 2021-04-29
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define INDEX_SEARCH

#define FILE_PATH "/Users/altair823/Desktop/data_structure/randdict_utf8.TXT"
#define END_OF_WORD '\n'

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

/* 단어 구조체 */
typedef struct{
    char *eng_word;
    int eng_word_len;
    char *kor_word;
    int kor_word_len;
}Word;

/* 단어 구조체를 동적으로 생성하는 함수 */
Word *CreateWord(char *eng_word, int eng_word_len, char *kor_word, int kor_word_len){
    Word *new_word = malloc(sizeof(Word));

    if (eng_word != NULL) {
        new_word->eng_word = malloc(sizeof(char) * eng_word_len);
        memcpy(new_word->eng_word, eng_word, sizeof(char) * eng_word_len);
    }
    if (kor_word != NULL) {
        new_word->kor_word = malloc(sizeof(char) * kor_word_len);
        memcpy(new_word->kor_word, kor_word, sizeof(char) * kor_word_len);
    }
    new_word->eng_word_len = eng_word_len;
    new_word->kor_word_len = kor_word_len;

    return new_word;
}

void DestructWord(Word *word){
    free(word->eng_word);
    free(word->kor_word);
    free(word);
}

/* 연결리스트 노드 구조체 */
typedef struct node{
    Word *word;
    struct node *next_node;
    struct node *previous_node;
}Node;

/* 단어를 받아 새 노드를 만드는 함수 */
Node *CreateNode(Word *word){
    Node *new_node = malloc(sizeof(Node));
    new_node->next_node = NULL;
    new_node->previous_node = NULL;
    new_node->word = CreateWord(word->eng_word, word->eng_word_len, word->kor_word, word->kor_word_len);
    return new_node;
}

void DestructNode(Node *node){
    DestructWord(node->word);
    free(node);
}

/* 원하는 자리에 새 노드를 넣는 함수.
 * 해당 위치의 색인이 없거나 새 노드가 색인보다 앞에 와야한다면
 * 색인을 새 노드로 바꾼다. */
void Insert(Node *before_node, Node *new_node, Node *index[]){
    if (before_node == NULL){
        printf("노드가 없습니다.");
    }
    new_node->previous_node = before_node;
    new_node->next_node = before_node->next_node;
    before_node->next_node->previous_node = new_node;
    before_node->next_node = new_node;


    int word_index = (int)(new_node->word->eng_word[0]) - 97;
    if (index[word_index] == NULL || strcmp(index[word_index]->word->eng_word, new_node->word->eng_word) >= 0){
        index[word_index] = new_node;
    }
}

void Delete(Node *llist, Node *target_node){

}

/* 연결리스트에서 해당 단어의 자리를 찾는 함수.
 * 더 큰 단어 바로 앞 노드 위치를 반환한다. */
Node *SearchEngWord(Node *LList, char *eng_word, Node *index[]){
    Node *current_node = LList->next_node;

#ifndef INDEX_SEARCH
    if (current_node->word == NULL){
        return previous_node;
    }
    while (strcmp(current_node->word->eng_word, eng_word) < 0){
        previous_node = current_node;
        current_node = current_node->next_node;
        if (current_node == LList){
            break;
        }
    }
    return previous_node;
#endif

#ifdef INDEX_SEARCH
    int word_index = (int)(eng_word[0]) - 97;
    //색인이 없다면 헤드부터 리니어 서치.
    if (index[word_index] == NULL){
        if (current_node->word == NULL){
            return current_node->previous_node;
        }
        while (strcmp(current_node->word->eng_word, eng_word) < 0){
            current_node = current_node->next_node;
            if (current_node == LList){
                break;
            }
        }
        return current_node->previous_node;
    }
    //색인이 있고 그 범위의 첫 노드보다 작다면 그 바로 앞 위치 반환.
    else if (strcmp(index[word_index]->word->eng_word, eng_word) > 0){
        while (current_node != index[word_index]->next_node){
            current_node = current_node->next_node;
        }
        return current_node->previous_node;
    }
    //색인이 있고 그 범위의 첫 노드보다 크거나 같다면 그 색인에서부터 리니어 서치.
    else{
        if (word_index == 14){

        }
        current_node = index[word_index];

        while (strcmp(current_node->word->eng_word, eng_word) < 0){
            current_node = current_node->next_node;
            //헤드로 돌아오거나 다음 색인으로 넘어가면 탐색 종료.
            if (current_node == LList || current_node == index[word_index + 1]){
                break;
            }
        }
        return current_node->previous_node;
    }
#endif

}

/* 연결리스트에서 해당 요소를 찾고 그 위치를 반환하는 함수 */
//Node *Search

/* DEBUG */
void Display(Node *llist){
    int node_count = 0;
    Node *current_node = llist->next_node;
    while (current_node != llist){
        printf("%.*s : %.*s\n", current_node->word->eng_word_len, current_node->word->eng_word, current_node->word->kor_word_len, current_node->word->kor_word);
        current_node = current_node->next_node;
        node_count++;
    }
    printf("node count : %d\n", node_count);
}

/* 단어를 임시로 저장할 큐의 연산 */
#define QUEUE_SIZE 100
#define ENQUEUE(latter) ((*rear = latter), rear++)
#define DEQUEUE(latter) ((latter = *front), front++)
#define IS_QUEUE_EMPTY (rear > front)
#define QUEUE_WORD_SIZE (rear - front)
#define INIT_QUEUE ((rear = queue), (front = queue))

/* 파일을 읽고 단어 하나를 반환하는 함수.
 * 단어를 읽지 못하면 NULL값 반환. */
Word* ReadAWord(FILE *dict_file_ptr){
    //임시 단어 구조체
    Word temp_word;

    /* 영어, 또는 한글 단어를 임시로 저장할 버퍼.
     * 단어 하나를 입력받기 전에 초기화를 거치므로 선형큐로 구현함. */
    char queue[QUEUE_SIZE];
    char *rear = queue;
    char *front = queue;

    if (dict_file_ptr == NULL){
        printf("there is no file!\n");
        return NULL;
    }

    char buffer = 0;

    /* 영어 단어를 파일에서 입력받는다. */
    while (buffer != ':'){
        buffer = (char)fgetc(dict_file_ptr);
        //파일 끝에 오면 입력 중단.
        if (buffer == EOF){
            return NULL;
        }
        ENQUEUE(buffer);
    }
    /* 공백과 ':'문자 제거 */
    rear--; rear--;
    //널 문자 삽입을 위해 크기 +1
    temp_word.eng_word_len = QUEUE_WORD_SIZE + 1;
    fgetc(dict_file_ptr);

    /*  영어 단어 동적 할당!  */
    temp_word.eng_word = malloc(sizeof(char) * temp_word.eng_word_len);
    for (int i = 0; IS_QUEUE_EMPTY; i++){
        DEQUEUE(temp_word.eng_word[i]);
    }
    temp_word.eng_word[temp_word.eng_word_len-1] = '\0';

    /*  큐 초기화  */
    INIT_QUEUE;

    /* 영어 단어의 뜻을 파일에서 입력받는다. */
    while (buffer != END_OF_WORD){
        buffer = (char)fgetc(dict_file_ptr);
        ENQUEUE(buffer);
    }

    rear--; rear--;
    temp_word.kor_word_len = QUEUE_WORD_SIZE+1;

    /*  한글 뜻 동적 할당!  */
    temp_word.kor_word = malloc(sizeof(char) * temp_word.kor_word_len);
    for (int i = 0; IS_QUEUE_EMPTY; i++) {
        DEQUEUE(temp_word.kor_word[i]);
    }
    temp_word.kor_word[temp_word.kor_word_len-1] = '\0';

    /* 소문자 변환. */
    for (int i = 0; i < temp_word.eng_word_len; i++){
        temp_word.eng_word[i] = (char) tolower(temp_word.eng_word[i]);
    }

    Word *result_word = CreateWord(temp_word.eng_word, temp_word.eng_word_len, temp_word.kor_word, temp_word.kor_word_len);

    /* 임시 단어 구조체 동적할당 해제 */
    free(temp_word.eng_word);
    free(temp_word.kor_word);

    return result_word;
}

/* 파일을 열고 단어들을 연결리스트에 넣는 함수 */
Node *ReadDictFile(Node *index[], int *node_count){
    Node *llist_head = malloc(sizeof(Node));
    llist_head->next_node = llist_head;
    llist_head->previous_node = llist_head;
    llist_head->word = NULL;

    Word *temp_word;
    FILE *dict_file = fopen(FILE_PATH, "r");

    while ((temp_word = ReadAWord(dict_file) )!= NULL){
        Node *node_before_target = SearchEngWord(llist_head, temp_word->eng_word, index);
        Insert(node_before_target, CreateNode(temp_word), index);
        (*node_count)++;
    }

    return llist_head;
}

/* 단어를 입력 받아 반환하는 함수 */
int InputWord(char **input){
    int input_size;
    char temp_input[80];
    printf(">>> ");
    gets(temp_input);

    input_size = (int)strlen(temp_input) + 1;
    *input = malloc(sizeof(char) * input_size);
    memcpy(*input, temp_input, input_size);


    return input_size;
}

/* 사전에서 단어를 찾는 함수. 만약 단어가 없다면
 * 그 뜻을 입력받아 새로 만든다. */
void SearchDict(Node *llist, Node *index[], int *node_count){
    char *input_eng_word = NULL;
    int input_eng_word_len = InputWord(&input_eng_word);

    for (int i = 0; i < input_eng_word_len-1; i++){
        input_eng_word[i] = (char) tolower(input_eng_word[i]);
    }
    /* SearchEngWord 함수는 주어진 단어보다 사전 순서상 하나 앞선 단어의 위치를 반환한다.
     * 따라서 그 단어 노드의 next_node가 찾는 단어와 같다면 탐색에 성공한 것이고,
     * 다르면 단어가 사전에 없어서 탐색에 실패한 것이다. */
    Node *before_target_ptr = SearchEngWord(llist, input_eng_word, index);
    if (strcmp(before_target_ptr->next_node->word->eng_word, input_eng_word) == 0){
        printf("%s\n", before_target_ptr->next_node->word->kor_word);
    }
    else{
        printf("단어가 존재하지 않습니다. 뜻을 추가하세요. (추가하지 않으려면 공백)\n");
        char *input_kor_word = NULL;
        int input_kor_word_len = InputWord(&input_kor_word);
        if (strcmp(input_kor_word, " ") != 0 && strcmp(input_kor_word, "\n") != 0){
            Node *new_word_node = CreateNode(CreateWord(input_eng_word, input_eng_word_len, input_kor_word, input_kor_word_len));
            Insert(before_target_ptr, new_word_node, index);
            (*node_count)++;
            printf("%s : %s 가 추가되었습니다. (총 %d개 단어)\n", new_word_node->word->eng_word, new_word_node->word->kor_word, *node_count);
        }
        free(input_kor_word);
    }
    free(input_eng_word);
}

int main() {

    //색인은 해당 범위의 바로 앞 노드를 가리킨다.
    Node *index[26] = {NULL, };
    int llist_node_count = 0;
    TimeCheckStart();
    Node *llist = ReadDictFile(index, &llist_node_count);
    TimeCheckEnd();
    Display(llist);
    while (1) {
        SearchDict(llist, index, &llist_node_count);
    }
    return 0;
}
