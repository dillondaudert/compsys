#Scheduler simulator

class Process():

    name = None
    arrival = None
    exec_time = None
    start_deadline = None

    def __init__(self,
                 name,
                 arrival,
                 exec_time,
                 start_deadline = None):
        self.name = name
        self.arrival = arrival
        self.exec_time = exec_time
        self.start_deadline = start_deadline

    def __str__(self):
        return "Process %s, %d, %d" % (self.name, self.arrival, self.exec_time)

class PeriodicProcess(Process):

    count = 0
    period = None

    def __init__(self,
                 name,
                 arrival,
                 exec_time,
                 period):
        Process.__init__(self, name, arrival, exec_time)
        self.period = period
        PeriodicProcess.count += 1


def priority(process,
             sched):
    """Calculate the priority of a process
    according to the scheduling algorithm."""
    return


def simulate_processor(processes,
                       sched,
                       quantum):
    """Simulate the execution of processes
    according to the scheduling algorithm specified."""
