#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int all, count = 0;
int N, barrier[20];
int queen(int row, int col, int ld, int rd){
	if(row == N)
		return 1;
	int available = all & ~(col | ld | rd) & ~barrier[row];
	int sum = 0;
	while(available){
		int choose = available & -available; // lowbit
		available ^= choose;
		sum += queen(row+1, col|choose, (ld|choose)<<1, (rd|choose)>>1);
	}
	return sum;
}
int main(){
	int case_num = 1;
	while(scanf("%d", &N) != EOF){
		count = 0;
		all = (1 << N) - 1;
		char s[20];
		for(int i = 0; i < N; i++){
			scanf("%s", s);
			barrier[i] = 0;
			for(int j = 0; j < N; j++)
				if(s[j] == '*')
					barrier[i] |= (1 << j);
		}
		int ans = 0;
		// double ts = omp_get_wtime();
#pragma omp parallel for schedule(dynamic, 1) reduction(+: ans)
			for(int i = 0; i < N; i++){
				int pos = 1<<i;
				if(pos & barrier[0])
					continue;
				// double t = omp_get_wtime() - ts;
				// printf("thread%d:%.0lf\n", omp_get_thread_num(), t);
				int num = queen(1, pos, pos<<1, pos>>1);
				ans += num;
			}
		printf("Case %d: %d\n", case_num++, ans);
	}
	return 0;
}
// bitwise operation for N_queen: https://zhuanlan.zhihu.com/p/22846106