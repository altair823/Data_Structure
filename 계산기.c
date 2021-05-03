/*
 * 자료구조 과제 2
 * 스택/큐
 *
 * 2020-04-01
 * 만든이: 20175154 김태현
 *
 * version 1.4
 * 입력받은 수식을 계산하여 결과를 출력하는 프로그램
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_EXPRESSION_SIZE 100
#define MAX_STACK_SIZE 100
#define MAX_INPUT 100
#define MAX_INPUT_NUM_SIZE 10
#define TRUE 1
#define FALSE 0

typedef int bool;

/*
 * 모든 피연산자와 연산자는 이 중에서 한 값을 타입으로 갖는다.
 */
typedef enum Type{
    OPERAND = 0,
    ADD = 10,
    SUB = 11,
    MUL = 100,
    MOD = 101,
    SQU = 1000,
    BRACKET_OPEN = 1,
    BRACKET_CLOSE = 2
}Element_type;

/*
 * 연산자와 피연산자 모두 이 형식의 구조체로 저장된다.
 */
typedef struct ExpressionElement{

    Element_type type;
    int value;

}Element;

/*
 * 스택 구조체
 */
typedef struct{
    Element element[MAX_STACK_SIZE];
    int top;
}Stack, *PStack;

/*
 * 스택을 동적으로 할당하여 그 포인터를 반환하는 함수
 */
PStack CreateStack(){

    PStack stack = malloc(sizeof(Stack));
    stack->top = -1;
    return stack;
}

/*
 * 스택과 관련된 연산 함수들
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
        printf("오류: 스택이 비어있습니다.");
        exit(9);
    }
    return stack->element[stack->top];
}

//스택의 재사용을 위해 초기화하는 함수.
void initStack(PStack stack){
    stack->top = -1;
}

//동적할당된 스택을 해제하는 함수.
void DestructStack(PStack stack){
    free(stack);
}

//스택이 해당 연산자를 갖고 있는지 체크하는 함수. 여는 괄호 존재 유무를 체크하는데 사용했다.
bool IsStackHasType(PStack stack, Element_type type){
    for (int i = 0; i < stack->top+1; ++i) {
        if (stack->element[i].type == type){
            return TRUE;
        }
    }
    return FALSE;
}

/*
 * 버퍼에 저장된 숫자들을 정수로 변환하여 저장하는 함수
 * 값을 저장할 장소, 버퍼의 크기, 버퍼를 매개변수로 받아 이를 값에 저장하고
 * 버퍼를 안전하게 초기화하는 역할을 한다. 이를 하지 않으면
 * 버퍼에 남은 쓰레기 값이 atoi 함수를 통해 값에 저장되어 버린다.
 */
void BufferToArray(int *value, int *buffer_size, char buffer[]){
    *value = atoi(buffer);
    for (int i = 0; i < *buffer_size; ++i) {
        buffer[i] = '\0';
    }
    *buffer_size = 0;
}

/*
 * 연산자가 입력되었을때 버퍼에 담긴 피연산자를 결과 식에 넣고
 * 이어서 연산자를 결과 식에 넣는 함수
 * 연산자의 중복을 체크한다.
 */
void OperatorAppear(Element_type type, Element expression[], int *expression_index, char buffer[], int *buffer_index, int blank_count, bool *IsExpressionCorrect){

    //버퍼에 저장된 숫자가 없고 앞에 닫는 괄호를 제외한 연산자가 저장되어 있다면 오류.
    if ((expression[(*expression_index)-1].type != OPERAND)&&(expression[(*expression_index)-1].type != BRACKET_CLOSE)&&(*buffer_index == 0)){
        for (int j = 0; j < blank_count; ++j) {
            printf(" ");
        }
        printf("^ 이 위치에 오류가 있습니다.\n");
        *IsExpressionCorrect = FALSE;
        return;
    }

    //버퍼에 저장된 숫자를 그 타입과 값으로 저장한다.
    if (*buffer_index != 0) {
        expression[*expression_index].type = OPERAND;
        BufferToArray(&expression[(*expression_index)++].value, buffer_index, buffer);
    }
    //현재 연산자를 저장한다.
    expression[*expression_index].type = type;
    (*expression_index)++;

}

