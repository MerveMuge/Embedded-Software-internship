#include <stdio.h>
#include "DS1302.h"

namespace {

const int kCePin   = 5;  // Chip Enable
const int kIoPin   = 6;  // Input/Output
const int kSclkPin = 7;  // Serial Clock

//DS1302 obj
DS1302 rtc(kCePin, kIoPin, kSclkPin);

String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Sunday";
    case Time::kMonday: return "Monday";
    case Time::kTuesday: return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday: return "Thursday";
    case Time::kFriday: return "Friday";
    case Time::kSaturday: return "Saturday";
  }
  return "(unknown day)";
}

void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);
  
  char buf[50];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day.c_str(),
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  Serial.println(buf);
}

}

void setup() {
  Serial.begin(115200);

  rtc.writeProtect(false);
  rtc.halt(false);

  //define start time //year / month / day /-/hour /min / sec /-/ day
  Time t(2019, 8, 20, 14, 57, 50, Time::kTuesday);

  // Set the time on the chip.
  rtc.time(t);
}

void loop() {
  printTime();
  delay(1000);
}
