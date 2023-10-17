#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

// process control block (PCB)
struct pcb{                     // stores info on a process
    unsigned int pid;           // pid
    char pname[20];             // pname
    unsigned int ptimeleft;     // time left to complete
    unsigned int ptimearrival;  // time of arrival
    bool isfirsttime;           // flag to check if process has run for the first time
    unsigned int pfirstruntime; // time at which process ran for the first time
}; typedef struct pcb pcb;

// queue node
struct dlq_node{           // doubly linked queue node
    struct dlq_node *pfwd; // pointer to next node
    struct dlq_node *pbck; // pointer to previous node
    struct pcb *data;      // pointer to data which is a pcb struct
}; typedef struct dlq_node dlq_node;

// queue
struct dlq{                // doubly linked queue
    struct dlq_node *head; // pointer to head of queue
    struct dlq_node *tail; // pointer to tail of queue
}; typedef struct dlq dlq;

// function to add a pcb to a new queue node -> creates a new doubly linked queue node and initialiezs its data to ndata - a pointer to a pcb struct
dlq_node *get_new_node(pcb *ndata){ // ndata is a pointer to a pcb struct
    if(!ndata) return NULL; // if ndata is null, return null

    dlq_node *new = malloc(sizeof(dlq_node)); // allocate memory for a new node
    if(!new){ // if new is null, print error and exit
        fprintf(stderr, "Error: allocating memory\n");
        exit(1);
    }
    // set the pointers to null and set the data to ndata
    new->pfwd = new->pbck = NULL;
    new->data = ndata;
    return new;
}

// function to add a node to the tail of queue
void add_to_tail(dlq *q, dlq_node *new){
    if(!new) return; // if new is null, return

    if(q->head == NULL){ // if queue is empty, set head and tail to the new node
        if(q->tail != NULL){ // if tail is not null, print error and exit since queue is inconsistent
            fprintf(stderr, "DLList inconsitent.\n");
            exit(1);
        }
        q->head = new; q->tail = q->head;
    }
    else{ // if queue is not empty, set the new node to the tail and set the tail to the new node
        new->pfwd = q->tail;
        new->pbck = NULL;
        new->pfwd->pbck = new;
        q->tail = new;
    }
}

// function to remove a node from the head of queue
dlq_node *remove_from_head(dlq *const q){
    if(q->head == NULL){ // if queue is empty, return null
        if(q->tail != NULL){
            fprintf(stderr, "DLList inconsitent.\n");
            exit(1);
        } // if tail is not null, print error and exit since queue is inconsistent
        return NULL;
    } else if(q->head == q->tail){ // if queue has only one node
        if(q->head->pbck != NULL || q->tail->pfwd != NULL){ // if head's previous or tail's next is not null, print error and exit since queue is inconsistent
            fprintf(stderr, "DLList inconsitent.\n");
            exit(1);
        }
        // set head and tail to null and return the node
        dlq_node *p = q->head;
        q->head = NULL; q->tail = NULL;

        p->pfwd = p->pbck = NULL;
        return p;
    } else{ // set the head to the next node and return the node
        dlq_node *p = q->head;
        q->head = q->head->pbck;
        q->head->pfwd = NULL;

        p->pfwd = p->pbck = NULL;
        return p;
    }
}

// function to print our queue
void print_q(const dlq *q){
    dlq_node *n = q->head;
    if(n == NULL)return;
    while(n){
        printf("%s(%d),", n->data->pname, n->data->ptimeleft);
        n = n->pbck;
    }
}

// function to check if the queue is empty
int is_empty(const dlq *q){
    if (q->head == NULL && q->tail == NULL) return 1;
    else if (q->head != NULL && q->tail != NULL) return 0;
    else{
        fprintf(stderr, "Error: DLL queue is inconsistent.");
        exit(1);
    }
}

// function to sort the queue on completion time
void sort_by_timetocompletion(const dlq *q){
    // bubble sort
    dlq_node *start = q->tail;
    dlq_node *end = q->head;

    while(start != end){ // while start and end are not equal
        dlq_node *node = start;      // set node to start
        dlq_node *next = node->pfwd; // set next to node's forward

        while(next != NULL){ // while next is not null
            if (node->data->ptimeleft < next->data->ptimeleft){ // if node's time left is less than next's time left, do a swap
                pcb *temp = node->data;
                node->data = next->data;
                next->data = temp;
            }
            node = next; // set node to next and next to node's forward
            next = node->pfwd;
        }
        end = end->pbck; // set end to end's backward
    }
}