/*
 * 수식을 입력 받아 이를 배열로 저장하고 그 크기를 반환하는 함수
 */
int InputExpression(Element* expression, bool *IsExpressionCorrect){
    int i;
    int buffer_index = 0;
    int expression_index = 0;
    char input[MAX_INPUT];
    char buffer[MAX_INPUT_NUM_SIZE];

    gets(input);

    //각 경우에 따라 피연산자라면 그 값을, 연산자라면 그 종류를 저장한다.
    for (i = 0; i < MAX_INPUT; ++i) {
        //입력 버퍼의 마지막으로 오면 마지막 숫자 저장 후 반복문을 벗어난다.
        if (input[i] == '\0') {
            if (buffer_index != 0) {
                expression[expression_index].type = OPERAND;
                BufferToArray(&expression[expression_index++].value, &buffer_index, buffer);
            }
            break;
        }

            //각 연산자에 맞게 저장한다.
        else if (input[i] == '+') {
            OperatorAppear(ADD, expression, &expression_index, buffer, &buffer_index, i, IsExpressionCorrect);
            if (*IsExpressionCorrect == FALSE){
                return 0;
            }
        }
        else if (input[i] == '-') {
            OperatorAppear(SUB, expression, &expression_index, buffer, &buffer_index, i, IsExpressionCorrect);
            //식에 오류가 발생하면 오류 출력 후 바로 다시 시작
            if (*IsExpressionCorrect == FALSE){
                return 0;
            }
        }
        else if (input[i] == '*') {
            OperatorAppear(MUL, expression, &expression_index, buffer, &buffer_index, i, IsExpressionCorrect);
            if (*IsExpressionCorrect == FALSE){
                return 0;
            }
        }
        else if (input[i] == '^') {
            OperatorAppear(SQU, expression, &expression_index, buffer, &buffer_index, i, IsExpressionCorrect);
            if (*IsExpressionCorrect == FALSE){
                return 0;
            }
        }
        else if (input[i] == '%') {
            OperatorAppear(MOD, expression, &expression_index, buffer, &buffer_index, i, IsExpressionCorrect);
            if (*IsExpressionCorrect == FALSE){
                return 0;
            }
        }
        else if (input[i] == '(') {
            //버퍼가 비어있지 않다면(여는 괄호 이전에 숫자가 나왔었다면)
            if (buffer_index != 0){
                for (int j = 0; j < i; ++j) {
                    printf(" ");
                }
                printf("^ 이 위치에 오류가 있습니다.\n");
                *IsExpressionCorrect = FALSE;
                return 0;
            }

            expression[expression_index++].type = BRACKET_OPEN;
        }
        else if (input[i] == ')') {

            //버퍼에 아무것도 없다는 것은 닫는 괄호 앞에 피연산자가 아닌 연산자가 있다는 뜻.
            //따라서 이는 오류이다.
            if (buffer_index != 0) {
                expression[expression_index].type = OPERAND;
                BufferToArray(&expression[expression_index++].value, &buffer_index, buffer);
            }
            else{
                for (int j = 0; j < i; ++j) {
                    printf(" ");
                }
                printf("^ 이 위치에 오류가 있습니다.\n");
                *IsExpressionCorrect = FALSE;
                return 0;
            }
            expression[expression_index++].type = BRACKET_CLOSE;
        }

            //피연산자일 경우
        else {
            //피연산자가 나오기 전 닫는 괄호가 등장했었다면
            if (expression[expression_index-1].type == BRACKET_CLOSE){
                for (int j = 0; j < i; ++j) {
                    printf(" ");
                }
                printf("^ 이 위치에 오류가 있습니다.\n");
                *IsExpressionCorrect = FALSE;
                return 0;
            }
            buffer[buffer_index++] = input[i];
        }

    }
    //getchar();
    return expression_index;
}

/*
 * 연산자 우선순위를 계산해서 첫 번째 매개변수 연산자가 우선순위가 높다면
 * 1을, 두 번째 연산자의 우선순위가 높다면 -1을, 같다면 0을 반환하는 함수
 * 괄호를 계산에 넣지 말 것.
 */
