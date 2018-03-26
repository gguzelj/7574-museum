#include <iostream>
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaforo.h"
#include "mensaje.h"
#include <time.h>
#include <cmath>

int main(int argc, char *argv[]) {
    srand(time(NULL));
    std::cout << "----- INAUGURAMOS MUSEO -----" << std::endl;

    int cola_resp, cola_solic, puerta;

    // Memoria compartida
    int shm = creashm(7574, 2 * sizeof(int));
    int *memoria = (int *) map(shm);
    memoria[0] = floor(CANT_PERSONAS / 10);
    memoria[1] = 0; // Cantidad de gente actualmente en el museo
    memoria[3] = 0; // Personas esperando por un guía

    // Crear semáforo de memoria compartida
    int sem = creasem(0);
    inisem(sem, 1);

    // Crear semáforo de guía
    int semGuia = creasem(GUIDE_SEM_ID);
    inisem(semGuia, 0);

    // Inicio al guía
    if (fork() == 0) {
        execl("./guide", "./guide", (char *) NULL);
        exit(0);
    }

    for (int i = 0; i < CANT_PUERTAS; i++) {
        cola_resp = 2 * i;
        cola_solic = 2 * i + 1;
        // Creamos las colas para cada puerta
        int respuesta_queue = creamsg(cola_resp);
        int solicitud_queue = creamsg(cola_solic);

        if (fork() == 0) {
            std::cout << "Puerta " << i << " con pid " << getpid() << std::endl;//
            int ret = execl("./puerta", "./puerta",
                std::to_string(cola_resp), std::to_string(cola_solic),
                (char *) NULL);
            if (ret == -1) {
                perror("Error puerta " + i);
            }
            exit(0);
        }
    }

    // Generador de personas
    for (int i = 0; i < CANT_PERSONAS; i++) {
        puerta = rand() % CANT_PUERTAS;
        cola_resp = 2 * puerta;
        cola_solic = 2 * puerta + 1;

        if (fork() == 0) {
            std::cout << "Persona " << i << " con pid " << getpid() << " a puerta " << puerta << std::endl;//
            int ret = execl("./persona", "./persona",
                std::to_string(cola_resp), std::to_string(cola_solic),
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
