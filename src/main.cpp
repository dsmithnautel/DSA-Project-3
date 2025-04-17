#include <iostream>
#include "DataLoader.h"
#include "Graph.h"
#include "HashTable.h"
#include <unordered_map>
#include <fstream>
#include <sstream>

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


int main() {
    auto ratings = DataLoader::loadRatings("/Users/weh/CLionProjects/Project3_DSA/data/u.data");
    std::cout << "Loaded ratings: " << ratings.size() << "\n";

    auto movieNames = loadMovieNames("/Users/weh/CLionProjects/Project3_DSA/data/u.item");

    GraphRecommender gr;
    HashTableRecommender hr;

    for (const auto& r : ratings) {
        gr.addRating(r.userId, r.movieId, r.rating);
        hr.addRating(r.userId, r.movieId, r.rating);
    }

    auto graphRecs = gr.recommend(1);
    auto hashRecs = hr.recommend(1);

    int topN = 10;

    if (graphRecs.size() > topN) {
        graphRecs = std::vector<int>(graphRecs.begin(), graphRecs.begin() + topN);
    }

    if (hashRecs.size() > topN) {
        hashRecs = std::vector<int>(hashRecs.begin(), hashRecs.begin() + topN);
    }

    std::cout << "Graph Recommendations for user 1:\n";
    for (int m : graphRecs) {
        std::cout << movieNames[m] << " (ID " << m << ")\n";
    }

    std::cout << "\nHashTable Recommendations for user 1:\n";
    for (int m : hashRecs) {
        std::cout << movieNames[m] << " (ID " << m << ")\n";
    }
}
