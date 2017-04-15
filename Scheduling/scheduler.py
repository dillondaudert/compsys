#Scheduler simulator
from collections import deque

class Process():

    name = None
    arrival = None
    exec_time = None
    start_deadline = None
    finish_deadline = None
    remaining_time = None
    time_waited = None

    def __init__(self,
                 name,
                 arrival,
                 exec_time,
                 start_deadline = None,
                 finish_deadline = None):
        self.name = name
        self.arrival = arrival
        self.exec_time = self.remaining_time = exec_time
        self.start_deadline = start_deadline
        self.finish_deadline = finish_deadline
        self.time_waited = 0

    def execute(self):
        self.time_waited = 0
        self.remaining_time -= 1

    def priority(self, sched):
        """Return the priortiy of this process based on the given scheduler"""
        #Shortest Process Next
        if sched == "SPN":
            return self.exec_time
        elif sched == "SRT":
            return self.remaining_time
        elif sched == "HRRN":
            priority = (self.time_waited + self.exec_time) / self.exec_time
            return priority
        elif sched == "EDUI":
            return self.start_deadline
        elif sched == "ED":
            return self.start_deadline
        else:
            return None


    def wait(self):
        self.time_waited += 1

    def __str__(self):
        return "%s" % (self.name)

class PeriodicProcess(Process):

    count = 0
    period = None
    prior = None

    def __init__(self,
                 name,
                 arrival,
                 exec_time,
                 deadline,
                 period,
                 count,
                 priority):
        Process.__init__(self, name, arrival, exec_time, finish_deadline=deadline)
        self.period = period
        self.count = count
        self.prior = priority

    def priority(self, sched):
        if sched == "FP":
            return self.prior
        elif sched == "ECED":
            return self.finish_deadline
        else:
            return None


    def __str__(self):
        return "%s(%d)" % (self.name, self.count)

def enqueue(process,
            queue,
            sched):
    """Add a process to the queue (a list) according to what kind of
    scheduling is being done."""

    queue.append(process)
    if process.priority(sched) is not None:
        queue.sort(key=lambda x: x.priority(sched), reverse= (sched == "HRRN"))

    return queue

def simulate_processor(processes,
                       sched,
                       quantum=None,
                       max_time=None):
    """Simulate the execution of processes
    according to the scheduling algorithm specified."""

    print("%s:" % sched)

    wait_queue = processes
    ready_queue = list()
    finished_tasks = list()
    current_task = None
    time = 0
    log_start = 0

    if sched == "EDUI":
        #If we are doing EDUI, prioritize based on start deadline
        wait_queue.sort(key=lambda x: x.start_deadline)

    if sched in ["FCFS", "SPN", "HRRN", "ED", "EDUI"]:
        check_preempt = False
    else:
        check_preempt = True


    #Begin processor execution loop
    while len(wait_queue) != 0 or len(ready_queue) != 0 or current_task is not None:

        #Ready processes
        for wait_proc in wait_queue:
            if wait_proc.arrival <= time:
                #Add process to (priority) queue
                enqueue(wait_proc, ready_queue, sched)
                wait_queue.remove(wait_proc)
            else:
                #If we don't add a certain process, don't add any that follow it
                #This is mostly for EDUI, where the wait queue isn't sorted by
                #arrival time.
                break
        """
        print("Time: %d" % time)
        print("Wait Queue:")
        print(",".join([str(p) for p in wait_queue]))

        print("Ready Queue:")
        print(",".join([str(p) for p in ready_queue]))

        print("Executing:")
        print(current_task)
        print("--")
        """

        #Determine which process to execute, and execute
        if len(ready_queue) == 0:
            if current_task is not None:
                current_task.execute()
        else:

            if current_task is None:
                #If there is no current task and there is a ready task
                current_task = ready_queue.pop(0)
                log_start = time
            else:
                #Else check if we are pre-empting
                if check_preempt:
                    preempt = False
                    if sched == "RR":
                        if (time - log_start) % quantum == 0:
                            preempt = True
                    elif ready_queue[0].priority(sched) < current_task.priority(sched):
                        preempt = True
                    else:
                        preempt = False

                    if preempt:

                        #Print log
                        print("%s:%d->%d" % (current_task, log_start, time))

                        #preempt the current task
                        swap_task = current_task
                        current_task = ready_queue.pop(0)
                        enqueue(swap_task, ready_queue, sched)

                        log_start = time


            current_task.execute()

            for task in ready_queue:
                task.wait()
            if current_task.priority(sched) is not None:
                ready_queue.sort(key=lambda x: x.priority(sched), reverse=(sched=="HRRN"))

        time += 1

        #Check if task is finished
        if current_task is not None:
            if current_task.remaining_time == 0:
                print("%s:%d->%d" % (current_task, log_start, time))
                finished_tasks.append(current_task)
                current_task = None

                #Check if a deadline has passed
                for task in ready_queue:
                    if task.start_deadline is not None:
                        if time > task.start_deadline:
                            print("%s:Missed" % (task))
                            ready_queue.remove(task)
                    elif task.finish_deadline is not None:
                        if time > task.finish_deadline - task.remaining_time:
                            print("%s:Missed" % (task))
                            ready_queue.remove(task)
    print()



