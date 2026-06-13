/* Problema 11 - Padroes (Latinhas)
 *
 * Fabrica de latinhas com 4 padroes de impressao em 2 tipos de folha.
 * Maximizar o lucro = 50 por latinha vendida - 5 por corpo extra
 * - 3 por tampa extra, que simplifica para 61L - 5C - 3T, onde
 * L = latinhas, C = total de corpos e T = total de tampas.
 * Uma latinha = 1 corpo + 2 tampas.
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int nPad = 4;

        int corpo[]    = {1, 2, 0, 4};
        int tampa[]    = {7, 3, 9, 4};
        int tempo[]    = {2, 3, 2, 1};
        int tamFolha[] = {1, 2, 1, 1};  // tipo de folha usada pelo padrao

        int maxFolha1 = 200;
        int maxFolha2 = 90;
        int maxTempo  = 100;
        double custoCorpo = 5;
        double custoTampa = 3;

        IloModel model(env);

        // x[k] = numero de impressoes do padrao k (inteiro >= 0)
        IloIntVarArray x(env, nPad, 0, IloIntMax);

        // Variaveis auxiliares: L = latinhas, C = corpos, T = tampas
        IloNumVar L(env, 0, IloInfinity);
        IloNumVar C(env, 0, IloInfinity);
        IloNumVar T(env, 0, IloInfinity);

        // Objetivo: max 61L - 5C - 3T
        model.add(IloMaximize(env, 61 * L - custoCorpo * C - custoTampa * T));

        // Definicao de C e T a partir dos padroes impressos
        IloExpr totCorpos(env), totTampas(env);
        for (int k = 0; k < nPad; k++) {
            totCorpos += corpo[k] * x[k];
            totTampas += tampa[k] * x[k];
        }
        model.add(C == totCorpos);
        model.add(T == totTampas);
        totCorpos.end();
        totTampas.end();

        // Latinhas limitadas pelos corpos e tampas (1 corpo + 2 tampas)
        model.add(L <= C);
        model.add(2 * L <= T);

        // Restricao: folhas de cada tamanho disponiveis
        IloExpr folhas1(env), folhas2(env);
        for (int k = 0; k < nPad; k++) {
            if (tamFolha[k] == 1) folhas1 += x[k];
            else                  folhas2 += x[k];
        }
        model.add(folhas1 <= maxFolha1);
        model.add(folhas2 <= maxFolha2);
        folhas1.end();
        folhas2.end();

        // Restricao: tempo total de impressao
        IloExpr tempoTotal(env);
        for (int k = 0; k < nPad; k++)
            tempoTotal += tempo[k] * x[k];
        model.add(tempoTotal <= maxTempo);
        tempoTotal.end();

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Problema de Padroes (Latinhas) ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Lucro otimo: " << cplex.getObjValue() << endl;
        for (int k = 0; k < nPad; k++)
            cout << "Padrao " << k + 1 << ": "
                 << cplex.getValue(x[k]) << " impressoes" << endl;
        cout << "Latinhas vendidas: " << cplex.getValue(L) << endl;
        cout << "Total de corpos:   " << cplex.getValue(C) << endl;
        cout << "Total de tampas:   " << cplex.getValue(T) << endl;

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
