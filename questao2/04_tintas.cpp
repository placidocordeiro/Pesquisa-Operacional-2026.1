/* Problema 4 - Tintas
 *
 * Produzir 1000 L de tinta SR e 250 L de tinta SN com custo minimo,
 * misturando SolA, SolB, SEC e COR. Cada tinta exige proporcoes
 * minimas dos componentes SEC e COR.
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int nTin = 2;  // tintas: 0=SR, 1=SN
        int nPro = 4;  // produtos: 0=SolA, 1=SolB, 2=SEC, 3=COR

        double custo[]   = {1.5, 1.0, 4.0, 6.0};  // custo por litro do produto
        double secComp[] = {0.3, 0.6, 1.0, 0.0};  // fracao de SEC em cada produto
        double corComp[] = {0.7, 0.4, 0.0, 1.0};  // fracao de COR em cada produto
        double volume[]  = {1000, 250};           // litros de cada tinta
        double minSEC[]  = {0.25, 0.20};          // proporcao minima de SEC
        double minCOR[]  = {0.50, 0.50};          // proporcao minima de COR

        const char* nomeTin[] = {"SR", "SN"};
        const char* nomePro[] = {"SolA", "SolB", "SEC", "COR"};

        IloModel model(env);

        // x[t][p] = litros do produto p usados na tinta t
        IloArray<IloNumVarArray> x(env, nTin);
        for (int t = 0; t < nTin; t++)
            x[t] = IloNumVarArray(env, nPro, 0, IloInfinity);

        // Objetivo: min custo total
        IloExpr obj(env);
        for (int t = 0; t < nTin; t++)
            for (int p = 0; p < nPro; p++)
                obj += custo[p] * x[t][p];
        model.add(IloMinimize(env, obj));
        obj.end();

        for (int t = 0; t < nTin; t++) {
            // Restricao: volume total da tinta
            IloExpr vol(env);
            for (int p = 0; p < nPro; p++)
                vol += x[t][p];
            model.add(vol == volume[t]);
            vol.end();

            // Restricao: proporcao minima de SEC
            IloExpr sec(env);
            for (int p = 0; p < nPro; p++)
                sec += secComp[p] * x[t][p];
            model.add(sec >= minSEC[t] * volume[t]);
            sec.end();

            // Restricao: proporcao minima de COR
            IloExpr cor(env);
            for (int p = 0; p < nPro; p++)
                cor += corComp[p] * x[t][p];
            model.add(cor >= minCOR[t] * volume[t]);
            cor.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Problema das Tintas ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Custo minimo: " << cplex.getObjValue() << endl;
        for (int t = 0; t < nTin; t++)
            for (int p = 0; p < nPro; p++)
                cout << "Tinta " << nomeTin[t] << " - " << nomePro[p]
                     << ": " << cplex.getValue(x[t][p]) << " L" << endl;

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
