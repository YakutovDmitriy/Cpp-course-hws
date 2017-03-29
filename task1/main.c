#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n;
    scanf("%d", &n);
    int **a = (int**) malloc(n * sizeof(int*));
    a[0] = (int*) malloc(n * n * sizeof(int));
    int i, j;
    for (i = 1; i < n; ++i)
    {
        a[i] = a[i - 1] + n;
    }
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            a[i][j] = (i + 1) * (j + 1);
        }
    }
    int x1, y1, x2, y2;
    while (scanf("%d", &x1) >= 0)
    {
        if (x1 == 0)
        {
            break;
        }
        scanf("%d%d%d", &y1, &x2, &y2);
        for (int i = x1 - 1; i < x2; ++i)
        {
            for (int j = y1 - 1; j < y2; ++j)
            {
                if (j >= y1)
                {
                    printf(" ");
                }
                printf("%d", a[i][j]);
            }
            printf("\n");
        }
        fflush(stdout);
    }
    free(a[0]);
    free(a);
}
