#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define MAXN 2005
char cell[2][MAXN][MAXN];
int main(){
    int N, M;
    scanf("%d%d", &N, &M);
    for(int i = 1; i <= N; i++){
        scanf("%s", cell[0][i]+1);
        for(int j = 1; j <= N; j++)
        	cell[0][i][j] -= '0';
    }
 
    for(int t = 1; t <= M; t++){
        char (*cur)[MAXN] = cell[t&1], (*bef)[MAXN] = cell[(t&1)^1];
        #pragma omp parallel for
        for(int i = 1; i <= N; i++)
            for(int j = 1; j <= N; j++){
                int num = bef[i-1][j-1] + bef[i-1][j] + bef[i-1][j+1]
                        + bef[i][j-1] + bef[i][j+1]
                        + bef[i+1][j-1] + bef[i+1][j] + bef[i+1][j+1];
                if(bef[i][j] && (num < 2 || num > 3))
                    cell[t&1][i][j] = 0;
                else if(!bef[i][j] && num == 3)
                    cur[i][j] = 1;
                else
                    cur[i][j] = cell[(t&1)^1][i][j];
            }
    }
    char (*last)[MAXN] = cell[M&1];
    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= N; j++)
            putchar(last[i][j]+'0');
        putchar('\n');
    }
    return 0;
}