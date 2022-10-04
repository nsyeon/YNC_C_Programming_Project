#define _CRT_SECURE_NO_WARNINGS  //���� ����
#include <stdio.h>   //printf, scanf_s �Լ��� ����ϱ� ����
#include <stdlib.h>   //srand, rand �Լ��� ����ϱ� ����
//������ ��ġ�� rand�Լ��� ������ ������ ����� ���̱⿡ 
//������ ���ذ��� ���� �ð����� ���
#include <time.h>   //time �Լ��� ����ϱ� ����
#include <memory.h>   //memset �Լ��� ����ϱ� ����
#include <windows.h> //color�� ����ϱ� ����
#include <mmsystem.h>   //sound�� ����ϱ� ����
#pragma comment(lib, "winmm.lib")//sound�� ����ϱ� ����
#include <mysql.h>  //DB������ ����
#pragma comment(lib, "libmysql.lib") //�߰����Ӽ�, ��Ŀ-�Է�-�߰����Ӽ� ==> libmysql.lib, ��Ÿ�� �� �� �ֱ⿡ �Է�

#define DB_HOST "127.0.0.1" //DB ȣ��Ʈ �̸�
#define DB_USER ""  //DB ���� �̸�
#define DB_PASS ""    //DB ��й�ȣ
#define DB_NAME "login" //DB �̸�
#define CHOP(x) x[strlen(x) - 1] =' '   //Ű����� �Է��� �� �� ���͸� �ľ��ϱ� ������ ������ ��ġ�� �Ǹ�, 
                                        //����Ű�� �������� �ٲ��ִ� ���ڿ��� ���� �ǹ�

MYSQL* connection = NULL, conn; //MYSQL�̶�� ����ü�� connection ������ ���� ����, conn�̶�� ����ü ���� ����
MYSQL_RES* sql_result;  //MYSQL_RES��� ����ü�� ����Ͽ�  sql_result������ �̿��Ͽ� ������� �޾ƿ��� ����
MYSQL_ROW sql_row;  //MYSQL_ROW��� ����ü�� ����Ͽ�  sql_row ������ �̿��Ͽ� ROW������ ���ڿ��� �о���� ����

char id[20] = { 0, };   //���̵� �Է¹��� �� �ֵ��� �迭 ����
char passwd[20] = { 0, };   //��й�ȣ�� �Է¹��� �� �ֵ��� �迭 ����
char query[255] = { 0, };   //������� ���۸� �ϳ� ����

//��ŷ�� ���� ����
int best_rsp = 0;
int best_m_n = 0;
int best_m_s = 0;

void main(void) {
    start();
}

//�α��� �� ȸ������
int start(void) {
    system("mode con: cols=32 lines=15");   //âũ�� ����

    int user_input = 0; //����ڰ� �Է��ϴ� ��

    system("cls");  //ȭ�� ����
    //�Ͼ������ �޽����� �����
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("������������������������������������������������������������\n");
    printf("��                            ��\n");
    printf("��        �ܼ�â ����         ��\n");
    printf("��                            ��\n");
    printf("��                            ��\n");
    printf("��     1.�α���/ȸ������      ��\n");
    printf("��     2.����                 ��\n");
    printf("��                            ��\n");
    printf("������������������������������������������������������������\n\n");
    printf(" ���� :  ");  //printf()�� �α���/ȸ������ �׸��� ���Ḧ ������ �� �ִ� �޽��� ���
    scanf("%d", &user_input);   //����ڷκ��� user_input�� ���������� �Է¹���

    switch (user_input) {     //����ڰ�
    case 1: //1���� �Է��ϸ�
        login();  //�α��� �� ȸ������ ����
        break;
    case 2: //2���� �Է��ϸ�
        exit(0);    //����
    }
    //�޴� ȭ������ �Ѿ
    menu();

    return 0;
}


//�޴�
int menu(void) {

    system("mode con: cols=32 lines=15");   //â ũ�� ����

    int user_input = 0; //����ڰ� �Է��ϴ� ��

    system("cls");  //ȭ�� ������
    //�Ͼ������ �޽����� �����
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("������������������������������������������������������������\n");
    printf("��                            ��\n");
    printf("��        �ܼ�â ����         ��\n");
    printf("��                            ��\n");
    printf("��     1.����������           ��\n");
    printf("��     2.���� ������          ��\n");
    printf("��     3.����ã��             ��\n");
    printf("��     4.����                 ��\n");
    printf("��                            ��\n");
    printf("������������������������������������������������������������\n\n");
    printf(" ���� :  ");  //printf()�� ������ ������ �� �ִ� �޽��� ���
    scanf("%d", &user_input);   //����ڷκ��� user_input�� ���������� �Է¹���

    switch (user_input) {     //����ڰ�
    case 1: //1���� �Է��ϸ�
        rsp();  //���������� ����
        break;
    case 2: //2���� �Է��ϸ�
        m_n();  //���� ���߱� ����
        break;
    case 3: //3���� �Է��ϸ�
        m_s();  //����ã�� ����
        break;
    case 4: //4���� �Է��ϸ�
        exit(0);    //����

    }
    return 0;
}



