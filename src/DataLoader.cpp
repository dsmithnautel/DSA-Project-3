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
        std::getline(ss, token, '|');
        id = std::stoi(token);
        std::getline(ss, token, '|');
        std::string title = token;
        movies[id] = Movie(id, title);
    }
    return movies;
}
