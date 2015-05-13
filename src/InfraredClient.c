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

#define BUFLEN 512
#define NPACK 10
#define PORT 9930
#define SRV_IP "172.16.169.67" // UPDATE VALUE FOR NEW SERVER IP

#define CAP_NAME "Capteur temp";

struct Capteur
{
    char* label; // Display name
    char *actions[3]; // an array with lables for the actions of the sensor
};

struct Capteur *createCapteur()
{
    struct Capteur *monCapteur = malloc(sizeof(struct Capteur));
    assert(monCapteur != NULL);
    monCapteur->label="";
    monCapteur->actions[0] = "ServiceNotAvailable";
    monCapteur->actions[1] = "ServiceNotAvailable";
    monCapteur->actions[2] = "ServiceNotAvailable";

    return monCapteur;
}

char * getInitialMessage(struct Capteur *c)
{
    char* msg;
    msg = malloc(512);
    strcpy(msg, c->label);
    strcat(msg, "#");
    strcat(msg, c->actions[0]);
    strcat(msg, "#");
    strcat(msg, c->actions[1]);
    strcat(msg, "#");
    strcat(msg, c->actions[2]);

    return msg;
}

int getTemp()
{
    srand(time(NULL));

    int r = 0;
    do
    {
        r = rand();
    }
    while(r>45 || r<0);
    return r;
}


int setTemp(int t)
{
    return t;
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
    struct Capteur *monCapteur = createCapteur();
    monCapteur->label = "Capteur Tmp";
    monCapteur->actions[0] = "GetTemp";
    monCapteur->actions[1] = "SetTemp";

    char* msg;
    msg = malloc(512);

    msg = getInitialMessage(monCapteur);

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