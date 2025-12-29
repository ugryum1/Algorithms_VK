#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <climits>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
};

struct ListGraph: public IGraph {
public:
    ListGraph(int size);
    ListGraph(const IGraph &graph);
    ~ListGraph();

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjacencyLists;
};

ListGraph::ListGraph(int size) : adjacencyLists(size) {}

ListGraph::ListGraph(const IGraph &graph) {
    adjacencyLists.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        adjacencyLists[i] = graph.GetNextVertices(i);
    }
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < adjacencyLists.size());
    assert(0 <= to && to < adjacencyLists.size());
    adjacencyLists[from].push_back(to);
    if (from != to) {
        adjacencyLists[to].push_back(from);
    }
}

int ListGraph::VerticesCount() const {
    return static_cast<int>(adjacencyLists.size());
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < adjacencyLists.size());
    return adjacencyLists[vertex];
}

bool IsHamiltonianPath(const IGraph& graph, const std::vector<int>& path) {
    int n = graph.VerticesCount();

    if (path.size() != n) {
        return false;
    }

    std::vector<bool> used(n, false);
    for (int v : path) {
        if (v < 0 || v >= n || used[v]) {
            return false;
        }
        used[v] = true;
    }

    for (int i = 0; i < path.size() - 1; ++i) {
        int from = path[i];
        int to = path[i + 1];

        bool edgeExists = false;
        for (int next : graph.GetNextVertices(from)) {
            if (next == to) {
                edgeExists = true;
                break;
            }
        }

        if (!edgeExists) {
            return false;
        }
    }

    return true;
}

int main() {
    int n, m, k;
    std::cin >> n;
    std::cin >> m;
    std::cin >> k;

    ListGraph graph(n);

    for (int i = 0; i < m; ++i) {
        int from, to;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    std::vector<int> path(k);
    for (int i = 0; i < k; ++i) {
        std::cin >> path[i];
    }

    std::cout << (IsHamiltonianPath(graph, path) ? 1 : 0) << std::endl;

    return 0;
}
