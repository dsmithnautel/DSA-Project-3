#ifndef PROJECT3_DSA_DATALOADER_H
#define PROJECT3_DSA_DATALOADER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "Movie.h"

struct Rating {
    int userId;
    int movieId;
    int rating;
};

class DataLoader {
public:
    static std::vector<Rating> loadRatings(const std::string& filePath);
    static std::unordered_map<int, Movie> loadMovies(const std::string& filePath);
};



#endif //PROJECT3_DSA_DATALOADER_H