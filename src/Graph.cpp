#include "Graph.h"
#include <vector>
#include <unordered_set>
#include <algorithm>

void GraphRecommender::addRating(int userId, int movieId, int rating) {
    graph[userId][movieId] = rating;
}

std::vector<int> GraphRecommender::recommendGenre(const int userId,
                                             const std::unordered_map<int, Movie>& movies,
                                             int topN) {
    std::vector<float> ratingProfile(19, 0.0);
    std::vector<int> count(19, 0);
    auto itrUser = graph.find(userId);

    if (itrUser == graph.end()) {return {};} // userId -> {movieId -> rating}


    // add genre ratings to rating profile
    for (const auto& [movieId, rating] : itrUser->second) { // for each movie rated by user
        const std::vector<int> genres = movies.at(movieId).genres; // get genres of each movie
        for (int i = 0; i < 19; i++) { // for each genre in movie's genre vector (which is 19 long and either 0 or 1)
            if (genres[i] == 1) { // if genre is present (1)
                ratingProfile[i] += rating; // then add rating to profile
                count[i]++; // and increment count for that genre (to normalize later)
            }
        }
    }

    // normalize rating profile
    for (int i = 0; i < 19; i++) {
        if (count[i] != 0) {
            ratingProfile[i] /= count[i];
        }
    }

    // score unseen movies based on rating profile
    std::vector<std::pair<int, float>> scoredMovies; // {movieId -> score}
    for (auto& [movieId, movie] : movies) { // for every movie
        if (itrUser->second.count(movieId) == 1) {continue;} // skip if user has already rated the movie
        float score = 0.0f;
        for (int i = 0; i < 19; i++) { // for each genre in movie's genre vector
            score += movie.genres[i] * ratingProfile[i]; // score only increases if genre is present
        }
        if (score > 0.0f) { // if score is greater than threshold
            scoredMovies.push_back({movieId, score}); // add to scored movies
        }
    }

    // sort scored movies by score
    std::sort(scoredMovies.begin(), scoredMovies.end(),
              [](const auto &a, const auto &b) { return a.second > b.second; }); // sort by score

    // take topN movieIDs
    std::vector<int> recommendations;
    for (int i = 0; i < topN && i < scoredMovies.size(); i++) {
        recommendations.push_back(scoredMovies[i].first);
    }

    return recommendations;
}

std::vector<int> GraphRecommender::recommend(const int userId, int topN) {
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

    // take topN movieIDs
    std::vector<int> topRecommendations;
    for (int i = 0; i < topN && i < recommendations.size(); i++) {
        topRecommendations.push_back(recommendations[i]);
    }

    return topRecommendations;
}