#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cassert>

struct RickGraph {
    int a, b, m;

    RickGraph(int a, int b, int m) : a(a), b(b), m(m) {}

    int VerticesCount() const {
        return m;
    }

    std::vector<int> GetNextVertices(int v) const {
        assert(0 <= v && v < m);

        return {
            (v + 1) % m,
            static_cast<int>((static_cast<long long int>(1) * v * v + 1) % m)
        };
    }
};

long long int Dijkstra(const RickGraph &graph, int start, int end) {
    const long long int INF = LLONG_MAX;
    int m = graph.VerticesCount();

    std::vector<long long int> dist(m, INF);
    dist[start] = 0;

    std::priority_queue<std::pair<long long int, int>,
                        std::vector<std::pair<long long int, int>>,
                        std::greater<std::pair<long long int, int>>
    > pq;

    pq.push({0, start});

    while (!pq.empty()) {
        auto [curDist, v] = pq.top();
        pq.pop();

        if (curDist > dist[v]) {
            continue;
        }

        if (v == end) {
            return curDist;
        }

        int to1 = (v + 1) % m;
        if (dist[to1] > curDist + graph.a) {
            dist[to1] = curDist + graph.a;
            pq.push({dist[to1], to1});
        }

        int to2 = static_cast<int>((static_cast<long long int>(1) * v * v + 1) % m);
        if (dist[to2] > curDist + graph.b) {
            dist[to2] = curDist + graph.b;
            pq.push({dist[to2], to2});
        }
    }

    return dist[end];
}

int main() {
    int a, b, m, x, y;
    std::cin >> a >> b >> m >> x >> y;

    RickGraph graph(a, b, m);

    std::cout << Dijkstra(graph, x, y) << std::endl;
    return 0;
}
