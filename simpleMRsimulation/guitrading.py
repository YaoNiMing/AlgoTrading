from appJar import gui
from trading import trading

trader = gui("trader","1250x700")

def press(btn):
	theta = trader.getEntry("theta")
	sigma = trader.getEntry("sigma")
	c = trader.getEntry("c")
	n_sim = trader.getEntry("n_sim")
	T = trader.getEntry("T")
	Ndt = trader.getEntry("n_dt")
	alpha = trader.getEntry("alpha")
	phi = trader.getEntry("phi")
	kpmin = trader.getEntry("kpmin")
	kpmax = trader.getEntry("kpmax")
	dkp = trader.getEntry("dkp")

	global kappastr
	kappastr = trading(theta,sigma,c,int(n_sim),T,int(Ndt),alpha,phi,kpmin,kpmax,dkp)
	trader.enableRadioButton("plottype")
	trader.reloadImage("plot","plots/rr.png")

def default(btn):
	trader.setEntry("theta",0.5)
	trader.setEntry("sigma",0.4)
	trader.setEntry("c",0.0001)
	trader.setEntry("alpha",0.01)
	trader.setEntry("phi",0.0002)
	trader.setEntry("n_sim",10000)
	trader.setEntry("T",1)
	trader.setEntry("n_dt",1000)
	trader.setEntry("kpmin",0)
	trader.setEntry("kpmax",10)
	trader.setEntry("dkp",1)
	

def toggleplot(radio):
	whichplot = trader.getRadioButton(radio)
	if whichplot=="risk return measure":
		trader.disableEntry("kappa")
		trader.disableButton("update")
		trader.reloadImage("plot","plots/rr.png")
	elif whichplot=="simulation result":
		trader.enableEntry("kappa")
		trader.enableButton("update")
		trader.reloadImage("plot","plots/sim"+str(max(kappastr)).replace('.','')+".png")

	else:
		trader.enableEntry("kappa")
		trader.enableButton("update")
		trader.reloadImage("plot","plots/pl"+str(max(kappastr)).replace('.','')+".png")

def update(btn):
	kappaupd = trader.getEntry("kappa")
	kappaupd = min(kappastr, key=lambda x:abs(x-kappaupd))
	pltupd = trader.getRadioButton("plottype")
	if pltupd == "simulation result":
		trader.reloadImage("plot","plots/sim"+str(kappaupd).replace('.','')+".png")
	else:
		trader.reloadImage("plot","plots/pl"+str(kappaupd).replace('.','')+".png")

trader.startPanedFrame("p1")
trader.addLabel("title", "Simulation Algo Trader",0,0,2)
trader.addHorizontalSeparator(3,0,4)
trader.addLabel("mktparam","Market Parameters",4,0,2)
trader.addLabel("theta","theta",5,0,1)
trader.addNumericEntry("theta",5,1,1)
trader.addLabel("sigma","sigma",6,0,1)
trader.addNumericEntry("sigma",6,1,1)
trader.addLabel("c","c",7,0,1)
trader.addNumericEntry("c",7,1,1)
#trader.setPadding("c",[10,10])
trader.addLabel("kpmin","kappa min",5,2,1)
trader.addNumericEntry("kpmin",5,3,1)
trader.addLabel("kpmax","kappa max",6,2,1)
trader.addNumericEntry("kpmax",6,3,1)
trader.addLabel("dkp","dkappa",7,2,1)
trader.addNumericEntry("dkp",7,3,1)
trader.addHorizontalSeparator(8,0,4)
trader.addLabel("mdlparam","Model Parameters",9,0,2)
trader.addLabel("alpha","alpha",10,0,1)
trader.addNumericEntry("alpha",10,1,1)
trader.addLabel("phi","phi",11,0,1)
trader.addNumericEntry("phi",11,1,1)
trader.addHorizontalSeparator(12,0,4)
trader.addLabel("simparam","Simulation Parameters",13,0,2)
trader.addLabel("T","T",14,0,1)
trader.addNumericEntry("T",14,1,1)
trader.addLabel("n_dt","n_dt",15,0,1)
trader.addNumericEntry("n_dt",15,1,1)
trader.addLabel("n_sim","n_sim",16,0,1)
trader.addNumericEntry("n_sim",16,1,1)
#trader.addButtons(["Simulate","Reset Default"], press, 16,0,4)
trader.addButton("Simulate",press,17,0,2)
trader.addButton("Restore Default",default,17,2,2)


trader.startPanedFrame("p2")
trader.addRadioButton("plottype","risk return measure",0,0,1)
trader.addRadioButton("plottype","simulation result",0,1,1)
trader.addRadioButton("plottype","P&L distribution",0,2,1)
trader.setRadioButtonFunction("plottype",toggleplot)
trader.addLabelNumericEntry("kappa",1,0,1)
trader.addButton("update",update,1,1,1)
trader.startLabelFrame("plot",2,0,3)
trader.setLabelFrameHeights("plot",600)
trader.addImage("plot","plots/logo.png")
trader.stopLabelFrame()
trader.stopPanedFrame()
trader.stopPanedFrame()

trader.disableRadioButton("plottype")
trader.disableEntry("kappa")
trader.disableButton("update")
default("restore default")
trader.go()
