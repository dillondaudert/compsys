#Read the scheduling example inputs
import csv
import collections
from scheduler import Process, PeriodicProcess

Scheduler = collections.namedtuple('Scheduler', ['stype', 'procs', 'quantum', 'max_time'])

def read_example(path):
    """Return a list of processes and the
    scheduler details."""

    processes = list()
    quantum = None
    max_time = None

    with open(path, 'r') as csvfile:
        reader = csv.reader(csvfile)
        header = next(reader)
        if header[0] == "U":
            stype = "uni"
            n_procs = int(header[1])
            quantum = int(header[2])
        elif header[0] == "RA":
            stype = "ra"
            n_procs = int(header[1])
        else:
            stype = "rp"
            n_procs = int(header[1])
            max_time = int(header[2])
        for row, proc in enumerate(reader):
            if stype == "uni":
                processes.append(Process(proc[0], int(proc[1]), int(proc[2])))
            elif stype == "ra":
                processes.append(Process(proc[0], int(proc[1]), int(proc[2]),
                                         int(proc[3])))
            else:
                #Periodic, create periodic processes
                p_period = int(proc[3])
                p_num = max_time // p_period
                for i in range(0,p_num):
                    arrival = int(proc[1]) + (i*p_period)
                    processes.append(PeriodicProcess(name=proc[0],
                                                     arrival=arrival,
                                                     exec_time=int(proc[2]),
                                                     deadline=arrival+p_period,
                                                     period=p_period,
                                                     count=i+1,
                                                     priority=row))
        #Sort the process list by arrival time
        processes.sort(key=lambda x: x.arrival)


    return Scheduler(stype=stype, procs=processes, quantum=quantum, max_time=max_time)
