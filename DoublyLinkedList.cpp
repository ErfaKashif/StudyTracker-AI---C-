#include <iostream>
#include<vector>
using namespace std;
template <typename T>
//used as adjacency list in graph
//har vertex ke liye indiviual list of dta
class DoublyNode {
public:
    T data;
    DoublyNode<T>* next;
    DoublyNode<T>* prev;

    DoublyNode() {
        data = T();
        next = prev = NULL;
    }

    DoublyNode(T val) {
        data = val;
        next = prev = NULL;
    }

    DoublyNode(const DoublyNode& other) {
        data = other.data;
        next = prev = NULL;
    }

    DoublyNode& operator=(const DoublyNode& other) {
        if (this == &other) return *this;
        data = other.data;
        next = prev = NULL;
        return *this;
    }

    void modifyData(T val) {
        data = val;
    }

    void display() {
        cout << "Data: " << data << endl;
    }
};

template <typename T>
class DoublyLinkedList {
private:
    DoublyNode<T>* head;
    DoublyNode<T>* tail;
    int count;
public:
    DoublyLinkedList() {
        head = tail = NULL;
        count=0;
    }

    void addLast(T val) {
        DoublyNode<T>* newNode = new DoublyNode<T>(val);
        count++;
        if (!head) {
            head = tail = newNode;
            return;
        }
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }

    void addFirst(T val) {
        DoublyNode<T>* newNode = new DoublyNode<T>(val);
        count++;
        if (!head) {
            head = tail = newNode;
            return;
        }
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    void deleteFirst() {
        if (!head) return;
        count--;
        DoublyNode<T>* temp = head;
        head = head->next;
        if (head) head->prev = NULL;
        else tail = NULL;
        delete temp;
    }
    void deleteLast() {
        if (!tail) return;
        count--;
        DoublyNode<T>* temp = tail;
        tail = tail->prev;
        if (tail) tail->next = NULL;
        else head = NULL;
        delete temp;
    }

    void deleteByValue(T val) {
    	if(isEmpty()) return;
        DoublyNode<T> *curr = searchByValue(val);
        if(curr==head) deleteFirst();
        else if(curr==tail) deleteLast();
        else if(curr!=NULL){
        	curr->prev->next = curr->next;
        	curr->next->prev = curr->prev;
        	delete curr;
		}
		return;
    }

    DoublyNode<T>* searchByValue(T val) {
        DoublyNode<T>* curr = head;
        while (curr) {
            if (curr->data == val) return curr;
            curr = curr->next;
        }
        return NULL;
    }

    void reverse() {
        DoublyNode<T>* curr = head;
        DoublyNode<T>* temp = NULL;
        while (curr) {
            temp = curr->prev;
            curr->prev = curr->next;
            curr->next = temp;
            curr = curr->prev;
        }
        if (temp) {
            tail = head;
            head = temp->prev;
        }
    }

    void display() {
        DoublyNode<T>* curr = head;
        while (curr) {
            cout << curr->data << " <-> ";
            curr = curr->next;
        }
        cout << "NULL" << endl;
    }
    bool isEmpty(){
    	if(head==NULL) return true;
    	return false;
	}
	int getCount(){
		return count;
	}
	void insertAtPosition(int pos, T data){
		if(pos<=0) return;
		if(pos==1){
			addFirst(data);
			return;
		}
		if(pos>count){
			addLast(data);
			return;
		}
		if(isEmpty()) return;
		DoublyNode<T> *temp=head;
		DoublyNode<T> *newNode=new DoublyNode<T>(data);
		 for (int i = 1; i < pos -1 && temp != NULL; i++) {
        temp = temp->next;
    	}
		if (temp == NULL) return;
		newNode->next=temp->next;
		newNode->prev=temp;
		if (temp->next != NULL) temp->next->prev = newNode;
    	temp->next = newNode;	
		return ;
	}
	DoublyNode<T>* getHead() const { return head; }
	DoublyNode<T>* getTail() const { return tail; }
	T getFirst() const { return head ? head->data : T(); }
	T getLast() const { return tail ? tail->data : T(); }
	void clear() {
    	while (!isEmpty()) deleteFirst();
	}
};

template <typename T>
class Node {
private:
    T data;
    Node *next;
    
public:
    Node(){
        next=NULL;
    }
    
