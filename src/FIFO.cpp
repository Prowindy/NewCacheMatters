#include <iostream>
#include <map>
#include <stack>
#include <unordered_map>
using namespace std;
const long long MaxSpace = 3LL*1024*1024*1024; //Unit: Kb

const int MaxUnits = MaxSpace/32;

const int MaxQueueCnt = 4; //S 4 LRU

struct Queue{

	int arr[MaxUnits+10];
	int head,size,tail;

}Q;
unordered_map<int, int> hashMap;
int CountStart = 0;
int update(int elem){
	if (hashMap.find(elem)==hashMap.end()){
		if (Q.head==Q.tail){
			CountStart = 1;	
			hashMap.erase(Q.arr[Q.head]);
			Q.head++;
			if (Q.head==Q.size)
				Q.head = 0;
		}
		hashMap[elem] = 1;
		Q.arr[Q.tail++] = elem;
		if (Q.tail==Q.size)
			Q.tail = 0;
		return 1;
	}
	return 0;
}
void init(){
	hashMap.clear();
	CountStart = 0;
	Q.size = MaxUnits;
	Q.head = -1, Q.tail = 0;
}
int main(){
	init();
	
	long long totCnt = 0, missCnt = 0;
	int elem;
	scanf("%d",&elem);
	hashMap[elem] = 1;
	Q.arr[Q.tail++] = elem;
	Q.head = 0;
	long long cnt=0;
	while(scanf("%d",&elem)!=EOF){
		cnt ++;
		if (update(elem)==1){
			if (CountStart)
				missCnt++;
		}
		if (CountStart)
			totCnt++;
		if (cnt%1000000==0)
			printf("%lld %d-%dmiss: %lld tot: %lld\n", cnt, Q.head, Q.tail,missCnt, totCnt); 
	}
	printf("Hit: %lld\nMiss: %lld\nTotal: %lld\nThe hit rate is %lf\n", totCnt-missCnt, missCnt, totCnt, (totCnt-missCnt)*1.0/totCnt);
	
}
	
	
	
	
	
