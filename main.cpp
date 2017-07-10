#include <iostream>
#include "SDG1025.h"
#include <windows.h>


using namespace std;

int main()
{
    setlocale(LC_ALL,"");
    cout << "Testy SDG1025" << endl;

    SDG1025 generator = SDG1025("USB0::0xF4ED::0xEE3A::SDG100E2150356::INSTR");
    generator.load(1,50);
    generator.setTypeWave(1, "SINE");
    generator.setAmplitude(1, 4);
    generator.setOfset(1, 0);
    generator.setFrequency(1, 0.567891);
    //generator.setDuty(1, 96.92);
    generator.chanelOnOff(1, true);

    for(float i=10000; i<15000; i=i+100){
        generator.setFrequency(1, i);
        Sleep(2000);
        cout<<i<<endl;
    }

    return 0;
}
