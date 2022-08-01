#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <iomanip>
#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>

using namespace std;

enum Color {RED, BLACK};

/*//-----------MOVIE CLASS---------//
class Movie
{
private:
    string movieName;
    string movieID;
    vector<string> genres;
    vector<pair<int, float>> reviews; //vector<pair<userID,rating>>
public:
    Movie();
    Movie(string movieNme, int movID, vector<string>& genre, vector<pair<int,float>>& review);
    string getMovieName();
    int getMovieID();
    vector<string> getGenres();
    vector<pair<int,float>> getReviews();
};
Movie::Movie()
{
    this->movieName = "";
    this->movieID = 0;
    this->genres = {};
    this->reviews = {};
}
Movie::Movie(string movieNme, int movID, vector<string>& genre, vector<pair<int, float>>& review)
{
    this->movieName = movieNme;
    this->movieID = movID;
    this->genres = genre;
    this->reviews = review;
}
string Movie::getMovieName()
{
    return this->movieName;
}
int Movie::getMovieID()
{
    return this->movieID;
}
vector<string> Movie::getGenres()
{
    return this->genres;
}
vector<pair<int,float>> Movie::getReviews()
{
    return this->reviews;
}
*/
//---------END OF MOVIE CLASS----------//


//-----------------------------START OF MAP IMPLEMENTATION-------------------------------//
// Class to store movie objects
struct Node
{
    string movieName; // movie title
    int id; // movie id
    vector<pair<int, float>> reviews; // all reviews associated with movie
    vector<string> genres; // all genres associated with movie
    Node *left;
    Node *right;
    Node *parent;
    bool color;

    //Overloaded Constructor
    Node(string& movieName, int id, vector<pair<int,float>>& reviews, vector<string>& genres)
    {
        this->movieName = movieName;
        this->id = id;
        this->reviews = reviews;
        this->genres = genres;
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
        this->color = BLACK;
    }
};

class Map
{
private:
    Node* root;
    vector<pair<int, pair<int, float>>> reviewsByUser;

    void rotateRight(Node* &root, Node* &node);
    void rotateLeft(Node* &root, Node* &node);
    void fixViolation(Node* &root, Node* &node);

public:
    Map()
    {
        this->root = nullptr;
    }

    void insertNode(string& movieName, int id, vector<pair<int,float>>& reviews, vector<string>& genres);
    Node* searchMovie(Node* root, string movie);
    Node* searchMovieID(Node* root, int id);
    void goodMovieReview(Node* root, string movie);
    void badMovieReview(Node* root, string movie);
    void updateReviewsByUser(vector<pair<int, pair<int, float>>> vec);
    vector<pair<double, string>> averageReviewByMovie();
    vector<pair<double, string>> averageReviewSpecificMovies(vector<Node*> movies);
    void goodMovieGenre(string movie);
    void badMovieGenre(string movie);
    void bestGenre(string genre);
    void worstGenre(string genre);
    void topOverall();
    void worstOverall();
    void inOrderRetrievalReviews(Node* root, unordered_map<string, vector<double>>& temp);
    void inOrderRetrievalGenres(Node* root, unordered_map<Node*, vector<string>>& temp);

    Node* getRoot();
    void inOrder(Node* root);
    //vector<pair<string>> returnSimilarGenres();
};

//static int help = 0;
void Map::inOrder(Node* root) {
    if (root != nullptr) {
        inOrder(root->left);
//        help++;
//        cout << root->movieName << " " << help << endl;
        inOrder(root->right);
    }
}
Node* Map::getRoot() {
    return root;
}
void Map::updateReviewsByUser(vector<pair<int, pair<int, float>>> vec) {
    reviewsByUser = vec;
}

//-------HELPER FUNCTION-----------//
Node* BSTinsertion(Node* root, Node* node)
{
    //1.Normal BST insertion

    //If the tree is empty, make a new node
    if (root == nullptr)
    {
        return node;
    }
    //Recurse through the left side
    if (node->movieName < root->movieName)
    {
        root->left  = BSTinsertion(root->left, node);
        root->left->parent = root;
    }
        //Recurse through the right side
    else if (node->movieName > root->movieName)
    {
        root->right = BSTinsertion(root->right, node);
        root->right->parent = root;
    }

    return root;
}

