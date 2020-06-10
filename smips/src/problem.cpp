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

void Problem::clearSub()
{
    if (not d_isSubProblemInitialised)  // if not already initialized
        return;                         // then do nothing

    // else, clear variables and cosnstraints
    GRBConstr *constrs = d_sub.getConstrs();
    GRBVar *vars = d_sub.getVars();

    size_t nConstrs = d_sub.get(GRB_IntAttr_NumConstrs);
    size_t nVars = d_sub.get(GRB_IntAttr_NumVars);

    for (size_t idx = 0; idx != nConstrs; ++idx)
        d_sub.remove(constrs[idx]);

    for (size_t idx = 0; idx != nVars; ++idx)
        d_sub.remove(vars[idx]);

    delete[] constrs;  // heap allocated and no longer required
    delete[] vars;

    delete[] d_constrs;  // heap allocated, but d_sub is cleared

    // this ensures that: (1) initSub() is called if evaluate()
    // is called, and destructor does not call delete[] d_constrs
    d_isSubProblemInitialised = false;
}

// TODO this should not be a part of Problem
double Problem::evaluate(arma::vec const &x)
{
    // computing cx
    double cx = arma::dot(d_firstStageCoeffs, x);
    arma::vec Tx = d_Tmat * x;

    // computing Q(x)
    if (not d_isSubProblemInitialised)
        initSub();  // initialize subproblem, rhs = 0.0

    double Q = 0.0;

    for (size_t scenario = 0; scenario != nScenarios(); ++scenario)
    {
        arma::vec rhs = scenarios().col(scenario) - Tx;

        d_sub.set(GRB_DoubleAttr_RHS, d_constrs, rhs.memptr(), d_Wmat.n_cols);
        d_sub.optimize();

        auto res = d_scenarioProbabilities[scenario];
        res *= d_sub.get(GRB_DoubleAttr_ObjVal);

        Q += res;
    }

    return cx + Q;
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

    // TODO

    return problem;
}

void Problem::initSub()
{
    char vTypes[d_Wmat.n_rows];
    std::fill(vTypes, vTypes + nSecondStageIntVars(), GRB_INTEGER);
    std::fill(vTypes + nSecondStageIntVars(),
              vTypes + d_Wmat.n_rows,
              GRB_CONTINUOUS);

    GRBVar *vars = d_sub.addVars(d_secondStageLowerBound.memptr(),
                                 d_secondStageUpperBound.memptr(),
                                 d_secondStageCoeffs.memptr(),
                                 vTypes,
                                 nullptr,
                                 d_Wmat.n_rows);

    // constraint senses
    char senses[d_Wmat.n_cols];
    std::fill(senses, senses + d_nSecondStageLeqConstraints, GRB_LESS_EQUAL);
    std::fill(senses + d_nSecondStageLeqConstraints,
              senses + d_nSecondStageLeqConstraints
                  + d_nSecondStageGeqConstraints,
              GRB_GREATER_EQUAL);
    std::fill(senses + d_nSecondStageLeqConstraints
                  + d_nSecondStageGeqConstraints,
              senses + d_Wmat.n_cols,
              GRB_EQUAL);

    // constraint rhs
    double rhs[d_Wmat.n_cols];
    std::fill(rhs, rhs + d_Wmat.n_cols, 0.0);

    GRBLinExpr Wy[d_Wmat.n_cols];
    for (auto iter = d_Wmat.begin(); iter != d_Wmat.end(); ++iter)
        Wy[iter.col()] += *iter * vars[iter.row()];

    // add constraints
    d_constrs = d_sub.addConstrs(Wy, senses, rhs, nullptr, d_Wmat.n_cols);
    d_isSubProblemInitialised = true;

    delete[] vars;
}
