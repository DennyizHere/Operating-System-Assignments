#include "player.h"

int file;
int diaFlag, clubFlag, heartFlag, spadeFlag;
int ret;
int count = 4;
int semFlag = 0;
char semaphoreMutx[1000];
sem_t FLAG;

void *diamond(void * array) {
	int counterD = 0;
	char *cardsD = (char *)array;
	pthread_t tidD = pthread_self();

	for (;;) {
		sem_wait(&FLAG);
		printf("Thread %d is running\n", tidD);	
		fprintf(file, "Diamond %s\n", cardsD[counterD]);
		counterD++;
		sem_post(&FLAG);
		if (counterD > 12) {
			diaFlag = 1;
			break;
		}
		usleep(125000);
	}
}

void *club(void * array) {
	int counterC = 0;
	char *cardsC = (char *)array;
	pthread_t tidC = pthread_self();

	for (;;) {
		sem_wait(&FLAG);
		printf("Thread %d is running\n", tidC);
		fprintf(file, "Club %s\n", cardsC[counterC]);
		counterC++;
		sem_post(&FLAG);
		if (counterC > 12) {
			clubFlag = 1;
			break;
		}
		usleep(250000);
	}
}

void *heart(void * array) {
	int counterH = 0;
	char *cardsH = (char *)array;
	pthread_t tidH = pthread_self();

	for (;;) {
		sem_wait(&FLAG);
		printf("Thread %d is running\n", tidH);
		fprintf(file, "Heart %s\n", cardsH[counterH]);
		counterH++;
		sem_post(&FLAG);
		if (counterH > 12) {
			heartFlag = 1;
			break;
		}
		usleep(500000);
	}
}

void *spade(void * array) {
	int counterS = 0;
	char *cardsS = (char *)array;
	pthread_t tidS = pthread_self();

	for (;;) {
		sem_wait(&FLAG);
		printf("Thread %d is running\n", tidS);
		fprintf(file, "Spade %s\n", cardsS[counterS]);
		counterS++;
		sem_post(&FLAG);
		if (counterS > 12) {
			spadeFlag = 1;
			break;
		}
		usleep(750000);
	}
}

int main(int argc, char *argv[]) {
        pthread_t diaThread, clubThread, heartThread, spadeThread;
        const char *cards[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
        diaFlag = clubFlag = heartFlag = spadeFlag = 0;

	file = fopen("STACK.txt", "w");
	fprintf(file, "Process ID: %d\n", getpid());
	sem_init(&FLAG, 0, 4);

        pthread_create(&diaThread, NULL, &diamond, cards);
        pthread_create(&clubThread, NULL, &club, cards);
        pthread_create(&heartThread, NULL, &heart, cards);
        pthread_create(&spadeThread, NULL, &spade, cards);

	pthread_join(&diaThread, NULL);
	pthread_join(&clubThread, NULL);
	pthread_join(&heartThread, NULL);
	pthread_join(&spadeThread, NULL);

        for (;;) {
                if (diaFlag == 1 && clubFlag == 1 && heartFlag == 1 && spadeFlag == 1) {
                        fclose(file);
                        sem_destroy(&FLAG);
			return 0;
                }
        }
}
