#include <stdio.h>
#include <stdlib.h>

int main()
{
    int data[3] = {1, 22, 3};  //Random data we want to send
    FILE *file;
    file = fopen("/dev/ttyUSB0","w");  //Opening device file
    //fprintf(file,"%c",1);
    int i = 0;
    for(i = 0 ; i < 3 ; i++)
    {
        fprintf(file,"%d",data[i]); //Writing to the file
        fprintf(file,"%c",','); //To separate digits
        //fprintf(file,"%c", data[i]);
        sleep(1);
    }

    fclose(file);
}
