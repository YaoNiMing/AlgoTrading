#include "trading.cpp"

using namespace std;

int main(){
	double *kappa = new double[2];
	kappa[0]=0.5;
	kappa[1]=1.2;
	Trading *t = new Trading(0.5,0.4,0.001,100,1,100,0.5,0.002,2);
	t->trading(kappa);
	return 0;
}
