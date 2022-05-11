#include <omp.h>        //Use OpenMP
#include <stdio.h>      //Provide I/O
#include <stdlib.h>     //Use NULL
#include <unistd.h>     //Use usleep (sleep for a number of microseconds)
#include <math.h>
#define NUM_PHIL 5      //The number of philosophers
#define MEALS 100        //The number of meals per philosopher

static omp_lock_t chopsticks[NUM_PHIL];   //Locks to represent chopsticks

int encrypt(char* eMessage, int key){

char ch;
int i;

 for(i = 0; eMessage[i] != '\0'; ++i){
            ch = eMessage[i];

            if(ch >= 'a' && ch <= 'z'){
                ch = ch + key;

                if(ch > 'z'){
                    ch = ch - 'z' + 'a' - 1;
                }

                eMessage[i] = ch;
            }
            else if(ch >= 'A' && ch <= 'Z'){
                ch = ch + key;

                if(ch > 'Z'){
                    ch = ch - 'Z' + 'A' - 1;
                }

                eMessage[i] = ch;
            }
        }

        printf("\nEncrypted message: %s", eMessage);
	printf("\nYour Key: %d",key); 

}

int decrypt(char* dMessage, int dkey){

char ch;
int i;
char message[1000];

                  for(i = 0; dMessage[i] != '\0'; ++i){
                    ch = dMessage[i];

                    if(ch >= 'a' && ch <= 'z'){
                        ch = ch - dkey;

                        if(ch < 'a'){
                            ch = ch + 'z' - 'a' + 1;
                        }

                        dMessage[i] = ch;
                    }
                    else if(ch >= 'A' && ch <= 'Z'){
                        ch = ch - dkey;

                        if(ch < 'A'){
                            ch = ch + 'Z' - 'A' + 1;
                        }

                        dMessage[i] = ch;
                    }
                }

	printf("\n Decrypted message: %s\n", dMessage);
}	

void philosopher()
{
  //Wait for all threads to start
  #pragma omp barrier

  //Set up variables for the thread id, right chopstick, and left chopstick.
  int id = omp_get_thread_num();
  int right_chopstick;
  int left_chopstick;
  double start = omp_get_wtime();
  //Philosophers 0 through NUM_PHIL-2 reach to the left first then the right.
  if(id < NUM_PHIL -1)
  {
    right_chopstick = id;
    left_chopstick = id+1;
  }
  //Philosopher NUM_PHIL-1 has its left and right swapped.  Notice that it reaches in the
  //opposite order of all other philosophers.
  else
  {
    right_chopstick = 0;
    left_chopstick = id;
  }
  //Acquire chopsticks (semaphores), eat, wait for 100 microseconds, then release
  //chopsticks (semaphores).
  int i;
  for(i = 0; i < MEALS; i++)
  {
    omp_set_lock(&chopsticks[left_chopstick]);
    omp_set_lock(&chopsticks[right_chopstick]);

    usleep(100);

    omp_unset_lock(&chopsticks[left_chopstick]);
    omp_unset_lock(&chopsticks[right_chopstick]);
  }
  double end = omp_get_wtime();
  double total = end - start;
  FILE *fptr;
  fptr = fopen("numbers.txt", "a");
  fprintf(fptr, "%g,", total);
  fclose(fptr);
  //printf("%g\n", total);
}

//Main function.
int main(int argc, char ** argv)
{
  int i;

  //Initialize locks
  for(i = 2; i < NUM_PHIL; i++)
    omp_init_lock(&chopsticks[i]);
//Create and start philosopher threads.
  #pragma omp parallel num_threads(NUM_PHIL)
  {
    philosopher();
  }
   //Wait for philosophers to finish then destroy locks.
  for(i = 0; i < NUM_PHIL; i++)
    omp_destroy_lock(&chopsticks[i]);

  //End program.
  float numbers[i];
  FILE *fptr;
  float number = 1;
  int num;
  fptr = fopen("numbers.txt", "r");
  for(i = 0; i < 5; i++)
  {
          fscanf(fptr, "%f,", &numbers[i]);
          numbers[i] = numbers[i] *100000000;
  }
//  for(i = 0; i < 3; i++)
//  {
//        number += numbers[i] / numbers[i + 1];
//  }
  number = numbers[0] / numbers[1] * numbers[2] / numbers[3] *numbers[4];
  num = (int)number;  //random number
  num = num % 10;
  printf("random num: %d \n", num);
  fclose(fptr);
  remove("numbers.txt");

//UI
  
  int userchoice;
  int deckey;
  char userstring[1000];
  char encrmessage[1000];
/*
  printf("Do you want to encrypt or decrypt a message (1 or 2)?\n");
  scanf("%d", &userchoice);

  if (userchoice == 1){
	printf("\nEnter the message you would like to encrypt: ");
	scanf("%s", userstring);
	encrypt(userstring, num);
  }else if (userchoice == 2){
	printf("\nEnter the encrypted message you would like to decrypt: ");
	scanf("%s", encrmessage);
	printf("\nEnter the decryption key: ");
	scanf("%d", &deckey);
	decrypt(encrmessage, deckey);
  }else{
	printf("Invalid!");	  
  }
  */
  return 0;
}

