#include <cmath>
#include <cstdlib>
#ifndef GRIDS_H
#define GRIDS_H
class Grids
{
private:
	double T;
	size_t n_dt;
	double dt;
	double *t_grid;
	
	double dep;
	size_t n_ep;
	double *ep_grid;

public:
	Grids(double T, size_t n_dt, double sigma, double theta, double kappa);
	double get_T();
	size_t get_n_dt();
	double get_dt();
	double *get_t_grid();
	double get_dep();
	size_t get_n_ep();
	double *get_ep_grid();
	void del_grids();
};
#endif
