#include "test.h"
#include<windows.h>
#include<iostream>
using namespace std;

double PCFreq = 0.0;
__int64 CounterStart = 0;

void Test::StartCounter() {
    LARGE_INTEGER li;
    if( !QueryPerformanceFrequency( & li ) )
        cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double( li.QuadPart ) / 1000000.0;

    QueryPerformanceCounter( & li );
    CounterStart = li.QuadPart;
}
double Test::GetCounter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter( & li );
    return double( li.QuadPart - CounterStart ) / PCFreq;
}