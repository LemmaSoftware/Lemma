import matplotlib.pyplot as plt 
import numpy as np 
from collections import defaultdict
import cpuinfo # pip install py-cpuinfo

light_grey = np.array([float(225)/float(255)]*3)

def fixLeg(legend):
    rect = legend.get_frame()
    #rect.set_color('None')
    rect.set_facecolor(light_grey)
    rect.set_linewidth(0.0)
    rect.set_alpha(1.0)

def deSpine(ax1):
    spines_to_remove = ['top', 'right']
    for spine in spines_to_remove:
        ax1.spines[spine].set_visible(False)
    #ax1.xaxis.set_ticks_position('none')
    #ax1.yaxis.set_ticks_position('none')
    ax1.get_xaxis().tick_bottom()
    ax1.get_yaxis().tick_left()

timings = np.recfromtxt(r"timings.csv", delimiter=r",", encoding=None)
BENCH = {} # defaultdict(str)

for i,line in enumerate(timings): 

    # check if compiler has been added already 
    if line[0].strip() in BENCH:
        pass
    else:
        BENCH[line[0].strip()] = {}
        print(line[1])
        BENCH[line[0].strip()]["version"] = line[1].strip()

    # check to see if Hankel has been added already 
    if line[4].strip() in BENCH[line[0].strip()]:
        pass
    else: 
        BENCH[line[0].strip()][line[4].strip()] = {}
        BENCH[line[0].strip()][line[4].strip()]["NP"] = []
        BENCH[line[0].strip()][line[4].strip()]["Time"] = []
        
    BENCH[line[0].strip()][line[4].strip()]["NP"].append( line[5] )
    BENCH[line[0].strip()][line[4].strip()]["Time"].append( line[6] )

#print (BENCH)
fig = plt.figure()

for compiler in BENCH:    
    print (compiler)
    for hankel in BENCH[compiler]:   
        if hankel != "version": 
            plt.plot(BENCH[compiler][hankel]["NP"], BENCH[compiler][hankel]["Time"], ".-", label=str(hankel))

plt.gca().set_yscale('log')    
plt.gca().set_title( str(compiler) + " " + str(BENCH[compiler]["version"]) ) 
#plt.suptitle( cpuinfo.get_cpu_info()['brand'] ) 



leg = plt.legend()

deSpine(plt.gca())
fixLeg(leg)

plt.gca().set_xlabel("OMP_NUM_THREADS")
plt.gca().set_ylabel("execution time (s)")

plt.savefig("timings.png")
plt.show()
