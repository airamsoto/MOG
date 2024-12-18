#include <iostream>
#include <limits.h>
#include <string.h>
#include <queue>
using namespace std;
 
// Número de vértices en el gráfico dado
#define V 6
 
/* Devuelve true si hay un camino desde la fuente de 's' bajando a 't' en
gráfico residual. También se llena de los parent [] para almacenar el camino*/
bool bfs(int grafoResidual[V][V], int fuente, int sumidero, int padre[]) {
       // Crear una matriz visitado y marcar todos los vértices como no visitados
       bool visitado[V];
       memset(visitado, 0, sizeof(visitado));

       // Creación de una cola, vértice fuente encola y marcar vértices fuente
       // como visitado
       queue<int> cola;
       cola.push(fuente);
       visitado[fuente] = true;
       padre[fuente] = -1;

       // Bucle estándar de BFS
       while (!cola.empty()) {
              int u = cola.front();
              cola.pop();

              for (int v = 0; v < V; v++) {
                     if (visitado[v] == false && grafoResidual[u][v] > 0) {
                            cola.push(v);
                            padre[v] = u;
                            visitado[v] = true;
                     }
              }
       }

       // Si llegamos a caer en BFS a partir de la fuente, y luego volver
       // true, else false
       return (visitado[sumidero] == true);
}
 
// Retorna el maximo flujo de s a t en el grafico dado
int fordFulkerson(int grafico[V][V], int fuente, int sumidero) {
       int u, v;

       // Crear un gráfico residual y llenar la gráfica residual con
       // capacidades que figuran en el gráfico original como las capacidades residuales
       // En el gráfico residual
       int graficoResidual[V][V]; // Gráfica residual donde graficoResidual[i][j]
       // indica la capacidad residual de borde de i a j (si hay un borde.
       // Si graficoResidual[i][j] es 0, entonces todavía no hay)
       for (u = 0; u < V; u++) {
              for (v = 0; v < V; v++) {
                     graficoResidual[u][v] = grafico[u][v];
              }
       }

       int padre[V]; // Esta matriz es llenada por BFS y ruta almacenada

       int flujoMaximo = 0; // No hay flujo inicialmente

       // Aumentar el flujo mientras que hay camino desde la fuente al sumidero
       while (bfs(graficoResidual, fuente, sumidero, padre)) {
             
             // Encuentra capacidad residual mínima de los bordes a lo largo del camino
              // llenado por BFS. O podemos decir encontrar el flujo máximo
              // a través de la ruta encontrada.
              int flujoCamino = INT_MAX;
              for (v = sumidero; v != fuente; v = padre[v]) {
                     u = padre[v];
                     flujoCamino = min(flujoCamino, graficoResidual[u][v]);
              }

              // Actualizar las capacidades residuales de las aristas y las aristas a lo largo de la trayectoria inversa
              for (v = sumidero; v != fuente; v = padre[v]) {
                     u = padre[v];
                     graficoResidual[u][v] -= flujoCamino;
                     graficoResidual[v][u] += flujoCamino;
              }

              // Agregar flujo de camino al flujo total
              flujoMaximo += flujoCamino;
       }

       // Devuelve el flujo total
       return flujoMaximo;
}

 
int main() {
    // Grafo 1
    int graph1[V][V] = {
        {0, 16, 13, 0, 0, 0},
        {0, 0, 10, 12, 0, 0},
        {0, 4, 0, 0, 14, 0},
        {0, 0, 9, 0, 0, 20},
        {0, 0, 0, 7, 0, 4},
        {0, 0, 0, 0, 0, 0}
    };

    // Grafo 2
    int graph2[V][V] = {
        {0, 10, 10, 0, 0, 0},
        {0, 0, 2, 4, 8, 0},
        {0, 0, 0, 0, 9, 0},
        {0, 0, 0, 0, 0, 10},
        {0, 0, 0, 6, 0, 10},
        {0, 0, 0, 0, 0, 0}
    };

    cout << "Con Grafo 1, el flujo máximo posible es: " << fordFulkerson(graph1, 0, 5) << "\n";
    cout << "Con Grafo 2, el flujo máximo posible es: " << fordFulkerson(graph2, 0, 5) << "\n";

    return 0;
}
