/**
 * TODO: add file header
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

/**
 * This class builds a graph from a formatted file
 * and uses BFS to find the shortest path between to actors
 */
class ActorGraph {
  protected:
    unordered_map<string, vector<string>> str2neighbours;

  public:
    /* constructor that initializes an ActorGraph */
    ActorGraph();

    /* build a graph from formatted file
     * Parameter: filename */
    bool buildGraphFromFile(const char* filename);

    /* find the shortest between two actors */
    void BFS(const string& fromActor, const string& toActor,
             string& shortestPath);

    /* predict possible links to given query actor */
    void predictLink(const string& queryActor, vector<string>& predictionNames,
                     unsigned int numPrediction);

    /* invoke deconstruction methods of all actor nodes */
    ~ActorGraph();
};

#endif  // ACTORGRAPH_HPP
