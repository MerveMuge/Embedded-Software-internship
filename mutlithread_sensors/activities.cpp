#include "activities.h"

Activities::Activities(std::array <char, BUFFER_SIZE > write_buff ,Serialport *serial, Utils *utils) {
    m_write_buff = write_buff;
    m_serial = serial;
    m_utils = utils;
}

std::array < char, BUFFER_SIZE> Activities::getWrite_buff(){
    return m_write_buff;
}

void Activities::setWrite_buff(std::array < char, BUFFER_SIZE > writeBuff ){
    m_write_buff = writeBuff;
}

void Activities::quit_menu(){
    m_utils->cleanScreen();
    exit(1);
    std::system("clear");
}

std::array < char, BUFFER_SIZE > Activities::nextInput(){
    char nextInput;
    std::cin >>nextInput;
    if( (nextInput == 'q') || (nextInput == 'Q') ){
        quit_menu();
    }
    else if( (nextInput == 'r') || (nextInput == 'R') ){
        m_write_buff.at(13) = '&';
        //m_write_buff.at(13) = '0';
        //m_serial->setIsPrint(true);
        m_write_buff = reload_menu();
    }
    else if((nextInput == 's') || (nextInput == 'S')){

        m_write_buff.at(13)= '&';
        m_serial->writeToPort(m_write_buff);
        m_write_buff.at(13)= '0';

        m_write_buff = setting_menu();
    }
    return  m_write_buff;
}

std::array < char, BUFFER_SIZE > Activities::settingMenu_LDR_maxMinValues(){
    int ldrMinValue;
    int ldrMaxValue;
    Utils * utils = new Utils(m_write_buff);

    utils->cleanScreen();
    m_write_buff.at(1) = 's';
    std::cout << "Please define a LDR max value [0, 999] then min value[0, 999]\n";
    std::cout << "Max value[0,999] ";
    std::cin >> ldrMaxValue;

    utils->cleanScreen();
    std::cout << "Max value is ";
    std::cout << ldrMaxValue ;
    std::cout << ".\nPlease type a min value[0, 999] ";
    std::cin >> ldrMinValue;

    utils->cleanScreen();
    std::cout << "Q- Quit,  R- Reload,   S- Setting   \n";
    std::cout << "Max value is, ";
    std::cout << ldrMaxValue ;
    std::cout << "\nMin value is, ";
    std::cout << ldrMinValue  ;
    std::cout << " .\n";

    int totalDigit_maxValue = utils->countDigit(ldrMaxValue);
    int totalDigit_minValue = utils->countDigit(ldrMinValue);

    auto maxValue_string = std::to_string(ldrMaxValue);
    auto minValue_string = std::to_string(ldrMinValue);

    int lenghtMax = maxValue_string.length();
    int lenghtMin = minValue_string.length();

    char maxValue_arrayC[lenghtMax];
    char minValue_arrayC[lenghtMin];

    strcpy(maxValue_arrayC, maxValue_string.c_str());
    strcpy(minValue_arrayC, minValue_string.c_str());

    m_write_buff =  utils->writeThreeDigitNumberToBuffer(6, totalDigit_maxValue, maxValue_arrayC);
    m_write_buff =  utils->writeThreeDigitNumberToBuffer(9, totalDigit_minValue, minValue_arrayC) ;

    m_write_buff = nextInput();

    return m_write_buff;
}

std::array < char, BUFFER_SIZE > Activities::settingMenu_ledStatus(){

    m_utils->cleanScreen();
    std::cout << "Q- Quit,  R- Reload,   S- Setting   \n";
    std::cout << "Please, define the led mode.\nH/h -high, L/l -low, P/p- pwm, F/f -flash\n";
    char ledStatus;
    std::cin >> ledStatus;
    if( (ledStatus == 'l') || (ledStatus == 'L')
            || (ledStatus == 'H') || (ledStatus == 'h')
            || (ledStatus == 'P') || (ledStatus == 'p')
            || (ledStatus == 'F') || (ledStatus == 'f') ){
        ledStatus = tolower(ledStatus);
        m_write_buff.at(1) = 's';
        m_write_buff.at(5) = ledStatus;
    }
    m_write_buff.at(13) = '&';
    m_serial->writeToPort(m_write_buff);
    m_write_buff.at(13) = '0';

    m_write_buff = nextInput();
    return m_write_buff;
}

