#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t* cond = NULL;

int* nums;
volatile int cnt = 0;

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(int arr[], int n)
{
    int i, j, min_idx;

    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        swap(&arr[min_idx], &arr[i]);
    }
}

void *count_factorial(void *ptr)
{
    int turn = *(int*)ptr;

    pthread_mutex_lock(&mutex);

    if (turn != cnt) {
        pthread_cond_wait(&cond[turn], &mutex);
    }

    int i, fact = 1;

    for (i = nums[turn]; i > 0; i--) {
        fact *= i;
    }
    printf("%d! = %d\n", nums[turn], fact);

    cnt++;
    pthread_cond_signal(&cond[cnt]);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t* tid;

    int arr[argc-1];
    volatile int i;

    cond = (pthread_cond_t*)malloc(sizeof(pthread_cond_t) * (argc-1));
    tid = (pthread_t*)malloc(sizeof(pthread_t) * (argc-1));
    nums = (int*)malloc(sizeof(int) * (argc-1));

    for (i = 0; i < argc-1; i++) {
        if (pthread_cond_init(&cond[i], NULL) != 0) {
            perror("Error - pthread_cond_init()\n");
        }
    }

    for (i = 0; i < argc-1; i++) 
        nums[i] = atoi(argv[i+1]), arr[i] = i;
    selectionSort(nums, argc-1);

    for (i = 0; i < argc-1; i++) {
        if (pthread_create(&tid[i], NULL, count_factorial, (void*) &arr[i])) {
            fprintf(stderr,"Error - pthread_create()\n");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < argc-1; i++) {
        pthread_join(tid[i], NULL);
    }

    exit(EXIT_SUCCESS);
}