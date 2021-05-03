/*
 * 자료구조 과제 1
 * 행렬
 *
 * 2020-03-20
 * 만든이: 20175154 김태현
 *
 * version 1.3
 * 행렬을 두 가지 방법으로 저장하고 계산하는 프로그램
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//행렬 한 행 또는 열의 최대 길이
#define MAX_SIZE 9

//행렬을 2차원 배열로 저장하는 구조체
typedef struct normal{

    //행렬의 크기, 정방형 행렬만 입력되므로 한 행 또는 열의 길이만 저장하도록 한다.
    int size;

    //행렬을 저장할 2차원 배열
    int element[MAX_SIZE][MAX_SIZE];

}matrix_normal;

//아래 행렬 저장방식을 위한 행렬의 요소 구조체
typedef struct sparse_element{
    int row;
    int col;
    int data;
}sparse_element;

//행렬을 0이 아닌 요소들만 저장하는 구조체
typedef struct sparse{

    //요소 구조체 배열의 길이
    int list_size;

    //요소 구조체 배열
    sparse_element element[MAX_SIZE * MAX_SIZE];

}matrix_sparse;

//행렬의 요소를 차례대로 입력받아 이를 방법 1, 방법 2로 각각 저장하는 함수
void create_matrix(int mat_num, matrix_normal *matrix_n, matrix_sparse *matrix_s){

    ///////////////////////////
    //두 저장 방법 모두 사용하는 코드//
    ///////////////////////////
    int i;

    matrix_n->size = 0;

    //모든 행렬의 요소를 공백을 기준으로 차례대로 입력받는다.
    printf("행렬 %d의 데이터를 입력하세요: ", mat_num);
    char input[100];
    scanf("%[^\n]s", input);

    //공백 문자를 기준으로 입력받은 요소들을 분리하여 저장한다.
    //행렬의 크기는 입력받지 못하므로 요소들의 개수에 따라
    //행렬의 크기에 맞춰 저장해야 한다.
    //먼저 버퍼 역할을 할 배열을 정의해 그 배열에 차례대로 저장한다.
    char *str_ptr = strtok(input, " ");
    int buffer[MAX_SIZE*MAX_SIZE] = {0, };
    for (i = 0; str_ptr != NULL; ++i) {
        buffer[i] = atoi(str_ptr);
        str_ptr = strtok(NULL, " ");
    }

    //버퍼 배열의 크기가 몇의 제곱인지 알아낸다.
    double input_matrix_size = sqrt(i);

    //만약 입력받은 행렬 요소의 개수에 제곱근을 취했을 때 자연수로 떨어지지 않으면 입력이 잘못된 것이다.
    if (input_matrix_size - (double)(int)input_matrix_size > 0){
        printf("정방형 행렬이 아닙니다.\n");
        exit(9);
    }
    getchar();d

    ///////////////////////////
    //행렬 저장 방법 1 관련 코드 시작//
    ///////////////////////////

    //행렬의 크기를 저장
    matrix_n->size = (int)input_matrix_size;
    int buffer_index = 0;
    int row, col;

    //계산된 행렬의 크기만큼 이중 반복문을 돌며 버퍼에 저장된 데이터들을 2차원 행렬에 저장한다.
    for (row = 0; row < matrix_n->size; ++row) {
        for (col = 0; col < matrix_n->size; ++col) {
            matrix_n->element[row][col] = buffer[buffer_index];
            buffer_index++;
        }
    }

    ///////////////////////////
    //행렬 저장 방법 2 관련 코드 시작//
    ///////////////////////////

    int matrix_s_index = 0;
    buffer_index = 0;
    //이중 반복문을 돌며 버퍼에 저장된 데이터가 0이 아닐 경우에만 저장한다.
    for (row = 0; row < (int)input_matrix_size; ++row) {
        for (col = 0; col < (int)input_matrix_size; ++col) {
            if (buffer[buffer_index] != 0){
                matrix_s->element[matrix_s_index].row = row;
                matrix_s->element[matrix_s_index].col = col;
                matrix_s->element[matrix_s_index].data = buffer[buffer_index];
                buffer_index++;
                matrix_s_index++;
            }
            else {
                buffer_index++;
            }
        }
    }

    //저장된 0이 아닌 요소들의 개수를 저장한다.
    matrix_s->list_size = matrix_s_index;
}

//방법 1로 저장한 행렬을 출력하는 함수
void print_matrix_normal(char *matrix_num, matrix_normal m){
    int i, j;
    printf("행렬 %s(%d)\n", matrix_num, m.size);
    //단순히 이중 반복문을 돌며 행렬의 모든 데이터를 출력한다.
    for (i = 0; i < m.size; ++i) {
        for (j = 0; j < m.size; ++j) {
            printf("%d ", m.element[i][j]);
        }
        printf("\n");
    }
}

//방법 2로 저장한 행렬을 출력하는 함수
void print_matrix_sparse(char *matrix_num, matrix_sparse m){
    int i;
    printf("행렬 %s(%d개)\n", matrix_num, m.list_size * 3);
    //저장된 0이 아닌 모든 요소들을 행, 열 값과함께 출력한다.
    for (i = 0; i < m.list_size; ++i) {
        printf("%d %d %d\n", m.element[i].row, m.element[i].col, m.element[i].data);
    }
}

//방식 1로 저장한 두 행렬을 더하여 결과 값을 반환하는 함수
matrix_normal add_normal(matrix_normal a, matrix_normal b){
    //두 행렬의 크기가 다르면 프로그램을 종료한다.
    if (a.size != b.size){
        printf("두 행렬의 크기가 다릅니다! 덧셈을 진행할 수 없습니다 \n");
        exit(8);
    }

    int i, j;
    //결과 값
    matrix_normal result;
    result.size = a.size;

    //두 행렬의 대응하는 항끼리 더해 결과 값에 대입한다.
    for (i = 0; i < a.size; ++i) {
        for (j = 0; j < a.size; ++j) {
            result.element[i][j] = a.element[i][j] + b.element[i][j];
        }
    }

    return result;
}

//방식 2로 저장한 두 행렬을 더하여 결과 값을 반환하는 함수
matrix_sparse add_sparse(matrix_sparse c, matrix_sparse d) {

    int i;
    int c_index = 0;
    int d_index = 0;
    int result_index = 0;
    matrix_sparse result;

    //두 행렬 중 하나의 행렬이 더 이상 비교할 요소가 없을 때 까지 반복한다.
    while ((c_index != c.list_size) && (d_index != d.list_size)) {

        //비교하고자 하는 요소 중 c의 요소의 행이 더 작다면
        if (c.element[c_index].row < d.element[d_index].row) {
            //그 요소를 결과 값에 대입
            result.element[result_index] = c.element[c_index];
            //인덱스 ++
            result_index++;
            c_index++;
        }
            //d의 요소의 행이 더 작다면
        else if (c.element[c_index].row > d.element[d_index].row) {
            result.element[result_index] = d.element[d_index];
            result_index++;
            d_index++;
        }
            //두 요소의 행이 같다면 열을 비교한다.
        else {
            //c의 요소가 열이 작다면
            if (c.element[c_index].col < d.element[d_index].col) {
                //결과 값에 c의 요소 대입
                result.element[result_index] = c.element[c_index];
                //인덱스 ++
                result_index++;
                c_index++;
            }
                //d의 요소가 열이 작다면
            else if (c.element[c_index].col > d.element[d_index].col) {
                result.element[result_index] = d.element[d_index];
                result_index++;
                d_index++;
            }
                //두 요소의 행과 열이 모두 같다면
            else {
                //일단 c의 값을 대입하고 d의 값을 더한다.
                result.element[result_index] = c.element[c_index];
                result.element[result_index].data += d.element[d_index].data;
                result_index++;
                c_index++;
                d_index++;
            }
        }
    }

    //c에 결과 값에 적용되지 않은 요소가 남아있을 때
    if (c_index != c.list_size) {
        //남은 모든 요소를 결과 값에 대입한다.
        for (i = c_index; i < c.list_size; ++i) {
            result.element[result_index] = c.element[i];
            result_index++;
        }
    }
    //d에 요소가 남아있을 때
    else if (d_index != d.list_size) {
        for (i = d_index; i < d.list_size; ++i) {
            result.element[result_index] = d.element[i];
            result_index++;
        }
    }

    //마지막으로 결과 값의 요소들의 개수를 저장한다.
    result.list_size = result_index;

    return result;
}

//한 행과 한 열을 매개변수로 받아 그 곱을 한 정수로 반환하는 함수
int mul_row_col(int size_row, int row[], int size_col, int col[]){
    //행과 열의 길이가 같은지 확인
    if (size_row != size_col){
        printf("행과 열의 길이가 다릅니다! \n");
        exit(7);
    }

    int i;
    int result = 0;

    //인덱스 번호가 같은 행과 열의 요소끼리 곱한다.
    for (i = 0; i < size_row; ++i) {
        result += row[i] * col[i];
    }
    return result;
}

//행렬에서 한 열을 배열로 반환하는 함수
//행은 쉽게 반환할 수 있지만 열은 그렇지 않기 때문에 필요
int* get_col(int col_num, matrix_normal m){
    //열을 배열로서 반환하기 위해 static 배열로 반환값을 정의한다.
    static int result[MAX_SIZE];
    int i;
    for (i = 0; i < m.size; ++i) {
        result[i] = m.element[i][col_num];
    }
    return result;
}

//방식 1로 저장한 두 행렬을 곱하여 결과 값을 반환하는 함수
matrix_normal multiply_normal(matrix_normal a, matrix_normal b){

    //이 문제에서는 두 행렬이 같은 크기의 정방형 행렬이라 결과 행렬도 같은 크기이지만
    //조건이 달라진다면 결과 행렬의 행의 크기과 열의 크기를 각각 따로 곱하려는 행렬들에서 대입해야 할 것이다.
    matrix_normal result;
    result.size = a.size;

    int row_index, col_index;

    //원하는 행과 열을 곱해 하나씩 결과 값에 대입한다.
    for (row_index = 0; row_index < result.size; ++row_index) {
        for (col_index = 0; col_index < result.size; ++col_index) {
            result.element[row_index][col_index] = mul_row_col(a.size, a.element[row_index], b.size, get_col(col_index, b));
        }
    }

    return result;
}

//매개변수로 받은 행과 열을 가지는 데이터의 인덱스를 반환하는 함수
int where_is_data(int row, int col, matrix_sparse m){
    int i;
    for (i = 0; i < m.list_size; ++i) {
        if ((m.element[i].row == row) && (m.element[i].col == col)){
            return i;
        }
    }
    printf("해당 요소가 존재하지 않습니다!\n");
    return -1;
}

//방식 2로 저장한 두 행렬을 곱하여 결과 값을 반환하는 함수
matrix_sparse multiply_sparse(matrix_sparse c, matrix_sparse d){
    int i;

    //결과 값을 저장할 저장 방식 2의 행렬
    matrix_sparse result;

    int c_index;
    int d_index;
    int result_index = 0;

    //색인 역할을 할 2차원 배열
    int is_data_exist[MAX_SIZE][MAX_SIZE] = {0, };

    //이중 반복문을 돌며 c 요소의 열의 값과 같은 값의 행을 가진 d의 요소를 찾는다.
    for (c_index = 0; c_index < c.list_size; ++c_index) {
        for (d_index = 0; d_index < d.list_size; ++d_index) {

            //만약 한 요소의 열의 값과 다른 한 요소의 행의 값이 같다면
            if (c.element[c_index].col == d.element[d_index].row){

                //만약 색인을 통해 해당 위치의 결과 값이 존재하지 않다는 것이 확인되면
                if (is_data_exist[c.element[c_index].row][d.element[d_index].col] != 1) {

                    //한 요소의 행의 값과 다른 한 요소의 열의 값을 가진 결과 값의 위치에 두 요소 데이터 값의 곱을 넣는다.
                    result.element[result_index].row = c.element[c_index].row;
                    result.element[result_index].col = d.element[d_index].col;
                    result.element[result_index].data = c.element[c_index].data * d.element[d_index].data;

                    //결과 값의 해당위치에 데이터가 존재한다는 것을 색인에 등록한다.
                    is_data_exist[result.element[result_index].row][result.element[result_index].col] = 1;
                    result_index++;
                    result.list_size = result_index;
                }

                //결과 값의 해당 위치에 데이터가 존재한다면
                else{
                    //해당 위치의 인덱스 번호를 요소 구조체 배열에서 찾고
                    int exist_data_index = where_is_data(c.element[c_index].row, d.element[d_index].col, result);

                    //두 요소를 곱한 값을 그 값에 더한다.
                    result.element[exist_data_index].data += c.element[c_index].data * d.element[d_index].data;
                }
            }
        }
    }

    //result.list_size = result_index;

    return result;

}

int main(){
    matrix_normal a, b;
    matrix_sparse c, d;

    //두 행렬을 입력받는다.
    create_matrix(1, &a, &c);
    create_matrix(2, &b, &d);

    //방법 1로 저장한 행렬들을 출력한다.
    printf("방식 1\n");
    print_matrix_normal("1", a);
    print_matrix_normal("2", b);

    //방법 1로 저장한 행렬들을 더하여 결과를 출력한다.
    print_matrix_normal("1 + 2", add_normal(a, b));
    //방법 1로 저장한 행렬들을 곱하여 결과를 출력한다.
    print_matrix_normal("1 * 2", multiply_normal(a, b));

    //방법 2로 저장한 행렬들을 출력한다.
    printf("방식 2\n");
    print_matrix_sparse("3", c);
    print_matrix_sparse("4", d);

    //방법 2로 저장한 행렬들을 더하여 결과를 출력한다.
    print_matrix_sparse("3 + 4", add_sparse(c, d));
    //방법 2로 저장한 행렬들을 곱하여 결과를 출력한다.
    print_matrix_sparse("3 * 4", multiply_sparse(c, d));

    return 0;
}