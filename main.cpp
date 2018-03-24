#include <iostream>
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaforo.h"
#include "mensaje.h"
#include <time.h>

int main() {
    srand(time(NULL));

    int cola_recep, cola_envia, puerta;

    // Memoria compartida
    int shm = creashm(7574, 2 * sizeof(int));
    int *memoria = (int *) map(shm);
    memoria[0] = MAX_PEOPLE;
    memoria[1] = 0; // Cantidad de gente actualmente en el museo
    memoria[2] = 0; // Puertas inicializadas

    // Crear semáforo de memoria compartida
    int sem = creasem(0);
    inisem(sem, 0);

    // Crear semáforo para empezar a generar personas
    // int semGen = creasem(1);
    // inisem(semGen, 0);
    /// No puedo crear un segundo semáforo...

    for (int i = 0; i < MAX_PUERTA; i++) {
        cola_recep = 2 * i;
        cola_envia = 2 * i + 1;

        if (fork() == 0) {
            std::cout << "Hello: " << getpid() << std::endl;//
            execl("./puerta", "./puerta", cola_recep, cola_envia, 0, (char *) NULL);
            std::cout << "Hello2: " << getpid() << std::endl;//
            exit(0);
        }
    }

    // No deberían generarse personas hasta q las puertas hayan creado sus colas
    sleep(1);///p(semGen);

    // Generador de personas
    for (int i = 0; i < 10; i++) {
        puerta = rand() % MAX_PUERTA;
        cola_recep = 2 * i;
        cola_envia = 2 * i + 1;

        if (fork() == 0) {
            execl("./persona", "./persona", cola_recep, cola_envia, 0, (char *) NULL);
            std::cout << "Bye: " << getpid() << " " << puerta << std::endl;//
            exit(0);
        }
        sleep(0.2); // Pausa entre creaciôn de 2 personas
    }

    while (true) {}
}