//�α��� �� ȸ������
int login(void) {

    system("mode con: cols=32 lines=17"); //â ũ�� ����

    int query_stat = 0; //Ű����� �Է��� �� �� ���͸� �ľ��ϱ� ������ ������ ��ġ�� �Ǹ�, 
                        //����Ű�� �������� �ٲ��ִ� ���ڿ��� ���� �ǹ�
    char ans = 0;   //ans ���� ����
    int ls = 0;     //�α��� �������θ� ���� ls ���� ����

    //mysql_real_connect() �Լ��� ���� �ʱ�ȭ
    mysql_init(&conn);

    //�ѱ� ������ �����ϱ� ����
    mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
    mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

    //mysql_real_connect : host���� �������� MySQL �����ͺ��̽� ������ ������ �����Ϸ��� �õ��ϴ� �Լ�
    //MYSQL DB�� ������ ������ connection�� ����
    connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

    //������ �� �߻��ߴ��� �����޽��� ���
    if (connection == NULL) {
        fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
        return 1;
    }

    //DB�� MYSQL ��ɾ� ������ ������
    query_stat = mysql_query(connection, "select * from login");
    if (query_stat != 0) {
        fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
        return 1;
    }

    //�޸𸮻� ������ Ȯ���Ǿ� DB�� �ִ� ������ �����
    sql_result = mysql_store_result(connection);

    //mysql_fetch_row()�� �̿��Ͽ� ��ü ������ �� row�� ������
    printf("%10s %10s\n", "���̵�", "�н�����");
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
        printf("%10s %10s\n", sql_row[0], sql_row[1]);
    }
    //mysql_store_result() �Լ��� ���� �Ҵ�� �޸𸮸� �����Ѵ�
    mysql_free_result(sql_result);

    //���ѷ���
    while (1) {

        printf("\nȸ�������� �Ͻðڽ��ϱ�? :");    //printf()�� ȸ�������� �ϰڴ��� �޽��� ���
        scanf(" %c", &ans); //����ڷκ��� ans�� �ϳ��� ���ڷ� �Է¹���
        scanf("%*c");   //����Ű ����

        //ȸ�������� �Ѵٰ� ���� ��
        if (ans == 'y' || ans == 'Y') {
            //���ѷ���
            while (1) {
                printf("\n���̵� : "); //printf()�� �̿��Ͽ� ���̵� �Է�
                fgets(id, 20, stdin);   //�� ���� �ʹ� �� ������ �迭�� ũ�⺸�� ũ�ٸ� 
                //�� ���� �� ���� �ʰ� �ߴ��� �� �ְ� ǥ�� �Է�(stdin) ���� �Է��� �ޱ����� stdin �Է�
                CHOP(id);    //����Ű�� ������ ����Ű�� �������� �ٲ���

                printf("�н����� : ");  //printf()�� �̿��Ͽ� ��й�ȣ �Է�
                fgets(passwd, 20, stdin);   //�� ���� �ʹ� �� ������ �迭�� ũ�⺸�� ũ�ٸ� 
                //�� ���� �� ���� �ʰ� �ߴ��� �� �ְ� ǥ�� �Է�(stdin) ���� �Է��� �ޱ����� stdin �Է�
                CHOP(passwd);    //����Ű�� ������ ����Ű�� �������� �ٲ���

                //�Է��� id�� DB�� ������ �ִ��� �˻�
                sprintf(query, "select * from login where id = '%s'", id);

                query_stat = mysql_query(connection, query); //DB�� ������
                if (query_stat != 0) {  //���࿡ ������ �߻��ϸ� ���п��� ���
                    fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
                    return 1;
                }

                sql_result = mysql_store_result(connection); //����� ������� �޸𸮿� �ø��� �ּҰ��� ����
                if (sql_row = mysql_fetch_row(sql_result) == NULL) { //���� row�� NULL���̸� ��, id�� ���� ���� ������
                    sprintf(query, "insert into login(id, passwd) values" "('%s', '%s')", id, passwd);  //�ٽ� �ѹ� query�� insert���� �־���

                    //DB�� ������
                    query_stat = mysql_query(connection, query);

                    //insert���� ���� query�迭�� DB�� �־��� ����, �� ������ Ȯ���ϴ� ���� üũ
                    if (query_stat != 0) {
                        fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
                        return 1;
                    }

                    printf("\nȸ������ �Ϸ�");   //ȸ������ �Ϸ� Ȯ��

                    _getch();    //�ƹ� Ű�� �Է��ϱ� ������ ȭ�� ����
                    break;  //��������
                }
                //NULL���� �ƴ� ���
                else {
                    printf("�ߺ��� ���̵� �ֽ��ϴ�.\n"); //�ߺ��� ���̵� ������ ���� �ö󰡼� �ٽ� ���̵�� ��й�ȣ �Է�

                    _getch();    //�ƹ� Ű�� �Է��ϱ� ������ ȭ�� ����
                    system("cls");  //ȭ�� ����

                    continue; //���
                }
            }
        }

        //ȸ�������� ���� �ʰ� �α����� �Ѵٰ� ���� ��
        else if (ans == 'n' || ans == 'N') {
            //���ѷ���
            while (1) {
                printf("\n���̵� : "); //printf()�� �̿��Ͽ� ���̵� �Է�
                fgets(id, 20, stdin);   //�� ���� �ʹ� �� ������ �迭�� ũ�⺸�� ũ�ٸ� 
                //�� ���� �� ���� �ʰ� �ߴ��� �� �ְ� ǥ�� �Է�(stdin) ���� �Է��� �ޱ����� stdin �Է�
                CHOP(id);   //����Ű�� ������ ����Ű�� �������� �ٲ���

                printf("�н����� : ");  //printf()�� �̿��Ͽ� ��й�ȣ �Է�
                fgets(passwd, 20, stdin);   //ǥ�� �Է�(stdin) ���� �Է��� �ޱ����� stdin �Է�
                CHOP(passwd);    //����Ű�� ������ ����Ű�� �������� �ٲ���

                //��ɹ� ��ü�� �����Ű�����ؼ� query�迭�� �־��ְ� �� query�迭 ��θ� DB�� �־���
                sprintf(query, "select * from login where id = '%s' and passwd = '%s'", id, passwd);

                //����� ��Ʃ������� DB���â�� �������� �ʾƼ� Ȥ�ó� ��Ÿ ������ ���� ������ �߻��ߴµ�
                //�׳� �𸣰� �Ѿ �� ������ �׻� üũ�� �ؾ� �ϱ� ������ query_stat ���� ���
                query_stat = mysql_query(connection, query);  //mysql_query�� db�� ��(�������� �� ���� ��ɹ�)�� �Է����ִ� �Լ�
                //connection�� query ��, ��ɹ��� �ְڴ� ( = �����Ǿ��ִ� DB������ select���� �����ض�)
                //0�� �ƴϸ� ��, ������ �߻��ߴٸ� fprintf�� �����ϰ� return���� ����
                if (query_stat != 0) {
                    fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
                    //stderr�� ������ ǥ�� �����̸� mysql_error(&conn)�� �̿��Ͽ� ������ ���� ���ڿ��� ��Ÿ��
                    return 1;
                }

                sql_result = mysql_store_result(connection); //����� DB���� �޸𸮿� ���� (= sql ��� ����)


                //���� �����ּҸ� ��� ��������� �ϳ��� �྿ ���� �� ����
                while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
                    printf("%10s %10s\n", sql_row[0], sql_row[1]);  //�� ���� ù��° ��(���̵�)�� sql_row[0], �ι�° ��(��й�ȣ)�� sql_row[1]
                    printf("\n�α��� ����\n");   //printf()�� �α��� ���� �޽��� ���
                    best_rsp = atoi(sql_row[2]);
                    best_m_n = atoi(sql_row[3]);
                    best_m_s = atoi(sql_row[4]);
                    ls++;
                    _getch();   //���� �Է��ϱ� ������ ȭ�� ����
                }
                if (ls > 0) {   //�α��� ���� ��
                    break;  //��������
                }
                else {  //�α��� ���� ���� ��
                    printf("\n�α��� ����\n");   //printf()�� �α��� ���� �޽��� ���

                    _getch();    //�ƹ� Ű�� �Է��ϱ� ������ ȭ�� ����
                    system("cls");  //ȭ�� ����

                    continue;   //���
                }
            }
        }
        //�߸� �Է��� ���
        else {
            printf("�߸� �Է��ϼ̽��ϴ�\n"); //printf()�� �߸� �Է��ߴٴ� �޽��� ���
            _getch();    //�ƹ� Ű�� �Է��ϱ� ������ ȭ�� ����
            system("cls");  //ȭ�� ����
        }
        //ȸ������ Ȥ�� �α����� �ϸ� �޴� ȭ������ �Ѿ
        menu();
    }
    mysql_close(connection);    //DB���� ����
    return 0;
}