    Node(T data){
        this->data=data;
        next=NULL;
    }
    
    T& getData() { return data; }
    Node<T>* getNext() { return next; }
    void setNext(Node<T>* n) { next = n; }
    void display() {
        data.display();
    }
};
// ============ QUEUE ============
template <typename T>
class Queue {
    Node<T>* front;
    Node<T>* rear;
    int size;
    
public:
    Queue(): front(NULL), rear(NULL), size(0) {}
    ~Queue() { clear(); }
    
    void clear() {
        while(!isEmpty()) dequeue();
    }
    
    bool isEmpty() const { return front == NULL; }
    
    void enqueue(const T &value) {
        try {
            Node<T>* newNode = new Node<T>(value);
            ++size;
            if(rear == NULL){
                front = rear = newNode;
                return;
            }
            rear->setNext(newNode);
            rear = newNode;
        } catch (const bad_alloc& e) {
            cerr << "Memory allocation failed" << endl;
        }
    }
    
    void dequeue() {
        if(isEmpty()) return;
        Node<T>* temp = front;
        front = front->getNext();
        if(front == NULL) rear = NULL;
        --size;
        delete temp;
    }
    
    T peekData() const {
        if(isEmpty()){
            return T();
        }
        return front->getData();
    }
    
    Node<T>* peekNode() const { return front; }
    int getSize() const { return size; }
};
class UndirectedGraph{
	public:
	int vertices;
	DoublyLinkedList<int> *list;//list is the dynamic array of adjacecny list of all th verticesof the graph.it stores ke whih vertice is connected with which vertice
	public:
		UndirectedGraph(int v){
			vertices=v;
			list=new DoublyLinkedList<int>[vertices];
		}
		void addEdge(int u,int v){
			list[u].addLast(v); //u ki adjacency list mein v add kiya
			list[v].addLast(u);//v ki adjacency list u add kiya
		}
		void printList(){
			for(int i=0;i<vertices;i++){
				cout<<i<<": ";
				list[i].display();
			}
		}
		void bfs(){//O(V+E) vertices + edges
			Queue<int> queue;
			vector<bool> visited(vertices,false);
			queue.enqueue(0);//pehle ko enqueue karliya
			cout<<queue.peekData();
			cout<<endl;
			visited[0]=true;//and mark it as visited
			while(!queue.isEmpty()){
				int u=queue.peekData();
				queue.dequeue();
				DoublyNode<int> *curr=list[u].getHead();
				while(curr){
					int v=curr->data;
					if(!visited[v]) {
						visited[v]=true;
						cout<<v<<"->";
						queue.enqueue(v);
					}
					curr=curr->next;
				}	
					cout<<endl;
		}
	}
	void dfs(DoublyNode<int> *curr,vector<bool> &visited){
		int u=curr->data;
			cout<<u<<" ";
			visited[u]=true;
		DoublyNode<int> *temp=list[u].getHead();
		while(temp){
			int v=temp->data;
			if(!visited[v]) dfs(temp,visited);
			temp=temp->next;
		}
		cout<<endl;
	}
    bool cycleDetectionUtil(int v, vector<bool> &visited, int parent) {
        visited[v] = true;
        DoublyNode<int>* curr = list[v].getHead();
        while (curr) {
            int adj = curr->data;
            if (!visited[adj]) {
                if (cycleDetectionUtil(adj, visited, v))
                    return true;
            } else if (adj != parent) {
                return true;
            }
            curr = curr->next;
        }
        return false;
    }
};
class DirectedGraph{
    public:
    int vertices;
    DoublyLinkedList<int> *list;//list is the dynamic array of adjacecny list of all th verticesof the graph.it stores ke whih vertice is connected with which vertice
    public:
        DirectedGraph(int v){
            vertices=v;
            list=new DoublyLinkedList<int>[vertices];
        }
        void addEdge(int u,int v){
            list[u].addLast(v); //u ki adjacency list mein v add kiya. NOT IN BOTH AS THIS IS DIRECTED GRAPH
        }
        void printList(){
            for(int i=0;i<vertices;i++){
                cout<<i<<": ";
                list[i].display();
            }
        }
        void dfs(DoublyNode<int> *curr,vector<bool> &Visited){
            int u=curr->data;
                Visited[u]=true;
                cout<<u<<" ";
            DoublyNode<int> *temp=list[u].getHead();
            while(temp){
                int v=temp->data;
                if(!Visited[v]){ 
                    dfs(temp,Visited);
                    cout<<"->";
                }
                temp=temp->next;
            }
        }
        void bfs(DoublyNode<int> *start,vector<bool> &visited){
            Queue<int> queue;
            queue.enqueue(start->data);
            visited[start->data]=true;
            while(!queue.isEmpty()){
                int u=queue.peekData();
                queue.dequeue();
                cout<<u<<" ";
                DoublyNode<int> *curr=list[u].getHead();
                while(curr){
                    int v=curr->data;
                    if(!visited[v]) {
                        visited[v]=true;
                        queue.enqueue(v);
                    }
                    curr=curr->next;
                }
            }
        }
        bool cycledetection(int curr,vector<bool> &visited,vector<bool> &recStack){
            visited[curr]=true;    
            recStack[curr]=true;
            DoublyNode<int> *temp=list[curr].getHead();
            while(temp){
                int v=temp->data;
                if(!visited[v]){
                    if(cycledetection(v,visited,recStack)) return true;
                }
                else if(recStack[v]){
                return true;
            }
            temp=temp->next;
        }
        recStack[curr]=false;
        return false;
    }
};
        // Example use
int main() {
    cout<<"Undirected Graph:"<<endl;
	UndirectedGraph g(5);
	g.addEdge(0,1);
	g.addEdge(1,2);
	g.addEdge(1,3);
	g.addEdge(2,4);
	g.printList();
	vector<bool> visited(g.vertices,false);
	g.dfs(g.list[0].getHead(),visited);
    cout<<"Directed Graph:"<<endl;
    DirectedGraph dg(5);
	dg.addEdge(0,1);
	dg.addEdge(1,2);
	dg.addEdge(1,3);
	dg.addEdge(2,4);
    dg.addEdge(2,3);
    dg.addEdge(4,1); //adding this edge to create a cycle
	dg.printList();
    vector<bool> visited1(g.vertices,false);
    dg.dfs(dg.list[0].getHead(),visited1);
    cout<<endl;
    vector<bool> visited2(g.vertices,false);
    dg.bfs(dg.list[0].getHead(),visited2);
    cout<<endl;
    vector<bool> visited3(g.vertices,false);
    vector<bool> stack(g.vertices,false);
    bool cycle=dg.cycledetection(0,visited3,stack);
    if(cycle) cout<<"Cycle detected in directed graph"<<endl;
    else cout<<"No cycle detected in directed graph"<<endl;
//    DoublyLinkedList<int> list;
//    list.addLast(10);
//    list.addLast(20);
//    list.addLast(30);
//    list.display();
//    list.addFirst(5);
//    list.display();
//    list.deleteFirst();
//    list.deleteLast();
//    list.deleteByValue(20);
//    list.display();
//	list.insertAtPosition(1,45);
//    //list.reverse();
//    list.insertAtPosition(2,40);
//    list.display();
//    list.insertAtPosition(2,45);
//    //list.reverse();
//    list.display();
//    list.reverse();
//    list.display();

//    list.deleteByValue(20);
//    list.display();

    return 0;
}

