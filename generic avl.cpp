#include<iostream>
using namespace std;

template<typename T>
class Node {
public:
    T data;
    int height;
    Node *left, *right;
    Node(T data) {
        this->data = data;
        left = right = NULL;
        height = 0;
    }
};

template<typename T>
class AVL {
public:
    Node<T>* root;
    AVL() { root = NULL; }

    Node<T>* insert(Node<T>* newNode, Node<T>* temp) {
        if (temp == NULL) return newNode;
        if (newNode->data < temp->data) temp->left = insert(newNode, temp->left);
        else temp->right = insert(newNode, temp->right);
        //Balancing
        temp->height = 1 + max(height(temp->left), height(temp->right));
        int balance = getBalance(temp);
        if (balance > 1) {
            if (newNode->data < temp->left->data) return rotateRight(temp);//LL Case  //rotate right
            else if (newNode->data > temp->left->data) {//LR Case
                temp->left = rotateLeft(temp->left);//left pe rotate left then temp ko rotate right
                return rotateRight(temp);
            }
        }
        if (balance < -1) {
            if (newNode->data > temp->right->data) return rotateLeft(temp);
            else if (newNode->data < temp->right->data) {
                temp->right = rotateRight(temp->right);
                return rotateLeft(temp);
            }
        }
        return temp;
    }

    int getBalance(Node<T>* node) {
        if (node == NULL) return 0;
        return height(node->left) - height(node->right);
    }

    int height(Node<T>* head) {
        if (head == NULL) return -1;
        int h_left = height(head->left);
        int h_right = height(head->right);
        return (h_left > h_right) ? (h_left + 1) : (h_right + 1);
    }

    Node<T>* rotateRight(Node<T>* x) {
        if (x == NULL || x->left == NULL) return x;
        Node<T>* y = x->left;
        Node<T>* temp = y->right;
        y->right = x;
        x->left = temp;
        x->height = 1 + max(height(x->left), height(x->right));
        y->height = 1 + max(height(y->left), height(y->right));
        return y;
    }

    Node<T>* rotateLeft(Node<T>* x) {
        if (x == NULL || x->right == NULL) return x;
        Node<T>* y = x->right;
        Node<T>* temp = y->left;
        y->left = x;
        x->right = temp;
        x->height = 1 + max(height(x->left), height(x->right));
        y->height = 1 + max(height(y->left), height(y->right));
        return y;
    }

    void deleteNeitherNode(Node<T>* deleted) {
        Node<T>* parent = findParent(deleted, root);
        if (parent == NULL) {//no parent means root delete ho rhi 
            root = (deleted->left != NULL) ? deleted->left : deleted->right;
            delete deleted;
            return;
        }
        Node<T>* child = deleted->left != NULL ? deleted->left : deleted->right;
        if (parent->right == deleted) parent->right = child;
        else parent->left = child;
        delete deleted;
    }

    void deleteFullNode(Node<T>* tobeDeleted) {
        Node<T>* pre = predecessor(tobeDeleted);
        swap(tobeDeleted->data, pre->data);
        if (tobeDeleted->left == NULL && tobeDeleted->right == NULL) delete tobeDeleted;
        else deleteNeitherNode(tobeDeleted);
    }

    Node<T>* deleteNode(Node<T>* temp, T find) {
        if (temp == NULL) return temp;
        if (temp->data > find) temp->left = deleteNode(temp->left, find);
        else if (find > temp->data) temp->right = deleteNode(temp->right, find);
        else {
            if (temp->left == NULL || temp->right == NULL) deleteNeitherNode(temp);
            else deleteFullNode(temp);
            return temp;
        }
        //balancing
        temp->height = 1 + max(height(temp->left), height(temp->right));
        int balance = getBalance(temp);
        if (balance > 1 && getBalance(temp->left) >= 0) return rotateRight(temp);
        if (balance > 1 && getBalance(temp->left) < 0) {
            temp->left = rotateLeft(temp->left);
            return rotateRight(temp);
        }
        if (balance < -1 && getBalance(temp->right) <= 0) return rotateLeft(temp);
        if (balance < -1 && getBalance(temp->right) > 0) {
            temp->right = rotateRight(temp->right);
            return rotateLeft(temp);
        }
        return temp;
    }

    Node<T>* findMax(Node<T>* root) {
        if (root == NULL) return NULL;
        if (root->right == NULL) return root;
        return findMax(root->right);
    }

    Node<T>* findMin(Node<T>* root) {
        if (root == NULL) return NULL;
        if (root->left == NULL) return root;
        return findMin(root->left);
    }

    void inorder(Node<T>* root) {
        if (root == NULL) return;
        inorder(root->left);
        cout << root->data << " -> ";
        inorder(root->right);
    }

    void preorder(Node<T>* root) {
        if (root == NULL) return;
        cout << root->data << " -> ";
        preorder(root->left);
        preorder(root->right);
    }

    void postorder(Node<T>* root) {
        if (root == NULL) return;
        postorder(root->left);
        postorder(root->right);
        cout << root->data << " -> ";
    }

    Node<T>* search(T find, Node<T>* temp) {
        if (temp == NULL) return NULL;
        if (find == temp->data) return temp;
        if (find < temp->data) return search(find, temp->left);
        else return search(find, temp->right);
    }

    Node<T>* predecessor(Node<T>* temp) {
        if (temp == NULL || temp->left == NULL) return NULL;
        Node<T>* leftchild = temp->left;
        while (leftchild->right != NULL) leftchild = leftchild->right;
        return leftchild;
    }

    Node<T>* successor(Node<T>* temp) {
        if (temp == NULL || temp->right == NULL) return NULL;
        Node<T>* rightchild = temp->right;
        while (rightchild->left != NULL) rightchild = rightchild->left;
        return rightchild;
    }

    Node<T>* findParent(Node<T>* temp, Node<T>* root) {
        if (root == NULL || temp == NULL) return NULL;
        if (root->left == temp || root->right == temp) return root;
        if (temp->data < root->data) return findParent(temp, root->left);
        else return findParent(temp, root->right);
    }
};

