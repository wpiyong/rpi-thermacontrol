/*
 * fancontrol.cpp
 *
 *  Created on: Mar 2, 2018
 *      Author: dev03
 */




extern "C" {
#include <wiringPi.h>
}

#include "lib/hardwaremanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//#include <stdio.h>
#include <termios.h>
#include <unistd.h>
char getch() {
    char buf = 0;
    struct termios old = { 0 };
    fflush(stdout);
    if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
    old.c_lflag    &= ~ICANON;   // local modes = Non Canonical mode
    old.c_lflag    &= ~ECHO;     // local modes = Disable echo.
    old.c_cc[VMIN]  = 1;         // control chars (MIN value) = 1
    old.c_cc[VTIME] = 0;         // control chars (TIME value) = 0 (No time)
    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0) perror("read()");
    old.c_lflag    |= ICANON;    // local modes = Canonical mode
    old.c_lflag    |= ECHO;      // local modes = Enable echo.
    if (tcsetattr(0, TCSADRAIN, &old) < 0) perror ("tcsetattr ~ICANON");
    return buf;
 }

using namespace Dsr;

int main (void)
{
	Dsr::HardwareManager* man = Dsr::HardwareManager::SharedInstance();
	//::std::this_thread::sleep_for(::std::chrono::seconds(200));

	char c;
	do
	{
		c = getch();  //getch is used so that the enter key need not be pressed and the
		::std::this_thread::sleep_for(::std::chrono::seconds(1));
	}while(c!='\r');

	man->thermalTask->abort();
	delete man;
	return 0 ;
}
