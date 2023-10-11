# Simple-Scheduler-Simulator

This program was made on receiving the homework assignment for the course ```CS232 Operating Systems```. The assignment is to write a simulator for a process scheduler using C language. Processes will be given to the program on the command line terminal, along with the scheduling algorithm to use, then the program will select the given processing algorithm to manage the processes with. A queue data structure has been made to store the processes based on time of arrival, and this queue will be passed as an argument to the scheduler along with the system time (by default it is always 0 thus a scheduler always starts from 0 system time).

The supported scheduling algorithms (basic) are:

<table>
    <tr>
        <th>FIFO - First In First Out</th>
        <th>SJF - Shortest Job First</th>
        <th>STCF - Shortest Time to Completion First</th>
        <th>RR - Round Robin</th>
    </tr>
</table>