import numpy as np

from .ProblemData import ProblemData
from .lib.lbdapy import DeterministicEquivalent as _DeterministicEquivalent


class DeterministicEquivalent:

    def __init__(self, problem: ProblemData):
        self.obj = _DeterministicEquivalent(problem.obj)

    def solve(self, time_limit: float = float("inf")) -> np.array:
        return np.array(self.obj.solve(time_limit))

    def is_optimal(self) -> bool:
        return self.obj.is_optimal()

    def mip_gap(self) -> float:
        return self.obj.mip_gap()

    def first_stage_objective(self) -> float:
        return self.obj.first_stage_objective()

    def second_stage_objective(self) -> float:
        return self.obj.second_stage_objective()

    def objective(self) -> float:
        return self.obj.objective()
