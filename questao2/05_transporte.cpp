/* Problema 5 - Transporte
 *
 * Distribuir produtos de 3 fabricas (ofertas) para 3 depositos (demandas)
 * minimizando o custo total de transporte. Problema balanceado (280 = 280).
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int nFab = 3;
        int nDep = 3;

        double custo[3][3] = {
            {8,  5,  6},
            {15, 10, 12},
            {3,  9,  10}
        };
        double oferta[]  = {120, 80, 80};
        double demanda[] = {150, 70, 60};

        IloModel model(env);

        // x[i][j] = quantidade transportada da fabrica i para o deposito j
        IloArray<IloNumVarArray> x(env, nFab);
        for (int i = 0; i < nFab; i++)
            x[i] = IloNumVarArray(env, nDep, 0, IloInfinity);

        // Objetivo: min custo total de transporte
        IloExpr obj(env);
        for (int i = 0; i < nFab; i++)
            for (int j = 0; j < nDep; j++)
                obj += custo[i][j] * x[i][j];
        model.add(IloMinimize(env, obj));
        obj.end();

        // Restricao: capacidade (oferta) de cada fabrica
        for (int i = 0; i < nFab; i++) {
            IloExpr saida(env);
            for (int j = 0; j < nDep; j++)
                saida += x[i][j];
            model.add(saida <= oferta[i]);
            saida.end();
        }

        // Restricao: demanda de cada deposito deve ser atendida
        for (int j = 0; j < nDep; j++) {
            IloExpr entrada(env);
            for (int i = 0; i < nFab; i++)
                entrada += x[i][j];
            model.add(entrada == demanda[j]);
            entrada.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Problema do Transporte ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Custo minimo: " << cplex.getObjValue() << endl;
        for (int i = 0; i < nFab; i++)
            for (int j = 0; j < nDep; j++)
                if (cplex.getValue(x[i][j]) > 1e-6)
                    cout << "Fabrica " << i + 1 << " -> Deposito " << j + 1
                         << ": " << cplex.getValue(x[i][j]) << endl;

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
