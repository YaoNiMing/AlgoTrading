#include <cstdio>
#include <cstdlib>
#include <random>

#ifndef SIM_H
#define SIM_H

class Sim{

private:
	double **simulation;
	double **trading_rate;
	double **inventory;
	double **book_value;
	

public:
	Sim(size_t n_kappa, size_t n_sim, size_t n_dt);
	void mkt_simulation(size_t n_dt, double dt, double theta, double sigma, double c, size_t n_sim, double kappa, size_t kappa_i);
	void empty_sim(size_t n_kappa, size_t n_sim, size_t n_dt);
	double **get_simulation();
	double **get_trading_rate();
	double **get_inventory();
	double **get_book_value();
};
#endif
