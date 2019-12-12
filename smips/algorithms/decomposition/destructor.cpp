#include "benders.h"

Benders::~Benders()
{
    // cout << "benders destructor called\n";
    delete[] d_xvals;
}