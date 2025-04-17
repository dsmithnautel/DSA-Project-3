#include "User.h"

User::User(int userId) : id(userId) {}

int User::getId() const {
    return id;
}

void User::addRating(int movieId, int rating) {
    ratings[movieId] = rating;
}

bool User::hasRated(int movieId) const {
    return ratings.find(movieId) != ratings.end();
}

int User::getRating(int movieId) const {
    auto it = ratings.find(movieId);
    return (it != ratings.end()) ? it->second : -1; // -1 = not rated
}

const std::unordered_map<int, int>& User::getAllRatings() const {
    return ratings;
}
