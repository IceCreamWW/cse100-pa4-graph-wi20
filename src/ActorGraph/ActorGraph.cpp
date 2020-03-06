/**
 * Actor Graph Implement
 */

#include "ActorGraph.hpp"

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_set>

using namespace std;

ActorGraph::ActorGraph() = default;

/* Build the actor graph from dataset file.
 * Each line of the dataset file must be formatted as:
 * ActorName <tab> MovieName <tab> Year
 * Two actors are connected by an undirected edge if they have worked in a movie
 * before.
 */
bool ActorGraph::buildGraphFromFile(const char* filename) {
    ifstream infile(filename);
    bool readHeader = false;

    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        // skip the header of the file
        if (!readHeader) {
            readHeader = true;
            continue;
        }

        // read each line of the dataset to get the movie actor relation
        istringstream ss(s);
        vector<string> record;
        while (ss) {
            string str;
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        // if format is wrong, skip current line
        if (record.size() != 3) {
            continue;
        }

        // extract the information
        string actor("(" + record[0] + ")");
        string title("[" + record[1] + "#@" + record[2] + "]");

        if (str2neighbours.find(actor) == str2neighbours.end())
            str2neighbours[actor] = vector<string>();
        if (str2neighbours.find(title) == str2neighbours.end())
            str2neighbours[title] = vector<string>();
        str2neighbours[actor].push_back(title);
        str2neighbours[title].push_back(actor);
    }

    // if failed to read the file, clear the graph and return
    if (!infile.eof()) {
        cerr << "Failed to read " << filename << endl;
        return false;
    }
    infile.close();

    return true;
}

void ActorGraph::BFS(const string& fromActor, const string& toActor,
                     string& shortestPath) {
    queue<string> q;

    auto fromActorFormatted = "(" + fromActor + ")";
    auto toActorFormatted = "(" + toActor + ")";

    if (str2neighbours.find(fromActorFormatted) == str2neighbours.end()) return;

    unordered_map<string, string> traceback;

    traceback[fromActorFormatted] = "";
    q.push(fromActorFormatted);

    while (!q.empty()) {
        auto node = q.front();
        q.pop();

        if (node == toActorFormatted){
            auto current = node;
            while (!current.empty()) {
                if (shortestPath.empty())
                    shortestPath = current;
                else if (shortestPath[0] == '[')
                    shortestPath = current + "--" + shortestPath;
                else
                    shortestPath = current + "-->" + shortestPath;

                current = traceback[current];
            }
            return;
        }

        for (auto neighbour : str2neighbours[node]) {
            if (traceback.find(neighbour) != traceback.end())
                continue;

            traceback[neighbour] = node;
            q.push(neighbour);
        }
    }
}

void ActorGraph::predictLink(const string& queryActor,
                             vector<string>& predictionNames,
                             unsigned int numPrediction) {}

ActorGraph::~ActorGraph() {}
