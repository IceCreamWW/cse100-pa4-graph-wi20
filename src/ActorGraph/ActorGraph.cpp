/**
 * TODO: add file header
 */

#include "ActorGraph.hpp"

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <tuple>
#include <unordered_set>

using namespace std;

/* TODO */
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
        string actor(record[0]);
        string title(record[1]);
        title += "#@";
        title += record[2];

        if (actor2node.find(actor) == actor2node.end())
            actor2node[actor] = new Actor(actor);

        if (movie2node.find(title) == movie2node.end())
            movie2node[title] = new Movie(title);

        actor2node[actor]->movies.push_back(movie2node[title]);
        movie2node[title]->actors.push_back(actor2node[actor]);
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
    queue<Actor*> q;

    if (actor2node.find(fromActor) == actor2node.end()) return;

    unordered_map<string, string> traceback;

    traceback["(" + fromActor + ")"] = "";
    q.push(actor2node[fromActor]);
    while (!q.empty()) {
        auto actorNode = q.front();
        q.pop();

        if (actorNode->name == toActor) {
            auto current = "(" + toActor + ")";
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

        for (auto movie : actorNode->movies) {
            if (traceback.find("[" + movie->title + "]") != traceback.end())
                continue;

            traceback["[" + movie->title + "]"] = "(" + actorNode->name + ")";

            for (auto newActorNode : movie->actors) {
                if (traceback.find("(" + newActorNode->name + ")") !=
                    traceback.end())
                    continue;

                traceback["(" + newActorNode->name + ")"] =
                    "[" + movie->title + "]";

                q.push(newActorNode);
            }
        }
    }
}

/* TODO */
void ActorGraph::predictLink(const string& queryActor,
                             vector<string>& predictionNames,
                             unsigned int numPrediction) {}

ActorGraph::~ActorGraph() {
    for (auto item : actor2node) delete item.second;
    for (auto item : movie2node) delete item.second;
}
