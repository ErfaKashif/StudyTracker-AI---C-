#include<iostream>
using namespace std;
template <typename T>
class Node{
	private:
	T data;
	Node *next;
	public:
	Node(){
		this->next=NULL;
	}
	Node(T data){
		this->data=data;
		this->next=NULL;
	}
	T getData(){
		return data;
	}
	Node<T>* getNext(){
		return next;
	}
	void setNext(Node *next){
		this->next=next;
	}
	void display(){
		getData().display();
	}
};
//maing stack from linked list
template <typename T>
class Stack{
	private:
	Node<T> *head;
	int size;
	public:
	Stack(){
		head=NULL;
		size=0;
	}
	bool isEmpty(){
		if(head==NULL) return true;
		return false;
	}
	void push(T c){
		Node<T> *newnode=new Node<T>(c);
		size++;
		if(isEmpty()){
			head=newnode;
			return;
		}
		newnode->setNext(head);
		head=newnode;
	}
	void pop(){
		if(isEmpty()){
			cout << "Stack is empty, cannot pop!" << endl;
			return;
		}
		Node<T> *temp=head->getNext();
		delete head;
		size--;
		head=temp;
	}
	Node<T>* topNode(){
		if(isEmpty()){
			return NULL;
		}
		return head;
	}
	void reverse(){
		Stack reversed;
		while(!this->isEmpty()){
			reversed.push(this->topData());
			pop();
		}
		while(!reversed.isEmpty()){
			this->push(reversed.topData());
			reversed.pop();
		}
	}
	T topData() {
        if (isEmpty()) {
            cout << "Stack is empty!" << endl;
            return T(); // return default value of T
        }
        return head->getData();
    }
    int getSize() {
        return size;
    }
    void display() {
        if (isEmpty()) {
            cout << "Stack is empty!" << endl;
            return;
        }
        Node<T>* curr = head;
        cout << "Stack (Top to Bottom): ";
        while (curr != NULL) {
        	curr->display();
            curr = curr->getNext();
        }
        cout << endl;
    }
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};




