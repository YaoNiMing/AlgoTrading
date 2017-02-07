#include "sim.h"
#include <iostream>

Sim::Sim(size_t n_kappa, size_t n_sim, size_t n_dt) {
	simulation = new double *[n_kappa];
	trading_rate = new double *[n_kappa];
	inventory = new double *[n_kappa];
	book_value = new double *[n_kappa];
	for (int ii=0;ii<n_kappa;++ii) {
		simulation[ii] = new double[n_sim*(n_dt+1)];
		trading_rate[ii] = new double[n_sim*(n_dt+1)];
		inventory[ii] = new double[n_sim*(n_dt+1)];
		book_value[ii] = new double[n_sim*(n_dt+1)];
	}
}

void Sim::mkt_simulation(size_t n_dt, double dt, double theta, double sigma, double c, size_t n_sim, double kappa, size_t kappa_i) {
	std::default_random_engine generator;
	std::normal_distribution<double> pdis(0,1);
	size_t i,j,s;
	for (i=0;i<n_sim;++i) {
		s = i*(n_dt+1);
		simulation[kappa_i][s] = theta;
		for (j=0;j<n_dt;++j) {
			simulation[kappa_i][s+j+1] = simulation[kappa_i][s+j] + kappa *
					(theta-simulation[kappa_i][s+j])*dt + 
					sigma * sqrt(dt) * pdis(generator);
		}
	}
	


}

void Sim::empty_sim(size_t n_kappa, size_t n_sim, size_t n_dt) {
	for (int ii=0;ii<n_kappa;++ii) {
		delete[] simulation[ii];
		delete[] trading_rate[ii];
		delete[] inventory[ii];
		delete[] book_value[ii];
	}
	delete[] simulation;
	delete[] trading_rate;
	delete[] inventory;
	delete[] book_value;
}

double **Sim::get_simulation(){ return simulation;}
double **Sim::get_trading_rate(){ return trading_rate;}
double **Sim::get_inventory(){ return inventory;}
double **Sim::get_book_value(){ return book_value;}




