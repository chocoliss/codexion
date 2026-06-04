// #include <stdio.h>
// #include <pthread.h>
// #include <unistd.h>

// #define NUM_WORKERS 4

// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
// int go = 0; // shared flag

// void *worker(void *arg) {
//     int id = *(int *)arg;

//     pthread_mutex_lock(&mutex);
//     while (!go) {
//         printf("Worker %d: waiting...\n", id);
//         pthread_cond_wait(&cond, &mutex); // releases mutex and sleeps
//     }
//     pthread_mutex_unlock(&mutex);

//     printf("Worker %d: got the signal, starting work!\n", id);
//     return NULL;
// }

// int main() {
//     pthread_t threads[NUM_WORKERS];
//     int ids[NUM_WORKERS];

//     // Spawn all worker threads
//     for (int i = 0; i < NUM_WORKERS; i++) {
//         ids[i] = i + 1;
//         pthread_create(&threads[i], NULL, worker, &ids[i]);
//     }

//     sleep(1); // let all workers reach the wait

//     printf("\nBoss: broadcasting GO signal to all workers!\n\n");
//     pthread_mutex_lock(&mutex);
//     go = 1;
//     pthread_cond_broadcast(&cond); // wake ALL waiting threads
//     pthread_mutex_unlock(&mutex);

//     for (int i = 0; i < NUM_WORKERS; i++)
//         pthread_join(threads[i], NULL);

//     pthread_mutex_destroy(&mutex);
//     pthread_cond_destroy(&cond);
//     return 0;
// }

#include <unistd.h>
#include <stdio.h>

void ft_putchar(int i)
{
    write(1,&i,1);
}

void ft_putnbr(int nb)
{
    int n;

	if (nb == -2147483648)
	{
		write(1,"-2147483648",11);
		return;
	}
	if (nb < 0)
	{
		nb = -nb;
		write(1,"-",1);
	}
	if (nb > 9)
        ft_putnbr(nb / 10);
	ft_putchar((nb % 10) + 48);
	return;
}

void ft_putstr(char *str)
{
	while(*str)
		write(1,str++,1);
}
int main()
{
    ft_putstr("hello world \n");
    return 0;
}