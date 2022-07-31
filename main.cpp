#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <iomanip>
#include <set>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

//-----------------------------START OF MAP IMPLEMENTATION-------------------------------//

//-----------MOVIE CLASS---------//
class Movie
{
private:
    string movieName;
    int movieID;
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

//---------END OF MOVIE CLASS----------//

//---------HELPER FUNCTIONS------------//
map<int, vector<pair<int, float>>> readInReviews(string fileName, map<int, vector<pair<int, float>>>& reviews) //map<movieID, vector<pair<userID, rating>>>
{
    //Variables
    map<int,vector<pair<int,float>>> tempMap;

    ifstream inFile(fileName);

    if (inFile.is_open())
    {
        //1.Read heading data from the file
        string lineFromFile;
        getline(inFile, lineFromFile);

        while (getline(inFile, lineFromFile))
        {
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

            tempMap[movieID].push_back(make_pair(userID,rating));
        }
    }
    return tempMap;
}

void readInMovies(string fileName, map<int,Movie>& movieIDs, map<int, vector<pair<int, float>>>& tempReviews, map<string, Movie>& movieNames)
{
    int count = 0;

    ifstream inFile(fileName);

    if (inFile.is_open())
    {
        //1.Read heading data from the file
        string lineFromFile;
        getline(inFile, lineFromFile);

        while (getline(inFile, lineFromFile))
        {
            istringstream  stream(lineFromFile);

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

            //Get userID AND reviews for specific movie
            vector<pair<int,float>> reviews = tempReviews[movieID]; //vector<pair<userID,rating>>

            //Get movie name
            getline(stream,tempMovieName);

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
                tempGenres = tempMovieName.substr(index,tempMovieName.size());
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
                tempGenres = tempMovieName.substr(index,tempMovieName.size());
            }

            //Fills up the Genre vector
            string tempString;
            for (int i = 0; i < tempGenres.size(); i++)
            {
                if (tempGenres.at(i) == '|' || i == tempGenres.size() - 1)
                {
                    genres.push_back(tempString);
                    tempString = "";
                    continue;
                }
                tempString += tempGenres.at(i);
            }

            Movie newMovie(movieName,movieID,genres,reviews);

            movieIDs.emplace(movieID,newMovie);
            movieNames.emplace(movieName,newMovie);
        }
    }
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

float calculateHighAverageRatings(vector<pair<int, float>> reviews)
{
    //Variables
    float result = 0.0;

    for (int i = 0; i < reviews.size(); i++)
    {
        if (reviews[i].second >= 4.0)
        {
            result += reviews[i].second;
        }
    }

    return result / (float)reviews.size();
}

vector<pair<string,float>> similarGenre(string movie, vector<string>& genres, map<string, Movie> movieNames) //Returns string of movie names
{
    //Variables
    vector<pair<string, float>> namesOfMovies;
    set<pair<string,vector<pair<int,float>>>> names;

    for (int i = 0; i < genres.size(); i++)
    {
        for (auto it = movieNames.begin(); it != movieNames.end(); it++)
        {
            if (it->first != movie)
            {
                for (int j = 0; j < it->second.getGenres().size(); j++)
                {
                    if (genres[i] == it->second.getGenres()[j] && it->second.getReviews().size() > 100)
                    {
                        //Puts it in a set so no copies of movies are made
                        names.emplace(make_pair(it->first,it->second.getReviews()));
                    }
                }
            }
        }
    }

    //Transfers from set to a vector that only contains movies that have more than 100 reviews
    for (auto it = names.begin(); it != names.end(); it++)
    {
        if (movieNames[it->first].getReviews().size() > 100)
        {
            namesOfMovies.emplace_back(make_pair(it->first, calculateAverageRatings(it->second)));
        }
    }

    sort(namesOfMovies.begin(),namesOfMovies.end(), [] (const auto &movie1, const auto &movie2) {return movie1.second > movie2.second;});

    /*vector<pair<string,float>> temp;

    if (namesOfMovies.size() > 100)
    {
        for (int i = 0; i < 100; i++)
        {
            temp.push_back(namesOfMovies[i]);
        }

        namesOfMovies.clear();
        namesOfMovies = temp;
        temp.clear();
    }*/

    return namesOfMovies;
}

float getAverageWithMovieList(string movie, vector<pair<string,float>>& moviesList)
{
    float sum = 0.0;
    //map<string,float> result;
    //vector<pair<string,float>> returnVector;
    float count = 0.0;

    for (int i = 0; i < moviesList.size(); i++)
    {
        if (moviesList[i].first == movie)
        {
            sum += moviesList[i].second;
            count++;
        }
    }

    return sum / count;
}

map<int, vector<pair<string,float>>> getMovieObjectsFromUserID(string fileName, map<int, vector<pair<string, float>>>& reviews, map<int, Movie>& movieIDs)
{
    //Variables
    map<int,vector<pair<string,float>>> tempMap;

    ifstream inFile(fileName);

    if (inFile.is_open())
    {
        //1.Read heading data from the file
        string lineFromFile;
        getline(inFile, lineFromFile);

        while (getline(inFile, lineFromFile))
        {
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

            tempMap[userID].push_back(make_pair(movieIDs[movieID].getMovieName(),rating));
        }
    }
    return tempMap;
}

vector<pair<string,float>> getReviewsFromMovie(string movie, map<string, Movie> movieNames, map<int, vector<pair<int, float>>>& mapReviews, map<int, Movie>& movieIDs) //Return pair<userID,review they gave on movie>>
{
    //Variables
    map<int, vector<pair<string, float>>> userIDsRatingsFromOtherMovies; //map<userID, vector<pair<movieName, review>>>
    vector<pair<int,float>> userIDsFromReviews; //vector<pair<UserID,rating they gave to movie> (ABOVE 4)
    vector<pair<string,float>> moviesList; //pair<movieName, normal rating>
    vector<pair<string,float>> finalList;
    map<int, vector<pair<string, float>>> UserIDsWithNamesAndRating = getMovieObjectsFromUserID("ratings 2.csv", UserIDsWithNamesAndRating, movieIDs);

    //Goes through the specific movie object and go through each review and compare to see if its more than 4.0
    for (int i = 0; i < movieNames[movie].getReviews().size(); i++)
    {
        if (movieNames[movie].getReviews()[i].second >= 4.0)
        {
            //Stores the UserID for the movie input that is over 4 (Used to later find what other movies they liked
            userIDsFromReviews.push_back(movieNames[movie].getReviews()[i]);
        }
    }

    //Sort the reviews and cut it down to only 100 reviews
    sort(userIDsFromReviews.begin(),userIDsFromReviews.end(), [] (const auto &movie1, const auto &movie2) {return movie1.second > movie2.second;});

    //cut it down to only 100 reviews
    vector<pair<int,float>> temp; //<UserID, rating>>

    if (userIDsFromReviews.size() > 100)
    {
        for (int i = 0; i < 100; i++)
        {
            temp.push_back(userIDsFromReviews[i]);
        }

        userIDsFromReviews.clear();
        userIDsFromReviews = temp;
        temp.clear();
    }

    /*//Use the userID to see what other movies they reviewed that were above 4.0
    for (int i = 0; i < userIDsFromReviews.size(); i++)
    {
        for (auto it = movieNames.begin(); it != movieNames.end(); it++)
        {
            for (int j = 0; j < it->second.getReviews().size(); j++)
            {
                if (userIDsFromReviews[i].first == it->second.getReviews()[j].first && it->second.getReviews()[j].second >= 4.0)
                {
                    userIDsRatingsFromOtherMovies[userIDsFromReviews[i].first].push_back(make_pair(it->first,it->second.getReviews()[j].second));
                }
            }
        }
    }*/

    //Use the userID to see what other movies they reviewed that were above 4.0
    for (int i = 0; i < userIDsFromReviews.size(); i++)
    {
        if (UserIDsWithNamesAndRating[userIDsFromReviews[i].first][i].first != movie)
        {
            userIDsRatingsFromOtherMovies[userIDsFromReviews[i].first].push_back(make_pair(UserIDsWithNamesAndRating[userIDsFromReviews[i].first][i].first, UserIDsWithNamesAndRating[userIDsFromReviews[i].first][i].second));
        }
    }

    //Converts the map to movieList that stores <movieName, normal rating>
    /*for (auto it = userIDsRatingsFromOtherMovies.begin(); it != userIDsRatingsFromOtherMovies.end(); it++)
    {
        for (int i = 0; i < userIDsFromReviews.size(); i++)
        {
            for (int j = 0; j < userIDsRatingsFromOtherMovies[userIDsFromReviews[i].first].size(); j++)
            {
                moviesList.push_back(userIDsRatingsFromOtherMovies[userIDsFromReviews[i].first][j]);
            }
        }
    }*/

    //Converts the map to movieList that stores <movieName, normal rating>
    for (auto it = userIDsRatingsFromOtherMovies.begin(); it != userIDsRatingsFromOtherMovies.end(); it++)
    {
        for (int i = 0; i < userIDsRatingsFromOtherMovies[it->first].size(); i++)
        {
            moviesList.push_back(userIDsRatingsFromOtherMovies[it->first][i]);
        }
    }

    for (int i = 0; i < moviesList.size(); i++)
    {
        finalList.emplace_back(make_pair(moviesList[i].first, getAverageWithMovieList(moviesList[i].first,moviesList)));
    }

    return finalList;
}


//-------------MAP IMPLEMENTATION METHODS------------------//

//OPTION 1 - Suggestions based on Movie
void suggestionsBasedOnMovie(string movie, map<string, Movie>& movieNames)
{
    //Variables
    bool movieFound = false;
    Movie movieObject;
    vector<pair<string,float>> moviesList;

    //1.Calculate 10 highest rated movies that share at least 1 genre
    for (auto it = movieNames.begin(); it != movieNames.end(); it++)
    {
        if (movie == it->first)
        {
            movieFound = true;
            movieObject = it->second;
            break;
        }
    }

    if (movieFound)
    {
        vector<string> genres;

        //Get the specific object's genre list
        for (int i = 0; i < movieObject.getGenres().size(); i++)
        {
            genres.push_back(movieObject.getGenres()[i]);
        }

        //Call helper function to find movies with similar genres
        moviesList = similarGenre(movie, genres, movieNames);
    }
    else //If movie name is not found
    {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    //Print out top 10 movies
    for (int i = 0; i < 10; i++)
    {
        cout << "MovieID: " << movieNames[moviesList[i].first].getMovieID() << " " << moviesList[i].first << " " << "Rating: " << moviesList[i].second << endl;
    }
}

//OPTION 2 - Suggestions Based on Genre
void suggestionsBasedOnGenre(string genre, map<string, Movie>& movieNames)
{
    //Variables
    bool genreFound = false;
    Movie movieObject;
    vector<pair<string,float>> moviesList; //pair<movieName, AverageRating>

    for (auto it = movieNames.begin(); it != movieNames.end(); it++)
    {
        for (int i = 0; i < it->second.getGenres().size(); i++)
        {
            if (genre == it->second.getGenres()[i] && it->second.getReviews().size() > 100)
            {
                genreFound = true;
                moviesList.emplace_back(make_pair(it->first, calculateAverageRatings(it->second.getReviews())));
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
        cout << "MovieID: " << movieNames[moviesList[i].first].getMovieID() << " " << moviesList[i].first << " " << "Rating: " << moviesList[i].second << endl;
    }
}

//OPTION 3
void suggestionsBasedOnReviews(string movie,  map<string, Movie>& movieNames, map<int, vector<pair<int, float>>>& mapReviews, map<int, Movie>& movieIDs)
{
    //Variables
    bool movieFound = false;
    Movie movieObject;
    vector<pair<string,float>> moviesList; // pair<movieID,reviews

    for (auto it = movieNames.begin(); it != movieNames.end(); it++)
    {
        if (it->first == movie)
        {
            movieFound = true;
            break;
        }
    }

    if (movieFound)
    {
        //Call helper function
        moviesList = getReviewsFromMovie(movie, movieNames, mapReviews, movieIDs);
    }
    else //If movie name is not found
    {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    //Sort final list
    sort(moviesList.begin(),moviesList.end(), [] (const auto &movie1, const auto &movie2) {return movie1.second > movie2.second;});

    //Print out top 10 movies
    for (int i = 0; i < 10; i++)
    {
        cout << "MovieID: " << movieNames[moviesList[i].first].getMovieID() << " " << moviesList[i].first << " " << "Rating: " << moviesList[i].second << endl;
    }
}


//OPTION 4
void avoidBasedOnMovie(string movie, map<string, Movie>& movieNames)
{
    //Variables
    bool movieFound = false;
    Movie movieObject;
    vector<pair<string,float>> moviesList;

    //1.Calculate 10 highest rated movies that share at least 1 genre
    for (auto it = movieNames.begin(); it != movieNames.end(); it++)
    {
        if (movie == it->first)
        {
            movieFound = true;
            movieObject = it->second;
            break;
        }
    }

    if (movieFound)
    {
        vector<string> genres;

        //Get the specific object's genre list
        for (int i = 0; i < movieObject.getGenres().size(); i++)
        {
            genres.push_back(movieObject.getGenres()[i]);
        }

        //Call helper function to find movies with similar genres
        moviesList = similarGenre(movie, genres, movieNames);
    }
    else //If movie name is not found
    {
        cout << "Invalid Movie! Please enter a valid movie title" << endl;
        return;
    }

    sort(moviesList.begin(),moviesList.end(), [] (const auto &movie1, const auto &movie2) {return movie1.second < movie2.second;});

    //Print out top 10 movies
    for (int i = 0; i < 10; i++)
    {
        cout << "MovieID: " << movieNames[moviesList[i].first].getMovieID() << " " << moviesList[i].first << " " << "Rating: " << moviesList[i].second << endl;
    }
}

//OPTION 5
void avoidBasedOnGenre(string genre, map<string, Movie>& movieNames)
{
    //Variables
    bool genreFound = false;
    Movie movieObject;
    vector<pair<string,float>> moviesList;

    for (auto it = movieNames.begin(); it != movieNames.end(); it++)
    {
        for (int i = 0; i < it->second.getGenres().size(); i++)
        {
            if (genre == it->second.getGenres()[i] && it->second.getReviews().size() > 100)
            {
                genreFound = true;
                moviesList.emplace_back(make_pair(it->first, calculateAverageRatings(it->second.getReviews())));
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
        cout << "MovieID: " << movieNames[moviesList[i].first].getMovieID() << " " << moviesList[i].first << " " << "Rating: " << moviesList[i].second << endl;
    }
}

//OPTION 7

//OPTION 8


int main()
{
    //---------------------------MAP IMPLEMENTATION----------------------------//
    //Variables
    map<int, Movie> movieIDs; //map<movieID, MovieObject>
    map<int, vector<pair<int, float>>> mapReviews;
    map<string, Movie> movieNames;
    mapReviews = readInReviews("ratings 2.csv", mapReviews);

    //READ IN MOVIES TO AN OBJECT THAT WILL BE PLACED IN A MAP
    readInMovies("movies.csv", movieIDs,mapReviews, movieNames);

    //suggestionsBasedOnMovie("Toy Story (1995)", movieNames);
    //suggestionsBasedOnGenre("Comedy",movieNames);
    suggestionsBasedOnReviews("Toy Story (1995)", movieNames, mapReviews, movieIDs);
    //avoidBasedOnMovie("Toy Story (1995)",movieNames);
    //avoidBasedOnGenre("Comedy",movieNames);
}
