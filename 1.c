#include <stdio.h>
#include <stdlib.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
int i;

typedef struct cell
{
    int key;
    int prev;
    int next;
} cl;

int allocate_object(cl *L, int *f);
void list_insert(cl *L, int *head, int p, int new);
void list_delete(cl *L, int *head, int a);
void free_object(cl *L, int *f, int x);

int main(void)
{
    cl List[50];     /* リストは cell の配列，数値数は 50 まで */
    int n;           /* 数値の数は N */
    int Data[50];    /* 数値を格納する配列は Data */
    int head, freeL; /* リスト List の先頭，空きアドレスリストの先頭 */
    FILE *fp;
    fp = fopen("data.txt", "r"); /* ファイルを読み込みモードで開く */
    if (fp == NULL)
        printf("ファイルの読み込みに失敗しました。\n");
    else
        printf("ファイルを読み込みます。\n");
    fscanf(fp, "%d", &n); /* N をファイルから読み込む */
    if (n > 50)
    {
        printf("N is too large, setting N = 50\n");
        n = 50; /* N が 50 を超えるときは警告をした上で */
    }           /* N =50 に設定する */
    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%d", &Data[i]); /* 整数値を順に読み込み，Data に格納する */
    }
    fclose(fp); /* 開いたファイルを閉じる */
    printf("ファイルを閉じました。\n");

    for (i = 0; i < 50; i++)
    {
        if (i == 0)
            List[i].prev = -1;
        else if (i == 49)
            List[i].next = -1;
        else
        {
            List[i].prev = i - 1;
            List[i].next = i + 1;
        }
    }
    printf("リストを初期化しました。\n");

    head = -1;
    freeL = 0; /* head freeL の初期化 */
    for (i = 0; i < n; i++)
    {
        int emp = allocate_object(List, &freeL);
        List[emp].key = Data[i];
        list_insert(List, &head, -1, emp);
    }
    printf("リストに数値を格納しました。\n");

    int mini;
    for (i = 0; i < n; i++)
    {
        mini = INT_MAX;
        for (int j = 0; j < n; j++)
        {
            mini = min(mini, (List + j)->key);
        }
        Data[i] = mini;
        free_object(List, &freeL, mini);
        list_delete(List, &head, mini);
    }
    printf("ソートが完了しました。\n");

    for (i = 0; i < n; i++)
    {
        printf("%d ", Data[i]);
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

void list_insert(cl *L, int *head, int p, int new)
{
    L[new].prev = p;
    L[new].next = *head;
    if (*head != -1)
        L[*head].prev = new;
    *head = new;
}

void list_delete(cl *L, int *head, int a)
{
    int x = a;
    while (x != -1 && L[x].key != a)
    {
        x = L[x].next;
    }

    if (x != -1)
    {
        int p = L[x].prev, q = L[x].next;
        if (p != -1)
        {
            L[p].next = q;
            if (q != 1)
                L[q].prev = p;
        }
        else
        {
            *head = q;
            if (q != -1)
                L[q].prev = -1;
        }
    }
}

void free_object(cl *L, int *f, int x)
{
    L[x].next = *f;
    *f = x;
}