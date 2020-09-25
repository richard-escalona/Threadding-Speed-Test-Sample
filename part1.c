//Author: Richard Escalona
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
void *ThreadB(void *arg);
void *ThreadFirstHalf(void *arg);
void *ThreadSecondHalf(void *arg);
void *mergeThread(void *arg);
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

//using to save/pass both halfs
struct merge
{
  double *firsthalf;
  double *secondhalf;
  double *C;
};
//global variable to save the array size
int arrSize;
int arrHalf;
int main(int argc, char *argv[])
{
  //get size from argument
  arrSize = atoi(argv[1]);
  arrHalf = arrSize / 2;
  if (argc != 2)
  {
    printf("ERROR: Enter the correct amount of arguments!\n");
    return -1;
  }
  int i;
  double randomNumber;
  pthread_t tid, tid1, tid2, tid3;
  clock_t start_t, end_t, start_t2, end_t2, total_1;
  double *THB;
  int count = 0;
  double *THA1;
  double *THA2;
  //Array A (n double values) random generated values.
  double A[arrSize];
  double B[arrSize];
  double C[arrSize];
  double AfirstHalf[arrHalf];
  double AsecondHalf[arrHalf];
  //putting random doubles into A
  for (i = 0; i < arrSize; i++)
  {
    randomNumber = (double) rand() / 10000000;
    A[i] = randomNumber;
    count++;
  }
 
  //copying A to B
  for (i = 0; i < arrSize; i++)
  {
    B[i] = A[i];
  }
  
  // IMPLEMENTATION FOR SINGLE THREAD USE
  start_t = clock();
  pthread_create(&tid, NULL, ThreadB, (void *)B);
  pthread_join(tid, (void **)&THB);
  end_t = clock() - start_t;

  printf("Sorting is done in %fms when one thread is used\n", end_t * 1000.0 / CLOCKS_PER_SEC);

  //*****************************Multi-threading / Merging part ***********************************

  //Copy first half A into AfirstHalf
  for (i = 0; i < arrHalf; i++)
  {
    AfirstHalf[i] = A[i];
  }
  //Copy second half A into AsecondHalf
  for (i = arrHalf; i < arrSize; i++)
  {
    AsecondHalf[i - arrHalf] = A[i];
  //printf("Secondhalf %d: %f %f \n",i,A[i],AsecondHalf[i]);
  }
  // MULTITHREADING AND MERGING
  struct merge MERGE;
  MERGE.C = (double *)malloc(arrSize * sizeof(double));
  start_t2 = clock(); // STARTING CLOCK
  pthread_create(&tid1, NULL, ThreadFirstHalf, (void *)AfirstHalf);
  pthread_create(&tid2, NULL, ThreadSecondHalf, (void *)AsecondHalf);
  pthread_join(tid1, (void **)&THA1);
  pthread_join(tid2, (void **)&THA2);
  MERGE.firsthalf = THA1;
  MERGE.secondhalf = THA2;
  pthread_create(&tid3, NULL, mergeThread, &MERGE);
  pthread_join(tid3, NULL);
  end_t2 = clock() - start_t2; //STOPPING CLOCK
  total_1 = end_t2 * 1000.0 / CLOCKS_PER_SEC;

  printf("Sorting is done in %fms when two threads are used\n", end_t2 * 1000.0 / CLOCKS_PER_SEC);
}
//THREAD FUNCTION TO SORT B
void *ThreadB(void *arg)
{
  double *myarr, *myptrret;
  myarr = (double *)arg;
  myptrret = (double *)malloc(arrSize * sizeof(double));
  int i, j;
  double temp;
  //Sorting Algo
  for (i = 1; i < arrSize; i++)
  {
    temp = myarr[i];
    j = i - 1;
    while (j >= 0 && myarr[j] > temp)
    {
      myarr[j + 1] = myarr[j];
      j = j - 1;
    }
    myarr[j + 1] = temp;
  }
  myptrret = myarr;
  pthread_exit((void *)myptrret);
}


//THREAD Function to sort firsthalf
void *ThreadFirstHalf(void *arg)
{
  double *myarr, *myptrret;
  myarr = (double *)arg;
  myptrret = (double *)malloc(arrHalf * sizeof(double));

  if (myptrret == NULL)
  {
    printf("faillllllleeed");
    exit(-1);
  }

  int i, j;
  int min;
  double temp;
//Sorting Algo
  for (i = 0; i < arrHalf; i++)
  {
    min = i;
    for (j = i + 1; j < arrHalf; j++)
    {
      if (myarr[j] < myarr[min])
        min = j;
    }
    temp = myarr[i];
    myarr[i] = myarr[min];
    myarr[min] = temp;
  }
  myptrret = myarr;
  pthread_exit((void *)myptrret);
}
 // Thead to sort the second half of A
void *ThreadSecondHalf(void *arg)
{
  double *myarr, *myptrret;
  myarr = (double *)arg;
  myptrret = (double *)malloc(arrHalf * sizeof(double));

  if (myptrret == NULL)
  {
    printf("faillllllleeed");
    exit(-1);
  }
  int i, j;
  int min;
  double temp;
  for (i = arrHalf; i < arrSize; i++)
  {
    min = i;
    for (j = i + 1; j < arrSize; j++)
    {
      if (myarr[j] < myarr[min])
        min = j;
    }
    temp = myarr[i];
    myarr[i] = myarr[min];
    myarr[min] = temp;
  }
  myptrret = myarr;

  pthread_exit((void *)myptrret);
}


//thread to merge to array C. Will be passing struct instead of array on this one.
void *mergeThread(void *arg)
{
  int i, j;
  struct merge *myt = (struct merge *)arg;
  for (i = 0; i < arrHalf; i++)
  {
    myt->C[i] = myt->firsthalf[i];
  }
  arrSize = arrHalf + arrHalf;
  for (i = 0, j = arrHalf; j < arrSize && i < arrHalf; i++, j++)
  {
    myt->C[j] = myt->secondhalf[i];
  }
  return NULL;
}