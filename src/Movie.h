//
// Created by Wyatt Harris on 4/16/25.
//

#ifndef PROJECT3_DSA_MOVIE_H
#define PROJECT3_DSA_MOVIE_H

#include <string>

class Movie {
public:
    int id;
    std::string title;

    Movie(int id = 0, const std::string& title = "") : id(id), title(title) {}
};


#endif //PROJECT3_DSA_MOVIE_H
