import numpy as np

from .ProblemData import ProblemData
from .cutfamilies import CutFamily
from .lib.lbdapy import MasterProblem as _MasterProblem


class MasterProblem:

    def __init__(self,
                 problem: ProblemData,
                 lower_bound: float = 0.,
                 upper_bound: float = float("inf")):
        self.obj = _MasterProblem(problem.obj, lower_bound, upper_bound)

    def solve_with(self,
                   cut_family: CutFamily,
                   tolerance: float = 1e-3) -> np.array:
        return np.asarray(self.obj.solve_with(cut_family.obj, tolerance))

    def first_stage_objective(self) -> float:
        return self.obj.first_stage_objective()

    def second_stage_objective(self) -> float:
        return self.obj.second_stage_objective()

    def objective(self) -> float:
        return self.obj.objective()
