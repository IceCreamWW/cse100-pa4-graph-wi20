//
// Created by icecreamww on 2020/3/5.
//

#include "Edge.hpp"

Vertex* Edge::getTarget(Vertex *self) {
    return source == self ? target : source;
}
