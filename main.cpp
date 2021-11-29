// Psk.cpp : Defines the entry point for the application.
//

#include "Psk.h"
#include <fstream>

using namespace std;

int main()
{
	PSK psk = PSK("./SK_M_XLG_Jim_01_Export_LOD0.psk");
	psk.Deserialize();
	psk.close();
	return 0;
}
