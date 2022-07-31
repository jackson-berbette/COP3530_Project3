#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <map>
#include <chrono>
#include <cmath>

using namespace std;

// Class to store movie objects
class Node {
public:

    string name; // movie title
    string id; // movie id
    vector<pair<int, float>> reviews; // all reviews associated with movie
    vector<string> genres; // all genres associated with movie
};

// Class to create a min heap
class minHeap {
private:
    Node *array; // array to store movie objects
    int capacity; // store capacity of array
    int size; // store current size of array
    vector<pair<int, pair<string, float>>> reviewsByUser; // store all reviews sorted by userID instead of movie

public:
    minHeap(int capacityNew);
    int getParent(int node);
    void insertNode(Node* node);
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
    void updateUserReviews(vector<pair<int, pair<string, float>>>);
};

// get the parent index of current node
int minHeap::getParent(int node) {
    return (node - 1) / 2;
}

// create a minHeap object and intialize it
minHeap::minHeap(int capacityNew) {
    size = 0;
    capacity = capacityNew;
    array = new Node[capacityNew];
}

// function to insert a node into the min heap
void minHeap::insertNode(Node* node) {

    // if the node won't fit, print error message
    if (size == capacity) {
        cout << "error: need bigger capacity" << endl;
        return;
    }

    // increment size and set array index equal to node
    size++;
    int i = size - 1;
    array[i] = *node;

    // movie node into right place in min heap by swapping if parent is larger
    while (i != 0 && array[getParent(i)].name > array[i].name) {
        Node temp = array[getParent(i)];
        array[getParent(i)] = array[i];
        array[i] = temp;
        i = getParent(i);
    }
}

// update the reviews by user vector in minHeap
void minHeap::updateUserReviews(vector<pair<int, pair<string, float>>> vec) {
    reviewsByUser = vec;
}

