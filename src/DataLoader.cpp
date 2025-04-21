#include "DataLoader.h"
#include <fstream>
#include <sstream>

std::vector<Rating> DataLoader::loadRatings(const std::string& filePath) {
    std::vector<Rating> ratings;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        int userId, movieId, rating;
        long timestamp;
        ss >> userId >> movieId >> rating >> timestamp;
        ratings.push_back({userId, movieId, rating});
    }
    return ratings;
}

std::unordered_map<int, Movie> DataLoader::loadMovies(const std::string& filePath) {
    std::unordered_map<int, Movie> movies;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        int id;
        std::string title;
        std::vector<int> genres;


        std::getline(ss, token, '|'); // movie id
        id = std::stoi(token);

        std::getline(ss, token, '|'); // movie title
        title = token;

        std::getline(ss, token, '|'); // release (dont care)
        std::getline(ss, token, '|'); // video release (dont care)
        std::getline(ss, token, '|'); // imdb url (dont care)
        std::getline(ss, token, '|'); // unknown (dont care)

        for(int i = 0; i < 19; i++) {           // genres vector
            std::getline(ss, token, '|');
            genres.push_back(std::stoi(token));
        }

        movies[id] = Movie(id, title, genres);
    }
    return movies;
}
