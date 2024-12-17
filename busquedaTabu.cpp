#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>

using namespace std;

struct comparator
{

    bool operator()(const pair<vector<int>, double> &a, const pair<vector<int>, double> &b)
    {
        return a.second < b.second;
    }
};

double funcionObjetivo(vector<int> const &solucion)
{
    double valor = 0.0;

    for (int i = 0; i < solucion.size(); i++)
    {
        valor += pow(solucion[i], 2);
    }

    return valor;
}

vector<vector<int>> generaVecindario(vector<int> const &solucionActual)
{
    vector<vector<int>> vecinos;

    for (int i = 0; i < solucionActual.size(); i++)
    {
        for (int j : {-1, 1})
        {
            vector<int> vecino = solucionActual;
            vecino[i] += j; // Se realizan pequeños cambios sobre la solución actual, miran los vecinos
            vecinos.push_back(vecino);
        }
    }

    return vecinos;
}

vector<int> busquedaTabu(vector<int> const &solucionInicial, int maxIteraciones, deque<vector<int>> &listaTabu)
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

int main()
{

    int dimension = 5;
    vector<int> solucionInicial(dimension);
    int maxIteraciones = 1000;
    int maxTabuSuze = 10;
    deque<vector<int>> listaTabu(maxTabuSuze);

    solucionInicial = {100, 200, 300, 400, 500};

    vector<int> mejorSolucion = busquedaTabu(solucionInicial, maxIteraciones, listaTabu);

    cout << "Mejor solución encontrada: ";
    for (int i : mejorSolucion)
    {
        cout << i << " ";
    }
    cout << "\n--------\n";

    return 0;
}