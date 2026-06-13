/* Problema 12 - Facilidades (Uncapacitated Facility Location)
 *
 * Decidir quais depositos abrir e como atribuir os clientes, minimizando
 * custo de abertura + custo de atendimento. Cada cliente e atendido por
 * exatamente um deposito aberto.
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int nDep = 3;
        int nCli = 4;

        double f[] = {100, 120, 80};  // custo de abertura de cada deposito

        // c[i][j] = custo do deposito i atender o cliente j
        double c[3][4] = {
            {10, 15, 20, 12},
            { 8, 10, 18, 14},
            {12,  8, 10, 16}
        };

        IloModel model(env);

        // y[i] = 1 se o deposito i e aberto
        IloBoolVarArray y(env, nDep);

        // x[i][j] = 1 se o cliente j e atendido pelo deposito i
        IloArray<IloBoolVarArray> x(env, nDep);
        for (int i = 0; i < nDep; i++)
            x[i] = IloBoolVarArray(env, nCli);

        // Objetivo: min custo de abertura + custo de atendimento
        IloExpr obj(env);
        for (int i = 0; i < nDep; i++) {
            obj += f[i] * y[i];
            for (int j = 0; j < nCli; j++)
                obj += c[i][j] * x[i][j];
        }
        model.add(IloMinimize(env, obj));
        obj.end();

        // Restricao: cada cliente atendido por exatamente um deposito
        for (int j = 0; j < nCli; j++) {
            IloExpr atend(env);
            for (int i = 0; i < nDep; i++)
                atend += x[i][j];
            model.add(atend == 1);
            atend.end();
        }

        // Restricao: cliente so pode ser atendido por deposito aberto
        for (int i = 0; i < nDep; i++)
            for (int j = 0; j < nCli; j++)
                model.add(x[i][j] <= y[i]);

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Problema das Facilidades ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Custo minimo: " << cplex.getObjValue() << endl;
        for (int i = 0; i < nDep; i++) {
            if (cplex.getValue(y[i]) > 0.5) {
                cout << "Deposito " << i + 1 << " aberto, atende clientes: ";
                for (int j = 0; j < nCli; j++)
                    if (cplex.getValue(x[i][j]) > 0.5)
                        cout << j + 1 << " ";
                cout << endl;
            }
        }

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
