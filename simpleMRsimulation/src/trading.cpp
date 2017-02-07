#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include "sim.h"
#include "grids.h"
#include "strategy.h"

using namespace std;

class Trading{
private:

	double theta;
	double sigma;
	double c;
	size_t n_sim;
	double T;
	size_t n_dt;
	double alpha;
	double phi;
	size_t n_kappa;
	
	Sim *sim;

public:
	Sim * get_sim(){return sim;}
	
	inline void print_list(double *l,int len){for (int i=0;i<len;++i) {cout << l[i] << " ";}}
	Trading(double theta, double sigma, double c, size_t n_sim, double T, size_t n_dt, double alpha, double phi, size_t n_kappa) {
		this->theta = theta;
		this->sigma = sigma;
		this->c = c;
		this->n_sim = n_sim;
		this->T = T;
		this->n_dt = n_dt;
		this->alpha = alpha;
		this->phi = phi;
		this->sim = new Sim(n_kappa, n_sim, n_dt);
		this->n_kappa = n_kappa;
	}

	int trading (double *kappa){
		for (size_t i=0;i<n_kappa;++i) {
			cout << "kappa = " << kappa[i] << endl;
			sim->mkt_simulation(n_dt,T/n_dt,theta,sigma,c,n_sim,kappa[i],i);
			double **simulation=sim->get_simulation();
			double **trading_rate=sim->get_trading_rate();
			double **inventory = sim->get_inventory();
			double **book_value=sim->get_book_value();
			Grids* grid = new Grids(T,n_dt,sigma,theta,kappa[i]);
			Strategy *strtg = new Strategy(alpha,phi,theta,sigma,c,kappa[i], grid,simulation[i],trading_rate[i],inventory[i],book_value[i],n_sim);
			
			grid->del_grids();
		}			
		return 0;	
	}

	void clean_trading() {
		sim->empty_sim(n_kappa,n_sim,n_dt);
	}
};


extern "C" {
	Trading* Trading_new(double theta, double sigma, double c, 
			size_t n_sim, double T, size_t n_dt, double alpha, double phi, size_t n_kappa){ 
		return new Trading(theta,sigma,c,n_sim,T,n_dt,alpha,phi,n_kappa); 
	}
	int trading(Trading* t, double *kappa) { return t->trading(kappa); }
	void clean_trading(Trading * t) { t->clean_trading(); }
	double **getsim_res(Trading* t) {(t->get_sim())->get_simulation(); }
	double **gettr_res(Trading* t) {(t->get_sim())->get_trading_rate(); }
	double **getinv_res(Trading* t) {(t->get_sim())->get_inventory(); }
	double **getbv_res(Trading* t) {(t->get_sim())->get_book_value(); }
}
