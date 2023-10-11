# Simple-Scheduler-Simulator

This program was made on receiving the homework assignment for the course ```CS232 Operating Systems```. The assignment is to write a simulator for a process scheduler using C language. Processes will be given to the program on the command line terminal, along with the scheduling algorithm to use, then the program will select the given processing algorithm to manage the processes with. A queue data structure has been made to store the processes based on time of arrival, and this queue will be passed as an argument to the scheduler along with the system time (by default it is always 0 thus a scheduler always starts from 0 system time).

The code is contained in the [scheduler](scheduler.c) file.

The supported scheduling algorithms (basic) are:
<table>
    <tr>
        <td>FIFO - First In First Out</td>
        <td>SJF - Shortest Job First</td>
        <td>STCF - Shortest Time to Completion First</td>
        <td>RR - Round Robin</td>
    </tr>
</table>

### Running The Program
The accompanying ```Makefile``` has commands for compiling the ```sheduler.c``` file which contains the program of the scheduler and running the output.

To compile: 
```make build```

To run:
```make run```
followed by the input

### Input Format:
    Line 1: N -> Number of Processes to run
    Line 2: Scheduling Algorithm (FIFO or SJF or STCF or RR)
    Lines 3 till N + 3 contain the processes in the following format(colon included):
    Process Name(string):Process ID(int):Completion Time(int):Arrival Time(int)
    end

For further clarifications, the input format can be viewed in the given [report](report.pdf)

