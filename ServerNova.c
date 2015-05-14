#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

///// SENSOR /////

struct Sensor
{
    char* ip;
    char* port;
    char* label;
    char* actions[3];
};


char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

//////////////
struct Sensor *createSensor(char * msg)
{

    struct Sensor *mySensor = malloc(sizeof(struct Sensor));
    assert(mySensor != NULL);

    char** tokens;
    tokens = str_split(msg, '#');

    if (tokens)
    {
        mySensor->label = *(tokens +0);
        mySensor->actions[0] = *(tokens +1);
        mySensor->actions[1] = *(tokens +2);
        mySensor->actions[2] = *(tokens +3);
        mySensor->ip = *(tokens +4);
        mySensor->port = *(tokens +5);

        free(tokens);
    }

    return mySensor;
}

/////////////

char * getInitialMessage(struct Sensor *s)
{
    char* msg;
    msg = malloc(512);
    strcpy(msg, s->label);
    strcat(msg, "#");
    strcat(msg, s->actions[0]);
    strcat(msg, "#");
    strcat(msg, s->actions[1]);
    strcat(msg, "#");
    strcat(msg, s->actions[2]);
    strcat(msg, "#");
    strcat(msg, s->ip);
    strcat(msg, "#");
    strcat(msg, s->port);
    return msg;
}
//////////////



#define SHMSZ 1024

int main(int argc, char **argv)
{
    struct Sensor* ptr;

    //
    struct Sensor* mySensor1;
    mySensor1->ip = "192.168.1.2";
    mySensor1->port = "abc";
    mySensor1->label = "label1";
    mySensor1->actions[0]="0Ac";
    mySensor1->actions[1]="1Ac";
    mySensor1->actions[2]="2Ac";

    /*
        struct Sensor* mySensor2;
        mySensor2.ip = "192.168.1.3";
        mySensor2.port = "abc";
        mySensor2.label = "label2";

        struct Sensor* mySensor3;
        mySensor3.ip = "192.168.1.4";
        mySensor3.port = "abc";
        mySensor3.label = "label3"; */
    //

    char ** msg1 = malloc(1024);

    int shmid;
    key_t key;
    key = 12345;

    /// SHARED MEMORY DEFINE ///
    if ((shmid = shmget(key, sizeof(msg1), IPC_CREAT | 0666)) < 0)
    {
        perror("Error creating shared memory");
        return 1;
    }

    printf("shmid = %d\n", shmid);

    msg1 = shmat(shmid, NULL, 0);

    printf("size : %lu \n", sizeof(msg1));

   // memset(msg1,0,sizeof(msg1));

    msg1 = getInitialMessage(mySensor1);

    printf("%c \n",*msg1++);

    /*  memcpy(&ptr[0], &mySensor1, sizeof(struct Sensor));
      memcpy(&ptr[1], &mySensor2, sizeof(struct Sensor));
      memcpy(&ptr[2], &mySensor3, sizeof(struct Sensor));*/

    /// ! SHARED MEMORY DEFINE ///
    return 0;
}

