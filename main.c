#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>  

#define N 30

char floresta[N][N];
pthread_mutex_t mutex_floresta[N][N];  // Mutex para cada célula da matriz

void* sensor_thread(void* args);
void* central_thread(void* args);
void* fire_generate(void* args);
void forest_inicializer();
void show_forest();

int main() {
    pthread_t sensores[N][N];  // Thread para cada nó sensor
    pthread_t central, gerador;

    forest_inicializer();

    // Criar thread para a central de controle
    pthread_create(&central, NULL, central_thread, NULL);

    // Criar threads para os sensores
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pthread_create(&sensores[i][j], NULL, sensor_thread, (void*)(intptr_t)(i * N + j));
        }
    }

    // Criar thread para gerar incêndios
    pthread_create(&gerador, NULL, fire_generate, NULL);

    // Exibir a floresta periodicamente
    while (1) {
        show_forest();  // Exibe o estado atual da floresta
        sleep(5);  // A cada 5 segundos, a floresta é exibida
    }

    // Aguardar threads
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pthread_join(sensores[i][j], NULL);
        }
    }

    pthread_join(central, NULL);
    pthread_join(gerador, NULL);

    return 0;
}
