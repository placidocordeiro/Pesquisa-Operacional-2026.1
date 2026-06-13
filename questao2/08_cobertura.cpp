/* Problema 8 - Cobertura de Conjuntos
 *
 * Construir o minimo de escolas em 6 bairros tal que todo bairro esteja
 * coberto (tem escola propria ou em bairro vizinho).
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int n = 6;  // bairros

        // cover[i][j] = 1 se escola no bairro j cobre o bairro i
        int cover[6][6] = {
            {1, 1, 1, 0, 0, 0},  // bairro 1: coberto por {1,2,3}
            {1, 1, 0, 1, 0, 0},  // bairro 2: coberto por {1,2,4}
            {1, 0, 1, 0, 1, 0},  // bairro 3: coberto por {1,3,5}
            {0, 1, 0, 1, 1, 1},  // bairro 4: coberto por {2,4,5,6}
            {0, 0, 1, 1, 1, 0},  // bairro 5: coberto por {3,4,5}
            {0, 0, 0, 1, 0, 1}   // bairro 6: coberto por {4,6}
        };

        IloModel model(env);

        // y[j] = 1 se uma escola e construida no bairro j
        IloBoolVarArray y(env, n);

        // Objetivo: min numero de escolas
        IloExpr obj(env);
        for (int j = 0; j < n; j++)
            obj += y[j];
        model.add(IloMinimize(env, obj));
        obj.end();

        // Restricao: todo bairro deve ser coberto por pelo menos uma escola
        for (int i = 0; i < n; i++) {
            IloExpr cobertura(env);
            for (int j = 0; j < n; j++)
                if (cover[i][j])
                    cobertura += y[j];
            model.add(cobertura >= 1);
            cobertura.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Problema de Cobertura (Escolas) ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Numero minimo de escolas: " << cplex.getObjValue() << endl;
        cout << "Escolas nos bairros: ";
        for (int j = 0; j < n; j++)
            if (cplex.getValue(y[j]) > 0.5)
                cout << j + 1 << " ";
        cout << endl;

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
