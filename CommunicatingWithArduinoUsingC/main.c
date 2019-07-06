#include <stdio.h>
#include <stdlib.h>

int main()
{
    //int data[3] = {1, 22, 3};  //Random data we want to send
    int size;
    printf("please define input array size: \n");
    scanf("%d", &size);
    int data[size];
    FILE *file;
    file = fopen("/dev/ttyUSB0","w");  //Opening device file

    printf("please write inputs value:\n");
    for(int i = 0; i < size ; i++ ){
        //printf("%d is coming !\n", %d , i);
        scanf("%d", &data[i]);
    }
    printf("processing...\n");

    for(int i = 0 ; i < size ; i++)
    {
        fprintf(file,"%d",data[i]); //Writing to the file
        fprintf(file,"%c",',');
        printf("%d ", data[i]);
        sleep(1);
    }

    fclose(file);
}
