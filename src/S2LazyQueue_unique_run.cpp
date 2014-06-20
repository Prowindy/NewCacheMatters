#include <iostream>
#include <map>
#include <stack>
#include <unordered_map>
#include <time.h>
#include <string.h>
using namespace std;

typedef unsigned long long ULLD;

const long long MaxSpace = 4.8*1024*1024*1024; //Unit: Kb

const int MaxUnits = MaxSpace/32;

const int MaxQueueCnt = 2; //S 4 FIFO

int EachSizeQ[MaxQueueCnt+10] = {
	MaxUnits/10*7,
	MaxUnits/10*3,
	MaxUnits/10,
	MaxUnits/10
};
int leastCntQ[MaxQueueCnt+10] = {
	0,
	2,
	5,
	41
};
// space for every layer.
double SpaceRatio[MaxQueueCnt+10] = {
	0.7,
	0.3
};

//total space under this layer.
double SummaryRatio[MaxQueueCnt + 10] = {
	0.0,
	0.7
};

//the ratio who can stay in current layer.
const double StayRatio = 0.5;

//the ratio who can upstair under discarding.
const double UpRatio = 0.75;

//the ratio who can stay by its last visit time is not far away.
const double LastVisitRatio = 0.1;

//Adjust the coming element's Count to the upper bound of this layer.
int MaxCntInLayer[MaxQueueCnt + 10] ;


struct Node {
	ULLD key;
	unsigned int info;
	long long lastVisit;
	Node(){
		key = info = lastVisit = 0;
	}
};


const int MaxRecCount = 1000;
struct fenwickTree{
	int recordTree[MaxRecCount+1];
	
	int add(int x, int val){
		for (int i = x; i<=MaxRecCount; i+= i&(-i))
			recordTree[i]+=val;
	}
	
	int getSum(int x){
		int ret = 0;
		for (int  i = x; i> 0; i-= i&(-i))
			ret+= recordTree[i];
		return ret;
	}
	
}fwTree;

//calculate the partial summary per second.
long long CntSum[MaxRecCount+1];
long long NumInCnt[MaxRecCount +1 ];

unordered_map<ULLD, Node *> hashMap;
int CountStart = 0;
long long visitCnt = 0;
struct Queue{

	Node *arr;
	int head,size,tail, level;
	Queue* Q;	
	void push(Node item){
	//	printf("%d push %d----\n",level, item.key);
		Node *cur = arr+tail;
		item.info = ((item.info>>2)<<2)|level;
		if (head==-1){
			tail++;
			if (tail==size)
				tail = 0;
			*cur = item;
			hashMap[cur->key] = cur;
			head = 0;
		}else if(head==tail){
			if (level==0&&!Q[level+1].full()){
				Q[level+1].push(item);
				return ;
			}
	
			pop();
			if (level>0)
				CountStart = 1;
			push(item);
		}else {
			*cur = item;
			hashMap[cur->key] = cur;
			tail++;
			if (tail==size)    
				tail = 0;
		}
	}
	void pop(){
	//	printf("%d pop\n",level);
		Node rec = arr[head++];
		if (head==size)
			head = 0;
		int cnt = rec.info>>2, i;
		double less_cnt = CntSum[cnt-1];
		double less_ratio = less_cnt / MaxUnits;
		//check if its level will move up
		for (i = MaxQueueCnt-1; i>0; i--)
			if (less_ratio >= SummaryRatio[i-1] + SpaceRatio[i-1]*UpRatio  )
				break;
		
		if (i>level){
			//level up
			Q[i].push(rec);
		}else if ((i==level && less_ratio >= SummaryRatio[i] + SpaceRatio[i]*StayRatio)
			|| ((visitCnt - rec.lastVisit)<=LastVisitRatio*MaxUnits)) {
			//Stay in this level
			Q[i].push(rec);
			
		}else if (level>0){
			//level down
			//avoid infinite loop, decrease its count below current level
			NumInCnt[rec.info>>2] --;
			if (MaxCntInLayer[level-1] < (rec.info>>2)){
				rec.info &= (1<<2)-1;
				rec.info |= MaxCntInLayer[level-1]<<2;
			}
			NumInCnt[rec.info>>2] ++;
			Q[level-1].push(rec);
		}else {
			//move out
			NumInCnt[rec.info>>2] --;
			hashMap.erase(rec.key);
			//CountStart = 1;
		}
	}
	bool empty(){
		return head==-1;
	}
	bool full(){
		return head==tail;
	}

}Q[4];

int update(ULLD elem, long long visitTime){
	if (hashMap.find(elem)==hashMap.end()){
		//miss
		Node tmp;
		tmp.key= elem;
		tmp.info = 1<<2;
		tmp.lastVisit = visitTime;
		NumInCnt[tmp.info>>2] ++;
		Q[0].push(tmp);
		return 0;
	}else {
		//hit
		Node* cur = hashMap[elem];
		NumInCnt[cur->info>>2] --;
		cur->info +=(1<<2);
		if ((cur->info>>2)>MaxRecCount){
			cur->info = (MaxRecCount<<2) + (cur->info&((1<<2)-1));
		}
		cur->lastVisit = visitTime;
		NumInCnt[cur->info>>2] ++;
		return 1;
	}
		
}

void reCalcCnt(){
	CntSum[0] = 0;
	int level = 0;
	for (int i = 1; i <= MaxRecCount; i++){
		CntSum[i] = CntSum[i-1] + NumInCnt[i];
		if (level+1 < MaxQueueCnt && CntSum[i-1] > Q[level].size)
			level++;
		MaxCntInLayer[level] = i;
	}
	for (level++; level < MaxQueueCnt; level++)
		MaxCntInLayer[level] = MaxCntInLayer[level-1];
}
void init(){
	hashMap.clear();
	for (int i = 0;i < MaxQueueCnt; i++){
		Q[i].size = EachSizeQ[i];
		Q[i].head = -1, Q[i].tail = 0;
		Q[i].level = i;
		Q[i].arr = (Node *)malloc(sizeof(Node)*Q[i].size);
		Q[i].Q = Q;
	}
	memset(fwTree.recordTree, 0, sizeof( fwTree.recordTree));
	memset(NumInCnt, 0, sizeof( NumInCnt ));
	
}
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
	
	
	
	
	
