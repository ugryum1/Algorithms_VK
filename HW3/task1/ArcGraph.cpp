#include "ArcGraph.h"

ArcGraph::ArcGraph(int size) : verticesCount(size) {}

ArcGraph::ArcGraph(const IGraph &graph) {
    int size = graph.VerticesCount();
    verticesCount = size;

    for (int from  = 0; from  < size; ++from ) {
        std::vector<int> nextVertices = graph.GetNextVertices(from);
        for (int to : nextVertices) {
            edges.emplace_back(from, to);
        }
    }
}

ArcGraph::~ArcGraph() {}

void ArcGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < verticesCount);
    assert(0 <= to && to < verticesCount);
    edges.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    return verticesCount;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < verticesCount);

    std::vector<int> nextVertices;
    for (const auto &edge :edges) {
        if (edge.first == vertex) {
            nextVertices.push_back(edge.second);
        }
    }

    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < verticesCount);

    std::vector<int> prevVertices;
    for (const auto &edge :edges) {
        if (edge.second == vertex) {
            prevVertices.push_back(edge.first);
        }
    }

    return prevVertices;
}