int WhichOperatorPriorityHigh(int op1, int op2){
    //두 연산자의 값 차이가 2 미만이라면, 두 연산자는 같은 우선순위를 갖는다.
    if (abs(op1-op2) < 2){
        return 0;
    }
        //두 연산자의 값 차이가 2 이상 나고 op1에서 op2를 뺀 값이 양수라면
        //이는 op1이 더 높은 우선순위를 갖는다는 뜻이다.
    else if((abs(op1-op2) > 2) && (op1 - op2 > 0)){
        return 1;
    }
    else{
        return -1;
    }
}

/*
 * 수식을 후위표기법으로 바꾸 배열에 저장하는 함수
 */
void ConvertToPostfix(int *size, Element* expression, PStack stack, bool *IsExpressionCorrect){

    Element result_expression[*size];
    int result_index = 0;
    for (int i = 0; i < *size; ++i) {
        //피연산자라면 바로 출력한다.
        if (expression[i].type == OPERAND){
            result_expression[result_index++] = expression[i];
        }
            //여는 괄호는 무조건 스택에 담는다.
        else if (expression[i].type == BRACKET_OPEN){
            if (expression[i-1].type == OPERAND){
                for (int j = 0; j < i; ++j) {
                    printf(" ");
                }
                printf("^ 이 위치에 오류가 있습니다.\n");
                *IsExpressionCorrect = FALSE;
                return;
            }
            Push(stack, expression[i]);
        }
            //닫는 괄호가 나왔다면 여는 괄호가 나올 때까지 스택에서 꺼내어 출력한다.
        else if (expression[i].type == BRACKET_CLOSE){
            if (!IsStackHasType(stack, BRACKET_OPEN)){
                for (int j = 0; j < i; ++j) {
                    printf(" ");
                }
                printf("^ 이 위치에 오류가 있습니다.\n");
                *IsExpressionCorrect = FALSE;
                return;
            }
            while (Peek(stack).type != BRACKET_OPEN){
                result_expression[result_index++] = Pop(stack);
            }
            //스택에 있는 여는 괄호 제거
            Pop(stack);
        }
            //연산자라면 스택에 담을지 출력할지 고려한다.
        else if (expression[i].type != OPERAND){
            //입력의 마지막 문자가 괄호를 제외한 연산자라면 이는 오류이다.
            if (i == (*size)-1){
                for (int j = 0; j < i; ++j) {
                    printf(" ");
                }
                printf("^ 이 위치에 오류가 있습니다.\n");
                *IsExpressionCorrect = FALSE;
                return;
            }
            //스택이 비어있다면 무조건 스택에 넣는다.
            if (IsStackEmpty(stack)){
                Push(stack, expression[i]);
            }
                //만약 스택 맨 위에 있는 연산자가 현재 연산자보다 우선순위가 낮으면
                //스택에 넣는다.
            else if (WhichOperatorPriorityHigh(Peek(stack).type, expression[i].type) == -1){
                Push(stack, expression[i]);
            }
                //만약 스택 맨 위에 있는 연산자가 현재 연산자보다 우선순위가 같거나 높으면
                //스택 맨 위에 있는 연산자가 현재 연산자보다 우선순위가 낮아질 때까지(만약 스택 빈다면 그때까지)
                //꺼내어 출력하고 현재 연산자를 스택에 넣는다.
            else if ((WhichOperatorPriorityHigh(Peek(stack).type, expression[i].type) == 0) ||
                     WhichOperatorPriorityHigh(Peek(stack).type, expression[i].type) == 1){
                while ((!IsStackEmpty(stack)) && (WhichOperatorPriorityHigh(Peek(stack).type, expression[i].type) != -1)) {
                    result_expression[result_index++] = Pop(stack);
                }
                Push(stack, expression[i]);
            }
        }
    }

    //스택이 빌 때까지 pop
    while (!IsStackEmpty(stack)){
        result_expression[result_index] = Pop(stack);
        //닫는 괄호가 없다면 수식 맨 끝에 오류 메세지 출력
        if (result_expression[result_index].type == BRACKET_OPEN){
            for (int i = 0; i < *size; ++i) {
                printf(" ");
            }
            printf("^ 이 위치에 오류가 있습니다.\n");
            *IsExpressionCorrect = FALSE;
            return;
        }
        result_index++;
    }
    *size = result_index;
    for (int i = 0; i < *size; ++i) {
        expression[i] = result_expression[i];
    }
}

