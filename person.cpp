#include "include/msg.h"
#include "message.h"
#include "constants.h"


unsigned int getRand();

struct person {
    int person_id;
    int receive_queue;
    int send_queue;
} person;

void init_person(char *pString[]);


/**
 * Argv[1] = Receive response queue
 * Argv[2] = Send request queue
 */
int main(int argc, char *argv[]) {

    init_person(argv);

    Message* message = Message_new(getpid(),person.person_id, true, false);

    safelog("Sending request to enter museum\n");
    send_msg(person.send_queue, message, sizeof(Message));

    message->mtype = person.person_id;
    receive_msg(person.receive_queue, message, sizeof(Message), getpid());
    safelog("Response received!\n");

    if (message->accepted) {
        safelog("Request accepted, entering the museum\n");
        sleep(getRand());

        safelog("Trying to leave the museum\n");
        message->enter = false;
        send_msg(person.send_queue, message, sizeof(Message));
        receive_msg(person.receive_queue, message, sizeof(Message), getpid());
        safelog("Response received!, leaving the museum\n");
    } else {
        safelog("Rejected from museum, going home..\n");
    }

}

unsigned int getRand() {
    return (unsigned int) rand() % PERSON_MAX_RIDE_TIME;
}

void init_person(char *argv[]) {
    init_logger("Person", getpid());
    person.person_id = getpid();
    person.receive_queue = get_msg(atoi(argv[1]));
    person.send_queue = get_msg(atoi(argv[2]));
}






