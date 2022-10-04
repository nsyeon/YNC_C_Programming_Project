#define _CRT_SECURE_NO_WARNINGS  //에러 방지
#include <stdio.h>   //printf, scanf_s 함수를 사용하기 위함
#include <stdlib.h>   //srand, rand 함수를 사용하기 위함
//지뢰의 위치는 rand함수로 생성한 난수를 사용할 것이기에 
//난수의 기준값을 현재 시간으로 사용
#include <time.h>   //time 함수를 사용하기 위함
#include <memory.h>   //memset 함수를 사용하기 위함
#include <windows.h> //color를 사용하기 위함
#include <mmsystem.h>   //sound를 사용하기 위함
#pragma comment(lib, "winmm.lib")//sound를 사용하기 위함
#include <mysql.h>  //DB연동을 위함
#pragma comment(lib, "libmysql.lib") //추가종속성, 링커-입력-추가종속성 ==> libmysql.lib, 오타가 날 수 있기에 입력

#define DB_HOST "127.0.0.1" //DB 호스트 이름
#define DB_USER ""  //DB 유저 이름
#define DB_PASS ""    //DB 비밀번호
#define DB_NAME "login" //DB 이름
#define CHOP(x) x[strlen(x) - 1] =' '   //키보드로 입력을 할 때 엔터를 쳐야하기 때문에 엔터의 위치가 되며, 
                                        //엔터키를 공백으로 바꿔주는 문자열의 끝을 의미

MYSQL* connection = NULL, conn; //MYSQL이라는 구조체로 connection 포인터 변수 선언, conn이라는 구조체 변수 선언
MYSQL_RES* sql_result;  //MYSQL_RES라는 구조체를 사용하여  sql_result변수를 이용하여 결과값을 받아오기 위함
MYSQL_ROW sql_row;  //MYSQL_ROW라는 구조체를 사용하여  sql_row 변수를 이용하여 ROW단위로 문자열을 읽어오기 위함

char id[20] = { 0, };   //아이디를 입력받을 수 있도록 배열 생성
char passwd[20] = { 0, };   //비밀번호를 입력받을 수 있도록 배열 생성
char query[255] = { 0, };   //쿼리라는 버퍼를 하나 만듦

//랭킹을 위한 선언
int best_rsp = 0;
int best_m_n = 0;
int best_m_s = 0;

void main(void) {
    start();
}

//로그인 및 회원가입
int start(void) {
    system("mode con: cols=32 lines=15");   //창크기 조절

    int user_input = 0; //사용자가 입력하는 값

    system("cls");  //화면 지움
    //하얀색으로 메시지를 출력함
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃                            ┃\n");
    printf("┃        콘솔창 게임         ┃\n");
    printf("┃                            ┃\n");
    printf("┃                            ┃\n");
    printf("┃     1.로그인/회원가입      ┃\n");
    printf("┃     2.종료                 ┃\n");
    printf("┃                            ┃\n");
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
    printf(" 선택 :  ");  //printf()로 로그인/회원가입 그리고 종료를 선택할 수 있는 메시지 출력
    scanf("%d", &user_input);   //사용자로부터 user_input을 정수형으로 입력받음

    switch (user_input) {     //사용자가
    case 1: //1번을 입력하면
        login();  //로그인 및 회원가입 실행
        break;
    case 2: //2번을 입력하면
        exit(0);    //종료
    }
    //메뉴 화면으로 넘어감
    menu();

    return 0;
}


//메뉴
int menu(void) {

    system("mode con: cols=32 lines=15");   //창 크기 조절

    int user_input = 0; //사용자가 입력하는 값

    system("cls");  //화면 지워짐
    //하얀색으로 메시지를 출력함
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃                            ┃\n");
    printf("┃        콘솔창 게임         ┃\n");
    printf("┃                            ┃\n");
    printf("┃     1.가위바위보           ┃\n");
    printf("┃     2.숫자 맞히기          ┃\n");
    printf("┃     3.지뢰찾기             ┃\n");
    printf("┃     4.종료                 ┃\n");
    printf("┃                            ┃\n");
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
    printf(" 선택 :  ");  //printf()로 게임을 선택할 수 있는 메시지 출력
    scanf("%d", &user_input);   //사용자로부터 user_input을 정수형으로 입력받음

    switch (user_input) {     //사용자가
    case 1: //1번을 입력하면
        rsp();  //가위바위보 실행
        break;
    case 2: //2번을 입력하면
        m_n();  //숫자 맞추기 실행
        break;
    case 3: //3번을 입력하면
        m_s();  //지뢰찾기 실행
        break;
    case 4: //4번을 입력하면
        exit(0);    //종료

    }
    return 0;
}



