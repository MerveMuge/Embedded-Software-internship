#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
#include <errno.h>   /* ERROR Number Definitions           */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void clearScreen(){
	const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

int setUp(char *argv){

int fd;
printf("\n +----------------------------------+");
printf("\n |        Serial Port Write         |");
printf("\n +----------------------------------+");

/*------------------------------- Opening the Serial Port -------------------------------*/
/* Change /dev/ttyUSB0 to the one corresponding to your system */

 fd = open( argv ,O_RDWR | O_NOCTTY | O_NDELAY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
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
    printf("\n  BaudRate = 115200 \n  StopBits = 1 \n  Parity   = none \n");
			
/*------------------------------- Write data to serial port -----------------------------*/
    return fd;
}

void print_mainMenu(){
	printf("Main Menu\n");
	printf("---------\n");
    printf("Please give an input which is a number from the options available \n");
  	printf("1. PWM \n");
	printf("2. Flasher \n");
  	printf("3. PWM & Flasher - run together \n");
  	printf("4. Led Selection \n");
}

char menu_page(){
	clearScreen();
  	print_mainMenu();

  	char c_menu;
  	scanf("%c" , &c_menu);

  	//printf("--%c\n", c_menu);
  	return c_menu;
}

char * select_menu_item_and_insert_array(char c_menu, char* write_buffer){

	write_buffer[0] = '*';

	if(c_menu == '1'){
		write_buffer[1] = '1';
	}
	else if( c_menu == '2'){
		write_buffer[1] = '2';
	}
	else if(c_menu == '3'){	
		write_buffer[1] = '3';
	}
	else if(c_menu == '4'){	
		write_buffer[1] = '4';
	}
	return write_buffer;
}

char * is_one_digit_or_more(char * request_temp ,char * result_array, int max_value){

	int second_digit = request_temp[0] - '0';
	int first_digit = request_temp[1] - '0';
	int input_value = ( (second_digit * 10) + first_digit );
	printf("input value is %d\n", input_value );

	if(first_digit < 0){ // if input is have unique digit 
		result_array[0] = '0';
		result_array[1] = request_temp[0];
	}
	else if( input_value <= max_value ){
		result_array[0] = request_temp[0];
		result_array[1] = request_temp[1];		
	}else{
		printf("Wrong input\n");
		result_array[0] = '0';
		result_array[1] = '0';
	}	

	return result_array;
}

char * request_pwm_info(char * request_temp){
	clearScreen();
	printf("Your input should be in the 0-10 range.\n");
  	printf("This input could help adjust the Led' brightness rate.\n");
  	printf("ps. 10 for 100%% rate and 0 for 0%%. \n");
	scanf("%s", request_temp);

	return request_temp;
}

char * upload_pwm_data(char * write_buffer , char * request_temp){

	char result_array[2];
	int second_digit = request_temp[0] - '0';
	int first_digit = request_temp[1] - '0';
	int input_value = ( (second_digit * 10) + first_digit);

	* result_array = * is_one_digit_or_more( request_temp , result_array , 10);
	write_buffer[5] = result_array[0];
	write_buffer[6] = result_array[1];

	return write_buffer;
}

char * request_flash_info(char * request_temp){
	clearScreen();
	printf("Your input would be a flash loop time.\n");
  	printf("ps. if input is 2, the flash period is 2\n");
  	scanf("%s", request_temp);

  	return request_temp;
}

char * upload_flash_info(char * write_buffer , char * request_temp){

	char result_array[2];
	int second_digit = request_temp[0] - '0';
	int first_digit = request_temp[1] - '0';
	int input_value = ( (second_digit * 10) + first_digit );

	* result_array =  * is_one_digit_or_more(request_temp , result_array , 60);
	write_buffer[7] = result_array[0];
	write_buffer[8] = result_array[1];

	return write_buffer;
}


char * request_run_tg_info(char * request_run_tg ){
	clearScreen();
	char request_temp[2];
	char result_array[2];
  	printf("In this option you should define pwm and flash rate\n");
  	printf("First, define pwm rate. (0 - 10)\n");

  	scanf("%s", request_temp);
  	* result_array = * is_one_digit_or_more(request_temp , result_array , 10);

  	request_run_tg[0] = result_array[0];
  	request_run_tg[1] = result_array[1];

  	clearScreen();
  	printf("Second, define flash rate.\n");
  	scanf("%s", request_temp);
  	* result_array = * is_one_digit_or_more(request_temp, result_array ,60);

  	request_run_tg[2] = result_array[0];
  	request_run_tg[3] = result_array[1];
  	clearScreen();

  	return request_run_tg;
}

char * upload_run_tg_info( char * write_buffer , char * request_run_tg ){
	
	write_buffer[5] = request_run_tg[0];
	write_buffer[6] = request_run_tg[1];
	write_buffer[7] = request_run_tg[2];
	write_buffer[8] = request_run_tg[3];

	return write_buffer;
}

char * request_led_switch_info(char * request_temp){
	
	clearScreen();
  	printf("There are 3 led which are numbers; 1,2,3. Select one of them or combinations.\n");
  	scanf("%s", request_temp);

  	return request_temp;
}

char * request_data(char * write_buffer){
	char request_temp[2];
	request_temp[0] = '0';
	request_temp[1] = '0';

	char request_run_tg[4];

	char request_temp_run_tg[4];
	for(int i = 0 ; i < 4 ; i++){
		request_temp_run_tg[i] = '0';
	}

	if(write_buffer[1] == '1'){
		upload_pwm_data(write_buffer, request_pwm_info(request_temp) );
		
	}else if(write_buffer[1] == '2'){
		upload_flash_info(write_buffer, request_flash_info(request_temp) );

	}else if(write_buffer[1] == '3'){
		upload_run_tg_info(write_buffer, request_run_tg_info(request_run_tg) );

	}else if(write_buffer[1] == '4'){
		//request_led_switch_info();
	}
	return write_buffer;

}


void main( int argc, char **argv ) {

	int fd;/*File Descriptor*/
	fd = setUp(argv[1]);

	char write_buffer[10]; //data array

	for(int i = 0; i < 10 ; i++){
		write_buffer[i] = '0';
	}

	bool isStarted = false;
	int index = 0;

	* write_buffer = * select_menu_item_and_insert_array(menu_page(), write_buffer);
	* write_buffer = * request_data(write_buffer);

	printf("here\n");
	for(int i = 0; i < 10 ; i++) {
		printf("%c\n",write_buffer[i]);
	}

	//menu_page();
	/*while(1){
		char c_temp;
		scanf("%c", &c_temp);		
		if( c_temp == 'q'){
			break;
		 }
		else if( (c_temp != '\n') && (c_temp != '\t') && (c_temp != '\r') ){ //check escape sequences
			if( (index == 0) && (!isStarted) ){
				write_buffer[index++] = '*';
				write_buffer[index++] = c_temp;
				isStarted = true;
			}else if( (index == 8) && (isStarted) ){
				write_buffer[index++] = c_temp;
				write_buffer[index] = '&';
				index = 0;
				isStarted = false;
				//process
				for(int i = 0; i < 10 ; i++){
					printf("%c \n", write_buffer[i]);
				}
				//process
			}else if(isStarted){
				write_buffer[index++] = c_temp;
			}
		}
	}*/

}
