from __future__ import annotations

import numpy as np
from scipy.sparse import csc_matrix

from .lib.lbdapy import ProblemData as _ProblemData


class ProblemData:

    def __init__(self,
                 A,
                 T,
                 W,
                 scenarios,
                 probs,
                 first_stage_senses,
                 second_stage_senses,
                 first_stage_var_types,
                 second_stage_var_types,
                 first_stage_coeffs,
                 second_stage_coeffs,
                 first_stage_lb,
                 first_stage_ub,
                 second_stage_lb,
                 second_stage_ub,
                 first_stage_rhs):
        # This helpfully converts a lot of Python arguments to the precise types
        # expected by the LBDA+ lib.
        A = csc_matrix(A)
        T = csc_matrix(T)
        W = csc_matrix(W)
        scenarios = np.asarray(scenarios)
        probs = np.asarray(probs)

        # These are a bit special: generally, they are specified as single
        # characters in Python. But the interface expects their integer values,
        # not a single-length Python string. Hence this business with ``ord``.
        first_senses = np.asarray(list(map(ord, first_stage_senses)))
        second_senses = np.asarray(list(map(ord, second_stage_senses)))
        first_var_types = np.asarray(list(map(ord, first_stage_var_types)))
        second_var_types = np.asarray(list(map(ord, second_stage_var_types)))

        first_stage_coeffs = np.asarray(first_stage_coeffs)
        second_stage_coeffs = np.asarray(second_stage_coeffs)
        first_stage_lb = np.asarray(first_stage_lb)
        first_stage_ub = np.asarray(first_stage_ub)
        second_stage_lb = np.asarray(second_stage_lb)
        second_stage_ub = np.asarray(second_stage_ub)
        first_stage_rhs = np.asarray(first_stage_rhs)

        self.obj = _ProblemData(A,
                                T,
                                W,
                                scenarios,
                                probs,
                                first_senses,
                                second_senses,
                                first_var_types,
                                second_var_types,
                                first_stage_coeffs,
                                second_stage_coeffs,
                                first_stage_lb,
                                first_stage_ub,
                                second_stage_lb,
                                second_stage_ub,
                                first_stage_rhs)

    @classmethod
    def from_smps(cls, location: str) -> ProblemData:
        instance = cls.__new__(cls)  # does not invoke __init__, as desired
        super(ProblemData, instance).__init__()  # any base classes

        instance.obj = _ProblemData.from_smps(location)
        return instance

    def Amat(self) -> csc_matrix:
        return csc_matrix(self.obj.Amat())

    def Tmat(self) -> csc_matrix:
        return csc_matrix(self.obj.Tmat())

    def Wmat(self) -> csc_matrix:
        return csc_matrix(self.obj.Wmat())
