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
		#include <unistd.h>

		void clearScreen(){
		  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
		  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
		}

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

				write_buffer[5] = '0';
				char character = data + '0';
				write_buffer[6] = character;


				for (int i = 7; i < 9; i++){
					write_buffer[i] = '0';
				}
			}
			else{
			    char buffer[2];

				sprintf(buffer, "%d", data);

			    write_buffer[5] = buffer[0];
			    write_buffer[6] = buffer[1];
			    write_buffer[7] = '0';
			    write_buffer[8] = '0';
			}
			return write_buffer;
		}

		char * insertDataIn_7_8(int data, char * write_buffer){
			if(data < 10){

				write_buffer[7] = '0';

				char character = data + '0';
				write_buffer[8] = character;
				
			}
			else{
			    char buffer[2];
				sprintf(buffer, "%d", data);

			    write_buffer[7] = buffer[0];
			    write_buffer[8] = buffer[1];
			  
			}
			return write_buffer;
		}

		char * returnAllElementsZeroArray(char * write_buffer){

			write_buffer[0] = '*';
			for(int i = 1; i < sizeof(write_buffer) ; i++){
				write_buffer[i] = '0';
			}
				return write_buffer;
		}

		int findTotalDigitInNumber(int number){
			int totalDigits = 0;
			
			while(number!=0){
			  number = number/10;
			  totalDigits ++;
			}
			//printf("Total digits in the number is %d\n",totalDigits);
			return totalDigits;
		}

		int setUp(){

		int fd;
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
                return fd;
		}

		char * pwm(char *  write_buffer){
			int pwm_temp;

			printf("please define pwm rate from 0 to 10\n");
			scanf("%d", &pwm_temp);
			int upper_limit =10;

			if(checkisBiggerThanZero_lessThanUpperLimit(pwm_temp, upper_limit) == 0 ){
				write_buffer[1] = '1';
				for(int i = 2 ; i < 5 ; i++ ){
					write_buffer[i] = '0';
				}
				*write_buffer = *insertDataIn_5_6(pwm_temp, write_buffer);
			}else{
				*write_buffer = *returnAllElementsZeroArray(write_buffer);
			}
			return write_buffer;

		}

		char * flash(char * write_buffer){
			int flash_temp;

			printf("please define flash rate from 1 to 60\n");
			scanf("%d", &flash_temp);
			int upper_limit_flash = 60;
			if(checkisBiggerThanZero_lessThanUpperLimit( flash_temp, upper_limit_flash) == 0 ){
				write_buffer[2] = '1';
				write_buffer[1] = '0';
			for(int i = 3 ; i < 5 ; i++ ){
				write_buffer[i] = '0';
			}
							
			*write_buffer = *insertDataIn_5_6(flash_temp, write_buffer);
			}else{
				*write_buffer = *returnAllElementsZeroArray(write_buffer);
			}
			return write_buffer;
		}

		char * run_tg(char * write_buffer){
			int run_tg_pwm_temp;
			int run_tg_flash_temp;
						//write_buffer[3] = '0x01';
			printf("please define pwm rate and flash rate\n");
			printf("first pwm \n");
			scanf("%d", &run_tg_pwm_temp);
			int run_tg_pwm_max = 10;
			int run_tg_flash_max = 60; 

			if(checkisBiggerThanZero_lessThanUpperLimit( run_tg_pwm_temp, run_tg_pwm_max) == 0 ){
							
				write_buffer[1] = '0';
				write_buffer[2] = '0';
				write_buffer[3] = '1'; // run tg is working
				write_buffer[4] = '0';
				*write_buffer = *insertDataIn_5_6(run_tg_pwm_temp, write_buffer);

				printf("please define flash rate max 60, min 1\n");
				scanf("%d",&run_tg_flash_temp);

				if(checkisBiggerThanZero_lessThanUpperLimit(run_tg_flash_temp, run_tg_flash_max) == 0 ){
					*write_buffer = *insertDataIn_7_8(run_tg_flash_temp , write_buffer);
				}else{
					*write_buffer = *returnAllElementsZeroArray(write_buffer);
				}
			}else{
				*write_buffer = *returnAllElementsZeroArray(write_buffer);
			}

			return write_buffer;
		}

		char * led_switch( char * write_buffer){
			int led_selection_temp;
			
			printf("In this section you will have 3 LEDs. \n Your input will 1 ,2, 3 or combination of this values.");
			printf("please define your input.\n");
			scanf("%d", &led_selection_temp);
			int totalDigit = findTotalDigitInNumber(led_selection_temp);
			if(totalDigit > 3){
				printf("totalDigit %d\n", totalDigit );
				printf("Wrong Input. Your input value could be have max 3 digits.\n");
			}else if(totalDigit < 1){
				printf("Something goes wrong! \n Please define input properly and try again. \n" );
			}else{
				*write_buffer = *returnAllElementsZeroArray(write_buffer);

				write_buffer[4] = '1'; //LED switch is work
				write_buffer[1] = '0';
				write_buffer[2] = '0';
				write_buffer[3] = '0';
				write_buffer[8] = '0';
				char buffer[3]; 
				sprintf(buffer, "%d", led_selection_temp); 

				for (int i = 0; i < totalDigit ; i++){
					if(buffer[i] == '1'){
						write_buffer[5] = '1';
					}
					else if(buffer[i] == '2'){
						write_buffer[6] = '1';
					}
					else if(buffer[i] == '3'){
						write_buffer[7] = '1';
					}
				}
			}
			return write_buffer;
		}

    	void main(void){

    	int odd =0;
        int fd;/*File Descriptor*/
    	fd = setUp();
		
        int size;
    	printf("\n\n please define input array size: ---\n");
    	scanf("%d", &size);
    	
		char write_buffer[size] ;	/* Buffer containing characters to write into port	     */	
		int  bytes_written  = 0;  	/* Value for storing the number of bytes written to the port */ 
		//bytes_written = write(fd,write_buffer , sizeof(write_buffer));/* use write() to send data to port                                            */
																      /* "fd"                   - file descriptor pointing to the opened serial port */
																      /*	"write_buffer"         - address of the buffer containing data	            */
																      /* "sizeof(write_buffer)" - No of bytes to write                               */				
		int index = -1;
    	char c ;
    	scanf("%c", &c);
    	* write_buffer = * returnAllElementsZeroArray(write_buffer);

		while(c != 'q'){ // stop bit
			odd++;

			if( c != '\0') {
				if(index == 0){
					write_buffer[0] = '*';

				}else{
					if(c == '1' ) {//PWM
						clearScreen();
						*write_buffer = *pwm(write_buffer);
					}
					else if( c == '2'){ // flash
						clearScreen();
						*write_buffer = *flash(write_buffer);
					}
					else if(c == '3'){ // run tg
						clearScreen();
						*write_buffer = *run_tg(write_buffer);
					}
					else if(c == '4'){ //led selection
						clearScreen();
						*write_buffer = *led_switch(write_buffer);
					}
			
					bytes_written = write(fd,write_buffer , sizeof(write_buffer));
			
				}
				index++;


			}
			if(odd % 2 == 1 ){
				printf("\n here it comes\n");

				for(int i = 0; i< sizeof(write_buffer) ; i++){ //print index numbers
					printf("%d ", i);
				}
				printf("\n");
				
				for(int i = 0; i < sizeof(write_buffer) ; i++){
					printf("%c ", write_buffer[i]);
				}
			}

		//	bytes_written = write(fd,write_buffer , sizeof(write_buffer));
			scanf("%c", &c);

		}
		//printf("\n  %d Bytes written to ttyUSB0", bytes_written);
		printf("\n  %d Bytes written to ttyUSB0", bytes_written);
		printf("\n +----------------------------------+\n\n");

		close(fd);/* Close the Serial port */
    	}
