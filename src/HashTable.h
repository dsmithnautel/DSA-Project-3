#ifndef PROJECT3_DSA_HASHTABLE_H
#define PROJECT3_DSA_HASHTABLE_H

#include <unordered_map>
#include <vector>
#include "User.h"

class HashTableRecommender {
private:
    std::unordered_map<int, User> users;

public:
    void addRating(int userId, int movieId, int rating);
    std::vector<int> recommend(int userId, int topN = 5);
};

#endif //PROJECT3_DSA_HASHTABLE_H
