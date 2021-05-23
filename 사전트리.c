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
#define FILE_PATH "/Users/altair823/Desktop/data_structure_sandbox/randdict_utf8.TXT"

#define RED 1
#define BLACK -1
#define UNSET 0
typedef char Color;

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
    if (to_compare ==NULL || new_node == NULL){
        printf("efegeg");
    }
    if (strcmp(to_compare->word->eng, new_node->word->eng) < 0){
        return -1;
    }
    else{
        return 1;
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

void SetColor(Node *new_node, Color color){
    if (new_node != NULL) {
        new_node->color = color;
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
 * Recursive function inserting a new node to binary tree by node comparing function.
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
                node_count += InsertNode(&((*root)->lchild), new_node, CompareFunc);
            } else{
                (*root)->lchild = new_node;
                node_count++;
            }
            break;
        case 1: //right
            if ((*root)->rchild != NULL) {
                node_count += InsertNode(&((*root)->rchild), new_node, CompareFunc);
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
    SetColor(parent, BLACK);
    //set red the grand parent node if the g_parent node isn't root node.
    if (g_parent != root) {
        SetColor(g_parent, RED);
    }
    //set black the uncle node.
    SetColor(uncle, BLACK);
}

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
        SetColor(parent, BLACK);
        SetColor(g_parent, RED);
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
        SetColor(current_node, BLACK);
        SetColor(g_parent, RED);
    }

}

/*
 * Recursive function called when two red nodes appear in succession.
 */
void IsSerialRedAppear(Node *root, Node *current_node){
    while (1) {
        if (current_node == root || current_node->parent == root) {
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
                SerialRedUncleRed(root, current_node, uncle);
                current_node = g_parent;
                continue;
            }
            //XYb
            else {
                SerialRedUncleBlack(&root, current_node, uncle);
                return;
            }
            //IsSerialRedAppear(root, g_parent);
        } else{
            return;
        }
    }
}

/*
 * In this function, node is inserted to B-tree by rules that make red-black tree.
 */
int InsertNodeBalanced(Node *root, Node **sub_root, Node *new_node, int (*CompareFunc)(Node *, Node *)) {
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

int Get_height(Node *root){
    int height = 0;
    if (root != NULL){
        int left_height = Get_height(root->lchild);
        int right_height = Get_height(root->rchild);
        height = 1 + MAX(left_height, right_height);
    }
    return height;
}

/*
 * Read 'A' Word from the dictionary file.
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
    Node *b_tree = NULL;
    Word *new_word = NULL;
    int node_count = 0;
    for (new_word = ReadAWord(file); new_word != NULL; new_word = ReadAWord(file)) {
        Node *new_node = CreateNode(new_word);
        node_count += InsertNode(&b_tree, new_node, CompareNode);
    }
    printf("사전 파일을 모두 읽었습니다. A 트리에는 %d개의 단어가 있습니다. \n", node_count);
    return b_tree;
}

Node *ReadDictFileBalanced(FILE *file){
    Node *b_tree = NULL;
    Word *new_word = NULL;
    int node_count = 0;
    for (new_word = ReadAWord(file); new_word != NULL; new_word = ReadAWord(file)) {
        Node *new_node = CreateNode(new_word);
        node_count += InsertNodeBalanced(b_tree, &b_tree, new_node, CompareNode);
    }
    printf("사전 파일을 모두 읽었습니다. B 트리에는 %d개의 단어가 있습니다. \n", node_count);
    return b_tree;
}

int main(){
    FILE *Afile = fopen(FILE_PATH, "r");
    FILE *Bfile = fopen(FILE_PATH, "r");
    Node *A_BTree = ReadDictFile(Afile);
    int A_BTree_height = Get_height(A_BTree);
    printf("A 트리의 전체 높이는 %d개 입니다. \n", A_BTree_height);
    Node *B_BTree = ReadDictFileBalanced(Bfile);
    int B_BTree_height = Get_height(B_BTree);
    printf("B 트리의 전체 높이는 %d개 입니다. \n", B_BTree_height);

    return 0;
}