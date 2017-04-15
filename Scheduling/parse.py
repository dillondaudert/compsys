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
            n_procs = header[1]
            quantum = header[2]
        elif header[0] == "RA":
            stype = "ra"
            n_procs = header[1]
        else:
            stype = "rp"
            n_procs = header[1]
            max_time = header[2]
        for proc in reader:
            if stype == "uni":
                processes.append(Process(proc[0], int(proc[1]), int(proc[2])))
            elif stype == "ra":
                processes.append(Process(proc[0], int(proc[1]), int(proc[2]),
                                         int(proc[3])))
            else:
                processes.append(PeriodicProcess(proc[0], int(proc[1]), int(proc[2]),
                                                 int(proc[3])))

    sched = Scheduler(stype=stype, procs=processes, quantum=quantum, max_time=max_time)

    return sched
