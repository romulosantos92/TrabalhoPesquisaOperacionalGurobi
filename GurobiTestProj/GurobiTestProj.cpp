/* Copyright 2020, Gurobi Optimization, LLC

 This example formulates and solves the following simple model
 with PWL constraints:

  maximize
        sum c[j] * x[j]
  subject to
        sum A[i,j] * x[j] <= 0,  for i = 0, ..., m-1
        sum y[j] <= 3
        y[j] = pwl(x[j]),        for j = 0, ..., n-1
        x[j] free, y[j] >= 0,    for j = 0, ..., n-1
  where pwl(x) = 0,     if x  = 0
               = 1+|x|, if x != 0

  Note
   1. sum pwl(x[j]) <= b is to bound x vector and also to favor sparse x vector.
      Here b = 3 means that at most two x[j] can be nonzero and if two, then
      sum x[j] <= 1
   2. pwl(x) jumps from 1 to 0 and from 0 to 1, if x moves from negative 0 to 0,
      then to positive 0, so we need three points at x = 0. x has infinite bounds
      on both sides, the piece defined with two points (-1, 2) and (0, 1) can
      extend x to -infinite. Overall we can use five points (-1, 2), (0, 1),
      (0, 0), (0, 1) and (1, 2) to define y = pwl(x)
*/

#include "gurobi_c++.h"
#include <sstream>
using namespace std;
#include <list>;

#include "SupportLib.h";

