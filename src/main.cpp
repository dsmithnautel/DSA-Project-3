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
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Will include glad if you add it later
#include <vector>
#include <string>


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

std::string formatGenres(const std::vector<int>& genreFlags) {
    std::vector<std::string> genres;

    for (int i = 0; i < genreFlags.size(); ++i) {
        if (genreFlags[i] == 1 && i < genreList.size()) {
            genres.push_back(genreList[i]);
        }
    }

    if (genres.empty()) return "None";

    std::ostringstream oss;
    for (size_t i = 0; i < genres.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << genres[i];
    }

    return oss.str();
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
        std::string name = input;
        if (input == "done") break;

        std::cout << "\nAvailable genres:\n";
        for (const auto& genre : genreList) {
            std::cout << "- " << genre << "\n";
        }
        std::cout << "\nGenres (comma-separated and case sensitive, e.g., Action,Comedy,Horror): ";
        std::getline(std::cin, input);

        std::vector<int> genreVector(19, 0);
        auto genreMap = createGenreIndexMap();

        std::stringstream ss(input);
        std::string genre;
        bool foundValidGenre = false;

        while (std::getline(ss, genre, ',')) {
            genre.erase(remove_if(genre.begin(), genre.end(), ::isspace), genre.end());
            std::transform(genre.begin(), genre.end(), genre.begin(), ::tolower);
            for (const auto& pair : genreMap) {
                std::string lowered = pair.first;
                std::transform(lowered.begin(), lowered.end(), lowered.begin(), ::tolower);
                if (genre == lowered) {
                    genreVector[pair.second] = 1;
                    foundValidGenre = true;
                    break;
                }
            }
        }

        if (!foundValidGenre) {
            genreVector[0] = 1; // Set to "unknown"
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

        std::cout << "✅ Added \"" << name << "\" with rating " << rating
                  << "/5 and genres: " << formatGenres(genreVector) << "\n";


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
            auto it = movieNames.find(movieId);
            if (it != movieNames.end()) {
                file << "    \"" << movieNode << "\" [label=\"" << it->second << "\"];\n";
            } else {
                file << "    \"" << movieNode << "\" [label=\"Unknown\"];\n";
            }
            // file << "    \"" << movieNode << "\" [label=\"" << movieNames.at(movieId) << "\"];\n";
    }

    file << "}\n";
    file.close();
}


int main() {
    /* Cout testing without visuals
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

    auto genreRecs = gr.recommendGenre(userId, movies, topN);
    std::cout << "\nGraph Genre Recommendations for user " << userId << ":\n";
    for (int m : genreRecs) {
        std::cout << movieNames[m] << " (ID " << m << ")\n";
    }

    auto htGenreRecs = hr.recommendGenre(userId, movieMap, topN);
    std::cout << "\nHashTable Genre Recommendations for user " << userId << ":\n";
    for (int m : htGenreRecs) {
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
*/

    std::vector<std::string> log_originalHashRecs;
    std::vector<std::string> log_updatedHashRecs;
    std::vector<std::string> log_graphRecs;
    std::vector<std::string> log_genreRecs;



    auto ratings = DataLoader::loadRatings("../data/u.data");

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

// Original HashTable recs
    auto originalHashRecs = hr.recommend(userId, topN);
    for (int m : originalHashRecs) {
        log_originalHashRecs.push_back(movieNames[m] + " (ID " + std::to_string(m) + ")");
    }

// Simulated Ratings
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(originalHashRecs.begin(), originalHashRecs.end(), gen);
    int simulatedRatings = std::min(3, (int)originalHashRecs.size());
    for (int i = 0; i < simulatedRatings; ++i) {
        int movieId = originalHashRecs[i];
        int rating = 3 + (gen() % 3);
        hr.addRating(userId, movieId, rating);
    }

// Updated HashTable recs
    auto updatedHashRecs = hr.recommend(userId, topN);
    for (int m : updatedHashRecs) {
        log_updatedHashRecs.push_back(movieNames[m] + " (ID " + std::to_string(m) + ")");
    }

// Graph recs
    auto graphRecs = gr.recommend(userId, topN);
    for (int m : graphRecs) {
        log_graphRecs.push_back(movieNames[m] + " (ID " + std::to_string(m) + ")");
    }

// Genre-based graph recs
    auto genreRecs = gr.recommendGenre(userId, movies, topN);
    for (int m : genreRecs) {
        log_genreRecs.push_back(movieNames[m] + " (ID " + std::to_string(m) + ")");
    }

    exportRecommendationsToDOT(movieNames, originalHashRecs, userId, "hashRecs_before.dot", "HashTable Recommendations - Before");
    exportRecommendationsToDOT(movieNames, updatedHashRecs, userId, "hashRecs_after.dot", "HashTable Recommendations - After");




    exportRecommendationsToDOT(movieNames, originalHashRecs, userId, "hashRecs_before.dot", "HashTable Recommendations - Before");
    exportRecommendationsToDOT(movieNames, updatedHashRecs, userId, "hashRecs_after.dot", "HashTable Recommendations - After");











    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Set up OpenGL version (ImGui uses 3.0+)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui + Project3_DSA", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Set style
    ImGui::StyleColorsDark();

    // Initialize ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Updated HashTable Recommendations
        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
        ImGui::Begin("Updated HashTable Recommendations", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        for (const auto& line : log_updatedHashRecs)
            ImGui::Text("%s", line.c_str());
        ImGui::End();

// Genre-based Graph Recommendations (auto-focus)
        ImGui::SetNextWindowFocus();
        ImGui::SetNextWindowPos(ImVec2(600, 50), ImGuiCond_FirstUseEver);
        ImGui::Begin("Genre-Based Recommendations", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        for (const auto& line : log_genreRecs)
            ImGui::Text("%s", line.c_str());
        ImGui::End();



        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);


    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();






    return 0;
}