void Map::insertNode(string& movieName, int id, vector<pair<int,float>>& reviews, vector<string>& genres)
{

    Node* node = new Node(movieName, id,reviews,genres);


//    //Normal insertion
//    root = BSTinsertion(root, node);

    Node* y = nullptr;
    Node* x = root;
    while (x != nullptr) {
        y = x;
        if (node->movieName < x->movieName) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }

    node->parent = y;
    if (y == nullptr) {
        root = node;
    }
    else if (node->movieName < y->movieName) {
        y->left = node;
    }
    else {
        y->right = node;
    }

    if (node->parent == nullptr) {
        node->color = BLACK;
        return;
    }
    if (node->parent->parent == nullptr) {
        return;
    }

    //If there are any violations of the Red-Black Tree, fix it
    fixViolation(root, node);
}

//Rotates the tree to the right
void Map::rotateRight(Node* &root, Node* &node)
{
    //Variables
    Node* node_left = node->left;
    node->left = node_left->right;

    if (node->left != nullptr)
    {
        node_left->right->parent = node;
    }

    node_left->parent = node->parent;

    if (node->parent == nullptr)
    {
        root = node_left;
    }
    else if (node == node->parent->right)
    {
        node->parent->right = node_left;
    }
    else
    {
        node->parent->left =node_left;
    }

    node_left->right = node;
    node->parent = node_left;
}

//Rotates the tree to the left
void Map::rotateLeft(Node *&root, Node *&node)
{
    //Variables
    Node* node_right = node->right;
    node->right = node_right->left;

    if (node_right->left != nullptr)
    {
        node_right->left->parent = node;
    }

    node_right->parent = node->parent;

    if (node->parent == nullptr)
    {
        root = node_right;
    }
    else if (node->parent->left == node)
    {
        node->parent->left = node_right;
    }
    else
    {
        node->parent->right = node_right;
    }

    node_right->left = node;
    node->parent = node_right;
}

