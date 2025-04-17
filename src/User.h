#ifndef PROJECT3_DSA_USER_H
#define PROJECT3_DSA_USER_H

#include <unordered_map>

class User {
private:
    int id;
    std::unordered_map<int, int> ratings; // movieId -> rating

public:
    User(int userId = 0);

    int getId() const;
    void addRating(int movieId, int rating);
    bool hasRated(int movieId) const;
    int getRating(int movieId) const;
    const std::unordered_map<int, int>& getAllRatings() const;
};

#endif //PROJECT3_DSA_USER_H
