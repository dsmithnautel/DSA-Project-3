#include "Graph.h"

void GraphRecommender::addRating(int userId, int movieId, int rating) {
    graph[userId][movieId] = rating;
}

std::vector<int> GraphRecommender::recommend(int userId) {
    std::unordered_set<int> seenMovies;
    std::vector<int> recommendations;

    for (const auto& [otherUser, movies] : graph) {
        if (otherUser == userId) continue;
        for (const auto& [movieId, rating] : movies) {
            // only recommend greater than 4 stars
            if (rating > 4) {
                seenMovies.insert(movieId);
            }
        }
    }

    for (const auto& [movieId, _] : graph[userId]) {
        seenMovies.erase(movieId);
    }

    for (int movieId : seenMovies) {
        recommendations.push_back(movieId);
    }

    return recommendations;
}
