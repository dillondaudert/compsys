#Main script for assignment
import os
from copy import deepcopy
from parse import read_example
from scheduler import simulate_processor

def main():
    unipath = os.path.join(os.path.dirname(__file__), "uni.ex")
    sched = read_example(unipath)

    uni_sched = ['FCFS', 'RR', 'SPN', 'SRT', 'HRRN']
    for stype in uni_sched:
        processes = deepcopy(sched.procs)
        simulate_processor(processes, stype, sched.quantum, sched.max_time)

    rapath = os.path.join(os.path.dirname(__file__), "ra.ex")
    sched2 = read_example(rapath)

    ra_sched = ['ED', 'EDUI', 'FCFS']
    for stype in ra_sched:
        processes = deepcopy(sched2.procs)
        simulate_processor(processes, stype, sched2.quantum, sched2.max_time)


    rppath = os.path.join(os.path.dirname(__file__), "rp.ex")
    sched3 = read_example(rppath)

    rp_sched = ['FP', 'ECED']
    for stype in rp_sched:
        processes = deepcopy(sched3.procs)
        simulate_processor(processes, stype, sched3.quantum, sched3.max_time)




if __name__ == "__main__":
    main()

