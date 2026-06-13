/* Problema 1 - Racao
 *
 * Maximizar o lucro liquido da producao de duas racoes (AMGS e RE),
 * respeitando os estoques de cereais e carne.
 *
 * Lucro liquido por kg = preco de venda - custo dos insumos:
 *   AMGS: 20 - 5*1 - 1*4 = 11
 *   RE:   30 - 2*1 - 4*4 = 12
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int n = 2;  // produtos: 0=AMGS, 1=RE

        double lucro[]   = {11, 12};  // lucro liquido por kg
        double cereais[] = {5, 2};    // kg de cereais por kg de produto
        double carne[]   = {1, 4};    // kg de carne por kg de produto
        double maxCereais = 30000;
        double maxCarne   = 10000;

        IloModel model(env);

        // x[i] = kg produzidos do produto i (continua >= 0)
        IloNumVarArray x(env, n, 0, IloInfinity);

        // Objetivo: max lucro total
        IloExpr obj(env);
        for (int i = 0; i < n; i++)
            obj += lucro[i] * x[i];
        model.add(IloMaximize(env, obj));
        obj.end();

        // Restricao: estoque de cereais
        IloExpr usoCereais(env);
        for (int i = 0; i < n; i++)
            usoCereais += cereais[i] * x[i];
        model.add(usoCereais <= maxCereais);
        usoCereais.end();

        // Restricao: estoque de carne
        IloExpr usoCarne(env);
        for (int i = 0; i < n; i++)
            usoCarne += carne[i] * x[i];
        model.add(usoCarne <= maxCarne);
        usoCarne.end();

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Problema da Racao ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Lucro otimo: " << cplex.getObjValue() << endl;
        cout << "AMGS (x1): " << cplex.getValue(x[0]) << " kg" << endl;
        cout << "RE   (x2): " << cplex.getValue(x[1]) << " kg" << endl;

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
