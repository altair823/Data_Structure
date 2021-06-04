/*
 * 자료구조 과제 6
 * 그래프
 *
 * 20175154 김태현
 *
 * 2021-06-03
 */

/*
      <ID - Station Table>
0 - 신도림,         15 - 을지로4가,
1 - 신길,           16 - 동대문역사문화공원,
2 - 노량진,         17 - 신당
3 - 용산,           18 - 왕십리
4 - 서울역,         19 - 고속터미널
5 - 시청,           20 - 옥수
6 - 종로3가,        21 - 동대입구
7 - 동대문,         22 - 충무로
8 - 교대,           23 - 동작
9 - 사당,           24- 이촌
10 - 영등포구청,     25 - 삼각지
11 - 당산,          26 - 여의도
12 - 홍대입구,       27 - 공덕
13 - 충정로,         28 - 흑석
14 - 을지로3가,      29 - 효창공원앞
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define STATION_COUNT 30

/*
* Macro that set all distance between stations to EQUAL_DIST.
* This allows us to verify that we have found the correct route and its distance.
*/
//#define SET_ALL_DIST_EQUAL
#define EQUAL_DIST 10

/*
 * The stations struct
 */
typedef struct station{
    //Station ID; see table above.
    int ID;
    //Station name string.
    char *name;
    //The struct pointer array for adjacent stations.
    struct station **adjacent;
    //The distance array for each adjacent stations.
    int *distance;
    //Number of adjacent stations.
    int adj_count;
}Station;

Station *CreateStation(char *s_name, int ID){
    Station *new_station = (Station *)malloc(sizeof(Station));
    new_station->ID = ID;
    int name_len = strlen(s_name) + 1;
    new_station->name = (char *)malloc(sizeof(char) * name_len);
    strcpy(new_station->name, s_name);
    new_station->name[name_len-1] = '\0';
    new_station->adjacent = NULL;
    new_station->distance = NULL;
    new_station->adj_count = 0;
    return new_station;
}

void AddAdjStation(Station *cur_station, Station *adj_station, int distance){
    //DO NOT add station which already existed in adjacent station array. 
    //This function DOES NOT check if the same element exists. 
    cur_station->adj_count++;
    adj_station->adj_count++;
    if (cur_station->adj_count == 1){
        //Initial allocation.
        cur_station->adjacent = (Station **) malloc(sizeof(Station *));
        cur_station->distance = (int *)malloc(sizeof(int));
    } else{
        //Resize the memory allocated.
        cur_station->adjacent = (Station **) realloc(cur_station->adjacent, sizeof(Station *) * cur_station->adj_count);
        cur_station->distance = (int *)realloc(cur_station->distance, sizeof(int) * cur_station->adj_count);
    }

    if (adj_station->adj_count == 1){
        adj_station->adjacent = (Station **) malloc(sizeof(Station *));
        adj_station->distance = (int *)malloc(sizeof(int));
    } else{
        adj_station->adjacent = (Station **) realloc(adj_station->adjacent, sizeof(Station *) * adj_station->adj_count);
        adj_station->distance = (int *)realloc(adj_station->distance, sizeof(int) * adj_station->adj_count);
    }
    
    cur_station->adjacent[cur_station->adj_count-1] = adj_station;
    adj_station->adjacent[adj_station->adj_count-1] = cur_station;
    cur_station->distance[cur_station->adj_count-1] = distance;
    adj_station->distance[adj_station->adj_count-1] = distance;
}


