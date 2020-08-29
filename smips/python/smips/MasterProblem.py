from cutfamilies import CutFamily
from utils import CppClass, get_library
from .ProblemData import ProblemData

_LIB = get_library()


class MasterProblem(CppClass):

    def __init__(self,
                 problem_data: ProblemData,
                 lower_bound: float = 0.,
                 upper_bound: float = float("inf")):
        pass

    def solveWith(self, cut_family: CutFamily, tolerance: float = 1e-4):
        pass

    def firstStageObjective(self) -> float:
        pass

    def secondStageObjective(self) -> float:
        pass

    def objective(self) -> float:
        pass
