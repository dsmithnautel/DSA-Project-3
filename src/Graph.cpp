#include "Graph.h"

void GraphRecommender::addRating(int userId, int movieId, int rating) {
    graph[userId][movieId] = rating;
}

std::vector<int> GraphRecommender::recommend(int userId) {
    // Placeholder logic: recommend movies user hasn't rated yet
    std::unordered_set<int> seenMovies;
    std::vector<int> recommendations;

    // Collect movies rated by other users
    for (const auto& [otherUser, movies] : graph) {
        if (otherUser == userId) continue;
        for (const auto& [movieId, _] : movies) {
            seenMovies.insert(movieId);
        }
    }

    // Remove movies already rated by this user
    for (const auto& [movieId, _] : graph[userId]) {
        seenMovies.erase(movieId);
    }

    // Add remaining as recommendations
    for (int movieId : seenMovies) {
        recommendations.push_back(movieId);
    }

    return recommendations;
}
