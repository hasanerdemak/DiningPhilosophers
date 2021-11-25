#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (philNum + 4) % N
#define RIGHT (philNum + 1) % N

void* waiter(void* num);
void serve(int waiterNum);

void* philosopher(void* num);
void take_chopsticks(int philNum);
void put_chopsticks(int philNum);
void test_and_eat(int philNum);

int quit = 0;

int N; // Number of Philosophers
int M; // Number of Waiters
int THINKING_TIME; // Thinking time(ms) for philosophers
int WHATSAPP_TIME; // Hanging on Whatsapp time(ms) for waiters
int CLOSING_TIME; // Restaurant closing time

int servingsCount = 0; // Number of servings in the tray (max 2*N)
int *philStates;
int *philosophers;
int *philEatNumber;

int *waiters;

sem_t mutex;
sem_t *chopstick_semaphores;

void exitfunc(int sig)
{
    printf("--------------------Restoran kapandi--------------------\n");
    for (int i = 0; i < N; ++i) {
        printf("%d. filozof %d porsiyon pilav yedi.\n", i+1, philEatNumber[i]);
    }
    quit = 1;

    // Deallocate the memory previously allocated.
    free(philStates);
    free(philosophers);
    free(philEatNumber);
    free(waiters);
    free(chopstick_semaphores);

    _exit(0);
}

int main(int argc, char *argv[]) {
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    THINKING_TIME = atoi(argv[3]);
    WHATSAPP_TIME = atoi(argv[4]);
    CLOSING_TIME = atoi(argv[5]);

    philStates = (int *) malloc (N * sizeof (int));
    philosophers = (int *) malloc (N * sizeof (int));
    philEatNumber = (int *) malloc (N * sizeof (int));
    chopstick_semaphores = (sem_t *) malloc (N * sizeof (sem_t));
    waiters = (int *) malloc (M * sizeof (int));

    for (int i = 0; i < N; ++i) {
        philosophers[i] = i;
    }
    for (int i = 0; i < N; ++i) {
        philEatNumber[i] = 0;
    }
    for (int i = 0; i < M; ++i) {
        waiters[i] = i;
    }

    // initialize the semaphores
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < N; i++)
        sem_init(&chopstick_semaphores[i], 0, 0);

    // Set the restaurant to close
    signal(SIGALRM, exitfunc);
    alarm(CLOSING_TIME); // Restaurant closing time

    printf("Filozoflar Diyar-i Kasigi Kirik lokantasina girdi ve masaya oturdu.\n");
    printf("Garsonlar Servis Yap-Whatsapp'ta Takil dongusune ve Filozoflar da Yeme-Dusunme dongusune basladi.\n");
    pthread_t phil_thread_id[N];
    pthread_t waiter_thread_id[M];

    // create waiter processes
    for (int i = 0; i < M; i++) {
        pthread_create(&waiter_thread_id[i], NULL, waiter, &waiters[i]);
        //printf("Waiter %d is hanging on Whatsapp\n", i + 1);
    }

    // create philosopher processes
    for (int i = 0; i < N; i++) {
        pthread_create(&phil_thread_id[i], NULL, philosopher, &philosophers[i]);
        //printf("Philosopher %d is thinking\n", i + 1);
    }

    for (int i = 0; i < M; i++)
        pthread_join(waiter_thread_id[i], NULL);
    for (int i = 0; i < N; i++)
        pthread_join(phil_thread_id[i], NULL);

}

void* philosopher(void* num) {
    int* i = num;
    while (!quit) {

        take_chopsticks(*i);

        put_chopsticks(*i);
    }
    pthread_exit(0);
}

void* waiter(void* num) {
    int* i = num;
    while (!quit) {
        serve(*i);
    }
    pthread_exit(0);
}

void serve(int waiterNum){
    sem_wait(&mutex);
    if(servingsCount != 2 * N){
        //printf("Waiter %d is serving.\n", waiterNum + 1);
        servingsCount++;
        //printf("Servis sonucu %d porsiyon pilav var.\n", servingsCount);
    }
    // Hanging on whatsapp after serving
    //printf("Waiter %d is hanging on Whatsapp.\n", waiterNum + 1);
    sem_post(&mutex);

    // Wake up Hungry philosophers after serving
    for (int i = 0; i < N; ++i) {
        if(philStates[i] == HUNGRY)
            sem_post(&chopstick_semaphores[i]);
    }

    // Wait while hanging on whatsapp
    usleep(WHATSAPP_TIME * 1000);
}


void take_chopsticks(int philNum) {
    sem_wait(&mutex);

    philStates[philNum] = HUNGRY;
    //printf("Philosopher %d is Hungry\n", philNum + 1);

    // Eat if neighbours are not eating
    test_and_eat(philNum);

    sem_post(&mutex);

    // If unable to eat wait to be signalled
    sem_wait(&chopstick_semaphores[philNum]);

}


void put_chopsticks(int philNum) {
    sem_wait(&mutex);

    //printf("Philosopher %d putting fork %d and %d down\n", philNum + 1, LEFT + 1, philNum + 1);

    philStates[philNum] = THINKING;
    //printf("Philosopher %d is thinking\n", philNum + 1);

    test_and_eat(LEFT);
    test_and_eat(RIGHT);

    sem_post(&mutex);

    // Wait while Thinking
    usleep(THINKING_TIME * 1000);
}

void test_and_eat(int philNum) {
    if (philStates[philNum] == HUNGRY && philStates[LEFT] != EATING && philStates[RIGHT] != EATING
        && servingsCount != 0) {

        //printf("Philosopher %d takes fork %d and %d\n", philNum + 1, LEFT + 1, philNum + 1);

        philStates[philNum] = EATING;
        //printf("Philosopher %d is Eating\n", philNum + 1);
        philEatNumber[philNum]++;
        servingsCount--;
        //printf("%d porsiyon pilav kaldi.\n", servingsCount);


        // Wake up hungry philosophers after eating
        sem_post(&chopstick_semaphores[philNum]);
    }
}




