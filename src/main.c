#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 1
#define MAX 100

typedef struct {
  int * v;
  int beg, end;
} thread_args;

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

  if (i <= j)
  {
    mid = (i+j)/2;
    mergesort(v, i, mid);
    mergesort(v, mid+1, j);
    merge(v, i, mid, mid+1, j);
  }
}

void *worker(void *arg) {
  thread_args *info = (thread_args *) arg;

  printf("%d %d %d %d\n", info->beg, (info)->end, (info)->v[(info)->beg], (info)->v[(info)->end]);

  mergesort(info->v, info->beg, info->end);
  free(info);

  return NULL;
}


int main()
{
  pthread_t workers[N];
  thread_args *send_args[N];
  int input[MAX];
  int k = 0;
  char c;

  do
  {
    scanf("%d", &input[k++]);
  }
  while((c = getchar()) != '\n');

  for (int i = 0; i < N; i++)
  {
    if (k > i) //?
    {
      (send_args[i]) = (thread_args*) malloc(sizeof(thread_args));
      (send_args[i])->v = input;
      (send_args[i])->beg = (k/N)*i;
      (send_args[i])->end = (k/N)*(i+1) - 1;
      pthread_create(&(workers[i]), NULL, worker, (void *) send_args[i]);
    }
  }

  printf(".\n");

  for (int i = 0; i < N; i++)
  {
    pthread_join(workers[i], NULL);
    printf("%d %d %d %d\n", (send_args[i])->beg, (send_args[i])->end, (send_args[i])->v[(send_args[i])->beg], (send_args[i])->v[(send_args[i])->end]);
    free(send_args[i]);
  }

  for (int i = 0; i < k-1; i++)
    printf("%d ", input[i]);
  printf("%d\n", input[k-1]);

  return 0;
}