// function to sort the queue on arrival time
void sort_by_arrival_time(const dlq *q){
    // bubble sort
    dlq_node *start = q->tail;
    dlq_node *end = q->head;

    while(start != end){
        dlq_node *node = start;
        dlq_node *next = node->pfwd;

        while(next != NULL){
            if (node->data->ptimearrival < next->data->ptimearrival){
                // do a swap
                pcb *temp = node->data;
                node->data = next->data;
                next->data = temp;
            }
            node = next;
            next = node->pfwd;
        }
        end = end->pbck;
    }
}

// function to tokenize the one row of data -> parses a line of input data and returns a pointer to a pcb struct from it
pcb *tokenize_pdata(char *buf){ // buf is a pointer to the line of input data containing the process data in the format pname:pid:duration:arrival time
    pcb *p = (pcb *)malloc(sizeof(pcb)); // allocate memory for a pcb struct
    if(!p){
        fprintf(stderr, "Error: allocating memory.\n");
        exit(1);
    }

    char *token = strtok(buf, ":\n"); // tokenize the line of input data
    if(!token){
        fprintf(stderr, "Error: Expecting token pname\n");
        exit(1);
    }
    strcpy(p->pname, token); // copy the token to pname

    token = strtok(NULL, ":\n"); // tokenize the line of input data
    if(!token){
        fprintf(stderr, "Error: Expecting token pid\n");
        exit(1);
    }
    p->pid = atoi(token); // convert the token to an integer and set it to pid

    token = strtok(NULL, ":\n");
    if(!token){
        fprintf(stderr, "Error: Expecting token duration\n");
        exit(1);
    }
    p->ptimeleft = atoi(token); // convert the token to an integer and set it to ptimeleft

    token = strtok(NULL, ":\n");
    if(!token){
        fprintf(stderr, "Error: Expecting token arrival time\n");
        exit(1);
    }
    p->ptimearrival = atoi(token); // convert the token to an integer and set it to ptimearrival

    p->isfirsttime = true; // set isfirsttime to true

    token = strtok(NULL, ":\n");
    if(token){
        fprintf(stderr, "Error: Oh, what've you got at the end of the line?\n");
        exit(1);
    }
    return p;
}

// implement the FIFO scheduling code
void sched_FIFO(dlq *const p_fq, int *p_time){
    // initialize a queue to manage processes that are ready to run
    dlq queue; queue.head = queue.tail = NULL;
    // initialize the first process node from the head of the queue
    dlq_node *process = remove_from_head(p_fq);

    // initialize performance metrics
    int num_processes = 1, rt1 = 0, response_time = 0, turnaround_time = 0, first_arrival = process->data->ptimearrival;
    while(1){
        (*p_time)++; // increment the system time
        // if there are no processes left to run and the queue and ready queue is empty, break the loop
        if(!(process->data->ptimeleft) && is_empty(&queue) && is_empty(p_fq)) break;
        
        // if there are processes left to run and the head of the queue has an arrival time less than the system time, add the head of the queue to the ready queue
        if(!is_empty(p_fq) && p_fq->head->data->ptimearrival < (*p_time)){
            add_to_tail(&queue, remove_from_head(p_fq)); num_processes++;
        }

        if(process->data->isfirsttime == true){ // if the process has not run before, set its first runtime to the system time and set isfirsttime to false
            process->data->isfirsttime = false;
            process->data->pfirstruntime = (*p_time);
            rt1 = (*p_time) - process->data->ptimearrival;
            if (rt1 < 0) rt1 = 1;
            response_time += rt1;
        }

        printf("%d:", (*p_time));
        // if the process still has to arrive, print idle, else decrement its time and print its name
        if(process->data->ptimearrival >= (*p_time)) printf("idle:");
        else{ process->data->ptimeleft--; printf("%s:", process->data->pname); }
        // if the queue is empty, print empty, else show contents of the queue
        if(is_empty(&queue)) printf("empty:\n");
        else{ print_q(&queue); printf(":\n");}

        // if process has finished, calculate the turnaround time and add it to the total turnaround time, then remove another process from the queue if queue is not empty
        if(process->data->ptimeleft == 0){
            turnaround_time += (*p_time) - process->data->ptimearrival;
            if (!is_empty(&queue)) process = remove_from_head(&queue);
        }
    } free(process);
    float throughput = (float)num_processes / ((*p_time) - first_arrival);
    float avg_turnaround_time = (float)turnaround_time / num_processes;
    float avg_response_time = (float)response_time / num_processes;
    printf("Throughput: %.3f\n", throughput);
    printf("Average turnaround time: %.3f\n", avg_turnaround_time);
    printf("Average response time: %.3f\n", avg_response_time);
    return;
}

