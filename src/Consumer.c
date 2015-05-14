#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>


struct theData
{
    int i,j;
    int tmpArray[24];
    int humArray[24];
    int decision;
};

struct theData *createData()
{
    struct theData *mydata = malloc(sizeof(struct theData));
    assert(mydata != NULL);
    mydata->decision = 0;
    mydata->i=0;
    mydata->j=0;
    return mydata;
}

void destructData(struct theData *myData)
{
    assert(myData != NULL);

    free(myData->humArray);
    free(myData->tmpArray);
    free(myData);
}

void printData(struct theData *mydata)
{
    printf("TMP index [i] : %d",mydata->i);
    printf("\tHUM index [j] : %d",mydata->j);

    printf("\tDecision : %d\nTMP\n",mydata->decision);
    int i;
    for(i = 0; i < 10; i++)
    {
        printf("[%d] %d|",i,mydata->tmpArray[i]);
    }
    printf("\nHUM\n");
    for(i = 0; i < 10; i++)
    {
        printf("[%d] %d|",i,mydata->humArray[i]);
    }
    printf("\n-----------\n");
}


int compareData(struct theData *data1, struct theData *data2)
{

    if(data1->i==data2->i)
    {
        if(data1->j==data2->j)
        {
            if(data1->decision==data2->decision)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}




#define	SHMSZ	1024

main()
{
    int shmid;
        key_t key;
        struct theData *shm = createData();

    /*
    * We need to get the segment named
    * "1234", created by the server.
    */
    key = 12345;

    /*
    * Locate the segment.
    */
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0)
    {
        perror("shmget");
        return 1;
    }

    /*
    * Now we attach the segment to our data space.
    */
    if ((shm = shmat(shmid, NULL, 0)) == (struct theData *) -1)
    {
        perror("shmat");
        return 1;
    }

    /*
    * Client writes measured humidity value to memory
    * for server to read.
    */
    for(;;)
    {
            if(shm->decision){

            printf("ALLUME \n");
            }else{

            printf("MCH ALLUME \n");
            }
            sleep(10);
    }
return 0;
}

/*

printf("There are %d sensors", index);
for(i=0;i<N;i++) {
	//printf("%s \n", sensors[i].label);
	printf("Element number : %d \n", i);
	printf("%s \n", sensors[i].ip);
	printf("%s \n", sensors[i].port);
	printf("%s \n", sensors[i].label);
	printf("%s \n", sensors[i].actions[0]);
	printf("%s \n", sensors[i].actions[1]);
	printf("%s \n", sensors[i].actions[2]);
}

*/
