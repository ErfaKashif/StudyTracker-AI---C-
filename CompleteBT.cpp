#include<iostream>
#include<vector>
#include "Queue.cpp"
using namespace std;
template <typename T>
class TreeNode{
	public:
	T data;
	TreeNode<T> *left,*right;
	TreeNode<T>* parent;
	TreeNode<T>(T data){
		this->data=data;
		left=right=parent=NULL;
	}
	void display() {
    	data.display();
	}
	T* getData(){
		return &data;
	}
	T& getDataObj(){
		return data;
	}
};
//array se 	queue ,queue se tree 
template <typename T>
class CompleteBT{
	public:
	TreeNode<T> *root;
		CompleteBT<T>(){
			root=NULL;
		}
	void createTree(vector<T> &arr){
		this->root=new TreeNode<T>(arr[0]);
		Queue<TreeNode<T>*> queue;
		queue.enqueue(root);
		int i=1;
		while(!queue.isEmpty() && i< arr.size()){
			TreeNode<T> *temp=queue.peekNode();
			queue.dequeueForTree();
			if(i<arr.size()){
				temp->left=new TreeNode<T>(arr[i]);
				queue.enqueue(temp->left);
				i++;
			}
			if(i<arr.size()){
				temp->right=new TreeNode<T>(arr[i]);
				queue.enqueue(temp->right);
				i++;
			}
		}
	}
	
	TreeNode<T>* insertNode(T value){
    if(!root){
        root = new TreeNode<T>(value);
        return root;
    }

    Queue<TreeNode<T>*> queue;
    queue.enqueue(root);

    while(!queue.isEmpty()){
        TreeNode<T>* temp = queue.peekData();
        queue.dequeueForTree();

        if(!temp->left){
            TreeNode<T>* newNode = new TreeNode<T>(value);
            temp->left = newNode;
            newNode->parent = temp;
            return newNode;
        } else queue.enqueue(temp->left);

        if(!temp->right){
            TreeNode<T>* newNode = new TreeNode<T>(value);
            temp->right = newNode;
            newNode->parent = temp;
            return newNode;
        } else queue.enqueue(temp->right);
    }

    return NULL;
}