//로그인 및 회원가입
int login(void) {

    system("mode con: cols=32 lines=17"); //창 크기 조절

    int query_stat = 0; //키보드로 입력을 할 때 엔터를 쳐야하기 때문에 엔터의 위치가 되며, 
                        //엔터키를 공백으로 바꿔주는 문자열의 끝을 의미
    char ans = 0;   //ans 변수 선언
    int ls = 0;     //로그인 성공여부를 위해 ls 변수 선언

    //mysql_real_connect() 함수를 위해 초기화
    mysql_init(&conn);

    //한글 깨짐을 방지하기 위함
    mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
    mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

    //mysql_real_connect : host에서 실행중인 MySQL 데이터베이스 엔진에 연결을 설정하려고 시도하는 함수
    //MYSQL DB에 연결한 정보를 connection에 담음
    connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

    //에러가 왜 발생했는지 에러메시지 출력
    if (connection == NULL) {
        fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
        return 1;
    }

    //DB에 MYSQL 명령어 정보를 던져줌
    query_stat = mysql_query(connection, "select * from login");
    if (query_stat != 0) {
        fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
        return 1;
    }

    //메모리상에 공간이 확보되어 DB에 있는 내용이 저장됨
    sql_result = mysql_store_result(connection);

    //mysql_fetch_row()를 이용하여 전체 내용을 한 row씩 꺼내옴
    printf("%10s %10s\n", "아이디", "패스워드");
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
        printf("%10s %10s\n", sql_row[0], sql_row[1]);
    }
    //mysql_store_result() 함수에 의해 할당된 메모리를 해제한다
    mysql_free_result(sql_result);

    //무한루프
    while (1) {

        printf("\n회원가입을 하시겠습니까? :");    //printf()로 회원가입을 하겠는지 메시지 출력
        scanf(" %c", &ans); //사용자로부터 ans을 하나의 문자로 입력받음
        scanf("%*c");   //엔터키 지움

        //회원가입을 한다고 했을 때
        if (ans == 'y' || ans == 'Y') {
            //무한루프
            while (1) {
                printf("\n아이디 : "); //printf()를 이용하여 아이디 입력
                fgets(id, 20, stdin);   //한 줄이 너무 길어서 저장할 배열의 크기보다 크다면 
                //한 줄을 다 읽지 않고 중단할 수 있고 표준 입력(stdin) 에서 입력을 받기위해 stdin 입력
                CHOP(id);    //엔터키가 들어오면 엔터키를 공백으로 바꿔줌

                printf("패스워드 : ");  //printf()를 이용하여 비밀번호 입력
                fgets(passwd, 20, stdin);   //한 줄이 너무 길어서 저장할 배열의 크기보다 크다면 
                //한 줄을 다 읽지 않고 중단할 수 있고 표준 입력(stdin) 에서 입력을 받기위해 stdin 입력
                CHOP(passwd);    //엔터키가 들어오면 엔터키를 공백으로 바꿔줌

                //입력한 id와 DB에 같은게 있는지 검사
                sprintf(query, "select * from login where id = '%s'", id);

                query_stat = mysql_query(connection, query); //DB에 던져줌
                if (query_stat != 0) {  //만약에 오류가 발생하면 실패원인 출력
                    fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
                    return 1;
                }

                sql_result = mysql_store_result(connection); //실행된 결과값을 메모리에 올리고 주소값을 얻음
                if (sql_row = mysql_fetch_row(sql_result) == NULL) { //만약 row가 NULL값이면 즉, id가 같은 값이 없으면
                    sprintf(query, "insert into login(id, passwd) values" "('%s', '%s')", id, passwd);  //다시 한번 query에 insert문을 넣어줌

                    //DB에 던져줌
                    query_stat = mysql_query(connection, query);

                    //insert문을 넣은 query배열을 DB에 넣어준 다음, 잘 들어갔는지 확인하는 오류 체크
                    if (query_stat != 0) {
                        fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
                        return 1;
                    }

                    printf("\n회원가입 완료");   //회원가입 완료 확인

                    _getch();    //아무 키를 입력하기 전까지 화면 멈춤
                    break;  //빠져나감
                }
                //NULL값이 아닌 경우
                else {
                    printf("중복된 아이디가 있습니다.\n"); //중복된 아이디가 있으면 위로 올라가서 다시 아이디와 비밀번호 입력

                    _getch();    //아무 키를 입력하기 전까지 화면 멈춤
                    system("cls");  //화면 지움

                    continue; //계속
                }
            }
        }

        //회원가입을 하지 않고 로그인을 한다고 했을 때
        else if (ans == 'n' || ans == 'N') {
            //무한루프
            while (1) {
                printf("\n아이디 : "); //printf()를 이용하여 아이디 입력
                fgets(id, 20, stdin);   //한 줄이 너무 길어서 저장할 배열의 크기보다 크다면 
                //한 줄을 다 읽지 않고 중단할 수 있고 표준 입력(stdin) 에서 입력을 받기위해 stdin 입력
                CHOP(id);   //엔터키가 들어오면 엔터키를 공백으로 바꿔줌

                printf("패스워드 : ");  //printf()를 이용하여 비밀번호 입력
                fgets(passwd, 20, stdin);   //표준 입력(stdin) 에서 입력을 받기위해 stdin 입력
                CHOP(passwd);    //엔터키가 들어오면 엔터키를 공백으로 바꿔줌

                //명령문 자체를 실행시키기위해서 query배열에 넣어주고 그 query배열 모두를 DB에 넣어줌
                sprintf(query, "select * from login where id = '%s' and passwd = '%s'", id, passwd);

                //비쥬얼 스튜디오에서 DB결과창을 보여주지 않아서 혹시나 오타 등으로 인한 에러가 발생했는데
                //그냥 모르고 넘어갈 수 있으니 항상 체크를 해야 하기 때문에 query_stat 변수 사용
                query_stat = mysql_query(connection, query);  //mysql_query는 db에 값(실질적인 값 말고 명령문)을 입력해주는 함수
                //connection에 query 즉, 명령문을 넣겠다 ( = 연동되어있는 DB에가서 select문을 실행해라)
                //0이 아니면 즉, 오류가 발생했다면 fprintf를 실행하고 return으로 종료
                if (query_stat != 0) {
                    fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
                    //stderr은 에러의 표현 형태이며 mysql_error(&conn)을 이용하여 에러에 대한 문자열을 나타냄
                    return 1;
                }

                sql_result = mysql_store_result(connection); //실행된 DB값을 메모리에 넣음 (= sql 결과 저장)


                //행의 시작주소만 계속 가지고오면 하나의 행씩 읽을 수 있음
                while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                    printf("%10s %10s\n", sql_row[0], sql_row[1]);  //그 행의 첫번째 값(아이디)은 sql_row[0], 두번째 값(비밀번호)은 sql_row[1]
                    printf("\n로그인 성공\n");   //printf()로 로그인 성공 메시지 출력
                    best_rsp = atoi(sql_row[2]);
                    best_m_n = atoi(sql_row[3]);
                    best_m_s = atoi(sql_row[4]);
                    ls++;
                    _getch();   //엔터 입력하기 전까지 화면 멈춤
                }
                if (ls > 0) {   //로그인 성공 시
                    break;  //빠져나감
                }
                else {  //로그인 성공 못할 시
                    printf("\n로그인 실패\n");   //printf()로 로그인 실패 메시지 출력

                    _getch();    //아무 키를 입력하기 전까지 화면 멈춤
                    system("cls");  //화면 지움

                    continue;   //계속
                }
            }
        }
        //잘못 입력할 경우
        else {
            printf("잘못 입력하셨습니다\n"); //printf()로 잘못 입력했다는 메시지 출력
            _getch();    //아무 키를 입력하기 전까지 화면 멈춤
            system("cls");  //화면 지움
        }
        //회원가입 혹은 로그인을 하면 메뉴 화면으로 넘어감
        menu();
    }
    mysql_close(connection);    //DB연동 해제
    return 0;
}


