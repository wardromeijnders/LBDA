#ifndef MAIN_H
#define MAIN_H

#include "cutfamilies/loosebenders.h"
#include "cutfamilies/lpdual.h"
#include "cutfamilies/strongbenders.h"
#include "deterministicequivalent.h"
#include "masterproblem.h"

#include <cstring>
#include <gurobi_c++.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

auto const USAGE = R"(
SMIPS. A program for solving two-stage mixed-integer stochastic programs.

Usage:
    smips [-h] [-m method] [-c cut -l lb -u ub | -t time] <file>

Options:
    -h  Prints this help text.
    -m  Selects solution method. One of {deq, decomp}:
        * "deq". Solves the deterministic equivalent (extensive form).
        * "decomp". Solves a decomposition of the problem into a first- and
          second-stage problem. Default.
    -c  Cut family to use when solving the decomposition. One of {lbda, lp, sb}:
        * "lbda". Loose Benders approximation cuts. Default.
        * "lp". Uses cuts based on the LP dual of the second-stage problem.
        * "sb". Uses strengthened Benders' cuts, derived from the Lagrangian
          relaxation of the second-stage problem.
    -l  Lower bound on the expected cost-to-go in the second stage. Default 0.
    -u  Upper bound on the expected cost-to-go in the second stage. Default +inf.
    -t  Time limit (in seconds) to set when solving the deterministic equivalent.
        No time limit is set by default.

Arguments:
    <file>  Location of the SMPS file triplet to solve. Should not contain any
            extensions.
)";

/**
 * Simple struct that gathers the command-line arguments.
 */
struct Arguments
{
    enum CutType
    {
        LP_DUAL,
        LOOSE_BENDERS,
        STRONG_BENDERS
    };

    enum MethodType
    {
        DETERMINISTIC_EQUIVALENT,
        DECOMPOSITION
    };

    MethodType methodType = DECOMPOSITION;  // solution method to use
    CutType cutType = LOOSE_BENDERS;        // cut family to use
    double timeLimit = arma::datum::inf;    // max. solve time in seconds
    bool printUsage = false;                // print help text?
    double lb = 0;                          // lower bound on theta
    double ub = arma::datum::inf;           // upper bound on theta
    std::string file;                       // smps file location
};

using argument_t = struct Arguments;

/**
 * Parses command-line arguments. Heavily tied into the Posix/Unix way of doing
 * this, using <code>getopt</code>.
 *
 * @throws std::runtime_error when the arguments could not correctly be parsed.
 *
 * @param argc  Number of command-line arguments.
 * @param argv  Command-line arguments.
 * @return      Struct with all parsed arguments.
 */
argument_t parseArguments(int argc, char **argv);

/**
 * Prints the (near) optimal first-stage decisions passed in, and some objective
 * information derived from the method.
 *
 * @param decisions Near optimal first-stage decisions, as a vector.
 * @param method    Solution method used - the deterministic equivalent, or the
 *                  first-stage master problem.
 */
template<class T> void printSolution(arma::vec const &decisions, T &method);

#endif  // MAIN_H
