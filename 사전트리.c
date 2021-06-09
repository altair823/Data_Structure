/*
 * 자료구조 과제 5
 * 트리
 *
 * 20175154 김태현
 *
 * 2021-05-22
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_FILE_LINE 200
#define MAX_WORD_LENGTH 100
#define END_OF_LINE '\n'
#define FILE_PATH "randdict_utf8.TXT"

typedef char Color;
#define RED 1
#define BLACK (-1)
#define UNSET 0

#define RAND_WORD_COUNT 10


typedef struct {
    char *eng;
    int eng_len;
    char *kor;
    int kor_len;
}Word;

Word *CreateWord(char *eng, int eng_len, char *kor, int kor_len){
    Word *new_word = malloc(sizeof(Word));
    new_word->eng_len = eng_len;
    new_word->kor_len = kor_len;
    new_word->eng = malloc(sizeof(char)*(eng_len+1));
    new_word->kor = malloc(sizeof(char)*(kor_len+1));
    strcpy(new_word->eng, eng);
    strcpy(new_word->kor, kor);
    return new_word;
}

typedef struct node{
    Word *word;
    struct node *lchild;
    struct node *rchild;

    //only used for red-black tree.
    struct node *parent;
    char color;
}Node;


/*
 * The function that compare two different nodes by alphabetically.
 * There will be only two options. Left child or right child.
 */
int CompareNode(Node *to_compare, Node *new_node){
    if (strcmp(to_compare->word->eng, new_node->word->eng) < 0){
        return -1;
    } else if (strcmp(to_compare->word->eng, new_node->word->eng) > 0){
        return 1;
    } else{
        return 0;
    }
}

Node *CreateNode(Word *word){
    Node *new_node = malloc(sizeof(Node));
    new_node->word = word;
    new_node->lchild = NULL;
    new_node->rchild = NULL;
    new_node->parent = NULL;
    new_node->color = UNSET;
    return new_node;
}

void SetRChild(Node *parent, Node *child){
    if (parent != NULL){
        parent->rchild = child;
    }
    if (child != NULL){
        child->parent = parent;
    }
}
void SetLChild(Node *parent, Node *child) {
    if (parent != NULL) {
        parent->lchild = child;
    }
    if (child != NULL){
        child->parent = parent;
    }
}

void SetColor(Node *root, Node *target_node, Color color){
    if (target_node == root)
    {
        target_node->color = BLACK;
    }
    if (target_node != root && target_node != NULL) {
        target_node->color = color;
    }
}

char GetColor(Node *node){
    if (node != NULL){
        return node->color;
    } else{
        return BLACK;
    }
}

/*
 * The recursive function inserting a new node to binary tree by node comparing function.
 * Not balanced!
 */
int InsertNode(Node **root, Node *new_node, int (*CompareFunc)(Node *, Node *)){
    int node_count = 0;
    if (*root == NULL){
        *root = new_node;
        node_count++;
        return node_count;
    }

    switch (CompareFunc(*root, new_node)) {
        case -1: //left
            if ((*root)->lchild != NULL) {
                node_count = InsertNode(&((*root)->lchild), new_node, CompareFunc);
            } else{
                (*root)->lchild = new_node;
                node_count++;
            }
            break;
        case 1: //right
            if ((*root)->rchild != NULL) {
                node_count = InsertNode(&((*root)->rchild), new_node, CompareFunc);
            } else{
                (*root)->rchild = new_node;
                node_count++;
            }
            break;
    }
    return node_count;
}



void SerialRedUncleRed(Node *root, Node *current_node, Node *uncle){
    Node *parent = current_node->parent;
    Node *g_parent = current_node->parent->parent;
    //set black the parent node.
    SetColor(root, parent, BLACK);
    //set red the grand parent node if the g_parent node isn't root node.
    SetColor(root, g_parent, RED);
    //set black the uncle node.
    SetColor(root, uncle, BLACK);
}

/*
 * XX means current and parent node are the same lchild or rchild of each parent.
 * XY means current and parent node are different lchild or rchild of each parent.
 * And the root of subtree(median of current, parent, grandparent) is determined by these.
 */
#define XX 0
#define XY 1

int WhichIsMedian(Node *current){
    Node *parent = current->parent;
    Node *g_parent = current->parent->parent;
    if (((current == parent->lchild) && (parent == g_parent->rchild)) || ((current == parent->rchild) && (parent == g_parent->lchild))){
        return XY;
    } else if (((current == parent->lchild) && (parent == g_parent->lchild)) || ((current == parent->rchild) && (parent == g_parent->rchild))){
        return XX;
    } else{
        exit(0);
    }
}

