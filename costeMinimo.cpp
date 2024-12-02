#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

const int INF = INT_MAX;

struct Edge {
    int to, capacity, cost, flow;
    Edge(int t, int c, int co) : to(t), capacity(c), cost(co), flow(0) {}
};

class MinCostMaxFlow {
public:
    MinCostMaxFlow(int n) : n(n), adj(n) {}

    void addEdge(int u, int v, int cap, int cost) {
        adj[u].emplace_back(v, cap, cost);
        adj[v].emplace_back(u, 0, -cost);  // reverse edge with negative cost
    }

    pair<int, int> minCostMaxFlow(int source, int sink) {
        int maxFlow = 0, minCost = 0;
        while (true) {
            vector<int> dist(n, INF), parent(n, -1), edgeId(n, -1);
            dist[source] = 0;
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
            pq.push({0, source});

            while (!pq.empty()) {
                int u = pq.top().second;
                int d = pq.top().first;
                pq.pop();

                if (d > dist[u]) continue;

                for (int i = 0; i < adj[u].size(); i++) {
                    Edge& e = adj[u][i];
                    if (e.capacity - e.flow > 0 && dist[u] + e.cost < dist[e.to]) {
                        dist[e.to] = dist[u] + e.cost;
                        parent[e.to] = u;
                        edgeId[e.to] = i;
                        pq.push({dist[e.to], e.to});
                    }
                }
            }

            if (dist[sink] == INF) break;  // No augmenting path

            // Find the maximum flow through the path
            int pathFlow = INF;
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                int edgeIndex = edgeId[v];
                pathFlow = min(pathFlow, adj[u][edgeIndex].capacity - adj[u][edgeIndex].flow);
            }

            // Update the flow and cost
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                int edgeIndex = edgeId[v];
                adj[u][edgeIndex].flow += pathFlow;
                adj[v][edgeIndex ^ 1].flow -= pathFlow;  // reverse edge
            }

            maxFlow += pathFlow;
            minCost += pathFlow * dist[sink];
        }
        return {maxFlow, minCost};
    }

private:
    int n;
    vector<vector<Edge>> adj;
};

int main() {
    int n = 4;  // Número de nodos
    MinCostMaxFlow mcmf(n);

    // Agregar aristas (u, v, capacidad, costo)
    mcmf.addEdge(0, 1, 3, 1);
    mcmf.addEdge(0, 2, 2, 2);
    mcmf.addEdge(1, 2, 1, 1);
    mcmf.addEdge(1, 3, 2, 3);
    mcmf.addEdge(2, 3, 3, 1);

    pair<int, int> result = mcmf.minCostMaxFlow(0, 3);
    cout << "Flujo máximo: " << result.first << endl;
    cout << "Costo mínimo: " << result.second << endl;

    return 0;
}
