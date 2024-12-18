#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

const int INF = INT_MAX;

struct Arista {
    int destino, capacidad, costo, flujo;
    Arista(int d, int c, int co) : destino(d), capacidad(c), costo(co), flujo(0) {}
};

class FlujoMaximoCostoMinimo {
public:
    FlujoMaximoCostoMinimo(int nodos) : nodos(nodos), adj(nodos) {}

    void agregarArista(int desde, int hasta, int capacidad, int costo) {
        adj[desde].emplace_back(hasta, capacidad, costo);  // Agregar arista hacia adelante
        adj[hasta].emplace_back(desde, 0, -costo);         // Agregar arista inversa con costo negativo
    }

    pair<int, int> flujoMaximoCostoMinimo(int fuente, int sumidero) {
        int flujoMaximo = 0, costoMinimo = 0;
        while (true) {
            vector<int> dist(nodos, INF), padre(nodos, -1), idArista(nodos, -1);
            dist[fuente] = 0;
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
            pq.push({0, fuente});

            while (!pq.empty()) {
                int u = pq.top().second;
                int d = pq.top().first;
                pq.pop();

                if (d > dist[u]) continue;

                for (int i = 0; i < adj[u].size(); i++) {
                    Arista& e = adj[u][i];
                    if (e.capacidad - e.flujo > 0 && dist[u] + e.costo < dist[e.destino]) {
                        dist[e.destino] = dist[u] + e.costo;
                        padre[e.destino] = u;
                        idArista[e.destino] = i;
                        pq.push({dist[e.destino], e.destino});
                    }
                }
            }

            if (dist[sumidero] == INF) break;  // No hay camino aumentante

            int flujoCamino = INF;
            for (int v = sumidero; v != fuente; v = padre[v]) {
                int u = padre[v];
                int indiceArista = idArista[v];
                flujoCamino = min(flujoCamino, adj[u][indiceArista].capacidad - adj[u][indiceArista].flujo);
            }

            for (int v = sumidero; v != fuente; v = padre[v]) {
                int u = padre[v];
                int indiceArista = idArista[v];
                adj[u][indiceArista].flujo += flujoCamino;
                adj[v][indiceArista ^ 1].flujo -= flujoCamino;  // arista inversa
            }

            flujoMaximo += flujoCamino;
            costoMinimo += flujoCamino * dist[sumidero];
        }
        return {flujoMaximo, costoMinimo};
    }

private:
    int nodos;
    vector<vector<Arista>> adj;
};

int main() {
    int nodos = 4;  // Número de nodos
    FlujoMaximoCostoMinimo fcm(nodos);

    // Agregar aristas (desde, hasta, capacidad, costo)
    fcm.agregarArista(0, 1, 3, 1);
    fcm.agregarArista(0, 2, 2, 2);
    fcm.agregarArista(1, 2, 1, 1);
    fcm.agregarArista(1, 3, 2, 3);
    fcm.agregarArista(2, 3, 3, 1);

    pair<int, int> resultado = fcm.flujoMaximoCostoMinimo(0, 3);
    cout << "Flujo Máximo: " << resultado.first << endl;
    cout << "Costo Mínimo: " << resultado.second << endl;

    return 0;
}
