import setuptools

MAJOR = 1
MINOR = 0
MAINTENANCE = 0
MODIFIER = ""

VERSION = "{0}.{1}.{2}{3}".format(MAJOR, MINOR, MAINTENANCE, MODIFIER)

setuptools.setup(
    name="lbdapy",
    version=VERSION,
    author="Niels van der Laan and Ward Romeijnders",
    author_email="w.romeijnders@rug.nl",
    description="LBDA+: a mixed-integer two-stage recourse problem solver using"
                " a loose Benders decomposition algorithm.",
    url="https://github.com/wardromeijnders/LBDA",
    project_urls={
        "Tracker": "https://github.com/wardromeijnders/LBDA/issues",
        "Source": "https://github.com/wardromeijnders/LBDA",
    },
    # TODO if this package gets larger this is no longer a durable solution.
    packages=setuptools.find_packages(include=['lbdapy', 'lbdapy.cutfamilies']),
    package_data={'lbdapy': ['lib/*.so', 'lib/*.dll']},
    classifiers=[
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.5",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Intended Audience :: Science/Research",
        "Development Status :: 5 - Production/Stable",
        "Topic :: Scientific/Engineering",
    ],
    python_requires='~=3.5',
    install_requires=[
        'numpy >= 1.15.2',
        'scipy >= 1.5',
    ])
