/*
 * 자료구조 과제 1
 * 다항식 저장방식 2
 *
 * 2020-03-19
 * 만든이: 20175154 김태현
 *
 * version 1.4
 * 다항식의 0이 아닌 계수를 갖는 항만을 저장하는 배열을 갖는 프로그램이다
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 11

//다항식의 항을 구현하기 위한 구조체
typedef struct poly_term{
    int expon;
    int coef;
}term;

//다항식을 저장하기 위한 구조체
typedef struct polynominal{

    //저장된 항의 개수, 즉 항을 저장한 배열에서 의미있는 만큼의 크기
    int size;

    //항을 저장할 배열
    term terms[MAX_SIZE];
}poly;

//다항식을 입력받아 저장하는 함수
void create_poly(int poly_num, poly *p){
    int i;

    //입력 받은 구조체의 맴버 변수를 모두 0으로 초기화 한다
    p->size = 0;
    /*
     * 배열의 크기 변수를 사용하여 배열에 안전하게 접근할 수 있으므로
     * 따로 배열을 초기화 시키지 않았다.
    for (i = 0; i < MAX_SIZE; ++i) {
        p->terms[i].expon = 0;
        p->terms[i].coef = 0;
    }
     */

    //모든 항의 계수를 공백을 기준으로 입력받는다
    printf("수식 %d을 입력하십시오: ", poly_num);
    char input[100];
    scanf("%[^\n]s", input);

    //공백 문자를 기준으로 입력받은 계수 문자열을 자른다
    char *str_ptr = strtok(input, " ");
    for (i=0; str_ptr != NULL; i++)
    {
        //자른 문자열 한 개를 정수로 변환하여 다항식 구조체의 coef 리스트에 넣는다
        p->terms[i].coef = atoi(str_ptr);
        str_ptr = strtok(NULL, " ");
        p->terms[i].expon = atoi(str_ptr);
        str_ptr = strtok(NULL, " ");
    }

    //i는 다항식의 길이
    p->size = i;
    getchar();
}

void print_poly(poly p){
    int i;

    for (i = 0; i < p.size; ++i) {
        //모든 항은 ?x^?의 형식으로 출력된다
        printf("%dx^%d", p.terms[i].coef, p.terms[i].expon);

        //마지막 항은 +를 뒤에 붙이지 않는다
        if (i != p.size-1){printf(" + ");}
    }
    printf("\n");
}

//두 다항식의 덧셈을 하는 함수
poly add_poly(poly a, poly b){
    poly result;

    //a 다항식과 b 다항식을 각각 순회하기 위해 만든 인덱스 번호 변수
    int pos_a=0, pos_b=0;

    //결과 다항식의 인덱스 번호
    int pos_result=0;
    int i=0;

    //두 다항식의 항을 앞에서부터 차례대로 차수를 비교하여
    //차수가 더 큰 항을 결과 값에 넣거나
    //차수가 같다면 두 항을 더해 결과 값에 넣는 while문
    while ((pos_a != a.size) && (pos_b != b.size)){

        //한 항의 차수가 더 클 경우
        if (a.terms[pos_a].expon > b.terms[pos_b].expon){

            //차수가 더 큰 항의 계수와 지수를 결과 값에 넣는다.
            result.terms[pos_result].coef = a.terms[pos_a].coef;
            result.terms[pos_result].expon = a.terms[pos_a].expon;
            //결과 값에 들어가게 된 항을 가진 다항식은 그 다음 항부터 비교하도록 한다
            pos_a++; pos_result++;
        }
        else if (a.terms[pos_a].expon < b.terms[pos_b].expon){
            result.terms[pos_result].coef = b.terms[pos_b].coef;
            result.terms[pos_result].expon = b.terms[pos_b].expon;
            pos_b++; pos_result++;
        }
        //두 항의 차수가 같을 경우
        else{
            //두 항의 계수를 더해 결과 값에 넣고
            result.terms[pos_result].coef = a.terms[pos_a].coef + b.terms[pos_b].coef;
            //한 항의 지수를 결과 값에 넣는다
            result.terms[pos_result].expon = a.terms[pos_a].expon;
            //두 다항식 모두 그 다음 항부터 비교하도록 한다
            pos_a++; pos_b++; pos_result++;
        }
    }

    //두 다항식 중 한 다항식이 크기 비교할 항이 없다면 위의 반복문이 더 이상 실행되지 않는다.
    //그렇다면 두 다항식 중 아직 결과 값에 적용되지 않은 항이 남은 다항식은
    //남은 항을 결과 다항에 단순히 넣기만 하면 된다.
    if (pos_a != a.size){
        for (i = pos_a; i < a.size; ++i) {
            result.terms[pos_result].coef = a.terms[i].coef;
            result.terms[pos_result].expon = a.terms[i].expon;
            pos_result++;
        }
    }
    else if (pos_b != b.size){
        for (i = pos_b; i < b.size; ++i) {
            result.terms[pos_result].coef = b.terms[i].coef;
            result.terms[pos_result].expon = b.terms[i].expon;
            pos_result++;
        }
    }

    //while문과 그 밑에 남은 항을 처리하는 for문을 순회한 횟수가 결과 값 다항식의 길이가 된다
    result.size = pos_result;

    return result;
}

