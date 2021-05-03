/*
 * 자료구조 과제 1
 * 다항식 저장방식 1
 *
 * 2020-03-17
 * 만든이: 20175154 김태현
 *
 * version 1.5
 * 다항식 모든 항의 계수만을 저장하는 배열을 갖는 프로그램이다
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

//최대 5차 다항식의 곱셈을 저장할 수 있도록 배열의 크기를 정한다
#define MAX_SIZE 11

//if 연산을 줄이는 방법을 사용하지 않는 덧셈 알고리즘을 사용하게 하는 매크로
//#define ADD_DEBUG

//if 연산의 횟수를 출력하게 하는 매크로
//#define IF_COUNT

//다항식을 저장할 구조체
typedef struct polynominal{

    //해당 다항식의 최대 차수
    int max_degree;

    //다항식의 계수를 저장하는 배열 포인터
    int coef_list[MAX_SIZE];
}poly;

//몇 번째 다항식인지, 그리고 다항식의 포인터를 받아
//문장을 출력하고 입력을 다항식에 저장하는 함수
void create_poly(int poly_num, poly *p){
    int i;

    //입력 받은 구조체의 맴버 변수를 모두 0으로 초기화 한다
    for (i = 0; i <= p->max_degree; ++i) {
        p->coef_list[i] = 0;
    }
    p->max_degree = 0;

    //모든 항의 계수를 공백을 기준으로 입력받는다
    printf("수식 %d을 입력하십시오: ", poly_num);
    char input[100];
    scanf("%[^\n]s", input);

    //공백 문자를 기준으로 입력받은 계수 문자열을 자른다
    char *str_ptr = strtok(input, " ");
    for (i=0; str_ptr != NULL; i++)
    {
        //자른 문자열 한 개를 정수로 변환하여 다항식 구조체의 coef 리스트에 넣는다
        p->coef_list[i] = atoi(str_ptr);
        str_ptr = strtok(NULL, " ");
    }

    //문자열 자르기를 반복한 횟수는(0 ~ i-1) 해당 다항식의 최고차수이다
    p->max_degree = i-1;

    getchar();
}

//주어진 다항식을 적절하게 출력하는 함수
void print_poly(poly p){
    int i;
    for (i = 0; i <= p.max_degree; ++i) {

        //만약 계수가 0인 항이 있다면 그 항을 출력하지 않고 건너뛴다
        if (p.coef_list[i] == 0){continue;}

        //모든 항은 ?x^?의 형식으로 출력된다
        printf("%dx^%d", p.coef_list[i], p.max_degree-i);

        //마지막 항은 +를 뒤에 붙이지 않는다
        if (i != p.max_degree){printf(" + ");}
    }
    printf("\n");
}

//두 다항식의 덧셈을 하는 함수
poly add_poly(poly a, poly b){
    poly result;

    //덧셈 결과 값의 최고차수는 두 다항식 중 더 큰 차수이다
    result.max_degree = MAX(a.max_degree, b.max_degree);

    int i;

    static int count=0;

#ifndef ADD_DEBUG
    //두 다항식의 차수 차이를 저장할 변수


    int difference = 0;

    //만약 a다항식이 차수가 더 크다면
    if (a.max_degree > b.max_degree) {
#ifdef IF_COUNT
        count++;
#endif
        //차수의 차이를 계산하고
        difference = a.max_degree - b.max_degree;

        //그 차이만큼 a다항식의 계수를 미리 결과값에 저장한다
        for (i = 0; i < difference; i++) {
            result.coef_list[i] = a.coef_list[i];
        }

        //차수가 같은 항만 남았다면 각 항의 차수 비교 없이 단순히 덧셈만 진행해주면 된다
        for (i = 0; i <= result.max_degree - difference; ++i) {
            result.coef_list[i + difference] = a.coef_list[i + difference] + b.coef_list[i];
        }
    }
    //만약 b다항식의 차수가 더 크다면
    else if (a.max_degree < b.max_degree) {
#ifdef IF_COUNT
        count++;
#endif
        difference = b.max_degree - a.max_degree;
        for (i = 0; i < difference; i++) {
            result.coef_list[i] = b.coef_list[i];
        }

        for (i = 0; i <= result.max_degree - difference; ++i) {
            result.coef_list[i + difference] = b.coef_list[i + difference] + a.coef_list[i];
        }
    }

    //두 다항식의 차수가 같다면 단순한 덧셈만 진행한다
    else {
#ifdef IF_COUNT
        count++;
#endif
        for (i = 0; i < result.max_degree; ++i) {
            result.coef_list[i] = a.coef_list[i] + b.coef_list[i];
        }
    }

#endif


    /*
     * 이하는 각 항의 차수 비교를 배열 끝까지 하는 코드이다
     * 차수비교를 최소화 하기 위해 아래와 같은 코드를 사용하지 않았다
     */