//지뢰찾기

//지뢰찾기 판의 크기를 가로 6, 세로 6으로 맞추고
//지뢰의 수는 7개로 고정
#define MINE_COUNT 7
#define X_COUNT 6
#define Y_COUNT 6

//게임을 시작하기 위해 지뢰가 설치된 게임 정보를 구성해야 하기에 CreateMineTable 함수를 만듦
void CreateMineTable(char mine_table[][X_COUNT], char check_table[][X_COUNT]) {
    int x, y, mine_count;   //함수 선언
    //CreateMineTable 함수의 초기화 루틴
    memset(mine_table, 0, Y_COUNT * X_COUNT);   //mine 정보를 저장할 메모리에 모두 0을 채움
    memset(check_table, 0, Y_COUNT * X_COUNT);   //사용자의 선택 정보를 저장할 메모리에 모두 0을 채움

    //MINE_COUNT개만큼 반복하면서 rand함수로 지뢰 설치
    //지뢰는 *로 설정
    //중복된 위치에 폭탄이 설치되지 않도록 지뢰의 위치를 체크하면서 생성해야 함
    for (mine_count = 0; mine_count < MINE_COUNT; mine_count++) {     //지뢰를 MINE_COUNT개만큼 생성함
       //난수를 사용하여 지뢰가 위치할 곳 생성
        x = rand() % X_COUNT;
        y = rand() % Y_COUNT;
        //지뢰가 설치된 곳이 아니라면 폭탄을 설치하고
        //이미 설치된 곳이라면 지뢰의 수를 줄여서 다시 시도하게 함
        if (mine_table[y][x] == 0) mine_table[y][x] = '*';
        else mine_count--;
    }

    //입력한 위치 주변에 몇 개의 지뢰가 있는지 체크하여 문자 형식의 숫자로 저장
    //즉, 지뢰가 아닌 칸은 0값을 가지고 있는데 0값을 가진 칸을 만나면
    //그 항목을 감싸고 있는 8칸에 지뢰가 있는지 체크하여 
    //만약 5개가 있다면 '5'문자를 기억
    //이때 나올 수 있는 최대 지뢰의 수는 8이지만, 현재 지뢰의 수가 7이기에 7이 최댓값
    for (y = 0; y < Y_COUNT; y++) {
        for (x = 0; x < X_COUNT; x++) {
            //지뢰가 아니라면 인접한 위치에 있는 지뢰가 몇 개 있는지 체크
            if (mine_table[y][x] == 0) {
                //지뢰의 수 초기화
                mine_count = 0;
                //현재 위치 (C)를 감싸고 있는 8칸에 폭탄이 몇 개 있는지 체크
                // 1, 2, 3
                // 4, C, 6
                // 7, 8, 9

                if ((y - 1) >= 0) {   //음수가 나오지 않게 체크

                   //1, 2, 3번 칸 체크
                    if ((x - 1) >= 0 && mine_table[y - 1][x - 1] == '*') mine_count++;
                    if (mine_table[y - 1][x] == '*') mine_count++;
                    if ((x + 1) < X_COUNT && mine_table[y - 1][x + 1] == '*') mine_count++;
                }
                //4, 5번 칸 체크
                if ((x - 1) >= 0 && mine_table[y][x - 1] == '*') mine_count++;
                if ((x + 1) < X_COUNT && mine_table[y][x + 1] == '*') mine_count++;

                //6, 7, 8번 칸 체크
                if ((y + 1) < Y_COUNT) {   //범위를 벗어나지 않게 체크
                    if ((x - 1) >= 0 && mine_table[y + 1][x - 1] == '*') mine_count++;
                    if (mine_table[y + 1][x] == '*') mine_count++;
                    if ((x + 1) < X_COUNT && mine_table[y + 1][x + 1] == '*') mine_count++;
                }
                //계산된 지뢰의 수를 정수에서 문자로 변경하여 현재 위치에 대입
                //5라면 '5'로 저장
                mine_table[y][x] = '0' + mine_count;
            }
        }
    }
}

