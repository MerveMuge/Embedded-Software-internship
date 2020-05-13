#include "utils.h"
#include<iostream>
#include <stdio.h> // standard input / output functions
#include <string.h> // string function definitions

using namespace std;
Utils::Utils(std::array <char, BUFFER_SIZE > write_buff){
    m_write_buff = write_buff;
}

std::array <char, BUFFER_SIZE> Utils::getWrite_buff(){
    return m_write_buff;
}

void Utils::setWrite_buff(std::array < char, BUFFER_SIZE > writeBuff ){
    m_write_buff = writeBuff;
}

void Utils::cleanScreen(){
    cout << "\x1B[2J"; // Clear the screen
    cout << "\x1B[0;0H"; // place cursor at home position
}


std::array <char, BUFFER_SIZE>  Utils::writeThreeDigitNumberToBuffer(int startIndex, int totalDigit, char* input) {

    if(totalDigit == 3){
        m_write_buff.at(startIndex) = input[0] ;
        m_write_buff.at(startIndex+1) = input[1];
        m_write_buff.at(startIndex+2) = input[2];
    }
    else if(totalDigit == 2){
        m_write_buff.at(startIndex) = '0';
        m_write_buff.at(startIndex+1) = input[0];
        m_write_buff.at(startIndex+2) = input[1];
    }
    else if(totalDigit == 1){
        m_write_buff.at(startIndex) = '0';
        m_write_buff.at(startIndex+1) = '0';
        m_write_buff.at(startIndex+2) = input[0];
    }
    return m_write_buff;
}

int Utils::countDigit(long long n)
{
    int count = 0;
    while (n != 0) {
        n = n / 10;
        ++count;
    }
    return count;
}
