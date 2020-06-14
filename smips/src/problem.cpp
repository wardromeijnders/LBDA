#include "problem.h"

#include "smps/smps.h"


Problem::Problem(GRBEnv &env) : d_sub(env)
{
}

Problem::~Problem()
{
    if (d_isSubProblemInitialised)
        delete[] d_constrs;
}

// TODO this should not be a part of Problem
double Problem::evaluate(arma::vec const &x)
{
    // computing Q(x)
    if (not d_isSubProblemInitialised)
        initSub();  // initialize subproblem, rhs = 0.0

    double Q = 0.0;
    arma::vec Tx = d_Tmat * x;

    for (size_t scenario = 0; scenario != nScenarios(); ++scenario)
    {
        arma::vec rhs = scenarios().col(scenario) - Tx;

        d_sub.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), d_Wmat.n_cols);
        d_sub.optimize();

        Q += probability(scenario) * d_sub.get(GRB_DoubleAttr_ObjVal);
    }

    return arma::dot(d_firstStageCoeffs, x) + Q;
}

Problem Problem::fromSmps(char const *location, GRBEnv &env)
{
    smps::Smps smps;
    smps.readSmps(location);

    Problem problem{env};

    problem.d_Amat = smps.Amat();
    problem.d_Wmat = smps.Wmat();
    problem.d_Tmat = smps.Tmat();

    problem.d_firstStageCoeffs = smps.firstStageObjCoeffs();
    problem.d_secondStageCoeffs = smps.secondStageObjCoeffs();

    problem.d_firstStageConstrSenses = smps.firstStageConstrSenses();
    problem.d_secondStageConstrSenses = smps.secondStageConstrSenses();

    problem.d_firstStageVarTypes = smps.firstStageVarTypes();
    problem.d_secondStageVarTypes = smps.secondStageVarTypes();

    problem.d_firstStageLowerBound = smps.firstStageLowerBound();
    problem.d_firstStageUpperBound = smps.firstStageUpperBound();
    problem.d_secondStageLowerBound = smps.secondStageLowerBound();
    problem.d_secondStageUpperBound = smps.secondStageUpperBound();

    problem.d_firstStageRhs = smps.firstStageRhs();

    // TODO

    return problem;
}

void Problem::initSub()
{
    GRBVar *vars = d_sub.addVars(d_secondStageLowerBound.memptr(),
                                 d_secondStageUpperBound.memptr(),
                                 d_secondStageCoeffs.memptr(),
                                 d_secondStageVarTypes.memptr(),
                                 nullptr,
                                 d_Wmat.n_rows);

    GRBLinExpr Wy[d_Wmat.n_cols];
    for (auto iter = d_Wmat.begin(); iter != d_Wmat.end(); ++iter)
        Wy[iter.col()] += *iter * vars[iter.row()];

    arma::vec rhs = arma::zeros(d_Wmat.n_cols);

    d_constrs = d_sub.addConstrs(Wy,
                                 d_secondStageConstrSenses.memptr(),
                                 rhs.memptr(),
                                 nullptr,
                                 d_Wmat.n_cols);

    d_isSubProblemInitialised = true;

    delete[] vars;
}
