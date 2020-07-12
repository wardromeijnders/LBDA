#!/usr/bin/env bash

# Dependencies for Armadillo (including cmake, which we also need!).
apt-get install cmake libopenblas-dev liblapack-dev libarpack2-dev libsuperlu-dev

# Armadillo itself.
apt-get install libarmadillo-dev