void Map::fixViolation(Node *&root, Node *&node)
{

//    cout << "WHAT" << endl;
    Node* temp;
        while (node->parent->color == RED) {
//            cout << "HEY" << endl;
            if (node->parent == node->parent->parent->right) {
                temp = node->parent->parent->left;
                if (temp->color == RED) {
                    temp->color = BLACK;
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rotateRight(root, node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateLeft(root, node->parent->parent);
                }
            } else {
                temp = node->parent->parent->right;
                if (temp->color == RED) {
                    temp->color = BLACK;
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        rotateLeft(root, node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateRight(root, node->parent->parent);
                }
            }
            if (node == root) {
                break;
            }
        }
    root->color = BLACK;
//    Node *parentNode = nullptr;
//    Node *grandparentNode = nullptr;
//
//    //Traverses the tree and does rotations as necessary
//    while(node != root && node->parent != nullptr && node->parent->color == RED && node->color != BLACK)
//    {
////        cout << "Node Color: " << node->color << endl;
////        cout << "Node Name: " << node->movieName << endl;
//        parentNode = node->parent;
//        grandparentNode = node->parent->parent;
//
////        cout << "Parent: " << parentNode->movieName << endl;
//
//        //Case I: Parent of node is the left child of Grandparent node
//        if (parentNode == grandparentNode->left)
//        {
//            Node* uncleNode = grandparentNode->right;
//
//            //Case 1: If uncle of node is also red, only recoloring is needed
//            if (uncleNode != nullptr && uncleNode->color == RED)
//            {
//                //Recolor the nodes accordingly
//                uncleNode->color = BLACK;
//                grandparentNode->color = RED;
//                parentNode->color = BLACK;
//
//                //Set node equal to grandparent node
//                node = grandparentNode;
//            }
//            else
//            {
//                //Case 2: Node is right child of parent (LEFT RIGHT ROTATION)
//                if (node == parentNode->right)
//                {
//                    //Rotate tree to the left
//                    rotateLeft(root, parentNode);
//                    //Re-assign what nodes point to
//                    node = parentNode;
//                    parentNode = node->parent;
//                }
//
//                //Case 3: Node is left child of parent (RIGHT ROTATION)
//                rotateRight(root,grandparentNode);
//
//                //Swap the colors of the parent and grandparent nodes
//                swap(parentNode->color,grandparentNode->color);
//
//                //Set node equal to the parent node
//                node = parentNode;
//            }
//        }
//            //Case II: Parent of node is the right child of Grandparent node
//        else
//        {
//            Node *uncleNode = grandparentNode->left;
//
//            //Case 1: If uncle of node is also red, only recoloring is needed
//            if (uncleNode != nullptr && uncleNode->color == RED)
//            {
//                //Recolor the nodes accordingly
//                uncleNode->color = BLACK;
//                grandparentNode->color = RED;
//                parentNode->color = BLACK;
//
//                //Set node equal to grandparent node
//                node = grandparentNode;
//            }
//            else
//            {
//                //Case 2: Node is left child of parent (RIGHT LEFT ROTATION)
//                if (node == parentNode->left)
//                {
//                    //Rotate right about the parentNode
//                    rotateRight(root, parentNode);
//
//                    //Reassigns what nodes point to
//                    node = parentNode;
//                    parentNode = node->parent;
//                }
//
//                //Case 3: Node is right child of parent (LEFT ROTATION)
//                rotateLeft(root, grandparentNode);
//
//                //Swap colors of the parent node and grandparent node
//                swap(parentNode->color, grandparentNode->color);
//
//                //Set node equal to parent node
//                node = parentNode;
//            }
//        }
////        cout << "ParentNodeColor: " << parentNode->color << endl;
//    }
//    //Root is always BLACK
//    root->color = BLACK;
}

//---------------------HELPER FUNCTIONS------------------------//

vector<Node*> getNodes(Node* root,vector<Node*> vectors)
{
    vector<Node*> movieObjects = vectors;

    if (root != nullptr)
    {
        movieObjects = getNodes(root->left, movieObjects);
        movieObjects.push_back(root);
        movieObjects = getNodes(root->right, movieObjects);
    }

    return movieObjects;
}

vector<pair<int, vector<pair<int, float>>>> getReviewsFromMovie(string fileName, vector<pair<int, pair<int, float>>>& reviewsByUser)
{
    //Variables
    vector<pair<int, vector<pair<int, float>>>> reviews;
    pair<int, vector<pair<int,float>>> userReviews;
    unordered_map<int, int> visited;


    ifstream inFile(fileName);

    if (inFile.is_open()) {
        //1.Read heading data from the file
        string lineFromFile;
        getline(inFile, lineFromFile);

        for (unsigned int i = 0; i < 27753444; i++)
        {
            getline(inFile, lineFromFile);
            istringstream stream(lineFromFile);

            //Variables
            string tempUserID;
            int userID = 0;
            string tempRating;
            float rating = 0.0;
            string tempMovieID;
            int movieID = 0;

            //Start reading in with getline
            getline(stream, tempUserID, ',');
            userID = stoi(tempUserID);

            getline(stream, tempMovieID, ',');
            movieID = stoi(tempMovieID);
            getline(stream, tempRating, ',');
            rating = stof(tempRating);

            //reviews.emplace_back(make_pair(movieID, reviews[count].second.emplace_back(make_pair(userID,rating))));
//            userReviews = make_pair(movieID, userReviews.second[count].push_back(make_pair(userID,rating)));

            if (visited.find(movieID) != visited.end())
            {
                reviews.at(visited[movieID]).second.emplace_back(make_pair(userID, rating));
            }
//            bool movieIDFound = false;
//            if (reviews.size() != 0 && movieID <= reviews.at(reviews.size() - 1).first) {
//                for (unsigned int j = 0; j < reviews.size(); j++) {
//                    if (reviews.at(j).first == movieID) {
//                        reviews.at(j).second.push_back(make_pair(userID, rating));
//                        movieIDFound = true;
//                        break;
//                    }
//                }
//            }
            else
            {
                vector<pair<int, float>> temp;
                temp.push_back(make_pair(userID, rating));
                reviews.push_back(make_pair(movieID, temp));
                visited[movieID] = reviews.size() - 1;
            }

//            cout << reviews.size() << endl;

//            sort(reviews.begin(), reviews.end());


            reviewsByUser.push_back(make_pair(userID, make_pair(movieID, rating)));

        }
    }


//    Map temp;
//    temp.updateReviewsByUser(reviewsByUser);
    return reviews;
}

void readIntoMap(string fileName, Map& map)
{
    ifstream inFile(fileName);

    if (inFile.is_open()) {
        //1.Read heading data from the file
        string lineFromFile;
        getline(inFile, lineFromFile);

        //Variables
        int count = 0;
        vector<pair<int, pair<int, float>>> reviewsByUser;
        vector<pair<int, vector<pair<int, float>>>> userReviews = getReviewsFromMovie("ratings 2.csv", reviewsByUser);

        //Sort the userReviews based on movieID to be able to access it
        sort(userReviews.begin(),userReviews.end());


        for (unsigned int j = 0; j < 58098; j++)
        {
            getline(inFile, lineFromFile);
            istringstream stream(lineFromFile);

            //Variables
            string tempMovieName;
            string movieName;
            string tempGenres;
            vector<string> genres;
            string tempMovieID;
            int movieID = 0;

            //Start reading in with getline
            getline(stream, tempMovieID, ',');
            movieID = stoi(tempMovieID);



            //Get movie name
            getline(stream, tempMovieName);


            //Checks if the movie title starts with quotation
            if (tempMovieName.at(0) == '\"')
            {
                int index = 0;
                for (int i = 1; i < tempMovieName.size(); i++)
                {
                    if (tempMovieName.at(i) == '\"')
                    {
                        index = i + 2;
                        break;
                    }
                    movieName += tempMovieName.at(i);
                }
                tempGenres = tempMovieName.substr(index, tempMovieName.size());
            }
                //If the movie title does not start with quotation
            else
            {
                int index = 0;
                for (int i = 0; i < tempMovieName.size(); i++)
                {
                    if (tempMovieName.at(i) == ',')
                    {
                        index = i + 1;
                        break;
                    }
                    movieName += tempMovieName.at(i);
                }
                tempGenres = tempMovieName.substr(index, tempMovieName.size());
            }


            //Fills up the Genre vector
            string tempString;
            if (tempString != "(no genres listed)") {
                for (int i = 0; i < tempGenres.size(); i++) {
                    if (tempGenres.at(i) == '|' || i == tempGenres.size() - 1) {
                        genres.push_back(tempString);
                        tempString = "";
                        continue;
                    }
                    tempString += tempGenres.at(i);
                }
            }


            vector<pair<int, float>> temporaryVec;
            for (unsigned int w = 0; w < userReviews.size(); w++) {
                if (userReviews.at(w).first == movieID) {
                   temporaryVec = userReviews.at(w).second;
                    break;
                }
            }


            //Insert the node into the map (Red-Black Tree)
            map.insertNode(movieName,movieID,temporaryVec,genres);

        }
        map.updateReviewsByUser(reviewsByUser);
    }
    map.inOrder(map.getRoot());

    map.badMovieGenre("Toy Story (1995)");


}

Node* Map::searchMovie(Node* root, string movie) {
    if (root == nullptr || root->movieName == movie) {
        return root;
    }
    if (movie < root->movieName) {
        return searchMovie(root->left, movie);
    }
    return searchMovie(root->right, movie);
}

Node* Map::searchMovieID(Node* root, int id) {
    if (root->id == id) {
        return root;
    }
//    if (root != nullptr) {
//        searchMovieID(root->left, id);
//        searchMovieID(root->right, id);
//    }
    Node* temp = nullptr;
    if (root->left != nullptr) {
        temp = searchMovieID(root->left, id);
    }
    if (temp != nullptr) {
        return temp;
    }
    if (root->right != nullptr) {
        temp = searchMovieID(root->right, id);
    }
    return temp;
}


void Map::goodMovieReview(Node* root, string movie) {
    auto start = chrono::high_resolution_clock::now();
    Node* temp = searchMovie(root, movie);
    if (temp == nullptr) {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    vector<pair<float, int>> highReviews;

    for (unsigned int i = 0; i < temp->reviews.size(); i++) {
        if (temp->reviews.at(i).second >= 4.0) {
            highReviews.push_back(make_pair(temp->reviews.at(i).second, temp->reviews.at(i).first));
        }
    }


    sort(highReviews.begin(), highReviews.end(), greater<>());

    vector<pair<float, int>> temporary;

    if (highReviews.size() > 100) {
        for (unsigned int k = 0; k < 100; k++) {
            temporary.push_back(make_pair(highReviews.at(k).first, highReviews.at(k).second));
        }
        highReviews.clear();
        highReviews = temporary;
        temporary.clear();
    }

    vector<pair<int, float>> reviewsByTitle;

    for (unsigned int h = 0; h < highReviews.size(); h++) {
        for (unsigned int l = 0; l < reviewsByUser.size(); l++) {
            if (reviewsByUser.at(l).first == highReviews.at(h).second && reviewsByUser.at(l).second.first != temp->id && reviewsByUser.at(l).second.second >= 4.0) {
                reviewsByTitle.push_back(make_pair(reviewsByUser.at(l).second.first, reviewsByUser.at(l).second.second));
            }
        }
    }


    unordered_map<int, vector<float>> averageReviews;

    for (unsigned int m = 0; m < reviewsByTitle.size(); m++) {
        averageReviews[reviewsByTitle.at(m).first].push_back(reviewsByTitle.at(m).second);
    }

    vector<pair<float, int>> averages;

    unordered_map<int, vector<float>>::iterator iter;
    for (iter = averageReviews.begin(); iter != averageReviews.end(); iter++) {
        float total = 0;
        if ((*iter).second.size() > 10) {
            for (unsigned int r = 0; r < (*iter).second.size(); r++) {
                total += (*iter).second.at(r);
            }
            averages.push_back(make_pair(((total) / (float) (*iter).second.size()), (*iter).first));
        }
    }

    sort(averages.begin(), averages.end(), greater<>());

    if (averages.size() < 10) {
        for (unsigned int p = 0; p < averages.size(); p++) {
            Node* tempID = searchMovieID(root, averages.at(p).second);
            if (tempID != nullptr) {
                float rate = round(averages.at(p).first * 10.0) / 10.0;
                cout << p + 1 << ". " << tempID->movieName << ", Rating: " << fixed << setprecision(1) << rate << endl;
            }
        }
    }

    else {
        for (unsigned int p = 0; p < 10; p++) {
            Node* tempID = searchMovieID(root, averages.at(p).second);
            if (tempID != nullptr) {
                float rate = round(averages.at(p).first * 10.0) / 10.0;
                cout << p + 1 << ". " << tempID->movieName << ", Rating: " << fixed << setprecision(1) << rate << endl;
            }
        }
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = (stop - start);
    auto time = chrono::duration_cast<chrono::seconds>(duration);

    cout << "This process took " << time.count() << " seconds!" << endl;

}

void Map::badMovieReview(Node* root, string movie) {
    auto start = chrono::high_resolution_clock::now();
    Node* temp = searchMovie(root, movie);
    if (temp == nullptr) {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    vector<pair<float, int>> lowReviews;

    for (unsigned int i = 0; i < temp->reviews.size(); i++) {
        if (temp->reviews.at(i).second <= 2.0) {
            lowReviews.push_back(make_pair(temp->reviews.at(i).second, temp->reviews.at(i).first));
        }
    }


    sort(lowReviews.begin(), lowReviews.end());

    vector<pair<float, int>> temporary;

    if (lowReviews.size() > 100) {
        for (unsigned int k = 0; k < 100; k++) {
            temporary.push_back(make_pair(lowReviews.at(k).first, lowReviews.at(k).second));
        }
        lowReviews.clear();
        lowReviews = temporary;
        temporary.clear();
    }

    vector<pair<int, float>> reviewsByTitle;

    for (unsigned int h = 0; h < lowReviews.size(); h++) {
        for (unsigned int l = 0; l < reviewsByUser.size(); l++) {
            if (reviewsByUser.at(l).first == lowReviews.at(h).second && reviewsByUser.at(l).second.first != temp->id && reviewsByUser.at(l).second.second <= 2.0) {
                reviewsByTitle.push_back(make_pair(reviewsByUser.at(l).second.first, reviewsByUser.at(l).second.second));
            }
        }
    }


    unordered_map<int, vector<float>> averageReviews;

    for (unsigned int m = 0; m < reviewsByTitle.size(); m++) {
        averageReviews[reviewsByTitle.at(m).first].push_back(reviewsByTitle.at(m).second);
    }

    vector<pair<float, int>> averages;

    unordered_map<int, vector<float>>::iterator iter;
    for (iter = averageReviews.begin(); iter != averageReviews.end(); iter++) {
        float total = 0;
        if ((*iter).second.size() > 10) {
            for (unsigned int r = 0; r < (*iter).second.size(); r++) {
                total += (*iter).second.at(r);
            }
            averages.push_back(make_pair(((total) / (float) (*iter).second.size()), (*iter).first));
        }
    }

    sort(averages.begin(), averages.end());

    vector<pair<float, string>> averageMovies;
    for (unsigned int e = 0; e < averages.size(); e++) {
        Node* tempID = searchMovieID(root, averages.at(e).second);
        if (tempID != nullptr) {
            averageMovies.push_back(make_pair(averages.at(e).first, tempID->movieName));
        }
    }

    sort(averageMovies.begin(), averageMovies.end());

    if (averageMovies.size() < 10) {
        for (unsigned int p = 0; p < averageMovies.size(); p++) {
//            Node* tempID = searchMovieID(root, averages.at(p).second);
//            if (tempID != nullptr) {
                float rate = round(averages.at(p).first * 10.0) / 10.0;
                cout << p + 1 << ". " << averageMovies.at(p).second << ", Rating: " << fixed << setprecision(1) << rate << endl;
//            }
        }
    }

    else {
        for (unsigned int p = 0; p < 10; p++) {
//            Node* tempID = searchMovieID(root, averages.at(p).second);
//            if (tempID != nullptr) {
                float rate = round(averages.at(p).first * 10.0) / 10.0;
                cout << p + 1 << ". " << averageMovies.at(p).second << ", Rating: " << fixed << setprecision(1) << rate << endl;
//            }
        }
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = (stop - start);
    auto time = chrono::duration_cast<chrono::seconds>(duration);

    cout << "This process took " << time.count() << " seconds!" << endl;
}

void Map::inOrderRetrievalReviews(Node* root, unordered_map<string, vector<double>>& temp) {
    if (root != nullptr) {
        inOrderRetrievalReviews(root->left, temp);
        for (unsigned int i = 0; i < root->reviews.size(); i++) {
            temp[root->movieName].push_back((double)root->reviews.at(i).second);
        }
        inOrderRetrievalReviews(root->right, temp);
    }
}

void Map::inOrderRetrievalGenres(Node* root, unordered_map<Node*, vector<string>>& temp) {
    if (root != nullptr) {
        inOrderRetrievalGenres(root->left, temp);
        for (unsigned int i = 0; i < root->genres.size(); i++) {
            temp[root].push_back(root->genres.at(i));
        }
        inOrderRetrievalGenres(root->right, temp);
    }
}

vector<pair<double, string>> Map::averageReviewByMovie() {
    unordered_map<string, vector<double>> tempReviews;
    inOrderRetrievalReviews(root, tempReviews);

    unordered_map<string, vector<double>>::iterator iter;
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

vector<pair<double, string>> Map::averageReviewSpecificMovies(vector<Node*> movies) {
    unordered_map<string, vector<double>> tempReviews;
    for (unsigned int i = 0; i < movies.size(); i++) {
        for (unsigned int j = 0; j < movies.at(i)->reviews.size(); j++) {
            tempReviews[movies.at(i)->movieName].push_back(movies.at(i)->reviews.at(j).second);
        }
    }

    unordered_map<string, vector<double>>::iterator iter;
    vector<pair<double, string>> average;
    for (iter = tempReviews.begin(); iter != tempReviews.end(); iter++) {
        double total = 0;
        if ((*iter).second.size() > 100) {
            for (unsigned int k = 0; k < (*iter).second.size(); k++) {
                total += (double)(*iter).second.at(k);
            }
            average.push_back(make_pair((total / (double) (*iter).second.size()), (*iter).first));
        }
    }

    return average;
}
void Map::goodMovieGenre(string movie) {
    auto start = chrono::high_resolution_clock::now();
    Node* temp = searchMovie(root, movie);
    if (temp == nullptr) {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }


    vector<Node*> correctMovies;
    unordered_map<Node*, vector<string>> temporary;
    inOrderRetrievalGenres(root, temporary);



    unordered_map<Node*, vector<string>>::iterator iter;
    for (iter = temporary.begin(); iter != temporary.end(); iter++) {
        if ((*iter).first->movieName != temp->movieName) {
            for (unsigned int i = 0; i < (*iter).second.size(); i++) {
                for (unsigned int j = 0; j < temp->genres.size(); j++) {
                    if ((*iter).second.at(i).find(temp->genres.at(j)) != string::npos) {
                        correctMovies.push_back((*iter).first);
                        break;
                    }
                }
            }
        }
    }


    vector<pair<double, string>> average = averageReviewSpecificMovies(correctMovies);

    sort(average.begin(), average.end(), greater<>());

    if (average.size() < 10) {
        for (unsigned int i = 0; i < average.size(); i++) {
            float rating = round(average.at(i).first * 10.0) / 10.0;
            cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
        }
    }

    else {
        for (unsigned int i = 0; i < 10; i++) {
            float rating = round(average.at(i).first * 10.0) / 10.0;
            cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
        }
    }


}
void Map::badMovieGenre(string movie) {
    auto start = chrono::high_resolution_clock::now();
    Node* temp = searchMovie(root, movie);
    if (temp == nullptr) {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }


    vector<Node*> correctMovies;
    unordered_map<Node*, vector<string>> temporary;
    inOrderRetrievalGenres(root, temporary);



    unordered_map<Node*, vector<string>>::iterator iter;
    for (iter = temporary.begin(); iter != temporary.end(); iter++) {
        if ((*iter).first->movieName != temp->movieName) {
            for (unsigned int i = 0; i < (*iter).second.size(); i++) {
                for (unsigned int j = 0; j < temp->genres.size(); j++) {
                    if ((*iter).second.at(i).find(temp->genres.at(j)) != string::npos) {
                        correctMovies.push_back((*iter).first);
                        break;
                    }
                }
            }
        }
    }


    vector<pair<double, string>> average = averageReviewSpecificMovies(correctMovies);

    sort(average.begin(), average.end());

    if (average.size() < 10) {
        for (unsigned int i = 0; i < average.size(); i++) {
            float rating = round(average.at(i).first * 10.0) / 10.0;
            cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
        }
    }

    else {
        for (unsigned int i = 0; i < 10; i++) {
            float rating = round(average.at(i).first * 10.0) / 10.0;
            cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
        }
    }
}
void bestGenre(string genre);
void worstGenre(string genre);
void topOverall();
void worstOverall();

int main()
{
    //---------------------------MAP IMPLEMENTATION----------------------------//
    //Variables
    Map movieNames;

    readIntoMap("movies.csv", movieNames);



}