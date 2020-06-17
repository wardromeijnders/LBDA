from glob import iglob

"""
Simple, one-off script for fixing up SCENARIO-based stoch files, and a time 
file.
"""


def main():
    for file in iglob("*.sto"):
        with open(file) as fh:
            data = fh.readlines()

        parse_stoch(file, data)

    for file in iglob("*.tim"):
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
