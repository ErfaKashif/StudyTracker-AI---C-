#include<iostream>
using namespace std;
template <typename T>
class Node {
    public:
        T data;
        Node* next;
        Node(T data) {
            this->data = data;
            next = NULL;
        }
    };
template <typename T>
class Queue {
    Node<T>* front;
    Node<T>* rear;
    int size;
public:
    Queue() {
        front = rear = NULL;
        size=0;
    }
    bool isEmpty() {
        return front == NULL;
    }
    void enqueue(T value) {
        Node<T>* newNode = new Node<T>(value);
        size++;
        if (rear == NULL) {
            front = rear = newNode;
            return;
        }
        rear->next = newNode;
        rear = newNode;
    }
    void enqueue(Node<T> *node) {
        Node<T>* newNode = node;
        size++;
        if (rear == NULL) {
            front = rear = newNode;
            return;
        }
        rear->next = newNode;
        rear = newNode;
    }
    void dequeue() {
        if (isEmpty()) return;
        Node<T>* temp = front;
        front = front->next;
        if (front == NULL) rear = NULL;
        size--;
        delete temp;
    }
    void dequeueForTree() {
    	size--;
        if (isEmpty()) return;
        Node<T>* temp = front;
        front = front->next;
        if (front == NULL) rear = NULL;
    }
    T peekData() {
        if (isEmpty()) cout<<"queue is empty";
        return front->data;
    }
    const T& peekData() const { // const version
        return front->getData();
    }
    Node<T>* peekNode() {
        if (isEmpty()) cout<<"queue is empty";
        return front;
    }
    int getSize(){
    	return size;
	}
};
template <typename T>
class CircularQueue{
	public:
	T *arr;
	int front;
	int rear;
	int size;
	CircularQueue(){
	arr=new T[5];
	front=0;
	rear=0;
	size=5;
	}
	void enqueue(T data){
		if((rear + 1) % size == front){
			front = (front + 1) % size;//front is moved one step ahead so that the prev front gets rewritten by the new value and becomes the rear
		}
		else{
			arr[rear]=data;
			rear=(rear+1)%size;
		}
		cout<<data<<" inserted\n";
	}
	void dequeue(){
		if(front==rear) {
			cout<<"Playlist is empty"<<endl;	
		}
		else{
			cout<<" "<<arr[front]<<" is deleted from the Playlist"<<endl;
			arr[front]="";
			front=(front+1)%size;	
		}
	}	
	void display() {
    if (front==rear) {
        cout<<"Playlist is empty!"<<endl;
        return;
    }
    cout<<"Playlist: ";
    int i=front;
    while(i!=rear){
        cout<<arr[i]<<" ";
        i=(i+1)%size;
    }
    cout<<endl;
}
};
//int main() {
//    CircularQueue<string> playlist;
//    playlist.enqueue("Song Manad");
//    playlist.enqueue("Song LOML");
//    playlist.enqueue("Song Blue");
//    playlist.enqueue("Song Wildflowers");
//    playlist.display();
//    playlist.dequeue();
//    playlist.display();
//    playlist.enqueue("Song Daylight");
//    playlist.enqueue("Song Cardigan");
//    playlist.display();
//    playlist.enqueue("Song The Great War");
//    playlist.display();
//    playlist.dequeue();
//    playlist.dequeue();
//    playlist.display();
//    playlist.enqueue("Song Arzoo");
//    playlist.enqueue("Song Delicate");
//    playlist.display();
//    return 0;
//}