// implement the SJF scheduling code
void sched_SJF(dlq *const p_fq, int *p_time){
    dlq queue; queue.head = queue.tail = NULL;
    dlq_node *process = remove_from_head(p_fq);

    int num_processes = 1, rt1 = 0, response_time = 0, turnaround_time = 0, first_arrival = process->data->ptimearrival;

    while(1){
        (*p_time)++;

        if(!(process->data->ptimeleft) && is_empty(&queue) && is_empty(p_fq)) break;
        // if processes left with arrival time less than system time, add them to the tail of the queue, and sort by time to completion
        if(!is_empty(p_fq) && p_fq->head->data->ptimearrival < (*p_time)){
            add_to_tail(&queue, remove_from_head(p_fq));
            num_processes++;
            sort_by_timetocompletion(&queue);
        }

        if(process->data->isfirsttime == true){
            process->data->isfirsttime = false;
            process->data->pfirstruntime = (*p_time);
            rt1 = (*p_time) - process->data->ptimearrival;
            if (rt1 < 0) rt1 = 1;
            response_time += rt1;
        }

        printf("%d:", *p_time);
        // if process still has to arrive, print idle, else decrement its time and print its name
        if(process->data->ptimearrival >= (*p_time)) printf("idle:");
        else{
            process->data->ptimeleft--;
            printf("%s:", process->data->pname);
        }
        // if queue is empty, print empty, else show contents of the queue
        if(is_empty(&queue)) printf("empty:\n");
        else{
            print_q(&queue);
            printf(":\n");
        }

        if(process->data->ptimeleft == 0){
            turnaround_time += (*p_time) - process->data->ptimearrival;
            if(!is_empty(&queue)) process = remove_from_head(&queue);
        }
    }
    free(process);
    float throughput = (float)num_processes / ((*p_time) - first_arrival);
    float avg_turnaround_time = (float)turnaround_time / num_processes;
    float avg_response_time = (float)response_time / num_processes;
    printf("Throughput: %.3f\n", throughput);
    printf("Average turnaround time: %.3f\n", avg_turnaround_time);
    printf("Average response time: %.3f\n", avg_response_time);
    return;
}

// implement the RR scheduling code
void sched_RR(dlq *const p_fq, int *p_time){
    dlq queue; queue.head = queue.tail = NULL;
    dlq_node *process = remove_from_head(p_fq);
    int quantum_time = 1, process_time = 0; // quantum is the time slice for each process, process_time is the time the process has been running

    int num_processes = 1, rt1 = 0, response_time = 0, turnaround_time = 0, first_arrival = process->data->ptimearrival;

    while(1){
        (*p_time)++;
        if (!(process->data->ptimeleft) && is_empty(&queue) && is_empty(p_fq)) break;

        if(!is_empty(p_fq) && p_fq->head->data->ptimearrival < (*p_time)){
            add_to_tail(&queue, remove_from_head(p_fq));
            num_processes++;
        }

        if(process->data->isfirsttime == true){
            process->data->isfirsttime = false;
            process->data->pfirstruntime = (*p_time);
            rt1 = (*p_time) - process->data->ptimearrival;
            if (rt1 < 0) rt1 = 1;
            response_time += rt1;
        }

        printf("%d:", *p_time);
        if(process->data->ptimearrival >= (*p_time)) printf("idle:");
        else{ // if process has arrived, decrement its time, update the time it has run for, and print its name
            process->data->ptimeleft--;
            process_time++;
            printf("%s:", process->data->pname);
        }

        if(is_empty(&queue)) printf("empty:\n");
        else{
            print_q(&queue);
            printf(":\n");
        }

        if(process->data->ptimeleft == 0){
            turnaround_time += (*p_time) - process->data->ptimearrival;
            if (!is_empty(&queue)) process = remove_from_head(&queue);
        }

        if(process_time == quantum_time){ // if process has run for the time slice, add it to the tail of the queue and set process_time to 0
            add_to_tail(&queue, process);
            process = remove_from_head(&queue);
            process_time = 0;
        }
    }
    free(process);
    float throughput = (float)num_processes / ((*p_time) - first_arrival);
    float avg_turnaround_time = (float)turnaround_time / num_processes;
    float avg_response_time = (float)response_time / num_processes;
    printf("Throughput: %.3f\n", throughput);
    printf("Average turnaround time: %.3f\n", avg_turnaround_time);
    printf("Average response time: %.3f\n", avg_response_time);
    return;
}

