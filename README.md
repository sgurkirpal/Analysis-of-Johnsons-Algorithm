# Analysing running time of Johnson’s algorithm using four different heaps

Implemented Johnson's algorithm using four different heaps(array based,
binary heap, binomial heap, fibonacci heap) to determine their running time. The
difference in their running time is due to the dijkstra part of the algorithm. As in
dijkstra we need to find and delete the minimum element and decrease their
value, we use heaps. Decrease key operation is the one which makes the
difference. As it is fastest in fibonacci heaps, fibonacci heaps implementation is
expected to be fastest.

## IMPLEMENTATION
I implemented all the functions in C++ making use of its standard Template library for
vectors. Bellman and Relax operation is a common function for all cases as it is
independent of the type of heap used. For dijkstra, initially all the nodes are inserted in
the heap and then the one with minimum distance is chosen and put in the visited cloud
and its adjacent nodes are relaxed and their values are updated in the heap using
decrease key operation. A map is used to store the pointer to the node for a particular
1vertex present in the heap to access the required node in O(1). It can be thought
of as a similar operation to BFS. Hence its time complexity can be
[Relax*Extract_Minimum*DecreaseKey].

### Array Based Implementation:
Array based implementation is simple requiring no extra function as it chooses
minimum by traversing through all the nodes. It is very slow as it takes O(V) just
to find the minimum edge. Here in dynamic array implementation, Insertion is
amortised O(2). Delete minimum is O(n) as all the nodes are transverse and
decrease key is O(1).

### Binary Heap Implementation:
It involves functions for insertion, deletion and decrease key.
Insertion [bin_insert()] in binary heaps is O(logN) though adding in the heap is
O(1) because after addition it is required to be percolated up to make it reach its
position.
Similarly ExtractMin is also O(logN) as it requires replacing of head node by last
node and then percolating the head node downwards to put it in its correct
position. Here FindMinimum is O(1) as it is always the head node.
For decrease key, index of the node is stored in a lookup table to access the node
in O(1) as and when required. So, decrease key is also O(logN) involving
decreasing the value and then percolating it up for its correct position.

### Binomial Heap Implementation:
Union of two binomial heaps is O(logN) as it includes merging and grouping of
heaps with same degree. Most of the operations of binomial heaps revolve
around the union operation.
Insertion is making a new heap of single element and its union with the existing
one. Making a new heap is O(1) and Union is O(logN), hence insertion is O(logN).
ExtractMin is deleting the node and making a different heap of its children and
2then the union of two heaps. It is also based on union operation ,hence it is also
O(logN).
For Decrease key, a lookup table is maintained to store the pointer to the node of
a particular vertex which is then used to access the node in O(1) while decrease
key. Decrease Key in binomial heaps is very similar to that in binary heaps
involving changing the value and then percolating up to maintain heap
property.Hence , percolate up is O(logN) which gives us O(logN) for Decrease
Key.

### Fibonacci Heap Implementation
ExtractMin in fibonacci heaps is O(logN) as it normally deletes the node and meld
its children to the head heap which makes it amortised O(1). and also
consolidates the heap to maintain heap property which makes it O(logN).
Insertion is O(1) as it just includes adding a new node in the main linked list of
the heap.
Decrease Key is also O(1) as it also makes the node separate adding it in the main
linked list.
Due to very efficient decrease key and extractMIn operations, Fibonacci haaps
are very efficient. But because of high space usage, it becomes a little slow
sometimes than other array based and binomial heaps.


## Data Obtained
![image](https://user-images.githubusercontent.com/58677568/119255762-0d483500-bbdb-11eb-91c1-cd5b04877bb4.png)
![image](https://user-images.githubusercontent.com/58677568/119255771-19cc8d80-bbdb-11eb-846a-2e20fe218008.png)

## CONCLUSION
From the graph and the data obtained from the code, it can be observed that
fibonacci and binomial heaps remain efficient throughout the code because of
low complexity of ExtractMin and decrease key . Because the number of nodes in
the graphs is not much hence array based implementation is also working
perfectly. As the number of nodes increase and graph becomes more and more
dense, binary and array become ineffective while fibonacci becomes effective
there.
