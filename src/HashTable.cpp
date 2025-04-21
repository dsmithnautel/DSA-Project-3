#include "HashTable.h"
#include <algorithm>

// Adds a user rating to the hash table
void HashTableRecommender::addRating(int userId, int movieId, int rating) {
    if (users.find(userId) == users.end()) {
        users[userId] = User(userId);
    }
    users[userId].addRating(movieId, rating);
}

// Recommend movies the user hasn't rated yet, based on others with similar ratings
std::vector<int> HashTableRecommender::recommend(int userId, int topN) {
    if (users.find(userId) == users.end()) return {};

    const User& targetUser = users[userId];
    const auto& targetRatings = targetUser.getAllRatings();

    std::unordered_map<int, double> movieScore;
    std::unordered_map<int, int> movieCount;

    for (const auto& [otherId, otherUser] : users) {
        if (otherId == userId) continue;

        const auto& otherRatings = otherUser.getAllRatings();

        for (const auto& [movieId, rating] : otherRatings) {
            if (targetRatings.find(movieId) == targetRatings.end()) {
                movieScore[movieId] += rating;
                movieCount[movieId]++;
            }
        }
    }

    std::vector<std::pair<int, double>> sortedMovies;
    for (const auto& [movieId, score] : movieScore) {
        double avg = score / movieCount[movieId];
        sortedMovies.emplace_back(movieId, avg);
    }

    std::sort(sortedMovies.begin(), sortedMovies.end(), [](const auto& a, const auto& b) {
        return b.second < a.second;
    });

    std::vector<int> recommendations;
    for (int i = 0; i < topN && i < sortedMovies.size(); ++i) {
        recommendations.push_back(sortedMovies[i].first);
    }

    return recommendations;
}
