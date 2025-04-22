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

std::vector<int> HashTableRecommender::recommendGenre(int userId,
                                                      const std::unordered_map<int, Movie>& movies,
                                                      int topN) {

    std::vector<float> rating_profile(19, 0.0);
    std::vector<int> count(19, 0);
    auto itrUser = users.find(userId);

    if (itrUser == users.end()) return {};

    // add all ratings into profile
    const auto& userRatings = itrUser->second.getAllRatings();
    for (const auto& [movieId, rating] : userRatings) {
        const std::vector<int> &genres = movies.at(movieId).genres;
        for (int i = 0; i < 19; i++) {
            if (genres[i]) {
                rating_profile[i] += rating;
                count[i]++;
            }
        }
    }

    // normalize profile
    for (int i = 0; i < 19; i++) {
        if (count[i] > 0) {
            rating_profile[i] /= count[i];
        }
    }

    // score unseen movies by profile
    std::vector<std::pair<int, float>> scoreMovies; // movieid , score
    for (auto& [movieId, movie] : movies) {
        if (itrUser->second.getAllRatings().count(movieId) == 1) {
            continue; // skip if already rated
        }

        float score = 0.0f;
        for (int i = 0; i < 19; i++) {
            score += movie.genres[i] * rating_profile[i];
        }
        if (score > 0.0f) {
            scoreMovies.push_back(std::make_pair(movieId, score));
        }
    }

    std::sort(scoreMovies.begin(), scoreMovies.end(),
               [](const auto& a, const auto& b) {return a.second > b.second;});

    std::vector<int> recommendations;
    for (int i = 0; i < topN && i < scoreMovies.size(); ++i) {
        recommendations.push_back(scoreMovies[i].first);
    }
    return recommendations;
}