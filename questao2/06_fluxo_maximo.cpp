/* Problema 6 - Fluxo Maximo
 *
 * Encontrar o fluxo maximo de s para t em uma rede dirigida, respeitando
 * as capacidades dos arcos e a conservacao de fluxo nos nos intermediarios.
 *
 * Nos: 0=s, 1=v1, 2=v2, 3=v3, 4=v4, 5=v5, 6=v6, 7=t
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int n = 8;
        int s = 0;
        int t = 7;

        // Matriz de capacidades (0 = arco nao existe)
        double cap[8][8] = {
            //s  v1 v2 v3 v4 v5 v6  t
            {0,  5,  4,  6,  0,  0,  0,  0},  // s
            {0,  0,  4,  0,  6,  0,  0,  0},  // v1
            {0,  0,  0,  3,  0,  4,  0,  0},  // v2
            {0,  0,  0,  0,  0,  0,  5,  0},  // v3
            {0,  0,  0,  0,  0,  5,  0,  5},  // v4
            {0,  0,  0,  0,  0,  0,  0,  3},  // v5
            {0,  0,  0,  0,  0,  7,  0,  6},  // v6
            {0,  0,  0,  0,  0,  0,  0,  0}   // t
        };

        IloModel model(env);

        // f[i][j] = fluxo no arco (i,j); limitado pela capacidade
        // (cap = 0 ja forca fluxo 0 em arcos inexistentes)
        IloArray<IloNumVarArray> f(env, n);
        for (int i = 0; i < n; i++) {
            f[i] = IloNumVarArray(env, n);
            for (int j = 0; j < n; j++)
                f[i][j] = IloNumVar(env, 0, cap[i][j]);
        }

        // F = fluxo total de s ate t
        IloNumVar F(env, 0, IloInfinity);

        // Objetivo: max F
        model.add(IloMaximize(env, F));

        // Restricao: conservacao de fluxo nos nos intermediarios
        for (int i = 1; i <= 6; i++) {
            IloExpr saldo(env);
            for (int j = 0; j < n; j++)
                saldo += f[i][j] - f[j][i];
            model.add(saldo == 0);
            saldo.end();
        }

        // Restricao: fluxo total saindo de s = F
        IloExpr saiS(env);
        for (int j = 0; j < n; j++)
            saiS += f[s][j];
        model.add(saiS == F);
        saiS.end();

        // Restricao: fluxo total entrando em t = F
        IloExpr entraT(env);
        for (int i = 0; i < n; i++)
            entraT += f[i][t];
        model.add(entraT == F);
        entraT.end();

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Problema do Fluxo Maximo ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Fluxo maximo: " << cplex.getObjValue() << endl;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (cap[i][j] > 0 && cplex.getValue(f[i][j]) > 1e-6)
                    cout << "Arco (" << i << "," << j << "): "
                         << cplex.getValue(f[i][j]) << " / " << cap[i][j] << endl;

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