//����ã��

//����ã�� ���� ũ�⸦ ���� 6, ���� 6���� ���߰�
//������ ���� 7���� ����
#define MINE_COUNT 7
#define X_COUNT 6
#define Y_COUNT 6

//������ �����ϱ� ���� ���ڰ� ��ġ�� ���� ������ �����ؾ� �ϱ⿡ CreateMineTable �Լ��� ����
void CreateMineTable(char mine_table[][X_COUNT], char check_table[][X_COUNT]) {
    int x, y, mine_count;   //�Լ� ����
    //CreateMineTable �Լ��� �ʱ�ȭ ��ƾ
    memset(mine_table, 0, Y_COUNT * X_COUNT);   //mine ������ ������ �޸𸮿� ��� 0�� ä��
    memset(check_table, 0, Y_COUNT * X_COUNT);   //������� ���� ������ ������ �޸𸮿� ��� 0�� ä��

    //MINE_COUNT����ŭ �ݺ��ϸ鼭 rand�Լ��� ���� ��ġ
    //���ڴ� *�� ����
    //�ߺ��� ��ġ�� ��ź�� ��ġ���� �ʵ��� ������ ��ġ�� üũ�ϸ鼭 �����ؾ� ��
    for (mine_count = 0; mine_count < MINE_COUNT; mine_count++) {     //���ڸ� MINE_COUNT����ŭ ������
       //������ ����Ͽ� ���ڰ� ��ġ�� �� ����
        x = rand() % X_COUNT;
        y = rand() % Y_COUNT;
        //���ڰ� ��ġ�� ���� �ƴ϶�� ��ź�� ��ġ�ϰ�
        //�̹� ��ġ�� ���̶�� ������ ���� �ٿ��� �ٽ� �õ��ϰ� ��
        if (mine_table[y][x] == 0) mine_table[y][x] = '*';
        else mine_count--;
    }

    //�Է��� ��ġ �ֺ��� �� ���� ���ڰ� �ִ��� üũ�Ͽ� ���� ������ ���ڷ� ����
    //��, ���ڰ� �ƴ� ĭ�� 0���� ������ �ִµ� 0���� ���� ĭ�� ������
    //�� �׸��� ���ΰ� �ִ� 8ĭ�� ���ڰ� �ִ��� üũ�Ͽ� 
    //���� 5���� �ִٸ� '5'���ڸ� ���
    //�̶� ���� �� �ִ� �ִ� ������ ���� 8������, ���� ������ ���� 7�̱⿡ 7�� �ִ�
    for (y = 0; y < Y_COUNT; y++) {
        for (x = 0; x < X_COUNT; x++) {
            //���ڰ� �ƴ϶�� ������ ��ġ�� �ִ� ���ڰ� �� �� �ִ��� üũ
            if (mine_table[y][x] == 0) {
                //������ �� �ʱ�ȭ
                mine_count = 0;
                //���� ��ġ (C)�� ���ΰ� �ִ� 8ĭ�� ��ź�� �� �� �ִ��� üũ
                // 1, 2, 3
                // 4, C, 6
                // 7, 8, 9

                if ((y - 1) >= 0) {   //������ ������ �ʰ� üũ

                   //1, 2, 3�� ĭ üũ
                    if ((x - 1) >= 0 && mine_table[y - 1][x - 1] == '*') mine_count++;
                    if (mine_table[y - 1][x] == '*') mine_count++;
                    if ((x + 1) < X_COUNT && mine_table[y - 1][x + 1] == '*') mine_count++;
                }
                //4, 5�� ĭ üũ
                if ((x - 1) >= 0 && mine_table[y][x - 1] == '*') mine_count++;
                if ((x + 1) < X_COUNT && mine_table[y][x + 1] == '*') mine_count++;

                //6, 7, 8�� ĭ üũ
                if ((y + 1) < Y_COUNT) {   //������ ����� �ʰ� üũ
                    if ((x - 1) >= 0 && mine_table[y + 1][x - 1] == '*') mine_count++;
                    if (mine_table[y + 1][x] == '*') mine_count++;
                    if ((x + 1) < X_COUNT && mine_table[y + 1][x + 1] == '*') mine_count++;
                }
                //���� ������ ���� �������� ���ڷ� �����Ͽ� ���� ��ġ�� ����
                //5��� '5'�� ����
                mine_table[y][x] = '0' + mine_count;
            }
        }
    }
}