//매개변수로 받은 배열에 원하는 요소가 있는지 체크하여 인덱스를 반환하는 함수
int where_expon_in_list_linear(int list_size, term* expon_list, int expon){
    int i;
    for (i = 0; i < list_size; ++i) {
        if (expon_list[i].expon == expon){
            return i;
        }
    }
    return -1;
}

//두 다항식의 곱셈을 하는 함수
poly mul_poly(poly a, poly b){
    int i, j;
    int temp_index;
    int temp_expon;

    //탐색하기 전 색인역할을 할 배열
    int is_expon_exist[MAX_SIZE] = {0, };

    poly result;
    result.size = 0;
    for (i = 0; i < MAX_SIZE; i++) {
        result.terms[i].coef = 0;
        result.terms[i].expon = 0;
    }
    int pos_result = 0;

    for (i = 0; i < a.size; ++i) {
        for (j = 0; j < b.size; ++j) {
            //두 항을 곱한 값의 차수를 가진 항이
            temp_expon = a.terms[i].expon + b.terms[j].expon;

            //색인에 등록되어 있는지 확인하고 또한 존재한다면
            if (is_expon_exist[temp_expon] == 1){
                //결과 다항식에 이미 존재하는지 체크
                temp_index = where_expon_in_list_linear(pos_result, result.terms, temp_expon);
            }
            //색인에 없다면 탐색자체를 하지 않음
            else {
                temp_index = -1;
            }


            //존재하지 않는다면 결과 값에 넣는다
            if (temp_index == -1){
                result.terms[pos_result].expon = temp_expon;
                result.terms[pos_result].coef = a.terms[i].coef * b.terms[j].coef;
                pos_result++;
                //색인에 해당 차수를 등록한다
                is_expon_exist[temp_expon] = 1;
            }
            //존재한다면 결과 다항식의 해당 항에 더한다
            else{
                result.terms[temp_index].coef += a.terms[i].coef * b.terms[j].coef;
            }
        }
    }

    //이때 결과 다항식의 길이는 이중 for문 안에서 새로운 항이 생긴 횟수이다.
    result.size = pos_result;

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

    for (i = 0; i < p.size; ++i) {
        //x값을 해당 항의 차수만큼 제곱한 값을 해당 항의 계수와 곱한다
        result += p.terms[i].coef * (int)(pow(x, p.terms[i].expon));
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
    print_poly(c);
    poly d = mul_poly(a, b);
    print_poly(d);

    //원하는 다항식 번호와 그 다항식에 원하는 값을 대입한다
    substitute_which_poly(a, b, c, d);

    return 0;
}