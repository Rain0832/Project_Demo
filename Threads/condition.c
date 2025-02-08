#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_cond_t cond;
pthread_mutex_t mutex;

// list sturcture type
struct Node
{
	int number;
	struct Node* next;
};

// list head node
struct Node* head = NULL;

// producer callback function
void* producer(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);

		// bound area<

		// create new node
		struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));

		// init node
		newNode->number = rand() % 1000;
		newNode->next = head;
		head = newNode;
		printf("producer, id: %ld, number: %d\n", pthread_self(), newNode->number);
		
		// >bound area
		pthread_mutex_unlock(&mutex);

	
		// awake the blocked thread...
		pthread_cond_broadcast(&cond);
		sleep(rand() % 3);
	}

	return NULL;
}


// consumer callback function
void* consumer(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		// bound area
		
		while(head == NULL)
		{
			// block consumer thread
			pthread_cond_wait(&cond, &mutex);
		}
		struct Node* node = head;
		printf("consumer, id: %ld, number: %d\n", pthread_self(), node->number);
		head = head -> next;
		free(node);
		
		// bound area
		pthread_mutex_unlock(&mutex);
		sleep(rand() % 3);
	}
	return NULL;
}



int main(void)
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	
	pthread_t t_p[5], t_c[5];
	for(int i = 0; i < 5; i++)
	{
		pthread_create(&t_p[i], NULL, producer, NULL);
	}

	for(int i = 0; i < 5; i++)
	{
		pthread_create(&t_c[i], NULL, consumer, NULL);
	}

	for(int i = 0; i < 5; i++)
	{
		pthread_join(t_p[i], NULL);
		pthread_join(t_c[i], NULL);
	}


	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	return 0;
}
