/*
 * 자료구조 과제 5
 * 트리
 *
 * 20175154 김태현
 *
 * 2021-05-21
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 100

//operator type.
typedef enum {
    OPERAND = 0,
    PLUS = 10,
    MINUS = 11,
    MULTI = 20,
    DIVIDE = 21
}Op_type;

typedef struct {
    int operand;
    Op_type type;
}Element;

/*
 * Function that set operator type and operand value in elements of expression.
 */
void SetTypeAndOperand(Element *element, Op_type type, int value){
    element->type = type;
    if (type != OPERAND){
        element->operand = 0;
    }
    else{
        element->operand = value;
    }
}

/*
 * Getting new string, and
 * Convert inputted string that contain mathematical expression to array of Elements.
 */
Element *InputInfixExpression(int *size){
    char input[MAX_INPUT_SIZE];
    gets(input);
    int length = (int)strlen(input);
    *size = length;
    Element *input_expression = malloc(sizeof(Element) * (length));
    for (int i = 0; i < length; ++i) {
        switch (input[i]) {
            case '+':
                SetTypeAndOperand(&(input_expression[i]), PLUS, 0);
                break;
            case '-':
                SetTypeAndOperand(&(input_expression[i]), MINUS, 0);
                break;
            case '*':
                SetTypeAndOperand(&(input_expression[i]), MULTI, 0);
                break;
            case '/':
                SetTypeAndOperand(&(input_expression[i]), DIVIDE, 0);
                break;
            default:
                //Operand input[i] has ASCII value for string. Subtract 48 make it integer.
                SetTypeAndOperand(&(input_expression[i]), OPERAND, ((int)input[i]) - 48);
        }
    }
    return input_expression;
}

/*
 * Stack operation for converting infix to postfix.
 */
#define PUSH(element)	((void) (*top = (element), ++top))
#define	POP(element)	((void) (--top, (element) = *top))
#define IS_STACK_EMPTY (stack == top)

/*
 * For convenience of store expression in B-tree, converting infix expression to postfix.
 */
Element *ConvertPostfix(Element *expression, int exp_size){
    Element stack[exp_size];
    Element *top = stack;

    Element *result_expression = malloc(sizeof(Element) * exp_size);
    int index = 0;

    for (int i = 0; i < exp_size; ++i) {
        if (expression[i].type == OPERAND){
            result_expression[index++] = expression[i];
        }
        else if(expression[i].type != OPERAND){
            if (IS_STACK_EMPTY){
                PUSH(expression[i]);
            }
            //(top - 1) means peek() operation.
            //type / 10 means the operator's priority. Bigger type/10 indicate high priority.
            else if ((top - 1)->type/10 < expression[i].type/10){
                PUSH(expression[i]);
            }
            else if ((top - 1)->type/10 >= expression[i].type/10){
                while (!IS_STACK_EMPTY && (top - 1)->type/10 >= expression->type/10){
                    POP(result_expression[index++]);
                }
                PUSH(expression[i]);
            }
        }
    }
    while (!IS_STACK_EMPTY){
        POP(result_expression[index++]);
    }
    return result_expression;
}

/*
 * Print elements, not B-Tree!
 */
void PrintaElement(Element element) {
    if (element.type == OPERAND) {
        printf("%d ", element.operand);
    } else if (element.type == PLUS) {
        printf("+ ");
    } else if (element.type == MINUS) {
        printf("- ");
    } else if (element.type == MULTI) {
        printf("* ");
    } else if (element.type == DIVIDE) {
        printf("/ ");
    } else {
        //'$'means element has garbage value.
        printf("$ ");
    }
}

typedef struct node {
    struct node *rchild;
    struct node *lchild;
    Element element;
}Node;

Node *CreateNode(Op_type opType, int operand){
    Node *new_node = malloc(sizeof(Node));
    new_node->element.type = opType;
    new_node->element.operand = operand;
    return new_node;
}

/*
 * When expression become postfix, it can be easily stored to B-tree.
 */
