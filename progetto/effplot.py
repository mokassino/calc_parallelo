import matplotlib.pyplot as plot
import numpy as np

def plot_properties(pos,gfunc):

    plot.subplot(2,1,pos)
    plot.xlabel('p - Numero processori')

    if gfunc.__name__ == "tau":
        plot.ylabel('Numero di operazioni al variare di p')
        plot.suptitle("Grafico dei tempi")
    elif gfunc.__name__ == "speedup":
        plot.ylabel('Speedup al variare di p')
        plot.suptitle("Grafico dello Speedup")
    elif gfunc.__name__ == "efficiency":
        plot.ylabel('Efficienza al variare di p')
        plot.suptitle("Grafico dell'Efficienza")
        


def iter_subplot(y,x, pos, gfunc):
        if gfunc.__name__== "speedup":
            plot_properties(pos,gfunc)
            plot.plot(x,x,label="Speedup ideale")
        elif gfunc.__name__== "efficiency":
            plot_properties(pos,gfunc)
            plot.plot(x,x/x,label="Efficienza ideale")

        for i in range(len(y)):
            plot_properties(pos,gfunc)
            plot.plot(x,gfunc(x,y[i]),label="N = " + str(y[i]) )
            plot.legend()
        

def efficiency(p, N):
    print("N: " + str(N) + "res: " +str(speedup(p,N)/p))
    return speedup(p, N)/p


def speedup(p, N): #Speedup

    return (2*N)/tau(p, N)

def tau(p, N): #Time graphs

    tau = np.zeros(len(p))

    tau[0] = 2*N
    for i in range(0,len(p)):
        tau[i] = float((2*N) /p[i])
        if N % p[i] != 0:
            tau[i] = tau[i] + 3
        else:
            tau[i] = tau[i] + 2

    tau[0] = 2*N
    print(tau)
    return tau

def main():
    x = np.array([1,2,3,4,5,6,7,8,16,32])
    yl = np.array([10,50,100,250,500])
    yh = np.array([1000,10000,100000,1000000]) #,5000000

    plot.gcf()
    plot.gca()


    plot.figure(1)
    iter_subplot(yl, x, 1, tau)
    iter_subplot(yh, x, 2, tau)


    plot.figure(2)
    iter_subplot(yl, x, 1, speedup)
    iter_subplot(yh, x, 2, speedup)

    plot.figure(3)
    iter_subplot(yl, x, 1, efficiency)
    iter_subplot(yh, x, 2, efficiency)


    plot.show()

    
if __name__=="__main__":
    main()