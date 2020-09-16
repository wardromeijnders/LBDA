from lbdapy import ProblemData
from lbdapy.lib.lbdapy.cutfamilies import StrongBenders as _StrongBenders

from .CutFamily import CutFamily


class StrongBenders(CutFamily):

    def __init__(self, problem: ProblemData):
        self.obj = _StrongBenders(problem.obj)