//생성된 값이 정확하게 생성되었는지 확인하기 위해 
//mine_table값을 출력하는 ShowMineTable 함수 구성
//ShowMineTable함수는 폭탄 설치 정보를 모두 보여주기에 게임이 끝났을 때 사용
void ShowMineTable(char mine_table[][X_COUNT]) {
    printf("\n ");   //줄바꿈
    for (int y = 0; y < Y_COUNT; y++) {
        printf("\t\t ");    //탭
        //한 줄의 정보 출력
        for (int x = 0; x < X_COUNT; x++) printf("%c", mine_table[y][x]);
        printf("\n");   //줄바꿈
    }
    printf("\n");   //줄바꿈
}

//'@'로 표시된 칸은 아직 사용자가 입력하지 않은 칸이고
//숫자로 표시된 칸은 입력이 된 칸
//확인할 위치를 음수로 입력하면 게임이 종료되고
//지뢰가 선택되어도 게임이 종료됨
//게임이 종료되면 폭탄의 위치를 모두 확인할 수 있음
//ShowCurrentState함수를 추가하여 사용자가 확인한 항목들만 보게 함
void ShowCurrentState(char mine_table[][X_COUNT], char check_table[][X_COUNT]) {
    printf("\n ");   //줄바꿈
    for (int y = 0; y < Y_COUNT; y++) {
        printf("\t\t ");    //탭
        for (int x = 0; x < X_COUNT; x++) {
            if (check_table[y][x]) printf("%c ", mine_table[y][x]);
            else printf("@ ");
        }
        printf("\n");   //줄바꿈
    }
    printf("\n");   //줄바꿈
}


