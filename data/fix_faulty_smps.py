from glob import iglob
from pathlib import Path

try:
    import gurobipy
except ImportError:
    HAS_GUROBI = False
else:
    HAS_GUROBI = True

"""
Simple, one-off script for fixing up SCENARIO-based stoch files, and a time 
file. Can be used to possibly correct problematic SMPS files, though you really
should back those up first before using this script.
"""

# Passed to glob below, so all glob wildcards are allowed here. This is the
# location of all three SMPS files, with extensions {cor, tim, sto}.
WHERE = "sizes/*"


def main():
    if HAS_GUROBI:
        # This basically hijacks Gurobi to do the iffy parsing, and then write
        # a standards-compliant core file as an MPS.
        for file in iglob(WHERE + ".cor"):
            # First renames the core file to have an mps extension, applies
            # Gurobi, and then resets the extension to cor.
            # TODO test this.
            loc = Path(file)
            loc.rename(loc.with_suffix(".mps"))

            m = gurobipy.read(str(loc))
            m.write(loc)

            loc.rename(loc.with_suffix(".cor"))

    for file in iglob(WHERE + ".sto"):
        with open(file) as fh:
            data = fh.readlines()

        parse_stoch(file, data)

    for file in iglob(WHERE + ".tim"):
        with open(file) as fh:
            data = fh.readlines()

        parse_time(file, data)


def parse_stoch(file, data):
    with open(file, "w") as fh:
        for line in data:
            keywords = line.split()

            if keywords[0].strip() == "STOCH":
                line = "STOCH".ljust(14) + keywords[1].strip()
                fh.write(line + '\n')
                continue

            if keywords[0].strip() == "SCENARIOS":
                line = "SCENARIOS".ljust(14) + "DISCRETE\n"
                fh.write(line)
                continue

            if keywords[0].strip() == "SC":
                line = " SC "
                line += keywords[1].strip().ljust(10)
                line += keywords[2].strip().ljust(10)
                line += keywords[3].strip().ljust(15)
                line += keywords[4].strip()

                fh.write(line + "\n")
                continue

            if keywords[0].strip() == "ENDATA":
                fh.write("ENDATA\n")
                break

            line = " " * 4 + keywords[0].strip().ljust(10)
            line += keywords[1].strip().ljust(10)
            line += keywords[2].strip()

            fh.write(line + '\n')


def parse_time(file, data):
    with open(file, "w") as fh:
        for line in data:
            keywords = line.split()

            if keywords[0].strip() == "TIME":
                line = "TIME".ljust(14) + keywords[1].strip()
                fh.write(line + '\n')
                continue

            if keywords[0].strip() == "PERIODS":
                fh.write("PERIODS\n")
                continue

            if keywords[0].strip() == "ENDATA":
                fh.write("ENDATA\n")
                break

            line = " " * 4 + keywords[0].strip().ljust(10)
            line += keywords[1].strip().ljust(25)
            line += keywords[2].strip()

            fh.write(line + '\n')


if __name__ == "__main__":
    main()
