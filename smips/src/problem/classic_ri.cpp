#include "problem.h"

#include <fstream>

void Problem::classic_ri()
{
    clearSub();

    d_n1 = 10;
    d_p1 = 0;
    d_m1 = 0;
    d_fs_leq = 0;
    d_fs_geq = 0;
    d_n2 = 5;
    d_p2 = 5;
    d_m2 = 5;
    d_ss_leq = 0;
    d_ss_geq = 5;
    d_S = 1000;

    std::vector<double> l1(d_n1, 0.0);
    std::vector<double> l2(d_n2, 0.0);
    std::vector<double> u1(d_n1, 1e20);
    std::vector<double> u2(d_n2, 1e20);

    std::vector<double> fs_costs = {3., 3., 1., 1., 2., 4., 3., 1., 3., 2.};
    std::vector<double> ss_costs = {5., 9., 6., 6., 9.};
    std::vector<std::vector<double>> Wmat = {{3, 3, 1, 4, 5},
                                             {1, 3, 4, 3, 5},
                                             {5, 5, 3, 1, 4},
                                             {1, 5, 3, 4, 1},
                                             {2, 5, 5, 5, 5}};

    std::vector<std::vector<double>> Tmat = {{4, 1, 3, 4, 1, 1, 1, 4, 3, 2},
                                             {1, 5, 2, 5, 3, 5, 2, 3, 3, 3},
                                             {4, 3, 4, 2, 1, 4, 5, 4, 4, 1},
                                             {4, 1, 3, 5, 1, 1, 5, 5, 5, 3},
                                             {1, 3, 5, 3, 2, 2, 2, 4, 5, 3}};


    std::vector<std::vector<double>> omega(d_S);

    std::ifstream file;

    file.open("../genAlpha/omega.txt");

    double val;
    size_t counter = 0;

    while (file >> val)
    {
        omega[counter / 5].push_back(val);
        ++counter;
    }


    file.close();

    std::vector<double> probs(d_S, 1.0 / d_S);

    d_l1 = l1;
    d_l2 = l2;
    d_u1 = u1;
    d_u2 = u2;

    d_c = fs_costs;
    d_q = ss_costs;
    d_Wmat = Wmat;
    d_Tmat = Tmat;
    d_omega = omega;
    d_probs = probs;
}