void SerialRedUncleBlack(Node **root, Node *current_node, Node *uncle) {
    Node *parent = current_node->parent;
    Node *g_parent = current_node->parent->parent;
    //if the median of three(current, parent, g_parent) is parent,
    if (WhichIsMedian(current_node) == XX) {
        //which child of side that g_parent is.
        if (g_parent == *root) {
            *root = parent;
        } else if (g_parent == g_parent->parent->lchild) {
            SetLChild(g_parent->parent, parent);
        } else {
            SetRChild(g_parent->parent, parent);
        }

        //which side the consecutive red nodes appeared.
        if (uncle == g_parent->rchild) {
            SetLChild(g_parent, parent->rchild);
            SetRChild(parent, g_parent);
        } else {
            SetRChild(g_parent, parent->lchild);
            SetLChild(parent, g_parent);
        }
        SetColor(*root, parent, BLACK);
        SetColor(*root, g_parent, RED);
    }
    //if the median of three is current,
    else {
        if (g_parent == *root) {
            *root = current_node;
        } else if (g_parent == g_parent->parent->lchild) {
            SetLChild(g_parent->parent, current_node);
        } else {
            SetRChild(g_parent->parent, current_node);
        }

        //which side the consecutive red nodes appeared.
        if (uncle == g_parent->rchild) {
            SetLChild(g_parent, current_node->rchild);
            SetRChild(current_node, g_parent);

            SetRChild(parent, current_node->lchild);
            SetLChild(current_node, parent);
        } else {
            SetRChild(g_parent, current_node->lchild);
            SetLChild(current_node, g_parent);

            SetLChild(parent, current_node->rchild);
            SetRChild(current_node, parent);
        }
        SetColor(*root, current_node, BLACK);
        SetColor(*root, g_parent, RED);
    }

}

/*
 * The function checking serial two red nodes.
 */
void IsSerialRedAppear(Node **root, Node *current_node){
    while (1) {
        if (current_node == *root || current_node->parent == *root) {
            return;
        }
        Node *parent = current_node->parent;
        Node *g_parent = current_node->parent->parent;
        if (GetColor(current_node) == RED && GetColor(parent) == RED) {
            Node *uncle;
            if (parent == g_parent->lchild) {
                uncle = g_parent->rchild;
            } else {
                uncle = g_parent->lchild;
            }
            //XYr
            if (GetColor(uncle) == RED) {
                SerialRedUncleRed(*root, current_node, uncle);
                current_node = g_parent;
                continue;
            }
            //XYb
            else {
                SerialRedUncleBlack(root, current_node, uncle);
                return;
            }
        } else{
            return;
        }
    }
}

/*
 * In this function, node is inserted to binary tree by rules that make red-black tree.
 */
int InsertNodeBalanced(Node **root, Node **sub_root, Node *new_node, int (*CompareFunc)(Node *, Node *)) {
    int node_count = 0;
    if (*sub_root == NULL){
        *sub_root = new_node;
        new_node->color = BLACK;
        node_count++;
        return node_count;
    }
    switch (CompareFunc(*sub_root, new_node)) {
        case -1: //left
            if ((*sub_root)->lchild != NULL) {
                node_count = InsertNodeBalanced(root, &((*sub_root)->lchild), new_node, CompareFunc);
            } else{
                SetLChild(*sub_root, new_node);
                new_node->color = RED;
                IsSerialRedAppear(root, new_node);
                node_count++;
            }
            break;
        case 1: //right
            if ((*sub_root)->rchild != NULL) {
                node_count = InsertNodeBalanced(root, &((*sub_root)->rchild), new_node, CompareFunc);
            } else{
                SetRChild(*sub_root, new_node);
                new_node->color = RED;
                IsSerialRedAppear(root, new_node);
                node_count++;
            }
            break;
    }
    return node_count;
}

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int GetTreeHeight(Node *root){
    int height = 0;
    if (root != NULL){
        int left_height = GetTreeHeight(root->lchild);
        int right_height = GetTreeHeight(root->rchild);
        height = 1 + MAX(left_height, right_height);
    }
    return height;
}



/*
 * Read a single Word from the dictionary file.
 * The 'Word' contains english word and korean word.
 */
Word *ReadAWord(FILE *file){

    if (file == NULL){
        printf("there is no file!\n");
        return NULL;
    }

    char buffer[MAX_FILE_LINE];
    int index = 0;
    char eng[MAX_WORD_LENGTH];
    char kor[MAX_WORD_LENGTH];
    int eng_length = 0;
    int kor_length = 0;

    if (fgets(buffer, MAX_FILE_LINE, file) == NULL){
        return NULL;
    }

    //english word
    for (int i = 0; buffer[index] != ':'; index++, i++) {
        eng[i] = buffer[index];
        eng_length++;
    }
    //insert NULL in last element.
    eng[--eng_length] = '\0';
    index++;
    index++;

    //korean word
    for (int i = 0; buffer[index] != END_OF_LINE; index++, i++) {
        kor[i] = buffer[index];
        kor_length++;
    }
    kor[--kor_length] = '\0';

    //replace all char to lowercase.
    for (int i = 0; i < eng_length; ++i) {
        eng[i] = (char)tolower(eng[i]);
    }

    return CreateWord(eng, eng_length, kor, kor_length);
}

Node *ReadDictFile(FILE *file){
    Node *tree = NULL;
    Word *new_word = NULL;
    int node_count = 0;
    for (new_word = ReadAWord(file); new_word != NULL; new_word = ReadAWord(file)) {
        Node *new_node = CreateNode(new_word);
        node_count += InsertNode(&tree, new_node, CompareNode);
    }
    printf("사전 파일을 모두 읽었습니다. A 트리에는 %d개의 단어가 있습니다. \n", node_count);
    return tree;
}