//������ ���� ��Ȯ�ϰ� �����Ǿ����� Ȯ���ϱ� ���� 
//mine_table���� ����ϴ� ShowMineTable �Լ� ����
//ShowMineTable�Լ��� ��ź ��ġ ������ ��� �����ֱ⿡ ������ ������ �� ���
void ShowMineTable(char mine_table[][X_COUNT]) {
    printf("\n ");   //�ٹٲ�
    for (int y = 0; y < Y_COUNT; y++) {
        printf("\t\t ");    //��
        //�� ���� ���� ���
        for (int x = 0; x < X_COUNT; x++) printf("%c", mine_table[y][x]);
        printf("\n");   //�ٹٲ�
    }
    printf("\n");   //�ٹٲ�
}

//'@'�� ǥ�õ� ĭ�� ���� ����ڰ� �Է����� ���� ĭ�̰�
//���ڷ� ǥ�õ� ĭ�� �Է��� �� ĭ
//Ȯ���� ��ġ�� ������ �Է��ϸ� ������ ����ǰ�
//���ڰ� ���õǾ ������ �����
//������ ����Ǹ� ��ź�� ��ġ�� ��� Ȯ���� �� ����
//ShowCurrentState�Լ��� �߰��Ͽ� ����ڰ� Ȯ���� �׸�鸸 ���� ��
void ShowCurrentState(char mine_table[][X_COUNT], char check_table[][X_COUNT]) {
    printf("\n ");   //�ٹٲ�
    for (int y = 0; y < Y_COUNT; y++) {
        printf("\t\t ");    //��
        for (int x = 0; x < X_COUNT; x++) {
            if (check_table[y][x]) printf("%c ", mine_table[y][x]);
            else printf("@ ");
        }
        printf("\n");   //�ٹٲ�
    }
    printf("\n");   //�ٹٲ�
}


