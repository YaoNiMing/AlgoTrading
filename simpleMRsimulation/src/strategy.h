#include "grids.h"
#ifndef STRTG_H
#define STRTG_H
class Strategy {
private:
	double alpha;
	double phi;
	Grids *grid;
	double *g2;
	double **h1;
	void init_h1_g2(double theta, double sigma, double c, double kappa);
	double interp_extrap(int l, double *x, double *v, double xq);
	void sim_trading(double c, double *simulation, double *trading_rate, double *inventory, double *book_value,size_t n_sim);
public:
	Strategy(double alpha, double phi, double theta, double sigma, double c, double kappa, Grids *grid, double *simulation,double *trading_rate, double *inventory, double *cash, size_t n_sim);
};

inline double linear_interp(double x1, double y1, double x2, double y2, double xq) {
	return (y2*(xq-x1)-y1*(xq-x2))/(x2-x1);
}
#endif
