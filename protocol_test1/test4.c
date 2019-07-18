        /*====================================================================================================*/
        /* Serial Port Programming in C (Serial Port Write)                                                   */
	/* Non Cannonical mode                                                                                */
	/*----------------------------------------------------------------------------------------------------*/
        /* Program writes a character to the serial port at 115200 bps 8N1 format                               */
	/* Baudrate - 115200                                                                                    */
	/* Stop bits -1                                                                                       */
	/* No Parity                                                                                          */
        /*----------------------------------------------------------------------------------------------------*/
	/* Compiler/IDE  : gcc 4.6.3                                                                          */
	/* Library       :                                                                                    */
	/* Commands      : gcc -o serialport_write serialport_write.c                                         */
	/* OS            : Linux(x86) (Linux Mint 13 Maya)(Linux Kernel 3.x.x)                                */                              
	/* Programmer    : Rahul.S                                                                            */
	/* Date	         : 21-December-2014                                                                   */
	/*====================================================================================================*/
	
	/*====================================================================================================*/
	/* Running the executable                                                                             */
	/* ---------------------------------------------------------------------------------------------------*/ 
	/* 1) Compile the  serialport_read.c  file using gcc on the terminal (without quotes)                 */
        /*                                                                                                    */
	/*	" gcc -o serialport_write serialport_write.c "                                                */
	/*                                                                                                    */
        /* 2) Linux will not allow you to access the serial port from user space,you have to be root.So use   */
        /*    "sudo" command to execute the compiled binary as super user.                                    */
        /*                                                                                                    */
        /*       "sudo ./serialport_write"                                                                    */
	/*                                                                                                    */
	/*====================================================================================================*/

	/*====================================================================================================*/
	/* Sellecting the Serial port Number on Linux                                                         */
	/* ---------------------------------------------------------------------------------------------------*/ 
	/* /dev/ttyUSBx - when using USB to Serial Converter, where x can be 0,1,2...etc                      */
	/* /dev/ttySx   - for PC hardware based Serial ports, where x can be 0,1,2...etc                      */
        /*====================================================================================================*/
	
	/*-------------------------------------------------------------*/
    	/* termios structure -  /usr/include/asm-generic/termbits.h    */ 
	/* use "man termios" to get more info about  termios structure */
	/*-------------------------------------------------------------*/

    	#include <stdio.h>
    	#include <fcntl.h>   /* File Control Definitions           */
    	#include <termios.h> /* POSIX Terminal Control Definitions */
    	#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
    	#include <errno.h>   /* ERROR Number Definitions           */
		#include <stdlib.h>
		#include <string.h>

		int checkisBiggerThanZero_lessThanUpperLimit(int temp ,int upper_limit){

			if( (temp > upper_limit) || (temp < 0) ){
				printf("Wrong Input.\n");

				return 1;
			}else{
				return 0;
			}
		}

		char * insertDataIn_5_6(int data, char * write_buffer){
			if(data < 10){

				char character = data + '0';
				write_buffer[5] = character;

				for (int i = 6; i < 9; i++){
					write_buffer[i] = '0x00';
				}
			}else{
			    char buffer[2];

			    printf("INSERT\n");
				sprintf(buffer, "%d", data);

			    printf(" %c %c \n", buffer[0] ,buffer[1]);
			    write_buffer[5] = buffer[0];
			    write_buffer[6] = buffer[1];
			    write_buffer[7] = '0x00';
			    write_buffer[8] = '0x00';
			}
			return write_buffer;
		}


    	void main(void)
    	{
        int fd;/*File Descriptor*/
		
		printf("\n +----------------------------------+");
		printf("\n |        Serial Port Write         |");
		printf("\n +----------------------------------+");

		/*------------------------------- Opening the Serial Port -------------------------------*/

		/* Change /dev/ttyUSB0 to the one corresponding to your system */

        	fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY | O_NDELAY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
			   						/* O_RDWR Read/Write access to serial port           */
									/* O_NOCTTY - No terminal will control the process   */
									/* O_NDELAY -Non Blocking Mode,Does not care about-  */
									/* -the status of DCD line,Open() returns immediatly */                                        
									
        	if(fd == -1)						/* Error Checking */
            	   printf("\n  Error! in Opening ttyUSB0  ");
        	else
            	   printf("\n  ttyUSB0 Opened Successfully ");

	
		/*---------- Setting the Attributes of the serial port using termios structure --------- */
		
		struct termios SerialPortSettings;	/* Create the structure                          */

		tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */

		cfsetispeed(&SerialPortSettings,B115200); /* Set Read  Speed as 115200                       */
		cfsetospeed(&SerialPortSettings,B115200); /* Set Write Speed as 115200                       */

		SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
		SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
		SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
		SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
	
		SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
		SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
		
		
		SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
		SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

		SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

		if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
		    printf("\n  ERROR ! in Setting attributes");
		else
                    printf("\n  BaudRate = 115200 \n  StopBits = 1 \n  Parity   = none");
			
	        /*------------------------------- Write data to serial port -----------------------------*/
        int size;
    	printf("\n\n please define input array size: ---\n");
    	scanf("%d", &size);
    	
		char write_buffer[size] ;	/* Buffer containing characters to write into port	     */	
		
		int  bytes_written  = 0;  	/* Value for storing the number of bytes written to the port */ 

		bytes_written = write(fd,write_buffer , sizeof(write_buffer));/* use write() to send data to port                                            */
									     /* "fd"                   - file descriptor pointing to the opened serial port */
									     /*	"write_buffer"         - address of the buffer containing data	            */
									     /* "sizeof(write_buffer)" - No of bytes to write                               */	
		
		int index = -1;
    	char c ;
    	scanf("%c", &c);
    	int pwm_temp;
    	int flash_temp;

		while(c != 'q'){ // stop bit

			if( c != '\0') {
			
				if(index == 0){

					write_buffer[0] = '0x00';

				}else{
					if(c == '1' ) {//PWM

						printf("please define pwm rate from 0 to 10\n");
						scanf("%d", &pwm_temp);
						int upper_limit =10;
						if(checkisBiggerThanZero_lessThanUpperLimit(pwm_temp, upper_limit) == 0 ){
							write_buffer[1] = '0x01';
							for(int i = 2 ; i < 5 ; i++ ){
								write_buffer[i] = '0x00';
							}
							//write_buffer = insertDataIn_5_6(pwm_temp);
							*write_buffer = insertDataIn_5_6(pwm_temp, write_buffer);
						}
						if(checkisBiggerThanZero_lessThanUpperLimit(pwm_temp, upper_limit) == 1 ){
							for(int i = 0; i< sizeof(write_buffer) ; i++){
								write_buffer[i] = '0x00';
							}
						}

					}
					else if( c == '2'){ // flash

						printf("please define flash rate from 0 to 10\n");
						scanf("%d", &flash_temp);
						int upper_limit_flash = 60;
						if(checkisBiggerThanZero_lessThanUpperLimit( flash_temp, upper_limit_flash) == 0 ){
							write_buffer[2] = '0x01';
							write_buffer[1] = '0x00' ;
							for(int i = 3 ; i < 5 ; i++ ){
								write_buffer[i] = '0x00';
							}
							//write_buffer = insertDataIn_5_6(flash_temp);
						}

					}
					else if(c == '3'){ // run tg
						int run_tg_pwm_temp;
						int run_tg_flash_temp;
						//write_buffer[3] = '0x01';
						printf("please define pwm rate and flash rate\n");
						printf("first pwm \n");
						scanf("%d", &run_tg_pwm_temp);
						int run_tg_pwm_max = 10;
						int run_tg_flash_max = 60; 

						if(checkisBiggerThanZero_lessThanUpperLimit( run_tg_pwm_temp, run_tg_pwm_max) == 0 ){
							printf("hello from here!!!!!!!!!!\n");
						}


						/*if( (run_tg_pwm_temp > 10) || (run_tg_pwm_temp < 0) ){
							printf("Wrong input\n");
						}else if(run_tg_pwm_temp < 10){
							char character = flash_temp + '0';
							write_buffer[5] = character;
							printf("pwm part is completed. \nPlease define flash rate");
							scanf("%d", &run_tg_flash_temp);


						}else if(run_tg_pwm_temp == 10){
								char buffer[2];

							    sprintf(buffer, "%d", run_tg_pwm_temp);

							    //printf(" %c %c \n", buffer[0] ,buffer[1]);
							    write_buffer[5] = buffer[0];
							    write_buffer[6] = buffer[1];
							    write_buffer[7] = '0x00';
							    write_buffer[8] = '0x00';
							printf("pwm part is completed. \nPlease define flash rate");
							scanf("%d", &run_tg_flash_temp);
						} */


					}
					else if(c == '4'){
						write_buffer[4] = '0x05';
					}
				}
				index++;
			
			}
			printf("\n here it comes\n");

			for(int i = 0; i< sizeof(write_buffer) ; i++){
				printf("%d ", i);
			}
			printf("\n");
			
			for(int i = 0; i < sizeof(write_buffer) ; i++){
				printf("%c ", write_buffer[i]);
			}
			bytes_written = write(fd,write_buffer , sizeof(write_buffer));
			scanf("%c", &c);

		}

		//printf("\n  %d Bytes written to ttyUSB0", bytes_written);
		printf("\n  %d Bytes written to ttyUSB0", bytes_written);
		printf("\n +----------------------------------+\n\n");

		close(fd);/* Close the Serial port */

    	}