//2���� �迭�� �����Ͽ� ���
int m_s(void) {

    // âũ�� ����
    system("mode con: cols=47 lines=30");

    //���� ���� ���
    printf("\t\t   ����ã��\n\n");

    // ���ڰ� ��ġ�� ������ ������ ����
    char mine_table[Y_COUNT][X_COUNT];
    //���� ������ ���� ����ڰ� � �׸���� �����ߴ��� ����ϱ� ���� check_table ���� �߰�
    //����ڰ� ������ ��ġ�� ����� ����
    char check_table[Y_COUNT][X_COUNT];
    //���� �ð��� �������� ���� ����
    srand((unsigned)time(NULL));

    //check_table ���� ���� ��ġ ������ �����ϴ� CreateMineTable �Լ��� �Բ� �Ѱ� �ʱ�ȭ ����
    //���ڸ� ��ġ�� ���� ����
    //����ڿ��� x, y��ǥ�� �Է¹޾� mine_table���� ���ڰ� ���õǾ����� Ȯ���ϴ� ���·� ����Ǹ�
    //���ڰ� ���õǰų� ����ڰ� ��ġ ������ ������ �Է��ϸ� ������ ����ȴ�.
    //������ ����Ǹ� CreateMineTable �Լ��� ȣ���Ͽ� ���� ��ġ ������ ������
    CreateMineTable(mine_table, check_table);

    //������ ������ �ݿ��Ͽ� ������ ���� ���
    //@���ڷ� ��µ� ���� ���� Ȯ���� �� �� �׸�
    ShowCurrentState(mine_table, check_table);

    int x = 0;  //x��ǥ ���� ����
    int y = 0;  //y��ǥ ���� ����
    int count = 0;  //�Է� Ƚ�� ���� ����
    int score = 0;  //���� ���� ����

    //�ݺ��� ���
    while (1) {
        //�Ͼ������ �޽����� �����
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("\nȮ���� ��ġ�� x, y ��ǥ�� �Է��ϼ���.\n");  //printf()�� �̹� Ȯ���ߴٴ� �޽��� ���
        printf("������ �Է��ϸ� ������ ����˴ϴ�.\n\n");  //printf()�� ������ �Է��ϸ� ������ ����ȴٴ� �޽��� ���

        printf("x��ǥ �Է� : ");    //x��ǥ�� �Է��϶�� �޽��� ���
        scanf_s("%d", &x);  //����ڷκ��� x��ǥ�� ���������� �Է¹���
        //������ �ԷµǸ� ���� ����
        if (x < 0) break;

        printf("y��ǥ �Է� : ", "\n\n");    //y��ǥ�� �Է��϶�� �޽��� ���
        scanf_s("%d", &y);  //����ڷκ��� y��ǥ�� ���������� �Է¹���
        //������ �ԷµǸ� ���� ����
        if (y < 0) break;

        //��ġ ������ ����� �Էµƴ��� Ȯ��
        if (x < X_COUNT && y < Y_COUNT) {
            //�̹� Ȯ���� ��ġ���� üũ
            if (check_table[y][x] == 0) {
                //����ڰ� ���ڸ� ������ ���
                if (mine_table[y][x] == '*') {
                    //���ڸ� �����ϸ� ��ź �Ҹ��� ����
                    PlaySound(TEXT("mine.wav"), NULL, SND_ASYNC);
                    while (1) {
                        break;
                    }
                    //���������� �޽����� �����
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                    printf("\n\n���ڸ� �����߽��ϴ�. Game Over\n\n");    //printf()�� ���ڸ� �����ߴٴ� �޽��� ���

                    break;   //���� �ߴ�
                }
                else {
                    //���������� ����
                    check_table[y][x] = 1;

                    //â�� �ʱ�ȭ �ŵ� ����� ���鼭 ��ǥ�� �Է��ϱ� ���� �� ��ġ�� �߰�
                    //ShowCurrentState �Լ��� ���� ���¸� �׷��ִ� �Լ���
                    //�Լ��� ����� �ڵ尡 ���� ����ǰ� ShowCurrentState �Լ��� �����ؾ߸� ���� ȭ���� ����� �� ���°� ��
                    //���� ȭ�� ����� ������ ���� �Է��� �� ���� ���¸� ���� �Է��� ����
                    _getch();
                    system("cls");

                    //������ ������ �ݿ��Ͽ� ���� ���� ���
                    //@ ���ڷ� ��µ� ���� ���� Ȯ���� �� �� �׸�
                    //���� �Էµ� ������ �ݿ��ؼ� ������
                    ShowCurrentState(mine_table, check_table);
                }
            }
            //�̹� Ȯ���� ��ġ�� �Է����� ���
            else {
                //���������� �޽����� �����
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("�̹� Ȯ���� ��ġ�Դϴ�.\n\n");    //printf()�� �̹� Ȯ���� ��ġ��� �޽��� ���
            }
        }
        //�߸��� ��ġ�� �Է����� ���
        else {
            //���������� �޽����� �����
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("�߸��� ��ġ�� �Է��߽��ϴ�.\n\n");  //printf()�� �߸��� ��ġ�� �Է��ߴٴ� �޽��� ���
        }
        count++;    //�Է�Ƚ�� ����
        score = score + 10; //�ѹ� �Է��� ������ 10�� ����
    }


    //��ü ��ġ ���� Ȯ��
    ShowMineTable(mine_table);
    //�Ͼ������ �޽����� �����
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\n�� �Է� Ƚ�� : %d\n", count);  //printf()�� count�� �� �Է� Ƚ�� ���
    printf("\n�� ���� : %d\n\n", score); //printf()�� score�� �� ���� ���

    //���� ���� �� ȭ���� ��µǰ� 
    //����Ű�� ������ �޴� ȭ������ �Ѿ
    // best_m_n�� ���� ������ ��
    if (score > best_m_s) {
        best_m_s = score;

        //mysql_real_connect() �Լ��� ���� �ʱ�ȭ
        mysql_init(&conn);

        //�ѱ� ������ �����ϱ� ����
        mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
        mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

        //mysql_real_connect : host���� �������� MySQL �����ͺ��̽� ������ ������ �����Ϸ��� �õ��ϴ� �Լ�
        //MYSQL DB�� ������ ������ connection�� ����
        connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

        //������ �� �߻��ߴ��� �����޽��� ���
        if (connection == NULL) {
            fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
            return 1;
        }

        //DB�� MYSQL ��ɾ� ������ ������
        sprintf(query, "update login set m_s=%d where id='%s' and passwd='%s';", best_m_s, id, passwd);
        //DB�� ������
        int query_stat = mysql_query(connection, query);
        //���࿡ ������ �߻��ϸ� ���п��� ���
        if (query_stat != 0) {
            fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
            return 1;
        }
        //mysql_store_result() �Լ��� ���� �Ҵ�� �޸𸮸� ����
        mysql_free_result(sql_result);
    }
    //DB�� MYSQL ��ɾ� ������ ������
    int query_stat = mysql_query(connection, "select id, m_s from login order by m_s desc");

    //���࿡ ������ �߻��ϸ� ���п��� ���
    if (query_stat != 0) {
        fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
        return 1;
    }

    //����� DB���� �޸𸮿� ���� (= sql ��� ����)
    sql_result = mysql_store_result(connection);

    //row������ ���̵�� ������ ��������� NULL�� �ƴ� ������ �ݺ�
    printf("%15s %10s\n", "���̵�", "����");
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
        printf("%15s %10s\n", sql_row[0], sql_row[1]);  //printf()�̿��Ͽ� ���̵�, ���� ���
    }

    //���� ���� �� ȭ���� ��µǰ� 
    //�ƹ� Ű�� ������ �޴� ȭ������ �Ѿ
    _getch();
    menu();

    return 0;
}

