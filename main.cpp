#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

class Node {
public:
    string name;
    string id;
    vector<pair<int, float>> reviews;
};

class minHeap {
private:
    Node *array;
    int capacity;
    int size;

public:
    minHeap(int capacityNew);
    void heapify(int root);
    int getParent(int node);
    int getLeftChild(int node);
    int getRightChild(int node);
    Node getMin();
//    bool isLarger(int index);
    void insertNode(Node* node);
    void printHeap();
    void printInOrder(int index);
    void insertReviewData(int movieID, int userID, float rating);
};

int minHeap::getParent(int node) {
    return (node - 1) / 2;
}
int minHeap::getLeftChild(int node) {
    return (2*node + 1);
}
int minHeap::getRightChild(int node) {
    return (2*node + 2);
}

minHeap::minHeap(int capacityNew) {
    size = 0;
    capacity = capacityNew;
    array = new Node[capacityNew];
}

//bool isLarger(int index) {
//    int parent = getParent(index);
//
//
//}

void minHeap::insertNode(Node* node) {
    if (size == capacity) {
        cout << "error: need bigger capacity" << endl;
    }

    size++;
    int i = size - 1;
    array[i] = *node;

    while (i != 0 && array[getParent(i)].name > array[i].name) {
        Node temp = array[getParent(i)];
        array[getParent(i)] = array[i];
        array[i] = temp;
        i = getParent(i);
    }
}

void minHeap::heapify(int root) {
    int leftChild = getLeftChild(root);
    int rightChild = getRightChild(root);

    int smallest = root;

    if (leftChild < size && array[leftChild].name < array[smallest].name) {
        smallest = leftChild;
    }
    if (rightChild < size && array[rightChild].name < array[smallest].name) {
        smallest = rightChild;
    }

    if (smallest != root) {
        Node temp = array[root];
        array[root] = array[smallest];
        array[smallest] = temp;
        heapify(smallest);
    }
}

void minHeap::printHeap() {
//    for (unsigned int i = 0; i < size; i++) {
//        cout << array[i].name << endl;
//    }


//    cout << array[0].name << endl;
}
void minHeap::printInOrder(int index) {
    for (unsigned int i = 0; i < size; i++) {
        cout << array[i].name << endl;
        for (unsigned int j = 0; j < array[i].reviews.size(); j++) {
            cout << array[i].reviews.at(j).second << endl;
        }
        if (2*i + 1 < size) {
            cout << array[2*i + 1].name << endl;
            for (unsigned int j = 0; j < array[2*i + 1].reviews.size(); j++) {
                cout << array[2*i + 1].reviews.at(j).second << endl;
            }
        }
        if (2*i + 2 < size) {
            cout << array[2*i + 2].name << endl;
            for (unsigned int j = 0; j < array[2*i + 2].reviews.size(); j++) {
                cout << array[2*i + 2].reviews.at(j).second << endl;
            }
        }
    }
}

void minHeap::insertReviewData(int movieID, int userID, float rating) {
    for (unsigned int i = 0; i < size; i++) {
        if (stoi(array[i].id) == movieID) {
            array[i].reviews.push_back(make_pair(userID, rating));
            break;
        }
    }
}


int main() {

    ifstream movieFile;
    movieFile.open("movies.csv");
    ifstream ratingsFile;
    ratingsFile.open("ratings 2.csv");
    minHeap heap = minHeap(58098);
//    minHeap heap = minHeap(10);
    string temporary = "";
    getline(movieFile, temporary);
    getline(ratingsFile, temporary);

//    vector<pair<int, pair<int, float>>> tempReviews;
    map<int, vector<pair<int, float>>> tempReviews;

    for (unsigned int i = 0; i < 27753444; i++) {
        string tempUserID = "";
        string tempMovieID = "";
        string tempRating = "";

        getline(ratingsFile, tempUserID, ',');
        getline(ratingsFile, tempMovieID, ',');
        getline(ratingsFile, tempRating, ',');

        int userID = stoi(tempUserID);
        int movieID = stoi(tempMovieID);
        float rating = stof(tempRating);

//        tempReviews.push_back(make_pair(movieID, make_pair(userID, rating)));
//
//        sort(tempReviews.begin(), tempReviews.end());

        tempReviews[movieID].push_back(make_pair(userID, rating));

//        heap.insertReviewData(movieID, userID, rating);
    }
    for (unsigned int i = 0; i < 58098; i++) {
        Node* temp = new Node;
        getline(movieFile, temp->id, ',');
//        cout << temp->id << " ";
        getline(movieFile, temp->name, ',');
        string tempName = "";
        if (temp->name.at(0) == '\"') {
            while (temp->name.at(temp->name.size() - 1) != '\"' && temp->name.find(')') == string::npos) {
                getline(movieFile, tempName, ',');
                temp->name += tempName;
            }
            temp->name = temp->name.substr(1, temp->name.size() - 2);
        }
//        while (temp->name.find(')') == string::npos) {
//            getline(movieFile, tempName, ',');
//            temp->name += tempName;
//        }
//        cout << temp->name << endl;

        temporary = "";
        getline(movieFile,temporary);

        for (unsigned int j = 0; j < tempReviews[stoi(temp->id)].size(); j++) {
            temp->reviews.push_back(make_pair(tempReviews[stoi(temp->id)].at(j).first, tempReviews[stoi(temp->id)].at(j).second));
        }
        heap.insertNode(temp);
    }

//    for (unsigned int i = 0; i < 27753444; i++) {
//        string tempUserID = "";
//        string tempMovieID = "";
//        string tempRating = "";
//
//        getline(ratingsFile, tempUserID, ',');
//        getline(ratingsFile, tempMovieID, ',');
//        getline(ratingsFile, tempRating, ',');
//
//        int userID = stoi(tempUserID);
//        int movieID = stoi(tempMovieID);
//        float rating = stof(tempRating);
//
//        heap.insertReviewData(movieID, userID, rating);
//    }
    cout << "AHUIFHKNJLKHAUEL" << endl;
//    cout << "." << endl;
//    heap.printHeap();

    heap.printInOrder(0);
    return 0;
}

