#include "cutfamilies/loosebenders.h"
#include "cutfamilies/lpdual.h"
#include "cutfamilies/strongbenders.h"
#include "deterministicequivalent.h"
#include "masterproblem.h"
#include "problemdata.h"
#include "sparse_converters.h"

#include <carma/carma.h>

namespace py = pybind11;

// TODO include the ProblemData getters

PYBIND11_MODULE(lbdapy, m)
{
    py::class_<ProblemData>(m, "ProblemData")
        .def(py::init<arma::sp_mat,
                      arma::sp_mat,
                      arma::sp_mat,
                      arma::mat,
                      arma::vec,
                      arma::Col<char>,
                      arma::Col<char>,
                      arma::Col<char>,
                      arma::Col<char>,
                      arma::vec,
                      arma::vec,
                      arma::vec,
                      arma::vec,
                      arma::vec,
                      arma::vec,
                      arma::vec>(),
             py::arg("Amat"),
             py::arg("Tmat"),
             py::arg("Wmat"),
             py::arg("scenarios"),
             py::arg("scenario_probabilities"),
             py::arg("first_stage_constr_senses"),
             py::arg("second_stage_constr_senses"),
             py::arg("first_stage_var_types"),
             py::arg("second_stage_var_types"),
             py::arg("first_stage_coeffs"),
             py::arg("second_stage_coeffs"),
             py::arg("first_stage_lower_bound"),
             py::arg("first_stage_upper_bound"),
             py::arg("second_stage_lower_bound"),
             py::arg("second_stage_upper_bound"),
             py::arg("first_stage_rhs"))
        .def("from_smps", &ProblemData::fromSmps)
        .def("Amat", &ProblemData::Amat)
        .def("Tmat", &ProblemData::Tmat)
        .def("Wmat", &ProblemData::Wmat);

    py::class_<DeterministicEquivalent>(m, "DeterministicEquivalent")
        .def(py::init<ProblemData const &>())
        .def(
            "solve",
            [](DeterministicEquivalent &detEqv, double timeLimit) {
                auto res = detEqv.solve(timeLimit);
                return carma::col_to_arr(*res);
            },
            py::arg("time_limit") = arma::datum::inf)
        .def("is_optimal", &DeterministicEquivalent::isOptimal)
        .def("mip_gap", &DeterministicEquivalent::mipGap)
        .def("first_stage_objective",
             &DeterministicEquivalent::firstStageObjective)
        .def("second_stage_objective",
             &DeterministicEquivalent::secondStageObjective)
        .def("objective", &DeterministicEquivalent::objective);

    py::class_<MasterProblem>(m, "MasterProblem")
        .def(py::init<ProblemData &, double, double>(),
             py::arg("problem_data"),
             py::arg("lower_bound") = 0.,
             py::arg("upper_bound") = arma::datum::inf)
        .def(
            "solve_with",
            [](MasterProblem &master, CutFamily &cutFamily, double tol) {
                auto res = master.solveWith(cutFamily, tol);
                return carma::col_to_arr(*res);
            },
            py::arg("cut_family"),
            py::arg("tol") = 1e-4)
        .def("first_stage_objective", &MasterProblem::firstStageObjective)
        .def("second_stage_objective", &MasterProblem::secondStageObjective)
        .def("objective", &MasterProblem::objective);

    py::module cf = m.def_submodule("cutfamilies");

    py::class_<CutFamily>(cf, "CutFamily");

    py::class_<LooseBenders, CutFamily>(cf, "LooseBenders")
        .def(py::init<ProblemData const &, arma::vec const &, double>(),
             py::arg("problem_data"),
             py::arg("alpha"),
             py::arg("time_limit") = 1e6);

    py::class_<StrongBenders, CutFamily>(cf, "StrongBenders")
        .def(py::init<ProblemData const &>());

    py::class_<LpDual, CutFamily>(cf, "LpDual")
        .def(py::init<ProblemData const &>());
}
