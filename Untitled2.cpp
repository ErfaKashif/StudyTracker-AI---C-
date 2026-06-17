#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>
#include <cctype>
#include <sstream>
#include <climits>
#include <fstream>
#include <algorithm>

// Platform-specific includes for password masking
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

using namespace std;

// ============ CONSOLE UI HELPERS ============
namespace ConsoleUI {
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void setColor(int color) {
        #ifdef _WIN32
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, color);
        #endif
    }
    
    // Windows colors: 1=Blue, 2=Green, 3=Cyan, 4=Red, 6=Yellow, 7=White, 9=Light Blue, 14=Light Yellow
    enum Color {
        WHITE = 7,
        CYAN = 11,
        GREEN = 10,
        YELLOW = 14,
        RED = 12,
        BLUE = 9
    };
    
    void printHeader(const string& title) {
        setColor(CYAN);
        cout << "\n+----------------------------------------------------------+\n";
        cout << "¦";
        setColor(YELLOW);
        int padding = (54 - title.length()) / 2;
        for(int i = 0; i < padding; i++) cout << " ";
        cout << title;
        for(int i = 0; i < 54 - padding - title.length(); i++) cout << " ";
        setColor(CYAN);
        cout << "¦\n";
        cout << "+----------------------------------------------------------+\n";
        setColor(WHITE);
    }
    
    void printSuccess(const string& msg) {
        setColor(GREEN);
        cout << "? " << msg << endl;
        setColor(WHITE);
    }
    
    void printError(const string& msg) {
        setColor(RED);
        cout << "? " << msg << endl;
        setColor(WHITE);
    }
    
    void printInfo(const string& msg) {
        setColor(CYAN);
        cout << "? " << msg << endl;
        setColor(WHITE);
    }
    
    string getPassword(const string& prompt) {
        string password = "";
        cout << prompt;
        
        #ifdef _WIN32
            char ch;
            while((ch = _getch()) != '\r') { // Enter key
                if(ch == '\b') { // Backspace
                    if(!password.empty()) {
                    	password.erase(password.length() - 1);
                        //password.pop_back();
                        cout << "\b \b";
                    }
                } else {
                    password += ch;
                    cout << '*';
                }
            }
        #else
            struct termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            
            char ch;
            while((ch = getchar()) != '\n') {
                if(ch == 127 || ch == 8) { // Backspace
                    if(!password.empty()) {
                        password.pop_back();
                        cout << "\b \b";
                    }
                } else {
                    password += ch;
                    cout << '*';
                }
            }
            
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        #endif
        
        cout << endl;
        return password;
    }
    
    void waitForEnter() {
        cout << "\nPress Enter to continue...";
        cin.ignore(10000, '\n');
        cin.get();
    }
}

// CUSTOM DATA STRUCTURES 
template <typename T>
class vector {
private:
    T* data;
    int capacity;
    int size;
    
