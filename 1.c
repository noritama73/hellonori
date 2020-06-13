/* 外部ハッシュ法のプログラム */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* 文字列関数を扱えるようにする */
#define W 10        /* W = 文字列の最大長さ，ここでは 10 に設定 */
#define m 97        /* m = ハッシュ表のサイズ，ここでは 97 に設定 */
#define n 50        /* n = 扱う文字列の最大数，ここでは 50 に設定 */
#define NIL -1

typedef struct cell
{
    char key[W + 1];
    int next;
} cl;

int allocate_object(cl *L, int *f);
int chained_hash_search(int *A, cl *L, char *a);
void chained_hash_insert(int *A, cl *L, int x);
void chained_hash_delete(int *A, cl *L, int x);
int hash_val(char *a);
void free_object(cl *L, int *f, int x);

int main(void)
{
    cl List[n]; /* リストは cell の配列，数値数は n まで */
    int A[m];   /* ハッシュ表を表す配列 */
    int N;      /* 数値の数は N */
    int freeL;  /* 空きアドレスのリストの先頭*/
    int i, h;
    int x, y;
    char word[W + 1]; /* ファイルから読み込んだ文字列を格納する変数 */

    char filename[128]; /* 読み込むファイルの名前 */
    FILE *fp;           /* 入力ファイル */
    for (i = 0; i < n - 1; i++)
    { /* allocate_object, free_object のための初期化 */
        List[i].next = i + 1;
    }
    List[n - 1].next = -1;
    freeL = 0; /* 空きリストの初期化 */
    for (h = 0; h < m; h++)
    {
        A[h] = -1;
    } /* ハッシュ表の初期化 */
    printf("ファイル名を入力してください。\n");
    fgets(filename, sizeof(filename), stdin);
    filename[strlen(filename) - 1] = '\0';
    fflush(stdin);

    fp = fopen(filename, "r"); /* ファイルを読み込みモードで開く */
    if (fp == NULL)
    {
        printf("ファイルを開けませんでした。\n");
        exit(1);
    }
    else
        printf("ファイルを読み込みます。\n");

    fscanf(fp, "%d", &N); /* N をファイルから読み込む */
    if (N > n)
    {
        printf("N is too large, setting N = %d\n", n);
        N = n;
    }
    for (i = 0; i < N; i++)
    {
        fscanf(fp, "%s", word);                 /* 文字列をファイルから読み込み，wordに格納 */
        y = chained_hash_search(A, List, word); /* ハッシュ表の中で文字列 word を探索 */
        if (y == NIL)
        {
            x = allocate_object(List, &freeL);
            strcpy(List[x].key, word);
            chained_hash_insert(A, List, x);
        }
        printf("%2d番目の単語を格納しました。\n", i);
    }
    fclose(fp); /* 開いたファイルを一旦閉じる */
    for (h = 0; h < m; h++)
    {
        int head = A[h];
        int c = 0;
        while (head != NIL)
        {
            head = List[head].next;
            c++;
        }
        printf("A[%2d]:%d ", h, c);
        if (h % 10 == 0)
            printf("\n");
        /* ハッシュ表のアドレス h が指す*/
        /* リスト A[h] の長さを出力 */
    }
    printf("\n");
    fp = fopen(filename, "r"); /* ファイルを再度読み込みモードで開く */
    fscanf(fp, "%d", &N);      /* N をファイルから読み込む */
    if (N > n)
    {
        N = n;
    }
    for (i = 0; i < N; i++)
    {
        fscanf(fp, "%s", word); /* 文字列をファイルから読み込み，wordに格納 */
        x = chained_hash_search(A, List, word);
        if (x != NIL)
        {
            chained_hash_delete(A, List, x);
            free_object(List, &freeL, x);
        } /* データを削除する部分 */
        printf("%2d番目の単語を削除しました。\n", i);
    }
    fclose(fp); /* 開いたファイルを閉じる */
    for (h = 0; h < m; h++)
    {
        int head = hash_val(List[h].key);
        int c = 0;
        while (head != NIL)
        {
            head = List[head].next;
            c++;
        }
        printf("A[%2d]:%d ", h, c);
        if (h % 10 == 0)
            printf("\n");
        /* ハッシュ表のアドレス h が指す*/
        /* リスト A[h] の長さを出力 */
    }
    printf("\n");

    return 0;
}

int allocate_object(cl *L, int *f) //(L,freeL)
{                                  /* リスト配列と空きアドレスリスト先頭はポインタ渡し */
    int x;
    if (*f == -1)
    {
        printf("error: out of space\n");
        x = -1;
    } /* 空きアドレスがなければエラーメッセージを表示 */
    else
    {
        x = *f;
        *f = L[x].next;
    }
    return x;
}

int chained_hash_search(int *A, cl *L, char *a)
{
    int x = A[hash_val(a)];

    while (x != NIL && strcmp(L[x].key, a) != 0)
    {
        x = L[x].next;
    }

    return x;
}

void chained_hash_insert(int *A, cl *L, int x)
{
    int h;
    h = hash_val(L[x].key);
    L[x].next = A[h];
    A[h] = x;
}

void chained_hash_delete(int *A, cl *L, int x)
{
    int z;

    if (A[hash_val(L[x].key)] == x)
        z = NIL;
    else
    {
        int tmp = A[hash_val(L[x].key)];
        while (tmp != x)
        {
            tmp = L[tmp].next;
        }
        z = tmp;

        if (z != NIL)
            L[z].next = L[x].next;
        else
            A[hash_val(L[x].key)] = L[x].next;
    }
}

int hash_val(char *a) /* 文字列はポインタ渡し */
{
    int h, i;
    h = 0;
    i = 0;
    while (a[i] != 0 && i < W) /* 文字の整数コードの和を計算 */
    {
        h = h + (int)a[i];
        i = i + 1;
    }
    h = h % m; /* m で割った余りを取る */
    return h;
}

void free_object(cl *L, int *f, int x)
{
    L[x].next = *f;
    *f = x;
}
