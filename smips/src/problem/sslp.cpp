#include "problem.h"

void Problem::sslp(size_t nServers, size_t nClients, size_t S)
{
    clear_sub();

    double penalty = 1000.0;
    double cap = nServers == 15 ? 112 : 188;

    size_t nPairs = nServers * nClients;

    d_n1 = nServers;
    d_p1 = nServers;
    d_m1 = 0;
    d_fs_leq = 0;
    d_fs_geq = 0;
    d_n2 = nPairs + nServers;
    d_p2 = nPairs;
    d_m2 = nServers + nClients;
    d_ss_leq = nServers;
    d_ss_geq = 0;
    d_S = S;

    std::vector<double> l1(d_n1, 0);
    std::vector<double> u1(d_n1, 1);  // fs variables are binary
    std::vector<double> l2(d_n2, 0);
    std::vector<double>
        u2(d_n2,
           1e20);  // first nPairs second-stage variables are binary

    std::fill_n(&u2[nPairs],
                nServers,
                1e20);  // upper bounds of continuous variables

    std::vector<double> fs_costs(d_n1, 0);
    std::vector<double> ss_costs(d_n2, 0);

    std::fill_n(&ss_costs[nPairs],
                nServers,
                penalty);  // lazy approach: check if correct if mistakes occur

    std::vector<std::vector<double>> Wmat;
    for (size_t con = 0; con != d_m2; ++con)
    {
        std::vector<double> row(d_n2, 0);
        Wmat.push_back(row);
    }

    for (size_t con = 0; con != nServers; ++con)
        Wmat[con][nPairs + con] = -1;

    std::vector<std::vector<double>> Tmat;
    for (size_t con = 0; con != d_m2; ++con)
    {
        std::vector<double> row(d_n1, 0);
        Tmat.push_back(row);
    }

    for (size_t var = 0; var != d_n1; ++var)
        Tmat[var][var] = -cap;

    // reading c, q, and W from the .lp-file of a scenario problem.
    std::ifstream lpFile;
    std::string filename = "data/sslp/sslp" + std::to_string(nServers) + "_"
                           + std::to_string(nClients) + ".lp";
    lpFile.open(filename);

    bool constraints = false;  // did we already encounter "Subject To"?
    size_t con = 0;            // constraint counter

    std::string line;
    std::vector<std::string> text;
    while (lpFile >> line)
    {
        if (line == "Bounds")
            break;

        text.push_back(line);
        if (line == "To")
            constraints = true;

        if (not constraints)  // then we read an element of c or q
        {
            if (line[0] == 'y')  // element of q
            {
                size_t pos1 = line.find("_");
                size_t pos2 = line.find("_", pos1 + 1);
                size_t client = stoi(line.substr(pos1 + 1, pos2 - (pos1 + 1)));
                size_t server = stoi(line.substr(pos2 + 1));

                size_t q_pos = (server - 1) * nClients + client - 1;
                std::string q_string = text[text.size() - 2];
                ss_costs[q_pos] = (q_string[0] == '-') ? -1 : -stoi(q_string);
            }

            if (line[0] == 'x')  // element of c or q: determine which is the
                                 // case based on number of underscores
            {
                size_t pos1 = line.find("_");
                size_t pos2 = line.find("_", pos1 + 1);
                if (pos2 == std::string::npos)  // only one underscore has been
                                                // found: element of c
                {
                    size_t server = stoi(line.substr(pos1 + 1));
                    fs_costs[server - 1] = stoi(text[text.size() - 2]);
                }
            }
        }
        else  // past "Subject To"
        {
            if (line[0] == 'c')  // new constraint encountered
                con = stoi(line.substr(1, line.length() - 2))
                      - 2;  // update constraint counter (row of W, start
                            // counting from zero)

            if (line[0] == 'y')  // then we read an element of W
            {
                size_t pos1 = line.find("_");  // as before
                size_t pos2 = line.find("_", pos1 + 1);
                size_t client = stoi(line.substr(pos1 + 1, pos2 - (pos1 + 1)));
                size_t server = stoi(line.substr(pos2 + 1));

                size_t var = (server - 1) * nClients + client - 1;
                double value;
                if (con < nServers)
                {
                    std::string element = text[text.size() - 2];
                    value = (element[0] == '-') ? 1 : stoi(element);
                }
                else
                    value = 1;

                Wmat[con][var] = value;
            }
        }
    }

    lpFile.close();

    std::vector<double> probs(S, 1.0 / S);

    // TODO: omega
    std::vector<std::vector<double>> omega;
    for (size_t s = 0; s != S; ++s)
    {
        std::vector<double> scenario(d_m2);
        omega.push_back(scenario);
    }

    // TODO the overwrite - is this intended?
    d_L = std::accumulate(&ss_costs[0], &ss_costs[nPairs], 0.0);
    d_L = -500;

    std::ifstream stoFile;
    std::string sto_name = "data/sslp/sslp_" + std::to_string(nServers) + "_"
                           + std::to_string(nClients) + "_" + std::to_string(S)
                           + ".sto";
    stoFile.open(sto_name);

    size_t scenario = -1;
    size_t client = nServers;

    while (stoFile >> line)
    {
        if (line == "STAGE-2")
        {
            ++scenario;
            client = nServers;
        }
        if (line.length() == 1)
        {
            omega[scenario][client] = stoi(line);
            ++client;
        }
    }
    stoFile.close();

    d_l1 = l1;
    d_u1 = u1;
    d_l2 = l2;
    d_u2 = u2;

    d_c = fs_costs;
    d_q = ss_costs;
    d_Wmat = Wmat;
    d_Tmat = Tmat;
    d_omega = omega;
    d_probs = probs;
}