//����������

int rsp(void) {
    system("mode con: cols=35 lines=20"); //â ũ�� ����


    int  cnt = 0;   //������ ���� ����ϱ� ���� ���� ����
    char ch = 0;    //y,n �� �Է¹ޱ� ���� ���� ����
    int  user = 0;  //����� ���� ����
    int com = 0;    //��ǻ�� ���� ����
    int  w = 0; //�̱� Ƚ���� ���� ���� ���� ����
    int d = 0;  //��� Ƚ���� ���� ���� ���� ����
    int l = 0;  //�� Ƚ���� ���� ���� ���� ����
    char str[][5] = { "����","����","��" };  //�迭 ����
    int score = 0;  //���� ���� ����

    srand((unsigned)time(NULL)); // ������ ������ ���� 

    printf("\t  <<����������>>\n\n");    //printf()�� ���� �̸� ���

    printf("�����ұ��? (y/n) : ");  //printf()�� ������ ���� ���� �޽��� ���
    scanf(" %c", &ch); //����ڷκ��� ch�� �ϳ��� ���ڷ� �Է¹���
    // y�Ǵ� n�� �Է� , ch�� �޾ƾ� �ؼ� %c �տ� �����̽��ٸ� ����� ��

    // y�Ǵ� Y�Է��϶� 
    while (ch == 'y' || ch == 'Y')
    {
        printf("\n===========< %d��° �� >===========\n", ++cnt); // ���� �� �޽��� ��� 
        do
        {
            printf("����(1), ����(2), ��(3) : ");    //printf()�� ����, ����,���� �� �� �ְ� �޽��� ���
            scanf("%d", &user); //����ڷκ��� user�� �Է��� ���� ���������� �Է¹���
        } while (user < 1 || user > 3); // �߸��� �Է��̸� do���� �ٽ� ���� 
        com = rand() % 3 + 1; // 1~3������ ������ com�� ���� 
        // � �� �´��� ��� 
        printf("(��ǻ��)==> %s   (�����)==> %s\n", str[com - 1], str[user - 1]);
        if (user == com) // ���ٸ� 
        {
            //�ʷϻ����� �޽����� �����
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("\n�����ϴ�!\n"); //printf()�� ���ٴ� �޽��� ���
            d++; // d���� 1���� 
        }
        else if ((user == 1 && com == 3) || (user == 2 && com == 1) || (user == 3 && com == 2)) // �̰�ٸ� 
        {
            //�Ķ������� �޽����� �����
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("\n�̰���ϴ�!\n");   //printf()�� �̰�ٴ� �޽��� ���
            w++; // w���� 1���� 
            score = score + 10; //10�� ����

        }
        else { // ���ٸ� 
             //���������� �޽����� �����
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("\n�����ϴ�!\n");
            l++; // l���� 1���� 
            score = score - 5;  //5�� ����

        }
        //�Ͼ������ �޽����� �����
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("==================================\n");
        printf("����ұ��? (y/n) : ");    //printf()�� ����ϴ��� ����� �޽��� ���
        scanf("%*c%c", &ch); //�Է¹��� �� ����Ű�� ����� �ϳ��� ���ڸ� �Է¹���
    }
    //�Ͼ������ ���� ��� 
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\n���ݱ����� ����: %d�� %d�� %d��\n", w, d, l);   //printf()�� w, d, l�� ���ݱ����� ������ ���� ���·� ���
    printf("\n�� ���� : %d\n", score); //printf()�� score�� �̿��Ͽ� �� ������ ���� ���·� ���

    // best_rsp�� ���� ������ ��
    if (score > best_rsp) {
        best_rsp = score;

        //mysql_real_connect() �Լ��� ���� �ʱ�ȭ
        mysql_init(&conn);

        //�ѱ� ������ �����ϱ� ����
        mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
        mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

        //mysql_real_connect : host���� �������� MySQL �����ͺ��̽� ������ ������ �����Ϸ��� �õ��ϴ� �Լ�
        //MYSQL DB�� ������ ������ connection�� ����
        connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

        //������ �� �߻��ߴ��� �����޽��� ���
        if (connection == NULL) {
            fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
            return 1;
        }

        //DB�� MYSQL ��ɾ� ������ ������
        sprintf(query, "update login set rsp=%d where id='%s' and passwd='%s';", best_rsp, id, passwd);
        //DB�� ������
        int query_stat = mysql_query(connection, query);
        //���࿡ ������ �߻��ϸ� ���п��� ���
        if (query_stat != 0) {
            fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
            return 1;
        }
        //mysql_store_result() �Լ��� ���� �Ҵ�� �޸𸮸� ����
        mysql_free_result(sql_result);
    }
    //DB�� MYSQL ��ɾ� ������ ������
    int query_stat = mysql_query(connection, "select id, rsp from login order by rsp desc");

    //���࿡ ������ �߻��ϸ� ���п��� ���
    if (query_stat != 0) {
        fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
        return 1;
    }
    //����� DB���� �޸𸮿� ���� (= sql ��� ����)
    sql_result = mysql_store_result(connection);

    //row������ ���̵�� ������ ��������� NULL�� �ƴ� ������ �ݺ�
    printf("%15s %10s\n", "���̵�", "����");
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
        printf("%15s %10s\n", sql_row[0], sql_row[1]);  //printf()�̿��Ͽ� ���̵�, ���� ���
    }

    //���� ���� �� ȭ���� ��µǰ� 
    //�ƹ� Ű�� ������ �޴� ȭ������ �Ѿ
    _getch();
    menu();

    return 0;
}


