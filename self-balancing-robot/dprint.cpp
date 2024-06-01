#include "dprint.h"

extern bool debugSetting;

size_t nothing() {
  return 0;
}

void dprintln(const char* message) {
  debugSetting ? Serial.println(message) : nothing();
}

void dprintln(int message) {
  debugSetting ? Serial.println(message) : nothing();
}

void dprintln(float message) {
  debugSetting ? Serial.println(message, 2) : nothing();  // Print the float with 2 decimal places
}

void dprintln(float message, int digits) {
  debugSetting ? Serial.println(message, digits) : nothing();  
}

void dprintln(double message) {
  debugSetting ? Serial.println(message, 2) : nothing();  // Print the double with 2 decimal places
}

void dprintln(double message, int digits) {
  debugSetting ? Serial.println(message, digits) : nothing();  
}

void dprintln(unsigned int message) {
  debugSetting ? Serial.println(message) : nothing();
}

void dprintln(long message) {
  debugSetting ? Serial.println(message) : nothing();
}

void dprintln(unsigned long message) {
  debugSetting ? Serial.println(message) : nothing();
}

void dprintln(byte message) {
  debugSetting ? Serial.println(message) : nothing();
}

void dprintln(bool message) {
  debugSetting ? (Serial.println(message ? "True" : "False")) : nothing();
}

void dprintln(uint64_t message){
  debugSetting ? Serial.print(message) : nothing();
}


//Same without newline
void dprint(const char* message) {
  debugSetting ? Serial.print(message) : nothing();
}

void dprint(int message) {
  debugSetting ? Serial.print(message) : nothing();
}

void dprint(float message) {
  debugSetting ? Serial.print(message, 2) : nothing();  // Print the float with 2 decimal places
}

void dprint(float message, int digits) {
  debugSetting ? Serial.print(message, digits) : nothing();  // Print the float with 2 decimal places
}

void dprint(double message) {
  debugSetting ? Serial.print(message, 2) : nothing();  // Print the double with 2 decimal places
}

void dprint(double message, int digits) {
  debugSetting ? Serial.print(message, digits) : nothing();  // Print the double with 2 decimal places
}

void dprint(unsigned int message) {
  debugSetting ? Serial.print(message) : nothing();
}

void dprint(long message) {
  debugSetting ? Serial.print(message) : nothing();
}

void dprint(unsigned long message) {
  debugSetting ? Serial.print(message) : nothing();
}

void dprint(byte message) {
  debugSetting ? Serial.print(message) : nothing();
}

void dprint(bool message) {
  debugSetting ? (Serial.print(message ? "True" : "False")) : nothing();
}

void dprint(uint64_t message){
  debugSetting ? Serial.print(message) : nothing();
}