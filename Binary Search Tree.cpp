#include<iostream>
using namespace std;
template <typename T>
class Node{
	public:
	T data;
	Node<T> *left,*right;
	Node<T>(T data){
		this->data=data;
		left=right=NULL;
	}
};
template <typename T>
class BinarySearchTree{
	public:
	Node<T> *root;
		BinarySearchTree(){
			root=NULL;
		}
		Node<T>* insert(Node<T>* newNode, Node<T>* temp){
			if(temp==NULL) return newNode;
			if(newNode->data<temp->data) temp->left=insert(newNode,temp->left);//updating parents left and right
			else temp->right=insert(newNode,temp->right);
			return temp;
		}
		Node<T>* insert(T key, Node<T>* temp){
			if(temp==NULL) return new Node<T>(key);
			if(key<temp->data) temp->left=insert(key,temp->left);//updating parents left and right
			else temp->right=insert(key,temp->right);
			return temp;
		}
		Node<T>* search(int find, Node<T> *temp){
			if(temp==NULL) return NULL;
			if(find==temp->data) return temp;
			if(find<temp->data) return search(find,temp->left);
			else return search(find,temp->right);
		}
		Node<T>* predecessor(Node<T> *temp){
			if (temp == NULL || temp->left == NULL) return NULL;
			Node<T> *leftchild=temp->left;
			while(leftchild->right!=NULL){
				leftchild=leftchild->right;
			}
			return leftchild;
		}
		Node<T>* successor(Node<T> *temp){
			if (temp == NULL || temp->right == NULL) return NULL;
			Node<T> *rightchild=temp->right;
			while(rightchild->left!=NULL){
				rightchild=rightchild->left;
			}
			return rightchild;
		}
		Node<T>* findParent(Node<T> *temp,Node<T> *root){
			if(root==NULL || temp==NULL ) return NULL;
			if(root->left==temp || root->right==temp ) return root;
			if(temp->data<root->data) return findParent(temp,root->left);
			else return findParent(temp,root->right);
		}
		void deleteNeitherNode(Node<T> *deleted){
			Node<T> *parent=findParent(deleted,root);
			if(parent == NULL) { //to delete leaf or neither root Node<T> safely
			    root = (deleted->left != NULL) ? deleted->left : deleted->right;
			    delete deleted;
			    return;
			}
			Node<T> *child = deleted->left!=NULL? deleted->left:deleted->right;
			if(parent->right==deleted) parent->right=child;
			else parent->left=child;
			delete deleted;
			return;
		}
		void deleteFullNode(Node<T> *tobeDeleted){
			Node<T> *pre=predecessor(tobeDeleted);
			swap(tobeDeleted->data,pre->data);
			//use delete lead or neither depending on the swapped Node<T>
			if(tobeDeleted->left==NULL && tobeDeleted->right==NULL) delete tobeDeleted;
			else deleteNeitherNode(tobeDeleted);
		}
		
		bool deleteNode(int find,Node<T> *temp){
			Node<T> *tobeDeleted=search(find,temp);
			if(tobeDeleted){
				if(tobeDeleted->left==NULL || tobeDeleted->right==NULL) deleteNeitherNode(tobeDeleted);
				else deleteFullNode(tobeDeleted);
				return true;
			}
			return false;
		}
		Node<T>* findMax(Node<T> *root){
			if(root==NULL) return NULL;
			if(root->right==NULL) return root;
			return findMax(root->right);
		}
		Node<T>* findMin(Node<T> *root){
			if(root==NULL) return NULL;
			if(root->left==NULL) return root;
			return findMin(root->left);
		}
		void inorder(Node<T> *root){
			if(root==NULL) return;
			inorder(root->left);
			cout<<root->data<<" -> ";
			inorder(root->right);
		}
		void preorder(Node<T> *root){
			if(root==NULL) return;
			cout<<root->data<<" -> ";
			preorder(root->left);
			preorder(root->right);
		}
		void postorder(Node<T> *root){
			if(root==NULL) return;
			postorder(root->left);
			postorder(root->right);
			cout<<root->data<<" -> ";
		}
		int height(Node<T> *head){
		if(head == NULL) return -1;
		else{
			int h_left= height(head->left);
			int h_right = height (head ->right);
			if (h_left > h_right)
			
			return (h_left + 1);
			else return ( h_right + 1);
		}
}
};
int main() {
    BinarySearchTree<int> bst;

    // Inserting nodes
    bst.root = bst.insert(new Node<int>(8), bst.root);           //8
    bst.root = bst.insert(new Node<int>(3), bst.root);    //3             10
    bst.root = bst.insert(new Node<int>(10), bst.root);//1     6              14
    bst.root = bst.insert(new Node<int>(1), bst.root);//    4    7         13
    bst.root = bst.insert(new Node<int>(6), bst.root);
    bst.root = bst.insert(new Node<int>(14), bst.root);
    bst.root = bst.insert(new Node<int>(4), bst.root);
    bst.root = bst.insert(new Node<int>(7), bst.root);
    bst.root = bst.insert(new Node<int>(13), bst.root);
	cout<<"Height of bst "<<bst.height(bst.root)<<endl;
    cout << "\nInorder traversal before deletion:\n";
    bst.inorder(bst.root);//1,3,4,6,7,8,10,13,14
    cout << "\n\nPreorder traversal:\n";
    bst.preorder(bst.root);//8,3,1,6,4,7,10,14,13
    cout << "\n\nPostorder traversal:\n";
    bst.postorder(bst.root);//1,4,7,6,3,13,14,10,8

    cout << "\n\nDeleting leaf Node<T> 1...\n";
    bst.deleteNode(1, bst.root);
	cout<<"Height of bst "<<bst.height(bst.root)<<endl;
    cout << "Deleting Node<T> with one child (14)...\n";
    bst.deleteNode(14, bst.root);
	cout<<"Height of bst "<<bst.height(bst.root)<<endl;
    cout << "Deleting full Node<T> (3)...\n";
    bst.deleteNode(3, bst.root);
	cout<<"Height of bst "<<bst.height(bst.root)<<endl;
    cout << "\n\nInorder traversal after deletions:\n";
    bst.inorder(bst.root);
    cout << endl;
    return 0;
}

