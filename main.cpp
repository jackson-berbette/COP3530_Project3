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

    Node* getRoot();
    void insertNode(string& movieName, int id, vector<pair<int,float>>& reviews, vector<string>& genres);
    Node* searchMovie(Node* root, string name);
    Node* searchMovieID(Node* root, int id);
    void updateReviewsByUser(vector<pair<int, pair<int, float>>> vec);
    void suggestionsBasedOnMovie(string name, vector<Node*>& traversePtrs); //Option 1
    void suggestionsBasedOnGenre(string genre, vector<Node*>& traversePtrs); //Option 2
    void avoidBasedOnMovie(string name, vector<Node*>& traversePtrs); //Option 5 (Inverse of 1)
    void avoidBasedOnGenre(string genre, vector<Node*>& traversePtrs); //Option 6 (Inverse of 2)
};

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
    Node* temp;
    while (node->parent->color == RED) {
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
}

float calculateAverageRatings(vector<pair<int, float>> reviews)
{
    //Variables
    float result = 0.0;

    for (int i = 0; i < reviews.size(); i++)
    {
        result += reviews[i].second;
    }

    return result / (float)reviews.size();
}

//OPTION 1
void Map::suggestionsBasedOnMovie(string name, vector<Node*>& traversePtrs)
{
    //Variables
    Node* movieNode = searchMovie(root, name);
    vector<pair<string, float>> namesOfMovies;
    set<pair<string,vector<pair<int,float>>>> similarMovieGenres;
    bool found = true;

    if (movieNode != nullptr)
    {
        //Finds movies with similar genres and add it to a set
        for (int i = 0; i < movieNode->genres.size(); i++)
        {
            //Iterate through EVERY MOVIE
            for (int j = 0; j < traversePtrs.size(); j++)
            {
                //Check if the movie is not the one inputted
                if (traversePtrs[j]->movieName != name)
                {
                    //Iterates through the reviews of the current MOVIE
                    for (int k = 0; k < traversePtrs[j]->genres.size(); k++)
                    {
                        if (movieNode->genres[i] == traversePtrs[j]->genres[k] && traversePtrs[j]->reviews.size() > 100)
                        {
                            //Puts it in a set so no copies of movies are made
                            similarMovieGenres.emplace(make_pair(traversePtrs[j]->movieName, traversePtrs[j]->reviews));
                        }
                    }
                }
            }
        }

        //Transfers from set to a vector that only contains movies that have more than 100 reviews
        for (auto it = similarMovieGenres.begin(); it != similarMovieGenres.end(); it++)
        {
            if (it->second.size() > 100)
            {
                namesOfMovies.emplace_back(make_pair(it->first, calculateAverageRatings(it->second)));
            }
        }

        sort(namesOfMovies.begin(),namesOfMovies.end(), [] (const auto &movie1, const auto &movie2) {return movie1.second > movie2.second;});

        //Print out top 10 movies
        for (int i = 0; i < 10; i++)
        {
            cout <<  i + 1 << ". " << namesOfMovies[i].first << ", Rating: " << setprecision(1) << namesOfMovies[i].second <<endl;
        }
    }
    else
    {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }
}

void Map::avoidBasedOnMovie(string name, vector<Node*>& traversePtrs)
{
    //Variables
    Node* movieNode = searchMovie(root, name);
    vector<pair<string, float>> namesOfMovies;
    set<pair<string,vector<pair<int,float>>>> similarMovieGenres;
    bool found = true;

    if (movieNode != nullptr)
    {
        //Finds movies with similar genres and add it to a set
        for (int i = 0; i < movieNode->genres.size(); i++)
        {
            //Iterate through EVERY MOVIE
            for (int j = 0; j < traversePtrs.size(); j++)
            {
                //Check if the movie is not the one inputted
                if (traversePtrs[j]->movieName != name)
                {
                    //Iterates through the reviews of the current MOVIE
                    for (int k = 0; k < traversePtrs[j]->genres.size(); k++)
                    {
                        if (movieNode->genres[i] == traversePtrs[j]->genres[k] && traversePtrs[j]->reviews.size() > 100)
                        {
                            //Puts it in a set so no copies of movies are made
                            similarMovieGenres.emplace(make_pair(traversePtrs[j]->movieName, traversePtrs[j]->reviews));
                        }
                    }
                }
            }
        }
        cout << similarMovieGenres.size() << endl;

        //Transfers from set to a vector that only contains movies that have more than 100 reviews
        for (auto it = similarMovieGenres.begin(); it != similarMovieGenres.end(); it++)
        {
            if (it->second.size() > 100)
            {
                namesOfMovies.emplace_back(make_pair(it->first, calculateAverageRatings(it->second)));
            }
        }

        sort(namesOfMovies.begin(),namesOfMovies.end(), [] (const auto &movie1, const auto &movie2) {return movie1.second < movie2.second;});

        //Print out top 10 movies
        for (int i = 0; i < 10; i++)
        {
            cout <<  i + 1 << ". " << namesOfMovies[i].first << ", Rating: " << setprecision(1) << namesOfMovies[i].second <<endl;
        }
    }
    else
    {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }
}

Node* Map::searchMovie(Node* root, string movie)
{
    if (root == nullptr || root->movieName == movie)
    {
        return root;
    }
    if (movie < root->movieName)
    {
        return searchMovie(root->left, movie);
    }
    return searchMovie(root->right, movie);
}