//2차원 배열을 선언하여 사용
int m_s(void) {

    // 창크기 조절
    system("mode con: cols=47 lines=30");

    //게임 제목 출력
    printf("\t\t   지뢰찾기\n\n");

    // 지뢰가 설치된 정보를 저장할 변수
    char mine_table[Y_COUNT][X_COUNT];
    //게임 진행을 위해 사용자가 어떤 항목들을 선택했는지 기억하기 위해 check_table 변수 추가
    //사용자가 선택한 위치를 기억할 변수
    char check_table[Y_COUNT][X_COUNT];
    //현재 시간을 기준으로 난수 설정
    srand((unsigned)time(NULL));

    //check_table 값도 지뢰 설치 정보를 구성하는 CreateMineTable 함수에 함께 넘겨 초기화 진행
    //지뢰를 설치할 정보 구성
    //사용자에게 x, y좌표를 입력받아 mine_table에서 지뢰가 선택되었는지 확인하는 형태로 진행되며
    //지뢰가 선택되거나 사용자가 위치 정보를 음수로 입력하면 게임이 종료된다.
    //게임이 종료되면 CreateMineTable 함수를 호출하여 지뢰 설치 정보를 보여줌
    CreateMineTable(mine_table, check_table);

    //선택한 정보를 반영하여 지뢰의 정보 출력
    //@문자로 출력된 것은 아직 확인이 안 된 항목
    ShowCurrentState(mine_table, check_table);

    int x = 0;  //x좌표 변수 선언
    int y = 0;  //y좌표 변수 선언
    int count = 0;  //입력 횟수 변수 선언
    int score = 0;  //점수 변수 선언

    //반복문 사용
    while (1) {
        //하얀색으로 메시지를 출력함
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("\n확인할 위치의 x, y 좌표를 입력하세요.\n");  //printf()로 이미 확인했다는 메시지 출력
        printf("음수를 입력하면 게임이 종료됩니다.\n\n");  //printf()로 음수를 입력하면 게임이 종료된다는 메시지 출력

        printf("x좌표 입력 : ");    //x좌표를 입력하라는 메시지 출력
        scanf_s("%d", &x);  //사용자로부터 x좌표를 정수형으로 입력받음
        //음수가 입력되면 게임 종료
        if (x < 0) break;

        printf("y좌표 입력 : ", "\n\n");    //y좌표를 입력하라는 메시지 출력
        scanf_s("%d", &y);  //사용자로부터 y좌표를 정수형으로 입력받음
        //음수가 입력되면 게임 종료
        if (y < 0) break;

        //위치 정보가 제대로 입력됐는지 확인
        if (x < X_COUNT && y < Y_COUNT) {
            //이미 확인한 위치인지 체크
            if (check_table[y][x] == 0) {
                //사용자가 지뢰를 선택한 경우
                if (mine_table[y][x] == '*') {
                    //지뢰를 선택하면 폭탄 소리가 나옴
                    PlaySound(TEXT("mine.wav"), NULL, SND_ASYNC);
                    while (1) {
                        break;
                    }
                    //빨간색으로 메시지를 출력함
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                    printf("\n\n지뢰를 선택했습니다. Game Over\n\n");    //printf()로 지뢰를 선택했다는 메시지 출력

                    break;   //게임 중단
                }
                else {
                    //선택했음을 설정
                    check_table[y][x] = 1;

                    //창이 초기화 돼도 결과를 보면서 좌표를 입력하기 위해 이 위치에 추가
                    //ShowCurrentState 함수는 현재 상태를 그려주는 함수라서
                    //함수를 지우는 코드가 먼저 수행되고 ShowCurrentState 함수가 수행해야만 기존 화면을 지우고 새 상태가 됨
                    //다음 화면 지우기 전까지 값을 입력할 때 현재 상태를 보며 입력이 가능
                    _getch();
                    system("cls");

                    //선택한 정보를 반영하여 지뢰 정보 출력
                    //@ 문자로 출력된 것은 아직 확인이 안 된 항목
                    //새로 입력된 정보를 반영해서 보여줌
                    ShowCurrentState(mine_table, check_table);
                }
            }
            //이미 확인한 위치를 입력했을 경우
            else {
                //빨간색으로 메시지를 출력함
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("이미 확인한 위치입니다.\n\n");    //printf()로 이미 확인한 위치라는 메시지 출력
            }
        }
        //잘못된 위치를 입력했을 경우
        else {
            //빨간색으로 메시지를 출력함
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("잘못된 위치를 입력했습니다.\n\n");  //printf()로 잘못된 위치를 입력했다는 메시지 출력
        }
        count++;    //입력횟수 증가
        score = score + 10; //한번 입력할 때마다 10점 누적
    }


    //전체 설치 정보 확인
    ShowMineTable(mine_table);
    //하얀색으로 메시지를 출력함
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\n총 입력 횟수 : %d\n", count);  //printf()와 count로 총 입력 횟수 출력
    printf("\n총 점수 : %d\n\n", score); //printf()와 score로 총 점수 출력

    //게임 종료 후 화면이 출력되고 
    //엔터키를 누르면 메뉴 화면으로 넘어감
    // best_m_n와 현재 점수를 비교
    if (score > best_m_s) {
        best_m_s = score;

        //mysql_real_connect() 함수를 위해 초기화
        mysql_init(&conn);

        //한글 깨짐을 방지하기 위함
        mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
        mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

        //mysql_real_connect : host에서 실행중인 MySQL 데이터베이스 엔진에 연결을 설정하려고 시도하는 함수
        //MYSQL DB에 연결한 정보를 connection에 담음
        connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

        //에러가 왜 발생했는지 에러메시지 출력
        if (connection == NULL) {
            fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
            return 1;
        }

        //DB에 MYSQL 명령어 정보를 던져줌
        sprintf(query, "update login set m_s=%d where id='%s' and passwd='%s';", best_m_s, id, passwd);
        //DB에 던져줌
        int query_stat = mysql_query(connection, query);
        //만약에 오류가 발생하면 실패원인 출력
        if (query_stat != 0) {
            fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
            return 1;
        }
        //mysql_store_result() 함수에 의해 할당된 메모리를 해제
        mysql_free_result(sql_result);
    }
    //DB에 MYSQL 명령어 정보를 던져줌
    int query_stat = mysql_query(connection, "select id, m_s from login order by m_s desc");

    //만약에 오류가 발생하면 실패원인 출력
    if (query_stat != 0) {
        fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
        return 1;
    }

    //실행된 DB값을 메모리에 넣음 (= sql 결과 저장)
    sql_result = mysql_store_result(connection);

    //row단위로 아이디와 점수를 가지고오고 NULL이 아닐 때까지 반복
    printf("%15s %10s\n", "아이디", "점수");
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
        printf("%15s %10s\n", sql_row[0], sql_row[1]);  //printf()이용하여 아이디, 점수 출력
    }

    //게임 종료 후 화면이 출력되고 
    //아무 키를 누르면 메뉴 화면으로 넘어감
    _getch();
    menu();

    return 0;
}

