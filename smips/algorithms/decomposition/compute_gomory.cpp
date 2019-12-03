#include "benders.h"

double Benders::compute_gomory(size_t s, int *vBasis, int *cBasis, double *ws, double *alpha)
{
  vector<double> basis(d_n2 + d_m2);
  copy(vBasis, vBasis + d_n2, basis.begin());
  copy(cBasis, cBasis + d_m2, basis.begin() + d_n2);
  
  vector<vector<double>> &visited_bases = d_visited[s];
  
  auto end = visited_bases.end();
  auto begin = visited_bases.begin();
  auto it = find(begin, end, basis);

  double gom_obj;
  if (it == end)  // not visited, update and solve d_gomory
  {
    double omega_alpha[d_m2];  // rhs vector of gomory relaxation  
    for (size_t row = 0; row!= d_m2; ++row)        // compute element-by-element
      omega_alpha[row] = ws[row] - alpha[row];  
        
    d_gomory.update(omega_alpha, vBasis, cBasis);  // update gomory relaxation
  
    gom_obj = d_gomory.solve();                    // solve gomory relaxation and store objective bound 

    visited_bases.push_back(basis);                // basis is now visited: store it 
    d_objectives[s].push_back(gom_obj);            // store corresponding objective value   
  } else          // visited before 
  {
    size_t idx = distance(begin, it);              // find index
    gom_obj = d_objectives[s][idx];                // and retrieve corresponding objective value
  }

  return gom_obj;
}