    void resize() {
        capacity = (capacity == 0) ? 1 : capacity * 2;
        T* newData = new T[capacity];
        
        for(int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        
        delete[] data;
        data = newData;
    }
    
public:
    vector() {
        data = NULL;
        capacity = 0;
        size = 0;
    }
    
    ~vector() {
        if(data != NULL) {
            delete[] data;
        }
    }
    
    vector(const vector<T>& other) {
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for(int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    
    vector<T>& operator=(const vector<T>& other) {
        if(this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for(int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    
    void push_back(const T& value) {
        if(size == capacity) {
            resize();
        }
        data[size++] = value;
    }
    
    void pop_back() {
        if(size > 0) {
            size--;
        }
    }
    
    T& operator[](int index) {
        return data[index];
    }
    
    const T& operator[](int index) const {
        return data[index];
    }
    
    int getSize() const {
        return size;
    }
    
    int getCapacity() const {
        return capacity;
    }
    
    bool empty() const {
        return size == 0;
    }
    
    void clear() {
        size = 0;
    }
    
    T& front() {
        return data[0];
    }
    
    T& back() {
        return data[size - 1];
    }
    
    const T& front() const {
        return data[0];
    }
    
    const T& back() const {
        return data[size - 1];
    }
};

// ============ STACK (for Focus Sessions) ============
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

template <typename T>
class Stack {
private:
    Node<T>* head;
    int size;
    
public:
    Stack(){
        head=NULL;
        size=0;
    }
    
    ~Stack() { clear(); }
    
    bool isEmpty() const { return head == NULL; }
    
    void push(const T &c){
        Node<T>* newnode = new Node<T>(c);
        ++size;
        newnode->setNext(head);
        head = newnode;
    }
    
    void pop(){
        if(isEmpty()){
            return;
        }
        Node<T>* temp = head->getNext();
        delete head;
        head = temp;
        --size;
    }
    
    Node<T>* topNode() const { return head; }
    
    T topData() const {
        if(isEmpty()){
            return T();
        }
        return head->getData();
    }
    
    int getSize() const { return size; }
    
    void display() const {
        if(isEmpty()){
            cout<<"No sessions yet"<<endl;
            return;
        }
        Node<T>* curr = head;
        while(curr){
            curr->display();
            curr=curr->getNext();
        }
    }
    
    void clear(){
        while(!isEmpty()) pop();
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

// ============ SINGLY LINKED LIST ============
template <typename T>
class SinglyNode {
private:
    T Data;
    SinglyNode<T>* next;
    
public:
    SinglyNode(T n): Data(n), next(NULL) {}
    ~SinglyNode() {}
    
    void display(){ Data.display(); }
    T& getData(){ return Data; }
    void setData(const T &value){ Data = value; }
    SinglyNode<T>* getNext(){ return next; }
    void setNext(SinglyNode<T>* n){ next = n; }
};

template <typename T>
class SinglyLinkedList {
public:
    SinglyNode<T>* head;
    
    SinglyLinkedList(): head(NULL) {}
    ~SinglyLinkedList() { clear(); }
    
    void clear() {
        while(head) {
            deleteFirst();
        }
    }
    
    int countNodes(int count, SinglyNode<T>* curr) const {
        if(curr == NULL) return count;
        return countNodes(count+1, curr->getNext());
    }
    
    void addLast(const T &n){
        try {
            SinglyNode<T>* newNode = new SinglyNode<T>(n);
            if(!head){
                head = newNode;
                return;
            }
            SinglyNode<T>* temp = head;
            while(temp->getNext() != NULL) temp = temp->getNext();
            temp->setNext(newNode);
        } catch (const bad_alloc& e) {
            cerr<<"Memory allocation failed"<<endl;
        }
    }
    
    void deleteFirst(){
        if(!head) return;
        SinglyNode<T>* temp = head->getNext();
        delete head;
        head = temp;
    }
    
    void deleteByValue(const T &Data){
        if(!head) return;
        SinglyNode<T>* temp=head;
        if(temp->getData()==Data){
            head=head->getNext();
            delete temp;
            return;
        }
        SinglyNode<T>* curr = head->getNext();
        SinglyNode<T>* prev = head;
        while(curr){
            if(curr->getData() == Data){
                prev->setNext(curr->getNext());
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->getNext();
        }
    }
    
    void display()const{
        if(!head){
            cout<<"List is empty"<<endl;
            return;
        }
        SinglyNode<T>* temp=head;
        while(temp){
            temp->display();
            temp=temp->getNext();
        }
    }
    
    T searchById(const string &id)const{
        SinglyNode<T>* curr=head;
        while(curr){
            if(curr->getData().getId() == id) return curr->getData();
            curr = curr->getNext();
        }
        return T();
    }
};

// ============ LIGHTWEIGHT USER INFO FOR GRAPH ============
struct UserInfo {
    string userId;
    string username;
    int score;
    
    UserInfo() : score(0) {}
    UserInfo(string uid, string uname, int sc = 0) 
        : userId(uid), username(uname), score(sc) {}
    
    string getUserId() const { return userId; }
    string getUsername() const { return username; }
    int getScore() const { return score; }
};

// Forward declarations
class UserManager;
class GroupStudyManager;

// ============ FOCUS SESSION ============
class FocusSession{
private:
    string sessionId;
    long elapsedMs;
    bool isPaused;
    time_t startTime, pausedAt;
    
public:
    FocusSession(){
        elapsedMs = 0;
        isPaused = false;
        startTime = time(0);
    }
    
    FocusSession(string sessionIdP){
        sessionId = sessionIdP;
        elapsedMs = 0;
        isPaused = false;
        startTime = time(0);
    }
    
    bool pause(){
        if(!isPaused){
            isPaused = true;
            pausedAt = time(0);
            return true;
        }
        return false;
    }
    
    bool resume(){
        if(isPaused){
            isPaused = false;
            elapsedMs += static_cast<long>(difftime(time(0), pausedAt));
            startTime = time(0);
            return true;
        }
        return false;
    }
    
    long getElapsed() const {
        if(isPaused) return elapsedMs;
        return elapsedMs + static_cast<long>(difftime(time(0), startTime));
    }
    
    string getId() const { return sessionId; }
    
    void display() const {
        cout << "Session: " << sessionId << ", Elapsed: " << getElapsed() << "s" << endl;
    }
    
    string serialize() const {
        stringstream ss;
        ss << sessionId << "|" << elapsedMs << "|" << isPaused;
        return ss.str();
    }

    static FocusSession deserialize(const string& line) {
        stringstream ss(line);
        string id;
        long elapsed;
        bool paused;
        
        getline(ss, id, '|');
        ss >> elapsed;
        ss.ignore(1, '|');
        ss >> paused;
        
        FocusSession fs(id);
        fs.elapsedMs = elapsed;
        fs.isPaused = paused;
        fs.startTime = time(0);
        return fs;
    }
};

// ============ FOCUS SESSION HISTORY ============
class FocusSessionHistory{
    Stack<FocusSession> focusSessions;
    
public:
    void addSession(){
        string topic;
        cout<<"Enter the topic for your focus session: ";
        getline(cin, topic);
        
        if(topic.empty()) {
            ConsoleUI::printError("Topic cannot be empty.");
            return;
        }
        
        string id = generateSessionId(topic);
        focusSessions.push(FocusSession(id));
        ConsoleUI::printSuccess("Focus session '" + topic + "' started! ID: " + id);
    }
    
    void addSession(const FocusSession& fs) {
        focusSessions.push(fs);
    }
    
    const Stack<FocusSession>& getSessions() const { return focusSessions; }
    Stack<FocusSession>& getSessions() { return focusSessions; }
    
    void pauseCurrentSession(){
        if(focusSessions.isEmpty()) {
            ConsoleUI::printError("No active session to pause.");
            return;
        }
        
        if(focusSessions.topData().pause()) {
            ConsoleUI::printSuccess("Session paused. Take a break!");
        } else {
            ConsoleUI::printInfo("Session is already paused.");
        }
    }
    
    void resumeCurrentSession(){
        if(focusSessions.isEmpty()) {
            ConsoleUI::printError("No session to resume.");
            return;
        }
        
        if(focusSessions.topData().resume()) {
            ConsoleUI::printSuccess("Welcome back! Session resumed.");
        } else {
            ConsoleUI::printInfo("Session is already active.");
        }
    }
    
    void showSummary(){
        if(focusSessions.isEmpty()) {
            ConsoleUI::printError("No active session.");
            return;
        }
        ConsoleUI::printHeader("Session Summary");
        cout<<"Session ID: "<<focusSessions.topData().getId()<<endl;
        cout<<"Duration: "<<focusSessions.topData().getElapsed()<<" seconds"<<endl;
    }
    
    void displayHistory(){
        if(focusSessions.isEmpty()) {
            ConsoleUI::printInfo("No session history available.");
            return;
        }
        ConsoleUI::printHeader("Focus Session History");
        focusSessions.display();
    }
    
    void clearHistory(){
        focusSessions.clear();
        ConsoleUI::printSuccess("Session history cleared.");
    }
    
    int sessionCount(){
        int count = focusSessions.getSize();
        if(count > 0) {
        	stringstream message_stream;
			message_stream << "You've completed " << count << " focus sessions!";
			string final_message = message_stream.str();
            ConsoleUI::printSuccess(final_message);
        } else {
            ConsoleUI::printInfo("No focus sessions yet.");
        }
        return count;
    }
    
    int getTotalStudyTime(){
        Node<FocusSession>* curr = focusSessions.topNode();
        int total = 0;
        
        while(curr){
            total += curr->getData().getElapsed();
            curr = curr->getNext();
        }
        
        if(total>0) {
        	stringstream message_stream;
			message_stream << "Total study time: %d"<<total<<" seconds!";
			std::string final_message = message_stream.str();
            ConsoleUI::printSuccess(final_message);
        } else {
            ConsoleUI::printInfo("No study time recorded yet.");
        }
        return total;
    }
    
private:
    string generateSessionId(const string &topic) {
        static bool seeded = false;
        if(!seeded) {
            srand(time(0));
            seeded = true;
        }
        
        string initials = "";
        bool foundFirst = false;
        
        for(size_t i = 0; i < topic.size(); i++) {
            if(isalpha(topic[i])) {
                initials += toupper(topic[i]);
                foundFirst = true;
                break;
            }
        }
        
        if(foundFirst) {
            for(size_t i = 0; i < topic.size(); i++) {
                if(topic[i] == ' ' && i + 1 < topic.size() && isalpha(topic[i+1])) {
                    initials += toupper(topic[i+1]);
                    break;
                }
            }
        }
        
        if(initials.size() == 1) {
            initials += initials[0];
        }
        if(initials.size() == 0) {
            initials = "FS";
        }
        
        string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        string suffix = "";
        for(int i = 0; i < 3; i++) {
            suffix += chars[rand() % chars.size()];
        }
        return initials + suffix;
    }
};

// ============ FLASHCARD ============
class Flashcard{
private:
    string id, question, answer, sourceChunkId, topic;
    
public:
    Flashcard(){}
    
    Flashcard(string idP, string questionP, string answerP, string sourceChunkIdP, string topicP){
        id = idP; question = questionP; answer = answerP; sourceChunkId = sourceChunkIdP; topic = topicP;
    }
    
    bool operator==(const Flashcard &other) const {
        return (question==other.question) && (answer==other.answer) && (sourceChunkId==other.sourceChunkId) && (topic==other.topic);
    }
    
    string getId() const { return id; }
    void setId(const string &idP) { id=idP; }
    string getQuestion() const { return question; }
    void setQuestion(const string &q) { question=q; }
    string getAnswer() const { return answer; }
    void setAnswer(const string &a) { answer=a; }
    string getSourceChunkId() const { return sourceChunkId; }
    void setSourceChunkId(const string &s) { sourceChunkId=s; }
    string getTopic() const { return topic; }
    void setTopic(const string &t) { topic=t; }
    
    void display() const {
        cout << "+-----------------------------------------+\n";
        cout << "¦ Flashcard: " << id << "\n";
        cout << "¦ Topic: " << topic << "\n";
        cout << "¦ Q: " << question << "\n";
        cout << "¦ A: " << answer << "\n";
        cout << "+-----------------------------------------+\n";
    }
    
    string serialize() const {
        return id + "|" + topic + "|" + question + "|" + answer + "|" + sourceChunkId;
    }
    
    static Flashcard deserialize(const string& line) {
        stringstream ss(line);
        string id, topic, question, answer, sourceChunkId;
        getline(ss, id, '|');
        getline(ss, topic, '|');
        getline(ss, question, '|');
        getline(ss, answer, '|');
        getline(ss, sourceChunkId, '|');
        return Flashcard(id, question, answer, sourceChunkId, topic);
    }
};

// ============ FLASHCARD MANAGER ============
class FlashcardManager {
    SinglyLinkedList<Flashcard> flashcards;
    
public:
    ~FlashcardManager() {}
    
    SinglyLinkedList<Flashcard>& getFlashcards(){ return flashcards; }
    const SinglyLinkedList<Flashcard>& getFlashcards() const { return flashcards; }

    bool addFlashcard(const Flashcard &f){
        if(isDuplicate(f)){
            ConsoleUI::printError("Duplicate flashcard found.");
            return false;
        }
        flashcards.addLast(f);
        ConsoleUI::printSuccess("Flashcard added: " + f.getId());
        return true;
    }
    
    bool addFlashcardInteractive(){
        string topic, question, answer, sourceChunkId;
        cout << "Enter Topic: ";
        getline(cin, topic);
        if(topic.empty()){ ConsoleUI::printError("Topic empty. Aborting."); return false; }
        
        cout << "Enter Question: ";
        getline(cin, question);
        if(question.empty()){ ConsoleUI::printError("Question empty. Aborting."); return false; }
        
        cout << "Enter Answer: ";
        getline(cin, answer);
        if(answer.empty()){ ConsoleUI::printError("Answer empty. Aborting."); return false; }
        
        cout << "Enter Source Chunk ID (or press Enter for 'unknown'): ";
        getline(cin, sourceChunkId);
        if(sourceChunkId.empty()) sourceChunkId = "unknown";
        
        string id = generateId();
        Flashcard newCard(id, question, answer, sourceChunkId, topic);
        return addFlashcard(newCard);
    }

    void displayAllFlashcards() const { 
        if(!flashcards.head) {
            ConsoleUI::printInfo("No flashcards available.");
            return;
        }
        ConsoleUI::printHeader("Your Flashcards");
        flashcards.display(); 
    }
    
    SinglyLinkedList<Flashcard>& getList(){ return flashcards; }
    
    int getFlashcardCount() const {
        return flashcards.countNodes(0, flashcards.head);
    }
    
    void deleteFlashcard(const string id){
        Flashcard curr=flashcards.searchById(id);
        if(curr.getId()==id){
            flashcards.deleteByValue(curr);
            ConsoleUI::printSuccess("Flashcard deleted");
            return;
        }
        ConsoleUI::printError("Invalid ID!");
    }
    
private:
    bool isDuplicate(const Flashcard &f) const {
        SinglyNode<Flashcard>* curr = flashcards.head;
        while(curr){
            if(curr->getData() == f) return true;
            curr = curr->getNext();
        }
        return false;
    }
    
    string generateId() {
        time_t t = time(0);
        stringstream ss;
        ss << "F" << t << (rand()%1000);
        return ss.str();
    }
};

// ============ QUIZ ============
class Quiz{
    Queue<Flashcard> quizQuestions;
    int count;
    int score;
    
public:
    Quiz(){ 
        score = 0; 
        count = 0;
    }
    
    void createQuiz(const SinglyLinkedList<Flashcard> &flashcards){
        quizQuestions.clear();
        count = 0;
        score = 0;
        
        SinglyNode<Flashcard>* curr = flashcards.head;
        while(curr){
            quizQuestions.enqueue(curr->getData());
            curr = curr->getNext();
            ++count;
        }
        if(count == 0) {
            ConsoleUI::printError("No flashcards for quiz");
        } else {
        	stringstream message_stream;
			message_stream << "Quiz created with %d" << count << " questions";
			string final_message = message_stream.str();
            ConsoleUI::printSuccess(final_message);
        }
    }
    
    void conductQuiz(){
        if(quizQuestions.isEmpty()){
            ConsoleUI::printError("No questions to conduct.");
            return;
        }
        
        ConsoleUI::printHeader("QUIZ TIME!");
        int questionNum = 1;
        
        while(!quizQuestions.isEmpty()){
            Flashcard question = quizQuestions.peekData();
            string ans;
            
            cout << "\n????????????????????????????????????????\n";
            cout << "Question " << questionNum << "/" << count << ":\n";
            ConsoleUI::setColor(ConsoleUI::YELLOW);
            cout << question.getQuestion() << endl;
            ConsoleUI::setColor(ConsoleUI::WHITE);
            cout << "Your answer: ";
            getline(cin, ans);
            
            if(submitAnswer(question, ans)) {
                ++score;
                ConsoleUI::printSuccess("Correct!");
            } else {
                ConsoleUI::printError("Incorrect. Answer: " + question.getAnswer());
            }
            
            quizQuestions.dequeue();
            questionNum++;
        }
        
        ConsoleUI::printHeader("QUIZ COMPLETED");
        cout << "Score: " << score << "/" << count << " (" 
             << (count > 0 ? (score * 100 / count) : 0) << "%)\n";
    }
    
    bool submitAnswer(const Flashcard &f, const string &ans) const {
        string userAns = ans;
        string correctAns = f.getAnswer();
        transform(userAns.begin(), userAns.end(), userAns.begin(), ::tolower);
        transform(correctAns.begin(), correctAns.end(), correctAns.begin(), ::tolower);
        return userAns == correctAns;
    }
    
    int getScore() const { return score; }
    int getCount() const { return count; }
};

// ============ QUIZ MANAGER (Hash Table) ============
class QuizManager {
    static const int TABLE_SIZE = 17;
    SinglyLinkedList<Flashcard> hashTable[TABLE_SIZE];
    
public:
    void insertFlashcard(const Flashcard &f){
        int idx = hashFunction(f.getTopic(), TABLE_SIZE);
        hashTable[idx].addLast(f);
    }
    
    SinglyLinkedList<Flashcard> getFlashcardsByTopic(const string &topic) {
        SinglyLinkedList<Flashcard> result;
        int index = hashFunction(topic, TABLE_SIZE);
        SinglyNode<Flashcard>* curr = hashTable[index].head;
        
        while(curr){
            if(curr->getData().getTopic() == topic){
                result.addLast(curr->getData());
            }
            curr = curr->getNext();
        }
        return result;
    }
    
    void displayTopics() const {
        ConsoleUI::printHeader("Available Topics");
        bool foundTopics = false;
        
        for(int i = 0; i < TABLE_SIZE; i++){
            SinglyNode<Flashcard>* curr = hashTable[i].head;
            while(curr){
                cout << "• " << curr->getData().getTopic() << endl;
                curr = curr->getNext();
                foundTopics = true;
            }
        }
        
        if(!foundTopics) {
            ConsoleUI::printInfo("No topics available. Add flashcards first!");
        }
    }
    
private:
    int hashFunction(const string &key, int tableSize) const {
        long hash = 0;
        int p = 31;
        for(size_t i = 0; i < key.length(); i++){
            hash = (hash * p + key[i]) % tableSize;
        }
        return static_cast<int>(hash);
    }
};

// ============ TASK (Min-Heap for To-Do) ============
// (Keeping your original tree-based heap implementation for Tasks)
// You can optionally convert this to vector-based if you want O(log n)

enum TaskStatus { PENDING, IN_PROGRESS, COMPLETED };

class Task {
private:
    int id;
    string title;
    string description;
    time_t deadline;
    TaskStatus status;

public:
    Task() {
        id = -1;
        title = "";
        description = "";
        deadline = 0;
        status = PENDING;
    }
    
    Task(const string& t, const string& d) {
        id = -1;
        title = t;
        description = d;
        setDeadlineFromInput();
        status = PENDING;
    }
    
    void setID(int i) { id = i; }
    int getID() const { return id; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    time_t getDeadline() const { return deadline; }
    TaskStatus getStatus() const { return status; }
    void setTitle(const string& t) { title = t; }
    void setDescription(const string& d) { description = d; }
    void setDeadline(time_t dl){ deadline=dl; }
    void setStatus(TaskStatus s){ status=s; }
    
    void setDeadlineFromInput() {
        string input;
        int year, month, day;
        char dash1, dash2;
        while (true) {
            cout << "Enter deadline (YYYY-MM-DD) or press Enter to skip: ";
            getline(cin, input);
    
            if (input.empty()) {
                deadline = 0;
                break;
            }
            stringstream ss(input);
            ss >> year >> dash1 >> month >> dash2 >> day;
    
            if (ss.fail() || dash1 != '-' || dash2 != '-') {
                ConsoleUI::printError("Invalid format! Try again.");
                continue;
            }
            if (!isValidDate(year, month, day)) {
                ConsoleUI::printError("Invalid date! Try again.");
                continue;
            }
    
            time_t t = convertToTimeT(year, month, day);
            if (!isTodayOrFuture(t)) {
                ConsoleUI::printError("Deadline cannot be in the past!");
                continue;
            }
    
            deadline = t;
            break;
        }
    }
    
    void markCompleted() { status = COMPLETED; }
    void markInProgress() { status = IN_PROGRESS; }
    
    bool isLate() const {
        if (deadline == 0) return false;
        time_t now = time(0);
        return difftime(now, deadline) > 0;
    }
    
    bool operator<(const Task& other) const {
        time_t d1 = (deadline == 0 ? LLONG_MAX : deadline);
        time_t d2 = (other.deadline == 0 ? LLONG_MAX : other.deadline);
        return d1 < d2;
    }
    
    bool operator==(const Task& other) const {
        return (deadline==other.getDeadline()) && (title==other.getTitle());
    }
    
    void display() const {
        cout << "\n+-----------------------------------------+\n";
        cout << "¦ Task " << id << "\n";
        cout << "¦ Title: " << title << "\n";
        cout << "¦ Description: " << description << "\n";
        cout << "¦ Deadline: " << formatDeadline(deadline) << "\n";
        cout << "¦ Status: ";
        if (status == PENDING) cout << "Pending";
        else if (status == IN_PROGRESS) cout << "In Progress";
        else cout << "Completed";
        if (isLate() && status != COMPLETED) cout << " (LATE!)";
        cout << "\n+-----------------------------------------+\n";
    }

    string serialize() const {
        stringstream ss;
        ss << id << "|" << title << "|" << description << "|" << deadline << "|" << static_cast<int>(status);
        return ss.str();
    }
    
    static Task deserialize(const string& line) {
        stringstream ss(line);
        int id, statusInt;
        string title, desc;
        time_t dl;
        char sep;
        
        ss >> id >> sep;
        getline(ss, title, '|');
        getline(ss, desc, '|');
        ss >> dl >> sep >> statusInt;
        
        Task t;
        t.setID(id);
        t.setTitle(title);
        t.setDescription(desc);
        t.setDeadline(dl);
        t.setStatus(static_cast<TaskStatus>(statusInt));
        return t;
    }

private:
    static string formatDeadline(time_t t) {
        if (t == 0) return "N/A";
        struct tm* d = localtime(&t);
        char buffer[11];
        sprintf(buffer, "%04d-%02d-%02d", d->tm_year + 1900, d->tm_mon + 1, d->tm_mday);
        return string(buffer);
    }
    
    bool isLeap(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }
    
    bool isValidDate(int year, int month, int day) {
        if (year < 1970 || month < 1 || month > 12 || day < 1) return false;
        int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
        if (month == 2 && isLeap(year)) return day <= 29;
        return day <= daysInMonth[month - 1];
    }
    
    time_t convertToTimeT(int year, int month, int day) {
        struct tm t = {0};
        t.tm_year = year - 1900;
        t.tm_mon = month - 1;
        t.tm_mday = day;
        t.tm_hour = 23;
        t.tm_min = 59;
        t.tm_sec = 0;
        return mktime(&t);
    }
    
    bool isTodayOrFuture(time_t t) {
        time_t now = time(NULL);
        return difftime(t, now) >= 0;
    }
};

// ============ TREE-BASED HEAP (Your original implementation) ============
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

template <typename T>
class CompleteBT{
public:
    TreeNode<T> *root;
    
    CompleteBT<T>(){
        root=NULL;
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
            queue.dequeue();

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
        TreeNode<T> *temp = NULL;
        
        while(!queue.isEmpty()){
            temp=queue.peekData();
            queue.dequeue();
            if(temp->left) queue.enqueue(temp->left);
            if(temp->right) queue.enqueue(temp->right);
        }
        return temp;
    }
    
    TreeNode<T>* search(T find,TreeNode<T>* temp){
        if(temp==NULL) return NULL;
        if(temp->data==find) return temp;
        TreeNode<T>* leftResult = search(find, temp->left);
        if (leftResult) return leftResult;
        return search(find, temp->right);
    }
};

template <typename T>
class Heap{
public:
    CompleteBT<T> heap;
    
    void heapifyUp(TreeNode<T>* temp){
        if(!temp || !temp->parent) return;
        if (temp->data < temp->parent->data) {
            swap(temp->data, temp->parent->data);
            heapifyUp(temp->parent);
        }
    }
    
    void heapifyDown(TreeNode<T>* temp){
        if(!temp) return;
        TreeNode<T>* smallest = temp;
        if (temp->left && temp->left->data < smallest->data) smallest = temp->left;
        if (temp->right && temp->right->data < smallest->data) smallest = temp->right;
        
        if (smallest != temp) {
            swap(temp->data, smallest->data);
            heapifyDown(smallest);
        }
    }
    
    void insert(T temp){
        TreeNode<T>* newNode=heap.insertNode(temp);
        heapifyUp(newNode);
    }
    
    TreeNode<T>* search(TreeNode<T>* node,int id) {
        if (!node) return NULL;
        if (node->data.getID()==id) return node;
        TreeNode<T>* leftResult = search(node->left, id);
        if (leftResult) return leftResult;
        return search(node->right, id);
    }
    
    void deleteNode(TreeNode<T>* node) {
        if (!node) return;
        TreeNode<T>* last = heap.lastNode();
        if (node == last) {
            if (last->parent) {
                if (last->parent->left == last) last->parent->left = NULL;
                else last->parent->right = NULL;
            } else {
                heap.root = NULL;
            }
            delete last;
            return;
        }
        swap(node->data, last->data);
        if (last->parent->left == last) last->parent->left = NULL;
        else last->parent->right = NULL;
        delete last;

        if (node->parent && node->data < node->parent->data)
            heapifyUp(node);
        else
            heapifyDown(node);
    }
    
    void display(){
        if(!heap.root){
            cout << "No tasks yet.\n";
            return;
        }
    
        Queue<TreeNode<T>*> queue;
        queue.enqueue(heap.root);
        while(!queue.isEmpty()){
            TreeNode<T>* temp = queue.peekData();
            queue.dequeue();
            temp->getData()->display();
            if(temp->left) queue.enqueue(temp->left);
            if(temp->right) queue.enqueue(temp->right);
        }
    }
    
    TreeNode<T>* getMin(){
        if(!heap.root){
            cout << "Heap is empty." << endl;
            return NULL;
        }
        return heap.root;
    }
};

// ============ TO-DO LIST ============
class ToDoList{
private:
    Heap<Task> todolist;
    static int count;
    
public:
    void addTask() {
        ConsoleUI::printHeader("Create New Task");
        string title, description;
        cout << "Enter task title: ";
        getline(cin, title);
        cout << "Enter task description: ";
        getline(cin, description);
        Task newTask(title, description);
        newTask.setID(count++);
        todolist.insert(newTask);
        ConsoleUI::printSuccess("Task added!");
    }
    
    void displayList(){
        ConsoleUI::printHeader("Your To-Do List");
        todolist.display();
    }
    
    void taskCompleted(){
        int id=-1;
        cout<<"Enter the ID of the completed task: ";
        cin>>id;
        cin.ignore();
        
        TreeNode<Task>* taskNode = todolist.search(todolist.getMin(),id);
        if (!taskNode) {
            ConsoleUI::printError("Task not found!");
            return;
        }
        taskNode->getData()->markCompleted();
        ConsoleUI::printSuccess("Task marked as completed!");
        taskNode->getData()->display();
        todolist.deleteNode(taskNode);
    }
    
    void deleteTask(){
        int id=-1;
        cout<<"Enter task ID to delete: ";
        cin>>id;
        cin.ignore();
        
        TreeNode<Task>* taskNode = todolist.search(todolist.getMin(),id);
        if (!taskNode) {
            ConsoleUI::printError("Task not found!");
            return;
        }
        taskNode->getData()->display();
        todolist.deleteNode(taskNode);
        ConsoleUI::printSuccess("Task deleted!");
    }
    
    void closestDeadline(){
        if(!todolist.getMin()) {
            ConsoleUI::printInfo("No tasks available.");
            return;
        }
        cout<<"Task #"<<todolist.getMin()->getData()->getID()<<" has the closest deadline:\n";
        todolist.getMin()->getData()->display();
    }
    
    static int getCount() { return count; }
    static void setCount(int c) { count = c; }
    
    void addTaskFromFile(const Task& task) {
        todolist.insert(task);
        if(task.getID() >= count) {
            count = task.getID() + 1;
        }
    }
    
    void getAllTasks(SinglyLinkedList<Task>& taskList) const {
        if(!todolist.heap.root) return;
        Queue<TreeNode<Task>*> queue;
        queue.enqueue(todolist.heap.root);
        while(!queue.isEmpty()) {
            TreeNode<Task>* temp = queue.peekData();
            queue.dequeue();
            taskList.addLast(temp->getDataObj());
            
            if(temp->left) queue.enqueue(temp->left);
            if(temp->right) queue.enqueue(temp->right);
        }
    }
};

int ToDoList::count = 1;

// ============ USER AVL TREE ============
class UserAVLNode {
public:
    string username;
    string userId;
    int height;
    UserAVLNode *left, *right;
    
    UserAVLNode(string uname, string uid) {
        username = uname;
        userId = uid;
        left = right = NULL;
        height = 0;
    }
};

class UserAVL {
public:
    UserAVLNode* root;
    
    UserAVL() {
        root = NULL;
    }
    
    int height(UserAVLNode* node) {
        if(node == NULL) return -1;
        return node->height;
    }
    
    int getBalance(UserAVLNode* node) {
        if(node == NULL) return 0;
        return height(node->left) - height(node->right);
    }
    
    UserAVLNode* rotateRight(UserAVLNode* x) {
        if(x == NULL || x->left == NULL) return x;
        UserAVLNode* y = x->left;
        UserAVLNode* temp = y->right;
        y->right = x;
        x->left = temp;
        x->height = 1 + max(height(x->left), height(x->right));
        y->height = 1 + max(height(y->left), height(y->right));
        return y;
    }
    
    UserAVLNode* rotateLeft(UserAVLNode* x) {
        if(x == NULL || x->right == NULL) return x;
        UserAVLNode* y = x->right;
        UserAVLNode* temp = y->left;
        y->left = x;
        x->right = temp;
        x->height = 1 + max(height(x->left), height(x->right));
        y->height = 1 + max(height(y->left), height(y->right));
        return y;
    }
    
    UserAVLNode* insert(UserAVLNode* node, string username, string userId) {
        if(node == NULL) return new UserAVLNode(username, userId);
        
        if(username < node->username)
            node->left = insert(node->left, username, userId);
        else if(username > node->username)
            node->right = insert(node->right, username, userId);
        else
            return node;
        
        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);
        
        if(balance > 1 && username < node->left->username)
            return rotateRight(node);
        if(balance < -1 && username > node->right->username)
            return rotateLeft(node);
        if(balance > 1 && username > node->left->username) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if(balance < -1 && username < node->right->username) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }
    
    UserAVLNode* search(UserAVLNode* node, string username) {
        if(node == NULL) return NULL;
        if(username == node->username) return node;
        if(username < node->username) return search(node->left, username);
        return search(node->right, username);
    }
    
    bool usernameExists(string username) {
        return search(root, username) != NULL;
    }
    
    string getUserId(string username) {
        UserAVLNode* node = search(root, username);
        if(node) return node->userId;
        return "";
    }
};

// ============ LEADERBOARD (Max-Heap) ============
class LeaderboardEntry {
public:
    string username;
    int score;
    
    LeaderboardEntry() : username(""), score(0) {}
    LeaderboardEntry(string uname, int sc) : username(uname), score(sc) {}
    
    bool operator>(const LeaderboardEntry& other) const {
        return score > other.score;
    }
    
    bool operator<(const LeaderboardEntry& other) const {
        return score < other.score;
    }
    
    bool operator==(const LeaderboardEntry& other) const {
        return username == other.username;
    }
    
    void display() const {
        cout << username << ": " << score << " points";
    }
};

class LeaderboardHeap {
private:
    TreeNode<LeaderboardEntry>* root;
    
    TreeNode<LeaderboardEntry>* insertNode(LeaderboardEntry value) {
        if(!root) {
            root = new TreeNode<LeaderboardEntry>(value);
            return root;
        }

        Queue<TreeNode<LeaderboardEntry>*> queue;
        queue.enqueue(root);

        while(!queue.isEmpty()) {
            TreeNode<LeaderboardEntry>* temp = queue.peekData();
            queue.dequeue();

            if(!temp->left) {
                TreeNode<LeaderboardEntry>* newNode = new TreeNode<LeaderboardEntry>(value);
                temp->left = newNode;
                newNode->parent = temp;
                return newNode;
            } else queue.enqueue(temp->left);

            if(!temp->right) {
                TreeNode<LeaderboardEntry>* newNode = new TreeNode<LeaderboardEntry>(value);
                temp->right = newNode;
                newNode->parent = temp;
                return newNode;
            } else queue.enqueue(temp->right);
        }
        return NULL;
    }
    
    void heapifyUp(TreeNode<LeaderboardEntry>* temp) {
        if(!temp || !temp->parent) return;
        if(temp->data > temp->parent->data) {
            swap(temp->data, temp->parent->data);
            heapifyUp(temp->parent);
        }
    }
    
    void heapifyDown(TreeNode<LeaderboardEntry>* temp) {
        if(!temp) return;
        TreeNode<LeaderboardEntry>* largest = temp;
        if(temp->left && temp->left->data > largest->data) largest = temp->left;
        if(temp->right && temp->right->data > largest->data) largest = temp->right;
        
        if(largest != temp) {
            swap(temp->data, largest->data);
            heapifyDown(largest);
        }
    }
    
    TreeNode<LeaderboardEntry>* searchByUsername(TreeNode<LeaderboardEntry>* node, string username) {
        if(!node) return NULL;
        if(node->data.username == username) return node;
        
        TreeNode<LeaderboardEntry>* leftResult = searchByUsername(node->left, username);
        if(leftResult) return leftResult;
        return searchByUsername(node->right, username);
    }
    
public:
    LeaderboardHeap() : root(NULL) {}
    
    void insert(string username, int score) {
        TreeNode<LeaderboardEntry>* existing = searchByUsername(root, username);
        
        if(existing) {
            existing->data.score = score;
            heapifyUp(existing);
            heapifyDown(existing);
            return;
        }
        
        TreeNode<LeaderboardEntry>* newNode = insertNode(LeaderboardEntry(username, score));
        heapifyUp(newNode);
    }
    
    int getUserRank(string username) {
        if(!root) return -1;
        
        Queue<TreeNode<LeaderboardEntry>*> queue;
        SinglyLinkedList<LeaderboardEntry> entries;
        queue.enqueue(root);
        
        while(!queue.isEmpty()) {
            TreeNode<LeaderboardEntry>* temp = queue.peekData();
            queue.dequeue();
            entries.addLast(temp->data);
            if(temp->left) queue.enqueue(temp->left);
            if(temp->right) queue.enqueue(temp->right);
        }
        
        int rank = 1;
        SinglyNode<LeaderboardEntry>* curr=entries.head;
        int userScore = -1;
        
        while(curr) {
            if(curr->getData().username == username) {
                userScore = curr->getData().score;
                break;
            }
            curr = curr->getNext();
        }
        
        if(userScore == -1) return -1;
        curr = entries.head;
        
        while(curr) {
            if(curr->getData().score > userScore) {
                rank++;
            }
            curr = curr->getNext();
        }
        return rank;
    }
    
    void displayTop(int n = 10) {
        if(!root) {
            ConsoleUI::printInfo("No leaderboard entries yet.");
            return;
        }
        stringstream message_stream;
		message_stream << "TOP %d" << n << " LEADERBOARD";
		string final_message = message_stream.str();
        ConsoleUI::printHeader(final_message);
        vector<LeaderboardEntry> sortedEntries;
        Queue<TreeNode<LeaderboardEntry>*> queue;
        queue.enqueue(root);
        
        while(!queue.isEmpty()) {
            TreeNode<LeaderboardEntry>* temp = queue.peekData();
            queue.dequeue();
            sortedEntries.push_back(temp->data);
            if(temp->left) queue.enqueue(temp->left);
            if(temp->right) queue.enqueue(temp->right);
        }
        
        heapSort(sortedEntries);
        int displayCount = (n < sortedEntries.getSize()) ? n : sortedEntries.getSize();
        
        for(int i = 0; i < displayCount; i++) {
            ConsoleUI::setColor(i == 0 ? ConsoleUI::YELLOW : (i == 1 ? ConsoleUI::CYAN : ConsoleUI::WHITE));
            cout << i + 1 << ". ";
            sortedEntries[i].display();
            cout << endl;
        }
        ConsoleUI::setColor(ConsoleUI::WHITE);
    }

private:
    void heapSort(vector<LeaderboardEntry>& arr) {
        int n = arr.getSize();
        for(int i = n / 2 - 1; i >= 0; i--)
            heapifyArray(arr, n, i);
        for(int i = n - 1; i > 0; i--) {
            LeaderboardEntry temp = arr[0];
            arr[0] = arr[i];
            arr[i] = temp;
            heapifyArray(arr, i, 0);
        }
        for(int i = 0; i < n / 2; i++) {
            LeaderboardEntry temp = arr[i];
            arr[i] = arr[n - 1 - i];
            arr[n - 1 - i] = temp;
        }
    }
    
    void heapifyArray(vector<LeaderboardEntry>& arr, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        
        if(left < n && arr[left].score > arr[largest].score)
            largest = left;
        if(right < n && arr[right].score > arr[largest].score)
            largest = right;
        
        if(largest != i) {
            LeaderboardEntry temp = arr[i];
            arr[i] = arr[largest];
            arr[largest] = temp;
            heapifyArray(arr, n, largest);
        }
    }
};

// ============ MESSAGE ============
class Message {
private:
    string senderId;
    string senderUsername;
    string content;
    time_t timestamp;
    
public:
    Message() : timestamp(time(0)) {}
    
    Message(string sid, string suname, string msg) {
        senderId = sid;
        senderUsername = suname;
        content = msg;
        timestamp = time(0);
    }
    
    string getSenderId() const { return senderId; }
    string getSenderUsername() const { return senderUsername; }
    string getContent() const { return content; }
    time_t getTimestamp() const { return timestamp; }
    
    void display() const {
        struct tm* timeInfo = localtime(&timestamp);
        char buffer[20];
        strftime(buffer, 20, "%H:%M:%S", timeInfo);
        cout << "[" << buffer << "] " << senderUsername << ": " << content << endl;
    }
    
    string serialize() const {
        stringstream ss;
        ss << senderId << "|" << senderUsername << "|" << content << "|" << timestamp;
        return ss.str();
    }
    
    static Message deserialize(const string& line) {
        stringstream ss(line);
        string sid, suname, msg;
        time_t ts;
        
        getline(ss, sid, '|');
        getline(ss, suname, '|');
        getline(ss, msg, '|');
        ss >> ts;
        
        Message m(sid, suname, msg);
        return m;
    }
};

// ============ GRAPH FOR GROUP STUDY ============
template <typename T>
class GraphEdge {
public:
    T* connectedNode;
    string connectionTopic;
    GraphEdge<T>* next;
    
    GraphEdge(T* node, string topic) {
        connectedNode = node;
        connectionTopic = topic;
        next = NULL;
    }
};

template <typename T>
class GraphNode {
public:
    T data;
    GraphEdge<T>* edges;
    
    GraphNode(T d) : data(d), edges(NULL) {}
    
    void addEdge(GraphNode<T>* targetNode, string topic) {
        GraphEdge<T>* newEdge = new GraphEdge<T>(&(targetNode->data), topic);
        newEdge->next = edges;
        edges = newEdge;
    }
    
    bool hasConnectionWith(const string& userId, const string& topic) {
        GraphEdge<T>* curr = edges;
        while(curr) {
            if(curr->connectedNode->getUserId() == userId && 
               curr->connectionTopic == topic) {
                return true;
            }
            curr = curr->next;
        }
        return false;
    }
};

template <typename T>
class Graph {
private:
    vector<GraphNode<T>*> nodes;
    
public:
    Graph() {}
    
    ~Graph() {
        for(int i = 0; i < nodes.getSize(); i++) {
            GraphEdge<T>* curr = nodes[i]->edges;
            while(curr) {
                GraphEdge<T>* temp = curr;
                curr = curr->next;
                delete temp;
            }
            delete nodes[i];
        }
    }
    
    void addNode(const T& userData) {
        GraphNode<T>* newNode = new GraphNode<T>(userData);
        nodes.push_back(newNode);
    }
    
    void addConnection(const string& userId1, const string& userId2, const string& topic) {
        GraphNode<T>* node1 = findNode(userId1);
        GraphNode<T>* node2 = findNode(userId2);
        
        if(!node1 || !node2) return;
        if(node1->hasConnectionWith(userId2, topic)) return;
        
        node1->addEdge(node2, topic);
        node2->addEdge(node1, topic);
    }
    
    vector<T> getConnectedUsers(const string& userId, const string& topic) {
        vector<T> connected;
        GraphNode<T>* userNode = findNode(userId);
        
        if(!userNode) return connected;
        
        GraphEdge<T>* curr = userNode->edges;
        while(curr) {
            if(curr->connectionTopic == topic) {
                connected.push_back(*(curr->connectedNode));
            }
            curr = curr->next;
        }
        
        return connected;
    }
    
private:
    GraphNode<T>* findNode(const string& userId) {
        for(int i = 0; i < nodes.getSize(); i++) {
            if(nodes[i]->data.getUserId() == userId) {
                return nodes[i];
            }
        }
        return NULL;
    }
};

// ============ GROUP STUDY SESSION ============
class GroupStudySession {
private:
    string sessionId;
    string topic;
    string creatorUserId;
    time_t createdAt;
    SinglyLinkedList<Message> chatHistory;
    vector<string> memberUserIds;
    
public:
    GroupStudySession() : createdAt(time(0)) {}
    
    GroupStudySession(string creator, string t) {
        creatorUserId = creator;
        topic = t;
        createdAt = time(0);
        sessionId = generateSessionId();
        memberUserIds.push_back(creator);
    }
    
    string getSessionId() const { return sessionId; }
    string getTopic() const { return topic; }
    string getCreatorId() const { return creatorUserId; }
    time_t getCreatedAt() const { return createdAt; }
    
    void addMember(const string& userId) {
        for(int i = 0; i < memberUserIds.getSize(); i++) {
            if(memberUserIds[i] == userId) return;
        }
        memberUserIds.push_back(userId);
    }
    
    bool isMember(const string& userId) const {
        for(int i = 0; i < memberUserIds.getSize(); i++) {
            if(memberUserIds[i] == userId) return true;
        }
        return false;
    }
    
    void sendMessage(const string& senderId, const string& senderUsername, const string& msg) {
        Message newMsg(senderId, senderUsername, msg);
        chatHistory.addLast(newMsg);
    }
    
    void displayChat() const {
        if(!chatHistory.head) {
            ConsoleUI::printInfo("No messages yet. Start the conversation!");
            return;
        }
        
        ConsoleUI::printHeader("CHAT: " + topic);
        SinglyNode<Message>* curr = chatHistory.head;
        while(curr) {
            curr->getData().display();
            curr = curr->getNext();
        }
    }
    
    void displayMembers() const {
        cout << "\n--- Study Group Members (" << memberUserIds.getSize() << ") ---\n";
        for(int i = 0; i < memberUserIds.getSize(); i++) {
            cout << (i+1) << ". User ID: " << memberUserIds[i] << endl;
        }
    }
    
    int getMemberCount() const { return memberUserIds.getSize(); }
    const vector<string>& getMembers() const { return memberUserIds; }
    
    string serialize() const {
        stringstream ss;
        ss << sessionId << "|" << topic << "|" << creatorUserId << "|" << createdAt << "|";
        
        for(int i = 0; i < memberUserIds.getSize(); i++) {
            ss << memberUserIds[i];
            if(i < memberUserIds.getSize() - 1) ss << ",";
        }
        ss << "|";
        
        int msgCount = 0;
        SinglyNode<Message>* curr = chatHistory.head;
        while(curr) {
            msgCount++;
            curr = curr->getNext();
        }
        ss << msgCount;
        
        return ss.str();
    }
    
    static GroupStudySession deserialize(const string& line) {
        stringstream ss(line);
        string sid, topic, creator;
        time_t created;
        string membersStr;
        int msgCount;
        
        getline(ss, sid, '|');
        getline(ss, topic, '|');
        getline(ss, creator, '|');
        ss >> created;
        ss.ignore(1, '|');
        getline(ss, membersStr, '|');
        ss >> msgCount;
        
        GroupStudySession session(creator, topic);
        session.sessionId = sid;
        session.createdAt = created;
        
        stringstream memberStream(membersStr);
        string member;
        session.memberUserIds.clear();
        while(getline(memberStream, member, ',')) {
            session.memberUserIds.push_back(member);
        }
        
        return session;
    }
    
    SinglyLinkedList<Message>& getChatHistory() { return chatHistory; }
    const SinglyLinkedList<Message>& getChatHistory() const { return chatHistory; }
    
private:
    string generateSessionId() {
        time_t t = time(0);
        stringstream ss;
        ss << "GS" << t << (rand() % 10000);
        return ss.str();
    }
};

// ============ GROUP STUDY MANAGER ============
class GroupStudyManager {
private:
    Graph<UserInfo> userGraph; // NOW USES UserInfo INSTEAD OF User
    SinglyLinkedList<GroupStudySession> activeSessions;
    string sessionsFile;
    
public:
    GroupStudyManager() {
        sessionsFile = "group_sessions.txt";
    }
    
    void addUserToGraph(const UserInfo& userInfo) {
        userGraph.addNode(userInfo);
    }
    
    GroupStudySession* createSession(const string& creatorId, const string& creatorUsername, 
                                     const string& topic) {
        GroupStudySession newSession(creatorId, topic);
        activeSessions.addLast(newSession);
        
        ConsoleUI::printSuccess("Group study session created!");
        cout << "Session ID: " << newSession.getSessionId() << endl;
        cout << "Topic: " << topic << endl;
        cout << "Share this ID with others!\n";
        
        // Return pointer to the newly added session
        SinglyNode<GroupStudySession>* curr = activeSessions.head;
        while(curr) {
            if(curr->getData().getSessionId() == newSession.getSessionId()) {
                return &(curr->getData());
            }
            curr = curr->getNext();
        }
        return NULL;
    }
    
    bool joinSession(const string& sessionId, const string& userId, const string& username) {
        SinglyNode<GroupStudySession>* curr = activeSessions.head;
        
        while(curr) {
            if(curr->getData().getSessionId() == sessionId) {
                curr->getData().addMember(userId);
                
                const vector<string>& members = curr->getData().getMembers();
                for(int i = 0; i < members.getSize(); i++) {
                    if(members[i] != userId) {
                        userGraph.addConnection(userId, members[i], curr->getData().getTopic());
                    }
                }
                
                curr->getData().sendMessage("SYSTEM", "System", 
                    username + " joined!");
                
                ConsoleUI::printSuccess("You've joined: " + curr->getData().getTopic());
                return true;
            }
            curr = curr->getNext();
        }
        
        ConsoleUI::printError("Session not found.");
        return false;
    }
    
    bool sendMessage(const string& sessionId, const string& senderId, 
                    const string& senderUsername, const string& message) {
        SinglyNode<GroupStudySession>* curr = activeSessions.head;
        
        while(curr) {
            if(curr->getData().getSessionId() == sessionId) {
                if(!curr->getData().isMember(senderId)) {
                    ConsoleUI::printError("You are not a member of this session.");
                    return false;
                }
                
                curr->getData().sendMessage(senderId, senderUsername, message);
                return true;
            }
            curr = curr->getNext();
        }
        
        return false;
    }
    
    void viewChat(const string& sessionId, const string& userId) {
        SinglyNode<GroupStudySession>* curr = activeSessions.head;
        
        while(curr) {
            if(curr->getData().getSessionId() == sessionId) {
                if(!curr->getData().isMember(userId)) {
                    ConsoleUI::printError("You are not a member.");
                    return;
                }
                
                curr->getData().displayChat();
                return;
            }
            curr = curr->getNext();
        }
        
        ConsoleUI::printError("Session not found.");
    }
    
    void displayAllSessions() const {
        if(!activeSessions.head) {
            ConsoleUI::printInfo("No active study sessions.");
            return;
        }
        
        ConsoleUI::printHeader("ACTIVE STUDY SESSIONS");
        SinglyNode<GroupStudySession>* curr = activeSessions.head;
        int count = 1;
        
        while(curr) {
            cout << count << ". " << curr->getData().getTopic() << "\n";
            cout << "   ID: " << curr->getData().getSessionId() << "\n";
            cout << "   Members: " << curr->getData().getMemberCount() << "\n";
            cout << "   ----------------------\n";
            curr = curr->getNext();
            count++;
        }
    }
    
    void displayUserSessions(const string& userId) const {
        ConsoleUI::printHeader("YOUR STUDY SESSIONS");
        SinglyNode<GroupStudySession>* curr = activeSessions.head;
        bool found = false;
        int count = 1;
        
        while(curr) {
            if(curr->getData().isMember(userId)) {
                cout << count << ". " << curr->getData().getTopic() << "\n";
                cout << "   ID: " << curr->getData().getSessionId() << "\n";
                cout << "   Members: " << curr->getData().getMemberCount() << "\n";
                cout << "   ----------------------\n";
                found = true;
                count++;
            }
            curr = curr->getNext();
        }
        
        if(!found) {
            ConsoleUI::printInfo("You haven't joined any sessions yet.");
        }
    }
    
    bool saveSessions() {
        try {
            ofstream file(sessionsFile.c_str());
            if(!file.is_open()) return false;
            
            SinglyNode<GroupStudySession>* curr = activeSessions.head;
            while(curr) {
                file << "[SESSION]" << endl;
                file << curr->getData().serialize() << endl;
                
                file << "[MESSAGES]" << endl;
                SinglyNode<Message>* msgCurr = curr->getData().getChatHistory().head;
                while(msgCurr) {
                    file << msgCurr->getData().serialize() << endl;
                    msgCurr = msgCurr->getNext();
                }
                
                curr = curr->getNext();
            }
            
            file.close();
            return true;
            
        } catch(const exception& e) {
            return false;
        }
    }
    
    bool loadSessions() {
        try {
            ifstream file(sessionsFile.c_str());
            if(!file.is_open()) return true;
            
            string line;
            GroupStudySession* currentSession = NULL;
            
            while(getline(file, line)) {
                if(line.empty()) continue;
                
                if(line == "[SESSION]") {
                    getline(file, line);
                    GroupStudySession session = GroupStudySession::deserialize(line);
                    activeSessions.addLast(session);
                    
                    // Get pointer to newly added session
                    SinglyNode<GroupStudySession>* curr = activeSessions.head;
                    currentSession = &(curr->getData());
                }
                else if(line == "[MESSAGES]" && currentSession) {
                    while(getline(file, line) && !line.empty() && line != "[SESSION]") {
                        Message msg = Message::deserialize(line);
                        currentSession->getChatHistory().addLast(msg);
                    }
                    if(line == "[SESSION]") {
                        getline(file, line);
                        GroupStudySession session = GroupStudySession::deserialize(line);
                        activeSessions.addLast(session);
                        
                        SinglyNode<GroupStudySession>* curr = activeSessions.head;
                        currentSession = &(curr->getData());
                    }
                }
            }
            
            file.close();
            return true;
            
        } catch(const exception& e) {
            return false;
        }
    }
    
    void findStudyBuddies(const string& userId, const string& topic) {
        vector<UserInfo> connectedUsers = userGraph.getConnectedUsers(userId, topic);
        
        if(connectedUsers.getSize() == 0) {
            ConsoleUI::printInfo("No study buddies found for " + topic);
            return;
        }
        
        ConsoleUI::printHeader("STUDY BUDDIES: " + topic);
        for(int i = 0; i < connectedUsers.getSize(); i++) {
            cout << (i+1) << ". " << connectedUsers[i].getUsername() 
                 << " (Score: " << connectedUsers[i].getScore() << ")\n";
        }
    }
};

// ============ USER CLASS ============
class User {
private:
    string userId;
    string username;
    string password;
    int score;
    time_t lastActive;
    FocusSessionHistory focusSessions;
    FlashcardManager flashcards;
    QuizManager quizzes;
    ToDoList todoList;
    UserManager* userManagerPtr;
    GroupStudyManager* groupStudyMgr;
    
public:
    User(): score(0), lastActive(time(0)), userManagerPtr(NULL), groupStudyMgr(NULL) {}
    
    User(string uname, string pwd): username(uname), password(pwd), score(0), lastActive(time(0)), userManagerPtr(NULL), groupStudyMgr(NULL) {
        userId = generateUserId();
    }
    
    void setUserId(const string &u){ userId = u; }
    string getUserId() const { return userId; }
    void setUsername(const string &u){ username = u; }
    string getUsername() const { return username; }
    void setPassword(const string &p){ password = p; }
    string getPassword() const { return password; }
    int getScore() const { return score; }
    void setScore(int s) { score = s; }
    void setUserManager(UserManager* mgr) { userManagerPtr = mgr; }
    void setGroupStudyManager(GroupStudyManager* mgr) { groupStudyMgr = mgr; }
    
    void createQuizFromTopic(const string &topic);
    
    FlashcardManager& getFlashcardManager(){ return flashcards; }
    const FlashcardManager& getFlashcardManager() const { return flashcards; }
    QuizManager& getQuizManager(){ return quizzes; }
    const QuizManager& getQuizManager() const { return quizzes; }
    ToDoList& getToDoList() { return todoList; }
    const ToDoList& getToDoList() const { return todoList; }
    FocusSessionHistory& getFocusHistory() { return focusSessions; }
    const FocusSessionHistory& getFocusHistory() const { return focusSessions; }

    void addFlashcard(const Flashcard &f){
        if(flashcards.addFlashcard(f)){
            quizzes.insertFlashcard(f);
            lastActive = time(0);
        }
    }
    
    void addFlashcardInteractive() {
        if(flashcards.addFlashcardInteractive()){
            SinglyNode<Flashcard>* curr = flashcards.getList().head;
            while(curr){
                quizzes.insertFlashcard(curr->getData());
                curr = curr->getNext();
            }
            lastActive = time(0);
        }
    }
    
    void deleteFlashcards(const string &id){
        flashcards.deleteFlashcard(id);
    }
    
    void reviewFlashcards(){
        flashcards.displayAllFlashcards();
    }
    
    void startFocusSession() {
        focusSessions.addSession();
        lastActive = time(0);
    }
    
    void pauseFocusSession() {
        focusSessions.pauseCurrentSession();
        lastActive = time(0);
    }
    
    void resumeFocusSession() {
        focusSessions.resumeCurrentSession();
        lastActive = time(0);
    }
    
    void showFocusSummary() {
        focusSessions.showSummary();
    }
    
    void showFocusHistory() {
        focusSessions.displayHistory();
    }
    
    void clearFocusHistory() {
        focusSessions.clearHistory();
    }
    
    void showTotalStudyTime() {
        focusSessions.getTotalStudyTime();
    }
    
    void showNumberOfSessions() {
        focusSessions.sessionCount();
    }
    
    void addTask() {
        todoList.addTask();
        lastActive = time(0);
    }
    
    void viewTasks() {
        todoList.displayList();
    }
    
    void completeTask() {
        todoList.taskCompleted();
        lastActive = time(0);
    }
    
    void removeTask() {
        todoList.deleteTask();
        lastActive = time(0);
    }
    
    void viewClosestDeadline() {
        todoList.closestDeadline();
    }
    
    string serialize() const {
        stringstream ss;
        ss << username << "|" << password << "|" << userId << "|" << score;
        return ss.str();
    }
    
    static User deserialize(const string& line) {
        stringstream ss(line);
        string uname, pwd, uid;
        int sc;
        
        getline(ss, uname, '|');
        getline(ss, pwd, '|');
        getline(ss, uid, '|');
        ss >> sc;
        
        User u(uname, pwd);
        u.setUserId(uid);
        u.setScore(sc);
        return u;
    }
    
    void createGroupSession() {
        if(!groupStudyMgr) {
            ConsoleUI::printError("Group study not initialized.");
            return;
        }
        
        string topic;
        cout << "Enter study topic: ";
        getline(cin, topic);
        
        if(topic.empty()) {
            ConsoleUI::printError("Topic cannot be empty.");
            return;
        }
        
        groupStudyMgr->createSession(userId, username, topic);
        lastActive = time(0);
    }
    
    void joinGroupSession() {
        if(!groupStudyMgr) return;
        
        groupStudyMgr->displayAllSessions();
        
        string sessionId;
        cout << "\nEnter Session ID to join: ";
        getline(cin, sessionId);
        
        if(groupStudyMgr->joinSession(sessionId, userId, username)) {
            lastActive = time(0);
        }
    }
    
    void viewMyGroupSessions() {
        if(!groupStudyMgr) return;
        groupStudyMgr->displayUserSessions(userId);
    }
    
    void chatInSession() {
        if(!groupStudyMgr) return;
        
        groupStudyMgr->displayUserSessions(userId);
        
        string sessionId;
        cout << "\nEnter Session ID: ";
        getline(cin, sessionId);
        
        ConsoleUI::clearScreen();
        groupStudyMgr->viewChat(sessionId, userId);
        
        while(true) {
            cout << "\nYour message (or 'exit' to leave): ";
            string msg;
            getline(cin, msg);
            
            if(msg == "exit" || msg == "EXIT") break;
            
            if(!msg.empty()) {
                groupStudyMgr->sendMessage(sessionId, userId, username, msg);
                ConsoleUI::clearScreen();
                groupStudyMgr->viewChat(sessionId, userId);
            }
        }
        lastActive = time(0);
    }
    
    void findStudyBuddies() {
        if(!groupStudyMgr) return;
        
        string topic;
        cout << "Enter topic: ";
        getline(cin, topic);
        
        groupStudyMgr->findStudyBuddies(userId, topic);
    }
    
    void viewAllStudySessions() {
        if(!groupStudyMgr) return;
        groupStudyMgr->displayAllSessions();
    }
    
private:
    string generateUserId() {
        time_t t = time(0);
        stringstream ss;
        ss << "U" << t << (rand() % 10000);
        return ss.str();
    }
};

// ============ USER MANAGER ============
class UserManager {
private:
    string usersFile;
    string userDataFile;
    UserAVL userTree;
    LeaderboardHeap leaderboard;
    GroupStudyManager groupStudyMgr;
    
public:
    UserManager() {
        usersFile = "users.txt";
        userDataFile = "userdata_";
    }
    
    bool signup(const string& username, const string& password) {
        if(username.empty() || password.empty()) {
            ConsoleUI::printError("Username and password cannot be empty.");
            return false;
        }
        
        if(userExists(username)) {
            ConsoleUI::printError("Username already exists.");
            return false;
        }
        
        User newUser(username, password);
        userTree.root = userTree.insert(userTree.root, username, newUser.getUserId());
        leaderboard.insert(username, 0);
        
        try {
            ofstream file(usersFile.c_str(), ios::app);
            if(!file.is_open()) {
                ConsoleUI::printError("Could not open users file.");
                return false;
            }
            
            file << newUser.serialize() << endl;
            file.close();
            
            ConsoleUI::printSuccess("Account created! Welcome, " + username + "!");
            return true;
            
        } catch(const exception& e) {
            ConsoleUI::printError("Failed to create account");
            return false;
        }
    }
    
    User* login(const string& username, const string& password) {
        if(username.empty() || password.empty()) {
            ConsoleUI::printError("Username and password cannot be empty.");
            return NULL;
        }
        
        try {
            ifstream file(usersFile.c_str());
            if(!file.is_open()) {
                ConsoleUI::printError("Users file not found. Please sign up first.");
                return NULL;
            }
            
            string line;
            while(getline(file, line)) {
                if(line.empty()) continue;
                
                User u = User::deserialize(line);
                if(u.getUsername() == username && u.getPassword() == password) {
                    file.close();
                    
                    User* loggedInUser = new User(u.getUsername(), u.getPassword());
                    loggedInUser->setUserId(u.getUserId());
                    loggedInUser->setScore(u.getScore());
                    
                    if(!userTree.usernameExists(username)) {
                        userTree.root = userTree.insert(userTree.root, username, loggedInUser->getUserId());
                    }
                    
                    loadUserData(*loggedInUser);
                    leaderboard.insert(username, loggedInUser->getScore());
                    ConsoleUI::printSuccess("Welcome back, " + username + "!");
                    return loggedInUser;
                }
            }
            
            file.close();
            ConsoleUI::printError("Invalid username or password.");
            return NULL;
            
        } catch(const exception& e) {
            ConsoleUI::printError("Login failed");
            return NULL;
        }
    }

    bool saveUserData(const User& user) {
        string filename = userDataFile + user.getUserId() + ".txt";
        
        try {
            ofstream file(filename.c_str());
            if(!file.is_open()) {
                ConsoleUI::printError("Could not save user data.");
                return false;
            }
            
            file << "[FLASHCARDS]" << endl;
            SinglyNode<Flashcard>* currCard = user.getFlashcardManager().getFlashcards().head;
            while(currCard) {
                file << currCard->getData().serialize() << endl;
                currCard = currCard->getNext();
            }
            
            file << "[TASKS]" << endl;
            file << ToDoList::getCount() << endl;
            SinglyLinkedList<Task> taskList;
            user.getToDoList().getAllTasks(taskList);
            SinglyNode<Task>* currTask = taskList.head;
            while(currTask) {
                file << currTask->getData().serialize() << endl;
                currTask = currTask->getNext();
            }
            
            file << "[SESSIONS]" << endl;
            Node<FocusSession>* currSession = user.getFocusHistory().getSessions().topNode();
            while(currSession) {
                file << currSession->getData().serialize() << endl;
                currSession = currSession->getNext();
            }
            
            file.close();
            return true;
            
        } catch(const exception& e) {
            ConsoleUI::printError("Failed to save user data");
            return false;
        }
    }

    bool loadUserData(User& user) {
        string filename = userDataFile + user.getUserId() + ".txt";
        
        try {
            ifstream file(filename.c_str());
            if(!file.is_open()) {
                return true;
            }
            
            string line;
            string currentSection = "";
            vector<string> sessionLines;
            bool nextIdRead = false;
            
            while(getline(file, line)) {
                if(line.empty()) continue;
                
                if(line == "[FLASHCARDS]") {
                    currentSection = "FLASHCARDS";
                    continue;
                } else if(line == "[TASKS]") {
                    currentSection = "TASKS";
                    nextIdRead = false;
                    continue;
                } else if(line == "[SESSIONS]") {
                    currentSection = "SESSIONS";
                    continue;
                }
                
                if(currentSection == "FLASHCARDS") {
                    Flashcard f = Flashcard::deserialize(line);
                    user.addFlashcard(f);
                } else if(currentSection == "TASKS") {
                    if(!nextIdRead) {
                        int taskCount;
                        stringstream ss(line);
                        ss >> taskCount;
                        ToDoList::setCount(taskCount);
                        nextIdRead = true;
                    } else {
                        Task t = Task::deserialize(line);
                        user.getToDoList().addTaskFromFile(t);
                    }
                } else if(currentSection == "SESSIONS") {
                    sessionLines.push_back(line);
                }
            }
            
            for(int i = sessionLines.getSize() - 1; i >= 0; i--) {
                FocusSession fs = FocusSession::deserialize(sessionLines[i]);
                user.getFocusHistory().addSession(fs);
            }
            
            file.close();
            return true;
            
        } catch(const exception& e) {
            ConsoleUI::printError("Failed to load user data");
            return false;
        }
    }
    
    bool updateUserScore(const User& user) {
        leaderboard.insert(user.getUsername(), user.getScore());
        
        try {
            ifstream inFile(usersFile.c_str());
            if(!inFile.is_open()) return false;
            
            vector<string> lines;
            string line;
            bool updated = false;
            
            while(getline(inFile, line)) {
                if(line.empty()) continue;
                
                User tempUser = User::deserialize(line);
                if(tempUser.getUserId() == user.getUserId()) {
                    lines.push_back(user.serialize());
                    updated = true;
                } else {
                    lines.push_back(line);
                }
            }
            inFile.close();
            
            if(!updated) return false;
            
            ofstream outFile(usersFile.c_str());
            if(!outFile.is_open()) return false;
            
            for(int i = 0; i < lines.getSize(); i++) {
                outFile << lines[i] << endl;
            }
            outFile.close();
            return true;
            
        } catch(const exception& e) {
            ConsoleUI::printError("Failed to update user score");
            return false;
        }
    }
    
    void displayLeaderboard(int n = 10) {
        leaderboard.displayTop(n);
    }
    
    void updateLeaderboard(const string& username, int score) {
        leaderboard.insert(username, score);
        stringstream message_stream;
		message_stream<<"Score updated to "<<score<<" points!";
		string final_message = message_stream.str();
        ConsoleUI::printSuccess(final_message);
    }
    
    int getUserRank(const string& username) {
        return leaderboard.getUserRank(username);
    }
    
    void loadAllUsersToTree() {
        try {
            ifstream file(usersFile.c_str());
            if(!file.is_open()) return;
            
            string line;
            while(getline(file, line)) {
                if(line.empty()) continue;
                User u = User::deserialize(line);
                userTree.root = userTree.insert(userTree.root, u.getUsername(), u.getUserId());
                leaderboard.insert(u.getUsername(), u.getScore());
            }
            file.close();
        } catch(const exception& e) {
            ConsoleUI::printError("Failed to load users");
        }
    }
    
    GroupStudyManager& getGroupStudyManager() { 
        return groupStudyMgr; 
    }
    
    void initializeGroupStudy() {
        groupStudyMgr.loadSessions();
    }
    
    void setupUserGroupStudy(User* user) {
        user->setGroupStudyManager(&groupStudyMgr);
        // Create lightweight UserInfo instead of full User
        UserInfo info(user->getUserId(), user->getUsername(), user->getScore());
        groupStudyMgr.addUserToGraph(info);
    }
    
    void saveGroupSessions() {
        groupStudyMgr.saveSessions();
    }

private:
    bool userExists(const string& username) {
        try {
            ifstream file(usersFile.c_str());
            if(!file.is_open()) return false;
            
            string line;
            while(getline(file, line)) {
                if(line.empty()) continue;
                
                User u = User::deserialize(line);
                if(u.getUsername() == username) {
                    file.close();
                    return true;
                }
            }
            
            file.close();
            return false;
            
        } catch(const exception& e) {
            return false;
        }
    }
};

// Define User::createQuizFromTopic (needs UserManager)
void User::createQuizFromTopic(const string &topic){
    SinglyLinkedList<Flashcard> topicFlashcards = quizzes.getFlashcardsByTopic(topic);
    
    if(!topicFlashcards.head){
        ConsoleUI::printError("No flashcards found for topic '" + topic + "'");
        return;
    }
    
    Quiz q;
    q.createQuiz(topicFlashcards);
    q.conductQuiz();
    
    int earnedPoints = q.getScore();
    score += earnedPoints;
    lastActive = time(0);
    
    // IMMEDIATE LEADERBOARD UPDATE
    if(userManagerPtr) {
        userManagerPtr->updateLeaderboard(username, score);
    }
    stringstream message_stream;
	message_stream << "You earned " << earnedPoints << " points!";
	string final_message = message_stream.str();
    ConsoleUI::printSuccess(final_message);
}

// ============ MENU FUNCTIONS ============
void displayMainMenu() {
    ConsoleUI::clearScreen();
    ConsoleUI::printHeader("STUDY TRACK APP");
    cout << "1. Login\n";
    cout << "2. Sign Up\n";
    cout << "3. Exit\n";
    cout << "\nEnter your choice: ";
}

void displayUserMenu(const string& username) {
    ConsoleUI::clearScreen();
    ConsoleUI::printHeader("Welcome, " + username);
    
    ConsoleUI::setColor(ConsoleUI::CYAN);
    cout << "--- FLASHCARDS ---\n";
    ConsoleUI::setColor(ConsoleUI::WHITE);
    cout << "1.  Add Flashcard\n";
    cout << "2.  View All Flashcards\n";
    cout << "3.  Delete Flashcard\n";
    cout << "4.  Take Quiz by Topic\n";
    cout << "5.  View Available Topics\n";
    
    ConsoleUI::setColor(ConsoleUI::CYAN);
    cout << "\n--- FOCUS SESSIONS ---\n";
    ConsoleUI::setColor(ConsoleUI::WHITE);
    cout << "6.  Start Focus Session\n";
    cout << "7.  Pause Focus Session\n";
    cout << "8.  Resume Focus Session\n";
    cout << "9.  Show Session Summary\n";
    cout << "10. Show Session History\n";
    cout << "11. Show Total Study Time\n";
    cout << "12. Show Number of Sessions\n";
    cout << "13. Clear Session History\n";
    
    ConsoleUI::setColor(ConsoleUI::CYAN);
    cout << "\n--- TO-DO LIST ---\n";
    ConsoleUI::setColor(ConsoleUI::WHITE);
    cout << "14. Add Task\n";
    cout << "15. View Tasks\n";
    cout << "16. Mark Task Complete\n";
    cout << "17. View Closest Deadline\n";
    cout << "18. Delete Task\n";
    
    ConsoleUI::setColor(ConsoleUI::CYAN);
    cout << "\n--- LEADERBOARD ---\n";
    ConsoleUI::setColor(ConsoleUI::WHITE);
    cout << "19. View Leaderboard\n";
    cout << "20. View My Rank\n";
    
    ConsoleUI::setColor(ConsoleUI::CYAN);
    cout << "\n--- GROUP STUDY ---\n";
    ConsoleUI::setColor(ConsoleUI::WHITE);
    cout << "22. Create Group Study Session\n";
    cout << "23. Join Group Study Session\n";
    cout << "24. View My Study Groups\n";
    cout << "25. Chat in Study Session\n";
    cout << "26. Find Study Buddies\n";
    cout << "27. View All Active Sessions\n";
    
    ConsoleUI::setColor(ConsoleUI::YELLOW);
    cout << "\n28. Save & Logout\n";
    ConsoleUI::setColor(ConsoleUI::WHITE);
    
    cout << "\nEnter your choice: ";
}

// ============ MAIN ============
int main(){
    srand(static_cast<unsigned int>(time(0)));
    
    UserManager userManager;
    userManager.loadAllUsersToTree();
    userManager.initializeGroupStudy();
    User* currentUser = NULL;
    
    ConsoleUI::clearScreen();
    ConsoleUI::printHeader("STUDY TRACK APP");
    ConsoleUI::printInfo("Your Personal Study Companion");
    ConsoleUI::waitForEnter();
    
    while(true) {
        if(currentUser == NULL) {
            displayMainMenu();
            
            int choice;
            if(!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                ConsoleUI::printError("Invalid input. Please enter a number.");
                ConsoleUI::waitForEnter();
                continue;
            }
            cin.ignore();
            
            switch(choice) {
                case 1: {
                    ConsoleUI::printHeader("LOGIN");
                    string username, password;
                    cout << "Username: ";
                    getline(cin, username);
                    password = ConsoleUI::getPassword("Password: ");
                    
                    currentUser = userManager.login(username, password);
                    if(currentUser) { 
                        userManager.setupUserGroupStudy(currentUser);
                        currentUser->setUserManager(&userManager);
                    }
                    ConsoleUI::waitForEnter();
                    break;
                }
                case 2: {
                    ConsoleUI::printHeader("SIGN UP");
                    string username, password;
                    cout << "Choose a username: ";
                    getline(cin, username);
                    password = ConsoleUI::getPassword("Choose a password: ");
                    
                    userManager.signup(username, password);
                    ConsoleUI::waitForEnter();
                    break;
                }
                case 3:
                    ConsoleUI::printSuccess("Thank you for using Study Track App. Goodbye!");
                    return 0;
                default:
                    ConsoleUI::printError("Invalid choice.");
                    ConsoleUI::waitForEnter();
            }
        } else {
            displayUserMenu(currentUser->getUsername());
            
            int choice;
            if(!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                ConsoleUI::printError("Invalid input.");
                ConsoleUI::waitForEnter();
                continue;
            }
            cin.ignore();
            
            switch(choice) {
                case 1:
                    currentUser->addFlashcardInteractive();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 2:
                    currentUser->reviewFlashcards();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 3:{
                    string id;
                    currentUser->reviewFlashcards();
                    cout<<"Enter flashcard ID to delete: ";
                    getline(cin,id);  
                    currentUser->deleteFlashcards(id);
                    ConsoleUI::waitForEnter();
                    break;
                }
                    
                case 4: {
                    string topic;
                    currentUser->getQuizManager().displayTopics();
                    cout << "\nEnter topic name: ";
                    getline(cin, topic);
                    currentUser->createQuizFromTopic(topic);
                    ConsoleUI::waitForEnter();
                    break;
                }
                
                case 5:
                    currentUser->getQuizManager().displayTopics();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 6:
                    currentUser->startFocusSession();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 7:
                    currentUser->pauseFocusSession();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 8:
                    currentUser->resumeFocusSession();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 9:
                    currentUser->showFocusSummary();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 10:
                    currentUser->showFocusHistory();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 11:
                    currentUser->showTotalStudyTime();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 12:
                    currentUser->showNumberOfSessions();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 13:
                    currentUser->clearFocusHistory();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 14:
                    currentUser->addTask();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 15:
                    currentUser->viewTasks();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 16:
                    currentUser->completeTask();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 17:
                    currentUser->viewClosestDeadline();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 18:
                    currentUser->removeTask();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 19:
                    userManager.displayLeaderboard(10);
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 20: {
                    int rank = userManager.getUserRank(currentUser->getUsername());
                    if(rank > 0) {
                    	stringstream message_stream;
						message_stream << "Your rank: # " << rank<<" with "<<currentUser->getScore()<<" focus sessions!";
						string final_message = message_stream.str();
                        ConsoleUI::printSuccess(final_message);
                    } else {
                        ConsoleUI::printInfo("Rank not found.");
                    }
                    ConsoleUI::waitForEnter();
                    break;
                }
                    
                case 22:
                    currentUser->createGroupSession();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 23:
                    currentUser->joinGroupSession();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 24:
                    currentUser->viewMyGroupSessions();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 25:
                    currentUser->chatInSession();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 26:
                    currentUser->findStudyBuddies();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 27:
                    currentUser->viewAllStudySessions();
                    ConsoleUI::waitForEnter();
                    break;
                    
                case 28:
                    ConsoleUI::printHeader("SAVING DATA");
                    userManager.saveUserData(*currentUser);
                    userManager.updateUserScore(*currentUser);
                    userManager.saveGroupSessions();
                    ConsoleUI::printSuccess("All data saved. See you soon, " + 
                                          currentUser->getUsername() + "!");
                    delete currentUser;
                    currentUser = NULL;
                    ConsoleUI::waitForEnter();
                    break;
                    
                default:
                    ConsoleUI::printError("Invalid choice.");
                    ConsoleUI::waitForEnter();
            }
        }
    }
    
    return 0;
}
