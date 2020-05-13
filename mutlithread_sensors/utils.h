#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <unistd.h>
#include <string>

#include <iterator>
#include <algorithm>
#include <array>
#include <constants.h>

class Utils
{
public:

    Utils( std::array <char, BUFFER_SIZE > write_buff);
    void cleanScreen();
    std::array <char, BUFFER_SIZE > writeThreeDigitNumberToBuffer(int startIndex, int totalDigit, char* input);
    int countDigit(long long n);
    void setWrite_buff(std::array <char,BUFFER_SIZE> writeBuff );
    std::array <char, BUFFER_SIZE> getWrite_buff();
private:
    std::array <char, BUFFER_SIZE > m_write_buff;
};

#endif // UTILS_H