	TreeNode<T>* lastNode(){
		if(!root) return NULL;
		Queue<TreeNode<T>*> queue;
		queue.enqueue(root);
		TreeNode<T> *temp;
		while(!queue.isEmpty()){
			temp=queue.peekData();
			queue.dequeueForTree();
			if(temp->left) queue.enqueue(temp->left);
			if(temp->right)	queue.enqueue(temp->right);
		}
		return temp;
	}
	TreeNode<T>* search(T find,TreeNode<T>* temp){
		if(temp==NULL) return NULL;
		if(temp->data==find) return temp;
		 TreeNode<T>* leftResult = search(find, temp->left);  // search left
    	if (leftResult) return leftResult;                    // found in left
    	return search(find, temp->right); 					//else search on right
	}
};
//creating a min heap bcz we use heap for tasks given priority on the basis of deadline
template <typename T>
class Heap{
	public:
	CompleteBT<T> heap;
	void heapifyUp(TreeNode<T>* temp){
		if(!temp || !temp->parent) return;
		if (temp->data < temp->parent->data) {  // Min-heap property violated
        swap(temp->data, temp->parent->data);
        heapifyUp(temp->parent);            // Recur on parent
    }
}
	void heapifyDown(TreeNode<T>* temp){
		if(!temp) return;
		TreeNode<T>* smallest = temp;
	    if (temp->left && temp->left->data < smallest->data) smallest = temp->left;
	    if (temp->right && temp->right->data < smallest->data) smallest = temp->right;
	    // Swap parent with largest child
	    if (smallest != temp) {
	        swap(temp->data, smallest->data);
	        heapifyDown(smallest);
	    }
	}
	void insert(T temp){
		TreeNode<T>* newNode=heap.insertNode(temp);
		heapifyUp(newNode);
	}
	void insert(TreeNode<T>* temp){
		heap.insertNode(temp);
		heapifyUp(temp);
	}
	TreeNode<T>* search(T* find,TreeNode<T>* temp){
		if(temp==NULL) return NULL;
		if(temp->data==find) return temp;
		 TreeNode<T>* leftResult = search(find, temp->left);  // search left
    	if (leftResult) return leftResult;                    // found in left
    	return search(find, temp->right); 					//else search on right
	}
	template <typename Predicate>
	TreeNode<T>* search(TreeNode<T>* node, Predicate pred) {
	    if (!node) return NULL;
	    if (pred(node->data)) return node;          // match found
	    TreeNode<T>* leftResult = search(node->left, pred);
	    if (leftResult) return leftResult;
	    return search(node->right, pred);
	}
	TreeNode<T>* search(TreeNode<T>* node,int id) {
	    if (!node) return NULL;
	    if (node->data.getID()==id) return node;          // match found
	    TreeNode<T>* leftResult = search(node->left, id);
	    if (leftResult) return leftResult;
	    return search(node->right, id);
	}
	void deleteNode(T *find){
	    TreeNode<T>* tobeDeleted = heap.search(find, heap.root);
	    if(!tobeDeleted){
	        cout << "Node does not exist\n";
	        return;
	    }
	    TreeNode<T>* last = heap.lastNode();
	    if(tobeDeleted == last){ // deleting the last node
	        if(last->parent){
	            if(last->parent->left == last) last->parent->left = NULL;
	            else last->parent->right = NULL;
	        } else {
	            heap.root = NULL; // tree becomes empty
	        }
	        delete last;
	        return;
	    }
	    // Swap values
	    swap(tobeDeleted->data, last->data);
	    // Detach last node from its parent
	    if(last->parent->left == last) last->parent->left = NULL;
	    else last->parent->right = NULL;
	    delete last;
	    // Heapify from swapped node
	    heapifyDown(tobeDeleted);
	}
	void deleteNode(T find){
	    TreeNode<T>* tobeDeleted = heap.search(find, heap.root);
	    if(!tobeDeleted){
	        cout << "Node does not exist\n";
	        return;
	    }
	    TreeNode<T>* last = heap.lastNode();
	    if(tobeDeleted == last){ // deleting the last node
	        if(last->parent){
	            if(last->parent->left == last) last->parent->left = NULL;
	            else last->parent->right = NULL;
	        } else {
	            heap.root = NULL; // tree becomes empty
	        }
	        delete last;
	        return;
	    }
	    // Swap values
	    swap(tobeDeleted->data, last->data);
	    // Detach last node from its parent
	    if(last->parent->left == last) last->parent->left = NULL;
	    else last->parent->right = NULL;
	    delete last;
	    // Heapify from swapped node
	    heapifyDown(tobeDeleted);
	}
	void display(){
	    if(!heap.root){
	        cout << "Heap is empty.\n";
	        return;
	    }
	
	    Queue<TreeNode<T>*> queue;
	    queue.enqueue(heap.root);
	    //cout << "Heap elements (level-order): ";
	
	    while(!queue.isEmpty()){
	        TreeNode<T>* temp = queue.peekData();
	        queue.dequeueForTree();
	        temp->getData()->display();
	        if(temp->left) queue.enqueue(temp->left);
	        if(temp->right) queue.enqueue(temp->right);
	    }
	
	    cout << endl;
	}
	TreeNode<T>* getMin(){
		if(!heap.root){
			cout << "Heap is empty." << endl;
        	return NULL;
		}
		return heap.root;
	}
};

//int main(){
//    Heap<int> h;
//
//    cout << "Inserting values into heap...\n";
//    h.insert(10);
//    h.insert(5);
//    h.insert(20);
//    h.insert(2);
//    h.insert(15);
//
//    h.display();
//
//    cout << "\nDeleting node 5...\n";
//    h.deleteNode(5);
//    h.display();
//
//    cout << "\nDeleting node 10...\n";
//    h.deleteNode(10);
//    h.display();
//
//    cout << "\nInserting 1...\n";
//    h.insert(1);
//    h.display();
//
//    return 0;
//}
