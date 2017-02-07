import numpy as np
import ctypes as ct
import matplotlib
matplotlib.use("TkAgg")
from matplotlib import pyplot as plt

lib = ct.cdll.LoadLibrary('./bin/trading.so')
#lib.getrr_res.restype = ct.POINTER(ct.c_double)
#lib.getpl_res.restype = ct.POINTER(ct.POINTER(ct.c_double))
lib.Trading_new.restype = ct.c_void_p
lib.getsim_res.restype = ct.POINTER(ct.POINTER(ct.c_double))
lib.gettr_res.restype = ct.POINTER(ct.POINTER(ct.c_double))
lib.getinv_res.restype = ct.POINTER(ct.POINTER(ct.c_double))
lib.getbv_res.restype = ct.POINTER(ct.POINTER(ct.c_double))

class Trading(object):
	def __init__(self,theta,sigma,c,n_sim,T,Ndt,alpha,phi,n_kappa):
		self.obj = lib.Trading_new(theta,sigma,c,n_sim,T,Ndt,alpha,phi,n_kappa)
	def trading(self,kappa):
		lib.trading(ct.c_void_p(self.obj),kappa)
	def getsim_res(self):
		return lib.getsim_res(ct.c_void_p(self.obj))
	def gettr_res(self):
		return lib.gettr_res(ct.c_void_p(self.obj))
	def getinv_res(self):
		return lib.getinv_res(ct.c_void_p(self.obj))
	def getbv_res(self):
		return lib.getbv_res(ct.c_void_p(self.obj))
	def clean_trading(self):
		lib.clean_trading(ct.c_void_p(self.obj))
	

def trading(theta,sigma,c,n_sim,T,Ndt,alpha,phi,kpmin,kpmax,dkp):
	
	
	kappa = np.arange(kpmin,kpmax+1e-4,dkp)+1e-4
	n_kappa = len(kappa)

	
	t = Trading(ct.c_double(theta), ct.c_double(sigma), ct.c_double(c),\
		ct.c_size_t(n_sim),ct.c_double(T), ct.c_size_t(Ndt),\
		ct.c_double(alpha), ct.c_double(phi),ct.c_size_t(n_kappa))
	t.trading((ct.c_double * n_kappa)(*kappa))

	sim=t.getsim_res()
	nu=t.gettr_res()
	q=t.getinv_res()
	cash = t.getbv_res()
	
	
	tx = np.linspace(0,T,Ndt+1)
	for k in range(n_kappa):
		plt.close('all')
		f,ax = plt.subplots()
		ax.hist(cash[k][Ndt:n_sim*(Ndt+1):(Ndt+1)],50)
		ax.set_xlabel('Profit and Loss')
		ax.set_ylabel('Frequency')
		plt.title('kappa = ' + str(kappa[k]))
		plt.savefig('plots/pl'+str(kappa[k]).replace('.',''),bbox_inches='tight')
		
		bv = cash[k][0:10*(Ndt+1)]+np.multiply(q[k][0:10*(Ndt+1)],sim[k][0:10*(Ndt+1)])
		
		plt.close('all')
		f,axarr = plt.subplots(4,sharex=True)
		for i in range(10):
			axarr[0].plot(tx,sim[k][i*(Ndt+1):(i+1)*(Ndt+1)],linewidth=1)
			axarr[1].plot(tx[0:Ndt],nu[k][i*(Ndt+1):(i+1)*(Ndt+1)-1],linewidth=1)
			axarr[2].plot(tx,q[k][i*(Ndt+1):(i+1)*(Ndt+1)],linewidth=1)
			axarr[3].plot(tx,bv[i*(Ndt+1):(i+1)*(Ndt+1)],linewidth=1)
		axarr[0].set_xlim([0,T])
		axarr[3].set_xlabel('time')
		axarr[0].set_ylabel('signal')
		axarr[1].set_ylabel('trading rate')
		axarr[2].set_ylabel('inventory')
		axarr[3].set_ylabel('book value')
		axarr[0].set_title('kappa = '+str(kappa[k]))
		plt.savefig('plots/sim'+str(kappa[k]).replace('.',''),bbox_inches='tight')

	rr = np.arange(4.0*n_kappa).reshape(n_kappa,4)
	for k in range(n_kappa):
		pl = cash[k][Ndt:n_sim*(Ndt+1):(Ndt+1)]
		rr[k,0] = np.median(pl)
		rr[k,1] = np.std(pl)
		rr[k,2] = len([i for i in pl if i<0])/len(pl)
		rr[k,3] = np.percentile(pl,10)

	plt.close('all')
	f = plt.figure()

	axarr = [f.add_subplot(311),f.add_subplot(312),f.add_subplot(313)]
	axarr[0].plot(rr[:,1],rr[:,0],'b.')
	axarr[0].set_xlabel('std.')
	axarr[0].set_xlim(0)
	axarr[0].set_ylabel('median')
	axarr[1].plot(rr[:,2],rr[:,0],'b.')
	axarr[1].set_xlim(0)
	axarr[1].set_xlabel('% loss')
	axarr[1].set_ylabel('median')
	axarr[2].plot(rr[:,3],rr[:,0],'b.')
	axarr[2].set_xlabel('10% worst')
	axarr[2].set_ylabel('median')	
	plt.tight_layout()
	plt.savefig('plots/rr.png',bbox_inches='tight')

	t.clean_trading()

	return kappa

#if __name__=='__main__': trading(0.5,0.4,1e-4,10000,1,1000,0.01,2e-4,0,10,1)
