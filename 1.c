#include <stdio.h>
#include <stdlib.h>
#define left(i) (2 * (i) + 1)
#define right(i) (2 * (i) + 2)

int i;

void swap(int *a, int *b);
int parent(int j);
void heapsort(int *A, int n);
void build_heap(int *A, int n);
void up_heap_sort(int *A, int j);
void down_heap_sort(int *A, int j);

int main(void)
{
    int Data[50]; /* 数値を格納する配列， 50 まで */
    int N;        
    FILE *fp;

    fp = fopen("data.txt", "r");
    fscanf(fp, "%d", &N);
    for (i = 0; i < N; i++)
    {
        fscanf(fp, "%d", &Data[i]);
    }
    fclose(fp);

    printf("Unsorted:\n");
    for (i = 0; i < N; i++)
    {
        printf("%d ", Data[i]); /* ソート前の数値の出力 */
    }
    printf("\n");

    heapsort(Data, N); /* ヒープソートを呼ぶ */

    printf("Sorted:\n");
    for (i = 0; i < N; i++)
    {
        printf("%d ", Data[i]); /* ソート後の数値の出力 */
    }
    printf("\n");

    return 0;
}

void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

int parent(int j)
{
    return (j - 1) / 2;
}

void heapsort(int *A, int n)
{
    build_heap(A, n);
    for (i = 1; i < n; i++)
    {
        swap(&A[n - i], &A[0]);
        down_heap_sort(A, n - i - 1);
    }
}

void build_heap(int *A, int n)
{
    for (i = 1; i < n; i++)
    {
        up_heap_sort(A, i);
    }
}

void up_heap_sort(int *A, int j)
{
    int u = j;
    while (u > 0 && A[parent(u)] < A[u])
    {
        swap(&A[u], &A[parent(u)]);
        u = parent(u);
    }
}

void down_heap_sort(int *A, int j)
{
    int u = 0;
    int l, r;

    for (;;)
    {
        if (left(u) <= j)
            l = left(u);
        else
            l = u;

        if (right(u) <= j)
            r = right(u);
        else
            r = u;

        if (A[l] == A[r])
            break;
        else
        {
            if (A[l] > A[r])
            {
                swap(&A[l], &A[u]);
                u = l;
            }
            else
            {
                swap(&A[r], &A[u]);
                u = r;
            }
        }
    }
}