#include <iostream>
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaforo.h"
#include "mensaje.h"
#include <time.h>
#include <cmath>

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int cola_recep, cola_envia, puerta;

    // Memoria compartida
    int shm = creashm(7574, 2 * sizeof(int));
    int *memoria = (int *) map(shm);
    memoria[0] = floor(CANT_PERSONAS / 10);
    memoria[1] = 0; // Cantidad de gente actualmente en el museo
    memoria[2] = 0; // Puertas inicializadas

    // Crear semáforo de memoria compartida
    int sem = creasem(0);
    inisem(sem, 0);

    // Crear semáforo para empezar a generar personas
    //int semGen = creasem(1);
    //inisem(semGen, 0);
    /// No puedo crear un segundo semáforo...

    for (int i = 0; i < CANT_PUERTAS; i++) {
        cola_recep = 2 * i;
        cola_envia = 2 * i + 1;

        if (fork() == 0) {
            std::cout << "Puerta " << i << " con pid " << getpid() << std::endl;//
            int ret = execl("./puerta", "./puerta",
                std::to_string(cola_recep), std::to_string(cola_envia),
                (char *) NULL);
            if (ret == -1) {
                perror("Error puerta " + i);
            }
            exit(0);
        }
    }

    // No deberían generarse personas hasta q las puertas hayan creado sus colas
    sleep(1);
    ///p(semGen);

    // Generador de personas
    for (int i = 0; i < CANT_PERSONAS; i++) {
        puerta = rand() % CANT_PUERTAS;
        cola_recep = 2 * puerta;
        cola_envia = 2 * puerta + 1;

        if (fork() == 0) {
            std::cout << "Persona " << i << " con pid " << getpid() << " a puerta " << puerta << std::endl;//
            int ret = execl("./persona", "./persona",
                std::to_string(cola_recep), std::to_string(cola_envia),
                (char *) NULL);
            if (ret == -1) {
                perror("Error persona " + i);
            }
            exit(0);
        }
        if (i % 30 == 0)
            sleep(1); // Pausa entre creaciôn de grupos de 30 personas
    }

    while (true) {}
}
