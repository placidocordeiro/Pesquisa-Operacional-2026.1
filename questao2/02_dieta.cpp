/* Problema 2 - Dieta
 *
 * Encontrar a combinacao de 6 ingredientes que fornece as quantidades
 * minimas das vitaminas A e C com custo minimo.
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int n = 6;  // ingredientes
        int m = 2;  // vitaminas: 0=A, 1=C

        double preco[]    = {35, 30, 60, 50, 27, 22};
        double quantMin[] = {9, 19};

        // vitamina[i][j] = quantidade da vitamina i por unidade do ingrediente j
        double vitamina[2][6] = {
            {1, 0, 2, 2, 1, 2},   // vitamina A
            {0, 1, 3, 1, 3, 2}    // vitamina C
        };

        IloModel model(env);

        // x[j] = quantidade do ingrediente j (continua >= 0)
        IloNumVarArray x(env, n, 0, IloInfinity);

        // Objetivo: min custo total
        IloExpr obj(env);
        for (int j = 0; j < n; j++)
            obj += preco[j] * x[j];
        model.add(IloMinimize(env, obj));
        obj.end();

        // Restricao: quantidade minima de cada vitamina
        for (int i = 0; i < m; i++) {
            IloExpr total(env);
            for (int j = 0; j < n; j++)
                total += vitamina[i][j] * x[j];
            model.add(total >= quantMin[i]);
            total.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Problema da Dieta ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Custo minimo: " << cplex.getObjValue() << endl;
        for (int j = 0; j < n; j++)
            cout << "Ingrediente " << j + 1 << ": " << cplex.getValue(x[j]) << endl;

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
