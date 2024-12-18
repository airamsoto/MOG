#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>
#include <functional>

using namespace std;

struct comparator
{

    bool operator()(const pair<vector<int>, double> &a, const pair<vector<int>, double> &b)
    {
        return a.second < b.second;
    }
};

vector<int> busquedaTabu(vector<int> const &solucionInicial, int maxIteraciones, deque<vector<int>> &listaTabu,
                         const function<vector<vector<int>>(vector<int> const &)> &generaVecindario, const function<double(vector<int> const &)> &funcionObjetivo)
{

    vector<int> solucionActual = solucionInicial;
    vector<int> mejorSolucion = solucionInicial;
    double mejorValor = funcionObjetivo(mejorSolucion);

    for (int i = 0; i < maxIteraciones; i++)
    {
        vector<vector<int>> vecindario = generaVecindario(solucionActual);

        vector<pair<vector<int>, double>> candidatos;
        for (auto vecino : vecindario)
        {
            if (find(listaTabu.begin(), listaTabu.end(), vecino) == listaTabu.end() || funcionObjetivo(vecino) < mejorValor)
            {
                if (funcionObjetivo(vecino) < mejorValor)
                {
                    candidatos.emplace_back(vecino, funcionObjetivo(vecino));
                }
            }
        }

        if (candidatos.empty())
            break;

        auto mejorCandidato = *min_element(candidatos.begin(), candidatos.end(), comparator());

        solucionActual = mejorCandidato.first;
        if (mejorCandidato.second < mejorValor)
        {
            mejorSolucion = mejorCandidato.first;
            mejorValor = mejorCandidato.second;
        }

        listaTabu.push_back(solucionActual);
        if (listaTabu.size() > listaTabu.max_size())
        {
            listaTabu.pop_front();
        }
    }

    return mejorSolucion;
}

void ejemplo1()
{

    int dimension = 5;
    vector<int> solucionInicial(dimension);
    int maxIteraciones = 1000;
    int maxTabu = 10;
    deque<vector<int>> listaTabu(maxTabu);

    solucionInicial = {100, 200, 300, 400, 500};

    function<vector<vector<int>>(vector<int> const &)> generaVecindario = [](vector<int> const &solucionActual)
    {
        vector<vector<int>> vecinos;

        for (int i = 0; i < solucionActual.size(); i++)
        {
            for (int j : {-1, 0, 1})
            {
                vector<int> vecino = solucionActual;
                vecino[i] += j; // Se realizan pequeños cambios sobre la solución actual, miran los vecinos
                vecinos.push_back(vecino);
            }
        }

        return vecinos;
    };

    function<double(vector<int> const &)> funcionObjetivo = [](vector<int> const &solucion)
    {
        double valor = 0.0;

        for (int i = 0; i < solucion.size(); i++)
        {
            valor += pow(solucion[i], 2); // suma de los cuadrados de los elementos
        }

        return valor;
    };

    vector<int> mejorSolucion = busquedaTabu(solucionInicial, maxIteraciones, listaTabu, generaVecindario, funcionObjetivo);

    cout << "Mejor solución encontrada: ";
    for (int i : mejorSolucion)
    {
        cout << i << " ";
    }
    cout << "\n--------\n";
}

void ejemplo2()
{

    int dimension = 5;
    vector<int> solucionInicial(dimension);
    int maxIteraciones = 100;
    int maxTabu = 20;
    deque<vector<int>> listaTabu(maxTabu);

    solucionInicial = {1, 2, 3, 4, 5};
    int distanciaEntreCiudades[5][5] = {
        {0, 10, 15, 20, 25},
        {10, 0, 35, 25, 30},
        {15, 35, 0, 30, 45},
        {20, 25, 30, 0, 55},
        {25, 30, 45, 55, 0}};

    function<vector<vector<int>>(vector<int> const &)> generaVecindario = [](vector<int> const &solucionActual)
    {
        vector<vector<int>> vecinos;

        for (int i = 0; i < solucionActual.size(); i++)
        {
            for (int j = 0; j < solucionActual.size(); j++)
            {
                vector<int> vecino = solucionActual;
                swap(vecino[i], vecino[j]); // se realizan cambios en el par de vecinos para ver si mejora la solución
                vecinos.push_back(vecino);
            }
        }

        return vecinos;
    };

    function<double(vector<int> const &)> funcionObjetivo = [distanciaEntreCiudades](vector<int> const &solucion)
    {
        double valor = 0.0;

        for (int i = 0; i < solucion.size() - 1; i++)
        {
            valor += distanciaEntreCiudades[solucion[i] - 1][solucion[i + 1] - 1]; // suma de las distancia entre ciudades
        }
        valor += distanciaEntreCiudades[solucion[solucion.size() - 1] - 1][solucion[0] - 1];

        return valor;
    };

    vector<int> mejorSolucion = busquedaTabu(solucionInicial, maxIteraciones, listaTabu, generaVecindario, funcionObjetivo);

    cout << "Mejor solución encontrada:\n ";
    for (int i = 0; i < mejorSolucion.size() - 1; i++)
    {
        cout << mejorSolucion[i] << " -> ";
    }
    cout << mejorSolucion[mejorSolucion.size() - 1] << "\n";
    for (int i = 0; i < mejorSolucion.size() - 1; i++)
    {
        cout << distanciaEntreCiudades[mejorSolucion[i] - 1][mejorSolucion[i + 1] - 1] << " -> ";
    }
    cout << distanciaEntreCiudades[mejorSolucion[mejorSolucion.size() - 1] - 1][mejorSolucion[0] - 1];
    cout << "\n--------\n";
}

int main()
{

    ejemplo1(); // menor suma de cuadrados de los elementos
    ejemplo2(); // menor distancia entre ciudades

    return 0;
}