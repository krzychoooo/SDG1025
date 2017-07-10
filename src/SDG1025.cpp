#include "../include/SDG1025.h"
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;


SDG1025::SDG1025()
{
    //ctor
}

std::string SDG1025::float_to_string( float liczba )
{
    std::stringstream floatToStr;
    std::string str;

    floatToStr << liczba;
    floatToStr >> str;
    floatToStr.clear();

    return str;
}

SDG1025::SDG1025(std::string miernikId) {
    //typedef int32_t ViStatus;
    //HINSTANCE hUSBTMCLIB;

    //hUSBTMCLIB = LoadLibrary("NIvisa64.dll");
    hUSBTMCLIB = LoadLibrary("visa32.dll");


    signed long (__stdcall * PviOpenDefaultRM_usb) (unsigned long *vi);
    signed long (__stdcall * PviFindRsrc_usb) (unsigned long sesn, char *expr, unsigned long *vi,
            unsigned long *retCnt, char far desc[]);
    signed long (__stdcall * PviOpen_usb) (unsigned long sesn, char *name, unsigned long mode,
            unsigned long timeout, unsigned long *vi);
    signed long (__stdcall * PviClose_usb) (unsigned long vi);
    signed long (__stdcall * PviWrite_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);
    signed long (__stdcall * PviRead_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);
    signed long (__stdcall * PviSetAttribute_usb) (unsigned long vi, unsigned long viAttr, unsigned long attrstat);


    PviOpenDefaultRM_usb = (signed long (__stdcall*) (unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viOpenDefaultRM");
    PviFindRsrc_usb = (signed long (__stdcall*) (unsigned long, char*, unsigned long*,
            unsigned long*, char[]))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viFindRsrc");
    PviClose_usb = (signed long (__stdcall*) (unsigned long))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viClose");
    PviOpen_usb = (signed long (__stdcall*) (unsigned long, char*, unsigned long,
            unsigned long, unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viOpen");
    PviWrite_usb	      = (signed long (__stdcall*)(unsigned long, unsigned char*, unsigned long,
                                unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR)"viWrite");
    PviRead_usb			  = (signed long (__stdcall*)(unsigned long, unsigned char*, unsigned long,
                                unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR)"viRead");
    PviSetAttribute_usb = (signed long (__stdcall*) (unsigned long, unsigned long, unsigned long))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viSetAttribute");


    if (PviOpenDefaultRM_usb == NULL ||
            PviFindRsrc_usb == NULL ||
            PviClose_usb == NULL ||
            PviOpen_usb == NULL ||
            PviWrite_usb == NULL ||
            PviRead_usb == NULL ||
            PviSetAttribute_usb == NULL
            ) {
        //FreeLibrary(hUSBTMCLIB);
        //printf("%x,%x,%x,%x,%x,%x,%x,",PviOpenDefaultRM_usb, PviFindRsrc_usb, PviClose_usb, PviOpen_usb, PviWrite_usb, PviRead_usb, PviSetAttribute_usb);
        //hUSBTMCLIB = NULL;
        //MessageBox(NULL, "NIVISA for USBTMC library not ready.", "Multimeter device test", MB_OK);
        printf("NIVISA for USBTMC library not ready.");
    }


    // Open Device -- Resource Manager
    status = PviOpenDefaultRM_usb(&m_defaultRM_usbtmc);
    if (status < 0L) {
        PviClose_usb(m_defaultRM_usbtmc);
        hUSBTMCLIB = NULL;
        m_defaultRM_usbtmc = 0;
        //		MessageBox(NULL, "USBTMC resource not found.", "Multimeter device test", MB_OK);
        printf("USBTMC resource not found.");
        //Sleep(5000);
    } else {
        //char id1[]="USB[0-9]*::0x164E::0x0DAD::?*INSTR";
        //char id1[]="USB[0-9]*::"+miernikId+"::?*INSTR";

        //printf("string-%s",miernikId.c_str());
        char * idUSB = new char[miernikId.size()+1];
        strcpy(idUSB, miernikId.c_str());
        status = PviFindRsrc_usb(m_defaultRM_usbtmc, idUSB, &m_findList_usbtmc, &m_nCount, instrDescriptor);

        if (status < 0L) {
            char id2[]="USB[0-9]*::5710::3501::?*INSTR";
            status = PviFindRsrc_usb(m_defaultRM_usbtmc, id2, &m_findList_usbtmc, &m_nCount, instrDescriptor);
            if (status < 0L) {
                PviClose_usb(m_defaultRM_usbtmc);
                hUSBTMCLIB = NULL;
                m_defaultRM_usbtmc = 0;
            } else {
                PviOpen_usb(m_defaultRM_usbtmc, instrDescriptor, 0, 0, &m_instr_usbtmc);
                status = PviSetAttribute_usb(m_instr_usbtmc, VI_ATTR_TMO_VALUE, m_Timeout);
            }
        } else {
            PviOpen_usb(m_defaultRM_usbtmc, instrDescriptor, 0, 0, &m_instr_usbtmc);
            status = PviSetAttribute_usb(m_instr_usbtmc, VI_ATTR_TMO_VALUE, m_Timeout);
            //Sleep(5000);
            //printf("miernik OK\n\r");
        }
    }

    if (!hUSBTMCLIB) {
        printf("Multimeter device connect failed.\n");
        Sleep(5000);
    }
}


void SDG1025::sendCommand(char* command,unsigned char* requestBuffer , int commandSize, bool readDevice){

    signed long (__stdcall * PviWrite_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);
    signed long (__stdcall * PviRead_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);

    PviWrite_usb = (signed long (__stdcall*) (unsigned long, unsigned char*, unsigned long,
            unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viWrite");
    PviRead_usb = (signed long (__stdcall*) (unsigned long, unsigned char*, unsigned long,
            unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viRead");

    // Send read command

    status = PviWrite_usb(m_instr_usbtmc, (unsigned char *) command, commandSize, &nWritten);
    Sleep(80);
    //cout<<"nW="<<nWritten<<" "<<endl;
    //printf(" output : READ?\n");

    if(readDevice){
        status = PviRead_usb(m_instr_usbtmc, requestBuffer, 64, &nRead);
        int i=0;
        while(status != 0){
            Sleep(80);
            status = PviWrite_usb(m_instr_usbtmc, (unsigned char *) command, commandSize, &nWritten);
            Sleep(80);
            //cout<<"PviRead_usb_start ";
            status = PviRead_usb(m_instr_usbtmc, requestBuffer, 64, &nRead);
            //cout<<"PviRead_usb_end ";
            Sleep(50);
            if(i++>100)break;
        }
        cout<<"i="<<i;
        requestBuffer[nRead] = '\0';
    }
}


void SDG1025::chanelOnOff(int chanel, bool onOff){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    /*Select CHANEL*/
	command = "C";
	ss << chanel;
	std::string ch = ss.str();
	command = command + ch + ":OUTP ";

	if(onOff) command = command + "ON"; else command = command + "OFF";
	sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}

void SDG1025::load(int chanel, int ohm){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    /*Select CHANEL*/
	command = "C";
	ss << chanel;
	std::string ch = ss.str();

	command = command + ch + ":OUTP LOAD,";

	ss.str(""); ss.clear();
	ss << ohm;
	ch = ss.str();

	command = command + ch;
	cout<<command<<endl;
	sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}


void SDG1025::setTypeWave(int chanel, std::string type){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    /*Select CHANEL*/
	command = "C";
	ss << chanel;
	std::string ch = ss.str();
	command = command + ch + ":BSWV WVTP,";

	command = command + type;
	cout<<command<<endl;
	sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}

void SDG1025::setAmplitude(int chanel, float amplitude){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    /*Select CHANEL*/
	command = "C";
	ss << chanel;
	std::string ch = ss.str();
	command = command + ch + ":BSWV AMP,";

	amplitude = round(amplitude*10000)/10000;
    ss.str(""); ss.clear();
    ss<<fixed<<setprecision(4)<<amplitude;
    std::string value = ss.str();

    command = command + value;

	cout<<command<<endl;
	sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}

void SDG1025::setOfset(int chanel, float ofset){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    /*Select CHANEL*/
	command = "C";
	ss << chanel;
	std::string ch = ss.str();
	command = command + ch + ":BSWV OFST,";

	ofset = round(ofset*10000)/10000;
    ss.str(""); ss.clear();
    ss<<fixed<<setprecision(4)<<ofset;
    std::string value = ss.str();

    command = command + value;

	cout<<command<<endl;
	sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}


void SDG1025::setFrequency(int chanel, float frequency){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    /*Select CHANEL*/
	command = "C";
	ss << chanel;
	std::string ch = ss.str();
	command = command + ch + ":BSWV FRQ,";

	frequency = round(frequency*10000000)/10000000;
    ss.str(""); ss.clear();
    ss<<fixed<<setprecision(7)<<frequency;
    std::string value = ss.str();

    command = command + value;

	cout<<command<<endl;
	sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}

void SDG1025::setDuty(int chanel, float duty){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    /*Select CHANEL*/
	command = "C";
	ss << chanel;
	std::string ch = ss.str();
	command = command + ch + ":BSWV DUTY,";

	duty = round(duty*10)/10;
    ss.str(""); ss.clear();
    ss<<fixed<<setprecision(1)<<duty;
    std::string value = ss.str();

    command = command + value;

	cout<<command<<endl;
	sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}


SDG1025::~SDG1025()
{
    //dtor
}
