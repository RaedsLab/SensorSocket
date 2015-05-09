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
  //  free(&myData->i);
   // free(myData);
}

void printData(struct theData *mydata)
{
    printf("TMP index [i] : %d",mydata->i);
    printf("\tHUM index [j] : %d",mydata->j);

    printf("\tDecision : %d\nTMP\n",mydata->decision);
    int i;
    for(i = 0; i < (mydata->i)%24; i++)
    {
        printf("([%d] %d) ",i,mydata->tmpArray[i]);
    }
    printf("\nHUM\n");
    for(i = 0; i < (mydata->j)%24; i++)
    {
        printf("[%d] %d|",i,mydata->humArray[i]);
    }
    printf("\n-----------\n");
}

void evaluateData(struct theData *mydata)
{
    int i;
    float sumTmp=0;
    float sumHum=0;
    for(i = 0; i < mydata->i; i++)
    {
        sumTmp+=mydata->tmpArray[i];
    }

    for(i = 0; i < mydata->j; i++)
    {
        sumHum+=mydata->humArray[i];
    }
    if(sumHum>0 && sumTmp>0){
        mydata->decision = 1;
    }
    else
    {
        mydata->decision = 0;
    }
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
