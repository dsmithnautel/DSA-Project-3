//
// Created by Wyatt Harris on 4/16/25.
//

#ifndef PROJECT3_DSA_MOVIE_H
#define PROJECT3_DSA_MOVIE_H

#include <string>
#include <vector>

class Movie {
public:
    int id;
    std::string title;
    std::vector<int> genres; // 19 long vector, either 0 or 1

    Movie(int id = 0,
          const std::string& title = "",
          const std::vector<int>& genres = std::vector<int>(19,0))
    : id(id), title(title), genres(genres) {}
};


#endif //PROJECT3_DSA_MOVIE_H
