//
// Created by icecreamww on 2020/3/3.
//

#ifndef PA4_GRAPH_MOVIE_HPP
#define PA4_GRAPH_MOVIE_HPP

#include <iostream>
#include <vector>
using namespace std;

class Actor;

class Movie {
  public:
    string title;
    vector<Actor*> actors;

    Movie(string title) : title(title) {}
};

#endif  // PA4_GRAPH_MOVIE_HPP