int
main(int argc,
    char* argv[])
{
    list<erlangRegister> listaRegistros = SupportLib::leArquivo();
    int restricoesErlang[24] = { 
        50, 200, 80, 120, 50, 80, 
        300, 100, 130, 170, 140, 900, 
        700, 1200, 1500, 1300, 900, 800, 
        900, 700, 500, 400, 100, 30 };


    double MatrizQuadroHorarios6Horas[24][24] = 
    {   {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, };

    /*
    
    */

    //Cria um ambiente vazio
    GRBEnv ambiente = new GRBEnv(true);
    ambiente.start();

    GRBModel modelo = GRBModel(ambiente);
    GRBModel modelo2 = GRBModel(ambiente);

    GRBVar funcionariosIniciandoTurno[24];
    for (int i = 0; i < 24; i++)
    {
        funcionariosIniciandoTurno[i] =  modelo.addVar(0.0, numeric_limits<double>::max(), 0.0, GRB_INTEGER);
    }

    try {
        modelo.setObjective(
        funcionariosIniciandoTurno[0] + funcionariosIniciandoTurno[1] + 
        funcionariosIniciandoTurno[2] + funcionariosIniciandoTurno[3] + 
        funcionariosIniciandoTurno[4] + funcionariosIniciandoTurno[5] + 
        funcionariosIniciandoTurno[6] + funcionariosIniciandoTurno[7] + 
        funcionariosIniciandoTurno[8] + funcionariosIniciandoTurno[9] + 
        funcionariosIniciandoTurno[10] + funcionariosIniciandoTurno[11] + 
        funcionariosIniciandoTurno[12] + funcionariosIniciandoTurno[13] + 
        funcionariosIniciandoTurno[14] + funcionariosIniciandoTurno[15] + 
        funcionariosIniciandoTurno[16] + funcionariosIniciandoTurno[17] + 
        funcionariosIniciandoTurno[18] + funcionariosIniciandoTurno[19] + 
        funcionariosIniciandoTurno[20] + funcionariosIniciandoTurno[21] + 
        funcionariosIniciandoTurno[22] + funcionariosIniciandoTurno[23], GRB_MINIMIZE);
    }
    catch (GRBException e)
    {
        std::cout << endl << "Lascou:" << e.getMessage() << endl;
    }
    
    try {
        for (int i = 0; i < 24; i++)
        {
            modelo.addConstr(
                funcionariosIniciandoTurno[0] * MatrizQuadroHorarios6Horas[0][i] +
                funcionariosIniciandoTurno[1] * MatrizQuadroHorarios6Horas[1][i] +
                funcionariosIniciandoTurno[2] * MatrizQuadroHorarios6Horas[2][i] +
                funcionariosIniciandoTurno[3] * MatrizQuadroHorarios6Horas[3][i] +
                funcionariosIniciandoTurno[4] * MatrizQuadroHorarios6Horas[4][i] +
                funcionariosIniciandoTurno[5] * MatrizQuadroHorarios6Horas[5][i] +
                funcionariosIniciandoTurno[6] * MatrizQuadroHorarios6Horas[6][i] +
                funcionariosIniciandoTurno[7] * MatrizQuadroHorarios6Horas[7][i] +
                funcionariosIniciandoTurno[8] * MatrizQuadroHorarios6Horas[8][i] +
                funcionariosIniciandoTurno[9] * MatrizQuadroHorarios6Horas[9][i] +
                funcionariosIniciandoTurno[10] * MatrizQuadroHorarios6Horas[10][i] +
                funcionariosIniciandoTurno[11] * MatrizQuadroHorarios6Horas[11][i] +
                funcionariosIniciandoTurno[12] * MatrizQuadroHorarios6Horas[12][i] +
                funcionariosIniciandoTurno[13] * MatrizQuadroHorarios6Horas[13][i] +
                funcionariosIniciandoTurno[14] * MatrizQuadroHorarios6Horas[14][i] +
                funcionariosIniciandoTurno[15] * MatrizQuadroHorarios6Horas[15][i] +
                funcionariosIniciandoTurno[16] * MatrizQuadroHorarios6Horas[16][i] +
                funcionariosIniciandoTurno[17] * MatrizQuadroHorarios6Horas[17][i] +
                funcionariosIniciandoTurno[18] * MatrizQuadroHorarios6Horas[18][i] +
                funcionariosIniciandoTurno[19] * MatrizQuadroHorarios6Horas[19][i] +
                funcionariosIniciandoTurno[20] * MatrizQuadroHorarios6Horas[20][i] +
                funcionariosIniciandoTurno[21] * MatrizQuadroHorarios6Horas[21][i] +
                funcionariosIniciandoTurno[22] * MatrizQuadroHorarios6Horas[22][i] +
                funcionariosIniciandoTurno[23] * MatrizQuadroHorarios6Horas[23][i] 
                >= restricoesErlang[i]);
        }
    }
    catch (GRBException e)
    {
        std::cout << endl << "Lascou: " << e.getMessage() << endl;
    }

    std::cout << endl << "Quase la" << endl;
    modelo.optimize();
    GRBVar* solucao = modelo.getVars();

    try {
        for(int i=0; i<24; i++)
        cout << solucao[i].get(GRB_StringAttr_VarName) << " "
            << solucao[i].get(GRB_DoubleAttr_X) << endl;
    }
    catch (GRBException e)
    {
        std::cout << endl << "Lascou:" << e.getMessage() << endl;
    }
    std::cout << endl << "Agora foi" << endl;
    std::cout << (int)modelo.get(GRB_DoubleAttr_ObjVal);

    GRBVar** matrizFuncionarios = new GRBVar*[(int)modelo.get(GRB_DoubleAttr_ObjVal)];
    for (int i = 0; i < (int)modelo.get(GRB_DoubleAttr_ObjVal); i++)
        matrizFuncionarios[i] = new GRBVar[24];

    for (int i = 0; i < (int)modelo.get(GRB_DoubleAttr_ObjVal); i++)
    {
        for (int j = 0; j < 24; j++)
            matrizFuncionarios[i][j] = modelo2.addVar(0.0, 1.0, 0.0, GRB_BINARY);
    }

    /*
    GRBEnv* env = 0;
    GRBVar* x = 0;
    GRBVar* y = 0;

    try {
        // Env and model
        env = new GRBEnv();
        GRBModel model = GRBModel(*env);
        model.set(GRB_StringAttr_ModelName, "gc_pwl_c++");

        // Add variables, set bounds and obj coefficients
        x = model.addVars(n);
        for (int i = 0; i < n; i++) {
            x[i].set(GRB_DoubleAttr_LB, -GRB_INFINITY);
            x[i].set(GRB_DoubleAttr_Obj, c[i]);
        }

        y = model.addVars(n);

        // Set objective to maximize
        model.set(GRB_IntAttr_ModelSense, GRB_MAXIMIZE);

        // Add linear constraints
        for (int i = 0; i < m; i++) {
            GRBLinExpr le = 0;
            for (int j = 0; j < n; j++) {
                le += A[i][j] * x[j];
            }
            model.addConstr(le <= 0);
        }

        GRBLinExpr le1 = 0;
        for (int j = 0; j < n; j++) {
            le1 += y[j];
        }
        model.addConstr(le1 <= 3);

        // Add piecewise constraints
        for (int j = 0; j < n; j++) {
            model.addGenConstrPWL(x[j], y[j], npts, xpts, ypts);
        }

        // Optimize model
        model.optimize();

        for (int j = 0; j < n; j++) {
            cout << "x[" << j << "] = " << x[j].get(GRB_DoubleAttr_X) << endl;
        }

        cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

    }
    catch (GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    }
    catch (...) {
        cout << "Exception during optimization" << endl;
    }

    delete[] x;
    delete[] y;
    delete env;

    */
    return 0;
}