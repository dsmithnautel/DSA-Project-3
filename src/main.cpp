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

const std::vector<std::string> genreList = {
        "unknown", "Action", "Adventure", "Animation", "Children's", "Comedy", "Crime",
        "Documentary", "Drama", "Fantasy", "Film-Noir", "Horror", "Musical",
        "Mystery", "Romance", "Sci-Fi", "Thriller", "War", "Western"
};

std::unordered_map<std::string, int> createGenreIndexMap() {
    std::unordered_map<std::string, int> map;
    for (int i = 0; i < genreList.size(); ++i) {
        map[genreList[i]] = i;
    }
    return map;
}

int getNextAvailableMovieId(const std::unordered_map<int, std::string>& movieNames) {
    int maxId = 0;
    for (const auto& pair : movieNames) {
        if (pair.first > maxId) {
            maxId = pair.first;
        }
    }
    return maxId + 1;
}

void addUserMoviesWithRatings(std::unordered_map<int, std::string>& movieNames,
                              std::unordered_map<int, Movie>& movieMap,
                              HashTableRecommender& hr,
                              GraphRecommender& gr,
                              int userId) {
    int nextId = getNextAvailableMovieId(movieNames);
    std::string input;

    std::cout << "\n--- Add Your Own Movies ---\n";
    std::cout << "Enter your own movies, their genres, and your rating.\n";
    std::cout << "Type 'done' at any time to stop.\n";

    while (true) {
        std::cout << "\nMovie name (or 'done'): ";
        std::getline(std::cin, input);
        if (input == "done") break;
        std::string name = input;

        std::cout << "Genres (comma-separated, e.g., Action,Comedy): ";
        std::getline(std::cin, input);
        if (input == "done") break;

        std::vector<int> genreVector(19, 0);
        auto genreMap = createGenreIndexMap();

        std::stringstream ss(input);
        std::string genre;
        while (std::getline(ss, genre, ',')) {
            // Trim whitespace
            genre.erase(remove_if(genre.begin(), genre.end(), ::isspace), genre.end());
            std::transform(genre.begin(), genre.end(), genre.begin(), ::tolower);
            for (auto& pair : genreMap) {
                std::string lowered = pair.first;
                std::transform(lowered.begin(), lowered.end(), lowered.begin(), ::tolower);
                if (genre == lowered) {
                    genreVector[pair.second] = 1;
                }
            }
        }

        std::cout << "Your rating for \"" << name << "\" (1-5): ";
        int rating;
        std::cin >> rating;
        std::cin.ignore(); // Clear newline
        if (rating < 1 || rating > 5) {
            std::cout << "Invalid rating. Must be between 1 and 5. Skipping movie.\n";
            continue;
        }

        // Create and store movie
        Movie newMovie;
        newMovie.id = nextId;
        newMovie.title = name;
        newMovie.genres = genreVector;

        movieNames[nextId] = name;
        movieMap[nextId] = newMovie;

        // Add rating to recommenders
        hr.addRating(userId, nextId, rating);
        gr.addRating(userId, nextId, rating);

        std::cout << "✅ Added \"" << name << "\" with rating " << rating << "/5 and genres [";
        for (const auto& g : genreVector) std::cout << g << " ";
        std::cout << "]\n";

        ++nextId;
    }
}

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

    std::unordered_map<int, Movie> movieMap;
    for (const auto& pair : movies) {
        movieMap[pair.first] = pair.second;
    }

    GraphRecommender gr;
    HashTableRecommender hr;

    for (const auto& r : ratings) {
        gr.addRating(r.userId, r.movieId, r.rating);
        hr.addRating(r.userId, r.movieId, r.rating);
    }

    int userId = 1;
    int topN = 10;

    addUserMoviesWithRatings(movieNames, movieMap, hr, gr, userId);

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