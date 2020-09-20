import ast
import codecs
import glob
import itertools
import pathlib
import re

TIME = """TIME          Deterministic Equivalent
PERIODS
    x0        R0                       PERIOD1
    y0        R0                       PERIOD2
ENDATA
"""

STOCH = """STOCH         Deterministic Equivalent
SCENARIOS     DISCRETE
{scenarios}ENDATA
"""


class Scenario:
    _counter = itertools.count(0)

    def __init__(self, rhs, prob):
        self._id = next(Scenario._counter)
        self._rhs = rhs
        self._prob = prob

    @classmethod
    def clear(cls):
        cls._counter = itertools.count(0)

    def __str__(self):
        name = f"Scen{self._id:03d}"
        prob = f"{self._prob:1.6f}"

        scenario = f" SC {name}   'ROOT'    {prob}       PERIOD2\n"

        for idx, val in enumerate(self._rhs):
            constr = f"R{idx:<2d}"
            value = f"{val:.5f}"

            scenario += f"    RHS       {constr}       {value}\n"

        return scenario


def main():
    for file in glob.iglob("core/*.mps"):
        loc = pathlib.Path(file)

        for idx in range(6):
            # Write .mps files as .cor files, and decode win-1252 to utf-8.
            with codecs.open(loc, encoding="cp1252") as source, \
                    codecs.open(f"{loc.stem}_{idx}.cor", "w", "utf8") as target:
                target.write(source.read())

            # Make .tim file
            with open(f"{loc.stem}_{idx}.tim", "w") as fh:
                fh.write(TIME)

            # Make .sto file
            size = loc.stem[:5]
            num = loc.stem[6:]

            with open(f"sto/{size}{num}_{idx}.sto") as fh:
                # From https://stackoverflow.com/a/23915099/4316405
                txt = re.sub(r"([^[])\s+([^]])", r"\1, \2", fh.read())
                scenarios = ast.literal_eval(txt)

            Scenario.clear()
            stoch = ""

            for scenario in scenarios:
                scen = Scenario(scenario, 1 / len(scenarios))
                stoch += str(scen)

            with open(f"{loc.stem}_{idx}.sto", "w") as fh:
                fh.write(STOCH.format(scenarios=stoch))


if __name__ == "__main__":
    main()
