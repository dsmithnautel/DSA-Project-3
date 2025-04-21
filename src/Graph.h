//
// Created by Wyatt Harris on 4/16/25.
//

#ifndef PROJECT3_DSA_GRAPH_H
#define PROJECT3_DSA_GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include "Movie.h"
#include <vector>

class GraphRecommender {
private:
    std::unordered_map<int, std::unordered_map<int, int>> graph; // userId -> {movieId -> rating}
public:
    void addRating(int userId, int movieId, int rating);
    std::vector<int> recommendGenre(int userId, const std::unordered_map<int, Movie>& movies, int topN = 10);
    std::vector<int> recommend(int userId, int topN = 10);
};


#endif //PROJECT3_DSA_GRAPH_H
