from __future__ import annotations

from utils import CppClass, get_library

_LIB = get_library()


class ProblemData(CppClass):

    def __init__(self,
                 A,
                 T,
                 W,
                 scenarios,
                 scenario_probabilities,
                 first_stage_constr_senses,
                 second_stage_constr_senses,
                 first_stage_var_types,
                 second_stage_var_types,
                 first_stage_coeffs,
                 second_stage_coeffs,
                 first_stage_lower_bound,
                 first_stage_upper_bound,
                 second_stage_lower_bound,
                 second_stage_upper_bound,
                 first_stage_rhs):
        pass

    @classmethod
    def fromSmps(cls, location: str) -> ProblemData:
        pass

    def firstStageConstrSenses(self):
        pass

    def secondStageConstrSenses(self):
        pass

    def firstStageVarTypes(self):
        pass

    def secondStageVarTypes(self):
        pass

    def nScenarios(self):
        pass

    def Amat(self):
        pass

    def Tmat(self):
        pass

    def Wmat(self):
        pass

    def firstStageCoeffs(self):
        pass

    def secondStageCoeffs(self):
        pass

    def firstStageRhs(self):
        pass

    def firstStageLowerBound(self):
        pass

    def firstStageUpperBound(self):
        pass

    def secondStageLowerBound(self):
        pass

    def secondStageUpperBound(self):
        pass

    def scenarios(self):
        pass

    def probability(self, scenario: int) -> float:
        pass
