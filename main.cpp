// Psk.cpp : Defines the entry point for the application.
//

#include "Psk.h"
#include <fstream>

using namespace std;

int main()
{
	PSK psk = PSK("./ABombsite_0_BombWires.pskx");
	psk.Deserialize();
	psk.close();
	return 0;
}
