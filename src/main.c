#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 3
#define MAX 100000

typedef struct {
  int * v;
  int beg, end;
} thread_args;

/* Merge two sorted arrays. */
void merge(int *v, int il, int jl, int ir, int jr)
{
    int tmp[MAX];
    int i,j,k;
    i = il;
    j = ir;
    k=0;

    while (i <= jl && j <= jr)
    {
      if (v[i] < v[j])
        tmp[k++] = v[i++];
      else
        tmp[k++] = v[j++];
    }

    while (i <= jl)
      tmp[k++] = v[i++];

    while (j <= jr)
      tmp[k++] = v[j++];

    for (i = il, j = 0; i <= jr; i++, j++)
      v[i] = tmp[j];
}

void mergesort(int * v, int i, int j)
{
  int mid;

  if (i < j)
  {
    mid = (i+j)/2;
    mergesort(v, i, mid);
    mergesort(v, mid+1, j);
    merge(v, i, mid, mid+1, j);
  }
}

void *worker(void *arg) {
  thread_args *info = (thread_args *) arg;

  mergesort(info->v, info->beg, info->end);

  return NULL;
}


int main()
{
  pthread_t workers[N];
  thread_args *send_args[N];
  int input[MAX];
  int n, k = 0;
  char c;

  /* Read input numbers. */
  do
  {
    scanf("%d", &input[k++]);
  }
  while((c = getchar()) != '\n');

  /* Max number of threads lesser or equal to number of entries. */
  if (k >= N)
    n = N;
  else
    n = k;

  /* Create args for each thread. */
  for (int i = 0; i < n; i++)
  {
    (send_args[i]) = (thread_args*) malloc(sizeof(thread_args));
    (send_args[i])->v = input;
    (send_args[i])->beg = (k/n)*i;
    (send_args[i])->end = (k/n)*(i+1) - 1;
  }
  /* If number of entries not divisible by n, add to last thread. */
  (send_args[n-1])->end += k%n;

  /* Create threads. */
  for (int i = 0; i < n; i++)
    pthread_create(&(workers[i]), NULL, worker, (void *) send_args[i]);

  /* Wait all threads to finish. */
  for (int i = 0; i < n; i++)
    pthread_join(workers[i], NULL);

  /* Merge all sorted subarrays of each thread. */
  for (int i = 0; i < n-1; i++)
  {
    merge(input, (send_args[0])->beg, (send_args[i])->end, (send_args[i+1])->beg, (send_args[i+1])->end);
  }

  /* Free memory from send_args struct. */
  for (int i = 0; i < n; i++)
    free(send_args[i]);

  /* Print sorted vector. */
  for (int i = 0; i < k-1; i++)
    printf("%d ", input[i]);
  printf("%d\n", input[k-1]);

  return 0;
}
