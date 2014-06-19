#include <stdio.h>
#include "cache_algorithms.h"
using namespace std;


int main(){
	init();
	CacheAlgorithm* algo = new(S2LazyQueue);

	long long totCnt = 0, missCnt = 0;
	ULLD elem;
	int debug = 1;
	long long cnt = 0;
	long long visitCnt = 0;
	clock_t lastTime = 0, curTime;
	while(scanf("%llu",&elem)!=EOF){
		//printf("scan ------%lld\n",elem);
		if (algo->update(elem, visitCnt)==0){
			if (CountStart)
				missCnt++;
		}
		visitCnt++;
		curTime = clock();
		if (curTime-lastTime>CLOCKS_PER_SEC||debug==2){
			algo->reCalcCnt();
			lastTime = curTime;
		}
		if (CountStart)
			totCnt++;
	
		if (cnt%1000000==0){
								printf("%lld miss: %lld tot: %lld\n", cnt, missCnt, totCnt);
		}
		cnt++;
	}
	printf("Hit: %lld\nMiss: %lld\nTotal: %lld\nThe hit rate is %lf\n", totCnt-missCnt, missCnt, totCnt, (totCnt-missCnt)*1.0/totCnt);
	
}
	
	
