/* Problema 3 - Plantio
 *
 * Cooperativa com 3 fazendas planta 3 culturas (milho, arroz, feijao).
 * Limites de area e agua por fazenda, area maxima global por cultura e
 * proporcao de area cultivada igual entre as fazendas. Maximizar o lucro.
 */
#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main() {
    IloEnv env;
    try {
        int nFaz = 3;
        int nCul = 3;  // 0=milho, 1=arroz, 2=feijao

        double areaFaz[]  = {400, 650, 350};    // acres por fazenda
        double aguaFaz[]  = {1800, 2200, 950};  // litros por fazenda
        double areaMax[]  = {660, 880, 400};    // area maxima por cultura
        double aguaCul[]  = {5.5, 4.0, 3.5};    // litros por acre da cultura
        double lucroCul[] = {5000, 4000, 1800}; // lucro por acre da cultura

        const char* nomeCul[] = {"Milho", "Arroz", "Feijao"};

        IloModel model(env);

        // x[i][j] = acres da cultura j plantados na fazenda i
        IloArray<IloNumVarArray> x(env, nFaz);
        for (int i = 0; i < nFaz; i++)
            x[i] = IloNumVarArray(env, nCul, 0, IloInfinity);

        // Objetivo: max lucro total
        IloExpr obj(env);
        for (int i = 0; i < nFaz; i++)
            for (int j = 0; j < nCul; j++)
                obj += lucroCul[j] * x[i][j];
        model.add(IloMaximize(env, obj));
        obj.end();

        // Restricao: limite de area por fazenda
        for (int i = 0; i < nFaz; i++) {
            IloExpr area(env);
            for (int j = 0; j < nCul; j++)
                area += x[i][j];
            model.add(area <= areaFaz[i]);
            area.end();
        }

        // Restricao: limite de agua por fazenda
        for (int i = 0; i < nFaz; i++) {
            IloExpr agua(env);
            for (int j = 0; j < nCul; j++)
                agua += aguaCul[j] * x[i][j];
            model.add(agua <= aguaFaz[i]);
            agua.end();
        }

        // Restricao: area maxima por cultura (somando todas as fazendas)
        for (int j = 0; j < nCul; j++) {
            IloExpr total(env);
            for (int i = 0; i < nFaz; i++)
                total += x[i][j];
            model.add(total <= areaMax[j]);
            total.end();
        }

        // Restricao: proporcao de area usada igual entre fazendas (linearizada)
        // sum_j x[i][j] / areaFaz[i] = sum_j x[i+1][j] / areaFaz[i+1]
        for (int i = 0; i < nFaz - 1; i++) {
            IloExpr usoI(env), usoI1(env);
            for (int j = 0; j < nCul; j++) {
                usoI  += x[i][j];
                usoI1 += x[i + 1][j];
            }
            model.add(areaFaz[i + 1] * usoI == areaFaz[i] * usoI1);
            usoI.end();
            usoI1.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        if (!cplex.solve()) {
            cout << "Falha ao resolver! Status: " << cplex.getStatus() << endl;
            env.end();
            return 1;
        }

        cout << "=== Problema do Plantio ===" << endl;
        cout << "Status: " << cplex.getStatus() << endl;
        cout << "Lucro otimo: " << cplex.getObjValue() << endl;
        for (int i = 0; i < nFaz; i++)
            for (int j = 0; j < nCul; j++)
                cout << "Fazenda " << i + 1 << " - " << nomeCul[j]
                     << ": " << cplex.getValue(x[i][j]) << " acres" << endl;

    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e << endl;
    }
    env.end();
    return 0;
}
