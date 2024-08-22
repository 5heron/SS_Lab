#include <stdio.h>
#include <sys/types.h>
#define MAX 30

int queue[MAX], front = -1, rear = -1;

typedef struct process {
  pid_t pid;
  int arrival_time, burst_time, completion_time, turn_around_time, waiting_time,
      remaining_burst;
} process_t;

void enqueue(int item) {
  if (front == -1)
    front = 0;
  queue[++rear] = item;
}

int dequeue() {
  int id = -1;
  id = queue[front];
  if (front == rear)
    front = rear = -1;
  else
    front = front + 1;
  return id;
}

void sort_processes(process_t *processes, int num_of_processes) {
  for (int i = 0; i < num_of_processes - 1; i++) {
    int swap = 0;
    for (int j = 0; j < num_of_processes - i - 1; j++) {
      if (processes[j].arrival_time > processes[j + 1].arrival_time) {
        process_t temp = processes[j];
        processes[j] = processes[j + 1];
        processes[j + 1] = temp;
        swap = 1;
      }
    }
    if (!swap)
      return;
  }
}

void round_robin(process_t *processes, int num_of_processes, int time_quantum) {
  int cpu_time = 0;
  process_t idle, gantt[MAX];
  idle.burst_time = 0;

  int i = 0, preemp_proc = -1, g_index = 0;

  while (front > -1 || i < num_of_processes || (preemp_proc != -1 && processes[preemp_proc].remaining_burst != 0)) {
    // queue is not || all process are not allocated once.
    while (i < num_of_processes && processes[i].arrival_time <= cpu_time) {
      // arrived processes
      enqueue(i);
      i++;
    }
    //prempted process is added only after all the arrived process are enqueued
    //because order of enqueue matters here
    if (preemp_proc != -1 && processes[preemp_proc].remaining_burst != 0)
      // add the preempted process back to the ready queue
      enqueue(preemp_proc);
    if (front >= 0) {
      //queue is not empty
      preemp_proc = dequeue();
      //add process to gantt
      gantt[g_index] = processes[preemp_proc];
      int temp = ((time_quantum < gantt[g_index].remaining_burst) ? time_quantum : gantt[g_index].remaining_burst);
      //update everything
      cpu_time += temp;
      processes[preemp_proc].remaining_burst -= temp;
      gantt[g_index++].completion_time = cpu_time;
      processes[preemp_proc].completion_time = cpu_time;
    } else {
      //if no process in queue then add idle to gantt
      gantt[g_index] = idle;
      //update everything
      gantt[g_index++].completion_time = cpu_time = processes[i].arrival_time;
    }
  }
  double tat = 0, twt = 0;
  for (i = 0; i < num_of_processes; i++) {
    processes[i].turn_around_time =
        processes[i].completion_time - processes[i].arrival_time;
    processes[i].waiting_time =
        processes[i].turn_around_time - processes[i].burst_time;
    twt = twt + processes[i].waiting_time;
    tat = tat + processes[i].turn_around_time;
  }
  printf("Gantt chart:\n");
  for (i = 0; i < g_index; i++) {
    if (gantt[i].burst_time == 0)
      printf("| Idle   ");
    else
      printf("| p%d    ", gantt[i].pid);
  }
  printf("|\n");
  printf("0\t");
  for (i = 0; i < g_index; i++)
    printf("%2d\t", gantt[i].completion_time);
  printf("\nTable :\n");
  printf(" _________________________________\n");
  printf("|Process| AT | BT | CT | TT | WT |\n");
  printf("|--------------------------------|\n");
  for (i = 0; i < num_of_processes; i++) {
    printf("|p%d \t| %2d | %2d | %2d | %2d | %2d |\n", processes[i].pid,
           processes[i].arrival_time, processes[i].burst_time,
           processes[i].completion_time, processes[i].turn_around_time,
           processes[i].waiting_time);
  }
  printf(" _________________________________\n");
  twt = twt / num_of_processes;
  tat = tat / num_of_processes;
  printf("\nAverage WT:%f\n", twt);
  printf("Average TT:%f\n", tat);
}

int main() {
  int num_of_processes, time_quantum;
  process_t processes[MAX];
  printf("Enter the number of processes: ");
  scanf("%d", &num_of_processes);
  printf("Enter the Time quantum: ");
  scanf("%d", &time_quantum);
  for (int i = 0; i < num_of_processes; i++) {
    printf("Enter Arrival time and Burst Time of process %d: \n", i + 1);
    scanf("%d%d", &processes[i].arrival_time, &processes[i].burst_time);
    processes[i].pid = i + 1;
    // rbt = bt
    processes[i].remaining_burst = processes[i].burst_time;
  }

  // sort process based on arrival time
  sort_processes(processes, num_of_processes);

  round_robin(processes, num_of_processes, time_quantum);
  return 0;
}

// 5 2 0 5 1 3 2 1 3 2 4 3
