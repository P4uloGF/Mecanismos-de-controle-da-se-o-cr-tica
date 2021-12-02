#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
long int request = 0;
long int response = 0;
long int sum = 0;
int n_threads,qtd;
void *server(void *args){
    while(1){
        while(!request);
        response = request;
        while(response);
        request = 0;
    }
    pthread_exit(NULL);
}

void *client(void *args){
    long int i = (long int)args-1;
    while(i < qtd){
        while(response != (long int) args){
            request = (long int) args;
        }
        i+=n_threads;
        sum++;
        response = 0;
    }
    pthread_exit(NULL);
}
int main(){
    pthread_t s,*c;

    scanf("%d %d",&n_threads,&qtd);
    c = malloc(sizeof(pthread_t));
    if(!c) return -1;
    printf("\nQuantidade de threads: %d\nQuantidade de execucoes: %d\n",n_threads,qtd);
    pthread_create(&s,NULL,server,NULL);

    for(int i = 0; i < n_threads; i++){
        pthread_create(&c[i],NULL,client,(void*)(long int)i+1);
    }
    for(int i = 0; i < n_threads; i++){
        pthread_join(c[i],NULL);
    }
    printf("valor de sum %ld\n",sum);

    return 0;
}
