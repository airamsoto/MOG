#include <iostream>
#include <limits.h>
#include <string.h>
#include <queue>
using namespace std;
 
// Número de vértices en el gráfico dado
#define V 6
 
/* Devuelve true si hay un camino desde la fuente de 's' bajando a 't' en
gráfico residual. También se llena de los parent [] para almacenar el camino*/
bool bfs(int rGraph[V][V], int s, int t, int parent[])
{
       // Crear una matriz visitado y marcar todos los vértices como no visitados
       bool visited[V];
       memset(visited, 0, sizeof(visited));
 
       // Creación de una cola, vértice fuente encola y marcar vértices fuente
       // como visitado
       queue <int> q;
       q.push(s);
       visited[s] = true;
       parent[s] = -1;
 
       // Standard BFS Loop
       while (!q.empty())
       {
             int u = q.front();
             q.pop();
 
             for (int v=0; v<V; v++)
             {
                    if (visited[v]==false && rGraph[u][v] > 0)
                    {
                           q.push(v);
                           parent[v] = u;
                           visited[v] = true;
                    }
             }
       }
 
       //Si llegamos a caer en BFS a partir de la fuente, y luego volver
       // true, else false
       return (visited[t] == true);
}
 
// Retorna el maximo flujo de  s a t en el grafico dado
int fordFulkerson(int graph[V][V], int s, int t)
{
       int u, v;
 
       // Crear un gráfico residual y llenar la gráfica residual con
       //capacidades que figuran en el gráfico original como las capacidades residuales
       //En el gráfico residual
       int rGraph[V][V]; // Gráfica residual donde rGraph [i] [j]
       //indica la capacidad residual de borde de i a j (si hay un borde.
       //Si rGraph [i] [j] es 0, entonces todavía no hay)
       for (u = 0; u < V; u++)
             for (v = 0; v < V; v++)
                    rGraph[u][v] = graph[u][v];
 
       int parent[V]; // Esta matriz es llenada por BFS y ruta almacenada
 
       int max_flow = 0; // No hay flujo inicialmente
 
       // Aumentar el flujo mientras que hay camino desde la fuente al sumidero
       while (bfs(rGraph, s, t, parent))
       {
             // Encuentra capacidad residual mínima de los bordes a lo largo del camino
             // llenado por BFS. O podemos decir encontrar el flujo máximo
             // a través de la ruta encontrada.
             int path_flow = INT_MAX;
             for (v=t; v!=s; v=parent[v])
             {
                    u = parent[v];
                    path_flow = min(path_flow, rGraph[u][v]);
             }
 
// Actualizar las capacidades residuales de las aristas y las aristas a lo largo de la trayectoria inversa
             for (v=t; v != s; v=parent[v])
             {
                    u = parent[v];
                    rGraph[u][v] -= path_flow;
                    rGraph[v][u] += path_flow;
             }
 
             // Add path flow to overall flow
             max_flow += path_flow;
       }
 
       // Devuelve el flujo total
       return max_flow;
}
 
// Programa  Conductor para probar funciones anteriores
int main()
{
       // Vamos a crear un gráfico que se muestra en el ejemplo anterior
       int graph[V][V] = { {0, 16, 13, 0, 0, 0}, //c01c02
                           {0, 0, 10, 12, 0, 0}, //c12c13
                           {0, 4, 0, 0, 14, 0}, //c21c24
                           {0, 0, 9, 0, 0, 20}, //c32c35
                           {0, 0, 0, 7, 0, 4}, //c43c45
                           {0, 0, 0, 0, 0, 0}
                                  };
 
       cout << "El flujo máximo posible es: " << fordFulkerson(graph, 0, 5)<<"\n";
 
       return 0;
}
