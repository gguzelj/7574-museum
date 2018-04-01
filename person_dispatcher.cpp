
#include <cstdlib>
#include "include/msg.h"
#include "message.h"
#include "constants.h"

/**
 * Just creates persons to enter the museum
 * @param argc
 * @param argv
 * @return
 */

void create_person();
int select_door();
unsigned int sleep_time();
bool running = true;

int main() {
    srand(time(NULL));

    while(running) {
        create_person();
        sleep(sleep_time());
    }
    return 0;
}

void create_person() {
    int selected_door = select_door();
    int send_queue_id = selected_door * 2;
    int receive_queue_id = selected_door * 2 + 1;
    if (fork() == 0) {
        char receive_queue[10], send_queue[10];
        snprintf(receive_queue, sizeof(receive_queue), "%d", receive_queue_id);
        snprintf(send_queue, sizeof(send_queue), "%d", send_queue_id);
        safelog("Creating person %d, to use door %d (%d, %d)\n", getpid(), selected_door, send_queue_id, receive_queue_id);
        execl("./person", "./person", receive_queue, send_queue, 0);
        exit(0);
    }
}

int select_door() {
    return rand() % MUSEUM_DOORS;
}

unsigned int sleep_time() {
    return static_cast<unsigned int>(rand() % 2);
}