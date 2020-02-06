#include "problem.h"

// TODO is this ssv95 or ssv98?
void Problem::ssv95(size_t S,
                    bool fs_continuous,
                    bool ss_binary,
                    bool standard_T)
{
    clearSub();

    d_n1 = 2;
    d_m1 = 0;
    d_fs_leq = 0;
    d_fs_geq = 0;
    d_p1 = fs_continuous ? 0 : 2;
    d_n2 = 4;
    d_p2 = 4;
    d_m2 = 2;
    d_ss_leq = 2;
    d_ss_geq = 0;
    d_S = S * S;

    d_l1 = arma::zeros(d_n1);
    d_l2 = arma::zeros(d_n2);
    d_u1 = std::vector<double>(d_n1, 5.0);

    double ub = ss_binary ? 1.0 : 1e20;
    d_u2 = std::vector<double>(d_n2, ub);

    d_c = std::vector<double>{-1.5, -4};
    d_q = std::vector<double>{-16, -19, -23, -28};

    d_probs = std::vector<double>(d_S, 1.0 / d_S);

    d_L = -320;

    d_Wmat = {{2, 3, 4, 5}, {6, 1, 3, 2}};
    d_Wmat = d_Wmat.t();

    if (standard_T)
        d_Tmat = {{1, 0}, {0, 1}};
    else
        d_Tmat = {{2.0 / 3, 1.0 / 3}, {1.0 / 3, 2.0 / 3}};

    d_Tmat = d_Tmat.t();

    arma::mat omega(d_S, 2);

    double jump = 10.0 / (S - 1);
    std::vector<double> omega_vals(S);

    for (size_t s = 0; s != S; ++s)
        omega_vals[s] = 5 + s * jump;

    for (size_t s1 = 0; s1 != S; ++s1)
        for (size_t s2 = 0; s2 != S; ++s2)
        {
            size_t s = s1 * S + s2;
            omega(s, 0) = omega_vals[s1];
            omega(s, 1) = omega_vals[s2];
        }

    d_omega = omega.t();
}