//가위바위보

int rsp(void) {
    system("mode con: cols=35 lines=20"); //창 크기 조절


    int  cnt = 0;   //게임의 수를 출력하기 위해 변수 선언
    char ch = 0;    //y,n 를 입력받기 위해 변수 선언
    int  user = 0;  //사용자 변수 선언
    int com = 0;    //컴퓨터 변수 선언
    int  w = 0; //이긴 횟수를 세기 위해 변수 선언
    int d = 0;  //비긴 횟수를 세기 위해 변수 선언
    int l = 0;  //진 횟수를 세기 위해 변수 선언
    char str[][5] = { "가위","바위","보" };  //배열 선언
    int score = 0;  //점수 변수 선언

    srand((unsigned)time(NULL)); // 일정한 랜덤값 방지 

    printf("\t  <<가위바위보>>\n\n");    //printf()로 게임 이름 출력

    printf("시작할까요? (y/n) : ");  //printf()로 시작할 건지 묻는 메시지 출력
    scanf(" %c", &ch); //사용자로부터 ch를 하나의 문자로 입력받음
    // y또는 n을 입력 , ch를 받아야 해서 %c 앞에 스페이스바를 해줘야 함

    // y또는 Y입력일때 
    while (ch == 'y' || ch == 'Y')
    {
        printf("\n===========< %d번째 판 >===========\n", ++cnt); // 게임 수 메시지 출력 
        do
        {
            printf("가위(1), 바위(2), 보(3) : ");    //printf()로 가위, 바위,보를 고를 수 있게 메시지 출력
            scanf("%d", &user); //사용자로부터 user가 입력한 것을 정수형으로 입력받은
        } while (user < 1 || user > 3); // 잘못된 입력이면 do부터 다시 실행 
        com = rand() % 3 + 1; // 1~3사이의 난수를 com에 저장 
        // 어떤 걸 냈는지 출력 
        printf("(컴퓨터)==> %s   (사용자)==> %s\n", str[com - 1], str[user - 1]);
        if (user == com) // 비겼다면 
        {
            //초록색으로 메시지를 출력함
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("\n비겼습니다!\n"); //printf()로 비겼다는 메시지 출력
            d++; // d변수 1증가 
        }
        else if ((user == 1 && com == 3) || (user == 2 && com == 1) || (user == 3 && com == 2)) // 이겼다면 
        {
            //파란색으로 메시지를 출력함
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("\n이겼습니다!\n");   //printf()로 이겼다는 메시지 출력
            w++; // w변수 1증가 
            score = score + 10; //10점 증가

        }
        else { // 졌다면 
             //빨간색으로 메시지를 출력함
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("\n졌습니다!\n");
            l++; // l변수 1증가 
            score = score - 5;  //5점 감소

        }
        //하얀색으로 메시지를 출력함
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("==================================\n");
        printf("계속할까요? (y/n) : ");    //printf()로 계속하는지 물어보는 메시지 출력
        scanf("%*c%c", &ch); //입력받을 때 엔터키를 지우고 하나의 문자를 입력받음
    }
    //하얀색으로 전적 출력 
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\n지금까지의 전적: %d승 %d무 %d패\n", w, d, l);   //printf()와 w, d, l로 지금까지의 전적을 정수 형태로 출력
    printf("\n총 점수 : %d\n", score); //printf()로 score을 이용하여 총 점수를 정수 형태로 출력

    // best_rsp와 현재 점수를 비교
    if (score > best_rsp) {
        best_rsp = score;

        //mysql_real_connect() 함수를 위해 초기화
        mysql_init(&conn);

        //한글 깨짐을 방지하기 위함
        mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
        mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

        //mysql_real_connect : host에서 실행중인 MySQL 데이터베이스 엔진에 연결을 설정하려고 시도하는 함수
        //MYSQL DB에 연결한 정보를 connection에 담음
        connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

        //에러가 왜 발생했는지 에러메시지 출력
        if (connection == NULL) {
            fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
            return 1;
        }

        //DB에 MYSQL 명령어 정보를 던져줌
        sprintf(query, "update login set rsp=%d where id='%s' and passwd='%s';", best_rsp, id, passwd);
        //DB에 던져줌
        int query_stat = mysql_query(connection, query);
        //만약에 오류가 발생하면 실패원인 출력
        if (query_stat != 0) {
            fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
            return 1;
        }
        //mysql_store_result() 함수에 의해 할당된 메모리를 해제
        mysql_free_result(sql_result);
    }
    //DB에 MYSQL 명령어 정보를 던져줌
    int query_stat = mysql_query(connection, "select id, rsp from login order by rsp desc");

    //만약에 오류가 발생하면 실패원인 출력
    if (query_stat != 0) {
        fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
        return 1;
    }
    //실행된 DB값을 메모리에 넣음 (= sql 결과 저장)
    sql_result = mysql_store_result(connection);

    //row단위로 아이디와 점수를 가지고오고 NULL이 아닐 때까지 반복
    printf("%15s %10s\n", "아이디", "점수");
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
        printf("%15s %10s\n", sql_row[0], sql_row[1]);  //printf()이용하여 아이디, 점수 출력
    }

    //게임 종료 후 화면이 출력되고 
    //아무 키를 누르면 메뉴 화면으로 넘어감
    _getch();
    menu();

    return 0;
}


