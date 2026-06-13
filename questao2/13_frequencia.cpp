/* Problema 13 - Frequencia (Coloracao de Grafos)
 *
 * Atribuir frequencias a 5 antenas de modo que antenas com interferencia
 * usem frequencias diferentes, minimizando o numero de frequencias usadas.
 * Equivalente a coloracao minima de grafos.
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int n = 5;  // antenas
        int K = 5;  // limite superior de frequencias (pior caso: n)

        // interf[i][j] = 1 se as antenas i e j interferem entre si
        int interf[5][5] = {
            {0, 1, 1, 0, 0},
            {1, 0, 1, 1, 0},
            {1, 1, 0, 1, 1},
            {0, 1, 1, 0, 1},
            {0, 0, 1, 1, 0}
        };

        IloModel model(env);

        // x[i][k] = 1 se a antena i usa a frequencia k
        IloArray<IloBoolVarArray> x(env, n);
        for (int i = 0; i < n; i++)
            x[i] = IloBoolVarArray(env, K);

        // z[k] = 1 se a frequencia k e usada por alguma antena
        IloBoolVarArray z(env, K);

        // Objetivo: min numero de frequencias usadas
        IloExpr obj(env);
        for (int k = 0; k < K; k++)
            obj += z[k];
        model.add(IloMinimize(env, obj));
        obj.end();

        // Restricao: cada antena usa exatamente uma frequencia
        for (int i = 0; i < n; i++) {
            IloExpr usa(env);
            for (int k = 0; k < K; k++)
                usa += x[i][k];
            model.add(usa == 1);
            usa.end();
        }

        // Restricao: antenas com interferencia usam frequencias diferentes
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if (interf[i][j] == 1)
                    for (int k = 0; k < K; k++)
                        model.add(x[i][k] + x[j][k] <= 1);

        // Restricao: z[k] = 1 se alguma antena usa a frequencia k
        for (int i = 0; i < n; i++)
            for (int k = 0; k < K; k++)
                model.add(x[i][k] <= z[k]);

        // Quebra de simetria: frequencias usadas em ordem
        for (int k = 1; k < K; k++)
            model.add(z[k] <= z[k - 1]);

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Problema de Frequencia (Coloracao) ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Numero minimo de frequencias: " << cplex.getObjValue() << endl;
        for (int i = 0; i < n; i++)
            for (int k = 0; k < K; k++)
                if (cplex.getValue(x[i][k]) > 0.5)
                    cout << "Antena " << i + 1 << " usa frequencia "
                         << k + 1 << endl;

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
