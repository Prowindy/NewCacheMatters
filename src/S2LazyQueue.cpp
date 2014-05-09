#include <iostream>
#include <map>
#include <stack>
#include <unordered_map>
using namespace std;
const long long MaxSpace = 3LL*1024*1024*1024; //Unit: Kb

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

struct Node {
	unsigned int key;
	unsigned int info;
};

unordered_map<unsigned int, Node *> hashMap;
int CountStart = 0;

struct Queue{

	Node *arr;
	int head,size,tail, level;
	Queue* Q;	
	void push(Node item){
		//printf("%d push %d----\n",level, item.key);
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
			pop();
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
		//printf("%d pop\n",level);
		Node rec = arr[head++];
		if (head==size)
			head = 0;
		int cnt = rec.info>>2, i;
		
		//check if its level will move up
		for (i = MaxQueueCnt-1; i>level; i--)
			if (cnt>=leastCntQ[i])
				break;
		if (i>level){
			//level up
			Q[i].push(rec);
		}else if (level>0){
			//level down
			//avoid infinite loop, decrease its count below current level
			rec.info = (leastCntQ[level]-1)<<2;
			Q[level-1].push(rec);
		}else {
			//move out
			hashMap.erase(rec.key);
			CountStart = 1;
		}
	}
	bool empty(){
		return head==-1;
	}
	bool full(){
		return head==tail;
	}

}Q[4];

int update(unsigned int elem){
	if (hashMap.find(elem)==hashMap.end()){
		//miss
		Node tmp;
		tmp.key= elem;
		tmp.info = 1<<2;
		Q[0].push(tmp);
		return 0;
	}else {
		//hit
		Node* cur = hashMap[elem];
		cur->info +=(1<<2);
		return 1;
	}
		
}
void init(){
	hashMap.clear();
	for (int i = 0;i < MaxQueueCnt; i++){
		Q[i].size = EachSizeQ[i];
		Q[i].head = -1, Q[i].tail = 0;
		Q[i].level = i;
		Q[i].arr = (Node *)malloc(sizeof(Node *)*Q[i].size);
		Q[i].Q = Q;
	}
}
int main(){
	init();
	
	long long totCnt = 0, missCnt = 0;
	unsigned int elem;
	int debug = 1;
long long cnt = 0;
	while(scanf("%u",&elem)!=EOF){
		//printf("scan ------%d\n",elem);
		if (update(elem)==0){
			if (CountStart)
				missCnt++;
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
	
	
	
	
	
