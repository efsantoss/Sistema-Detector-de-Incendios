#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 30

extern char floresta[N][N];
extern pthread_mutex_t mutex_floresta[N][N];  // Mutex 

void forest_inicializer() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (rand() % 5 == 0) {
                floresta[i][j] = '-';  // Áreas livres representadas por '-'
            } else {
                floresta[i][j] = 'T';  // Células monitoradas por nós sensores representadas por 'T'
            }
            pthread_mutex_init(&mutex_floresta[i][j], NULL);
        }
    }
}

void show_forest() {
    printf("\nEstado atual da floresta:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c ", floresta[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Thread do sensor
void* sensor_thread(void* args) {
    int id = (intptr_t) args;
    int x = id / N;
    int y = id % N;

    while (1) {
        pthread_mutex_lock(&mutex_floresta[x][y]);
        
        if (floresta[x][y] == '@') {
            // Notifica vizinhos sobre incêndio
            printf("Fogo avistado em [%d, %d]!\n", x, y);
            
            // Propagar o incêndio para os vizinhos
            if (x > 0 && floresta[x-1][y] == 'T') floresta[x-1][y] = '@';  // Propaga para o norte
            if (x < N-1 && floresta[x+1][y] == 'T') floresta[x+1][y] = '@';  // Propaga para o sul
            if (y > 0 && floresta[x][y-1] == 'T') floresta[x][y-1] = '@';  // Propaga para o oeste
            if (y < N-1 && floresta[x][y+1] == 'T') floresta[x][y+1] = '@';  // Propaga para o leste
            
            // Central de controle se estiver na borda
            if (x == 0 || x == N-1 || y == 0 || y == N-1) {
                // Enviar mensagem para central
                printf("Notificando central sobre fogo na borda [%d, %d]\n", x, y);
            }
        }

        pthread_mutex_unlock(&mutex_floresta[x][y]);
        sleep(1);  // Espera antes de verificar novamente
    }
    return NULL;
}

// Thread geradora de incêndios
void* fire_generate(void* args) {
    while (1) {
        int x = rand() % N;
        int y = rand() % N;

        pthread_mutex_lock(&mutex_floresta[x][y]);
        if (floresta[x][y] == 'T') {
            floresta[x][y] = '@';  // Incêndio iniciado
            printf("Incêndio iniciado em [%d, %d]\n", x, y);
        }
        pthread_mutex_unlock(&mutex_floresta[x][y]);

        sleep(3);  // Gera incêndios a cada 3 segundos
    }
    return NULL;
}

// Thread da central de controle
void* central_thread(void* args) {
    while (1) {
        // A central coleta mensagens e combate incêndios
        // Implementar lógica de coleta de mensagens e combate ao fogo

        sleep(1);  // Ação contínua da central
    }
    return NULL;
}
