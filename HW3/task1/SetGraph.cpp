#include "SetGraph.h"

SetGraph::SetGraph(int size) : adjacencySets(size) {}

SetGraph::SetGraph(const IGraph &graph) {
    int size = graph.VerticesCount();
    adjacencySets.resize(size);

    for (int from  = 0; from  < size; ++from ) {
        std::vector<int> nextVertices = graph.GetNextVertices(from);
        for (int to : nextVertices) {
            adjacencySets[from].insert(to);
        }
    }
}

SetGraph::~SetGraph() {}

void SetGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < adjacencySets.size());
    assert(0 <= to && to < adjacencySets.size());
    adjacencySets[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return static_cast<int>(adjacencySets.size());
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacencySets.size());

    std::vector<int> nextVertices;
    nextVertices.reserve(adjacencySets[vertex].size());

    for (int to : adjacencySets[vertex]) {
        nextVertices.push_back(to);
    }

    return nextVertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacencySets.size());

    std::vector<int> prevVertices;

    for (int from = 0; from < adjacencySets.size(); ++from) {
        for (int to: adjacencySets[from]) {
            if (to == vertex) {
                prevVertices.push_back(from);
                break;
            }
        }
    }

    return prevVertices;
}
