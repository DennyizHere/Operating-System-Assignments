/*
 * Name: Denny Cao
 * Class account: cssc1191
 * Class info: CS570, Summer 2018
 * Assignment: #3, Alarm
 * Filename: tevent.c
 * File manifest: Makefile, tevent.c, tevent.h       
 */
#include "tevent.h"

int timer;
int child_id;
pid_t child1, child2;
pthread_t countThread, clockThread;
int countdownClock[2], clockParent[2];
char countdownFlag, endFlag;

// Countdown function, just deincrement the timer variable and pipes a flag
void *countdown() {
	printf("In the countdown");
	for (;;) {
		if (timer > 0) { 
			timer--;
			printf("%d", timer);
		}
		else {
			close(countdownClock[0]);
			write(countdownClock[1], "1", 1);
			close(countdownClock[1]); 
			kill(child1, SIGTERM);
		}
		sleep(1);
	}
}

// Prints out the current date and time, and terminates when timer gets to 0/get a flag from the other child
void *wclock() {
	time_t current_time;
	char* c_time_string;

	current_time = time(NULL);
	c_time_string = ctime(&current_time);

	for (;;) {
		if (countdownFlag == '1' || timer < 0) {
			close(clockParent[0]);
			write(clockParent[1], "1", 1);
			close(clockParent[1]);
			kill(child2, SIGTERM);
		}
		printf("%s", c_time_string);
		timer--;
		sleep(1);
		current_time = time(NULL);
		c_time_string = ctime(&current_time);
		close(countdownClock[1]);
		read(countdownClock[0], countdownFlag, 1);
		close(countdownClock[0]);
	}
}

void main (int argc, char* argv[]) {
	int child_id = 0;
	pipe(countdownClock);
	pipe(clockParent);
	pid_t parent = getpid();

	// Checks what time to countdown from, makes 25 as default.
	if (argc < 2) 
		timer = 25;
	else if ((int)argv[1] < 25) 
		timer = 25;
	else {
		long x = strtol(argv[1], NULL, 10);
		timer = (int)x;
	}

	printf("Counting down from %d seconds\n", timer);

	// Creates the first child and makes it go to the countdown function
	if (child_id == 0) {
		printf("Countdown should be starting\n");
		if ((child1 = fork()) == 0) {
			printf("Wow, it probably didn't run for some reason");
			pthread_create(&countThread, NULL, &countdown, NULL);
		}
	}

	child_id++;

	// Creates the second child and make is go create a thread and go to the wall clock function
	if (child_id == 1 && getpid() == parent) {
		if ((child2 = fork()) == 0) {
			pthread_create(&clockThread, NULL, &wclock, NULL);
		}
	}
	
	// Parent waits for children to finish to terminate program
	for(;;) {
		sleep(1);
		if (endFlag == '1') {
			printf("Exiting\n");
			exit(0);
		}
		close(clockParent[1]);
		read(clockParent[0], endFlag, 1);
		close(countdownClock[0]);
	}
			
}