std::array < char, BUFFER_SIZE > Activities::settingMenu_relay(){

    m_utils->cleanScreen();
    std::cout << "Please, define the period frequency. [0, 999] \n";
    m_write_buff.at(1) = 's';
    int relayPeriod = 0;
    std::cin >> relayPeriod;

    int totalDigitOfTheInput = m_utils->countDigit(relayPeriod);

    auto string_relayPeriod = std::to_string(relayPeriod); //convert int to string
    int lenghtRelayPeriodS = string_relayPeriod.length();

    char relayPeriod_arrayC[lenghtRelayPeriodS];
    strcpy(relayPeriod_arrayC, string_relayPeriod.c_str());

    m_write_buff = m_utils->writeThreeDigitNumberToBuffer(2, totalDigitOfTheInput, relayPeriod_arrayC);
    m_utils->cleanScreen();
    std::cout << "Q- Quit,  R- Reload,   S- Setting   \n";
    std::cout <<"Relay value is updated. New relay frequency is, ";
    std::cout << relayPeriod;
    std::cout << "\n";

    m_write_buff = nextInput();

    return m_write_buff;
}

std::array < char, BUFFER_SIZE > Activities::setting_menu(){
    m_serial->setIsPrint(false);
    char setting_menu_input;
    m_utils->cleanScreen();
    std::cout << "Q- Quit,  R- Reload,   S- Setting   \n";
    std::cout << "1. Relay Frequency\n";
    std::cout << "2. Led Status(High, Low, PWM, Flash) \n";
    std::cout << "3. LDR max, min values\n";

    m_write_buff.at(0)='*';
    m_write_buff.at(1) = 's';
    m_write_buff.at(13) = '&';

    std::cin >> setting_menu_input;

    if(setting_menu_input == '1'){
        std::cout << "relay\n";
        m_write_buff.at(12) = '1';
        m_write_buff = settingMenu_relay();
    }
    else if(setting_menu_input == '2'){
        std::cout << "led status\n";
        m_write_buff.at(12) = '2';
        m_write_buff = settingMenu_ledStatus();
    }
    else if(setting_menu_input == '3'){
        std::cout << "LDR max-min\n";
        m_write_buff.at(12) = '3';
        m_write_buff = settingMenu_LDR_maxMinValues();

    }else if( (setting_menu_input == 'Q') || (setting_menu_input == 'q')){
        exit(1);
    }else {
        std::cout << "Wrong input\n";
        exit(1);
    }
    return m_write_buff;
}

std::array < char, BUFFER_SIZE > Activities::reload_menu(){

    m_utils->cleanScreen();
    char reloadInput;
    std::cout << "Q- Quit,  R- Reload,   S- Setting   \n";

    m_write_buff.at(1) = 'r';
    m_write_buff.at(13) = '&';

    m_serial->setIsPrint(true);
    m_serial->writeToPort(m_write_buff);
    //m_serial->setIsPrint(false);
    //m_serial->run();
    //m_serial->setIsPrint(false);

    while(  m_write_buff.at(1) == 'r' ){
        m_write_buff = nextInput();

        std::cin >> reloadInput ;
        if( (reloadInput == 'q') || (reloadInput == 'Q') ){
            (m_write_buff).at(1) = 'q';
        }else if( (reloadInput == 'r') || (reloadInput == 'R') ){
            (m_write_buff).at(1) = 'r';
        }else if( (reloadInput == 's') || (reloadInput == 'S') ){
            (m_write_buff).at(1) = 's';
        }
    }
    return m_write_buff;
}

std::array < char, BUFFER_SIZE> Activities::menu(){

    std::cout << "Q- Quit,  R- Reload,   S- Setting   \n";
    char menu_char;
    m_write_buff.at(0) = '*';

    while(m_write_buff.at(13) != '&'){
        std::cin >> menu_char;

        if( (menu_char == 'q') || (menu_char == 'Q')  ){
            m_write_buff.at(13) = '&';
            quit_menu();
        }
        else if( ( menu_char == 'r' ) || ( menu_char == 'R' ) ){

            m_write_buff = reload_menu();
        }
        else if ( (menu_char == 's') || (menu_char == 'S') ) {

            m_write_buff = setting_menu();
        }
        else {
            std::cout << "Wrong Input\n";
            m_write_buff.at(13) = '&';
        }
    }
    return m_write_buff;
}
