#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h> /* for strncpy */
#include <assert.h>

#include <time.h>
#include <stdlib.h>

#include <stdbool.h> /* bool */

#define BUFLEN 512
#define NPACK 10
#define PORT 9930
#define SRV_IP "172.16.169.67" // UPDATE VALUE FOR NEW SERVER IP

#define CAP_NAME "Infrared Sensor";

struct Sensor
{
    char* label; // Display name
    char *actions[3]; // an array with lables for the actions of the sensor
};

struct Sensor *createSensor()
{
    struct Sensor *mySensor = malloc(sizeof(struct Sensor));
    assert(mySensor != NULL);
    mySensor->label = "";
    mySensor->actions[0] = "ServiceNotAvailable";
    mySensor->actions[1] = "ServiceNotAvailable";
    mySensor->actions[2] = "ServiceNotAvailable";

    return mySensor;
}

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

    return msg;
}

bool isActive()
{
    return true;
}

////////////
void diep(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];

    //TEST IF First RUN
    int firstRun = 1;
    //VARS FOR SENSOR
    struct Sensor *mySensor = createSensor();
    mySensor->label = CAP_NAME;
    mySensor->actions[0] = "isActive";

    char* msg;
    msg = malloc(512);

    msg = getInitialMessage(mySensor);

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        diep("socket");

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    if (inet_aton(SRV_IP, &si_other.sin_addr)==0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    if(firstRun!=0)
    {
        printf("FIRST RUN \n");
	printf("%s", mySensor->label);
        firstRun =0;
        //SEND INITIAL DISVOVERY MESSAGE
        if (sendto(s, msg, BUFLEN, 0,(struct sockaddr *) &si_other, slen)==-1)
        {
            diep("sendto()");
        }
    }


    /*for (i=0; i<NPACK; i++)
    {
        printf("Sending packet %d\n", i);
        sprintf(buf, "This is packet %d\n", i);
        if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1)
            diep("sendto()");
    }*/

    close(s);
    return 0;
}
