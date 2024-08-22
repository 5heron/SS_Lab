#include <stdio.h>
#include <stdlib.h>

#define MAX 20

int temp;
//Sort based on requests 
void sort(int *requests, int number_of_request) {
  for (int i = 0; i < number_of_request - 1; i++) {
    for (int j = 0; j < number_of_request - 1; j++) {
      if (requests[j] > requests[j + 1]) {
        temp = requests[j];
        requests[j] = requests[j + 1];
        requests[j + 1] = temp;
      }
    }
  }
}

void fcfs(int *requests, int number_of_request) {
  int i, total_head_movement = 0;
  //Print 1st traversed request
  printf("Sequence : %d ", requests[0]);
  //Go through traversed requests array
  //Number_of_request is less than actual as we didn't count initial_position  
  for (i = 0; i < number_of_request; i++) {
    //Print next request traversed
    printf("%d ", requests[i + 1]);
    //Calculate cylinders to be traversed by subtracting curent request cylinder and next request cylinder
    //Update total_head_movement 
    total_head_movement += abs(requests[i + 1] - requests[i]);
  }
  printf("\nTotal head movement = %d \n", total_head_movement);
}

void scan(int *copy_of_array, int number_of_request, int initial_position) {
  int choice, total_head_movement = 0, loc, i, traverse_index, disk_start, disk_end, inc,requests[MAX], traversed[MAX];
  for (i = 0; i < number_of_request +1; i++)
    requests[i]=copy_of_array[i];
  printf("Choose 1: Left or 2: Right :");
  scanf("%d", &choice);
  printf("Enter the Start and End of disk :");
  scanf("%d%d", &disk_start, &disk_end);
  //Add disk start and end to request array
  //Increment size of request array
  requests[++number_of_request] = disk_start, requests[++number_of_request] = disk_end;
  //sort requests
  sort(requests, ++number_of_request);
  //Find index of initial_position in request array
  for (i = 0; i < number_of_request && requests[i] != initial_position; i++);
  loc = i;
  //Initial chosen direction 
  //Increment(+1) or Decrement(-1) from initial_position 
  inc = (choice == 1) ? (-1) : 1;
  //MAIN LOGIC
  for (i = loc, traverse_index = 0; traverse_index < number_of_request - 1;) {
    //Go through all sorted requests starting from initial position
    traversed[traverse_index++] = requests[i];
    //Increment index in chosen direction
    i += inc;
    //When head reaches end of disk (0 or n-1 index)  
    if ((i < 0 || i > number_of_request - 1) && traverse_index < number_of_request - 1)
      //Reset index to next to initial_position to start traversal from that request to other end
      //Reset direction to opposite of initial chosen direction by changing increment
      inc *= (-1), i = loc + inc;
  }
  //Print 1st traversed request
  printf("Sequence : %d ", traversed[0]);
  //Go through traversed requests array
  for (i = 0; i < traverse_index - 1; i++) {
    //Print next request traversed
    printf("%d ", traversed[i + 1]);
    //Calculate cylinders to be traversed by subtracting curent request cylinder and next request cylinder
    //Update total_head_movement 
    total_head_movement += abs(traversed[i + 1] - traversed[i]);
  }
  printf("\nTotal head movement = %d \n", total_head_movement);
}

void cscan(int copy_of_array[], int number_of_request, int initial_position) {
  int choice, total_head_movement = 0, loc, i, traverse_index, disk_start, disk_end, inc,requests[MAX], traversed[MAX];
  for (i = 0; i < number_of_request + 1; i++)
    requests[i]=copy_of_array[i];
  printf("Choose 1: Left or 2: Right :");
  scanf("%d", &choice);
  printf("Enter the Start and End of disk :");
  scanf("%d%d", &disk_start, &disk_end);
  //Add disk start and end to request array
  //Increment size of request array
  requests[++number_of_request] = disk_start, requests[++number_of_request] = disk_end;
  //sort requests
  sort(requests, ++number_of_request);
  //Find index of initial_position in request array
  for (i = 0; i < number_of_request && requests[i] != initial_position; i++);
  loc = i;
  //Initial chosen direction 
  //Increment(+1) or Decrement(-1) from initial_position 
  inc = (choice == 1) ? (-1) : 1;
  //MAIN LOGIC
  for (i = loc, traverse_index = 0; traverse_index < number_of_request;) {
    //Go through all sorted requests starting from initial position
    traversed[traverse_index++] = requests[i];
    //Increment index in chosen direction
    i += inc;
    //When head reaches end of disk (0 or n-1 index)  
    if ((i < 0 || i > number_of_request - 1) && traverse_index < number_of_request - 1)
      //Reset index to other end to start traversal from that end
      i = (i < 0) ? (number_of_request - 1) : 0;
  }
  //Print 1st traversed request
  printf("Sequence : %d ", traversed[0]);
  //Go through traversed requests array
  for (i = 0; i < traverse_index - 1; i++) {
    //Print next request traversed
    printf("%d ", traversed[i + 1]);
    //Calculate cylinders to be traversed by subtracting curent request cylinder and next request cylinder
    //Update total_head_movement 
    total_head_movement += abs(traversed[i + 1] - traversed[i]);
  }
  printf("\nTotal head movement = %d \n", total_head_movement);
}

int main() {
  int i, number_of_request, requests[MAX], choice
  , initial_position;
  //Read the requests
  printf("Enter the number of requests :");
  scanf("%d", &number_of_request);
  printf("Enter the Requests :");
  for (i = 1; i < number_of_request + 1; i++)
    scanf("%d", &requests[i]);
  //Enter the initial position
  printf("Enter the initial position :");
  scanf("%d", &initial_position);
  requests[0] = initial_position;
  do {
    //Choose the scheduling 
    printf("Enter 1:FCFS 2:SCAN 3:CSCAN 4:Exit :\n");
    scanf("%d", &choice
    );
    switch (choice) {
      case 1:
        fcfs(requests, number_of_request);
        break;
      case 2:
        scan(requests, number_of_request, initial_position);
        break;
      case 3:
        cscan(requests, number_of_request, initial_position);
      case 4:
        break;
      default:
        printf("Invalid input\n");
    }
  } while (choice
   != 4);
  return 0;
}
