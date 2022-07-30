#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <map>
#include <set>

using namespace std;

class Node {
public:
    string name;
    string id;
    vector<pair<int, float>> reviews;
    vector<string> genres;
};

class minHeap {
private:
    Node *array;
    int capacity;
    int size;

public:
    vector<pair<int, pair<string, float>>> reviewsByUser;
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
    void goodMovieReview(string movie);
    void badMovieReview(string movie);
    void goodMovieGenre(string movie);
    void badMovieGenre(string movie);
    void bestGenre(string genre);
    void worstGenre(string genre);
    void topOverall();
    void worstOverall();
    vector<pair<double, string>> averageReviewByMovie();
    vector<pair<double, string>> averageReviewSpecificMovies(vector<int> movies);
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
        cout << "heyyyy" << endl;
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

void minHeap::goodMovieReview(string movie) {
//    int index = -1;
//    for (unsigned int i = 0; i < size; i++) {
//        if (array[i].name.find(movie) != string::npos) {
//            index = i;
//            break;
//        }
//    }
//
//    if (index == -1) {
//        cout << "Invalid Movie! Please enter a valid movie title" << endl;
//        return;
//    }
//
//    vector<pair<float, int>> highReviews;
//
//    for (unsigned int j = 0; j < array[index].reviews.size(); j++) {
//        if (array[index].reviews.at(j).second >= 4.0) {
//            highReviews.push_back(make_pair(array[index].reviews.at(j).second, array[index].reviews.at(j).first));
////            cout << array[index].reviews.at(j).first << endl;
//        }
//    }
//
////    cout << highReviews.size() << endl;
//
//    sort(highReviews.begin(), highReviews.end());
//
//    vector<pair<string, float>> reviewsByTitle;
//
//    for (unsigned int k = 0; k < 50; k++) {
//        for (unsigned int l = 0; l < reviewsByUser.size(); l++) {
//            if (reviewsByUser.at(l).first == highReviews.at(k).second && reviewsByUser.at(l).second.first != array[index].id && reviewsByUser.at(l).second.second >= 4.0) {
////                cout << reviewsByUser.at(l).second.first << " " << reviewsByUser.at(l).second.second << endl;
//                reviewsByTitle.push_back(make_pair(reviewsByUser.at(l).second.first, reviewsByUser.at(l).second.second));
////                cout << reviewsByUser.at(l).second.first << endl;
//            }
//        }
//    }
//
//    vector<pair<string, string>> movieNames;
//
////    vector<string> movieNames15;
//
//    for (unsigned int m = 0; m < reviewsByTitle.size(); m++) {
//        for (unsigned int n = 0; n < size; n++) {
//            if (array[n].id == reviewsByTitle.at(m).first) {
//                for (unsigned int r = 0; r < array[n].genres.size(); r++) {
//                    movieNames.push_back(make_pair(array[n].name, array[n].genres.at(r)));
//                }
////                cout << array[n].name << endl;
////                movieNames15.push_back(array[n].name);
//            }
//        }
//    }
//
//
//    set<string> correctMovies;
//
//
//    for (unsigned int s = 0; s < movieNames.size(); s++) {
//        for (unsigned int w = 0; w < array[index].genres.size(); w++) {
//            if (movieNames.at(s).second == array[index].genres.at(w)) {
//                correctMovies.insert(movieNames.at(s).first);
//                break;
//            }
//        }
//    }
//
//
//    set<string>::iterator iter = correctMovies.begin();
//    if (correctMovies.size() < 10) {
//        for (iter = correctMovies.begin(); iter != correctMovies.end(); iter++) {
//            cout << *iter << endl;
//        }
//    }
//    else {
//        for (unsigned int i = 0; i < 10; i++) {
//            cout << *iter << endl;
//            iter++;
//        }
//    }

    int index = -1;
    for (unsigned int i = 0; i < size; i++) {
        if (array[i].name.find(movie) != string::npos) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    vector<pair<float, int>> highReviews;

    for (unsigned int j = 0; j < array[index].reviews.size(); j++) {
        if (array[index].reviews.at(j).second >= 4.0) {
            highReviews.push_back(make_pair(array[index].reviews.at(j).second, array[index].reviews.at(j).first));
//            cout << array[index].reviews.at(j).first << endl;
        }
    }

//    cout << highReviews.size() << endl;

    sort(highReviews.begin(), highReviews.end(), greater<>());

//    for (unsigned int t = 0; t < highReviews.size(); t++) {
//        cout << highReviews.at(t).first << endl;
//    }

    vector<pair<float, int>> temp;

    if (highReviews.size() > 100) {
        for (unsigned int q = 0; q < 100; q++) {
            temp.push_back(make_pair(highReviews.at(q).first, highReviews.at(q).second));
        }

        highReviews.clear();
        highReviews = temp;
        temp.clear();
    }

    vector<pair<string, float>> reviewsByTitle;

    for (unsigned int k = 0; k < highReviews.size(); k++) {
        for (unsigned int l = 0; l < reviewsByUser.size(); l++) {
            if (reviewsByUser.at(l).first == highReviews.at(k).second && reviewsByUser.at(l).second.first != array[index].id && reviewsByUser.at(l).second.second >= 4.0) {
//                cout << reviewsByUser.at(l).second.first << " " << reviewsByUser.at(l).second.second << endl;
                reviewsByTitle.push_back(make_pair(reviewsByUser.at(l).second.first, reviewsByUser.at(l).second.second));
//                cout << reviewsByUser.at(l).second.first << endl;
            }
        }
    }

    map<string, vector<float>> averageReviewsTemp;

    for (unsigned int m = 0; m < reviewsByTitle.size(); m++) {
        averageReviewsTemp[reviewsByTitle.at(m).first].push_back(reviewsByTitle.at(m).second);
    }

    map <float, string> averageReviews;

    map<string, vector<float>>::iterator iterTemp;

    for (iterTemp = averageReviewsTemp.begin(); iterTemp != averageReviewsTemp.end(); iterTemp++) {
        float total = 0;
        for (unsigned int r = 0; r < (*iterTemp).second.size(); r++) {
            total += (*iterTemp).second.at(r);
        }
        averageReviews[(total/(float) (*iterTemp).second.size())] = (*iterTemp).first;
    }

    map<float, string>::iterator iter = averageReviews.begin();
    for (unsigned int i = 0; i < 10; i++) {
        for (unsigned int w = 0; w < size; w++) {
            if (array[w].id == (*iter).second) {
                cout << array[w].name << endl;
            }
        }
        iter++;
    }

//    vector<pair<string, string>> movieNames;
//
////    vector<string> movieNames15;
//
//    for (unsigned int m = 0; m < reviewsByTitle.size(); m++) {
//        for (unsigned int n = 0; n < size; n++) {
//            if (array[n].id == reviewsByTitle.at(m).first) {
//                for (unsigned int r = 0; r < array[n].genres.size(); r++) {
//                    movieNames.push_back(make_pair(array[n].name, array[n].genres.at(r)));
//                }
////                cout << array[n].name << endl;
////                movieNames15.push_back(array[n].name);
//            }
//        }
//    }
//
//
//    set<string> correctMovies;
//
//
//    for (unsigned int s = 0; s < movieNames.size(); s++) {
//        for (unsigned int w = 0; w < array[index].genres.size(); w++) {
//            if (movieNames.at(s).second == array[index].genres.at(w)) {
//                correctMovies.insert(movieNames.at(s).first);
//                break;
//            }
//        }
//    }
//
//
//    set<string>::iterator iter = correctMovies.begin();
//    if (correctMovies.size() < 10) {
//        for (iter = correctMovies.begin(); iter != correctMovies.end(); iter++) {
//            cout << *iter << endl;
//        }
//    }
//    else {
//        for (unsigned int i = 0; i < 10; i++) {
//            cout << *iter << endl;
//            iter++;
//        }
//    }
}

void minHeap::badMovieReview(string movie) {
//    int index = -1;
//    for (unsigned int i = 0; i < size; i++) {
//        if (array[i].name.find(movie) != string::npos) {
//            index = i;
//            break;
//        }
//    }
//
//    if (index == -1) {
//        cout << "Invalid Movie! Please enter a valid movie title" << endl;
//        return;
//    }
//
//    vector<pair<float, int>> highReviews;
//
//    for (unsigned int j = 0; j < array[index].reviews.size(); j++) {
//        if (array[index].reviews.at(j).second <= 2.0) {
//            highReviews.push_back(make_pair(array[index].reviews.at(j).second, array[index].reviews.at(j).first));
//            cout << array[index].reviews.at(j).second << endl;
//        }
//    }
//
////    cout << highReviews.size() << endl;
//
//    sort(highReviews.begin(), highReviews.end());
//
//    vector<pair<string, float>> reviewsByTitle;
//
//    for (unsigned int k = 0; k < 50; k++) {
//        for (unsigned int l = 0; l < reviewsByUser.size(); l++) {
//            if (reviewsByUser.at(l).first == highReviews.at(k).second && reviewsByUser.at(l).second.first != array[index].id && reviewsByUser.at(l).second.second <= 2.0) {
////                cout << reviewsByUser.at(l).second.first << " " << reviewsByUser.at(l).second.second << endl;
//                reviewsByTitle.push_back(make_pair(reviewsByUser.at(l).second.first, reviewsByUser.at(l).second.second));
////                cout << reviewsByUser.at(l).second.first << endl;
//            }
//        }
//    }
//
//    vector<pair<string, string>> movieNames;
//
////    vector<string> movieNames15;
//
//    for (unsigned int m = 0; m < reviewsByTitle.size(); m++) {
//        for (unsigned int n = 0; n < size; n++) {
//            if (array[n].id == reviewsByTitle.at(m).first) {
//                for (unsigned int r = 0; r < array[n].genres.size(); r++) {
//                    movieNames.push_back(make_pair(array[n].name, array[n].genres.at(r)));
//                }
////                cout << array[n].name << endl;
////                movieNames15.push_back(array[n].name);
//            }
//        }
//    }
//
//
//    set<string> correctMovies;
//
//    for (unsigned int s = 0; s < movieNames.size(); s++) {
//        for (unsigned int w = 0; w < array[index].genres.size(); w++) {
//            if (movieNames.at(s).second == array[index].genres.at(w)) {
//                correctMovies.insert(movieNames.at(s).first);
//                break;
//            }
//        }
//    }
//
//
//    set<string>::iterator iter = correctMovies.begin();
//    if (correctMovies.size() < 10) {
//        for (iter = correctMovies.begin(); iter != correctMovies.end(); iter++) {
//            cout << *iter << endl;
//        }
//    }
//    else {
//        for (unsigned int i = 0; i < 10; i++) {
//            cout << *iter << endl;
//            iter++;
//        }
//    }

    int index = -1;
    for (unsigned int i = 0; i < size; i++) {
        if (array[i].name.find(movie) != string::npos) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    vector<pair<float, int>> highReviews;

    for (unsigned int j = 0; j < array[index].reviews.size(); j++) {
        if (array[index].reviews.at(j).second <= 2.0) {
            highReviews.push_back(make_pair(array[index].reviews.at(j).second, array[index].reviews.at(j).first));
//            cout << array[index].reviews.at(j).first << endl;
        }
    }

//    cout << highReviews.size() << endl;

    sort(highReviews.begin(), highReviews.end());
//    for (unsigned int t = 0; t < highReviews.size(); t++) {
//        cout << highReviews.at(t).first;
//    }

    vector<pair<float, int>> temp;

    if (highReviews.size() > 100) {
        for (unsigned int q = 0; q < 100; q++) {
            temp.push_back(make_pair(highReviews.at(q).first, highReviews.at(q).second));
        }

        highReviews.clear();
        highReviews = temp;
        temp.clear();
    }

    vector<pair<string, float>> reviewsByTitle;

    for (unsigned int k = 0; k < highReviews.size(); k++) {
        for (unsigned int l = 0; l < reviewsByUser.size(); l++) {
            if (reviewsByUser.at(l).first == highReviews.at(k).second && reviewsByUser.at(l).second.first != array[index].id && reviewsByUser.at(l).second.second <= 2.0) {
//                cout << reviewsByUser.at(l).second.first << " " << reviewsByUser.at(l).second.second << endl;
                reviewsByTitle.push_back(make_pair(reviewsByUser.at(l).second.first, reviewsByUser.at(l).second.second));
//                cout << reviewsByUser.at(l).second.first << endl;
            }
        }
    }

    map<string, vector<float>> averageReviewsTemp;

    for (unsigned int m = 0; m < reviewsByTitle.size(); m++) {
        averageReviewsTemp[reviewsByTitle.at(m).first].push_back(reviewsByTitle.at(m).second);
    }

    map <float, string> averageReviews;

    map<string, vector<float>>::iterator iterTemp;

    for (iterTemp = averageReviewsTemp.begin(); iterTemp != averageReviewsTemp.end(); iterTemp++) {
        float total = 0;
        for (unsigned int r = 0; r < (*iterTemp).second.size(); r++) {
            total += (*iterTemp).second.at(r);
        }
        averageReviews[(total/(float) (*iterTemp).second.size())] = (*iterTemp).first;
    }

    map<float, string>::iterator iter = averageReviews.begin();
    for (unsigned int i = 0; i < 10; i++) {
        for (unsigned int w = 0; w < size; w++) {
            if (array[w].id == (*iter).second) {
                cout << array[w].name << endl;
            }
        }
        iter++;
    }

}

vector<pair<double, string>> minHeap::averageReviewByMovie() {
    map<string, vector<double>> tempReviews;
    for (unsigned int i = 0; i < size; i++) {
        for (unsigned int j = 0; j < array[i].reviews.size(); j++) {
            tempReviews[array[i].name].push_back(array[i].reviews.at(j).second);
        }
    }

    map<string, vector<double>>::iterator iter;
    vector<pair<double, string>> average;
    for (iter = tempReviews.begin(); iter != tempReviews.end(); iter++) {
        double total = 0;
        if ((*iter).second.size() > 100) {
            for (unsigned int k = 0; k < (*iter).second.size(); k++) {
                total += (double) (*iter).second.at(k);
            }
            average.push_back(make_pair((total / (double) (*iter).second.size()), (*iter).first));
        }
    }

    return average;
}

vector<pair<double, string>> minHeap::averageReviewSpecificMovies(vector<int> movies) {
    map<string, vector<double>> tempReviews;
    for (unsigned int i = 0; i < movies.size(); i++) {
        for (unsigned int j = 0; j < array[movies.at(i)].reviews.size(); j++) {
            tempReviews[array[movies.at(i)].name].push_back(array[movies.at(i)].reviews.at(j).second);
        }
    }

    map<string, vector<double>>::iterator iter;
    vector<pair<double, string>> average;
    for (iter = tempReviews.begin(); iter != tempReviews.end(); iter++) {
        double total = 0;
        if ((*iter).second.size() > 100) {
            for (unsigned int k = 0; k < (*iter).second.size(); k++) {
                total += (double) (*iter).second.at(k);
            }

            average.push_back(make_pair((total / (double) (*iter).second.size()), (*iter).first));
        }
    }

    return average;
}
void minHeap::goodMovieGenre(string movie) {
    int index = -1;
    for (unsigned int i = 0; i < size; i++) {
        if (array[i].name.find(movie) != string::npos) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    vector<int> correctGenreIndices;

    for (unsigned int i = 0; i < size; i++) {
        if (i != index) {
            for (unsigned int j = 0; j < array[i].genres.size(); j++) {
                for (unsigned int k = 0; k < array[index].genres.size(); k++) {
                    if (array[i].genres.at(j).find(array[index].genres.at(k)) != string::npos) {
                        correctGenreIndices.push_back(i);
                        break;
                    }
                }
            }
        }
    }

    vector<pair<double, string>> average = averageReviewSpecificMovies(correctGenreIndices);
    sort(average.begin(), average.end(), greater<>());
    for (unsigned int i = 0; i < 10; i++) {
        cout << average.at(i).second << endl;
    }
    cout << endl;
}
void minHeap::badMovieGenre(string movie) {
    int index = -1;
    for (unsigned int i = 0; i < size; i++) {
        if (array[i].name.find(movie) != string::npos) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    vector<int> correctGenreIndices;

    for (unsigned int i = 0; i < size; i++) {
        if (i != index) {
            for (unsigned int j = 0; j < array[i].genres.size(); j++) {
                for (unsigned int k = 0; k < array[index].genres.size(); k++) {
                    if (array[i].genres.at(j).find(array[index].genres.at(k)) != string::npos) {
                        correctGenreIndices.push_back(i);
                        break;
                    }
                }
            }
        }
    }

    vector<pair<double, string>> average = averageReviewSpecificMovies(correctGenreIndices);
    sort(average.begin(), average.end());
    for (unsigned int i = 0; i < 10; i++) {
        cout << average.at(i).second << endl;
    }
    cout << endl;
}
void minHeap::bestGenre(string genre) {
    vector<int> indices;
    for (unsigned int i = 0; i < size; i++) {
        for (unsigned int j = 0; j < array[i].genres.size(); j++) {
            if (array[i].genres.at(j).find(genre) != string::npos) {
//                cout << array[i].genres.at(j) << " " << array[i].name << endl;
                indices.push_back(i);
                break;
            }
        }
    }

    vector<pair<double, string>> average = averageReviewSpecificMovies(indices);
    sort(average.begin(), average.end(), greater<>());
    if (average.size() < 10) {
        for (unsigned int i = 0; i < average.size(); i++) {
            cout << average.at(i).second << endl;
        }
    }
    else {
        for (unsigned int i = 0; i < 10; i++) {
            cout << average.at(i).second << endl;
        }
    }
    cout << endl;
}
void minHeap::worstGenre(string genre) {
    vector<int> indices;
    for (unsigned int i = 0; i < size; i++) {
        for (unsigned int j = 0; j < array[i].genres.size(); j++) {
            if (array[i].genres.at(j).find(genre) != string::npos) {
//                cout << array[i].genres.at(j) << " " << array[i].name << endl;
                indices.push_back(i);
                break;
            }
        }
    }

    vector<pair<double, string>> average = averageReviewSpecificMovies(indices);
    sort(average.begin(), average.end());
    if (average.size() < 10) {
        for (unsigned int i = 0; i < average.size(); i++) {
            cout << average.at(i).second << endl;
        }
    }
    else {
        for (unsigned int i = 0; i < 10; i++) {
            cout << average.at(i).second << endl;
        }
    }
    cout << endl;
}
void minHeap::topOverall() {
    vector<pair<double, string>> average = averageReviewByMovie();
    sort(average.begin(), average.end(), greater<>());
    for (unsigned int i = 0; i < 10; i++) {
        cout << average.at(i).second << endl;
    }
    cout << endl;
}
void minHeap::worstOverall() {
    vector<pair<double, string>> average = averageReviewByMovie();
    sort(average.begin(), average.end());
    for (unsigned int i = 0; i < 10; i++) {
        cout << average.at(i).second << endl;
    }
    cout << endl;
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
        string restOfLine = "";

        getline(ratingsFile, tempUserID, ',');
        getline(ratingsFile, tempMovieID, ',');
        getline(ratingsFile, tempRating, ',');
        getline(ratingsFile, restOfLine);

        int userID = stoi(tempUserID);
        int movieID = stoi(tempMovieID);
        float rating = stof(tempRating);

//        tempReviews.push_back(make_pair(movieID, make_pair(userID, rating)));
//
//        sort(tempReviews.begin(), tempReviews.end());

        tempReviews[movieID].push_back(make_pair(userID, rating));
        heap.reviewsByUser.push_back(make_pair(userID, make_pair(tempMovieID, rating)));

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
                int p = 0;


        while (temporary.find('|') != string::npos) {
            p = temporary.find('|');
            temp->genres.push_back(temporary.substr(0,p));
            temporary = temporary.substr(p + 1);
        }
        if (temporary != "(no genres listed)") {
            temp->genres.push_back(temporary);
        }

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

//    heap.printInOrder(0);
//    heap.badMovieReview("Interstellar");

//    heap.topOverall();
//    heap.worstOverall();

    heap.bestGenre("Action");
//    heap.worstGenre("Action");
    return 0;
}
