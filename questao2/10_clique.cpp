/* Problema 10 - Clique Maxima
 *
 * Dado um grafo G=(V,E), encontrar o maior subconjunto de vertices onde
 * todos sao adjacentes entre si. Restricao: dois vertices NAO adjacentes
 * nao podem estar ambos na clique.
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int n = 6;

        // adj[i][j] = 1 se existe aresta entre i e j
        int adj[6][6] = {
            {0, 1, 1, 1, 0, 0},
            {1, 0, 1, 0, 1, 0},
            {1, 1, 0, 1, 1, 0},
            {1, 0, 1, 0, 1, 1},
            {0, 1, 1, 1, 0, 1},
            {0, 0, 0, 1, 1, 0}
        };

        IloModel model(env);

        // x[i] = 1 se o vertice i pertence a clique
        IloBoolVarArray x(env, n);

        // Objetivo: max tamanho da clique
        IloExpr obj(env);
        for (int i = 0; i < n; i++)
            obj += x[i];
        model.add(IloMaximize(env, obj));
        obj.end();

        // Restricao: vertices nao adjacentes nao podem estar ambos na clique
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if (adj[i][j] == 0)
                    model.add(x[i] + x[j] <= 1);

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Problema da Clique Maxima ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Tamanho da clique maxima: " << cplex.getObjValue() << endl;
        cout << "Vertices na clique: ";
        for (int i = 0; i < n; i++)
            if (cplex.getValue(x[i]) > 0.5)
                cout << i + 1 << " ";
        cout << endl;

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
