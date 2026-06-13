/* Problema 9 - Mochila 0-1 (Knapsack)
 *
 * Selecionar itens para uma mochila de capacidade W = 10 kg maximizando
 * o valor total, sem ultrapassar o peso. Cada item entra no maximo 1 vez.
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int n = 6;

        double peso[]  = {4, 3, 3, 2, 5, 2};
        double valor[] = {3, 5, 2, 3, 6, 4};
        double W = 10;

        IloModel model(env);

        // x[i] = 1 se o item i e colocado na mochila
        IloBoolVarArray x(env, n);

        // Objetivo: max valor total
        IloExpr obj(env);
        for (int i = 0; i < n; i++)
            obj += valor[i] * x[i];
        model.add(IloMaximize(env, obj));
        obj.end();

        // Restricao: capacidade da mochila
        IloExpr pesoTotal(env);
        for (int i = 0; i < n; i++)
            pesoTotal += peso[i] * x[i];
        model.add(pesoTotal <= W);
        pesoTotal.end();

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Problema da Mochila 0-1 ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Valor otimo: " << cplex.getObjValue() << endl;
        cout << "Itens escolhidos: ";
        double pesoUsado = 0;
        for (int i = 0; i < n; i++)
            if (cplex.getValue(x[i]) > 0.5) {
                cout << i + 1 << " ";
                pesoUsado += peso[i];
            }
        cout << endl << "Peso usado: " << pesoUsado << " / " << W << endl;

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
