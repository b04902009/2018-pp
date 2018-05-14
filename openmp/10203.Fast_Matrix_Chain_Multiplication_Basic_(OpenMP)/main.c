#include <stdio.h>
#include <omp.h>
#define MAXN 2048
#define INF (1LL<<60)
int N;
long long dp[MAXN*MAXN], SZ[MAXN+1];
int main(){
 
    while(scanf("%d", &N) == 1){
        for(int i = 0; i <= N; i++)
            scanf("%lld", &SZ[i]);
        for(int i = 0; i < N; i++)
            dp[i*N+i] = 0;
 
      #pragma omp parallel
      {
        for(int i = 1; i < N; i++){
            #pragma omp for
            for(int j = 0; j < N-i; j++){
                int r = j+i;
                long long local = INF;
                long long tmp = SZ[j] * SZ[r+1];
                for(int k = j; k < r; k++){
                    long long t = dp[j*N+k] + dp[r*N+(k+1)] + SZ[k+1] * tmp;
                    if(t < local)   local = t;
                }
                dp[j*N+r] = dp[r*N+j] = local;
            }
        }
      }
        printf("%lld\n", dp[0*N+N-1]);
    }
    return 0;
}