Node *ReadDictFileBalanced(FILE *file){
    Node *tree = NULL;
    Word *new_word = NULL;
    int node_count = 0;
    for (new_word = ReadAWord(file); new_word != NULL; new_word = ReadAWord(file)) {
        Node *new_node = CreateNode(new_word);
        node_count += InsertNodeBalanced(&tree, &tree, new_node, CompareNode);
    }
    printf("사전 파일을 모두 읽었습니다. B 트리에는 %d개의 단어가 있습니다. \n", node_count);
    return tree;
}

typedef struct {
    Node *node;
    int height;
}NodeAndHeight;

/*
 * The function that returns the node and its height by NodeAndHeight*, corresponding to a specific index.
 * Traverse in preorder.
 */
NodeAndHeight *GetNodeByIndex(Node *root, int *index, int height){
    Node *current = root;
    NodeAndHeight *result = NULL;
    if (*index == 0){
        result = malloc(sizeof(NodeAndHeight));
        result->node = current;
        result->height = height;
        return result;
    }
    if (current->lchild != NULL){
        (*index)--;
        result = GetNodeByIndex(current->lchild, index, height+1);
        if (result != NULL){
            return result;
        }
        if (*index == 0){
            result = malloc(sizeof(NodeAndHeight));
            result->node = current;
            result->height = height;
            return result;
        }
    }
    if (current->rchild != NULL){
        (*index)--;
        result = GetNodeByIndex(current->rchild, index, height+1);
        if (result != NULL){
            return result;
        }
        if (*index == 0){
            result = malloc(sizeof(NodeAndHeight));
            result->node = current;
            result->height = height;
            return result;
        }
    }
    return NULL;
}

NodeAndHeight **SelectRand10Word(Node* tree, int node_count){
    NodeAndHeight **rand_node_arr = malloc(sizeof(NodeAndHeight *) * RAND_WORD_COUNT);
    srand((unsigned int) clock());
    for (int i = 0; i < RAND_WORD_COUNT; i++) {
        int rand_index = rand() % node_count;
        rand_node_arr[i] = GetNodeByIndex(tree, &rand_index, 0);
    }
    return rand_node_arr;
}

/*
 * Search the height of given node.
 */
NodeAndHeight *GetNodeHeight(Node *root, Node *target, int(*CompareFunc)(Node *, Node *), int initial_height){
    NodeAndHeight *result = NULL;
    if (root != NULL){
        switch (CompareFunc(root, target)) {
            case -1:
                if (root->lchild != NULL){
                    result = GetNodeHeight(root->lchild, target, CompareFunc, initial_height+1);
                    if (result != NULL){
                        return result;
                    }
                } else{
                    printf("%s는 존재하지 않는 단어 입니다\n", target->word->eng);
                    return NULL;
                }
                break;
            case 1:
                if (root->rchild != NULL){
                    result = GetNodeHeight(root->rchild, target, CompareFunc, initial_height+1);
                    if (result != NULL){
                        return result;
                    }
                } else{
                    printf("%s는 존재하지 않는 단어 입니다\n", target->word->eng);
                    return NULL;
                }
                break;
            case 0:
                result = malloc(sizeof(NodeAndHeight));
                result->node = target;
                result->height = initial_height;
                return result;
        }
    }
    return NULL;
}

int main(){
    FILE *Afile = fopen(FILE_PATH, "r");
    FILE *Bfile = fopen(FILE_PATH, "r");

    Node *A_Bi_Tree = ReadDictFile(Afile);
    int A_tree_height = GetTreeHeight(A_Bi_Tree);
    printf("A 트리의 전체 높이는 %d개 입니다. \n", A_tree_height);

    Node *B_Bi_Tree = ReadDictFileBalanced(Bfile);
    int B_tree_height = GetTreeHeight(B_Bi_Tree);
    printf("B 트리의 전체 높이는 %d개 입니다. \n\n", B_tree_height);

    printf("랜덤하게 선택된 10개의 단어들이 갖는 레벨을 두 트리에서 비교합니다.\n");
    NodeAndHeight **rand_node_arr = SelectRand10Word(A_Bi_Tree, 48405);
    for (int i = 0; i < RAND_WORD_COUNT; i++) {
        printf("%s - %s / 깊이: %d\n", rand_node_arr[i]->node->word->eng, rand_node_arr[i]->node->word->kor, rand_node_arr[i]->height);
    }
    printf("\n");
    NodeAndHeight **rand_node = malloc(sizeof(NodeAndHeight *) * RAND_WORD_COUNT);
    for (int i = 0; i < RAND_WORD_COUNT; ++i) {
        rand_node[i] = GetNodeHeight(B_Bi_Tree, rand_node_arr[i]->node, CompareNode, 0);
        printf("%s - %s / 깊이: %d\n", rand_node[i]->node->word->eng, rand_node[i]->node->word->kor, rand_node[i]->height);
    }
    return 0;
}
