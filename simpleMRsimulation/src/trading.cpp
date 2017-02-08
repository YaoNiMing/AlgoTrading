#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <pthread.h>
#include "sim.h"
#include "grids.h"
#include "strategy.h"

using namespace std;
typedef struct _thread_kappa_t {
	size_t n_sim;
	size_t n_dt;
	size_t kap_i;
	double T;
	double theta;
	double sigma;
	double c;
	double alpha;
	double phi;
	double kappa;
	Sim *sim;
} thread_kappa_t;

	void *kappa_thread(void *kappa_vp) {
		thread_kappa_t *kappa_p = (thread_kappa_t *)kappa_vp;
		kappa_p->sim->mkt_simulation(kappa_p->n_dt,(kappa_p->T)/(kappa_p->n_dt),kappa_p->theta,kappa_p->sigma,
					kappa_p->c,kappa_p->n_sim,kappa_p->kappa,kappa_p->kap_i);
		double **simulation=kappa_p->sim->get_simulation();
		double **trading_rate=kappa_p->sim->get_trading_rate();
		double **inventory = kappa_p->sim->get_inventory();
		double **book_value= kappa_p->sim->get_book_value();
		
		Grids *grid = new Grids(kappa_p->T,kappa_p->n_dt,kappa_p->sigma,kappa_p->theta,kappa_p->kappa);

		Strategy *strtg = new Strategy(kappa_p->alpha, kappa_p->phi, kappa_p->theta, kappa_p->sigma,
						kappa_p->c,kappa_p->kappa,grid,simulation[kappa_p->kap_i],trading_rate[kappa_p->kap_i],
						inventory[kappa_p->kap_i],book_value[kappa_p->kap_i],kappa_p->n_sim);
		grid->del_grids();

	  	pthread_exit(NULL);
	}


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
		pthread_t thr[n_kappa];
		thread_kappa_t kappa_data[n_kappa];
		int rc;
		for (size_t i=0;i<n_kappa;++i) {
			kappa_data[i].theta = this->theta;
			kappa_data[i].sigma = this->sigma;
			kappa_data[i].c = this->c;
			kappa_data[i].n_sim = this->n_sim;
			kappa_data[i].T = this->T;
			kappa_data[i].n_dt = this->n_dt;
			kappa_data[i].alpha = this->alpha;
			kappa_data[i].phi = this->phi;
			kappa_data[i].kap_i = i;
			kappa_data[i].kappa = kappa[i];
			kappa_data[i].sim = this->sim;
			cout << "starting thread "<<i<<" for kappa = " << kappa[i]<<endl;
			if ((rc = pthread_create(&thr[i],NULL,kappa_thread, &kappa_data[i]))) {
				wcerr<<"error: pthread_create, rc: "<<rc;
				return EXIT_FAILURE;
			}
				
		}
		for (size_t i=0;i<n_kappa;++i) {
			pthread_join(thr[i],NULL);
			cout << "thread " << i << " finished!" <<endl;
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