Node *StorePostfixExpBTree(Element *expression, int size){
    //using stack that has Node pointer for elements.
    //it allows the stack to store a node or subtrees.
    Node *stack[size];
    Node *(*top) = stack;

    for (int i = 0; i < size; ++i) {
        Node *new_node = CreateNode(expression[i].type, expression[i].operand);
        switch (expression[i].type) {
            case OPERAND:
                break;
            //if operator appears, pop 2 operand and make a subtree.
            default:
                POP(new_node->rchild);
                POP(new_node->lchild);
        }
        //push the new subtree.
        PUSH(new_node);
    }

    Node *result_B_tree;
    POP(result_B_tree);
    return result_B_tree;
}

/*
 * Functions for traverse the B-Tree.
 */
void PrintBTreePreorder(Node *b_tree){

    Node *current_node = b_tree;
    PrintaElement(current_node->element);
    if (current_node->lchild != NULL) {
        PrintBTreePreorder(current_node->lchild);
    }
    if (current_node->rchild != NULL) {
        PrintBTreePreorder(current_node->rchild);
    }
}
void PrintBTreeInorder(Node *b_tree){
    Node *current_node = b_tree;
    if (current_node->lchild != NULL) {
        PrintBTreeInorder(current_node->lchild);
    }
    PrintaElement(current_node->element);
    if (current_node->rchild != NULL) {
        PrintBTreeInorder(current_node->rchild);
    }
}
void PrintBTreePostorder(Node *b_tree){
    Node *current_node = b_tree;
    if (current_node->lchild != NULL) {
        PrintBTreePostorder(current_node->lchild);
    }
    if (current_node->rchild != NULL) {
        PrintBTreePostorder(current_node->rchild);
    }
    PrintaElement(current_node->element);
}

/*
 * To traverse B-tree by level-order, the queue must be needed.
 */
#define QUEUE_SIZE 100
#define ENQUEUE(element) ((*rear = (element)), rear++)
#define DEQUEUE(element) (((element) = *front), front++)
#define IS_QUEUE_EMPTY (rear == front)
void PrintBTreeLevelOrder(Node *b_tree){
    Node *queue[QUEUE_SIZE];
    Node *(*rear) = queue;
    Node *(*front) = queue;

    ENQUEUE(b_tree);
    while (!IS_QUEUE_EMPTY){
        Node *current_node;
        DEQUEUE(current_node);
        if (current_node != NULL){
            PrintaElement(current_node->element);
            //enqueueing next level nodes.
            ENQUEUE(current_node->lchild);
            ENQUEUE(current_node->rchild);
        }
    }
}

/*
 * Recursive function that calculate expression in B-tree by postfix.
 * while calculating, calculated node will be destruct(free).
 */
void Calculate(Node *b_tree){
    Node *current_node = b_tree;
    int result = 0;
    if (current_node->lchild->lchild != NULL && current_node->lchild->rchild != NULL){
        Calculate(current_node->lchild);
    }
    if (current_node->rchild->lchild != NULL && current_node->rchild->rchild != NULL){
        Calculate(current_node->rchild);
    }
    switch (current_node->element.type) {
        case PLUS:
            result = current_node->lchild->element.operand + current_node->rchild->element.operand;
            break;
        case MINUS:
            result = current_node->lchild->element.operand - current_node->rchild->element.operand;
            break;
        case MULTI:
            result = current_node->lchild->element.operand * current_node->rchild->element.operand;
            break;
        case DIVIDE:
            result = current_node->lchild->element.operand / current_node->rchild->element.operand;
            break;
        default:
            printf("wrong B-tree!");
            exit(1);
    }
    free(current_node->lchild);
    free(current_node->rchild);
    current_node->lchild = NULL;
    current_node->rchild = NULL;
    current_node->element.type = OPERAND;
    current_node->element.operand = result;
}

int main(){
    int expression_size = 0;
    Element *expression = InputInfixExpression(&expression_size);
    Element *postfix_expression = ConvertPostfix(expression, expression_size);
    Node *B_Tree = StorePostfixExpBTree(postfix_expression, expression_size);

    printf("전위 순회: ");
    PrintBTreePreorder(B_Tree);
    printf("\n중위 순회: ");
    PrintBTreeInorder(B_Tree);
    printf("\n후위 순회: ");
    PrintBTreePostorder(B_Tree);
    printf("\n레벨 순회: ");
    PrintBTreeLevelOrder(B_Tree);
    printf("\n계산 결과: ");
    Calculate(B_Tree);
    PrintaElement(B_Tree->element);

    free(expression);
    free(postfix_expression);
    free(B_Tree);
    return 0;
}