#ifdef ADD_DEBUG
    //차수의 크기비교를 위한 변수
    int a_degree = a.max_degree; int b_degree = b.max_degree;

    for (i = 0; i <= result.max_degree; i++) {

        //아직 처리되지 않은 항을 비교하여
        //어느 한 식의 항의 차수가 더 크면
        //해당 항의 계수를 결과 값에 넣는다
        if (a_degree > b_degree){
#ifdef IF_COUNT
        count++;
#endif
            result.coef_list[i] = a.coef_list[a.max_degree - a_degree];
            a_degree--;
        }
        else if (a_degree < b_degree){
#ifdef IF_COUNT
        count++;
#endif
            result.coef_list[i] = b.coef_list[b.max_degree - b_degree];
            b_degree--;
        }
        //두 항의 차수가 같으면
        //두 항의 계수를 더해 결과 값에 넣는다
        else{
#ifdef IF_COUNT
        count++;
#endif
            result.coef_list[i] = a.coef_list[a.max_degree - a_degree] + b.coef_list[b.max_degree - b_degree];
            a_degree--; b_degree--;
        }
    }
#endif

#ifdef IF_COUNT
    printf("------if count is %d------\n", count);
#endif
    return result;
}

//두 다항식의 곱셈을 하는 함수
poly mul_poly(poly a, poly b){
    int i, j;

    poly result;

    result.max_degree = a.max_degree + b.max_degree;

    //계산 결과를 결과 값에 대입하는 것이 아닌
    //더할 것이기 때문에 0으로 초기화 해준다
    for (i = 0; i <= result.max_degree; i++){
        result.coef_list[i] = 0;
    }

    //두 식의 곱을 결과 식에 넣는다
    for (i = 0; i <= a.max_degree; ++i) {
        for (j = 0; j <= b.max_degree; ++j) {
            result.coef_list[i+j] += a.coef_list[i] * b.coef_list[j];
        }
    }

    return result;
}

//다항식 번호와 x값을 입력받는 함수
void input_x(int* poly_num, int* x_value){
    int i;

    //다항식의 번호와 x값에 대입할 값
    int poly_num_in, x_value_in;

    printf("수식에 값을 넣으세요: ");
    char input[10];
    scanf("%[^\n]s", input);

    char *str_ptr = strtok(input, " ");

    poly_num_in = atoi(str_ptr);
    str_ptr = strtok(NULL, " ");
    x_value_in = atoi(str_ptr);

    *x_value = x_value_in;
    *poly_num = poly_num_in;
    getchar();
}

//전달받은 x값을 전달받은 다항식에 대입하여 결과 값을 반환하는 함수
int substitute_poly(poly p, int x){
    int i;

    int result = 0;

    for (i = p.max_degree; i >= 0; --i) {

        //x의 i제곱과 계수를 곱한 값을 결과 값에 더한다
        result += p.coef_list[p.max_degree - i] * (int)(pow(x, i));
    }
    return result;
}

//원하는 다항식에 x값을 대입하여 결과 값을 출력하는 함수
void substitute_which_poly(poly a, poly b, poly c, poly d){
    int poly_num = -1, x_value;

    //다항식 번호로 0이 입력되면 입력을 중지하고 프로그램을 종료한다
    while (poly_num != 0) {
        //다항식 번호와 x값을 입력받아 저장한다
        input_x(&poly_num, &x_value);

        if (poly_num == 1) {
            printf("결과 값은 %d\n", substitute_poly(a, x_value));
        } else if (poly_num == 2) {
            printf("결과 값은 %d\n", substitute_poly(b, x_value));
        } else if (poly_num == 3) {
            printf("결과 값은 %d\n", substitute_poly(c, x_value));
        } else if (poly_num == 4) {
            printf("결과 값은 %d\n", substitute_poly(d, x_value));
        } else if (poly_num == 0) {
            return;
        } else {
            printf("잘못된 입력입니다");
        }
    }
}

int main(){


    poly a, b;

    //두 다항식을 입력받아 저장한다
    create_poly(1, &a);
    create_poly(2, &b);
    print_poly(a);
    print_poly(b);

    //두 다항식의 덧셈과 곱셈을 하여 저장한다
    poly c = add_poly(a, b);
    poly d = mul_poly(a, b);
    print_poly(c);
    print_poly(d);

    //원하는 다항식 번호와 그 다항식에 원하는 값을 대입한다
    substitute_which_poly(a, b, c, d);

    return 0;
}