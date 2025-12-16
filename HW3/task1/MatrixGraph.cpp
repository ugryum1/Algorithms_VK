#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(int size) : adjacencyMatrix(size) {
    for (auto &row : adjacencyMatrix) {
        row.resize(size, false);
    }
}

MatrixGraph::MatrixGraph(const IGraph &graph) {
    int size = graph.VerticesCount();
    adjacencyMatrix.resize(size);

    for (auto &row : adjacencyMatrix) {
        row.resize(size, false);
    }

    for (int i = 0; i < size; ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (int to : nextVertices) {
            adjacencyMatrix[i][to] = true;
        }
    }
}

MatrixGraph::~MatrixGraph() {}

void MatrixGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < adjacencyMatrix.size());
    assert(0 <= to && to < adjacencyMatrix.size());
    adjacencyMatrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return static_cast<int>(adjacencyMatrix.size());
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacencyMatrix.size());

    std::vector<int> nextVertices;
    for (int to = 0; to < adjacencyMatrix.size(); ++to) {
        if (adjacencyMatrix[vertex][to]) {
            nextVertices.push_back(to);
        }
    }

    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacencyMatrix.size());

    std::vector<int> prevVertices;
    for (int from = 0; from < adjacencyMatrix.size(); ++from) {
        if (adjacencyMatrix[from][vertex]) {
            prevVertices.push_back(from);
        }
    }

    return prevVertices;
}