//숫자맞추기 게임
int m_n(void) {

    // 창크기 조절
    system("mode con: cols=28 lines=20");

    srand(time(NULL));  //램덤 수 초기화
    int answer = rand() % 100;  //(100 이하의 랜덤 값)
    int userinput = 0;  //사용자가 입력하는 값
    int sum = 0; //합계
    int score = 100;   //100점을 주고 시작하기 위함

    printf("     <<숫자맞히기 게임>>\n\n");    //printf()로 게임 이름 출력  

    //무한루프
    while (1)
    {
        //하얀색으로 메시지를 출력함
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("숫자를 맞혀보세요! : ");    //printf()로 숫자를 맞혀라는 메시지 출력
        scanf("%d", &userinput);    //사용자로부터 userinput을 정수 형태로 입력받음
        sum++;  // sum변수 1증가 
        score = score - 10; //기본점수에서 10점 감점

        //하얀색으로 메시지를 출력함
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        if (answer == userinput)    //입력한 값과 답이 맞다면
        {
            //파란색으로 메시지를 출력함
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);

            printf("\n숫자를 맞혔습니다.\n");   //printf()와 숫자를 맞혔다는 메시지 출력
            printf("시도횟수 : %d\n\n", sum);   //printf()와 sum으로 시도횟수 출력
            //하얀색으로 메시지를 출력함
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("\n총 점수 : %d\n", score);  //printf()와 score로 총 점수 출력
            break;  //정답이면 while을 빠져나감
        }
        else if (answer > userinput)    //입력한 값보다 답이 더 크면
        {
            //빨간색으로 메시지를 출력함
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("더 높은 숫자를 입력해주세요.\n\n");  //printf()로 더 높은 숫자를 입력하라는 메시지 출력
        }
        else {  //입력한 값보다 답이 더 작으면
             //빨간색으로 메시지를 출력함
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("더 낮은 숫자를 입력해주세요.\n\n"); //printf()로 더 낮은 숫자를 입력하라는 메시지 출력
        }
    }


    // best_m_n와 현재 점수를 비교
    if (score > best_m_n) {
        best_m_n = score;

        //mysql_real_connect() 함수를 위해 초기화
        mysql_init(&conn);

        //한글 깨짐을 방지하기 위함
        mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
        mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

        //mysql_real_connect : host에서 실행중인 MySQL 데이터베이스 엔진에 연결을 설정하려고 시도하는 함수
        //MYSQL DB에 연결한 정보를 connection에 담음
        connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

        //에러가 왜 발생했는지 에러메시지 출력
        if (connection == NULL) {
            fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
            return 1;
        }

        //DB에 MYSQL 명령어 정보를 던져줌
        sprintf(query, "update login set m_n=%d where id='%s' and passwd='%s';", best_m_n, id, passwd);
        //DB에 던져줌
        int query_stat = mysql_query(connection, query);
        //만약에 오류가 발생하면 실패원인 출력
        if (query_stat != 0) {
            fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
            return 1;
        }
        //mysql_store_result() 함수에 의해 할당된 메모리를 해제
        mysql_free_result(sql_result);
    }
    //DB에 MYSQL 명령어 정보를 던져줌
    int query_stat = mysql_query(connection, "select id, m_n from login order by m_n desc");

    //만약에 오류가 발생하면 실패원인 출력
    if (query_stat != 0) {
        fprintf(stderr, "실패원인 : %s\n", mysql_error(&conn));
        return 1;
    }
    //실행된 DB값을 메모리에 넣음 (= sql 결과 저장)
    sql_result = mysql_store_result(connection);

    //row단위로 아이디와 점수를 가지고오고 NULL이 아닐 때까지 반복
    printf("%15s %10s\n", "아이디", "점수");
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
        printf("%15s %10s\n", sql_row[0], sql_row[1]);  //printf()이용하여 아이디, 점수 출력
    }

    //게임 종료 후 화면이 출력되고 
    //아무 키를 누르면 메뉴 화면으로 넘어감
    _getch();
    menu();

    return 0;
}