Node* Map::searchMovieID(Node* root, int id)
{
    if (root->id == id)
    {
        return root;
    }

    Node* temp = nullptr;
    if (root->left != nullptr)
    {
        temp = searchMovieID(root->left, id);
    }
    if (temp != nullptr)
    {
        return temp;
    }
    if (root->right != nullptr)
    {
        temp = searchMovieID(root->right, id);
    }
    return temp;
}

Node *Map::getRoot()
{
    return this->root;
}

void Map::suggestionsBasedOnGenre(string genre, vector<Node*>& traversePtrs)
{
    //Variables
    bool genreFound = false;
    vector<pair<string,float>> moviesList; //pair<movieName, AverageRating>

    for (int i = 0; i < traversePtrs.size(); i++)
    {
        for (int k = 0; k < traversePtrs[i]->genres.size(); k++)
        {
            if (genre == traversePtrs[i]->genres[k] && traversePtrs[i]->reviews.size() > 100)
            {
                genreFound = true;
                moviesList.emplace_back(make_pair(traversePtrs[i]->movieName, calculateAverageRatings(traversePtrs[i]->reviews)));
                break;
            }
        }
    }

    if (genreFound)
    {
        sort(moviesList.begin(),moviesList.end(), [] (const auto &movie1, const auto &movie2) {return movie1.second > movie2.second;});
    }
    else
    {
        cout << "Invalid Genre! Please enter a valid genre" << endl;
        return;
    }

    //Print out top 10 movies
    for (int i = 0; i < 10; i++)
    {
        cout <<  i + 1 << ". " << moviesList[i].first << ", Rating: " << setprecision(1) << moviesList[i].second <<endl;
    }
}

void Map::updateReviewsByUser(vector<pair<int, pair<int, float>>> vec)
{
    reviewsByUser = vec;
}

void Map::avoidBasedOnGenre(string genre, vector<Node *> &traversePtrs)
{
    //Variables
    bool genreFound = false;
    vector<pair<string,float>> moviesList; //pair<movieName, AverageRating>

    for (int i = 0; i < traversePtrs.size(); i++)
    {
        for (int k = 0; k < traversePtrs[i]->genres.size(); k++)
        {
            if (genre == traversePtrs[i]->genres[k] && traversePtrs[i]->reviews.size() > 100)
            {
                genreFound = true;
                moviesList.emplace_back(make_pair(traversePtrs[i]->movieName, calculateAverageRatings(traversePtrs[i]->reviews)));
                break;
            }
        }
    }

    if (genreFound)
    {
        sort(moviesList.begin(),moviesList.end(), [] (const auto &movie1, const auto &movie2) {return movie1.second < movie2.second;});
    }
    else
    {
        cout << "Invalid Genre! Please enter a valid genre" << endl;
        return;
    }

    //Print out top 10 movies
    for (int i = 0; i < 10; i++)
    {
        cout <<  i + 1 << ". " << moviesList[i].first << ", Rating: " << setprecision(1) << moviesList[i].second <<endl;
    }
}


//---------------------READING IN FILES------------------------//

vector<Node*> getNodes(Node* root,vector<Node*>& vectors)
{
    vector<Node*> movieNodes = vectors;

    if (root != nullptr)
    {
        movieNodes = getNodes(root->left, movieNodes);
        movieNodes.push_back(root);
        movieNodes = getNodes(root->right, movieNodes);
    }

    return movieNodes;
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

            if (visited.find(movieID) != visited.end())
            {
                reviews.at(visited[movieID]).second.emplace_back(make_pair(userID, rating));
            }
            else
            {
                vector<pair<int, float>> temp;
                temp.push_back(make_pair(userID, rating));
                reviews.push_back(make_pair(movieID, temp));
                visited[movieID] = reviews.size() - 1;
            }

            reviewsByUser.push_back(make_pair(userID, make_pair(movieID, rating)));

        }
    }
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
        sort(userReviews.begin(), userReviews.end());


        for (unsigned int j = 0; j < 58098; j++) {
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
            if (tempMovieName.at(0) == '\"') {
                int index = 0;
                for (int i = 1; i < tempMovieName.size(); i++) {
                    if (tempMovieName.at(i) == '\"') {
                        index = i + 2;
                        break;
                    }
                    movieName += tempMovieName.at(i);
                }
                tempGenres = tempMovieName.substr(index, tempMovieName.size());
            }
                //If the movie title does not start with quotation
            else {
                int index = 0;
                for (int i = 0; i < tempMovieName.size(); i++) {
                    if (tempMovieName.at(i) == ',') {
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
            map.insertNode(movieName, movieID, temporaryVec, genres);
        }
    }
}

int main()
{
    //---------------------------MAP IMPLEMENTATION----------------------------//
    //Variables
    Map movieNames;

    //Read in values from the database
    readIntoMap("movies.csv", movieNames);

    //TraversePtrs contains all nodes from the tree
    vector<Node*> traversePtrs = getNodes(movieNames.getRoot(), traversePtrs);

    //------------METHOD CALLS------------//
    movieNames.suggestionsBasedOnMovie("Toy Story (1995)", traversePtrs); //Option 1
    //movieNames.suggestionsBasedOnGenre("Comedy", traversePtrs); //Option 2
    //movieNames.avoidBasedOnMovie("Toy Story (1995)", traversePtrs); //Option 5
    //movieNames.avoidBasedOnGenre("Comedy", traversePtrs); //Option 6

}