
#include "Map.hpp"

/* TODO */
Map::Map() {}

/* Build the map graph from vertex and edge files */
bool Map::buildMapFromFile(const string& vertexFileName,
                           const string& edgeFileName) {
    // add vertices first
    ifstream vertexFile(vertexFileName);
    while (vertexFile) {
        string s;
        if (!getline(vertexFile, s)) break;

        // process data at each line
        istringstream ss(s);
        vector<string> data;
        while (ss) {
            string str;
            if (!getline(ss, str, ' ')) break;
            data.push_back(str);
        }
        if (data.size() != 3) continue;

        // add vertex defined in this line to the graph
        string name(data[0]);
        float x = stoi(data[1]);
        float y = stoi(data[2]);

        addVertex(name, x, y);
    }

    // then add edges
    ifstream edgeFile(edgeFileName);
    while (edgeFile) {
        string s;
        if (!getline(edgeFile, s)) break;

        // process data at each line
        istringstream ss(s);
        vector<string> data;
        while (ss) {
            string str;
            if (!getline(ss, str, ' ')) break;
            data.push_back(str);
        }
        if (data.size() != 2) continue;

        // add edge defined in this line to the graph
        string name1(data[0]);
        string name2(data[1]);

        addEdge(name1, name2);
    }

    return true;
}

/*
 * Add a vertex with name and x, y coordinates to the map graph. Returns
 * false if the name already existed in the map graph, and true otherwise
 */
bool Map::addVertex(const string& name, float x, float y) {
    if (vertexId.count(name) > 0) return false;
    vertexId[name] = vertices.size();
    vertices.push_back(new Vertex(name, x, y));
    return true;
}

/*
 * Add an undirected edge between vertices with names "name1" and "name2".
 * Returns false if either name is not in the map graph.
 */
bool Map::addEdge(const string& name1, const string& name2) {
    if (vertexId.count(name1) == 0 || vertexId.count(name2) == 0) {
        return false;
    }
    unsigned int id1 = vertexId[name1];
    unsigned int id2 = vertexId[name2];
    Vertex* v1 = vertices[id1];
    Vertex* v2 = vertices[id2];
    float weight = sqrt(pow(v1->x - v2->x, 2) + pow(v1->y - v2->y, 2));
    v1->outEdges.push_back(new Edge(v1, v2, weight));
    v2->outEdges.push_back(new Edge(v2, v1, weight));

    undirectedEdges.push_back(new Edge(v1, v2, weight));
    return true;
}

void Map::Dijkstra(const string& from, const string& to,
                   vector<Vertex*>& shortestPath) {
    vector<bool> added(vertices.size(), false);
    vector<int> traceback(vertices.size(), -1);
    vector<float> weight(vertices.size(), INFINITY);

    int vertexIdTo = vertexId[to];
    int curId = vertexIdTo;
    weight[vertexIdTo] = 0;

    for (int i = 0; i < vertices.size(); ++i) {
        float minWeight = INFINITY;
        int minWeightId = -1;

        // add current vertex to set
        added[curId] = true;
        for (auto edge : vertices[curId]->outEdges) {
            // for all vertices curId has edge to, update their weight
            int targetId =
                vertexId[edge->getTarget(vertices[vertexIdTo])->name];

            if (edge->weight + weight[curId] < weight[targetId]) {
                weight[targetId] = edge->weight + weight[curId];
                traceback[targetId] = curId;
            }
        }
        // choose the smallest
        for(int i = 0; i < vertices.size(); ++i) {
            if (added[i]) continue;
            if (weight[i] < minWeight) {
                minWeight = weight[i];
                minWeightId = i;
            }
        }
        curId = minWeightId;
        if (curId == vertexId[from]) break;
    }
    int tracebackId = vertexId[from];
    while (tracebackId != -1) {
        shortestPath.push_back(vertices[tracebackId]);
        tracebackId = traceback[tracebackId];
    }
}

void Map::findMST(vector<Edge*>& MST) {
    priority_queue<Edge*, vector<Edge*>, EdgePtrComp> pq;
    for (auto edge : undirectedEdges) pq.push(edge);
    int added = 0;
    vector<int> side(vertices.size());
    for (int i = 0; i < side.size(); ++i) side[i] = i;
    while (added < vertices.size() - 1) {
        Edge* edge = pq.top();
        pq.pop();
        int sourceId = vertexId[edge->source->name];
        int targetId = vertexId[edge->target->name];
        if (side[sourceId] != side[targetId]) {
            int mergedSide = min(side[sourceId], side[targetId]);
            side[sourceId] = mergedSide;
            side[targetId] = mergedSide;
            ++added;
            MST.push_back(edge);
        }
    }
}

void Map::crucialRoads(vector<Edge*>& roads) {
    int index = 0;
    vector<int> num(vertices.size(), 0);
    vector<int> low(vertices.size(), 0);
    dfs(index, num, low, roads, 0, 0);
}

void Map::dfs(int& index, vector<int>& num, vector<int>& low,
              vector<Edge*>& roads, int curId, int fatherId) {
    ++index;
    num[curId] = index;
    low[curId] = index;

    for (auto edge : vertices[curId]->outEdges) {
        int nextId = vertexId[edge->getTarget(vertices[curId])->name];
        if (num[nextId] == 0) {
            dfs(index, num, low, roads, nextId, curId);
            low[curId] = min(low[curId], low[nextId]);

            if (low[nextId] > num[curId]) roads.push_back(edge);
        } else if (nextId != fatherId)
            low[curId] = min(low[curId], num[nextId]);
    }
}

/* Destructor of Map graph */
Map::~Map() {
    for (Vertex* v : vertices) {
        for (Edge* e : v->outEdges) {
            delete e;
        }
        delete v;
    }
    for (Edge* e : undirectedEdges) {
        delete e;
    }
}
