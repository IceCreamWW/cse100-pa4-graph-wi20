//
// Created by icecreamww on 2020/3/3.
//

#ifndef PA4_GRAPH_ACTOR_HPP
#define PA4_GRAPH_ACTOR_HPP

#include <iostream>
#include <vector>

class Movie;

using namespace std;

/**
 * This class defines an actor by his/her name
 * and provides access to actor's edges
 */
class Actor {
  public:
    string name;
    vector<Movie*> movies;

    /* constructor that initializes an Actor */
    Actor(string name) : name(name) {}
};

#endif  // PA4_GRAPH_ACTOR_HPP
