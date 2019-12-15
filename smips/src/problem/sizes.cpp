#include "problem.h"

void Problem::sizes(size_t S)
{
    clear_sub();
    // bookkeeping
    size_t N = 10;
    size_t z_start = 0;  // z-variables: 0-9,
    size_t y_start = N;
    size_t x_start = 2 * N;
    size_t nVars = x_start + N * (N + 1) / 2;
    size_t nConstrs = 3 * N + 1;

    double bigM = 201;
    double setup_cost = 453;
    double sub_cost = 8;
    double y_base = 748;
    double y_inc = 10.4;
    double prod_cap = 200;

    std::vector<double> fs_demand{2.5, 7.5, 12.5, 10, 35, 25, 15, 12.5, 12.5, 5};

    // size params
    d_n1 = nVars;
    d_p1 = N;
    d_m1 = nConstrs;
    d_fs_leq = 2 * N + 1;
    d_fs_geq = N;
    d_n2 = nVars;
    d_p2 = N;
    d_m2 = nConstrs;
    d_ss_leq = 2 * N + 1;
    d_ss_geq = N;
    d_S = S;

    // A, T, and W
    std::vector<std::vector<double>> Amat;

    // filling first N rows of Amat
    size_t start = x_start;
    for (size_t row = 0; row != N; ++row)
    {
        std::vector<double> Arow(nVars, 0);
        size_t nOnes = row + 1;  // number of ones (length of e vector)
        std::fill_n(&Arow[start], nOnes, 1);  // fill with ones

        Arow[y_start + row] = -1;  // corresponds to y variable
        start += nOnes;
        Amat.push_back(Arow);
    }

    // filling next N rows of Amat
    for (size_t row = 0; row != N; ++row)
    {
        std::vector<double> Arow(nVars, 0);
        Arow[y_start + row] = 1;
        Arow[z_start + row] = -bigM;
        Amat.push_back(Arow);
    }

    // next row of Amat
    std::vector<double> Arow(nVars, 0);
    std::fill_n(&Arow[y_start], N, 1);
    Amat.push_back(Arow);

    // final N rows of Amat
    for (size_t row = 0; row != N; ++row)
    {
        std::vector<double> Arow(nVars, 0);
        Amat.push_back(Arow);
    }
    start = x_start;
    for (size_t idx = 1; idx != N + 1; ++idx)
    {
        for (size_t nz = 0; nz != idx; ++nz)
            Amat[2 * N + 1 + nz][start + idx - 1 - nz] = 1;
        start += idx;
    }

    std::vector<std::vector<double>> Wmat = Amat;
    std::vector<std::vector<double>> Tmat(&Amat[0], &Amat[N]);
    for (size_t row = 0; row != 2 * N + 1; ++row)
    {
        std::vector<double> Trow(nVars, 0);
        Tmat.push_back(Trow);
    }

    // (incorporating a typo by the original authors)
    Tmat[5][15] = 0;
    Tmat[4][15] = -1;

    d_Amat = Amat;
    d_Wmat = Wmat;
    d_Tmat = Tmat;

    // enforcing binary decisions
    std::vector<double> lb(nVars, 0.0);
    d_l1 = lb;
    d_l2 = lb;

    std::vector<double> ub(nVars, 1e20);
    std::fill_n(ub.begin(), N, 1.0);
    d_u1 = ub;
    d_u2 = ub;

    // costs
    std::vector<double> costs(nVars);
    std::fill(&costs[0], &costs[y_start], setup_cost);
    for (size_t var = y_start; var != x_start; ++var)
        costs[var] = y_base + (var - y_start) * y_inc;

    std::fill(&costs[x_start], &costs[nVars], sub_cost);
    size_t jump = 0;
    for (size_t var = 0; var != N; ++var)
    {
        costs[x_start + jump] = 0;
        jump += (var + 1);
    }

    d_c = costs;

    std::vector<double> rhs(nConstrs, 0);
    rhs[2 * N] = prod_cap;
    copy(fs_demand.begin(), fs_demand.end(), &rhs[2 * N + 1]);

    d_b = rhs;

    if (S != 3 && S != 5 && S != 10)
        std::cout << "This problem does not exist: S has to be 3, 5, or 10.\n";

    double q_s = roundf(sub_cost / S * 100) / 100 * S;
    // second-stage costs (different due to rounding)

    std::fill(&costs[x_start], &costs[nVars], q_s);
    jump = 0;
    for (size_t var = 0; var != N; ++var)
    {
        costs[x_start + jump] = 0;
        jump += (var + 1);
    }

    std::vector<double> y_costs{
        748, 758.4, 768.8, 779.2, 789.6, 800, 810.4, 820.8, 831.2, 841.6};

    for (size_t var = 0; var != N; ++var)
        y_costs[var] = roundf(y_costs[var] / S * 100) / 100 * S;

    copy(y_costs.begin(), y_costs.end(), &costs[y_start]);
    d_q = costs;

    std::vector<double> probs(S);
    fill_n(probs.begin(), d_S, 1.0 / S);
    d_probs = probs;

    std::vector<std::vector<double>> demands;

    if (S == 3)
        demands = {{1.75, 5.25, 8.75, 7, 24.5, 17.5, 10.5, 8.75, 8.75, 3.5},
                   {2.5, 7.5, 12.5, 10, 35, 25, 15, 12.5, 12.5, 5},
                   {3.25, 9.75, 16.25, 13, 45.5, 32.5, 19.5, 16.25, 16.25, 6.5}};
    if (S == 5)
        demands = {{1.5, 4.5, 7.5, 6., 21, 15, 9, 7.5, 7.5, 3},
                   {2, 6, 10, 8, 28, 20, 12, 10, 10, 4},
                   {2.5, 7.5, 12.5, 10, 35, 25, 15, 12.5, 12.5, 5},
                   {3, 9, 15, 12, 42, 30, 18, 15, 15, 6},
                   {3.5, 10.5, 17.5, 14, 49, 35, 21, 17.5, 17.5, 7}};
    if (S == 10)
        demands = {{1.25, 3.75, 6.25, 5, 17.5, 12.5, 7.5, 6.25, 6.25, 2.5},
                   {1.5, 4.5, 7.5, 6, 21, 15, 9, 7.5, 7.5, 3},
                   {1.75, 5.25, 8.75, 7, 24.5, 17.5, 10.5, 8.75, 8.75, 3.5},
                   {2, 6, 10, 8, 28, 20, 12, 10, 10, 4},
                   {2.25, 6.75, 11.25, 9, 31.5, 22.5, 13.5, 11.25, 11.25, 4.5},
                   {2.75, 8.25, 13.75, 11, 38.5, 27.5, 16.5, 13.75, 13.75, 5.5},
                   {3, 9, 15, 12, 42, 30, 18, 15, 15, 6},
                   {3.25, 9.75, 16.25, 13, 45.5, 32.5, 19.5, 16.25, 16.25, 6.5},
                   {3.5, 10.5, 17.5, 14, 49, 35, 21, 17.5, 17.5, 7},
                   {3.75, 11.25, 18.75, 15, 52.5, 37.5, 22.5, 18.75, 18.75, 7.5}};

    std::vector<std::vector<double>> omega;

    for (size_t s = 0; s != d_S; ++s)
    {
        std::vector<double> scenario(nConstrs, 0);
        scenario[2 * N] = prod_cap;
        std::copy(demands[s].begin(), demands[s].end(), &scenario[2 * N + 1]);
        omega.push_back(scenario);
    }

    d_omega = omega;
}
