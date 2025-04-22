#ifndef PROJECT3_DSA_HASHTABLE_H
#define PROJECT3_DSA_HASHTABLE_H

#include <unordered_map>
#include <vector>
#include "User.h"
#include "Movie.h"
#include <algorithm>

class HashTableRecommender {
private:
    std::unordered_map<int, User> users;

public:
    void addRating(int userId, int movieId, int rating);
    std::vector<int> recommend(int userId, int topN = 5);
    std::vector<int> recommendGenre(int userId, const std::unordered_map<int, Movie>& movies, int topN = 5);
};

#endif //PROJECT3_DSA_HASHTABLE_H
