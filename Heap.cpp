#include <iostream>
#include <vector>
#include <algorithm> 
using namespace std;

template <typename T>
class Heap {
private:
    vector<T> a;
    void heapifyUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (a[i] < a[parent]) {      
                swap(a[i], a[parent]);
                i = parent;
            } else {
                break;
            }
        }
    }
    void heapifyDown(int i) {
        int n = static_cast<int>(a.size());
        while (true) {
            int left  = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            if (left < n && a[left] < a[smallest])  smallest = left;
            if (right < n && a[right] < a[smallest]) smallest = right;

            if (smallest != i) {
                swap(a[i], a[smallest]);
                i = smallest;
            } else {
                break;
            }
        }
    }

public:
    Heap() {}

    bool isEmpty() const { return a.empty(); }

    int size() const { return static_cast<int>(a.size()); }
    void insert(const T& value) {
        a.push_back(value);                   
        heapifyUp(static_cast<int>(a.size()) - 1);
    }
    T* getMin() {
        if (a.empty()) {
            cout << "Heap is empty." << endl;
            return NULL;
        }
        return &a[0];
    }

    const T* getMin() const {
        if (a.empty()) {
            cout << "Heap is empty." << endl;
            return NULL;
        }
        return &a[0];
    }
    bool deleteMin() {
        if (a.empty()) return false;
        if (a.size() == 1) {
            a.pop_back();
            return true;
        }

        swap(a[0], a.back());
        a.pop_back();
        heapifyDown(0);
        return true;
    }
    bool deleteNode(const T& value) {
        int n = static_cast<int>(a.size());
        int idx = -1;
        for (int i = 0; i < n; ++i) {
            if (a[i] == value) {
                idx = i;
                break;
            }
        }

        if (idx == -1) {
            cout << "Node does not exist\n";
            return false;
        }
        swap(a[idx], a[n - 1]);
        a.pop_back();
        if (idx >= static_cast<int>(a.size()))
            return true;
        heapifyUp(idx);
        heapifyDown(idx);
        return true;
    }
    template <typename Predicate>
    int findIndex(Predicate pred) const {
        for (int i = 0; i < static_cast<int>(a.size()); ++i) {
            if (pred(a[i])) return i;
        }
        return -1;
    }
    void display() const {
        if (a.empty()) {
            cout << "Heap is empty.\n";
            return;
        }

        cout << "Heap elements (level-order / array order): ";
        for (int i = 0; i < static_cast<int>(a.size()); ++i) {
            cout << a[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    Heap<int> h;

    cout << "Inserting values into heap...\n";
    h.insert(10);
    h.insert(5);
    h.insert(20);
    h.insert(2);
    h.insert(15);

    h.display();

    cout << "\nDeleting node 5...\n";
    h.deleteNode(5);
    h.display();

    cout << "\nDeleting node 10...\n";
    h.deleteNode(10);
    h.display();

    cout << "\nInserting 1...\n";
    h.insert(1);
    h.display();

    cout << "\nDeleting min...\n";
    h.deleteMin();
    h.display();

    return 0;
}
