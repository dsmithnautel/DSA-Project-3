//
// Created by Wyatt Harris on 4/16/25.
//

#ifndef PROJECT3_DSA_GRAPH_H
#define PROJECT3_DSA_GRAPH_H

#include <map>
#include <unordered_set>
#include <unordered_map>
#include "Movie.h"
#include <vector>

class GraphRecommender {
private:
    std::map<int, std::map<int, int>> graph; // userId -> {movieId -> rating}
public:
    void addRating(int userId, int movieId, int rating);
    std::vector<int> recommendGenre(int userId, const std::unordered_map<int, Movie>& movies, int topN = 5);
    std::vector<int> recommend(int userId, int topN = 5);
};


#endif //PROJECT3_DSA_GRAPH_H