/*
* Create all stations in the route map. 
* And return its array pointer.
* The station count is pre-defined(30). 
*/
Station **InitializeRouteMap(){
    Station **station_list = (Station **)malloc(sizeof(Station *) * STATION_COUNT);
    //Set station name
    {
    
        station_list[0] = CreateStation("신도림", 0);
        station_list[1] = CreateStation("신길", 1);
        station_list[2] = CreateStation("노량진", 2);
        station_list[3] = CreateStation("용산", 3);
        station_list[4] = CreateStation("서울역", 4);
        station_list[5] = CreateStation("시청", 5);
        station_list[6] = CreateStation("종로3가", 6);
        station_list[7] = CreateStation("동대문", 7);
        station_list[8] = CreateStation("교대", 8);
        station_list[9] = CreateStation("사당", 9);
        station_list[10] = CreateStation("영등포구청", 10);
        station_list[11] = CreateStation("당산", 11);
        station_list[12] = CreateStation("홍대입구", 12);
        station_list[13] = CreateStation("충정로", 13);
        station_list[14] = CreateStation("을지로3가", 14);
        station_list[15] = CreateStation("을지로4가", 15);
        station_list[16] = CreateStation("동대문역사문화공원", 16);
        station_list[17] = CreateStation("신당", 17);
        station_list[18] = CreateStation("왕십리", 18);
        station_list[19] = CreateStation("고속터미널", 19);
        station_list[20] = CreateStation("옥수", 20);
        station_list[21] = CreateStation("동대입구", 21);
        station_list[22] = CreateStation("충무로", 22);
        station_list[23] = CreateStation("동작", 23);
        station_list[24] = CreateStation("이촌", 24);
        station_list[25] = CreateStation("삼각지", 25);
        station_list[26] = CreateStation("여의도", 26);
        station_list[27] = CreateStation("공덕", 27);
        station_list[28] = CreateStation("흑석", 28);
        station_list[29] = CreateStation("효창공원앞", 29);
    }

    //Set adjacent station and its edges.
    {
        //신도림
        AddAdjStation(station_list[0], station_list[1], 13);
        AddAdjStation(station_list[0], station_list[10], 20);
        AddAdjStation(station_list[0], station_list[9], 29);
        //신길
        AddAdjStation(station_list[1], station_list[10], 21);
        AddAdjStation(station_list[1], station_list[2], 12);
        AddAdjStation(station_list[1], station_list[26], 21);
        //노량진
        AddAdjStation(station_list[2], station_list[26], 10);
        AddAdjStation(station_list[2], station_list[3], 6);
        AddAdjStation(station_list[2], station_list[28], 6);
        //용산
        AddAdjStation(station_list[3], station_list[29], 15);
        AddAdjStation(station_list[3], station_list[4], 23);
        AddAdjStation(station_list[3], station_list[24], 9);
        //서울역
        AddAdjStation(station_list[4], station_list[5], 17);
        AddAdjStation(station_list[4], station_list[22], 18);
        AddAdjStation(station_list[4], station_list[25], 19);
        //시청
        AddAdjStation(station_list[5], station_list[13], 17);
        AddAdjStation(station_list[5], station_list[6], 21);
        AddAdjStation(station_list[5], station_list[14], 13);
        //종로3가
        AddAdjStation(station_list[6], station_list[13], 20);
        AddAdjStation(station_list[6], station_list[7], 9);
        AddAdjStation(station_list[6], station_list[15], 14);
        AddAdjStation(station_list[6], station_list[14], 18);
        //동대문
        AddAdjStation(station_list[7], station_list[16], 16);
        //교대
        AddAdjStation(station_list[8], station_list[9], 20);
        AddAdjStation(station_list[8], station_list[19], 12);
        //사당
        AddAdjStation(station_list[9], station_list[23], 19);
        //영등포구청
        AddAdjStation(station_list[10], station_list[11], 10);
        //당산
        AddAdjStation(station_list[11], station_list[12], 16);
        AddAdjStation(station_list[11], station_list[26], 8);
        //홍대입구
        AddAdjStation(station_list[12], station_list[13], 19);
        AddAdjStation(station_list[12], station_list[27], 12);
        //충정로
        AddAdjStation(station_list[13], station_list[27], 25);
        //을지로3가
        AddAdjStation(station_list[14], station_list[15], 10);
        AddAdjStation(station_list[14], station_list[22], 12);
        //을지로4가
        AddAdjStation(station_list[15], station_list[16], 16);
        //동대문역사문화공원
        AddAdjStation(station_list[16], station_list[17], 12);
        AddAdjStation(station_list[16], station_list[18], 24);
        AddAdjStation(station_list[16], station_list[22], 24);
        //신당
        AddAdjStation(station_list[17], station_list[18], 20);
        //왕십리
        AddAdjStation(station_list[18], station_list[20], 26);
        //고속터미널
        AddAdjStation(station_list[19], station_list[23], 32);
        AddAdjStation(station_list[19], station_list[20], 21);
        //옥수
        AddAdjStation(station_list[20], station_list[24], 21);
        AddAdjStation(station_list[20], station_list[21], 21);
        //동대입구
        AddAdjStation(station_list[21], station_list[22], 11);
        //충무로
        //동작
        AddAdjStation(station_list[23], station_list[28], 8);
        AddAdjStation(station_list[23], station_list[24], 12);
        //이촌
        AddAdjStation(station_list[24], station_list[25], 17);
        //삼각지
        //여의도
        AddAdjStation(station_list[26], station_list[27], 20);
        //공덕
        AddAdjStation(station_list[27], station_list[29], 6);
        //흑석
        //효창공원앞    
    }

    #ifdef SET_ALL_DIST_EQUAL
    for (int i = 0; i < STATION_COUNT; i++){
        for (int j = 0; j < station_list[i]->adj_count; j++){
            station_list[i]->distance[j] = 10;
        }
    }
    #endif

    return station_list;
}