//���ڸ��߱� ����
int m_n(void) {

    // âũ�� ����
    system("mode con: cols=28 lines=20");

    srand(time(NULL));  //���� �� �ʱ�ȭ
    int answer = rand() % 100;  //(100 ������ ���� ��)
    int userinput = 0;  //����ڰ� �Է��ϴ� ��
    int sum = 0; //�հ�
    int score = 100;   //100���� �ְ� �����ϱ� ����

    printf("     <<���ڸ����� ����>>\n\n");    //printf()�� ���� �̸� ���  

    //���ѷ���
    while (1)
    {
        //�Ͼ������ �޽����� �����
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("���ڸ� ����������! : ");    //printf()�� ���ڸ� ������� �޽��� ���
        scanf("%d", &userinput);    //����ڷκ��� userinput�� ���� ���·� �Է¹���
        sum++;  // sum���� 1���� 
        score = score - 10; //�⺻�������� 10�� ����

        //�Ͼ������ �޽����� �����
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        if (answer == userinput)    //�Է��� ���� ���� �´ٸ�
        {
            //�Ķ������� �޽����� �����
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);

            printf("\n���ڸ� �������ϴ�.\n");   //printf()�� ���ڸ� �����ٴ� �޽��� ���
            printf("�õ�Ƚ�� : %d\n\n", sum);   //printf()�� sum���� �õ�Ƚ�� ���
            //�Ͼ������ �޽����� �����
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("\n�� ���� : %d\n", score);  //printf()�� score�� �� ���� ���
            break;  //�����̸� while�� ��������
        }
        else if (answer > userinput)    //�Է��� ������ ���� �� ũ��
        {
            //���������� �޽����� �����
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("�� ���� ���ڸ� �Է����ּ���.\n\n");  //printf()�� �� ���� ���ڸ� �Է��϶�� �޽��� ���
        }
        else {  //�Է��� ������ ���� �� ������
             //���������� �޽����� �����
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("�� ���� ���ڸ� �Է����ּ���.\n\n"); //printf()�� �� ���� ���ڸ� �Է��϶�� �޽��� ���
        }
    }


    // best_m_n�� ���� ������ ��
    if (score > best_m_n) {
        best_m_n = score;

        //mysql_real_connect() �Լ��� ���� �ʱ�ȭ
        mysql_init(&conn);

        //�ѱ� ������ �����ϱ� ����
        mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
        mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

        //mysql_real_connect : host���� �������� MySQL �����ͺ��̽� ������ ������ �����Ϸ��� �õ��ϴ� �Լ�
        //MYSQL DB�� ������ ������ connection�� ����
        connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

        //������ �� �߻��ߴ��� �����޽��� ���
        if (connection == NULL) {
            fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
            return 1;
        }

        //DB�� MYSQL ��ɾ� ������ ������
        sprintf(query, "update login set m_n=%d where id='%s' and passwd='%s';", best_m_n, id, passwd);
        //DB�� ������
        int query_stat = mysql_query(connection, query);
        //���࿡ ������ �߻��ϸ� ���п��� ���
        if (query_stat != 0) {
            fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
            return 1;
        }
        //mysql_store_result() �Լ��� ���� �Ҵ�� �޸𸮸� ����
        mysql_free_result(sql_result);
    }
    //DB�� MYSQL ��ɾ� ������ ������
    int query_stat = mysql_query(connection, "select id, m_n from login order by m_n desc");

    //���࿡ ������ �߻��ϸ� ���п��� ���
    if (query_stat != 0) {
        fprintf(stderr, "���п��� : %s\n", mysql_error(&conn));
        return 1;
    }
    //����� DB���� �޸𸮿� ���� (= sql ��� ����)
    sql_result = mysql_store_result(connection);

    //row������ ���̵�� ������ ��������� NULL�� �ƴ� ������ �ݺ�
    printf("%15s %10s\n", "���̵�", "����");
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
        printf("%15s %10s\n", sql_row[0], sql_row[1]);  //printf()�̿��Ͽ� ���̵�, ���� ���
    }

    //���� ���� �� ȭ���� ��µǰ� 
    //�ƹ� Ű�� ������ �޴� ȭ������ �Ѿ
    _getch();
    menu();

    return 0;
}