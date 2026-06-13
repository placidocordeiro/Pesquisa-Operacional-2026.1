/* Problema 7 - Escalonamento de Horarios (Enfermeiras)
 *
 * Minimizar o numero de enfermeiras para cobrir a demanda dos 7 dias da
 * semana. Cada enfermeira trabalha 4 dias consecutivos (semana circular)
 * e descansa 3.
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int n = 7;  // dias da semana

        double d[] = {14, 13, 15, 16, 19, 18, 11};  // demanda de cada dia

        // cover[i][j] = 1 se quem comeca no dia i trabalha no dia j
        // (4 dias consecutivos, semana circular)
        int cover[7][7];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                cover[i][j] = 0;
        for (int i = 0; i < n; i++)
            for (int k = 0; k < 4; k++)
                cover[i][(i + k) % n] = 1;

        IloModel model(env);

        // x[i] = numero de enfermeiras que comecam a trabalhar no dia i
        IloIntVarArray x(env, n, 0, IloIntMax);

        // Objetivo: min total de enfermeiras contratadas
        IloExpr obj(env);
        for (int i = 0; i < n; i++)
            obj += x[i];
        model.add(IloMinimize(env, obj));
        obj.end();

        // Restricao: demanda de cada dia deve ser coberta
        for (int j = 0; j < n; j++) {
            IloExpr cobertura(env);
            for (int i = 0; i < n; i++)
                if (cover[i][j])
                    cobertura += x[i];
            model.add(cobertura >= d[j]);
            cobertura.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Escalonamento de Enfermeiras ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Total de enfermeiras: " << cplex.getObjValue() << endl;
        for (int i = 0; i < n; i++)
            cout << "Comecam no dia " << i + 1 << ": "
                 << cplex.getValue(x[i]) << endl;

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