// implement the STCF scheduling code
void sched_STCF(dlq *const p_fq, int *p_time){
    dlq queue; queue.head = queue.tail = NULL;
    dlq_node *process = remove_from_head(p_fq);

    int num_processes = 1, rt1 = 0, response_time = 0, turnaround_time = 0, first_arrival = process->data->ptimearrival;

    while(1){
        (*p_time)++;
        if(!(process->data->ptimeleft) && is_empty(&queue) && is_empty(p_fq)) break;

        if(!is_empty(p_fq) && p_fq->head->data->ptimearrival < (*p_time)){
            add_to_tail(&queue, remove_from_head(p_fq));
            num_processes++;
        }

        if(process->data->isfirsttime == true){
            process->data->isfirsttime = false;
            process->data->pfirstruntime = (*p_time);
            rt1 = (*p_time) - process->data->ptimearrival;
            if (rt1 < 0) rt1 = 1;
            response_time += rt1;
        }

        sort_by_timetocompletion(&queue); // sort the queue by time to completion

        printf("%d:", *p_time);
        if(process->data->ptimearrival >= (*p_time)) printf("idle:");
        else{
            if(!is_empty(&queue) && (process->data->ptimeleft > queue.head->data->ptimeleft)){
                add_to_tail(&queue, process);
                process = remove_from_head(&queue);
            }
            if(process->data->isfirsttime == true){
                process->data->isfirsttime = false;
                process->data->pfirstruntime = (*p_time);
                rt1 = (*p_time) - process->data->ptimearrival;
                if (rt1 < 0) rt1 = 0;
                response_time += rt1;
            }
            process->data->ptimeleft--;
            printf("%s:", process->data->pname);
        }

        if(is_empty(&queue)) printf("empty:\n");
        else{
            sort_by_timetocompletion(&queue);
            print_q(&queue);
            printf(":\n");
        }

        if(process->data->ptimeleft == 0){
            turnaround_time += (*p_time) - process->data->ptimearrival;
            if(!is_empty(&queue)) process = remove_from_head(&queue);
        }
    }
    free(process);
    float throughput = (float)num_processes / ((*p_time) - first_arrival);
    float avg_turnaround_time = (float)turnaround_time / num_processes;
    float avg_response_time = (float)response_time / num_processes;
    printf("Throughput: %.3f\n", throughput);
    printf("Average turnaround time: %.3f\n", avg_turnaround_time);
    printf("Average response time: %.3f\n", avg_response_time);
    return;
}

int main(){
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int N = 0;                 // number of processes
    char tech[20] = {'\0'};    // scheduling policy
    char buffer[100] = {'\0'}; // buffer to store the input data
    scanf("%d", &N);           // read the number of processes
    scanf("%s", tech);         // read the scheduling policy

    dlq queue;         // create a queue
    queue.head = NULL; // set the head and tail to null
    queue.tail = NULL;
    for (int i = 0; i < N; ++i){ // for each process, read the data, tokenize it, and add it to the queue
        scanf("%s\n", buffer);
        pcb *p = tokenize_pdata(buffer);
        add_to_tail(&queue, get_new_node(p));
    }

    unsigned int system_time = 0;
    sort_by_arrival_time(&queue);

    // run scheduler
    if (!strncmp(tech, "FIFO", 4)) sched_FIFO(&queue, &system_time);
    else if (!strncmp(tech, "SJF", 3)) sched_SJF(&queue, &system_time);
    else if (!strncmp(tech, "STCF", 4)) sched_STCF(&queue, &system_time);
    else if (!strncmp(tech, "RR", 2)) sched_RR(&queue, &system_time);
    else fprintf(stderr, "Error: unknown POLICY\n");
    return 0;
}