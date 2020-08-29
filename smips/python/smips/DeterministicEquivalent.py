from utils import CppClass, get_library
from .ProblemData import ProblemData

_LIB = get_library()


class DeterministicEquivalent(CppClass):

    def __init__(self, problem_data: ProblemData):
        self._obj = _LIB.DeterministicEquivalent_new(problem_data.obj)

    def solve(self, time_limit=float("inf")) -> float:
        res = _LIB.DeterministicEquivalent_solve(self.obj, time_limit)

        # TODO

    def firstStageObjective(self) -> float:
        res = _LIB.DeterministicEquivalent_firstStageObjective(self.obj)

        # TODO

    def secondStageObjective(self) -> float:
        res = _LIB.DeterministicEquivalent_secondStageObjective(self.obj)

        # TODO

    def objective(self) -> float:
        res = _LIB.DeterministicEquivalent_objective(self.obj)

        # TODO
