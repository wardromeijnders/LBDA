from lbdapy import ProblemData
from lbdapy.lib.lbdapy.cutfamilies import LpDual as _LpDual

from .CutFamily import CutFamily


class LpDual(CutFamily):

    def __init__(self, problem: ProblemData):
        self.obj = _LpDual(problem.obj)
