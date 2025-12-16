#ifndef ARCGRAPH_H
#define ARCGRAPH_H

#include "IGraph.h"
#include <utility>

struct ArcGraph: public IGraph {
public:
    ArcGraph(int size);
    ArcGraph(const IGraph &graph);
    ~ArcGraph();

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::pair<int, int>> edges;
    int verticesCount;
};

#endif // ARCGRAPH_H
