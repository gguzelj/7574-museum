#include <iostream>
#include <fcntl.h>
#include "./include/msg.h"
#include "include/shm.h"
#include "constants.h"
#include "include/semaphore.h"


void create_shared_memory();
void create_semaphores();
void create_doors();

void create_person_dispatcher();

int main() {
    init_logger("main", getpid());
    open(DIRECTORY, O_WRONLY | O_APPEND | O_CREAT, 0644);
    create_shared_memory();
    create_semaphores();
    create_doors();
    create_person_dispatcher();
    return 0;
}

void create_shared_memory() {
    int shm = create_shm(MUSEUM_SHM_ID, sizeof(int) * 2);
    int *memoria = (int *) map(shm);
    memoria[0] = MUSEUM_CAPACITY;
    memoria[1] = 0;
}

void create_semaphores() {
    int museum_sem_id = create_sem(MUSEUM_SEM_ID);
    init_sem(museum_sem_id, 1);
}

void create_doors() {
    safelog("Initializing doors processes\n");
    for (int i = 0; i < MUSEUM_DOORS * 2; i+=2) {
        create_msg(i);
        create_msg(i + 1);
        if (fork() == 0) {
            char receive_queue[10], send_queue[10];
            snprintf(receive_queue, sizeof(receive_queue), "%d", i);
            snprintf(send_queue, sizeof(send_queue), "%d", i + 1);
            execl("./door", "./door", receive_queue, send_queue, 0);
            exit(0);
        }
    }
}

void create_person_dispatcher() {
    safelog("Initializing person dispatcher process\n");
    execl("./person_dispatcher", "./person_dispatcher", 0);
}
