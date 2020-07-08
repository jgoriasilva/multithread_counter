/* Nome: João Antônio Gória Silva
 * RA: 199567
 */


#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#define N_THREADS 4
#define SIZE 256


unsigned long long dados[SIZE];
int count_dados_leitura = 0, count = 0;
int n_dados = 0;
pthread_t threads[N_THREADS];
pthread_mutex_t trava;

void *process(void *args){
	char* p_j  = (char*) args;
	char j  = *p_j;
	unsigned long long n;
	int flag = 1;
	while(1){
		pthread_mutex_lock(&trava);
		if(count_dados_leitura>=n_dados){
			pthread_mutex_unlock(&trava);
			break;
		}
		//printf("Na thread %d, count_dados_leitura = %d\n", j, count_dados_leitura);
		n = dados[count_dados_leitura++]; 
		pthread_mutex_unlock(&trava);
		//printf("Thread %d processa: %llu\n", j, n);
    /* Verifica se o número é primo */
    if(n==1)
    	flag=0;
    else{
    	for(unsigned long long i = 2; i < n/2; i++){
      	if(n%i==0){
        	flag = 0;
          //printf("Thread %d diz que %llu nao eh primo!\n", j, n);
          break;
        }
      }
    }
    if(flag){
    	//printf("Thread %d diz que %llu eh primo!\n", j, n);
      count++;
    }
 }
}


int main(){
	unsigned long long n;
	char line[SIZE], *val;
	char delims[] = " \n";

	/* Ler entrada do programa */
	fgets(line, SIZE, stdin);
	val = strtok(line, delims);
	////printf("val: %s\n", val);

	/* Colocar cada número no vetor, individualmente */
	int count_dados_escrita = 0;
	for(;;){
    if(sscanf(val, "%llu", &n)){
			dados[count_dados_escrita++] = n;
			val = strtok(NULL, delims);
			////printf("val: %s\n", val);
			if(!(val == NULL))
				sscanf(val, "%llu", &n);
			else
				break;
		}
  }
	n_dados = count_dados_escrita;
	//printf("Loop terminado\nDados = %d\n", n_dados);

	/* Gera N_THREADS threads para processamento dos números */
	for(char j=0; j<N_THREADS; j++){
		pthread_mutex_lock(&trava);
		//printf("Gerando thread %d...\n", j);
		pthread_create(&threads[j], NULL, process, &j);
		pthread_mutex_unlock(&trava);
	}

	/* Espera finalização de todos os processos filhos */
	//printf("Todos as threads foram geradas. Esperando...\n");
  for (char j=0; j<N_THREADS; j++){
		pthread_join(threads[j], NULL);
		//printf("Thread %d finalizou\n", j);
	}

  //printf("Todos as threads terminaram!\n");
	//printf("Numero de primos = %d\n", count);
	printf("%d\n", count);

	return(0);
}
