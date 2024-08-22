#include<stdio.h>
#include<unistd.h>
#define MAX 10
struct process
{
	int arrival_time,burst_time,completion_time,pid,turn_around_time,waiting_time;
};
void main()
{
	int num_process;
	struct process processes[MAX];
	//Read the input
	printf("Enter the number of processes :");
	scanf("%d",&num_process);
	for(int i=0;i<num_process;i++){
		printf("Enter Arrival time and Burst Time of process %d : \n",i+1);
		scanf("%d%d",&processes[i].arrival_time,&processes[i].burst_time);
		processes[i].pid=i+1;
	}
	//sort based on arrival time
    for (int i = 0; i < num_process - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < num_process - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                struct process temp = processes[j];
				processes[j] = processes[j+1];
				processes[j+1] = temp;
                swapped = 1;
            }
        }
        // If no two elements were swapped
        // by inner loop, then break
        if (swapped == 0)
            break;
    }
	struct process gant[MAX],idle;
	//idle has no burst time 
	idle.burst_time=0;
	//traverse through the sorted processes
	int gant_index = 0;
	for(int i=0,cpu_time=0;i<num_process;i++){
		//if current time < arrival time i.e process has not arrived yet
		if(cpu_time<processes[i].arrival_time){
			//add idle to gantt chart
			gant[gant_index] = idle;
			//idle completes when process arrives
			//update cpu time and completion time for idle in gantt chart
			cpu_time = processes[i].arrival_time;
    		gant[gant_index++].completion_time = cpu_time;
		}
		//New process has now arrived
		//update cpu time and completion time for process   
    	cpu_time += processes[i].burst_time;
    	processes[i].completion_time = cpu_time;
		//add in gantt chart
    	gant[gant_index++] = processes[i];
	}
	float total_tat=0,total_wt=0;
	//calculate tat,wt for all process
	for(int i=0;i<num_process;i++){
		//tat = ct - at
		//wt = tat - bt
		processes[i].turn_around_time=processes[i].completion_time-processes[i].arrival_time;
		processes[i].waiting_time=processes[i].turn_around_time-processes[i].burst_time;
		//calculate total tat,wt to find average
		total_wt=total_wt+processes[i].waiting_time;
		total_tat=total_tat+processes[i].turn_around_time;
	}
	//Display everything
	printf("Gantt chart:\n");
    for(int i=0;i<gant_index;i++){
		if(gant[i].burst_time==0)
			printf("| Idle   ");
		else
			printf("| P%d    ",gant[i].pid);
	}
	printf("|\n");
	printf("0\t");  
	for(int i=0;i<gant_index;i++)
		printf("%2d\t",gant[i].completion_time);
	printf("\nTable :\n");
	printf(" _________________________________\n");
	printf("|Process| AT | BT | CT | TT | WT |\n");
	printf("|--------------------------------|\n");
	for(int i=0;i<num_process;i++){
		printf("|P%d \t| %2d | %2d | %2d | %2d | %2d |\n",processes[i].pid,processes[i].arrival_time,processes[i].burst_time,processes[i].completion_time,processes[i].turn_around_time,processes[i].waiting_time);
	}
	printf(" _________________________________\n");
	//Calculate average of tat,wt;
	total_wt=total_wt/num_process;
	total_tat=total_tat/num_process;
	printf("\nAverage WT:%f\n",total_wt);	
	printf("Average TT:%f\n",total_tat);		
}