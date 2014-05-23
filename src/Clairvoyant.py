import  heapq;
from bintrees import RBTree
class PrioQ:
    def __init__(self):
        self.q = [];
        self.cnt = 0;
    def push(self, val, item):
        self.cnt+=1;
        heapq.heappush(self.q, (val, self.cnt, item));

    def pop(self):
        heapq.heappop(self.q);

    def size(self):
        return len(self.q);
    def empty(self):
        return len(self.q)==0;
    


def calc(arr, max_size):
    import  heapq;

    print max_size
    nxt = arr[:];
    pos = {};

    for i in range(len(arr)):
        pos[arr[i]] = len(arr);

    for i in reversed(range(len(arr))):
        nxt[i] = pos[arr[i]];
        pos[arr[i]] = i;

    print nxt;
    prio_queue = RBTree();
    in_queue = {};
    cache_miss = 0;
    cur_in_queue_cnt = 0;
    print arr;
    
    for i in range(len(arr)):
        out_str = str(i)+" ";
        print prio_queue;
        #cache missed
        if in_queue.has_key(arr[i]) == False:
            #while( space need):
            out_str += "Missed: ";
            cache_miss +=1;
            #add into cache
            
            if (cur_in_queue_cnt+1>max_size):
        
                del in_queue[prio_queue.min_item()[1]];
                
                cur_in_queue_cnt-=1;
                out_str+= str(prio_queue.min_item()[1])+" is kicked. And ";
                prio_queue.discard(prio_queue.min_key());
              
                
                
            out_str+= str(arr[i])+" is added into Q.";
            prio_queue.insert((-nxt[i],i),arr[i]);

            cur_in_queue_cnt+=1;
            in_queue[arr[i]] = 1;
        else :
            prio_queue.discard(prio_queue.ceiling_key((-i,0)));
            prio_queue.insert((-nxt[i],i),arr[i]);
            
            out_str += str(arr[i])+" hit.";

        print out_str;
        print cur_in_queue_cnt;
        #print prio_queue.q;

    print "Cache hit: "+str(len(arr)-cache_miss);
    print "Cache miss: "+str(cache_miss);
    print "Ratio: "+ str((len(arr)-cache_miss)*1.0/len(arr));
    

        
        
        
        



if __name__ == "__main__":
    arr = [];
	for lines in stdin:
		arr.append(lines);
	print sys.argu;
    calc(arr, max_size)
    
