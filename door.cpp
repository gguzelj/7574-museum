#include <iostream>
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaphore.h"
#include "message.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct door {
    int receive_queue;
    int send_queue;
    int museum_sem;
    int *cur_occupacy;
    int *max_occupacy;
    bool running;
} door;

int getRand();
void init_door(char *pString[]);
Message *receive_request();
void send_response(Message *);


/**
 * Argv[1] = Receive response queue
 * Argv[2] = Send request queue
 */
int main(int argc, char *argv[]) {

    init_door(argv);

    while (door.running) {
        Message *m = receive_request();
        sleep(getRand()); //simulo atender al cliente
        if (m->enter) {
            p(door.museum_sem);
            {
                m->accepted = *door.cur_occupacy < *door.max_occupacy;
                if (m->accepted) {
                    safelog("Person %d accepted. Current capacity: %d / %d\n", m->person_id, *door.cur_occupacy, *door.max_occupacy);
                    (*door.cur_occupacy)++;
                } else {
                    safelog("Person %d not accepted. Current capacity: %d / %d\n", m->person_id, *door.cur_occupacy, *door.max_occupacy);
                }
            }v(door.museum_sem);
        } else {
            m->accepted = true;
            p(door.museum_sem);
            {
                safelog("Person %d leaving the museum. Current capacity: %d / %d\n", m->person_id, *door.cur_occupacy, *door.max_occupacy);
                (*door.cur_occupacy)--;
            }v(door.museum_sem);
        }
        send_response(m);
        free(m);
    }
}

Message *receive_request() {
    safelog("Receiving message from queue...\n");
    Message* message = Message_new(1);
    receive_msg(door.receive_queue, message, sizeof(Message), 0);
    safelog("Message received from %d\n", message->person_id);
    return message;
}

void send_response(Message *response) {
    safelog("Sending response to %d\n", response->person_id);
    response->mtype = response->person_id;
    send_msg(door.send_queue, response, sizeof(Message));
}

void init_door(char *argv[]) {
    srand(time(NULL));
    init_logger("Door", getpid());
    door.running = true;

    //init queues
    door.receive_queue = get_msg(atoi(argv[1]));
    door.send_queue = get_msg(atoi(argv[2]));

    //init shared memory
    int shm = get_shm(MUSEUM_SHM_ID);
    int* mapped_memory = (int *) map(shm); //[0]: cant_max, [1]: cant_actual
    door.max_occupacy = mapped_memory;
    door.cur_occupacy = mapped_memory+1;

    //init semaphores
    door.museum_sem = get_sem(MUSEUM_SEM_ID);
}


int getRand() {
    return rand() % DOOR_MAX_RESPONSE_TIME;
}







