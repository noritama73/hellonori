#include <stdio.h>
#include <stdlib.h>

#define NIL -1

typedef struct node
{
    int key;
    struct node *parent;
    struct node *left;
    struct node *right;
} nod;

nod *tree_search(nod *T, int a);
nod *tree_minimum(nod *T, nod *x);
nod *tree_maximum(nod *T, nod *x);
nod *tree_insert(nod *T, nod *x);
nod *tree_delete(nod *T, nod *z);
void inorder_tree_walk(nod *x);
void tree_show(nod *T, nod *x);

int main(void)
{
    nod *Tree;      /* ２分探索木を表す変数は Tree */
    nod *x, *y, *z; /* 挿入，削除等に用いる node 型変数 */
    int N;          /* 数値の数は N */
    int Data[50];   /* 数値を格納する配列は Data */
    int i;
    FILE *fp;                    /* ファイル名は 128 文字まで対応可能にする */
    fp = fopen("data.txt", "r"); /* ファイルを読み込みモードで開く */
    fscanf(fp, "%d", &N);        /* N をファイルから読み込む */
    if (N > 50)
    {
        printf("N is too large, setting N = 50\n");
        N = 50; /* N が 50 を超えるときは警告をした上で */
    }           /* N =50 に設定する */
    for (i = 0; i < N; i++)
    {
        fscanf(fp, "%d", &Data[i]); /* 整数値を順に読み込み，Data に格納する */
    }
    fclose(fp); /* 開いたファイルを閉じる */
    printf("ファイルを読み込みました。\n");
    Tree = NULL; /* Tree の初期化，最初は空 */
    /* Tree は２分探索木の根を指す*/
    for (i = 0; i < N; i++)
    { /* データ挿入部分のくりかえし */
        x = (nod *)malloc(sizeof(nod));
        x->key = Data[i]; /* 新しい頂点 x を生成して key などを指定 */
        x->parent = x->left = x->right = NULL;
        Tree = tree_insert(Tree, x); /* x を Tree に挿入して根の情報を更新*/
        tree_show(Tree, Tree);
    }
    printf("データの格納が完了しました。\n");
    for (i = 0; i < N; i++)
    { /* データ削除部分のくりかえし */
        z = tree_search(Tree, Data[i]);
        tree_delete(Tree, z);
        tree_show(Tree, Tree);
    }
}

nod *tree_search(nod *T, int a)
{
    nod *x = T;
    while (x != NIL && x->key != a)
    {
        if (a < x->key)
            x = x->left;
        else
            x = x->right;
    }

    return x;
}

nod *tree_minimum(nod *T, nod *x)
{
    while (x->left != NIL)
    {
        x = x->left;
    }

    return (x);
}

nod *tree_maximum(nod *T, nod *x)
{
    while (x->right != NIL)
    {
        x = x->right;
    }

    return (x);
}

nod *tree_insert(nod *T, nod *x)
{
    nod *r, *y, *z;
    r = T; /* 根を変数 r に格納 */
    y = NULL;
    z = r;

    while (z != NIL)
    {
        if (x->key < z->key)
            y = z, z = z->left;
        else
            y = z, z = z->right;
    }

    x->parent = y;

    if (y == NIL)
        r = x;
    else if (y->key < x->key)
        y->right = x->key;
    else
        y->left = x->left;

    return (r);
}

nod *tree_delete(nod *T, nod *z)
{
    nod *x, *y, *p;

    if (z->left == NULL || z->right == NULL)
        y = z;
    else
        y = tree_minimum(T, z->right);

    p = y->parent;

    if (y->right != NULL || y->left != NULL)
        x = (y->left != NULL ? y->left : y->right);
    else
        x = NIL;

    if (p == NIL)
        T = x;
    else
    {
        if (p->key < y->key)
            p->right = x;
        else
            p->left = x;
    }

    if (y != z)
        z->key = y->key;

    free(y);

    return (T);
}

void inorder_tree_walk(nod *x)
{
    if (x != NULL)
    {
        inorder_tree_walk(x->left);
        printf(" %d", x->key);
        inorder_tree_walk(x->right);
    }
}

void tree_show(nod *T, nod *x)
{
    inorder_tree_walk(x);
    printf("\n");
    printf("MAX:%d, MIN:%d\n", *tree_maximum(T, x), *tree_minimum(T, x));
}