/*
 * 스택과 연산자를 매개변수로 받아 해당 연산에 필요한 피연산자를 스택에서 꺼내어 계산한 뒤
 * 그 값을 스택에 저장하는 함수. 피연산자를 타입에 넣지 말것.
 */
void CalCulateOperator(Element_type type, PStack stack, bool *IsExpressionCorrect){
    Element operand1, operand2, result;
    switch (type) {
        case ADD:
            operand1 = Pop(stack);
            operand2 = Pop(stack);
            result.type = OPERAND;
            result.value = operand1.value + operand2.value;
            Push(stack, result);
            break;
        case SUB:
            operand1 = Pop(stack);
            operand2 = Pop(stack);
            result.type = OPERAND;
            //빼기는 pop한 순서 거꾸로 빼야한다.
            result.value = operand2.value - operand1.value;
            Push(stack, result);
            break;
        case MUL:
            operand1 = Pop(stack);
            operand2 = Pop(stack);
            result.type = OPERAND;
            result.value = operand1.value * operand2.value;
            Push(stack, result);
            break;
        case MOD:
            operand1 = Pop(stack);
            operand2 = Pop(stack);
            result.type = OPERAND;
            result.value = operand2.value % operand1.value;
            Push(stack, result);
            break;
        case SQU:
            operand1 = Pop(stack);
            operand2 = Pop(stack);
            result.type = OPERAND;
            //왜인지 디버깅시 오류가 발생한다...
            //result.value = (int)pow((double)(operand2.value), (double)(operand1.value));
            result.value = 1;
            for (int i = 0; i < operand1.value; ++i) {
                result.value *= operand2.value;
            }

            Push(stack, result);
            break;
        default:
            printf("올바른 연산자가 아닙니다. ");
            *IsExpressionCorrect = FALSE;
            return;
    }
}

/*
 * 후위표기법으로 저장된 수식을 계산하는 함수
 */
int CalculatePostfix(int size, Element* expression, PStack stack, bool *IsExpressionCorrect){

    int cal_result;
    for (int i = 0; i < size; ++i) {
        //수식의 해당 요소가 피연산자라면 스택에 저장한다.
        if (expression[i].type == OPERAND){
            Push(stack, expression[i]);
        }
            //해당 요소가 연산자라면 필요한 피연산자를 pop하여 계산한 값을 push한다.
        else if (expression[i].type != OPERAND){
            CalCulateOperator(expression[i].type, stack, IsExpressionCorrect);
        }
    }
    cal_result = Pop(stack).value;
    if (!IsStackEmpty(stack)){
        printf("피연산자가 남아있습니다!");
        *IsExpressionCorrect = FALSE;
        return 0;
    }
    return cal_result;
}

int main(){
    //입력받은 수식을 정형화하여 저장할 배열.
    Element expression[MAX_EXPRESSION_SIZE];

    //수식에서 오류가 발생했는지 체크하기 위한 플래그
    //FALSE라면 오류가 있는것.
    bool IsExpressionCorrect;

    //스택 포인터.
    PStack stack = CreateStack();

    int result;
    int expression_size;
    while (TRUE) {
        //수식 배열과 오류 플래그를 초기화한다.
        IsExpressionCorrect = TRUE;
        for (int i = 0; i < MAX_EXPRESSION_SIZE; ++i) {
            expression[i].type = 0;
            expression[i].value = 0;
        }
        //수식을 입력받는다.
        expression_size = InputExpression(expression, &IsExpressionCorrect);
        if (IsExpressionCorrect == FALSE){
            continue;
        }
        /*
        //첫 시행에서만 스택을 동적으로 할당한다.
        if (stack == NULL) {
            stack = CreateStack();
        }
         */
        //후위표기법으로 변환한다.
        ConvertToPostfix(&expression_size, expression, stack, &IsExpressionCorrect);
        //다음 스택 사용을 위해 스택을 초기화한다.
        initStack(stack);
        if (IsExpressionCorrect == FALSE){
            continue;
        }
        //수식의 답을 계산한다.
        result = CalculatePostfix(expression_size, expression, stack, &IsExpressionCorrect);
        initStack(stack);
        if (IsExpressionCorrect == FALSE){
            continue;
        }
        //결과 값 출력.
        printf("= %d\n", result);
    }
}