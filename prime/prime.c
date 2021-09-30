// finds the primes between 2 and n;
// deleting all multiples of 2, 3, 5 etc
// not efficient each thread should do deleting for a whole
// block of values of base before going to nextbase for more

// usage: ntreads n
// where nthreads is the number of worker threads

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_N 100000000
#define MAX_THREADS 100

// shared variables
int ntreads,          // number of threads
    n,                // upper bound of range in which to find primes
    prime[MAX_N + 1], // in the end, prime[i] = 1 if i prme, else 0
    nextbase;         // next sieve multiplier checked

int work[MAX_THREADS]; //

pthread_mutex_t nextbaselock = PTHREAD_MUTEX_INITIALIZER;

// id structs for the threads
pthread_t id[MAX_THREADS];

// crossout all multiples of k, from k * k on
void
crossout (int k)
{
  int i;
  for (i = k; i * k <= n; i++)
    {
      prime[i * k] = 0;
    }
}

// work thread routine
void *
worker (int tn) // tn is thread number (0,1,...)
{
  int lim, base;

  // no need to checked multiples bigger than sqrt(n)
  lim = sqrt (n);

  do
    {
      pthread_mutex_lock (&nextbaselock);
      base = nextbase += 2;
      pthread_mutex_unlock (&nextbaselock);
      if (base <= lim)
        {
          work[tn]++;
          if (prime[base])
            crossout (base);
        }
      else
        return 0;
    }
  while (1);
}

int
main (int argc, char *argv[])
{
  int nprimes, totwork, i;
  void *p;

  n = atoi (argv[1]);
  ntreads = atoi (argv[2]);
  for (i = 2; i <= n; i++)
    prime[i] = 1;
  crossout (2);
  nextbase = 1;

  for (i = 0; i < ntreads; i++)
    {
      pthread_create (&id[i], NULL, (void *)worker, (void *)i);
    }

  totwork = 0;

  for (i = 0; i < ntreads; i++)
    {
      pthread_join (id[i], &p);
      printf ("%d values of base done\n", work[i]);
      totwork += work[i];
    }
  printf ("%d total values of base done\n", totwork);

  nprimes = 0;
  for (i = 2; i <= n; i++)
    if (prime[i])
      nprimes++;
  printf ("the number of primes found was %d\n", nprimes);

  return 0;
}
