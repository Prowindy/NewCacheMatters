/*
☆ LRU
Hit: 	5480958763
Miss: 	1179787053
Total:	6660745816
FullIt:	487101878
The hit rate is 0.822875
*/

#include <iostream>
#include <map>
#include <stack>
#include <unordered_map>
using namespace std;
const long long MaxSpace = 3LL*1024*1024*1024; //Unit: Kb

const int MaxUnits = MaxSpace/32;

const int MaxQueueCnt = 1; //S 4 LRU

//size of every Queue
const int MaxQueueSize[MaxQueueCnt] = {
	MaxUnits, ///MaxQueueCnt,
	//MaxUnits/10,   //MaxQueueCnt,
	//MaxUnits/10,   //MaxQueueCnt,
	//MaxUnits/10,   //MaxQueueCnt,
};


struct Node{
	unsigned int key;
	int pre;
	int next;
};
Node NodeBuf[MaxUnits+10];
char grade[MaxUnits+10];

stack<int> freeBufs;
unordered_map<unsigned int, int> hashMap;
int CountStart;
int newNode(){
	if (freeBufs.empty())
		return -1;
	int ret = freeBufs.top();
	freeBufs.pop();
	return ret;
}
void freeNode(int p){
	
	freeBufs.push(p);
	
}

class LRU_Q{
public:
	int head, tail;
	int size;
	void init(){
		head = newNode();
		tail = newNode();
		NodeBuf[head].next = tail;
		NodeBuf[tail].pre = head;
		size = 0;
	}
	
	inline void delNode(int p){
		NodeBuf[NodeBuf[p].pre].next = NodeBuf[p].next;
		NodeBuf[NodeBuf[p].next].pre = NodeBuf[p].pre;
		size--;
	}
	
	inline void addBetween(int p, int a, int b){
		NodeBuf[a].next = p;
		NodeBuf[p].pre = a;
		NodeBuf[p].next = b;
		NodeBuf[b].pre = p;
	}
	
	void pushNode(int p){
		addBetween(p, NodeBuf[tail].pre, tail);
		size++;
	}
	
	int popNode(){
		int ret = 0;
		if (NodeBuf[head].next!=tail){
			ret = NodeBuf[head].next;
			delNode(NodeBuf[head].next);
		//	size--;
		}
		return ret;
	}
	
};

LRU_Q Queue[MaxQueueCnt];
void init(){
	while(!freeBufs.empty())
		freeBufs.pop();
	for (int i = 0; i < MaxUnits+10; i++)
		freeBufs.push(i);
	for (int i = 0; i < MaxQueueCnt; i++)
		Queue[i].init();
}

//leave one slot out to a particular Queue, maybe by send down some items.
//items are evicted from the tail of a queue to the head of the next lower queue to maintain the size invariants. 
//Items evicted from queue 0 are evicted from the cache.
void adjustQueue(int Qno){
	if (Queue[Qno].size<MaxQueueSize[Qno])
		return ;
	if (Queue[Qno].size==MaxQueueSize[Qno]){
		if (Qno==0){
			//evict from the total cache
			int front = Queue[Qno].popNode();
			
			//free hashMap to maintain size in TotElems
			hashMap.erase(NodeBuf[front].key);
			
			//free this node
			freeNode(front);
			grade[front] = -1;
			

			//count start.
			CountStart = 1;
		}else {
			//move a item to next lower queue
			int front = Queue[Qno].popNode();
			grade[front] = -1;
			adjustQueue(Qno-1);
			Queue[Qno-1].pushNode(front);
			grade[front] = Qno-1;
		}
			
	}else printf("ERROR size larger than MaxQueueSize\n");
}  
	
int update(unsigned int elem){
	int ret = 0;
	int cur;
	
	//update in hashmap
	if (hashMap.find(elem)==hashMap.end())
	{
		ret = 1;
		hashMap[elem] = cur = newNode();
		NodeBuf[cur].key = elem;
		grade[cur] = -1;
	}
	else 
		cur = hashMap[elem];
		
	//take out from current queue
	if (grade[cur]!=-1){
		Queue[grade[cur]].delNode(cur);
	}
	
	//calculate the next queue need to go
	grade[cur]++;
	grade[cur] = min((int)grade[cur], MaxQueueCnt-1);
	
	//adjust the queue by pop some items.
	int Qno = grade[cur];
	
	adjustQueue(grade[cur]);
	
	Queue[grade[cur]].pushNode(cur);
/*	
	printf("%d--%d\n",grade[cur],grade[0]);
	for (int i = 1; i<10;i++)
		printf("%d%c",hashMap.find(i)==hashMap.end()?-1:grade[hashMap[i]], (i==9?'\n':' '));
	for (int i = 0;i<MaxQueueCnt; i++){
		 int cur = Queue[i].head;
		 //printf("===%d %d\n",Queue[i].head, Queue[i].tail);
         cur = NodeBuf[cur].next;
		 while(cur!=Queue[i].tail){
			 printf("%d ",NodeBuf[cur].key);
			 cur = NodeBuf[cur].next;
		 }
		 printf("----Queue %d\n",i);
	}
	
*/	
	
	return ret;
		

}
int main(){
	init();
	hashMap.clear();
	long long totCnt = 0, missCnt = 0;
	unsigned int elem;
	CountStart = 0;
	long long index = 0;
	while(scanf("%u",&elem)!=EOF){
		index ++;
		if (update(elem)==1){
			if (CountStart)
			    missCnt++;
		}
		if (CountStart)
			totCnt++;
		if (index%1000000==0){
			printf("%lld miss:%lld tot:%lld\n",index, missCnt, totCnt);
		}
		/*
		for (int i= 0;i<MaxQueueCnt; i++)
		
			printf("%d%c",Queue[i].size, (i+1==MaxQueueCnt)?'\n':' ');
		*/
	}
	printf("Hit: %lld\nMiss: %lld\nTotal: %lld\nThe hit rate is %lf\n", totCnt-missCnt, missCnt, totCnt, (totCnt-missCnt)*1.0/totCnt);
	
}
	
	
	
	
	
