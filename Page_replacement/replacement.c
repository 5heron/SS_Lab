
//OPTIMAL NOT IN SYLLABUS 

#include <stdio.h>
#define MAX 100
int count[MAX], time[MAX], inframe[MAX], frame[MAX];
void fifo(int reference_array[], int no_of_references, int no_of_frames) {
  int no_of_miss = 0, i, no_of_allocatedframes = 0, frame_index = 0;
  for (i = 0; i < no_of_references; i++) {
    //If PAGEi referenced is not in a frame 
    if (inframe[reference_array[i]] == 0) {
      if (no_of_allocatedframes < no_of_frames)
        //If there are free frames allocate referenced PAGE in reference_array to free frame
        frame[no_of_allocatedframes++] = reference_array[i];
        //Increment allocated frames index (acts like rear, only grow till frames are full) 
      else {
        //If there are no free frames
        //Take out page at front/frame index
        inframe[frame[frame_index]] = 0;
        //Insert new page
        frame[frame_index] = reference_array[i];
        //Update front/frame_index
        frame_index = (frame_index + 1) % no_of_frames;
      }
      //Voila! PAGEi is now in a frame
      inframe[reference_array[i]] = 1;
      printf("Frames: ");
      //Display all the frames currently in frame
      for (int k = 0; k < no_of_allocatedframes; k++) {
      	printf("%d  ", frame[k]);
      }
      printf("\n");
      //As referenced PAGEi was not in frame(i.e it was miss/page fault) increment no_of_miss
      no_of_miss++;
    }
  }
  //Display number of misses
  printf("Page faults = %d\n", no_of_miss);
}
void lru(int reference_array[], int no_of_references, int no_of_frames) {
  //Initialise no of missese ,no of allocated frames
  int no_of_miss = 0, no_of_allocatedframes = 0, loc, min, i, frame_index;
  //Go through the reference string
  for (i = 0; i < no_of_references; i++) {
    time[reference_array[i]] = i;
    //If PAGEi referenced is not in a frame 
    if (inframe[reference_array[i]] == 0) {
      if (no_of_allocatedframes < no_of_frames)
        //If there are free frames allocate referenced PAGE in reference_array to free frame
        frame[no_of_allocatedframes++] = reference_array[i];
        //Increment allocated frames index (acts like rear, only grow till frames are full) 
      else {
        //If there are no free frames
        //Finding minimum logic
        loc = 0;
        min = time[frame[0]];
        for (frame_index = 1; frame_index < no_of_frames; frame_index++) {
          //There's is a new short king !
          if ((time[frame[frame_index]]) < min) {
            //update min and strore it's location
            min = time[frame[frame_index]];
            loc = frame_index;
          }
        }
        inframe[frame[loc]] = 0;
        //Add the new page
        frame[loc] = reference_array[i];
      }
      //Voila! PAGEi is now in a frame
      inframe[reference_array[i]] = 1;
      printf("Frames: ");
      //Display all the frames currently in frame
      for (int k = 0; k < no_of_allocatedframes; k++) {
      	printf("%d  ", frame[k]);
      }
      printf("\n");
      //As referenced PAGEi was not in frame(i.e it was miss/page fault) increment no_of_miss
      no_of_miss++;
    }
  }
  //Display number of misses
  printf("Page faults = %d\n", no_of_miss);
}
void lfu(int reference_array[], int no_of_references, int no_of_frames) {
  //Initialise no of missese ,no of allocated frames
  int no_of_miss = 0, no_of_allocatedframes = 0, loc, min, i, frame_index, first_in = 0;
  //Go through the reference string
  for (i = 0; i < no_of_references; i++) {
    count[reference_array[i]]++;
    //If PAGEi referenced is not in a frame 
    if (inframe[reference_array[i]] == 0) {
      time[reference_array[i]] = i;
      if (no_of_allocatedframes < no_of_frames)
        //If there are free frames allocate referenced PAGE in reference_array to free frame
        frame[no_of_allocatedframes++] = reference_array[i];
        //Increment allocated frames index (acts like rear, only grow till frames are full)
      else {
        //If there are no free frames
        //Take out page least frequently used i.e with least count of reference(count[] minimum)
        //sort based on time
        for (int i = 0; i < no_of_frames - 1; i++)
        {
          for (int j = 0; j < no_of_frames - 1; j++)
          {
            if (time[frame[j]] > time[frame[j + 1]])
            {
              int temp = frame[j];
              frame[j] = frame[j + 1];
              frame[j + 1] = temp;
            }
          }
        }
        //Finding minimum logic
        //Assume frame at 0 index is minimum 
        loc = 0;
        min = count[frame[0]];
        for (frame_index = 1; frame_index < no_of_frames; frame_index++) {
          //There's is a new short king !
          if ((count[frame[frame_index]]) < min) {
            //update min and strore it's location
            min = count[frame[frame_index]];
            loc = frame_index;
          }
        }
        inframe[frame[loc]] = 0;
        count[frame[loc]] = 0;
        //Add the new page
        frame[loc] = reference_array[i];
      }
      //Voila! PAGEi is now in a frame
      inframe[reference_array[i]] = 1;
      printf("Frames: ");
      //Display all the frames currently in frame
      for (int k = 0; k < no_of_allocatedframes; k++) {
      	printf("%d  ", frame[k]);
      }
      printf("\n");
      //As referenced PAGEi was not in frame(i.e it was miss/page fault) increment no_of_miss
      no_of_miss++;
    }
  }
  //Display number of misses
  printf("Page faults = %d\n", no_of_miss);
}
void optimal(int reference_array[], int no_of_references, int no_of_frames) {
  //Initialise no of missese ,no of allocated frames
  int no_of_miss = 0, no_of_allocatedframes = 0, loc, max, i, frame_index;
  //Go through the reference string
  for (i = 0; i < no_of_references; i++) {
    time[reference_array[i]] = i;
    //If PAGEi referenced is not in a frame 
    if (inframe[reference_array[i]] == 0) {
      if (no_of_allocatedframes < no_of_frames)
        //If there are free frames allocate referenced PAGE in reference_array to free frame
        frame[no_of_allocatedframes++] = reference_array[i];
        //Increment allocated frames index (acts like rear, only grow till frames are full) 
      else {
        //If there are no free frames
        //setting next occuring time of page in reference string
        //go through frames
        for (frame_index = 0; frame_index < no_of_frames; frame_index++) {
          int flag = 0;
          //go through the rest of reference string
          for(int j = i; j < no_of_references; j++){
            //first occurence of page in reference string
            if(frame[frame_index] == reference_array[j]){
              time[frame[frame_index]] = j;
              flag = 1;
              break;
            }
          }
          //no references found
          if(flag == 0)
            //set time to a very large value
            time[frame[frame_index]] = no_of_references + 1;
        }
        //Take out optimal page i.e with  time of reference(time[] maximum)
        //Finding maximum logic
        //Assume frame at 0 index is maximum 
        loc = 0;
        max = time[frame[0]];
        for (frame_index = 1; frame_index < no_of_frames; frame_index++) {
          //There's is a new tall king !
          if ((time[frame[frame_index]]) > max) {
            //update min and strore it's location
            max = time[frame[frame_index]];
            loc = frame_index;
          }
        }
        //Take it out of frame
        inframe[frame[loc]] = 0;
        //Add the new page
        frame[loc] = reference_array[i];
      }
      //Voila! PAGEi is now in a frame
      inframe[reference_array[i]] = 1;
      printf("Frames: ");
      //Display all the frames currently in frame
      for (int k = 0; k < no_of_allocatedframes; k++) {
      	printf("%d  ", frame[k]);
      }
      printf("\n");
      //As referenced PAGEi was not in frame(i.e it was miss/page fault) increment no_of_miss
      no_of_miss++;
    }
  }
  //Display number of misses
  printf("Page faults = %d\n", no_of_miss);
} 
int main() {
  int i, no_of_references, reference_array[MAX], no_of_frames, choice;
  //Read the reference string
  printf("Enter the number of references in reference string :");
  scanf("%d", &no_of_references);
  printf("Enter the reference string : ");
  for (i = 0; i < no_of_references; i++)
    scanf("%d", &reference_array[i]);
  //Read the number of frames
  printf("Enter the number of frames :");
  scanf("%d", &no_of_frames);
  do {
    //Initialise count[],time[],inframe[] each time you execute replacement algorithm
    for (i = 0; i < no_of_references; i++) {
      count[reference_array[i]] = 0;
      time[reference_array[i]] = 0;
      inframe[reference_array[i]] = 0;
    }
    //Read choice of which replacement algorithm to execute
    printf("Enter 1:FIFO 2:LRU 3:LFU 4:Optimal 5:Exit : ");
    scanf("%d", &choice);
    switch (choice) {
    case 1:
      fifo(reference_array, no_of_references, no_of_frames);
      break;
    case 2:
      lru(reference_array, no_of_references, no_of_frames);
      break;
    case 3:
      lfu(reference_array, no_of_references, no_of_frames);
      break;
    case 4:
      optimal(reference_array, no_of_references, no_of_frames);
    case 5:break;
    default:
      printf("Invalid input\n");
    }
  } while (choice != 5);
  return 0;
}
//20 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1 3 4
//16 7 0 2 4 3 1 4 7 2 0 4 3 0 3 2 7 3 3