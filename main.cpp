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
        this->color = RED;
    }
};

class Map
{
private:
    Node* root;

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
    void suggestionsBasedOnMovie(string name, vector<Node*>& traversePtrs); //Option 1
    void suggestionsBasedOnGenre(string genre, vector<Node*>& traversePtrs); //Options 2
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


    //Normal insertion
    root = BSTinsertion(root, node);

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
        node->left->parent = node;
    }

    node_left->parent = node->parent;

    if (node->parent == nullptr)
    {
        root = node_left;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = node_left;
    }
    else
    {
        node->parent->right =node_left;
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

    if (node->right != nullptr)
    {
        node->right->parent = node;
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
    Node *parentNode = nullptr;
    Node *grandparentNode = nullptr;

    //Traverses the tree and does rotations as necessary
    while(node != root && node->parent != nullptr && node->parent->color == RED && node->color != BLACK)
    {
        parentNode = node->parent;
        grandparentNode = node->parent->parent;

        //Case I: Parent of node is the left child of Grandparent node
        if (parentNode == grandparentNode->left)
        {
            Node* uncleNode = grandparentNode->right;

            //Case 1: If uncle of node is also red, only recoloring is needed
            if (uncleNode != nullptr && uncleNode->color == RED)
            {
                //Recolor the nodes accordingly
                uncleNode->color = BLACK;
                grandparentNode->color = RED;
                parentNode->color = BLACK;

                //Set node equal to grandparent node
                node = grandparentNode;
            }
            else
            {
                //Case 2: Node is right child of parent (LEFT RIGHT ROTATION)
                if (node == parentNode->right)
                {
                    //Rotate tree to the left
                    rotateLeft(root, parentNode);
                    //Re-assign what nodes point to
                    node = parentNode;
                    parentNode = node->parent;
                }

                //Case 3: Node is left child of parent (RIGHT ROTATION)
                rotateRight(root,grandparentNode);

                //Swap the colors of the parent and grandparent nodes
                swap(parentNode->color,grandparentNode->color);

                //Set node equal to the parent node
                node = parentNode;
            }
        }
        //Case II: Parent of node is the right child of Grandparent node
        else
        {
            Node *uncleNode = grandparentNode->left;

            //Case 1: If uncle of node is also red, only recoloring is needed
            if (uncleNode != nullptr && uncleNode->color == RED)
            {
                //Recolor the nodes accordingly
                uncleNode->color = BLACK;
                grandparentNode->color = RED;
                parentNode->color = BLACK;

                //Set node equal to grandparent node
                node = grandparentNode;
            }
            else
            {
                //Case 2: Node is left child of parent (RIGHT LEFT ROTATION)
                if (node == parentNode->left)
                {
                    //Rotate right about the parentNode
                    rotateRight(root, parentNode);

                    //Reassigns what nodes point to
                    node = parentNode;
                    parentNode = node->parent;
                }

                //Case 3: Node is right child of parent (LEFT ROTATION)
                rotateLeft(root, grandparentNode);

                //Swap colors of the parent node and grandparent node
                swap(parentNode->color, grandparentNode->color);

                //Set node equal to parent node
                node = parentNode;
            }
        }
    }
    //Root is always BLACK
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

void Map::suggestionsBasedOnMovie(string name, vector<Node*>& traversePtrs)
{
    //Variables
    Node* movieNode = searchMovie(root, name);
    vector<pair<string, float>> namesOfMovies;
    set<pair<string,vector<pair<int,float>>>> similarMovieGenres;

    if (movieNode != nullptr)
    {
        //Finds movies with similar genres and add it to a set
        for (int i = 0; i < movieNode->genres.size(); i++)
        {
            for (int j = 0; j < traversePtrs.size(); j++)
            {
                if (traversePtrs[j]->movieName != name)
                {
                    //Iterates through the reviews of the current Node in traversePtrs
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

        sort(namesOfMovies.begin(),namesOfMovies.end(), [] (const auto &movie1, const auto &movie2) {return movie1.second > movie2.second;});

        //Print out top 10 movies
        for (int i = 0; i < 10; i++)
        {
            cout <<  namesOfMovies[i].first << " " << "Rating: " << namesOfMovies[i].second <<endl;
        }
    }
    else
    {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }
}

Node* Map::searchMovie(Node* root, string name)
{
    if (root->left == nullptr && root->right == nullptr)
    {
        return nullptr;
    }

    if (root->movieName == name)
    {
        return root;
    }
    else
    {
        if (name < root->movieName)
        {
            return searchMovie(root->left, name);
        }
        else
        {
            return searchMovie(root->right, name);
        }
    }

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
        cout << " " << moviesList[i].first << " " << "Rating: " << moviesList[i].second << endl;
    }
}

//---------------------HELPER FUNCTIONS------------------------//

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

vector<pair<int, vector<pair<int, float>>>> getReviewsFromMovie(string fileName)
{
    //Variables
    vector<pair<int, vector<pair<int, float>>>> reviews;
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
        vector<pair<int, vector<pair<int, float>>>> userReviews = getReviewsFromMovie("ratings 2.csv");

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
            for (int i = 0; i < tempGenres.size(); i++)
            {
                if (tempGenres.at(i) == '|' || i == tempGenres.size() - 1) {
                    genres.push_back(tempString);
                    tempString = "";
                    continue;
                }
                tempString += tempGenres.at(i);
            }


            //Insert the node into the map (Red-Black Tree)
            map.insertNode(movieName,movieID,userReviews[count].second,genres);

            count++;
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

    movieNames.suggestionsBasedOnMovie("Toy Story (1995)", traversePtrs); //Option 1
    movieNames.suggestionsBasedOnGenre("Comedy", traversePtrs); //Option 2
}