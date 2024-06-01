#include <stdint.h>
#ifndef dprint_h
#define dprint_h
#include <Arduino.h>

size_t nothing();

void dprint(const char* message);

void dprint(int message);

void dprint(float message);

void dprint(float message, int digits);

void dprint(double message);

void dprint(double message, int digits);

void dprint(unsigned int message);

void dprint(long message);

void dprint(unsigned long message);

void dprint(byte message);

void dprint(bool message);

void dprint(uint64_t message);


//Same without newline
void dprintln(const char* message);

void dprintln(int message);

void dprintln(float message);

void dprintln(float message, int digits);

void dprintln(double message);

void dprintln(double message, int digits);

void dprintln(unsigned int message);

void dprintln(long message);

void dprintln(unsigned long message);

void dprintln(byte message);

void dprintln(bool message);

void dprintln(uint64_t message);



#endif