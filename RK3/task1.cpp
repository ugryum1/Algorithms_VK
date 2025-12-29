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

int componentsCount(ListGraph &graph, int n) {
    std::vector<bool> visited(n, false);
    int components = 0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            ++components;

            std::queue<int> q;
            q.push(i);
            visited[i] = true;

            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (int next : graph.GetNextVertices(v)) {
                    if (!visited[next]) {
                        visited[next] = true;
                        q.push(next);
                    }
                }
            }
        }
    }

    return components;
}

int main() {
    int n, m;
    std::cin >> n;
    std::cin >> m;

    ListGraph graph(n);

    for (int i = 0; i < m; ++i) {
        int from, to;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    std::cout << componentsCount(graph, n) << std::endl;
    return 0;
}
