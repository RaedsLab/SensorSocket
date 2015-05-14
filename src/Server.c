#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include <stdlib.h>

#define BUFLEN 512
#define NPACK 10
#define PORT 9930
#define N 3

#define SHMSZ 1024

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

struct Sensor
{
    char* ip;
    char* port;
    char* label;
    char* actions[3];
};

struct Sensor sensors[N];

// EMPTY SENSOR
struct Sensor *newSensor()
{
    struct Sensor *mySensor = malloc(sizeof(struct Sensor));
    assert(mySensor != NULL);
    return mySensor;
}

// PORT NEED TO CHANGE AND GET CASTED
struct Sensor *createSensor(char * msg, char * ip, int port)
{

    struct Sensor *mySensor = malloc(sizeof(struct Sensor));
    assert(mySensor != NULL);

    mySensor->ip = ip;

    char** tokens;
    tokens = str_split(msg, '#');

    if (tokens)
    {
        mySensor->label = *(tokens +0);
        mySensor->actions[0] = *(tokens +1);
        mySensor->actions[1] = *(tokens +2);
        mySensor->actions[2] = *(tokens +3);

        free(tokens);
    }

    return mySensor;
}

void diep(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{

    int shmid;
    key_t key;
    struct Sensor *shm = newSensor();
    struct Sensor *tmp = newSensor();
    /////
    struct sockaddr_in si_me, si_other;
    int s, i, slen=sizeof(si_other);
    int index ; //Shared var

    //structure de la mémoire partagée
    //les variables à partager : int index, sturct sensors



    char buf[BUFLEN];

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        diep("socket");

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s,(struct sockaddr *) &si_me, sizeof(si_me))==-1)
        diep("bind");

    while(1)
    {
        if (recvfrom(s, buf, BUFLEN, 0,(struct sockaddr *) &si_other, &slen)==-1)
        {
            printf("error");
            diep("recvfrom()");
        }

        struct Sensor *mySensor = createSensor(buf,inet_ntoa(si_other.sin_addr),ntohs(si_other.sin_port));


        for(i=0; i<N; i++)
        {
            if(sensors[i].label == NULL)
            {
                sensors[i].ip = mySensor->ip;
                sensors[i].port = mySensor->port;
                sensors[i].label = mySensor->label;
                sensors[i].actions[0] = mySensor->actions[0];
                sensors[i].actions[1] = mySensor->actions[1];
                sensors[i].actions[2] = mySensor->actions[2];
                index++;
                break;
            }
        }

        printf("Received packet from %s:%d\nData: %s\n\n",
               inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
    }

    close(s);
    return 0;
}
