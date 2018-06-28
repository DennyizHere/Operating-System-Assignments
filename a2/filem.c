#include "filem.h"
#include <string.h>
int choice, choice2;
char string[256], string2[256];
char string_backup[256];
char c;
FILE *fp, *fp2;
pid_t pid, kpid;
struct dirent *de;
struct stat fileStat;
DIR *dr;

void sigcatcher() {
	exit(0);
}

int reverse_compare (const void * a, const void * b) {
	return (*(int*)b - *(int*)b);
}

int main (int argc, char* argv[]){
signal(SIGTERM, sigcatcher);
for (;;) {
	printf("Choose a process to do:\n\n");
	printf("1 - Create a new directory file\n");
	printf("2 - Create a new regular file\n");
	printf("3 - Create a child process to write sorted output\n");
	printf("4 - Create child process to shadow write/read regular files\n");
	printf("5 - Read a file\n");
	printf("6 - Write to a file\n");
	printf("7 - Print out file status\n");
	printf("8 - Print directory listing\n");
	printf("0 - Exit\n");

	choice = getchar();

	if (choice == '1') {
		printf("Making a directory, enter the name of the directory:\n");
		scanf("%s", string);
		mkdir(string, 0777);
		printf("Exiting");
	}

	else if (choice == '2') {
		printf("Making a file, enter the name of the file:\n");
		scanf("%s", string);
		fp = fopen(string, "w");
		printf("Exiting");
	}

	else if (choice == '3') {
		printf("What file would you like to write to?\n");
                scanf("%s", string);
                pid = fork();
		fp = fopen(string, "a");
                printf("What do you want to write?\n");
                scanf("%s", string);
                fprintf(fp, string);
                fp = fopen("alphabetical", "w");
                fp2 = fopen("reversealpha", "w");
                pid = fork();
                for (;;) {
                     	if (pid == 0) {
                        	qsort(string, (sizeof(string)/sizeof(string[0])), sizeof(string), strcmp);
                             	fprintf(fp, string);
				qsort(string, (sizeof(string)/sizeof(string[0])), sizeof(string), reverse_compare);
                            	fprintf(fp2, string);
                    	}
         	}
	}

	else if (choice == '4') {
		printf("What file would you like to read or write from?\n");
		scanf("%s", string);
		printf("Would you like to read or write?\n\n");
		printf("1 - Read the file\n");
		printf("2 - Write to file\n");
		printf("0 - Exit\n");
		getchar();
		choice = getchar();

		if (choice == '1') {
                	kpid = fork();
			fp = fopen(string, "r");
                	c = fgetc(fp);
			if (kpid != 0) {
				strcat(string_backup, string);
				strcat(string_backup, ".bak");
			}
                	for (;;) {
                        	pid = wait(NULL);
                        	if (kpid != 0)
                                	pid = wait(NULL);
                        	else {
                                	fp2 = fopen(string_backup, "w");
                                	while (c != EOF) {
                                        	fputc(c, fp2);
                                        	printf("%c", c);
                                        	c = fgetc(fp);
                                	}
                                	fclose(fp);
                                	fclose(fp2);
                        	}
                	}
		}

		else if (choice == '2') {
			strcpy(string2, string);
			kpid = fork();
			fp = fopen(string2, "w");
			printf("What would you like to write?\n");
			scanf("%s", string);
			fprintf("%s", string);
			if (kpid != 0) {
				strcat(string_backup, string);
				strcat(string_backup, ".bak");
			}
                        for (;;) {
                                pid = wait(NULL);
                                if (kpid != 0)
                                        pid = wait(NULL);
                                else {
                                        fp2 = fopen(string_backup, "w");
                                        while (c != EOF) {
                                                fputc(c, fp2);
                                                c = fgetc(fp);
                                        }
                                        fclose(fp);
                                        fclose(fp2);
                                }
                        }
		}

		else if (choice == '3') {
			printf("Going back\n");
			continue;
		}

		else if (choice == '0') {
			printf("Exiting\n");
			return 0;
		}
	}

	else if (choice == '5') {
		printf("Reading file, enter the name of the file:\n");
		scanf("%s", string);
		fp = fopen(string, "r");
		if (fp == NULL) {
			printf("Cannot open file, exiting");
		}
		c = fgetc(fp);		
		while (c != EOF) {
			printf("%c", c);
			c = fgetc(fp);
		}
	}
	
	else if (choice == '6') {
		printf("Choose how you would like to write to a file:\n\n");
		printf("1 - Append at end of file\n");
		printf("2 - Insert somewhere in file (decide via byte)\n");
		printf("3 - Overwrite\n"); 
		printf("4 - Return\n");
		printf("0 - Exit\n");

		getchar();
		choice2 = getchar();

		if (choice2 == '1') {
                        printf("What file would you like to write to?\n");
                        scanf("%s", string);
			fp = fopen(string, "a");
			printf("What do you want to write?\n");
			scanf("%s", string);
			fprintf(fp, string);
		}

		else if (choice2 == '2') {
			int where;

                        printf("What file would you like to write to?\n");
                        scanf("%s", string);
                        fp = fopen(string, "w");
			printf("What byte do you want to start writing to?\n");
			scanf("%d", where);
			fseek(fp, where, SEEK_SET);
                        printf("What do you want to write?\n");
                        scanf("%s", string);
                        fprintf(fp, string);
		}

		else if (choice2 == '3') {
                        printf("What file would you like to write to?\n");
                        scanf("%s", string);
                        fp = fopen(string, "w");
                        printf("What do you want to write?\n");
                        scanf("%s", string);
                        fprintf(fp, string);
		}
		
		else if (choice2 == '4') {
			continue;
		}

		else if (choice2 == '0') {
			return 0;
		}

		else {
			printf("Not a valid number");
			return 0;
		}
	}

	else if (choice == '7') {
		printf("Getting file information, enter file name:\n");
		scanf("%s", string);	
		if (stat(string, &fileStat) < 0) {
			printf("Error, exiting");
		}
		else {
			printf("Permissions: %d\n", fileStat.st_mode);
			printf("Size: %d bytes\n", fileStat.st_size);
			printf("Number of links: %d\n", fileStat.st_nlink);
			printf("File inode: %d\n", fileStat.st_ino);
			printf("User ID: %d\n", fileStat.st_uid);
			printf("Last access time: %s\n", ctime(fileStat.st_atime));
			printf("Exiting");
		}
	}

	else if (choice == '8') {
		dr = opendir(".");
		while ((de = readdir(dr)) != NULL)
			printf("%s\n", de->d_name);
		closedir(dr);
		printf("Exiting");
	}

	else if (choice == '0') {
		printf("Exiting");
		return 0;
	}

	else {
		printf("Not a valid input, exiting");
		return 0;
	}	
getchar();
}
killpg(getpid(), SIGTERM);
return 0;
}
