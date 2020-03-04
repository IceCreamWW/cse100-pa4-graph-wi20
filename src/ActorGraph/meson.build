inc = include_directories('.')

actorgraph = library('actorgraph', sources: ['ActorGraph.cpp', 'Actor.hpp', 'Movie.hpp'], include_directories: inc)

actorgraph_dep = declare_dependency(include_directories: inc, link_with: actorgraph)