#ifndef SDG1025_H
#define SDG1025_H
#include "visa.h"
#include <windows.h>
#include <string>

class SDG1025
{
    public:
    HINSTANCE hUSBTMCLIB;
    unsigned long m_defaultRM_usbtmc, m_instr_usbtmc;
    unsigned long m_findList_usbtmc;
    unsigned long m_nCount;
    ViStatus status;
    const static int m_Timeout = 700;
    char *pStrout; // Write out data buffer
    unsigned char pStrin[64]; // Read in data buffer
    int len;
    unsigned long nWritten;
    unsigned long  nRead;
    char buffer[256];
    char instrDescriptor[256];

    SDG1025();
    SDG1025(std::string deviceId);
    virtual ~SDG1025();

    void sendCommand(char*, unsigned char*, int, bool);
    std::string float_to_string( float liczba );
    void chanelOnOff(int , bool);
    void load(int, int);

    /** \brief setTypeWave
     *
     * \param int chanel
     * \param std::string = SINE | SQUARE | RAMP | PULSE | NOISE | ARB | DC
     * \return void
     */
    void setTypeWave(int, std::string); //<type>:={SINE, SQUARE, RAMP, PULSE, NOISE, ARB ,DC}
    void setAmplitude(int, float);
    void setOfset(int, float);
    void setFrequency(int, float);
    void setDuty(int, float);


    protected:

    private:
};

#endif // SDG1025_H
