#include <iostream>
#include "DataLoader.h"
#include "Graph.h"
#include "HashTable.h"
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <set>

std::unordered_map<int, std::string> loadMovieNames(const std::string& filename) {
    std::unordered_map<int, std::string> movieNames;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, name;
        std::getline(ss, idStr, '|');    // movie ID
        std::getline(ss, name, '|');     // movie name

        int id = std::stoi(idStr);
        movieNames[id] = name;
    }

    return movieNames;
}

void exportRecommendationsToDOT(const std::unordered_map<int, std::string>& movieNames,
                                const std::vector<int>& recommendedMovies,
                                int userId,
                                const std::string& filename,
                                const std::string& title) {
    std::ofstream file(filename);
    file << "digraph G {\n";
    file << "    label=\"" << title << "\";\n";
    file << "    labelloc=top;\n";
    file << "    fontsize=20;\n";
    file << "    node [style=filled, fillcolor=lightblue];\n";

    std::string userNode = "User" + std::to_string(userId);
    file << "    \"" << userNode << "\" [shape=box, fillcolor=lightgreen];\n";

    for (int movieId : recommendedMovies) {
        std::string movieNode = "Movie" + std::to_string(movieId);
        file << "    \"" << userNode << "\" -> \"" << movieNode
             << "\" [label=\"Recommended\", color=red];\n";
        file << "    \"" << movieNode << "\" [label=\"" << movieNames.at(movieId) << "\"];\n";
    }

    file << "}\n";
    file.close();
}


int main() {
    auto ratings = DataLoader::loadRatings("../data/u.data");
    std::cout << "Loaded ratings: " << ratings.size() << "\n";

    auto movieNames = loadMovieNames("../data/u.item");

    auto movies = DataLoader::loadMovies("../data/u.item");

    GraphRecommender gr;
    HashTableRecommender hr;

    for (const auto& r : ratings) {
        gr.addRating(r.userId, r.movieId, r.rating);
        hr.addRating(r.userId, r.movieId, r.rating);
    }

    int userId = 1;
    int topN = 10;

    auto originalHashRecs = hr.recommend(userId, topN);

    std::cout << "\nOriginal HashTable Recommendations for user " << userId << ":\n";
    for (int m : originalHashRecs) {
        std::cout << movieNames[m] << " (ID " << m << ")\n";
    }



    // Simulate user watching 3 movies from original recommendations
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(originalHashRecs.begin(), originalHashRecs.end(), gen);

    int simulatedRatings = std::min(3, static_cast<int>(originalHashRecs.size()));
    for (int i = 0; i < simulatedRatings; ++i) {
        int movieId = originalHashRecs[i];
        int rating = 3 + (gen() % 3); // Give a rating between 3 and 5
        std::cout << "\nSimulating rating movie ID " << movieId << ": \"" << movieNames[movieId] << "\" with " << rating << "/5\n";
        hr.addRating(userId, movieId, rating);
    }

    auto updatedHashRecs = hr.recommend(userId, topN);

    std::cout << "\nUpdated HashTable Recommendations for user " << userId << ":\n";
    for (int m : updatedHashRecs) {
        std::cout << movieNames[m] << " (ID " << m << ")\n";
    }

    auto graphRecs = gr.recommend(userId, topN);

    std::cout << "\nOriginal Graph Recommendations for user " << userId << ":\n";
    for (int m : graphRecs) {
        std::cout << movieNames[m] << " (ID " << m << ")\n";
    }

    auto genreRecs = gr.recommendGenre(userId, movies, topN);
    std::cout << "\nGraph Genre Recommendations for user " << userId << ":\n";
    for (int m : genreRecs) {
        std::cout << movieNames[m] << " (ID " << m << ")\n";
    }

    exportRecommendationsToDOT(movieNames, originalHashRecs, userId, "hashRecs_before.dot", "HashTable Recommendations - Before");
    exportRecommendationsToDOT(movieNames, updatedHashRecs, userId, "hashRecs_after.dot", "HashTable Recommendations - After");

    return 0;
}