// function to get movie recommendations from user reviews
void minHeap::goodMovieReview(string movie) {

    // start timer
    auto start = chrono::high_resolution_clock::now();

    // find the index of movie from user input
    int index = -1;
    for (unsigned int i = 0; i < size; i++) {
        if (array[i].name == movie) {
            index = i;
            break;
        }
    }

    // if movie is not valid, send error message
    if (index == -1) {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    // initialize vector of high reviews
    vector<pair<float, int>> highReviews;

    // for all reviews of user input movie, find the ones greater than or equal to 4.0
    for (unsigned int j = 0; j < array[index].reviews.size(); j++) {
        if (array[index].reviews.at(j).second >= 4.0) {
            highReviews.push_back(make_pair(array[index].reviews.at(j).second, array[index].reviews.at(j).first));
        }
    }


    // sort the reviews from highest to lowest
    sort(highReviews.begin(), highReviews.end(), greater<>());

    // intialize a temporary vector
    vector<pair<float, int>> temp;

    // if there are more than 100 reviews greater than or equal to 4.0, only keep the first 100
    if (highReviews.size() > 100) {
        for (unsigned int q = 0; q < 100; q++) {
            temp.push_back(make_pair(highReviews.at(q).first, highReviews.at(q).second));
        }

        // put these 100 values back into the vector
        highReviews.clear();
        highReviews = temp;
        temp.clear();
    }

    // create a vector to store the reviews by each user associated with movie titles
    vector<pair<string, float>> reviewsByTitle;

    // for every userID in highReviews
    for (unsigned int k = 0; k < highReviews.size(); k++) {
        // for every review in the vector sorted by user
        for (unsigned int l = 0; l < reviewsByUser.size(); l++) {
            // if the userIDs match, the movie being reviewed isn't the original movie, and the review is greater than or equal to 4.0, push this review's information into the vector
            if (reviewsByUser.at(l).first == highReviews.at(k).second && reviewsByUser.at(l).second.first != array[index].id && reviewsByUser.at(l).second.second >= 4.0) {
                reviewsByTitle.push_back(make_pair(reviewsByUser.at(l).second.first, reviewsByUser.at(l).second.second));
            }
        }
    }

    // create a temporary map for easy access
    map<string, vector<float>> averageReviewsTemp;

    // for each of the reviews in the above vector, push them into the map to store all the review values for each movie in individual vectors
    for (unsigned int m = 0; m < reviewsByTitle.size(); m++) {
        averageReviewsTemp[reviewsByTitle.at(m).first].push_back(reviewsByTitle.at(m).second);
    }

    map <float, string, greater<>> averageReviews;

    map<string, vector<float>>::iterator iterTemp;

    // iterate through the map created above and add up all the reviews for each movie, dividing them by the total number of reviews to get an average
    for (iterTemp = averageReviewsTemp.begin(); iterTemp != averageReviewsTemp.end(); iterTemp++) {
        float total = 0;
        for (unsigned int r = 0; r < (*iterTemp).second.size(); r++) {
            total += (*iterTemp).second.at(r);
        }

        // store average movie reviews in a map
        averageReviews[(total/(float) (*iterTemp).second.size())] = (*iterTemp).first;
    }

    // iterate through this final map and find the movie titles associated with each review, and print the top 10
    map<float, string, greater<>>::iterator iter = averageReviews.begin();

    // if there are less than 10 movies, print them all out
    if (averageReviews.size() < 10) {
        int i = 0;
        for (iter = averageReviews.begin(); iter != averageReviews.end(); iter++) {
            for (unsigned int w = 0; w < size; w++) {
                if (array[w].id == (*iter).second) {
                    float rating = round((*iter).first * 10.0) / 10.0;
                    cout << i + 1 << ". " << array[w].name << ", Rating: " << fixed << setprecision(1) << rating << endl;
                    break;
                }
            }
            i++;
        }
    }

    // otherwise, print 10 with highest reviews
    else {
        for (unsigned int i = 0; i < 10; i++) {
            for (unsigned int w = 0; w < size; w++) {
                if (array[w].id == (*iter).second) {
                    float rating = round((*iter).first * 10.0) / 10.0;
                    cout << i + 1 << ". " << array[w].name << ", Rating: " << fixed << setprecision(1) << rating << endl;
                    break;
                }
            }
            iter++;
        }
    }

    // stop timer and print duration of function
    auto stop = chrono::high_resolution_clock::now();
    auto duration = (stop - start);
    auto time = chrono::duration_cast<chrono::seconds>(duration);

    cout << "This process took " << time.count() << " seconds!" << endl;

}

void minHeap::badMovieReview(string movie) {

    // start timer
    auto start = chrono::high_resolution_clock::now();

    // find the index of the movie from user input
    int index = -1;
    for (unsigned int i = 0; i < size; i++) {
        if (array[i].name == movie) {
            index = i;
            break;
        }
    }

    // if movie was not found, print error message
    if (index == -1) {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    // create a vector to store the lowest reviews
    vector<pair<float, int>> lowReviews;

    // for all reviews of user input movie, find the ones less than or equal to 2.0
    for (unsigned int j = 0; j < array[index].reviews.size(); j++) {
        if (array[index].reviews.at(j).second <= 2.0) {
            lowReviews.push_back(make_pair(array[index].reviews.at(j).second, array[index].reviews.at(j).first));
        }
    }

    // sort the reviews from lowest to highest
    sort(lowReviews.begin(), lowReviews.end());

    vector<pair<float, int>> temp;

    // if there are more than 100 reviews, pick the 100 lowest ones
    if (lowReviews.size() > 100) {
        for (unsigned int q = 0; q < 100; q++) {
            temp.push_back(make_pair(lowReviews.at(q).first, lowReviews.at(q).second));
        }

        // put 100 lowest back into original vector
        lowReviews.clear();
        lowReviews = temp;
        temp.clear();
    }

    // create a vector to store the reviews by each user associated with the movie titles
    vector<pair<string, float>> reviewsByTitle;

    // for every userID in lowReviews
    for (unsigned int k = 0; k < lowReviews.size(); k++) {
        // for every review in the vector sorted by user
        for (unsigned int l = 0; l < reviewsByUser.size(); l++) {
            // if the userIDs match, the movie being reviewed isn't the original movie, and the review is less than or equal to 2.0, push this review's information into the vector
            if (reviewsByUser.at(l).first == lowReviews.at(k).second && reviewsByUser.at(l).second.first != array[index].id && reviewsByUser.at(l).second.second <= 2.0) {
                reviewsByTitle.push_back(make_pair(reviewsByUser.at(l).second.first, reviewsByUser.at(l).second.second));
            }
        }
    }

    // create a temporary map for easy access
    map<string, vector<float>> averageReviewsTemp;

    // for each of the reviews in the above vector, push them into the map to store all the reviews for each movie in individual vectors
    for (unsigned int m = 0; m < reviewsByTitle.size(); m++) {
        averageReviewsTemp[reviewsByTitle.at(m).first].push_back(reviewsByTitle.at(m).second);
    }

    // create a second map to store the average values
    map <float, string> averageReviews;

    map<string, vector<float>>::iterator iterTemp;

    // iterate through the map above and add up all review values for each movie, dividing them by the number of reviews to get an average
    for (iterTemp = averageReviewsTemp.begin(); iterTemp != averageReviewsTemp.end(); iterTemp++) {
        float total = 0;
        for (unsigned int r = 0; r < (*iterTemp).second.size(); r++) {
            total += (*iterTemp).second.at(r);
        }

        // store the average review in map
        averageReviews[(total/(float) (*iterTemp).second.size())] = (*iterTemp).first;
    }

    // iterate through this map to print lowest average reviewed movies
    map<float, string>::iterator iter = averageReviews.begin();

    // if the number of movies is less than 10, print them all
    if (averageReviews.size() < 10) {
        int i = 0;
        for (iter = averageReviews.begin(); iter != averageReviews.end(); iter++) {
            for (unsigned int w = 0; w < size; w++) {
                if (array[w].id == (*iter).second) {
                    float rating = round((*iter).first * 10.0) / 10.0;
                    cout << i + 1 << ". " << array[w].name << ", Rating: " << fixed << setprecision(1) << rating << endl;
                    break;
                }
            }
            i++;
        }
    }

    // otherwise, print the lowest 10 reviewed movies
    else {
        for (unsigned int i = 0; i < 10; i++) {
            for (unsigned int w = 0; w < size; w++) {
                if (array[w].id == (*iter).second) {
                    float rating = round((*iter).first * 10.0) / 10.0;
                    cout << i + 1 << ". " << array[w].name << ", Rating: " << fixed << setprecision(1) << rating << endl;
                }
            }
            iter++;
        }
    }

    // stop the timer and print duration
    auto stop = chrono::high_resolution_clock::now();
    auto duration = (stop - start);
    auto time = chrono::duration_cast<chrono::seconds>(duration);

    cout << "This process took " << time.count() << " seconds!" << endl;

}

// function to calculate the average review for every movie in database
vector<pair<double, string>> minHeap::averageReviewByMovie() {

    // create a temporary map
    map<string, vector<double>> tempReviews;

    // for every movie in the database, store each of their reviews in a vector in the map
    for (unsigned int i = 0; i < size; i++) {
        for (unsigned int j = 0; j < array[i].reviews.size(); j++) {
            tempReviews[array[i].name].push_back(array[i].reviews.at(j).second);
        }
    }


    // iterate through the map and add all the values associated with each movie, dividing them by the number of reviews to get an average
    map<string, vector<double>>::iterator iter;
    vector<pair<double, string>> average;
    for (iter = tempReviews.begin(); iter != tempReviews.end(); iter++) {
        double total = 0;
        if ((*iter).second.size() > 100) {
            for (unsigned int k = 0; k < (*iter).second.size(); k++) {
                total += (double) (*iter).second.at(k);
            }

            // push this average into a vector
            average.push_back(make_pair((total / (double) (*iter).second.size()), (*iter).first));
        }
    }

    // return the vector
    return average;
}

// function to get average reviews for specific movies
vector<pair<double, string>> minHeap::averageReviewSpecificMovies(vector<int> movies) {

    // get indices of movies in array from input

    // for each of these specific movies, store their review data in a map of vectors
    map<string, vector<double>> tempReviews;
    for (unsigned int i = 0; i < movies.size(); i++) {
        for (unsigned int j = 0; j < array[movies.at(i)].reviews.size(); j++) {
            tempReviews[array[movies.at(i)].name].push_back(array[movies.at(i)].reviews.at(j).second);
        }
    }

    // for every value in the map, calculate its average by adding all the reviews and dividing by the number of them
    map<string, vector<double>>::iterator iter;
    vector<pair<double, string>> average;
    for (iter = tempReviews.begin(); iter != tempReviews.end(); iter++) {
        double total = 0;
        if ((*iter).second.size() > 100) {
            for (unsigned int k = 0; k < (*iter).second.size(); k++) {
                total += (double) (*iter).second.at(k);
            }

            // store these averages in a vector
            average.push_back(make_pair((total / (double) (*iter).second.size()), (*iter).first));
        }
    }

    // return the vector
    return average;
}

// function to find highly rated movies in the same genre as a user input movie
void minHeap::goodMovieGenre(string movie) {

    // start the timer for the function
    auto start = chrono::high_resolution_clock::now();

    // find the index of the user input movie
    int index = -1;
    for (unsigned int i = 0; i < size; i++) {
        if (array[i].name == movie) {
            index = i;
            break;
        }
    }

    // if the movie cannot be found, print an error message
    if (index == -1) {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    // create a vector for movies with at least one genre in common with the original movie
    vector<int> correctGenreIndices;

    // for every movie in the database
    for (unsigned int i = 0; i < size; i++) {
        // if the movie is not the one the user typed in
        if (i != index) {
            // go through current movie's genres
            for (unsigned int j = 0; j < array[i].genres.size(); j++) {
                // for each of the genres associated with the target movie
                for (unsigned int k = 0; k < array[index].genres.size(); k++) {
                    // if the genres are the same, push that movie into the vector
                    if (array[i].genres.at(j).find(array[index].genres.at(k)) != string::npos) {
                        correctGenreIndices.push_back(i);
                        break;
                    }
                }
            }
        }
    }

    // create a vector to store average reviews for each of these movies
    vector<pair<double, string>> average = averageReviewSpecificMovies(correctGenreIndices);

    // sort the vector from largest to smallest
    sort(average.begin(), average.end(), greater<>());

    // if there are less than 10 movies, print them all
    if (average.size() < 10) {
        for (unsigned int i = 0; i < average.size(); i++) {
            float rating = round(average.at(i).first * 10.0) / 10.0;
            cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
        }
    }

    // otherwise, print the 10 highest reviewed movies
    else {
        for (unsigned int i = 0; i < 10; i++) {
            float rating = round(average.at(i).first * 10.0) / 10.0;
            cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
        }
    }

    // stop the timer and print the duration of the function
    auto stop = chrono::high_resolution_clock::now();
    auto duration = (stop - start);
    auto time = chrono::duration_cast<chrono::seconds>(duration);

    cout << "This process took " << time.count() << " seconds!" << endl;
}

// function to find low rated movies in the same genre as a user input movie
void minHeap::badMovieGenre(string movie) {

    // start the timer for the function
    auto start = chrono::high_resolution_clock::now();

    // search for the user input movie in the heap
    int index = -1;
    for (unsigned int i = 0; i < size; i++) {
        if (array[i].name == movie) {
            index = i;
            break;
        }
    }

    // if the movie cannot be found, print error message
    if (index == -1) {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    // create a vector for movies with at least one genre in common with the original movie
    vector<int> correctGenreIndices;

    // for every movie in the database
    for (unsigned int i = 0; i < size; i++) {
        // if the movie is not the one the user typed in
        if (i != index) {
            // go through the current movie's genres
            for (unsigned int j = 0; j < array[i].genres.size(); j++) {
                // for each of the genres associated with the target movie
                for (unsigned int k = 0; k < array[index].genres.size(); k++) {
                    // if the genres are the same, push that movie into the vector
                    if (array[i].genres.at(j).find(array[index].genres.at(k)) != string::npos) {
                        correctGenreIndices.push_back(i);
                        break;
                    }
                }
            }
        }
    }

    // create a vector and store the average reviews for each of the movies found above
    vector<pair<double, string>> average = averageReviewSpecificMovies(correctGenreIndices);

    // sort the vector from lowest to highest
    sort(average.begin(), average.end());

    // if there are less than 10 movies, print them all
    if (average.size() < 10) {
        for (unsigned int i = 0; i < average.size(); i++) {
            float rating = round(average.at(i).first * 10.0) / 10.0;
            cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
        }
    }

    // otherwise, print the 10 lowest rated movies
    else {
        for (unsigned int i = 0; i < 10; i++) {
            float rating = round(average.at(i).first * 10.0) / 10.0;
            cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
        }
    }

    // stop the timer and print the duration for this function
    auto stop = chrono::high_resolution_clock::now();
    auto duration = (stop - start);
    auto time = chrono::duration_cast<chrono::seconds>(duration);

    cout << "This process took " << time.count() << " seconds!" << endl;
}

// function to print highest rated movies in a specific genre
void minHeap::bestGenre(string genre) {

    // start the timer for function
    auto start = chrono::high_resolution_clock::now();

//    vector<string> validGenres = {"Action", "Adventure", "Animation", "Children",
//                                  "Comedy", "Crime", "Drama", "Fantasy", "Horror",
//                                  "Thriller", "Mystery", "Sci-Fi", "Thriller",
//                                  "IMAX", "Documentary", "Musical", "Romance",
//                                  "War", "Western", "Film-Noir"};

    // create a vector to store indices of all movies with desired genre
    vector<int> indices;

    // for every movie in the database
    for (unsigned int i = 0; i < size; i++) {
        // for all of the genres listed for the current movie
        for (unsigned int j = 0; j < array[i].genres.size(); j++) {
            // if one of its genres matched the desired genre, push its index into the vector
            if (array[i].genres.at(j) == genre) {
                indices.push_back(i);
                break;
            }
        }
    }

    // if the genre was not found, print error message
    if (indices.size() == 0) {
        cout << "Invalid Genre!" << endl;
        return;
    }

    // get the average reviews for each movie in the vector
    vector<pair<double, string>> average = averageReviewSpecificMovies(indices);

    // sort the reviews from highest to lowest
    sort(average.begin(), average.end(), greater<>());

    // if the number of movies is less than 10, print them all
    if (average.size() < 10) {
        for (unsigned int i = 0; i < average.size(); i++) {
            float rating = round(average.at(i).first * 10.0) / 10.0;
            cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
        }
    }

    // otherwise, print the top 10 rated movies
    else {
        for (unsigned int i = 0; i < 10; i++) {
            float rating = round(average.at(i).first * 10.0) / 10.0;
            cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
        }
    }

    // stop the timer and print the duration of the function
    auto stop = chrono::high_resolution_clock::now();
    auto duration = (stop - start);
    auto time = chrono::duration_cast<chrono::seconds>(duration);

    cout << "This process took " << time.count() << " seconds!" << endl;
}

// function to print lowest rated movies in certain genre
void minHeap::worstGenre(string genre) {

    // start timer for function
    auto start = chrono::high_resolution_clock::now();

    // create a vector to store indices
    vector<int> indices;

    // for every movie in the database
    for (unsigned int i = 0; i < size; i++) {
        // for every genre of the current movie
        for (unsigned int j = 0; j < array[i].genres.size(); j++) {
            // if one of the genres is the desired genre, push that movie's index into the vector
            if (array[i].genres.at(j) == genre) {
                indices.push_back(i);
                break;
            }
        }
    }

    // if the genre was not found, print error message
    if (indices.size() == 0) {
        cout << "Invalid Genre!" << endl;
        return;
    }

    // get the average review for each movie in the above vector
    vector<pair<double, string>> average = averageReviewSpecificMovies(indices);

    // sort the reviews from lowest to highest
    sort(average.begin(), average.end());

    // if there are less than 10 movies in the vector, print them all
    if (average.size() < 10) {
        for (unsigned int i = 0; i < average.size(); i++) {
            float rating = round(average.at(i).first * 10.0) / 10.0;
            cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
        }
    }

    // otherwise, print the top 10 lowest rated movies
    else {
        for (unsigned int i = 0; i < 10; i++) {
            float rating = round(average.at(i).first * 10.0) / 10.0;
            cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
        }
    }

    // stop the timer and print the duration of the function
    auto stop = chrono::high_resolution_clock::now();
    auto duration = (stop - start);
    auto time = chrono::duration_cast<chrono::seconds>(duration);

    cout << "This process took " << time.count() << " seconds!" << endl;

}

// print the top 10 rated movies across all genres
void minHeap::topOverall() {

    // start the timer for the function
    auto start = chrono::high_resolution_clock::now();

    // create a vector and set it equal to the average reviews for each movie
    vector<pair<double, string>> average = averageReviewByMovie();

    // sort this vector from highest to lowest
    sort(average.begin(), average.end(), greater<>());

    // print the top 10 movies
    for (unsigned int i = 0; i < 10; i++) {
        float rating = round(average.at(i).first * 10.0) / 10.0;
        cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
    }

    // stop the timer and print the duration
    auto stop = chrono::high_resolution_clock::now();
    auto duration = (stop - start);
    auto time = chrono::duration_cast<chrono::seconds>(duration);

    cout << "This process took " << time.count() << " seconds!" << endl;
}

// print the 10 worst rated movies across all genres
void minHeap::worstOverall() {

    // start the timer for the function
    auto start = chrono::high_resolution_clock::now();

    // create a vector and set it equal to the average reviews for each movie
    vector<pair<double, string>> average = averageReviewByMovie();

    // sort this vector from lowest to highest
    sort(average.begin(), average.end());

    // print the 10 lowest rated movies
    for (unsigned int i = 0; i < 10; i++) {
        float rating = round(average.at(i).first * 10.0) / 10.0;
        cout << i + 1 << ". " << average.at(i).second << ", Rating: " << fixed << setprecision(1) << rating << endl;
    }

    // stop the timer and print the duration of the function
    auto stop = chrono::high_resolution_clock::now();
    auto duration = (stop - start);
    auto time = chrono::duration_cast<chrono::seconds>(duration);

    cout << "This process took " << time.count() << " seconds!" << endl;

}


int main() {

    ifstream movieFile;
    movieFile.open("movies.csv");
    ifstream ratingsFile;
    ratingsFile.open("ratings 2.csv");
    minHeap heap = minHeap(58098);
    string temporary = "";
    getline(movieFile, temporary);
    getline(ratingsFile, temporary);

    map<int, vector<pair<int, float>>> tempReviews;
    vector<pair<int, pair<string, float>>> userReviews;

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

        tempReviews[movieID].push_back(make_pair(userID, rating));
        userReviews.push_back(make_pair(userID, make_pair(tempMovieID, rating)));

    }
    for (unsigned int i = 0; i < 58098; i++) {
        Node* temp = new Node;
        getline(movieFile, temp->id, ',');


        getline(movieFile, temp->name, ',');
        string tempName = "";
        if (temp->name.at(0) == '\"') {
            temp->name += ",";
            while (temp->name.at(temp->name.size() - 1) != '\"' && temp->name.find(')') == string::npos) {
                getline(movieFile, tempName, ',');
                temp->name += tempName;
            }
            temp->name = temp->name.substr(1, temp->name.size() - 2);
//            cout << temp->name << endl;
        }


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

    heap.updateUserReviews(userReviews);
    
//    heap.printHeap();

//    heap.printInOrder(0);
    heap.goodMovieReview("Toy Story (1995)");
    cout << endl;
    heap.badMovieReview("Toy Story (1995)");
    cout << endl;

    heap.goodMovieGenre("Toy Story (1995)");
    cout << endl;
    heap.badMovieGenre("Toy Story (1995)");
    cout << endl;

    heap.topOverall();
    cout << endl;
    heap.worstOverall();
    cout << endl;

    heap.bestGenre("Comedy");
    cout << endl;
    heap.worstGenre("Comedy");
    cout << endl;
    return 0;
}
