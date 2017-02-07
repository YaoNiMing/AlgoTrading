#include "grids.h"
Grids::Grids(double T, size_t n_dt, double sigma, double theta, double kappa){
	size_t ii;
	this->T = T;
	this->n_dt = n_dt;
	dt = T/n_dt;
	t_grid = new double[n_dt+1];
	for (ii=0;ii<=n_dt;++ii) {
		t_grid[ii] = dt*ii;
	}

	double nsigma,ep_max,ep_min;
	dep = sigma*sqrt(3*dt);
	nsigma  = 5*sigma*(1-exp(-2*kappa*T))/sqrt(2*kappa);
	ep_max = theta + ceil(nsigma / dep) * dep;
	ep_min = theta - ceil(nsigma / dep) * dep;
	n_ep = size_t(rint((ep_max-ep_min)/dep)+1);
	ep_grid = new double[n_ep];
	for (ii=0; ii<n_ep; ++ii) {
		ep_grid[ii] = ep_min + ii*dep;
	}
}



double Grids::get_T() {return T;}

size_t Grids::get_n_dt() {return n_dt;}

double Grids::get_dt() {return dt;}

double *Grids::get_t_grid() {return t_grid;}

double Grids::get_dep() {return dep;}

size_t Grids::get_n_ep() {return n_ep;}

double *Grids::get_ep_grid() {return ep_grid;}

void Grids::del_grids() {
	delete[] t_grid;
	delete[] ep_grid;
}

