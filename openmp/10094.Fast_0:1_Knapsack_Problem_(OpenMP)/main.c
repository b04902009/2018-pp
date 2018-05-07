#include <stdio.h>
#include <stdlib.h>
#define MAXN 10005
#define MAXM 1000005
struct item{
	int w, v;	
} item;
struct item items[MAXN];
int dp[2][MAXM];
int max(int a, int b){return a > b? a : b;}
int min(int a, int b){return a < b? a : b;}
int cmp(const void* a, const void* b){
	struct item A = *(struct item*)a, B = *(struct item*)b;
	return A.w < B.w;
}
int main(){
	int N, M;
	scanf("%d%d", &N, &M);
	for(int i = 1; i <= N; i++)
		scanf("%d%d", &items[i].w, &items[i].v);
	qsort(items, N, sizeof(struct item), cmp);

#pragma omp parallel 
{
	int sum = 0;
	for(int i = 1; i <= N; i++){
		int *cur = dp[i&1], *bef = dp[(i&1)^1];
		sum = min(sum+items[i].w, M);
		#pragma omp for
		for(int j = items[i].w; j <= sum; j++)
			cur[j] = max(bef[j-items[i].w]+items[i].v, bef[j]);
	}
}
	int *last = dp[N&1];
	for (int i = 1; i <= M; i++)
    	last[i] = max(last[i], last[i-1]);

	printf("%d\n", last[M]);

	return 0;
}