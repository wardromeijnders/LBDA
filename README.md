# Stochastic mixed-integer programming solver (SMIPS)

TODO intro.

## What's needed to build the code?

(Assumes a Unix environment)

- Get CMake. This might already be available on your system. If not, and you
  also need to install Armadillo (see below), the `scripts/install_arma.sh`
  script covers this step as a side-effect.

- Make sure you have Gurobi installed. Any version should do, but the code was
  written and tested with Gurobi 9. The headers should be available in some 
  standard location (e.g., where the linker can find them). Since this is 
  commercial software, you need to perform this step yourself.

- For linear algebra, we use [Armadillo](http://arma.sourceforge.net/), which is
  generally available for most distributions. The code was written and tested with
  Armadillo 9.8. You can install this yourself from the linked website, or run the
  script `scripts/install_arma.sh` - this should work on most Debian-based distro's,
  like Ubuntu. You probably need to be root for the script to work.

- If everything's set-up correctly, you should now be able to compile `smips`.
  Running `cmake build smips && make` should do the trick.
 
- To test if the executable runs correctly, try running `bin/smips -h`. You should
  be greeted by a message detailing usage. 

## Contact

TODO
