#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "main.h"

struct Sensor
{
    u_int port; // port number of the Sensor
    char ip[15]; // ip address of the Sensor
    char label[30]; // Display name
    int state; // Active / Inactive
    char *actions[3]; // an array with lables for the actions of the sensor
};

/*

int main()
{
    printf("Hello");
	exit(EXIT_SUCCESS);
} */