/*
* The function for printing status of all stations.
* NOT using BFS or DFS
*/
void PrintAllStationStatus(Station **station_list){
    for (int j = 0; j < STATION_COUNT; j++)
    {
        for (int i = 0; i < station_list[j]->adj_count; i++)
        {
            printf("%s - %s - %d\n", station_list[j]->name, station_list[j]->adjacent[i]->name, station_list[j]->distance[i]);
        }
        printf("%d\n", station_list[j]->adj_count);
    }
}

/*
* Statk implements for DFS search. 
*/
#define PUSH(element)	((void) (*top = (element), ++top))
#define	POP(element)	((void) (--top, (element) = *top))
#define IS_STACK_EMPTY (stack == top)

#define VISITED 1
#define NOT_VISITED 0
/*
* The function printing all station information by DFS.
*/
void PrintAllStationDFS(Station **station_list){
    Station *stack[STATION_COUNT];
    Station **top = stack;

    char visted[STATION_COUNT] = {NOT_VISITED, };

    //Start list index is 0
    int start_index = 0;

    Station *current = station_list[start_index];

    while (1){
        //Flag that indicating the vertex can visited. 
        int can_go = 0;
        //Print only unvisited vertex.
        if (visted[current->ID] == NOT_VISITED){
            printf("%s\n", current->name);
        }
        visted[current->ID] = VISITED;
        for (int i = 0; i < current->adj_count; i++){
            if (visted[current->adjacent[i]->ID] == NOT_VISITED){
                can_go = 1;
                PUSH(current);
                current = current->adjacent[i];
                break;
            }
        }
        //If there is no unvisited vertex,
        if (can_go == 0){
            //If there is no element to pop in stack, break.
            if (top == stack){
                break;
            } else {            
                POP(current);
            }
        }
    }
}

/*
* Inputing just ONE station name.
* and return its ID. 
*/
int InputAStationName(Station **station_list){
    char buffer[100];
    scanf("%s", buffer);
    int len = strlen(buffer) + 1;
    char *name = (char *)malloc(sizeof(char) * len);
    strcpy(name, buffer);
    name[len] = '\0';

    for (int i = 0; i < STATION_COUNT; i++){
        if (strcmp(station_list[i]->name, name) == 0){
            //return finded station name.
            return station_list[i]->ID;
        }
    }
    //If there is no such named station, return NULL.
    return -1;
}

typedef struct{
    int is_found;
    int from_index;
    int distance;
}ShortestPath;

#define FOUND 1
#define NOT_FOUND 0
#define INF INT_MAX

