#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
int main(){
	int ah, aw, bh, bw;
	int A[500][500], B[500][500], diff[500][500];
	while(scanf("%d%d%d%d", &ah, &aw, &bh, &bw) != EOF){
		for(int i = 0; i < ah; i++)
			for(int j = 0; j < aw; j++)
				scanf("%d", &A[i][j]);
		for(int i = 0; i < bh; i++)
			for(int j = 0; j < bw; j++)
				scanf("%d", &B[i][j]);

	int h = ah-bh+1, w = aw-bw+1;
	#pragma omp parallel for 
		for(int i = 0; i < h; i++)
			for(int j = 0; j < w; j++){
				int tmp = 0;
				for(int k = 0; k < bh; k++)
					for(int l = 0; l < bw; l++)
						tmp += (A[i+k][j+l]-B[k][l]) * (A[i+k][j+l]-B[k][l]);
				diff[i][j] = tmp;
			}

		int min = 2147483647, r, c;
		for(int i = 0; i < h; i++)
			for(int j = 0; j < w; j++)
				if(diff[i][j] < min){
					min = diff[i][j];
					r = i;
					c = j;
				}
		printf("%d %d\n", r+1, c+1);
	}
	return 0;
}