#include <stdio.h>
#include <cache_algorithms.h>
using namespace std;


int main(){
	init();
	
	long long totCnt = 0, missCnt = 0;
	ULLD elem;
	int debug = 1;
	long long cnt = 0;
	visitCnt = 0;
	clock_t lastTime = 0, curTime;
	while(scanf("%llu",&elem)!=EOF){
		//printf("scan ------%lld\n",elem);
		if (update(elem, visitCnt)==0){
			if (CountStart)
				missCnt++;
		}
		visitCnt++;
		curTime = clock();
		if (curTime-lastTime>CLOCKS_PER_SEC||debug==2){
			reCalcCnt();
			lastTime = curTime;
		}
		if (CountStart)
			totCnt++;
		if (debug==2)
				for (int j,i = 0;i < MaxQueueCnt; i++){
					   for (j = 0;j < EachSizeQ[i];j++){
							printf("%d ", Q[i].arr[j].key);
						}
						printf("\n");
				}
	
		if (cnt%1000000==0){
								printf("%lld miss: %lld tot: %lld\n", cnt, missCnt, totCnt);
				for (int i = 0;debug&&i<MaxQueueCnt; i++)
					printf("%d; ", (Q[i].tail-Q[i].head-1+Q[i].size)%Q[i].size);
				printf("---%d\n",hashMap.size());
		}


		cnt++;
	}
	printf("Hit: %lld\nMiss: %lld\nTotal: %lld\nThe hit rate is %lf\n", totCnt-missCnt, missCnt, totCnt, (totCnt-missCnt)*1.0/totCnt);
	
}
	
	