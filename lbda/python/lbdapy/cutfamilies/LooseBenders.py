import numpy as np
from lbdapy import ProblemData
from lbdapy.lib.lbdapy.cutfamilies import LooseBenders as _LooseBenders

from .CutFamily import CutFamily


class LooseBenders(CutFamily):

    def __init__(self,
                 problem: ProblemData,
                 alpha: np.array,
                 time_limit: float = 1e3):
        self.obj = _LooseBenders(problem.obj, np.asarray(alpha), time_limit)
