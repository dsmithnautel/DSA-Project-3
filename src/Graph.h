//
// Created by Wyatt Harris on 4/16/25.
//

#ifndef PROJECT3_DSA_GRAPH_H
#define PROJECT3_DSA_GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

class GraphRecommender {
private:
    std::unordered_map<int, std::unordered_map<int, int>> graph; // userId -> {movieId -> rating}
public:
    void addRating(int userId, int movieId, int rating);
    std::vector<int> recommend(int userId);
};


#endif //PROJECT3_DSA_GRAPH_H
