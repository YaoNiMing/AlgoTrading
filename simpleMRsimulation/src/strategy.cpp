#include "strategy.h"
#include <iostream>
Strategy::Strategy(double alpha, double phi, double theta, double sigma, double c, double kappa, Grids *grid, double *simulation, double *trading_rate, double *inventory, double *cash, size_t n_sim) {
	this->alpha = alpha;
	this->phi = phi;
	this->grid = grid;
	init_h1_g2(theta, sigma, c, kappa);
	sim_trading(c,simulation,trading_rate, inventory, cash, n_sim);
}


void Strategy::init_h1_g2(double theta, double sigma, double c, double kappa) {
	size_t n_dt = grid->get_n_dt();
	size_t n_ep = grid->get_n_ep();
	double dt = grid->get_dt();
	double dep = grid->get_dep();
	double *ep_grid = grid->get_ep_grid();
	g2 = new double[n_dt+1];
	h1 = new double *[n_ep];

	size_t ii;
	int k;
	for (ii=0;ii<n_ep;++ii) {
		h1[ii] = new double[n_dt+1]();
	}
	
		
	double *dh1  = new double[n_ep];
	double *ddh1 = new double[n_ep];
	g2[n_dt] = -alpha;
	for (k=n_dt-1;k>=0;--k) {
		for (ii=1;ii<n_ep-1;++ii) {
			dh1[ii] = (h1[ii+1][k+1]-h1[ii-1][k+1])/(2*dep);
			ddh1[ii] = (h1[ii+1][k+1]-2*h1[ii][k+1]+h1[ii-1][k+1])/(dep*dep);
			h1[ii][k] = h1[ii][k+1]+dt*(kappa*(theta-ep_grid[ii])*(dh1[ii]+1) +
					0.5*sigma*sigma*ddh1[ii] + h1[ii][k+1]*g2[k+1]/c);
		}
		g2[k] = g2[k+1]+dt*(g2[k+1]*g2[k+1]/c - phi);
		h1[n_ep-1][k] = 2*h1[n_ep-2][k] - h1[n_ep-3][k];
		h1[0][k] = 2*h1[1][k] - h1[2][k];
	}
	delete[] dh1;
	delete[] ddh1;	
}

double Strategy::interp_extrap(int l, double *x, double *v, double xq) {
	if (xq <= x[0]) { return linear_interp(x[0],v[0],x[1],v[1],xq); } 
	if (xq >= x[l-1]) { return linear_interp(x[l-2],v[l-2],x[l-1],v[l-1],xq);}
	int ii=1;
	while (1) {
		if (xq <= x[ii]) { return linear_interp(x[ii-1],v[ii-1],x[ii],v[ii],xq); }
		++ii;
	}
}

void Strategy::sim_trading(double c, double *simulation, double *trading_rate, double *inventory, double *cash, size_t n_sim) {
	size_t n_dt = grid->get_n_dt();
	size_t n_ep = grid->get_n_ep();
	double dt = grid->get_dt();
	double *ep_grid = grid->get_ep_grid();
	double **h1tmp = new double *[n_dt+1];
	size_t ii,k,s;
	for (k=0;k<=n_dt;++k){
		h1tmp[k] = new double[n_ep];
		for (ii=0;ii<n_ep;++ii) { h1tmp[k][ii] = h1[ii][k]; }
	}
	for (ii=0;ii<n_sim;++ii) {
		s = ii*(n_dt+1);
		inventory[s] = 0;
		cash[s] = 0;
		for (k=0;k<n_dt;++k) {
			trading_rate[s+k] = 0.5/c*(interp_extrap(n_ep,ep_grid,h1tmp[k],simulation[s+k]) + 2*inventory[s+k]*g2[k]);
			inventory[s+k+1] = inventory[s+k] + trading_rate[s+k] * dt;
			cash[s+k+1] = cash[s+k] - (simulation[s+k] + c*dt*trading_rate[s+k])*trading_rate[s+k]*dt;
		}
		cash[s+n_dt] = cash[s+n_dt-1] + (simulation[s+n_dt-1] - c*inventory[s+n_dt-1])*inventory[s+n_dt-1];
		inventory[s+n_dt] = 0;
	}
	for (k=0;k<=n_dt;++k) {delete[] h1tmp[k];}
	delete[] h1tmp;
}

/*double *Strategy::getfinalX(){
	double* finalX = new double[mkt->getn_sim()];
	int ii;
	int n_dt = grid->getn_dt();
	int n_sim = mkt->getn_sim();
	for (ii=0;ii<n_sim;++ii) {
		finalX[ii] = X[ii][n_dt];
	}
	return finalX;
}

double *Strategy::get_tr10(){
	int n_dt = grid->getn_dt();
	double *tr10 = new double[(n_dt+1)*10];
	for (int i=0;i<(n_dt+1)*10;++i) {
		tr10[i] = nu[i/(n_dt+1)][i%(n_dt+1)]; 
	}
	return tr10;
}

double *Strategy::get_inv10(){
	int n_dt = grid->getn_dt();
	double *inv10 = new double[(n_dt+1)*10];
	for (int i=0;i<(n_dt+1)*10;++i) {
		inv10[i] = Q[i/(n_dt+1)][i%(n_dt+1)];
	}
	return inv10;
}

double *Strategy::get_bv10(){
	int n_dt = grid->getn_dt();
	double *bv10 = new double[(n_dt+1)*10];
	double **ep_path = mkt->getep_paths();
	for (int i=0;i<(n_dt+1)*10;++i) {
		bv10[i] = X[i/(n_dt+1)][i%(n_dt+1)] + 
			Q[i/(n_dt+1)][i%(n_dt+1)] * ep_path[i/(n_dt+1)][i%(n_dt+1)];
	}
	return bv10;
}*/