/*
* Update all distances to adjacent vertices
* if there is new shorter distance appears. 
*/
void UpdateVertexDistance(Station **station_list, ShortestPath *path_list){
    //For all vertices in found_set,
    for (int i = 0; i < STATION_COUNT; i++){
        if (path_list[i].is_found == FOUND){
            //For all each adjacent vertices of it, 
            for (int j = 0; j < station_list[i]->adj_count; j++){
                //And the adjacent vertex is not include in found_set, 
                if (path_list[station_list[i]->adjacent[j]->ID].is_found == NOT_FOUND){
                    //If the stored distance of adjacent vertex in path_list 
                    //is longer than
                    //the sum of distance between start vertex to current vertex
                    //and distance between current vertex to adjacent vertex,
                    //update the shortest distance in path_list.
                    if (path_list[station_list[i]->adjacent[j]->ID].distance > path_list[i].distance + station_list[i]->distance[j]){
                        path_list[station_list[i]->adjacent[j]->ID].distance = path_list[i].distance + station_list[i]->distance[j];
                        path_list[station_list[i]->adjacent[j]->ID].from_index = i;
                    }
                }
            }
        }        
    }
}

/*
* Input start station and end station. 
* And excute dijkstra algorithm to find shortest path.
* Returning stations array of shortest path.
*/
ShortestPath *FindShortestPath(Station **station_list, int start, int end){
    int start_index = start;
    int end_index = end;
    
    //List stored shortest path of each vertices. 
    //Vertices that have FOUND value on 'is_found' are included the found vertices set!
    //HAVE TO FREE!
    ShortestPath *path_list = (ShortestPath *)malloc(sizeof(ShortestPath) * STATION_COUNT);
    //The initial value of shortes distance to other vertices are INFINITE! 
    for (int i = 0; i < STATION_COUNT; i++){
        path_list[i].is_found = NOT_FOUND;
        path_list[i].distance = INF;
    }

    //Start station always has 0 distance, and is included found set.
    path_list[start_index].from_index = start_index;
    path_list[start_index].distance = 0;
    path_list[start_index].is_found = FOUND;

    //This is indicated the closest vertex index from found vertices set.
    int closest_index = -1;

    //If we're reached the end_index(destination),
    //we don't need to search shortest path continue.
    while (closest_index != end_index){
        //Update the shortest paths from the found verticex set
        //that just included the closest vertex.
        UpdateVertexDistance(station_list, path_list);
        int min_dist = INF;
        for (int i = 0; i < STATION_COUNT; i++){
            if (path_list[i].is_found == NOT_FOUND && path_list[i].distance < min_dist){
                min_dist = path_list[i].distance;
                closest_index = i;
            }
        }
        //Include the closest vertex to found set.
        path_list[closest_index].is_found = FOUND;
    }
    return path_list;
}

/*
* Extract the Shortest path (start_vertex to end_vertex)
* from given dijkstra shortest path list. 
*/
int *ExtractShortestPath(ShortestPath *path_list, int start_index, int end_index){
    int index = 0;
    int path_reversed[STATION_COUNT + 1] = {0, };
    index = end_index;
    path_reversed[0] = index;
    int path_count = 1;
    for (int i = 0; index != start_index; i++){
        index = path_list[index].from_index;
        path_reversed[i + 1] = index;
        path_count++;
    }

    int *shortest_path = (int *)malloc(sizeof(int) * (path_count + 1));

    for (int i = 0; i < path_count; i++){
        shortest_path[i] = path_reversed[path_count - (i + 1)];
    }
    shortest_path[path_count] = -1;

    return shortest_path;

}

int main(){
    Station **station_list;
    station_list = InitializeRouteMap();

    Station *visited = NULL;

    printf("모든 역을 표시합니다 >>> \n");
    PrintAllStationDFS(station_list);

    printf("출발역을 입력하시오 >>> ");
    int start_index = InputAStationName(station_list);

    printf("도착역을 입력하시오 >>> ");
    int end_index = InputAStationName(station_list);

    ShortestPath *path_list = FindShortestPath(station_list, start_index, end_index);
    int *shortest_path_list = ExtractShortestPath(path_list, start_index, end_index);
    printf("경로와 거리를 표시 >>>\n");
    for (int i = 0; shortest_path_list[i] != -1; i++){
        printf("%s ", station_list[shortest_path_list[i]]->name);
        if (shortest_path_list[i+1] != -1){
            printf("- ");
        }
    }
    printf("(%.1fkm)\n", (double)path_list[end_index].distance/10);

    free(path_list);
    free(shortest_path_list);
	return 0;
}
