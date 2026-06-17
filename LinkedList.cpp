#include<iostream>
#include<vector>
using namespace std;
template <typename T>
class SinglyNode{
	private:
	T Data;
	SinglyNode<T> *next;
	public:
	SinglyNode<T>(T n){
		this->Data=n;
		this->next=NULL;
	}
	SinglyNode<T>(const SinglyNode<T>& other){
		this->Data=other.Data;
		this->next=NULL;
	}
	SinglyNode<T>& operator=(const SinglyNode<T>& other){
		if(&other==this) return *this;
		this->Data=other.Data;
		this->next=NULL;
	}
	void display(){
		getData().display();
	}
	T& getData() {
    	return Data;
	}
	void setData(const T &value) {
    	Data = value;
	}
	
	SinglyNode<T>* getNext() {
	    return next;
	}
	
	void setNext(SinglyNode<T> *n) {
	    next = n;
	}
};
template <typename T>
class SinglyLinkedList{
	public:
	SinglyNode<T> *head;
	SinglyLinkedList(){
		head=NULL;
	}
	int countNodes(int count,SinglyNode<T> *curr){
		if(curr==NULL){
			return count;
		}
		return countNodes(++count,curr->getNext());
	}
	void Reverse(SinglyNode<T> *curr,SinglyNode<T> *prev,SinglyNode<T> *next){
		if(curr==NULL) return;
		//next=curr->next;
		curr->setNext(prev);
		//prev=curr;
		//curr=next;
		Reverse(next,curr,curr->getNext());
	}
	SinglyNode<T>* getNthNode(int n,SinglyNode<T> *temp){
		if(!temp) return NULL;
		if(n==0) return temp;
		return getNthNode(--n,temp->getNext());
	}
	void deleteNthNode(int n,SinglyNode<T> *temp,SinglyNode<T> *prev){
		if(temp==NULL) return;
		if(n==0){
			if(prev==NULL){
				head=temp->getNext();
				delete temp;
				return;
			}
			else {
            	prev->setNext(temp->getNext());
        	}
			delete temp;
			return;
		}
		else return deleteNthNode(--n,temp->getNext(),temp);
	}

	void addLast(SinglyNode<T> *newNode){
		if(head==NULL){
			head=newNode;
			return;
		}
		SinglyNode<T> *temp=head;
		while(temp->getNext()!=NULL){
			temp=temp->getNext();
		}
		temp->setNext(newNode);
	}
	void addLast(T n){
		SinglyNode<T> *newNode=new SinglyNode<T>(n);
		if(head==NULL){
			head=newNode;
			return;
		}
		SinglyNode<T> *temp=head;
		while(temp->getNext()!=NULL){
			temp=temp->getNext();
		}
		temp->setNext(newNode);
	}
	void addFirst(SinglyNode<T> *newNode){
		if(head==NULL){
			head=newNode;
			return;
		}
		newNode->setNext(head);
		head=newNode;
	}
	void addAfterPos(T n, int pos){
		SinglyNode<T>* newNode = new SinglyNode<T>(n);
		if(head==NULL){
			head=newNode;//insert at head
		}
		SinglyNode<T> *curr=head;
		while(curr!=NULL){
			if(pos==0){
				newNode->setNext(curr->getNext());
				curr->setNext(newNode);
			}
			--pos;
			curr=curr->getNext();
		}
	}
	void deleteFirst(){
		if(head==NULL) return;
		SinglyNode<T> *temp=head->getNext();
		delete head;
		head=temp;
	}
	void deleteLast(){
		if(head==NULL){
			return;
		}
		if(head->getNext()==NULL) {
			delete head;
			head=NULL;
		}
		SinglyNode<T> *temp=head->getNext();
		SinglyNode<T> *prev=head;
		while(temp->getNext()!=NULL){
			prev=temp;
			temp=temp->getNext();
		}
		prev->setNext(NULL);
		delete temp;
	}
	void deleteAfterPos(int pos){
		if(head==NULL){
			return;
		}
		if(head->getNext()==NULL){
			if(pos!=0) return;
			delete head;
			head=NULL;
		}
		SinglyNode<T> *curr=head->getNext();
		SinglyNode<T> *prev=head;
		while(curr!=NULL){
			if(pos==0){
				prev->setNext(curr->getNext());
				delete curr;
				return;
			}
			pos--;
			prev=curr;
			curr=curr->getNext();
		}
	}
	void deleteByValue(T Data){
		if(head==NULL) return;
			SinglyNode<T> *temp=head;//check head
			if(temp->getData()==Data){
				head=head->getNext();
				delete temp;
				return;
			}
		//check rest of list
		SinglyNode<T> *curr=head->getNext();
		SinglyNode<T> *prev=head;
		while(curr!=NULL){
			if(curr->getData()==Data){
				prev->setNext(curr->getNext());
				delete curr;
				break;
			}
			prev=curr;
			curr=curr->getNext();
		}
		return;
	}
	SinglyNode<T>* searchByID(const string &id) {
	    SinglyNode<T> *curr = head;
	    while (curr != NULL) {
	        if (curr->getData().getId() == id) {
	            return curr;
	        }
	        curr = curr->getNext();
	    }
	    return NULL;
	}
	void display(){
		if(head==NULL){
			cout<<"List is empty"<<endl;
			return;
		}
		SinglyNode<T> *temp=head;
		while(temp!=NULL){
			temp->display();
			temp=temp->getNext();
		}
	}
	T searchById(const string &id) {
	    SinglyNode<T>* curr = head;
	    while(curr != NULL) {
	        if(curr->getData().getId() == id) {
	            return curr->getData();  // return object by value
	        }
	        curr = curr->getNext();
	    }
	    return T();  // return default-constructed object if not found
	}
	
};
