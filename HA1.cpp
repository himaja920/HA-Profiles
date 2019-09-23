/* 
 * File:   main.cpp
 * Author: swetha
 *
 * Created on 11 July, 2013, 2:33 PM
 */

#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <wchar.h>
#include <net/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/file.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sched.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <list>
#include <signal.h>
#include <libAlmond.h>
#include <libAlmondHA.h>
#include "main.h"
#include <unicode.h>
#include <Logger.h>
#include <libAlmondHA_Private.h>
#include "almond_timer.h"
#include "almondList.h"
#include <math.h>
#include <bits/stdc++.h>
using namespace std;

#define RGB(r,g,b)              (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //5 red | 6 green | 5 blue
#define APP_EX "hadata"
#define APP_ID "4c09"

void theCallback(Device* notifiedDevice);
void theEventCallback(Event *e);
void btnCaptureOnTouch(void);
void ImageCountOnTouch(void);
void ResolutionOnTouch(void);
void btndeativeOnTouch(void);
extern int roundfun(float num);
void LIST(void);
void Value(void);


Memory mem;
Languages lan;
Logger *logger;
DeviceList dL;
Device * notifiedDevice;
Event *e;
Screen *connected_home;
WidgetType type;
DevType contDev = UnknownDeviceType;
list<Device>::iterator it;

siteInfo *IREventData, *deviceLocation;
almondList *Irlst;

lcdImage readLCDArea, /*readLCDArea2,*/ screenData;
ImagePNG sensorImg, pngImg;
Widget *NameBox, *LocationBox, *DoneButton, *Dummy, *TimeBox, *WDTime;
Keyboard *kb1,  *kb3, *kb5 = NULL, *kb4 = NULL, *kb6 = NULL, *kb7 = NULL, *kb9; //kb3 newal2

double SetpointTmp, LedSatur = 0.0;
float RMSCurrent, ActivePower, RMSVoltage ,thermoCurrent = -1.0;

unsigned int presentColor = 0;
short int userind = 0;
short int ImageLoaded[20];
unsigned short int failed_deviceId = 0, Ledhue = 0;

int sensorsCount = 17, xCount = 2, yCount = 3, pageNum, curPage = 0;
int Acflag = 0, CheckAcFan = 0, tempvaluesize = 0, AcIrFlag = 0, CheckIrMode = 0;
int arrow_plus, pen_1, arrow_minus, trash_png, trashopen_png, color_shades;
int moreIcon, startAt,  deviceToControl = -1, crossonTouch,img1,img2,img3,img4; //newgeneric
int crossdefault, critical, battery_c, alarm_default, alarm_onTouch, alarm_off_default, alarm_off_onTouch, luminanceImage;
int Tick, color_temp, in_home, away_home, battery_icon, color_png, fanImage, humidityImage, color_temp1, color_png1;
int change, basicVal, filed, valueToSave = -1, backButtonImg, backButtonPressImg;

int contDevID = -1, AcIrFanMode = 0, preMode;
int irApplianceSelected, irAppButtonSelected, irAppLedSelected, irAppButton;
int device_key = -1, soundtype = -1, StartIndxCnt = 0;
int thermoCool = -1, thermoHeat = -1, thermotarget = -1;
int old_version, new_version, updates_available = 0,u_val, s_devid = -1, per = 0;
int dimmerCurrLevel, thermoCoolCurrLevel = 0, thermoHeatCurrLevel = 0, percentage;

int xCoords[3] = {X_COORD_START, X_COORD_START + 150, X_COORD_START + 300};
int yCoords[4] = {Y_COORD_START, Y_COORD_START + 60, Y_COORD_START + 120, Y_COORD_START + 180};
int devIdList[250], MODE = LIST_MODE, format = FORMAT_F;


bool ConValFlag = false, MultiSoundFlag = false; //ConFlag = false,, ConMode = false;
bool ParameterFlag = false, ValueSizeFlag = false, ValueFlag = false; //IRCodeFlag = false,
bool kbMode = false, En_Key = false, enableTouch = false;
bool ConMode = false, IRCodeFlag = false, ConFlag = false;
bool StartLED = true, StartBlinkLED = false, StartSiren = false ;
bool ota_upgrade_flag = false, updatetouchflag = false, current_update = false;

unsigned char Userno;
unsigned char appled[7];
char pin[10];
char nVer[512], output[180], t_temp[500];
char ConfigParam[10] = "", ConfigValue[10] = "", ConfigSet[10] = "";
char thermoMode[20], thermoFan[20], thermoState[20], thermoFanState[20], thermoaway[10], thermounit[10], tmpc[10] = "";
char *xpin, almond_mode[180], DevName[180], DevLocation[180], Userpin[20] = "", Time[10] = "";

wchar_t valunt[10], ConfigParam_opt[180]; //TMP_opt[180],
wchar_t opt[180], N_opt[180], As_opt[180];

int8u in = 1;
genericDevice gen;
unsigned char grid1[3], grid2[3], grid3[3];
bool more = false, tamper = false, indexState = false, multiswitch = false;


Widget GarageDoorOpenButton(type.Button, 80, 130, 80, 40, 0, lan.get("open", "connected_sensor")), GarageDoorCloseButton(type.Button, 170, 130, 80, 40, 0, L"close"); //newal2
Widget EconModeButton(type.Button, 80, 190, 60, 30, 0, L"Econ"), HeatModeButton(type.Button, 150, 190, 60, 30, 0, lan.get("heat", "connected_sensor")); //heatit
Widget ConfigurationButton(type.Button, 120, 95, 85, 30, 0, L"Configuration"), ConfigurationButton1(type.Button, 110, 190, 90, 30, 0, L"Configuration");
Widget IRCodeSetButton(type.Button, 200, 100, 50, 40, 0, lan.get("set", "common")), SetConfigurationButton(type.Button, 246, 150, 50, 30, 0, lan.get("set", "common"));
Widget ValueSizeSetTextModifyPlus(type.Button, 80, 150, 20, 30, 0, L"+"), ValueSizeSetTextModifyMinus(type.Button, 146, 150, 20, 30, 0, L"-");
Widget DoorLockButtonWithUserCode(type.Button, 50, 190, 70, 30, 0, lan.get("lock", "common")), DoorUnlockButtonWithUserCode(type.Button, 135, 190, 70, 30, 0, lan.get("unlock", "common"));

Widget DoorLockButton(type.Button, 85, 130, 70, 30, 0, lan.get("lock", "common")), DoorUnlockButton(type.Button, 165, 130, 70, 30, 0, lan.get("unlock", "common"));
Widget SwitchOnButton(type.Button, 85, 150, 70, 30, 0, lan.get("on_1", "common")), SwitchOffButton(type.Button, 165, 150, 70, 30, 0, lan.get("off_1", "common"));
Widget ACSwitchOnButton(type.Button, 85, 130, 70, 30, 0, lan.get("on_1", "common")), ACSwitchOffButton(type.Button, 165, 130, 70, 30, 0, lan.get("off_1", "common"));
Widget EditButton(type.Button, 220, 190, 80, 30, 0, lan.get("edit", "common")), TamperButton(type.Button, 15, 190, 80, 30, 0, lan.get("tampered", "connected_sensor"));
Widget PanicButton(type.Button, 15, 190, 80, 30, 0, lan.get("panic", "connected_sensor")), EmerButton(type.Button, 100, 190, 80, 30, 0, lan.get("emergency", "connected_sensor"));
Widget YesButton(type.Button, 85, 140, 70, 30, 0, lan.get("yes", "common")), NoButton(type.Button, 165, 140, 70, 30, 0, lan.get("no", "common"));

Widget multilevelOnButton(type.Button, 85, 152, 70, 30, 0, lan.get("on_1", "common")), multilevelOffButton(type.Button, 165, 152, 70, 30, 0, lan.get("off_1", "common"));
Widget colorOnButton(type.Button, 100, RECT_Y + RECT_HEIGHT + 45, 55, 30, 0, lan.get("on_1", "common")), colorOffButton(type.Button, 160, RECT_Y + RECT_HEIGHT + 45, 55, 30, 0, lan.get("off_1", "common"));
Widget settingsButton(type.Button, 20, RECT_Y + RECT_HEIGHT + 45, 75, 30, 0, L"Settings");
Widget checkforupgradeButton(type.Button, 100, 170, 130, 30, 0, L"Check For Updates"), upgradeButton(type.Button, 100, 170, 130, 30, 0, L"Upgrade Firmware");
Widget basicSetTextModifyPlus(type.Button, 85, 170, 18, 30, 0, lan.get("plus", "common")), basicSetTextModifyMinus(type.Button, 137, 170, 20, 30, 0, lan.get("minus", "common"));

Widget EmergencySoundButton(type.Button, 20, 95, 80, 45, 0, lan.get("emergency", "connected_sensor")), FireSoundButton(type.Button, 110, 95, 80, 45, 0, lan.get("fire", "connected_sensor"));
Widget AmbulanceSoundButton(type.Button, 200, 95, 80, 45, 0, lan.get("ambulance", "connected_sensor")), PoliceSoundButton(type.Button, 20, 155, 80, 45, 0, lan.get("police", "connected_sensor"));
Widget DoorSoundButton(type.Button, 110, 155, 80, 45, 0, lan.get("door_chime", "connected_sensor")), BeepSoundButton(type.Button, 200, 155, 80, 45, 0, lan.get("beep", "connected_sensor"));
Widget MultiSwitchOnButton(type.Button, 90, 100, 70, 30, 0, lan.get("on_1", "common")), MultiSwitchOffButton(type.Button, 180, 100, 70, 30, 0, lan.get("off_1", "common"));
Widget MultiSwitchOnButton1(type.Button, 90, 150, 70, 30, 0, lan.get("on_1", "common")), MultiSwitchOffButton1(type.Button, 180, 150, 70, 30, 0, lan.get("off_1", "common"));

Widget ApplianceOne(type.Button, 25, 95, 85, 30, 0, L""), ApplianceTwo(type.Button, 120, 95, 85, 30, 0, L""), ApplianceThree(type.Button, 215, 95, 85, 30, 0, L"");
Widget ApplianceFour(type.Button, 25, 135, 85, 30, 0, L""), ApplianceFive(type.Button, 120, 135, 85, 30, 0, L""), ApplianceSix(type.Button, 215, 135, 85, 30, 0, L"");
Widget AppButtonOne(type.Button, 25, 95, 60, 30, 0, L""), AppButtonTwo(type.Button, 95, 95, 60, 30, 0, L""), AppButtonThree(type.Button, 165, 95, 60, 30, 0, L""), AppButtonFour(type.Button, 235, 95, 60, 30, 0, L"");
Widget AppButtonFive(type.Button, 25, 135, 60, 30, 0, L""), AppButtonSix(type.Button, 95, 135, 60, 30, 0, L""), AppButtonSeven(type.Button, 165, 135, 60, 30, 0, L""), AppButtonEight(type.Button, 235, 135, 60, 30, 0, L"");
Widget SelectIRLed(type.Button, 205, 190, 90, 30, 0, L"Select LED"), ChangeButtonName(type.Button, 25, 190, 100, 30, 0, L"Change Button");

Widget LedBrightness(type.Button, 20, 145, 90, 30, 0, L"Brightness"),selectIRLedDone(type.Button, 220, 190, 80, 30, 0, L"Done");
Widget ChangeApplianceName(type.Button, 25, 190, 85, 30, 0, L"Edit Name"), MoreButton(type.Button, 120, 190, 80, 30, 0, L"More");

Widget SelectSoundButton(type.ColoredButton, 50, 120, 100, 50, 0, RGB(0, 205, 20), 1, 20.0f, 65535, L""); 
Widget StopMSSirenButton(type.ColoredButton, 170, 120, 100, 50, 0, RGB(204, 0, 0), 1, 30.0f, 65535, L"stop"); 
Widget RemovedeviceButton(type.ColoredButton, 15, 190, 80, 30, 0, RGB(150, 0, 0), 1, 18.0f, 65535, lan.get("remove", "common"));
Widget AdvanceButton(type.ColoredButton, 15, 190, 70, 30, 0, RGB(204, 0, 0), 1, 15.0f, 65535, L"Advanced");

Widget TempBox(type.TextBox, 100, 140, 78, 25, 0, L""), IRCodeSetText(type.TextBox, 135, 100, 50, 40, 0, L"");
Widget ParameterSetText(type.TextBox, 10, 150, 60, 30, 0, L""), ValueSetText(type.TextBox, 176, 150, 60, 30, 0, L"");
Widget AlmondTimer(type.TextBox, 180, 185, 55, 30, 0, L"∞"), AlmondLedTimer(type.TextBox, 175, 190, 55, 30, 0, L"∞");
Widget ValueSizeSetText(type.TextBox, 103, 150, 40, 30, 0, L"");

Widget
basicGetButton(type.Button, 85, 115, 70, 30, 0, lan.get("get", "common")), basicGetTextBox(type.TextBox, 165, 115, 70, 30, 0, L""),
basicSetButton(type.Button, 165, 155, 70, 30, 0, lan.get("set", "common")), basicSetText(type.TextBox, 105, 155, 30, 30, 0, L"");

Widget RetainButton(type.RadioButton, 100, 130, 80, 40, 0, L"");
Widget SirenButton(type.RadioButton, 20, 95, 40, 25, 0, L""), AlarmButton(type.RadioButton, 105, 95, 40, 25, 0, L""), DoorBellButton(type.RadioButton, 200, 95, 40, 25, 0, L"");
Widget LowVolume(type.RadioButton, 20, 140, 25, 25, 0, L""), MedVolume(type.RadioButton, 105, 140, 25, 25, 0, L""), HighVolume(type.RadioButton, 200, 140, 25, 25, 0, L"");

Widget *IRLed1, *IRLed2, *IRLed3, *IRLed4, *IRLed5, *IRLed6; 


Widget btnCapture(type.Button, 155, 95, 55, 30, 0, L"Capture"), btnImageCount(type.Button, 155, 135, 55, 30, 0, L""), btnResolution(type.Button, 155, 175, 55, 30, 0, L"");
Widget btnDeActive(type.Button, 25, 190, 80, 30, 0, L"Deactivate");
void doNothing(void) {

    return;
}

Device* getDevice(int devid) {
    list <Device>::iterator iterator;
  char buffer[123556];
    for (iterator = dL.devices.begin(); iterator != dL.devices.end(); ++iterator) {
        if ((*iterator).getID() == devid) {
            break;

        }
    }
    if (iterator == dL.devices.end())
        return NULL;
    else
        return &(*iterator);
}

void getBatteryVal(void) {

    try {
        Device d(contDevID);
        d.getBatteryStatus();
    } catch (int exeption) {
        return;
    }
    sprintf(t_temp, " get battery level for device %d ", contDevID);
    LOG(Logger::INFO, t_temp);
}

/*void doRefresh(void) {
    bool Local = true;
    while (Local) {
        try {
            dL.Populate();
            Local = false;
        } catch (int ex) {
            LOG(Logger::WARN, "Connected home Got Exception");
            if (ex != 0 && ex != -1) {
                RemoveDevice(ex);
            }
        }
    }
    int cnt = 0;
    for (list <Device>::iterator iterator = dL.devices.begin(); (iterator != dL.devices.end()) && (cnt < 250); ++iterator) {
        devIdList[cnt++] = (*iterator).getID();
    }
    sensorsCount = dL.devices.size();
}*/
bool a = false;


int main(int argc, char** argv) {
    
    logger = Logger::getInstance();
    char logfile[100] = "/tmp/connected_home.log";
    logger->setFileName(logfile);
    logger->setMethod(Logger::ALL, Logger::LOGFILE);
    LOG(Logger::INFO, "<===============================Connected_home started ==============================>");
    for(int i=0;i<argc;i++)
    printf("%s\n",argv[i]);
    init();	
    HADevices::initialize();
    HADevices::genericCallback(&theCallback);
    HADevices::eventCallback(&theEventCallback);
    setTouchHandler(tH);
    libTouchInit();
    LIST();
    Value();
    a = true;
    while (1) {
       pause;
    } 
    return 0;
}
void LIST(void)
{
	int cnt =0 ,d_devid;
        for (list <Device>::iterator iterator = dL.devices.begin(); (iterator != dL.devices.end()) && (cnt < 250); ++iterator) {
        devIdList[cnt++] = (*iterator).getID();
        d_devid = (*iterator).getID();
        cout<<"Device ID : "<<d_devid<<endl;
        cout<<"Device Name: "<<(*iterator).getDeviceName()<<endl;
        cout<<"Device Location : "<<(*iterator).getDeviceLocation()<<endl;
        cout<<"Device Type :"<<(*iterator).getDeviceType()<<endl;
        cout<<"Device Technology : "<<(*iterator).getDeviceTechnology()<<endl;
        cout<<"Bulb ID   : "<<(*iterator).getbulbid()<<endl;
        cout<<"Friend Device : "<<(*iterator).getFriendlyDeviceType()<<endl;
        cout<<"OZWNode : "<<(*iterator).getOZWNode()<<endl;
        cout<<"ZigBeeShortID : 0x"<<(*iterator).getShortID()<<endl;
        cout<<"ZigBeeEUI64 : "<<(*iterator).getZigBeeEUI64()<<endl;
        cout<<"Association Timestamp : "<<(*iterator).getAssociationTimeStamp()<<endl;
        cout<<"Manufacturername : "<<(*iterator).getManufacturername()<<endl;;
        cout<<"Version : "<<(*iterator).getVersion()<<endl;
        cout<<"Model ID : "<<(*iterator).getModelid()<<endl;
        cout<<"Value Count : "<<(*iterator).getValueCount()<<endl;
        int i=0;
        for(i = 0;i<=(*iterator).getValueCount();++i)
        { 
          cout<<"Index = "<<i<<"\t"<<"Endpoint = "<<(*iterator).getEndPoint(i)<<"\t"<<"Value = "<<(*iterator).getValue(i)<<"\t"<<"Name : "<<(*iterator).getValueName(i)<<endl;
         }
        cout<<"---------------------------------------------------"<<endl;
        cout<<"---------------------------------------------------"<<endl;  
     }
}


void Value(void)
{
   loop:
   int DevID,IdxID;
   char IdxVal[5];
   printf("Enter the device ID\n");
   scanf("%d",&DevID);
   printf("Enter the index ID\n");
   scanf("%d",&IdxID);
   printf("Enter the index Value\n");
   scanf("%s",IdxVal);
   getDevice(DevID);
   Device d(DevID);
   d.setValue(IdxID,IdxVal);
   HADevices::genericCallback(&theCallback);
   goto loop;
}



    void init(void) {
    int timerD = 0;
    AlmondTimerInfo info;
    wcscpy(opt, lan.get("connected_sensor_title", "connected_sensor"));
    connected_home = new Screen(opt);   
    memset(opt, 0, sizeof opt);
    connected_home->backHandler = &backOnTouch;
    connected_home->onExit = &onExit;
    connected_home->keyboard.keyboardReturn = &draw;

    NameBox = new Widget(type.TextBox, 100, 100, 170, 30, 0, L"");
    NameBox->onTouch = &NameOnTouch;

    DoneButton = new Widget(type.Button, 220, 190, 80, 30, 0, lan.get("done", "common"));
    DoneButton->onTouch = &DoneButtonOnTouch;

    LocationBox = new Widget(type.TextBox, 100, 135, 170, 30, 0, L"");
    LocationBox->onTouch = &LocationBoxOnTouch;
    Dummy = new Widget(type.Container, 100, 100, 1, 1, 0, L"");

    WDTime = new Widget(type.TextBox, 135, 140, 50, 25, 0, L"");
    WDTime->onTouch = &TimeBoxOnTouch;
    
    kb1 = new Keyboard();
    kb3 = new Keyboard();
    kb4 = new Keyboard();
    kb5 = new Keyboard(); //newgeneric
    kb6 = new Keyboard(); //newal2
    kb7 = new Keyboard(); //newal2
    kb9 = new Keyboard();
    
    kb1->keyboardReturn = &kbReturn;
    kb3->keyboardReturn = &NumericKbReturn;
    kb4->keyboardReturn = &NumericKbReturn;
    kb5->keyboardReturn = &IRCodeNumericKbReturn; //newgeneric
    kb6->keyboardReturn = &TempNumericKbReturn; //newal2
    kb7->keyboardReturn = &ConfigParamNumericKbReturn; //newal2
    kb9->keyboardReturn = &IRAppNameKbReturn;

    pageNum = sensorsCount / (xCount * yCount);
    bool Local = true;
    while (Local) {
        try {
            dL.Populate();
            Local = false;
        } catch (int ex) {
            fprintf(stderr, "Connected_home:got exception:%d\n", ex);
            LOG(Logger::WARN, "Connected home Got Exception");
            if (ex != 0 && ex != -1) {
                RemoveDevice(ex);
            }
        }
    }
    list <Device>::iterator it;
    int d_devid;
    int cnt = 0;
//        if ((*it).getDeviceType() == 5) {
//            Door_Lock Dl;
//            Dl.Device_id = (*it).getID();
//            Dl.state = false;
//            Door_Lock_info.push_back(Dl);
//        }
    for (list <Device>::iterator iterator = dL.devices.begin(); (iterator != dL.devices.end()) && (cnt < 250); ++iterator) {
        devIdList[cnt++] = (*iterator).getID();
        d_devid = (*iterator).getID();
        if ((*iterator).getDeviceType() == 50) {
            peanut_plug PL;
            PL.S_Dev_ID = (*iterator).getID();
            PL.S_state = false;
            PL.percentage = 0;
            peanut_plug_info.push_back(PL);
        }
    }
    sensorsCount = dL.devices.size();
    setKeyboardValidationFunction(&kbVal);
    readLCDArea.data = (int *) malloc(320 * 240 * sizeof (int));

    HADevices::initializeFree2();
    HADevices::genericCallback(&theCallback);
    HADevices::eventCallback(&theEventCallback);

    DoorUnlockButton.onTouch = &doorUnlock_onTouch;
    DoorLockButton.onTouch = &doorLock_onTouch;
    DoorLockButtonWithUserCode.onTouch = &doorLock_onTouch;
    DoorUnlockButtonWithUserCode.onTouch = &doorUnlock_onTouch;

    SwitchOnButton.onTouch = &switchOn_onTouch;
    SwitchOffButton.onTouch = &switchOn_offTouch;
    ACSwitchOnButton.onTouch = &switchOn_onTouch;
    ACSwitchOffButton.onTouch = &switchOn_offTouch;

    AdvanceButton.onTouch = &Advancebutton_onTouch;
    RetainButton.onTouch = &RetainButton_onTouch;
    checkforupgradeButton.onTouch = &upgradeButton_onTouch;
    upgradeButton.onTouch = &upgrade;

    GarageDoorOpenButton.onTouch = &GarageDoorOpen_onTouch;
    GarageDoorCloseButton.onTouch = &GarageDoorClose_onTouch;
    StopMSSirenButton.onTouch = &multilevelOff_onTouch;
    SelectSoundButton.onTouch = &SelectSoundButton_onTouch;

    EconModeButton.onTouch = &EconModeButton_onTouch;
    HeatModeButton.onTouch = &HeatModeButton_onTouch;
    TempBox.onTouch = &TempBoxOnTouch;
    ConfigurationButton.onTouch = &ConfigurationButton_onTouch;
    ConfigurationButton1.onTouch = &ConfigurationButton_onTouch;
    IRCodeSetText.onTouch = &IRCodeBox_onTouch;
    IRCodeSetButton.onTouch = &SetConfigurationButton_onTouch;
    ParameterSetText.onTouch = &ParameterBox_onTouch;
    ValueSetText.onTouch = &ConfigValueBox_onTouch;
    ValueSizeSetTextModifyPlus.onTouch = &ValueSizeSetTextModifyPlus_onTouch;
    ValueSizeSetTextModifyMinus.onTouch = &ValueSizeSetTextModifyMinus_onTouch;
    SetConfigurationButton.onTouch = &SetConfigurationButton_onTouch;

    ///////////////////////////////////////////////////////////////////////////newal2
    RemovedeviceButton.onTouch = &Remove_onTouch;
    TamperButton.onTouch = &Tamper_onTouch;
    PanicButton.onTouch = &Panic_onTouch;
    EmerButton.onTouch = &Emer_onTouch;

    YesButton.onTouch = &YesButton_onTouch;
    NoButton.onTouch = &NoButton_onTouch;
    EditButton.onTouch = &EditButton_onTouch;

    multilevelOnButton.onTouch = &multilevelOn_onTouch;
    multilevelOffButton.onTouch = &multilevelOff_onTouch;
    colorOnButton.onTouch = &multilevelOn_onTouch;
    colorOffButton.onTouch = &multilevelOff_onTouch;
    settingsButton.onTouch = &settings_onTouch;
   

    basicGetButton.onTouch = &basicGetButton_onTouch;
    basicGetTextBox.onTouch = &basicGetText_onTouch;
    basicSetButton.onTouch = &basicSetButton_onTouch;
    basicSetText.onTouch = &basicSetText_onTouch;
    basicSetTextModifyPlus.onTouch = &basicSetTextModifyPlus_onTouch;
    basicSetTextModifyMinus.onTouch = &basicSetTextModifyMinus_onTouch;
    MoreButton.onTouch = &moreGeneric_onTouch;

    MultiSwitchOnButton.onTouch = &MultiSwitchOnButton_onTouch;
    MultiSwitchOffButton.onTouch = &MultiSwitchOffButton_onTouch;
    MultiSwitchOnButton1.onTouch = &MultiSwitchOnButton1_onTouch;
    MultiSwitchOffButton1.onTouch = &MultiSwitchOffButton1_onTouch;
    //---------------//newgeneric---------------
    EmergencySoundButton.onTouch = &TypeOfSoundButton_onTouch;
    FireSoundButton.onTouch = &TypeOfSoundButton_onTouch;
    AmbulanceSoundButton.onTouch = &TypeOfSoundButton_onTouch;
    PoliceSoundButton.onTouch = &TypeOfSoundButton_onTouch;
    DoorSoundButton.onTouch = &TypeOfSoundButton_onTouch;
    BeepSoundButton.onTouch = &TypeOfSoundButton_onTouch;

    AlarmButton.onTouch = &AlarmButton_onTouch;
    SirenButton.onTouch = &SirenButton_onTouch;
    DoorBellButton.onTouch = &DoorBellButton_onTouch;
    LowVolume.onTouch = &AlmondSirenLowVolume_onTouch;
    MedVolume.onTouch = &AlmondSirenMedVolume_onTouch;
    HighVolume.onTouch = &AlmondSirenHighVolume_onTouch;
    AlmondTimer.onTouch = &AlmondTimer_onTouch;

    AlmondLedTimer.onTouch = &AlmondLedTimer_onTouch;
    ApplianceOne.onTouch = &applianceButton_OnTouch;
    ApplianceTwo.onTouch = &applianceButton_OnTouch;
    ApplianceThree.onTouch = &applianceButton_OnTouch;
    ApplianceFour.onTouch = &applianceButton_OnTouch;
    ApplianceFive.onTouch = &applianceButton_OnTouch;
    ApplianceSix.onTouch = &applianceButton_OnTouch;

    AppButtonOne.onTouch = &applianceEventButton_OnTouch;
    AppButtonTwo.onTouch = &applianceEventButton_OnTouch;
    AppButtonThree.onTouch = &applianceEventButton_OnTouch;
    AppButtonFour.onTouch = &applianceEventButton_OnTouch;
    AppButtonFive.onTouch = &applianceEventButton_OnTouch;
    AppButtonSix.onTouch = &applianceEventButton_OnTouch;
    AppButtonSeven.onTouch = &applianceEventButton_OnTouch;
    AppButtonEight.onTouch = &applianceEventButton_OnTouch;

    SelectIRLed.onTouch = &SelectIRLed_OnTouch;
    ChangeButtonName.onTouch = &ChangeButtonName_OnTouch;
    selectIRLedDone.onTouch = &selectIRLedDone_OnTouch;
    ChangeApplianceName.onTouch = &ChangeApplianceName_OnTouch;
    
    btnCapture.onTouch = &btnCaptureOnTouch;
    btnImageCount.onTouch = &ImageCountOnTouch;
    btnResolution.onTouch = &ResolutionOnTouch;

    btnDeActive.onTouch = &btndeativeOnTouch;
    

    timerD = open("/dev/almond_timer", O_RDONLY);
    info.pid = getpid();
    if (timerD < 0) {
        system("mknod /dev/almond_timer c 97 0");
        timerD = open("/dev/almond_timer", O_RDONLY);
        if (timerD < 0) {
            perror("Could not Open almond_timer\n");
            return;
        }

    }
    ioctl(timerD, SET_PID, &info); //&pidForTimerDriver);
    signal(SIGTSTP, saveVal);
    info.mills = 200;
    ioctl(timerD, SET_PERIOD, &info); //200);
    memset(ImageLoaded, -1, sizeof (ImageLoaded));
}

void connectedHome_init(void) {
    //img=pngImg.loadPNG("/almond/home_settings.png");
    moreIcon = pngImg.loadPNG("/almond/more.png");
    crossonTouch = pngImg.loadPNG("/almond/wrong_onTouch.png");
    // humidityImage = pngImg.loadPNG("/almond/water_off_20x20.png");
    critical = pngImg.loadPNG("/almond/critical_45x45.png");
    battery_c = pngImg.loadPNG("/almond/battery_critical.png");
    battery_icon = pngImg.loadPNG("/almond/battery_40x15.png");
    Tick = pngImg.loadPNG("/almond/Tick.png");
    in_home = pngImg.loadPNG("/almond/in.png");
    away_home = pngImg.loadPNG("/almond/out.png");
    crossdefault = pngImg.loadPNG("/almond/wrong_default.png");
    alarm_default = pngImg.loadPNG("/almond/alarm_default.png");
    alarm_onTouch = pngImg.loadPNG("/almond/alarm_onTouch.png");
    alarm_off_default = pngImg.loadPNG("/almond/alarm_off_default.png");
    alarm_off_onTouch = pngImg.loadPNG("/almond/alarm_off_onTouch.png");
    color_temp = pngImg.loadPNG("/almond/color_temperature.png");
    color_temp1 = pngImg.loadPNG("/almond/color_temperature_generic.png");
    color_png = pngImg.loadPNG("/almond/color_strip.png");
    fanImage = pngImg.loadPNG("/almond/fan_20x20.png");
    moreIcon = pngImg.loadPNG("/almond/more.png");
    color_png1 = pngImg.loadPNG("/almond/color_strip_generic.png");
    humidityImage = pngImg.loadPNG("/almond/water_off_20x20.png");
    arrow_plus = pngImg.loadPNG("/almond/arrow_plus.png");
    arrow_minus = pngImg.loadPNG("/almond/arrow_minus.png");
    pen_1 = pngImg.loadPNG("/almond/pen_icon_1.png");
    trash_png = pngImg.loadPNG("/almond/trashbinicon_white.png");
    trashopen_png = pngImg.loadPNG("/almond/trashbin_open.png");
    luminanceImage = pngImg.loadPNG("/almond/luminance_20x20.png"); //newgeneric
    color_shades = pngImg.loadPNG("/almond/color_shades.png");
    backButtonImg = pngImg.loadPNG("/almond/backimage.png");
    backButtonPressImg = pngImg.loadPNG("/almond/backimagePressed.png");
    
    filed = open("/dev/almond_lcd", O_RDONLY);
    if (filed < 0) {
        perror("open");
    }
}

void connectedHome_draw(void) {
    char pngImageNames[100][150] = {
        "/almond/unknown.png", /* 0 */
        "/almond/ON-OFF.png",
        "/almond/Light_bulb.png",
        "/almond/door_window_sensor.png",
        "/almond/Light_bulb.png",
        "/almond/door_lock.png",
        "/almond/Alarm.png",
        "/almond/thermostat.png",
        "/almond/door_window_sensor.png",
        "/almond/shade.png",
        "/almond/door_window_sensor.png", /* 10 */
        "/almond/motion.png",
        "/almond/door_sensor.png",
        "/almond/fire_sensor.png",
        "/almond/water_sensor.png",
        "/almond/door_window_sensor.png",
        "/almond/door_window_sensor.png",
        "/almond/door_window_sensor.png",
        "/almond/door_window_sensor.png",
        "/almond/key_fob.png",
        "/almond/door_window_sensor.png", /* 20 */
        "/almond/door_window_sensor.png",
        "/almond/ON-OFF.png",
        "/almond/ON-OFF.png",
        "/almond/temperature_sensor.png", //OccupancySensor
        "/almond/Light_sensor.png", // LightSensor
        "/almond/roller_shutter.png", // WindowCovering
        "/almond/temperature_sensor.png", // TemperatureSensor
        "/almond/door_lock.png", //ZigbeeDoorlock
        "/almond/temperature_sensor.png", // ColorControl, 
        "/almond/unknown.png", /* 30 */ // PressureSensor, 
        "/almond/unknown.png", //FlowSensor, 
        "/almond/Light_bulb.png", //ColorDimmableLight, 
        "/almond/unknown.png", //HAPump, 
        "/almond/unknown.png", //Shade, 
        "/almond/door_window_sensor.png", // MultilevelSensor,
        "/almond/door_window_sensor.png", // SmokeDetector,
        "/almond/water_sensor.png", // FloodSensor,
        "/almond/door_window_sensor.png", // ShockSensor,
        "/almond/door_sensor.png", // DoorSensor,
        "/almond/door_window_sensor.png", /* 40 */ // MoistureSensor,
        "/almond/motion.png", // MovementSensor
        "/almond/Alarm.png", //siren
        "/almond/ON-OFF.png", //MultiSwitch
        "/almond/ON-OFF.png", //UnknownOnOffModule
        "/almond/ON-OFF.png", //BinaryPowerSwitch
        "/almond/thermostat.png", //SetPointThermostat
        "/almond/unknown.png", //UnknownThermostat
        "/almond/Light_bulb.png", //HUE bulb
        "/almond/motion.png", //multisesor
        "/almond/ON-OFF.png", /* 50 *///peanutplug
        "/almond/unknown.png",
        "/almond/roller_shutter.png",
        "/almond/roller_shutter.png",
        "/almond/thermostat.png",
        "/almond/Alarm.png",
        "/almond/door_window_sensor.png",
        "/almond/thermostat.png",
        "/almond/nest_protect_icon_25x25.png",
        "/almond/unknown.png", //NotificationSensor
        "/almond/door_window_sensor.png", /* 60 */
        "/almond/key_fob.png",
        "/almond/thermostat.png",
        "/almond/Alarm.png",
        "/almond/Light_sensor.png",
        "/almond/thermostat.png"
    };

    connected_home->draw();
    startAt = curPage != 0 ? curPage * xCount * yCount - curPage : 0;
    it = dL.devices.begin();

    //char mode[500];
    int devicenum = 0, index = 0, indx = 0;
    /*Almond Al;
    Al.getAlmondProDB(AlmondMode, mode);
    HADevices::GetAlmondMode(mode); 
    sprintf(t_temp, "getalmondmode %s", mode);
    LOG(Logger::INFO, t_temp);
    strncpy(almond_mode, mode, 2);
    if (atoi(almond_mode) == 2) {
        pngImg.drawPNG(290, 5, 0, BACKGROUND_LAYER, in_home);
    } else if (atoi(almond_mode) == 3) {
        pngImg.drawPNG(290, 5, 0, BACKGROUND_LAYER, away_home);
    }*/
    sensorsCount = dL.devices.size();
    if (sensorsCount == 0) {
        ///Add the HomeAway
        drawText(lan.get("nosensors_1", "connected_sensor"), 30, 100, 250, 1, 0, 20.0f, 65535, 1);
        drawText(lan.get("nosensors_2", "connected_sensor"), 30, 125, 250, 1, 0, 20.0f, 65535, 1);
        return;
    }
    if (curPage != 0) {
        int temp_val;
        for (temp_val = 0; temp_val < startAt; temp_val++)
            it++;
    }
    for (int cY = 0; cY < yCount; cY++) {
        for (int cX = 0; cX < xCount; cX++) {
            if (((cX + cY * xCount) + (startAt)) > sensorsCount - 1) {
                MODE = LIST_MODE;
                return;
            }

            if (cX == xCount - 1 && cY == yCount - 1 && (((cX + cY * xCount) + (startAt)) != (sensorsCount - 1))) {
                drawTransparentRectangle(xCoords[cX], yCoords[cY], xCoords[cX] + 140, yCoords[cY] + 50, WIDGET_CONTAINER_LAYER, 150);
                swprintf(opt, 179, L"%ls", lan.get("moresensor_1", "connected_sensor"));
                swprintf(opt, 179, L"%ls\n%ls", opt, lan.get("moresensor_2", "connected_sensor"));
                drawText(opt, xCoords[cX] + 22, yCoords[cY] + 8, 118, 2, 0, 16.0f, 65535, 1);
                memset(opt, 0, sizeof opt);
                pngImg.drawPNG(xCoords[cX] + 1, yCoords[cY] + 15, 1, BACKGROUND_LAYER, moreIcon);
            } else {
                drawTransparentRectangle(xCoords[cX], yCoords[cY], xCoords[cX] + 140, yCoords[cY] + 50, WIDGET_CONTAINER_LAYER, 150);
                int nameLen = strlen((*it).getDeviceName());
                bzero(&opt, sizeof opt);
                utf8_to_wchar((*it).getDeviceName(), nameLen, opt, nameLen, UTF8_IGNORE_ERROR);
                drawText(opt, xCoords[cX] + 22, yCoords[cY] + 17, 118, 1, 0, 16.0f, 65535, 1);
                memset(opt, 0, sizeof (opt));
                devicenum = (int) (*it).getDeviceType();
                if (pngImageNames[devicenum][0] != 0) {
                    if (index = checkifImageAlreadyLoaded(pngImageNames[devicenum], &indx)) {
                        ImageLoaded[index] = sensorImg.loadPNG(pngImageNames[devicenum]);
                        sprintf(t_temp, "Image loaded :imageload[%d]:%d indx:%d", index, ImageLoaded[index], indx);
                        LOG(Logger::INFO, t_temp);
                        sensorImg.drawPNG(xCoords[cX] + 1, yCoords[cY] + 15, 1, BACKGROUND_LAYER, ImageLoaded[index]);
                    } else {
                        sprintf(t_temp, "Image drawn :indx:%d", indx);
                        LOG(Logger::INFO, t_temp);
                        sensorImg.drawPNG(xCoords[cX] + 1, yCoords[cY] + 15, 1, BACKGROUND_LAYER, indx);
                    }
                }
                indx = 0;
            }
            it++;
        }
        MODE = LIST_MODE;
    }
}

float getTempinFloat(char *str) {
    int i = 0;
    char tbuff[10] = "";
    while (str[i] && (str[i] >= '0' && str[i] <= '9') || str[i] == '.') {
        tbuff[i] = str[i++];
    }
    return atof(tbuff);
}

void btnCaptureOnTouch() {
    readArea(0, 0, 319, 239);
    connectedHomeSetValue(4, "true");
    Modal mW;
    mW.setLine1(L"");
    mW.setLine2(L"Please wait");
    mW.setLine3(L" Capturing...");
    mW.modalReturn = &drawArea;
    mW.draw(MODAL_TYPE_INFORMATION, 60);
}

void ImageCountOnTouch() {
    wcscmp(btnImageCount.W.text,  L"3") == 0 ? connectedHomeSetValue(6, 6) : connectedHomeSetValue(6, 3) ;
}

void ResolutionOnTouch() {
    wcscmp(btnResolution.W.text,  L"VGA") == 0 ? connectedHomeSetValue(7, 2) : connectedHomeSetValue(7, 1);
}

void btndeativeOnTouch() {
    connectedHomeSetValue(1, "false");
}
void tH(int x, int y) {
  //pngImg.drawPNG(40,55, 1,BACKGROUND_LAYER, img1);
  //pngImg.drawPNG(130,55, 1,BACKGROUND_LAYER, img2);
  connected_home->getTouch(x,y);

    if (a == false) 
    {   usleep(20000);
        return;
    }

    if (kbMode || MODE == KEYBOARD_MODE) {
        kb1->getTouch(x, y);
        kb3->getTouch(x, y);
        kb4->getTouch(x, y);
        kb5->getTouch(x, y); //newgeneric
        kb6->getTouch(x, y); //newal2
        kb7->getTouch(x, y); //newal2
        kb9->getTouch(x, y);
        
        connected_home->getTouch(x,y);
        return;
    }
    if (MODE == LIST_MODE) {
        connected_home->getTouch(x, y);
       /* if (x > 275 && x < 275 + 40 && y > 5 && y < 5 + 40) {
            char *ptr = NULL;
            char mode[500], data[500], email[500];
            Almond Al;
            Al.getAlmondProDB(AlmondMode, data);
            HADevices::GetAlmondMode(data);  
            LOG(Logger::INFO, data);
            strncpy(almond_mode, data, 2);
            FILE *fp;
            if (fp = fopen("/almond/email", "r")) {
                fgets(email, 180, fp);
                strcpy(data + 2, email);
                fclose(fp);
            }
            almond_mode[1] = '\0';
            if (atoi(almond_mode) == 2) {
                waitForPenUp();
                ptr = strchr(data + 2, '\n');
                if (ptr != NULL) {
                    *ptr = '\0';
                }
                sprintf(mode, "3 %s", data + 2);
                pngImg.drawPNG(290, 5, 0, BACKGROUND_LAYER, away_home);
            } else if (atoi(almond_mode) == 3) {
                waitForPenUp();
                ptr = strchr(data + 2, '\n');
                if (ptr != NULL) {
                    *ptr = '\0';
                }
                sprintf(mode, "2 %s", data + 2);
                pngImg.drawPNG(290, 5, 0, BACKGROUND_LAYER, in_home);
            }
            LOG(Logger::INFO, mode);
            if (Al.setAlmondProDB(AlmondMode, mode)) {  
                LOG(Logger::INFO, "setting almond mode success");
            } else {
                LOG(Logger::INFO, "setting almond mode failure");
            }
        }*/
        for (int cY = 0; cY < yCount; cY++) {
            for (int cX = 0; cX < xCount; cX++) {
                if (x > xCoords[cX] && y > yCoords[cY] && x < xCoords[cX] + 140 && y < yCoords[cY] + 50) {
                    if (((cX + cY * xCount) + (startAt)) > sensorsCount - 1)
                        return;

                    readArea(xCoords[cX], yCoords[cY], 141, 51);
                    if (cX == xCount - 1 && cY == yCount - 1 && (((cX + cY * xCount) + (startAt)) != (sensorsCount - 1))) {
                        drawTransparentRectangle(xCoords[cX], yCoords[cY], xCoords[cX] + 140, yCoords[cY] + 50, 65535, 60);
                        waitForPenUp();
                        drawArea();
                        more_onTouch();
                        return;
                    } else {
                        drawTransparentRectangle(xCoords[cX], yCoords[cY], xCoords[cX] + 140, yCoords[cY] + 50, 65535, 60);
                        waitForPenUp();
                        drawArea();
                        deviceToControl = devIdList[(startAt)+(cX + cY * xCount)];
                        for (it = dL.devices.begin(); it != dL.devices.end(); ++it) {
                            if (deviceToControl == (*it).getID()) {
                                drawControlDevice((*it));
                                break;
                            }
                        }

                    }
                }
            }
        }

    } else if (MODE == CONTROL_MODE) {
        if (x > 282 && x < 317 && y > 47 && y < 83) { //close button
            readArea(0, 0, 319, 239);
            fillRect(287, 57, 307, 77, RR_COLOR);
            pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossonTouch);
            gen.p_area = 0;
            multiswitch = false;
            waitForPenUp();
            LOG(Logger::INFO, "Close button pressed");
            lcd_clear(BACKGROUND_LAYER);
            MODE = LIST_MODE;
            connectedHome_draw();
            return;
        }
        if ((contDev != NestThermostat) && (contDev != NestSmokeDetector) && (contDev != AlmondSiren) && (contDev != AlmondBlink)) {
            EditButton.checkTouch(x, y);
        }
        memset(opt, 0, sizeof (opt));
        switch (contDev) {
            case UnknownOnOffModule:
            case BinarySwitch:
            case BinaryPowerSwitch:
                if (SwitchOnButton.checkTouch(x, y))
                    return;
                else if (SwitchOffButton.checkTouch(x, y))
                    return;
                break; //Switch";
            case MultilevelSwitch:

                if (x > RECT_X + 10 && y > RECT_Y && x < RECT_X + RECT_WIDTH - 10 && y < RECT_Y + RECT_HEIGHT) {
                    while (x > RECT_X + 10 && y > RECT_Y && x < RECT_X + RECT_WIDTH - 10 && y < RECT_Y + RECT_HEIGHT) {
                        if (x > RECT_X + RECT_WIDTH - 10) {
                            //fillCircle(90 + dimmerCurrLevel, 155, 7, RGB(23, 191, 227));
                            fillRect(RECT_X + 6, RECT_Y, RECT_X + RECT_WIDTH - 6, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
                            drawLine(RECT_X + 5, RECT_Y + (RECT_HEIGHT / 2), RECT_X + RECT_WIDTH - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            fillCircle(RECT_X + RECT_WIDTH - 10, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            dimmerCurrLevel = 100 * 2;
                        } else if (x < RECT_X) {
                            //fillCircle(90 + dimmerCurrLevel, 155, 7, RGB(23, 191, 227));
                            fillRect(RECT_X + 6, RECT_Y, RECT_X + RECT_WIDTH - 6, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
                            drawLine(RECT_X + 5, RECT_Y + (RECT_HEIGHT / 2), RECT_X + RECT_WIDTH - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            fillCircle(RECT_X + 10, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            dimmerCurrLevel = 0 * 2;
                        } else {
                            //fillCircle(90 + dimmerCurrLevel, 155, 7, RGB(23, 191, 227));
                            fillRect(RECT_X + 6, RECT_Y, RECT_X + RECT_WIDTH - 6, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
                            drawLine(RECT_X + 5, RECT_Y + (RECT_HEIGHT / 2), RECT_X + RECT_WIDTH - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            fillCircle(x, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            dimmerCurrLevel = (x - (RECT_X + 10));
                        }

                        usleep(90000);
                        x = readX();
                        y = readY();
                    }
                    connectedHomeSetValue(1, (dimmerCurrLevel / 2));
                    return;
                }

                multilevelOnButton.checkTouch(x, y);
                multilevelOffButton.checkTouch(x, y);
                break; //Dimmer";
            case MultilevelSwitchOnOff:

                if (x > 25 + 10 && y > RECT_Y && x < 301 - 10 && y < RECT_Y + RECT_HEIGHT) {
                    while (x > 25 + 10 && y > RECT_Y && x < 301 - 10 && y < RECT_Y + RECT_HEIGHT) {
                        if (x > 301 - 10) {
                            //fillCircle(90 + dimmerCurrLevel, 155, 7, RGB(23, 191, 227));
                            fillRect(25 + 6, RECT_Y, 301 - 6, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            fillCircle(301 - 10, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            dimmerCurrLevel = 255;
                        } else if (x < RECT_X) {
                            //fillCircle(90 + dimmerCurrLevel, 155, 7, RGB(23, 191, 227));
                            fillRect(25 + 6, RECT_Y, RECT_X + RECT_WIDTH - 6, 301, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            fillCircle(25 + 10, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            dimmerCurrLevel = 0;
                        } else {
                            //fillCircle(90 + dimmerCurrLevel, 155, 7, RGB(23, 191, 227));
                            fillRect(25 + 6, RECT_Y, 301 - 6, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            fillCircle(x, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            dimmerCurrLevel = (x - (25 + 10));
                        }

                        usleep(90000);
                        x = readX();
                        y = readY();
                    }
                    connectedHomeSetValue(1, dimmerCurrLevel);
                    return;
                }

                multilevelOnButton.checkTouch(x, y);
                multilevelOffButton.checkTouch(x, y);
                break; //Dimmer";
            case ColorDimmableLight:
            {


                if (x > 25 + 10 && y > RECT_Y && x < 301 - 10 && y < RECT_Y + RECT_HEIGHT) {
                    while (x > 25 + 10 && y > RECT_Y && x < 301 - 10 && y < RECT_Y + RECT_HEIGHT) {
                        if (x > 301 - 10) {
                            //fillCircle(90 + dimmerCurrLevel, 155, 7, RGB(23, 191, 227));
                            fillRect(25 + 6, RECT_Y, 301 - 6, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            fillCircle(301 - 10, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            dimmerCurrLevel = 255;
                        } else if (x < RECT_X) {
                            //fillCircle(90 + dimmerCurrLevel, 155, 7, RGB(23, 191, 227));
                            fillRect(25 + 6, RECT_Y, RECT_X + RECT_WIDTH - 6, 301, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            fillCircle(25 + 10, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            dimmerCurrLevel = 0;
                        } else {
                            //fillCircle(90 + dimmerCurrLevel, 155, 7, RGB(23, 191, 227));
                            fillRect(25 + 6, RECT_Y, 301 - 6, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            fillCircle(x, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            dimmerCurrLevel = (x - (25 + 10));
                        }

                        usleep(90000);
                        x = readX();
                        y = readY();
                    }
                    connectedHomeSetValue(1, dimmerCurrLevel);
                    return;
                } else if (x > 25 && y > RECT_Y + 40 && x < 301 - 10 && y < RECT_Y + RECT_HEIGHT + 40) {
                    int s_val;
                    while (x > 25 && y > RECT_Y + 40 && x < 301 - 10 && y < RECT_Y + RECT_HEIGHT + 40) {
                        pngImg.drawPNG(25, RECT_Y + 40, 1, 0, color_temp);
                        drawLine(x - 2, RECT_Y + 40, x - 2, RECT_Y + 40 + RECT_HEIGHT, 0);
                        s_val = ((x - 25)*32) + 1000; //colortemperature 
                        usleep(90000);
                        x = readX();
                    }
//                    setbulbcolortemperature(s_val);
                    connectedHomeSetValue(5, s_val);
                }
                colorOnButton.checkTouch(x, y);
                colorOffButton.checkTouch(x, y);
                settingsButton.checkTouch(x, y);
            }
                break;
            case DoorLock:
            case ZigbeeDoorLock:
            {

                int UsrNum = 0;
                Device *d = getDevice(contDevID);
                if (d) {
                    if (d->getDeviceTechnology() == ZWaveTechnology)
                        UsrNum = atoi(d->getValue(4));
                    else
                        UsrNum = atoi(d->getValue(3));

                    if (UsrNum) {
                        if (DoorLockButtonWithUserCode.checkTouch(x, y)) {
                            return;
                        } else if (DoorUnlockButtonWithUserCode.checkTouch(x, y)) {
                            return;
                        } else if (x > 159 && x < 186 && y > 127 && y < 154) {
                            waitForPenUp();
                            UserNoBackButton_onTouch();
                            return;
                        } else if (x > 219 && x < 246 && y > 127 && y < 154) {
                            waitForPenUp();
                            UserNoNextButton_onTouch();
                            return;
                        } else if (x > 234 && x < 256 && y > 159 && y < 181) {
                            PinboxonTouch();
                            return;
                        } else if (x > 274 && x < 296 && y > 159 && y < 181) {
                            fillRect(274, 159, 296, 181, RR_COLOR);
                            pngImg.drawPNG(275, 160, 1, BACKGROUND_LAYER, trashopen_png);
                            waitForPenUp();
                            fillRect(274, 159, 296, 181, RR_COLOR);
                            pngImg.drawPNG(275, 160, 1, BACKGROUND_LAYER, trash_png);
                            strcpy(Userpin, "");
                            if (d->getDeviceTechnology() == ZWaveTechnology) {
                                connectedHomeSetValue(Userno + 4, "");
                            } else {
                                connectedHomeSetValue(Userno + 3, "");
                            }
                            return;
                        } else if ((d->getDeviceTechnology() == ZWaveTechnology) && (x > 243 && x < 283 && y > 62 && y < 67)) {
                            waitForPenUp();
                            getBatteryVal();
                            return;
                        }
                    } else {
                        if (DoorLockButton.checkTouch(x, y)) {
                            return;
                        } else if (DoorUnlockButton.checkTouch(x, y)) {
                            return;
                        }
                    }
                }
                break; //Door Lock";
            }
            case ZenThermostat:
            case Thermostat:

            {
                Device *d = getDevice(contDevID);
                if (x > THERMO_COOL_X + 10 && y > THERMO_COOL_Y && x < THERMO_COOL_X + THERMO_COOL_WIDTH - 10 && y < THERMO_COOL_Y + THERMO_COOL_HEIGHT) {
                    while (x > THERMO_COOL_X + 10 && y > THERMO_COOL_Y && x < THERMO_COOL_X + THERMO_COOL_WIDTH - 10 && y < THERMO_COOL_Y + THERMO_COOL_HEIGHT) {
                        if (x > (THERMO_COOL_X + 10) && x < (THERMO_COOL_X + THERMO_COOL_WIDTH - 10)) {
                            fillRect(THERMO_COOL_X + 6, THERMO_COOL_Y, THERMO_COOL_X + THERMO_COOL_WIDTH - 6, THERMO_COOL_Y + THERMO_COOL_HEIGHT, RGB(23, 191, 227));
                            drawLine(THERMO_COOL_X + 5, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), THERMO_COOL_X + THERMO_COOL_WIDTH - 5, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 65535);
                            fillCircle(x, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);
                            thermoCoolCurrLevel = x - THERMO_COOL_X - 10;
                        }
                        usleep(90000);
                        x = readX();
                        y = readY();
                    }
//                    setThermoCoolCurrLevel(((thermoCoolCurrLevel) / 3) + 35);
                    connectedHomeSetValue(5, ((thermoCoolCurrLevel) / 3) + 35);
                } else if (x > THERMO_HEAT_X + 10 && y > THERMO_HEAT_Y && x < THERMO_HEAT_X + THERMO_HEAT_WIDTH - 10 && y < THERMO_HEAT_Y + THERMO_HEAT_HEIGHT) {
                    while (x > THERMO_HEAT_X + 10 && y > THERMO_HEAT_Y && x < THERMO_HEAT_X + THERMO_HEAT_WIDTH - 10 && y < THERMO_HEAT_Y + THERMO_HEAT_HEIGHT) {
                        if (x > THERMO_HEAT_X + 10 && x < THERMO_HEAT_X + THERMO_HEAT_WIDTH - 10) {
                            fillRect(THERMO_HEAT_X + 6, THERMO_HEAT_Y, THERMO_HEAT_X + THERMO_HEAT_WIDTH - 6, THERMO_HEAT_Y + THERMO_HEAT_HEIGHT, RGB(23, 191, 227));
                            drawLine(THERMO_HEAT_X + 5, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 65535);
                            fillCircle(x, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);
                            thermoHeatCurrLevel = x - THERMO_HEAT_X - 10;
                        }
                        usleep(90000);
                        x = readX();
                        y = readY();
                    }
//                    setThermoHeatCurrLevel((thermoHeatCurrLevel / 3) + 35);
                    connectedHomeSetValue(4, (thermoHeatCurrLevel / 3) + 35);
                } else if (x > 60 && x < 110 && y > 97 && y < 125) { //auto
                    fillRect(60, 97, 110, 125, RGB(23, 191, 227));
                    fillRect(115, 97, 165, 125, BUTTON_LAYER);
                    fillRect(170, 97, 220, 125, BUTTON_LAYER);
                    drawText(lan.get("auto", "connected_sensor"), 60, 103, 50, 1, 0, 16.0f, 65535, 1);
                    drawText(lan.get("cool", "connected_sensor"), 115, 103, 50, 1, 0, 16.0f, 65535, 1);
                    drawText(lan.get("heat", "connected_sensor"), 170, 103, 50, 1, 0, 16.0f, 65535, 1);
//                    setThermostatMode("Auto");
                    connectedHomeSetValue(2, "Auto");
                    waitForPenUp();
                } else if (x > 115 && x < 165 && y > 97 && y < 125) { //cool
                    fillRect(60, 97, 110, 125, BUTTON_LAYER);
                    fillRect(115, 97, 165, 125, RGB(23, 191, 227));
                    fillRect(170, 97, 220, 125, BUTTON_LAYER);
                    drawText(lan.get("auto", "connected_sensor"), 60, 103, 50, 1, 0, 16.0f, 65535, 1);
                    drawText(lan.get("cool", "connected_sensor"), 115, 103, 50, 1, 0, 16.0f, 65535, 1);
                    drawText(lan.get("heat", "connected_sensor"), 170, 103, 50, 1, 0, 16.0f, 65535, 1);
//                    setThermostatMode("Cool");
                    connectedHomeSetValue(2, "Cool");
                    waitForPenUp();
                } else if (x > 170 && x < 220 && y > 97 && y < 125) { //heat
                    fillRect(60, 97, 110, 125, BUTTON_LAYER);
                    fillRect(115, 97, 165, 125, BUTTON_LAYER);
                    fillRect(170, 97, 220, 125, RGB(23, 191, 227));
                    drawText(lan.get("auto", "connected_sensor"), 60, 103, 50, 1, 0, 16.0f, 65535, 1);
                    drawText(lan.get("cool", "connected_sensor"), 115, 103, 50, 1, 0, 16.0f, 65535, 1);
                    drawText(lan.get("heat", "connected_sensor"), 170, 103, 50, 1, 0, 16.0f, 65535, 1);
//                    setThermostatMode("Heat");
                    connectedHomeSetValue(2, "Heat");
                    waitForPenUp();
                } else if ((x > 60 && x < 135 && y > 190 && y < 218) && ((d->getValueCount() != 10) || (d->getDeviceTechnology() == ZigBeeTechnology))) { //Auto Low
                    fillRect(60, 190, 135, 218, RGB(23, 191, 227));
                    fillRect(140, 190, 215, 218, BUTTON_LAYER);
                    drawText(lan.get("auto_low", "connected_sensor"), 60, 195, 75, 1, 0, 16.0f, 65535, 1);
                    drawText(lan.get("on_low", "connected_sensor"), 140, 195, 75, 1, 0, 16.0f, 65535, 1);
//                    setThermostatFanMode("Auto Low");
                    connectedHomeSetValue(6, "Auto Low");
                    waitForPenUp();
                } else if ((x > 140 && x < 215 && y > 190 && y < 218) && ((d->getValueCount() != 10) || (d->getDeviceTechnology() == ZigBeeTechnology))) { //On Low
                    fillRect(60, 190, 135, 218, BUTTON_LAYER);
                    fillRect(140, 190, 215, 218, RGB(23, 191, 227));
                    drawText(lan.get("auto_low", "connected_sensor"), 60, 195, 75, 1, 0, 16.0f, 65535, 1);
                    drawText(lan.get("on_low", "connected_sensor"), 140, 195, 75, 1, 0, 16.0f, 65535, 1);
//                    setThermostatFanMode("On Low");
                    connectedHomeSetValue(6, "On Low");
                    waitForPenUp();
                } else if (x > 225 && x < 300 && y > 97 && y < 125) {
                    waitForPenUp();
                    fillRect(225, 97, 300, 125, RGB(50, 191, 20));
                    convertTemperature(1, opt, true);
                    drawText(opt, 225, 97 + 6, 75, 1, 0, 18.0f, 65535, 1);
                } else if (x > 240 && y > 60 && x < 280 && y < 75) {
                    waitForPenUp();
                    getBatteryVal();
                    return;
                } else if ((x > 60 && x < 100 && y > 190 && y < 218) && (d->getValueCount() == 10)) {//Auto Low with RH
                    fillRect(60, 190, 100, 218, !(strstr(thermoFan, "Auto Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
                    fillRect(110, 190, 150, 218, !(strstr(thermoFan, "On Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
                    drawText(L"Auto", 62, 195, 40, 1, 0, 16.0f, 65535, 1);
                    drawText(L"On", 110, 195, 40, 1, 0, 16.0f, 65535, 1);
                    pngImg.drawPNG(60, 197, 1, BACKGROUND_LAYER, Tick);
//                    setThermostatFanMode("Auto Low");
                    connectedHomeSetValue(6, "Auto Low");
                    waitForPenUp();
                } else if ((x > 110 && x < 150 && y > 190 && y < 218) && (d->getValueCount() == 10)) { //On Low with RH
                    fillRect(60, 190, 100, 218, !(strstr(thermoFan, "Auto Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
                    fillRect(110, 190, 150, 218, !(strstr(thermoFan, "On Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
                    drawText(L"Auto", 62, 195, 40, 1, 0, 16.0f, 65535, 1);
                    drawText(L"On", 110, 195, 40, 1, 0, 16.0f, 65535, 1);
                    pngImg.drawPNG(110, 197, 1, BACKGROUND_LAYER, Tick);
//                    setThermostatFanMode("On Low");
                    connectedHomeSetValue(6, "On Low");
                    waitForPenUp();
                }
            }
                break; //Thermostat";
            case Gateway:
                break; //Gateway";
            case SceneController:
                break; //Scene Control";
            case SmartACSwitch:
            {
                if (ACSwitchOnButton.checkTouch(x, y))
                    return;
                else if (ACSwitchOffButton.checkTouch(x, y))
                    return;
            }
                break; //Switch";
            case SecurifiSmartSwitch:
            {
                if (ACSwitchOnButton.checkTouch(x, y))
                    return;
                else if (ACSwitchOffButton.checkTouch(x, y))
                    return;
                else if (AdvanceButton.checkTouch(x, y))
                    return;
            }
                break;
            case KeyFob:
            {
                Device localD(contDevID);
                if (strcmp(localD.getValue(2), "true") == 0) {
                    PanicButton.checkTouch(x, y);
                }
                if (strcmp(localD.getValue(3), "true") == 0) {
                    EmerButton.checkTouch(x, y);
                }
            }
                break;
            case StandardWarningDevice:
            {
                if ((x > 45 && x < 125 && y > 125 && y < 195)) { //Alarm
                    pngImg.drawPNG(40, NOTIFICATION_Y - 5, 1, BACKGROUND_LAYER, alarm_onTouch);
                    drawText(lan.get("alarm", "connected_sensor"), NOTIFICATION_X + 17, NOTIFICATION_Y + 27, 110, 1, 0, 25.0f, 65535, 1);
                    waitForPenUp();
                    LOG(Logger::INFO, "Alarm On");
//                    Sirenon_onTouch(65535);
                    connectedHomeSetValue(1, "65535");
                    WDTime->setText(L"65535");
                    WDTime->draw();
                    pngImg.drawPNG(40, NOTIFICATION_Y - 5, 1, BACKGROUND_LAYER, alarm_default);
                    drawText(lan.get("alarm", "connected_sensor"), NOTIFICATION_X + 17, NOTIFICATION_Y + 27, 110, 1, 0, 25.0f, 65535, 1);
                } else if (x > 200 && x < 280 && y > 125 && y < 195) {//Alarm off
                    pngImg.drawPNG(195, NOTIFICATION_Y - 5, 1, BACKGROUND_LAYER, alarm_off_onTouch);
                    drawText(lan.get("off_1", "common"), NOTIFICATION_X + 175, NOTIFICATION_Y + 27, 100, 1, 0, 25.0f, 65535, 1);
                    waitForPenUp();
                    LOG(Logger::INFO, "Alarm Off");
//                    Sirenoff_onTouch();
                    connectedHomeSetValue(1, "0");
                    WDTime->setText(L"0");
                    WDTime->draw();
                    pngImg.drawPNG(195, NOTIFICATION_Y - 5, 1, BACKGROUND_LAYER, alarm_off_default);
                    drawText(lan.get("off_1", "common"), NOTIFICATION_X + 175, NOTIFICATION_Y + 27, 100, 1, 0, 25.0f, 65535, 1);
                }
                WDTime->checkTouch(x, y);
                Device d(contDevID);
                if (strcmp(d.getValue(3), "true") == 0 || enableTouch == true) {
                    enableTouch = false;
                    TamperButton.checkTouch(x, y);
                }
            }
                break;
            case RemoteControl:
            {
                Device *d = getDevice(contDevID);
                if (wcscmp(btnDeActive.W.text, L"Deactivate") == 0) {
                    btnDeActive.checkTouch(x, y);
                }
                if (d->getIndexNoFromName("TAMPER", -1) > 0 && enableTouch == true) {
                    enableTouch = false;
                    TamperButton.checkTouch(x, y);
                }
            }
                break;
            case LightSensor:
            {
                Device *d = getDevice(contDevID);
                if ((x > 240 && y > 60 && x < 280 && y < 75) && d->getIndexNoFromName("BATTERY", -1) > 0) {
                    waitForPenUp();
                    getBatteryVal();
                } else if (x > 190 && y > 95 && x < 300 && y < 125) {
                    int i;
                    if ((i = d->getIndexNoFromName("TEMPERATURE", -1)) > 0) {
                        waitForPenUp();
                        fillRect(190, 95, 300, 125, GREEN_COLOR);
                        memset(opt, 0, sizeof (opt));
                        convertTemperature(i, opt, true);
                        drawText(opt, 200, 100, 100, 1, 0, 20.0f, 65535, 1);
                    }
                } else if (d->getIndexNoFromName("TAMPER", -1) > 0 && enableTouch == true) {
                    enableTouch = false;
                    TamperButton.checkTouch(x, y);
                }
            }
                break;
            case ZigbeeMultiSensor:
            {
                Device *d = getDevice(contDevID);
                if (x > 190 && y > 95 && x < 300 && y < 125) {
                    int i;
                    if ((i = d->getIndexNoFromName("TEMPERATURE", -1)) > 0) {
                        waitForPenUp();
                        fillRect(190, 95, 300, 125, GREEN_COLOR);
                        memset(opt, 0, sizeof (opt));
                        convertTemperature(i, opt, true);
                        drawText(opt, 200, 100, 100, 1, 0, 20.0f, 65535, 1);
                    }
                } else if (d->getIndexNoFromName("TAMPER", -1) > 0 && enableTouch == true) {
                    enableTouch = false;
                    TamperButton.checkTouch(x, y);
                }
            }
                break;
            case ContactSwitch:
            case StandardCIE:
            case MotionSensor:
            case FireSensor:
            case WaterSensor:
            case GasSensor:
            case PersonalEmergencyDevice:
            case VibrationOrMovementSensor:
            case Keypad:
            case Alarm:
            case TemperatureSensor:
            case SmokeDetector:
            case DoorSensor:
            case MovementSensor:
            case ShockSensor:
            case BinarySensor:
            case FloodSensor:
            {
                Device *d = getDevice(contDevID);
                if (x > 240 && y > 60 && x < 280 && y < 75) {
                    waitForPenUp();
                    getBatteryVal();
                    return;
                } else {
                    if (d) {
                        if (strcmp(d->getValue(3), "true") == 0) {
                            enableTouch = false;
                            TamperButton.checkTouch(x, y);
                        }
                    } 
                }
                if (strcmp(d->getValue(4), "NaN") && strcmp(d->getValue(4), "#ERR")) {
                    if (x > 200 && x < 300 && y > 92 && y < 120) {
                        waitForPenUp();
                        convertTemperature(4, opt, true);
                        fillRect(215, 93, 300, 122, GREEN_COLOR);

                        drawText(opt, 215, 95, 85, 1, 0, 25.0f, 65535, 1);
                    }
                }
            }
                break;
            case MoistureSensor:
            {
                if ((x > 190 && y > 95 && x < 300 && y < 125)) {
                    waitForPenUp();
                    fillRect(190, 95, 300, 125, GREEN_COLOR);
                    convertTemperature(1, opt, true);
                    drawText(opt, 200, 100, 100, 1, 0, 25.0f, 65535, 1);
                }
            }
            break;
            case OccupancySensor:
            {

                if (x > 140 && x < 240 && y > 130 && y < 165) {
                    waitForPenUp();
                    convertTemperature(2, opt, true);
                    fillRect(140, 130, 240, 165, GREEN_COLOR);

                    drawText(opt, 140, 133, 100, 1, 0, 30.0f, 65535, 1);

                }
            }
                break;
            case ZigbeeTempSensor:
            {
                Device *d = getDevice(contDevID);
                if (x > 180 && x < 260 && y > 105 && y < 135) {
                    int i;
                    if ((i = d->getIndexNoFromName("TEMPERATURE", -1)) > 0) {
                        waitForPenUp();
                        memset(opt, 0, sizeof (opt));
                        convertTemperature(i, opt, true);
                        fillRect(180, 105, 260, 135, GREEN_COLOR);
                        drawText(opt, 185, 110, 70, 1, 0, 20.0f, 65535, 1);

                    }
                }
            }
                break;
            case SecurifiButton:
            {
                Device *d = getDevice(contDevID);
                if (strcmp(d->getValue(3), "true") == 0) {
                    TamperButton.checkTouch(x, y);
                }

            }
                break;
            case WindowCovering:
            {
                if (x > 25 && y > 95 && x < 105 && y < 125) {
                    fillRect(25, 95, 105, 125, BUTTON_LAYER_ONTOUCH);
                    drawText(L"Up", 25, 101, 80, 1, 0, 18.0f, 65535, 1);
                    waitForPenUp();
                    fillRect(25, 95, 105, 125, BUTTON_LAYER);
                    drawText(L"Up", 25, 101, 80, 1, 0, 18.0f, 65535, 1);
//                    setWindowCoveringMode(1);
                    connectedHomeSetValue(1, "1");
                } else if (x > 120 && y > 95 && x < 200 && y < 125) {
                    fillRect(120, 95, 200, 125, BUTTON_LAYER_ONTOUCH);
                    drawText(L"Down", 120, 101, 80, 1, 0, 18.0f, 65535, 1);
                    waitForPenUp();
                    fillRect(120, 95, 200, 125, BUTTON_LAYER);
                    drawText(L"Down", 120, 101, 80, 1, 0, 18.0f, 65535, 1);
//                    setWindowCoveringMode(2);
                    connectedHomeSetValue(1, "2");
                } else if (x > 215 && y > 95 && x < 295 && y < 125) {
                    fillRect(215, 95, 295, 125, BUTTON_LAYER_ONTOUCH);
                    drawText(L"Stop", 215, 101, 80, 1, 0, 18.0f, 65535, 1);
                    waitForPenUp();
                    fillRect(215, 95, 295, 125, BUTTON_LAYER);
                    drawText(L"Stop", 215, 101, 80, 1, 0, 18.0f, 65535, 1);
//                    setWindowCoveringMode(3);
                    connectedHomeSetValue(1, "3");
                }
            }
                break;
            case GenericDevice:
            {
                genericControlDeviceTouch(x, y);
                return;
            }
                break;
            case GarageDoorOpener://newal2
            {
                if (GarageDoorOpenButton.checkTouch(x, y)) {
                    return;
                }
                if (GarageDoorCloseButton.checkTouch(x, y)) {
                    return;
                }
            }
                break;

            case MultiSoundSiren://newal2
            {
                waitForPenUp();
                if (StopMSSirenButton.checkTouch(x, y))
                    return;
                if (SelectSoundButton.checkTouch(x, y))
                    return;
            }
                break;
            case EnergyReader://newal2
            case MultiSensor://newal2
            {
                if ((x > 190 && y > 95 && x < 300 && y < 125) && contDev != EnergyReader) {
                    waitForPenUp();
                    fillRect(190, 95, 300, 125, GREEN_COLOR);
                    convertTemperature(4, opt, true);
                    drawText(opt, 200, 100, 100, 1, 0, 25.0f, 65535, 1);
                } else if (x > 240 && y > 60 && x < 280 && y < 75) {
                    waitForPenUp();
                    getBatteryVal();
                    return;
                }

            }
                break;

            case RollerShutter://newal2
            {
                if ((x > 40 && x < 120 && y > 110 && y < 140)) {
                    drawRoundedRectangle(40, 110, 120, 140, RGB(0, 100, 200), 255);
                    drawText(lan.get("open", "connected_sensor"), 45, 115, 80, 1, 0, 20.0f, 65535, 1);
                    waitForPenUp();
                    connectedHomeSetValue(1, "99");
                    drawRoundedRectangle(40, 110, 120, 140, BUTTON_LAYER, 255);
                    drawText(lan.get("open", "connected_sensor"), 45, 115, 80, 1, 0, 20.0f, 65535, 1);
                } else if ((x > 200 && x < 280 && y > 110 && y < 140)) {
                    drawRoundedRectangle(200, 110, 280, 140, RGB(0, 100, 200), 255);
                    drawText(L"up", 200, 115, 80, 1, 0, 20.0f, 65535, 1);
                    waitForPenUp();
                    connectedHomeSetValue(2, "99");
                    //basicSetValue(99);
                    drawRoundedRectangle(200, 110, 280, 140, BUTTON_LAYER, 255);
                    drawText(L"up", 200, 115, 80, 1, 0, 20.0f, 65535, 1);
                    //drawText(lan.get("up", "connected_sensor"), 273, 115, 100, 1, 0, 26.0f, 65535, 1);
                } else if ((x > 40 && x < 120 && y > 150 && y < 180)) {
                    drawRoundedRectangle(40, 150, 120, 180, RGB(0, 100, 200), 255);
                    drawText(L"close", 45, 155, 80, 1, 0, 20.0f, 65535, 1);
                    waitForPenUp();
                    connectedHomeSetValue(1, "0");
                    drawRoundedRectangle(40, 150, 120, 180, BUTTON_LAYER, 255);
                    drawText(L"close", 45, 155, 80, 1, 0, 20.0f, 65535, 1);
                } else if ((x > 200 && x < 280 && y > 150 && y < 180)) {
                    drawRoundedRectangle(200, 150, 280, 180, RGB(0, 100, 200), 255);
                    drawText(L"down", 200, 155, 80, 1, 0, 20.0f, 65535, 1);
                    waitForPenUp();
//                    basicSetButton_onTouch();
                    connectedHomeSetValue( 2, basicVal);
                    drawRoundedRectangle(200, 150, 280, 180, BUTTON_LAYER, 255);
                    drawText(L"down", 200, 155, 80, 1, 0, 20.0f, 65535, 1);
                } else if ((x > 125 && x < 195 && y > 130 && y < 170)) {
                    drawRoundedRectangle(125, 130, 195, 170, RGB(255, 17, 0), 255);
                    drawText(L"STOP", 135, 140, 80, 1, 0, 25.0f, 65535, 0);
                    waitForPenUp();
                    connectedHomeSetValue(3, "");
                    drawRoundedRectangle(125, 130, 195, 170, RGB(247, 35, 20), 255);
                    drawText(L"STOP", 135, 140, 80, 1, 0, 25.0f, 65535, 0);
                }
            }
                break;

            case Siren://newal2
            {
                if (x > 240 && y > 60 && x < 280 && y < 75) {
                    waitForPenUp();
                    getBatteryVal();
                    return;
                } else if (x > 40 && y > 140 && x < 140 && y < 185) {//alarm
                    drawRoundedRectangle(40, 140, 140, 185, RGB(247, 64, 43), 255);
                    swprintf(opt, 179, L"%ls", lan.get("alarm", "connected_sensor"));
                    drawText(opt, 40, 147, 100, 1, 0, 30.0f, 65535, 1);
                    waitForPenUp();
                    drawRoundedRectangle(40, 140, 140, 185, RGB(255, 0, 0), 255);
                    swprintf(opt, 179, L"%ls", lan.get("alarm", "connected_sensor"));
                    drawText(opt, 40, 147, 100, 1, 0, 30.0f, 65535, 1);
                    LOG(Logger::PRINT, "Sending ALARM Signal....");
//                    switchOn_onTouch();
                    connectedHomeSetValue(1, "true");
                } else if (x > 180 && y > 140 && x < 280 && y < 185) {//alarm
                    drawRoundedRectangle(180, 140, 280, 185, RGB(5, 250, 83), 255);
                    swprintf(opt, 179, L"%ls", lan.get("off_1", "common"));
                    drawText(opt, 180, 147, 100, 1, 0, 30.0f, 65535, 1);
                    waitForPenUp();
                    drawRoundedRectangle(180, 140, 280, 185, GREEN_COLOR, 255);
                    swprintf(opt, 179, L"%ls", lan.get("off_1", "common"));
                    drawText(opt, 180, 147, 100, 1, 0, 30.0f, 65535, 1);
                    LOG(Logger::PRINT, "Sending OFF Signal....");
//                    switchOn_offTouch();
                    connectedHomeSetValue(1, "false");
                }
            }
                break;


            case ZWtoACIRExtender://newal2 touch
            {
                int SetTemp = 0;
                
                Device *d = getDevice(contDevID);
                if (x > 240 && y > 60 && x < 280 && y < 75) {
                    waitForPenUp();
                    getBatteryVal();
                    return;
                } else if (x > 210 && y > 90 && x < 300 && y < 125) {
                    waitForPenUp();
                    fillRect(210, 95, 300, 125, GREEN_COLOR);
                    convertTemperature(1, opt, true);
                    drawText(opt, 200, 100, 100, 1, 0, 20.0f, 65535, 1);
                } else if (x > 70 && y > 160 && x < 292 && y < 184) { //temp set
                    if (!strcmp(d->getValue(2), "Cool") || !strcmp(d->getValue(2), "Heat")) {//newal3
                        while (x > 74 && y > 160 && x < 288 && y < 184) {
                            if (x > (78) && x < (288)) {
                                fillRect(70, 160, 296, 184, RGB(23, 191, 227));
                                drawLine(74, 171, 292, 171, 65535);
                                drawLine(74, 167, 74, 175, 65535);
                                drawLine(292, 167, 292, 175, 65535);
                                fillCircle(x, 171, 7, 65535);
                                SetTemp = x - 72;
                            }
                            usleep(90000);
                            x = readX();
                            y = readY();
                        }
                        SetTemp = ((((SetTemp) / 14) + 16))*9.0 / 5+32;
                        if (!strcmp(d->getValue(2), "Heat")) {
//                            setThermoHeatCurrLevel(((SetTemp) / 14) + 16);
                            connectedHomeSetValue(3, SetTemp);
                        } else {
//                            setThermoCoolCurrLevel(((SetTemp) / 14) + 16);
                            connectedHomeSetValue(4, SetTemp);
                        }
                    }
                } else if ((x > 70 && y > 130 && x < 95 && y < 155) || (x > 190 && y > 130 && x < 215 && y < 155)) {//for mode
                    int modelocalflag = 0;
                    while ((x > 70 && y > 130 && x < 95 && y < 155) || (x > 190 && y > 130 && x < 215 && y < 155)) {
                        if ((x > 70 && y < 155) || (x > 190 && y < 155)) {
                            fillRect(97, 130, 187, 155, BUTTON_LAYER);

                            if (x > 70 && y > 130 && x < 95 && y < 155) {//Minus Arrow
                                AcIrFlag--;
                                if (AcIrFlag == 0) {
                                    AcIrFlag = 8;
                                }
                            } else if (x > 190 && y > 130 && x < 215 && y < 155) {//Plus Arrow
                                AcIrFlag++;
                                if (AcIrFlag == 9) {
                                    AcIrFlag = 1;
                                }
                            }

                            if (AcIrFlag == 1) {
                                drawText(lan.get("auto", "connected_sensor"), 105, 135, 80, 1, 0, 20.0f, 65535, 1);
                                modelocalflag = 1;
                                if (CheckIrMode == 1) {
                                    pngImg.drawPNG(103, 132, 1, BACKGROUND_LAYER, Tick);
                                }
                            } else if (AcIrFlag == 2) {
                                drawText(lan.get("cool", "connected_sensor"), 105, 135, 80, 1, 0, 20.0f, 65535, 1);
                                modelocalflag = 2;
                                if (CheckIrMode == 2) {
                                    pngImg.drawPNG(103, 132, 1, BACKGROUND_LAYER, Tick);
                                }
                            } else if (AcIrFlag == 3) {
                                drawText(lan.get("heat", "connected_sensor"), 105, 135, 80, 1, 0, 20.0f, 65535, 1);
                                modelocalflag = 3;
                                if (CheckIrMode == 3) {
                                    pngImg.drawPNG(103, 132, 1, BACKGROUND_LAYER, Tick);
                                }
                            } else if (AcIrFlag == 4) {
                                drawText(L"Changeover", 103, 135, 80, 1, 0, 15.0f, 65535, 1);
                                modelocalflag = 4;
                                if (CheckIrMode == 4) {
                                    pngImg.drawPNG(103, 132, 1, BACKGROUND_LAYER, Tick);
                                }
                            } else if (AcIrFlag == 5) {
                                drawText(L"Off", 105, 135, 80, 1, 0, 20.0f, 65535, 1);
                                modelocalflag = 5;
                                if (CheckIrMode == 5) {
                                    pngImg.drawPNG(103, 132, 1, BACKGROUND_LAYER, Tick);
                                }
                            } else if (AcIrFlag == 6) {
                                drawText(L"Resume", 105, 135, 80, 1, 0, 20.0f, 65535, 1);
                                modelocalflag = 6;
                                if (CheckIrMode == 6) {
                                    pngImg.drawPNG(103, 132, 1, BACKGROUND_LAYER, Tick);
                                }
                            } else if (AcIrFlag == 7) {
                                drawText(L"Fan Only", 105, 135, 80, 1, 0, 20.0f, 65535, 1);
                                modelocalflag = 7;
                                if (CheckIrMode == 7) {
                                    pngImg.drawPNG(103, 132, 1, BACKGROUND_LAYER, Tick);
                                }
                            } else if (AcIrFlag == 8) {
                                drawText(L"Dry Air", 105, 135, 80, 1, 0, 20.0f, 65535, 1);
                                modelocalflag = 8;
                                if (CheckIrMode == 8) {
                                    pngImg.drawPNG(103, 132, 1, BACKGROUND_LAYER, Tick);
                                }
                            }
                        }
                        usleep(200000);
                        x = readX();
                        y = readY();
                    }
                    if (modelocalflag == 1) {
//                        setThermostatMode("Auto");
                        connectedHomeSetValue(2, "Auto");
                    } else if (modelocalflag == 2) {
//                        setThermostatMode("Cool");
                        connectedHomeSetValue(2, "Cool");
                    } else if (modelocalflag == 3) {
//                        setThermostatMode("Heat");
                        connectedHomeSetValue(2, "Heat");
                    } else if (modelocalflag == 4) {
//                        setThermostatMode("Auto Changeover");
                        connectedHomeSetValue(2, "Auto Changeover");
                    } else if (modelocalflag == 5) {
//                        setThermostatMode("Off");
                        connectedHomeSetValue(2, "Off");
                    } else if (modelocalflag == 6) {
//                        setThermostatMode("Resume");
                        connectedHomeSetValue(2, "Resume");
                    } else if (modelocalflag == 7) {
//                        setThermostatMode("Fan Only");
                        connectedHomeSetValue(2, "Fan Only");
                    } else if (modelocalflag == 8) {
//                        setThermostatMode("Dry Air");
                        connectedHomeSetValue(2, "Dry Air");
                    }
                } else if ((x > 190 && y > 190 && x < 215 && y < 215) || (x > 70 && y > 190 && x < 95 && y < 215)) { //for plus Arrow fan mode
                    while ((x > 190 && y > 190 && x < 215 && y < 215) || (x > 70 && y > 190 && x < 95 && y < 215)) {
                        int localflag = 0;
                        if ((x > 190 && y < 215) || (x > 70 && y < 215)) {
                            fillRect(97, 190, 187, 215, RGB(23, 191, 227));

                            if (x > 70 && y > 190 && x < 95 && y < 215) {
                                Acflag--;
                                if (Acflag < 1) {
                                    Acflag = 4;
                                }
                            } else {
                                Acflag++;
                                if (Acflag > 4) {
                                    Acflag = 1;
                                }
                            }
                            if (Acflag == 1) {
                                drawText(lan.get("auto", "connected_sensor"), 107, 195, 70, 1, 0, 20.0f, 65535, 1);
                                localflag = 1;
                                if (CheckAcFan == 1) {
                                    pngImg.drawPNG(100, 195, 1, BACKGROUND_LAYER, Tick);
                                }
                            } else if (Acflag == 2) {
                                drawText(L"Low", 107, 195, 70, 1, 0, 20.0f, 65535, 1);
                                localflag = 2;
                                if (CheckAcFan == 2) {
                                    pngImg.drawPNG(100, 195, 1, BACKGROUND_LAYER, Tick);
                                }
                            } else if (Acflag == 3) {
                                //drawText(lan.get("medium", "connected_sensor"), 105, 200, 70, 1, 0, 17.0f, 65535, 1);
                                drawText(L"Medium", 107, 195, 70, 1, 0, 20.0f, 65535, 1);
                                localflag = 3;
                                if (CheckAcFan == 3) {
                                    pngImg.drawPNG(100, 195, 1, BACKGROUND_LAYER, Tick);
                                }
                            } else if (Acflag == 4) {
                                drawText(L"High", 107, 195, 70, 1, 0, 20.0f, 65535, 1);
                                localflag = 4;
                                if (CheckAcFan == 4) {
                                    pngImg.drawPNG(100, 195, 1, BACKGROUND_LAYER, Tick);
                                }
                            }

                        }//if
                        usleep(150000);
                        x = readX();
                        y = readY();
                        if (localflag == 1) {
//                            setThermostatFanMode("Auto Low");
                            connectedHomeSetValue(5, "Auto Low");
                        } else if (localflag == 2) {
//                            setThermostatFanMode("On Low");
                            connectedHomeSetValue(5, "On Low");
                        } else if (localflag == 3) {
//                            setThermostatFanMode("Unknown 5");
                            connectedHomeSetValue(5, "Unknown 5");
                        } else if (localflag == 4) {
//                            setThermostatFanMode("On High");
                            connectedHomeSetValue(5, "On High");
                        }
                    }//while
                } else if (x > 220 && x < 300 && y > 130 && y < 155) { //Swing
                    fillRect(220, 130, 300, 155, RGB(12, 106, 246)); //swing 
                    if (atoi(d->getValue(8)) == 0) {
                        //drawText(lan.get("swing_off", "connected_sensor"), 220, 135, 80, 1, 0, 20.0f, 65535, 1);
                        drawText(L"Swing Off", 220, 135, 80, 1, 0, 20.0f, 65535, 1);
                    } else if (atoi(d->getValue(8)) == 1) {
                        // drawText(lan.get("swing_on", "connected_sensor"),  220, 135, 80, 1, 0, 20.0f, 65535, 1);
                        drawText(L"Swing On", 220, 135, 80, 1, 0, 20.0f, 65535, 1);
                    }
                    waitForPenUp();
                    fillRect(220, 130, 300, 155, BUTTON_LAYER); //swing 
                    if (atoi(d->getValue(8)) == 0) {
                        //drawText(lan.get("swing_off", "connected_sensor"), 220, 135, 80, 1, 0, 20.0f, 65535, 1);
                        drawText(L"Swing Off", 220, 135, 80, 1, 0, 20.0f, 65535, 1);
                        connectedHomeSetValue(8, "1");
                    } else if (atoi(d->getValue(8)) == 1) {
                        // drawText(lan.get("swing_on", "connected_sensor"),  220, 135, 80, 1, 0, 20.0f, 65535, 1);
                        drawText(L"Swing On", 220, 135, 80, 1, 0, 20.0f, 65535, 1);
                        connectedHomeSetValue(8, "0");
                    }
                } else if (x > 70 && x < 115 && y > 95 && y < 125) { //Power
                    fillRect(70, 95, 115, 125, RGB(12, 106, 246));
                    if (atoi(d->getValue(9)) == 0) {
                        drawText(lan.get("off_1", "common"), 70, 100, 50, 1, 0, 20.0f, 65535, 1);
                    } else {
                        drawText(lan.get("on_1", "common"), 70, 100, 50, 1, 0, 20.0f, 65535, 1);
                    }
                    waitForPenUp();
                    fillRect(70, 95, 115, 125, BUTTON_LAYER);
                    if (atoi(d->getValue(9)) == 0) {
                        drawText(lan.get("off_1", "common"), 70, 100, 50, 1, 0, 20.0f, 65535, 1);
//                        basicSetValue(255);
                        connectedHomeSetValue(9, 255);
                    } else {
                        drawText(lan.get("on_1", "common"), 70, 100, 50, 1, 0, 20.0f, 65535, 1);
//                        basicSetValue(0);
                        connectedHomeSetValue(9, 0);
                    }
                    waitForPenUp();
                } else if (ConfigurationButton.checkTouch(x, y)) {
                    return;
                }

            }
                break;

            case SetPointThermostat:
            {
                int er;
                double temp = 0.0;
                char output[180];
                Device *d = getDevice(contDevID);
                if (x > 240 && y > 60 && x < 280 && y < 75) {//battery
                    waitForPenUp();
                    getBatteryVal();
                    return;
                } else if (x > 70 && x < 100 && y > 140 && y < 165 && ((format == FORMAT_F && SetpointTmp >= 43) || (format == FORMAT_C && SetpointTmp >= 6))) {//-arrow
                    waitForPenUp();
                    SetpointTmp--;
                    if (format == FORMAT_C) {
                        sprintf(output, "   %.2lf °C", SetpointTmp);
                    } else {
                        sprintf(output, "   %.2lf °F", SetpointTmp);
                    }
                    utf8_to_wchar(output, strlen(output), opt, strlen(output), UTF8_IGNORE_ERROR);
                    TempBox.setText(opt);
                    TempBox.draw();
                    if (format == FORMAT_C) {
//                        er = (tmpd *9)/5.0 + 32 ;
                        temp = ((((SetpointTmp *9)) / 5.0) + 32);
                        temp = ceil(temp);
                    } else {
                        temp = SetpointTmp;
                    }
                    er = (int) temp;
//                    setThermoHeatCurrLevel(er);
                    connectedHomeSetValue(1, er);
                } else if (x > 185 && x < 208 && y > 140 && y < 165 && ((format == FORMAT_F && SetpointTmp <= 103) || (format == FORMAT_C && SetpointTmp <= 39))) {//+arrow
                    waitForPenUp();
                    SetpointTmp++;
                    if (format == FORMAT_C) {
                        sprintf(output, "   %.2lf °C", SetpointTmp);
                    } else {
                        sprintf(output, "   %.2lf °F", SetpointTmp);
                    }
                    utf8_to_wchar(output, strlen(output), opt, strlen(output), UTF8_IGNORE_ERROR);
                    TempBox.setText(opt);
                    TempBox.draw();
                    if (format == FORMAT_C) {
                        temp = ((((SetpointTmp *9)) / 5.0) + 32);
                    } else {
                        temp = SetpointTmp;
                    }
                    er = (int) temp;
//                    setThermoHeatCurrLevel(er);
                    connectedHomeSetValue(1, er);
                } else if (x > 80 && x < 140 && y > 190 && y < 220) {//econ on touch
                    if (strcmp(d->getValue(5), "#ERR")) {
                        HeatModeButton.draw();
                        pngImg.drawPNG(85, 195, 1, BACKGROUND_LAYER, Tick);
                        waitForPenUp();
//                        EconModeButton_onTouch();
                        connectedHomeSetValue(5, "Heat Econ");
                    } else {
                        return;
                    }
                } else if (x > 150 && x < 210 && y > 190 && y < 220) {//heaton
                    if (strcmp(d->getValue(5), "#ERR")) {
                        EconModeButton.draw();
                        pngImg.drawPNG(153, 195, 1, BACKGROUND_LAYER, Tick);
                        waitForPenUp();
//                        HeatModeButton_onTouch();
                        connectedHomeSetValue(5, "Heat");
                    } else {
                        return;
                    }
                } else if (x > 100 && x < 178 && y > 140 && y < 165) {//tempbox
                    TempBox.checkTouch(x, y);
                }
                if (x > 190 && y > 95 && x < 300 && y < 125) {//temperature
                    char ss[180];
                    waitForPenUp();
                    //fillRect(190, 95, 300, 125, GREEN_COLOR);
                    fillRect(210, 95, 300, 125, GREEN_COLOR);
                    if (format == FORMAT_F) {
                        if (strcmp(d->getValue(5), "#ERR")) {
                            if (!strcmp(d->getValue(5), "Heat Econ")) {
                                SetpointTmp = (atoi(d->getValue(6)) - 32)*5.0 / 9;
                                SetpointTmp = ceil(SetpointTmp);
                                sprintf(ss, "   %.2lf °C", SetpointTmp);
                            } else {
                                SetpointTmp = (atoi(d->getValue(1)) - 32)*5.0 / 9;
                                SetpointTmp = ceil(SetpointTmp);
                                sprintf(ss, " %.2lf °C", SetpointTmp);
                            }
                        } else {
                            SetpointTmp = (atoi(d->getValue(1)) - 32)*5.0 / 9;
                            SetpointTmp = ceil(SetpointTmp);
                            sprintf(ss, "   %.2lf °C", SetpointTmp);
                        }
                        utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
                        TempBox.setText(opt);
                        TempBox.draw();

                        //fillRect(190, 95, 300, 125, GREEN_COLOR);
                        convertTemperature(4, opt, true);
                        // drawText(opt, 200, 93, 100, 1, 0, 25.0f, 65535, 1);
                        drawText(opt, 200, 100, 100, 1, 0, 25.0f, 65535, 1);
                    } else {
                        convertTemperature(4, opt, true);
                        //drawText(opt, 200, 93, 100, 1, 0, 25.0f, 65535, 1);
                        drawText(opt, 200, 100, 100, 1, 0, 25.0f, 65535, 1);

                        if (strcmp(d->getValue(5), "#ERR")) {
                            if (!strcmp(d->getValue(5), "Heat Econ")) {
                                SetpointTmp = getTempinFloat(d->getValue(6));
                                sprintf(ss, "   %.2lf °F", SetpointTmp);
                                //sprintf(output, "   %s °F",  d->getValue(6));
                            } else {
                                SetpointTmp = getTempinFloat(d->getValue(1));
                                sprintf(ss, "   %.2lf °F", SetpointTmp);
                                //sprintf(output, "   %s °F",  d->getValue(1));
                            }
                        } else {
                            SetpointTmp = getTempinFloat(d->getValue(1));
                            sprintf(ss, "   %.2lf °F", SetpointTmp);
                            //sprintf(output, "   %s °F",  d->getValue(6));
                        }
                        utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
                        TempBox.setText(opt);
                        TempBox.draw();
                    }

                }

            }
                break;
            case HueLamp:
            {

                Device *d = getDevice(contDevID);
                if (x > 25 + 10 && y > RECT_Y && x < 301 - 10 && y < RECT_Y + RECT_HEIGHT) {
                    while (x > 25 + 10 && y > RECT_Y && x < 301 - 10 && y < RECT_Y + RECT_HEIGHT) {
                        if (x > 301 - 10) {
                            fillRect(25 + 6, RECT_Y, 301 - 6, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            fillCircle(301 - 10, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            dimmerCurrLevel = 255;
                        } else if (x < RECT_X) {
                            fillRect(25 + 6, RECT_Y, RECT_X + RECT_WIDTH - 6, 301, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            fillCircle(25 + 10, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            dimmerCurrLevel = 0;
                        } else {
                            fillRect(25 + 6, RECT_Y, 301 - 6, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            fillCircle(x, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            dimmerCurrLevel = (x - (25 + 10));
                        }

                        usleep(90000);
                        x = readX();
                        y = readY();
                    }
                    connectedHomeSetValue(5, dimmerCurrLevel);
                } else if (x > 25 && y > RECT_Y + 38 && x < 280 && y < RECT_Y + RECT_HEIGHT + 38) {
                    int newCol;
                    while (x >= 25 && y >= RECT_Y + 38 && x <= 301 - 10 && y <= RECT_Y + RECT_HEIGHT + 38) {
                        pngImg.drawPNG(25, RECT_Y + 38, 1, 0, color_png);
                        fillRect(280, RECT_Y + 38, 300, 187, RGB(255, 255, 255));
                        drawLine(x - 2, RECT_Y + 38, x - 2, RECT_Y + 42 + RECT_HEIGHT, 0);
                        newCol = getColorFromScreen(x, y);
                        x = readX();
                        y = readY();
                        usleep(90000);
                    }
                    if (strstr(d->getValue(2), "true")) {
                        d->setColorBulb(newCol);
                    }
                }
                if (x > 280 && x < 300 && y > RECT_Y + 38 && y < RECT_Y + RECT_HEIGHT + 38) {
                    waitForPenUp();
                    pngImg.drawPNG(25, RECT_Y + 38, 1, 0, color_png);
                    fillRect(280, RECT_Y + 38, 300, 187, RGB(255, 255, 255));
                    drawLine(x - 2, RECT_Y + 38, x - 2, RECT_Y + 42 + RECT_HEIGHT, 0);
//                    setbulbsaturation(0);
                    connectedHomeSetValue(4, "0");
                }
                colorOnButton.checkTouch(x, y);
                colorOffButton.checkTouch(x, y);
                settingsButton.checkTouch(x, y);
            }
                break;
            case NestThermostat:
            {
                Device *d = getDevice(contDevID);
                double er;
                int xlow, xhigh;
                er = getTempinFloat(d->getValue(5));
                xlow = thermoCool = (int) er;
                er = getTempinFloat(d->getValue(6));
                xhigh = thermoHeat = (int) er;
                strcpy(thermoMode, d->getValue(2));
                strcpy(thermoState, d->getValue(11));
                strcpy(thermoaway, d->getValue(8));
                strcpy(thermounit, d->getValue(7));
                strcpy(thermoFanState, d->getValue(9));
                thermoCoolCurrLevel = (thermoCool - 50)*6 + 55;
                thermoHeatCurrLevel = (thermoHeat - 50)*6 + 55;

                if (!strcmp(d->getValue(11), "true")) {
                    if (x > 235 && y > 97 && x < 303 && y < 125) { // current temp mode
                        waitForPenUp();
                        if (!strcmp(thermoState, "true")) {
                            fillRect(235, 97, 303, 125, GREEN_COLOR);
                            convertTemperature(10, opt, true);
                            drawText(opt, 245, 97 + 6, 60, 1, 0, 18.0f, 65535, 0);
                        }
                    }

                    if (strcmp(d->getValue(14), "true")) { // check if IS_USING_EMERGENCY_HEAT is not true
                        if (!strcmp(d->getValue(8), "home")) {
                            if ((!strcmp(d->getValue(12), "true")) && (!strcmp(d->getValue(13), "true"))) {
                                if (x > 55 && x < 123 && y > 97 && y < 125) { //heat-cool mode
                                    fillRect(55, 97, 123, 125, RGB(23, 191, 227));
                                    fillRect(127, 97, 177, 125, BUTTON_LAYER);
                                    fillRect(181, 97, 231, 125, BUTTON_LAYER);
                                    drawText(L"Heat-Cool", 70, 105, 50, 1, 0, 13.0f, 65535, 1);
                                    drawText(lan.get("cool", "connected_sensor"), 132, 105, 50, 1, 0, 13.0f, 65535, 1);
                                    drawText(lan.get("heat", "connected_sensor"), 186, 105, 50, 1, 0, 13.0f, 65535, 1);
//                                    setThermostatMode("heat-cool");
                                    connectedHomeSetValue(2, "heat-cool");
                                    pngImg.drawPNG(57, 105, 1, BACKGROUND_LAYER, Tick);
                                    waitForPenUp();
                                }

                            }
                            if (!strcmp(d->getValue(12), "true")) {
                                if (x > 127 && x < 177 && y > 97 && y < 125) { //cool mode
                                    if (!strcmp(d->getValue(13), "true")) {
                                        fillRect(55, 97, 123, 125, BUTTON_LAYER);
                                        fillRect(181, 97, 231, 125, BUTTON_LAYER);
                                        drawText(L"Heat-Cool", 70, 105, 50, 1, 0, 13.0f, 65535, 1);
                                        drawText(lan.get("heat", "connected_sensor"), 186, 105, 50, 1, 0, 13.0f, 65535, 1);
                                    }
                                    fillRect(127, 97, 177, 125, RGB(23, 191, 227));
                                    drawText(lan.get("cool", "connected_sensor"), 132, 105, 50, 1, 0, 13.0f, 65535, 1);
//                                    setThermostatMode("cool");
                                    connectedHomeSetValue(2, "cool");
                                    pngImg.drawPNG(129, 105, 1, BACKGROUND_LAYER, Tick);
                                    waitForPenUp();
                                }

                            }
                            if (!strcmp(d->getValue(13), "true")) {
                                if (x > 181 && x < 231 && y > 97 && y < 125) { //heat mode
                                    if (!strcmp(d->getValue(12), "true")) {
                                        fillRect(55, 97, 123, 125, BUTTON_LAYER);
                                        fillRect(127, 97, 177, 125, BUTTON_LAYER);
                                        drawText(L"Heat-Cool", 70, 105, 50, 1, 0, 13.0f, 65535, 1);
                                        drawText(lan.get("cool", "connected_sensor"), 132, 105, 50, 1, 0, 13.0f, 65535, 1);
                                    }
                                    fillRect(181, 97, 231, 125, RGB(23, 191, 227));
                                    drawText(lan.get("heat", "connected_sensor"), 186, 105, 50, 1, 0, 13.0f, 65535, 1);
//                                    setThermostatMode("heat");
                                    connectedHomeSetValue(2, "heat");
                                    pngImg.drawPNG(183, 105, 1, BACKGROUND_LAYER, Tick);
                                    waitForPenUp();
                                }
                            }
                        }
                    } else {
                    }


                    if (!strcmp(d->getValue(8), "home")) {
                        if (!strcmp(d->getValue(2), "heat-cool")) {
                            if (!strcmp(d->getValue(13), "true") && !strcmp(d->getValue(12), "true")) {
                                if (x >= 59 && y > 130 && x < 298 && y < 155) {//cool slider
                                    while (x >= 59 && y > 130 && x < 298 && y < 155) {
                                        //if ((thermoHeatCurrLevel / 6 + 50 + 3) <= thermoHeat) {
                                        if (!strcasecmp("C", d->getValue(7))) {
                                            thermoHeatCurrLevel = x - 59;
                                            xlow = (thermoHeatCurrLevel / 5.6) + 48;
                                        } else {
                                            thermoHeatCurrLevel = x - 55;
                                            xlow = thermoHeatCurrLevel / 6 + 50;
                                        }
                                        if (xlow + 3 <= thermoHeat) {
                                            fillRect(THERMO_COOL_X - 5, THERMO_COOL_Y, 303, 155, RGB(23, 191, 227));
                                            drawLine(THERMO_COOL_X, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 298, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 65535);
                                            drawLine(THERMO_COOL_X, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
                                            drawLine(THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
                                            fillCircle(x, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);
                                            format = (format == FORMAT_C) ? FORMAT_C : FORMAT_F;
                                            if (format == FORMAT_F) {
                                                swprintf(opt, 20, L" %d", xlow);
                                                drawText(opt, x - 20, 128, 40, 1, 0, 14.0f, 65535, 1);
                                            } else {
                                                int celcius = (int) (((xlow - 32)*5.0) / 9);
                                                swprintf(opt, 20, L" %d", celcius);
                                                drawText(opt, x - 20, 128, 40, 1, 0, 14.0f, 65535, 1);
                                            }
                                            //}                        
                                        }
                                        usleep(90000);
                                        x = readX();
                                        y = readY();
                                    }
                                    char temp[10];
                                    int i = xlow;
                                    sprintf(temp, "%d", i);
                                    connectedHomeSetValue(5, temp);
                                } else if (x >= 59 && y > 160 && x < 298 && y < 185) {//heat slider
                                    while (x >= 59 && y > 160 && x < 298 && y < 185) {
                                        if (!strcasecmp("C", d->getValue(7))) {
                                            thermoHeatCurrLevel = x - 59;
                                            xhigh = (thermoHeatCurrLevel / 5.6) + 48;
                                        } else {
                                            thermoHeatCurrLevel = x - 55;
                                            xhigh = thermoHeatCurrLevel / 6 + 50;
                                        }
                                        if (xhigh - 3 > thermoCool) {
                                            fillRect(THERMO_HEAT_X - 5, THERMO_HEAT_Y, THERMO_HEAT_X + THERMO_HEAT_WIDTH + 43, THERMO_HEAT_Y + THERMO_HEAT_HEIGHT, RGB(23, 191, 227));
                                            drawLine(THERMO_HEAT_X, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 65535);
                                            drawLine(THERMO_HEAT_X, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
                                            drawLine(THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
                                            fillCircle(x, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);
                                            format = (format == FORMAT_C) ? FORMAT_C : FORMAT_F;
                                            if (format == FORMAT_F) {
                                                swprintf(opt, 20, L" %d", xhigh);
                                                drawText(opt, x - 20, 158, 40, 1, 0, 14.0f, 65535, 1);
                                            } else {
                                                int celcius = (int) (((xhigh - 32)*5.0) / 9);
                                                swprintf(opt, 20, L" %d", celcius);
                                                drawText(opt, x - 20, 158, 40, 1, 0, 14.0f, 65535, 1);
                                            }
                                        }
                                        usleep(90000);
                                        x = readX();
                                        y = readY();
                                    }


                                    char temp[10];
                                    int i = xhigh;
                                    sprintf(temp, "%d", i);
                                    connectedHomeSetValue(6, temp);
                                }
                            }
                        } else if (!strcmp(d->getValue(2), "heat")) {
                            if (!strcmp(d->getValue(13), "true")) {
                                if (x >= 59 && y > 160 && x < 298 && y < 185) {//heat slider
                                    while (x >= 59 && y > 160 && x < 298 && y < 185) {
                                        fillRect(THERMO_HEAT_X - 5, THERMO_HEAT_Y, THERMO_HEAT_X + THERMO_HEAT_WIDTH + 43, THERMO_HEAT_Y + THERMO_HEAT_HEIGHT, RGB(23, 191, 227));
                                        drawLine(THERMO_HEAT_X, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 65535);
                                        drawLine(THERMO_HEAT_X, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
                                        drawLine(THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
                                        fillCircle(x, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);
                                        if (!strcasecmp("C", d->getValue(7))) {
                                            thermoHeatCurrLevel = x - 59;
                                            xhigh = (thermoHeatCurrLevel / 5.6) + 48;
                                            sprintf(t_temp, "%d %d %d\n", thermoHeatCurrLevel, xhigh, (thermoHeatCurrLevel / 5.6));
                                            LOG(Logger::INFO, t_temp);
                                        } else {
                                            thermoHeatCurrLevel = x - 55;
                                            xhigh = (thermoHeatCurrLevel / 6) + 50;
                                        }

                                        format = (format == FORMAT_C) ? FORMAT_C : FORMAT_F;
                                        if (format == FORMAT_F) {
                                            swprintf(opt, 20, L" %d", xhigh);
                                            drawText(opt, x - 20, 158, 40, 1, 0, 14.0f, 65535, 1);
                                        } else {
                                            int celcius = (int) (((xhigh - 32)*5.0) / 9);
                                            swprintf(opt, 20, L" %d", celcius);
                                            drawText(opt, x - 20, 158, 40, 1, 0, 14.0f, 65535, 1);
                                        }

                                        usleep(90000);
                                        x = readX();
                                        y = readY();
                                    }
                                    char temp[10];
                                    int i = xhigh;
                                    sprintf(temp, "%d", i);
                                    connectedHomeSetValue(3, temp);
                                }
                            }
                        } else if (!strcmp(d->getValue(2), "cool")) {
                            if (!strcmp(d->getValue(12), "true")) {
                                if (x >= 59 && y > 130 && x < 298 && y < 155) {//cool slider
                                    while (x >= 59 && y > 130 && x < 298 && y < 155) {
                                        fillRect(THERMO_COOL_X - 5, THERMO_COOL_Y, 303, 155, RGB(23, 191, 227));
                                        drawLine(THERMO_COOL_X, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 298, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 65535);
                                        drawLine(THERMO_COOL_X, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
                                        drawLine(THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
                                        fillCircle(x, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);
                                        if (!strcasecmp("C", d->getValue(7))) {
                                            thermoHeatCurrLevel = x - 59;
                                            xlow = (thermoHeatCurrLevel / 5.6) + 48;
                                        } else {
                                            thermoHeatCurrLevel = x - 55;
                                            xlow = thermoHeatCurrLevel / 6 + 50;
                                        }
                                        format = (format == FORMAT_C) ? FORMAT_C : FORMAT_F;
                                        if (format == FORMAT_F) {
                                            swprintf(opt, 20, L" %d", xlow);
                                            drawText(opt, x - 20, 128, 40, 1, 0, 14.0f, 65535, 1);
                                        } else {
                                            int celcius = (int) (((xlow - 32)*5.0) / 9);
                                            swprintf(opt, 20, L" %d", celcius);
                                            drawText(opt, x - 20, 128, 40, 1, 0, 14.0f, 65535, 1);
                                        }

                                        usleep(90000);
                                        x = readX();
                                        y = readY();
                                    }

                                    char temp[10];
                                    int i = xlow;
                                    sprintf(temp, "%d", i);
                                    connectedHomeSetValue(3, temp);
                                }
                            }
                        }
                        if (!strcmp(d->getValue(15), "true") && strcmp(d->getValue(2), "off")) { // check if has_fan is true or not
                            if (x > 138 && x < 219 && y > 190 && y < 220) { //fan state
                                fillRect(138, 190, 219, 220, BUTTON_LAYER);
                                pngImg.drawPNG(143, 195, 1, BACKGROUND_LAYER, fanImage);
                                if (!strcmp(thermoFanState, "false")) {
                                    drawText(L" : Start", 165, 196, 60, 1, 0, 18.0f, 65535, 0);
                                    connectedHomeSetValue(9, "true");
                                } else if (!strcmp(thermoFanState, "true")) {
                                    drawText(L" : Stop", 165, 196, 60, 1, 0, 18.0f, 65535, 0);
                                    connectedHomeSetValue(9, "false");
                                }
                                waitForPenUp();
                            }
                        }
                    }

                    if (x > THERMO_HEAT_X - 5 && x < 136 && y > 190 && y < 220) { //away
                        fillRect(THERMO_HEAT_X - 5, 190, 136, 220, (strcmp(thermoaway, "away")) ? BUTTON_LAYER : RGB(23, 191, 227));
                        if (!strcmp(thermoaway, "away")) {
                            drawText(L"Home", THERMO_HEAT_X + 5, 198, 50, 1, 0, 18.0f, 65535, 1);
                            connectedHomeSetValue(8, "home");
                        } else if (!strcmp(thermoaway, "home")) {
                            drawText(L"Away", THERMO_HEAT_X + 5, 198, 50, 1, 0, 18.0f, 65535, 1);
                            connectedHomeSetValue(8, "away");
                        } else if (!strcmp(thermoaway, "auto-away")) {
                            drawText(L"Auto-Away", THERMO_HEAT_X + 5, 198, 50, 1, 0, 16.0f, 65535, 1);
                            connectedHomeSetValue(8, "home");
                        } else if (!strcmp(thermoaway, "unknown")) {
                            drawText(L"Unknown", THERMO_HEAT_X + 5, 198, 50, 1, 0, 18.0f, 65535, 1);
                            connectedHomeSetValue(8, "home");
                        }
                        waitForPenUp();
                    }
                }//printf("\nLINE = %d, %s\n", __LINE__, thermoFanState);

            }
                break;
            case NestSmokeDetector:
            {

            }
                break;
            case AlmondSiren:
            {
                if (AlarmButton.checkTouch(x, y)) {
                    checkRadioButtonState(1, TONE_SECTION);
                    return;
                } else if (SirenButton.checkTouch(x, y)) {
                    checkRadioButtonState(2, TONE_SECTION);
                    return;
                } else if (DoorBellButton.checkTouch(x, y)) {
                    checkRadioButtonState(3, TONE_SECTION);
                    return;
                } else if (x > 20 && x < 100 && y > 185 && y < 215) {
                    fillRect(20, 185, 100, 215, BUTTON_LAYER_ONTOUCH);
                    if (StartSiren == false) {
                        drawText(L"Ring", 20, 191, 80, 1, 0, 18.0f, 65535, 1);
                        waitForPenUp();
                        fillRect(20, 185, 100, 215, RGB(23, 191, 227));
                        drawText(L"Ring", 20, 191, 80, 1, 0, 18.0f, 65535, 1);
//                        StartStopSiren_onTouch(true);
                        connectedHomeSetValue(2, "true");
                        //StartSiren = true;
                    } else {
                        drawText(L"Stop", 20, 191, 80, 1, 0, 18.0f, 65535, 1);
                        waitForPenUp();
                        fillRect(20, 185, 100, 215, RGB(23, 191, 227));
                        drawText(L"Stop", 20, 191, 80, 1, 0, 18.0f, 65535, 1);
//                        StartStopSiren_onTouch(false);
                        connectedHomeSetValue(2, "false");
                        // StartSiren = false;
                    }
                } else if (AlmondTimer.checkTouch(x, y)) {
                    return;
                } else if (LowVolume.checkTouch(x, y)) {
                    checkRadioButtonState(1, VOLUME_SECTION);
                    //                    checkSirenEnableState();
                    return;
                } else if (MedVolume.checkTouch(x, y)) {
                    checkRadioButtonState(2, VOLUME_SECTION);
                    //                    checkSirenEnableState();
                    return;
                } else if (HighVolume.checkTouch(x, y)) {
                    checkRadioButtonState(3, VOLUME_SECTION);
                    return;
                }
            }
                break;

            case AlmondBlink:
            {
                Device d(contDevID);
                if (x > 25 && x < 125 && y > 145 && y < 175) {
                    fillRect(25, 145, 125, 175, BUTTON_LAYER_ONTOUCH);
                    drawText(L"Brightness", 25, 151, 100, 1, 0, 18.0f, 65535, 1);
                    waitForPenUp();
                    fillRect(25, 145, 125, 175, RGB(23, 191, 227));
                    drawText(L"Brightness", 25, 151, 100, 1, 0, 18.0f, 65535, 1);
                    drawAlmondBlinkColorStrip();
                } else if (AlmondLedTimer.checkTouch(x, y)) {
                    return;
                } else if (x > 25 && x < 125 && y > 95 && y < 125) {
                    fillRect(25, 95, 125, 125, BUTTON_LAYER_ONTOUCH);
                    if (StartLED == false) {//false
                        drawText(L"Turn Off", 25, 101, 100, 1, 0, 18.0f, 65535, 1);
                        waitForPenUp();
                        fillRect(25, 95, 125, 125, RGB(23, 191, 227));
                        drawText(L"Turn Off", 25, 101, 100, 1, 0, 18.0f, 65535, 1);
                        d.setValue(1, "false");
                        // StartLED = true;
                    } else {//ONLY ELSE present
                        drawText(L"Turn On", 25, 101, 100, 1, 0, 18.0f, 65535, 1);
                        waitForPenUp();
                        fillRect(25, 95, 125, 125, RGB(23, 191, 227));
                        drawText(L"Turn On", 25, 101, 100, 1, 0, 18.0f, 65535, 1);
                        d.setValue(1, "true");
                        // StartLED = false;
                    }
                } else if (x > 25 && x < 125 && y > 190 && y < 220) {
                    fillRect(25, 190, 125, 220, BUTTON_LAYER_ONTOUCH);
                    if (StartBlinkLED == false) {
                        drawText(L"Stop Blinking", 25, 196, 100, 1, 0, 18.0f, 65535, 1);
                        waitForPenUp();
                        fillRect(25, 190, 125, 220, RGB(23, 191, 227));
                        drawText(L"Stop Blinking", 25, 196, 100, 1, 0, 18.0f, 65535, 1);
                        d.setValue(2, "false");
                        //StartBlinkLED = true;
                    } else {
                        drawText(L"Blink", 25, 196, 100, 1, 0, 18.0f, 65535, 1);
                        waitForPenUp();
                        fillRect(25, 190, 125, 220, RGB(23, 191, 227));
                        drawText(L"Blink", 25, 196, 100, 1, 0, 18.0f, 65535, 1);
                        d.setValue(2, "true");
//                        LedBlinkStart_onTouch();
                        // StartBlinkLED = false;
                    }
                }
            }
                break;

                
            case ZigbeeCamImage:
            {
                btnImageCount.checkTouch(x, y);
                btnResolution.checkTouch(x, y);
                btnCapture.checkTouch(x, y);
            }
                break;
            case MultiSwitch:
            {
                if (MultiSwitchOnButton.checkTouch(x, y)) {
                    return;
                } else if (MultiSwitchOffButton.checkTouch(x, y)) {
                    return;
                } else if (MultiSwitchOnButton1.checkTouch(x, y)) {
                    return;
                } else if (MultiSwitchOffButton1.checkTouch(x, y)) {
                    return;
                }
            }
                break;
            case ZBIRExtender:
            {
                if ((irApplianceSelected = 1) && ApplianceOne.checkTouch(x, y)) {
                    return;
                } else if ((irApplianceSelected = 2) && ApplianceTwo.checkTouch(x, y)) {
                    return;
                } else if ((irApplianceSelected = 3) && ApplianceThree.checkTouch(x, y)) {
                    return;
                } else if ((irApplianceSelected = 4) && ApplianceFour.checkTouch(x, y)) {
                    return;
                } else if ((irApplianceSelected = 5) && ApplianceFive.checkTouch(x, y)) {
                    return;
                } else if ((irApplianceSelected = 6) && ApplianceSix.checkTouch(x, y)) {
                    return;
                } else if (ChangeApplianceName.checkTouch(x, y)) {
                    return;
                }
            }
                break;
            default:
            {
                Device d(contDevID);
                if (d.getDeviceTechnology() == ZigBeeTechnology) {
                } else {
                    basicSetButton.checkTouch(x, y);
                    basicSetText.checkTouch(x, y);
                    basicGetButton.checkTouch(x, y);
                    basicGetTextBox.checkTouch(x, y);
                    basicSetTextModifyPlus.checkTouch(x, y);
                    basicSetTextModifyMinus.checkTouch(x, y);
                }
            }
                break; //Unknown Device Type";
        }
        memset(opt, 0, sizeof opt);
    } else if (MODE == DEVICE_MODE) {
        
        if (x > 282 && x < 317 && y > 47 && y < 83) { //close button
            //readArea(0, 0, 319, 239);
            ConMode = false;
            ConValFlag = false; //confignewlyadded
            fillRect(287, 57, 307, 77, RR_COLOR);
            pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossonTouch);
            waitForPenUp();
            fillRect(287, 57, 307, 77, RR_COLOR);
            pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossdefault);
            if (device_key == ZWtoACIRExtender || (device_key == MultiSoundSiren && MultiSoundFlag)) {
                MultiSoundFlag = false;
                drawControlDevice(*getDevice(contDevID));
                MODE = CONTROL_MODE;
            } else {
                EditButton_onTouch();
                MODE = KEY_MODE;
            }
            LOG(Logger::INFO, "DEVICE_MODE");
            strcpy(ConfigParam, "");
            strcpy(ConfigSet, "");
            strcpy(ConfigValue, "");
            ParameterFlag = false, ValueFlag = false, ValueSizeFlag = false;
            tempvaluesize = 0;
            return;
        }

        if ((device_key != ZWtoACIRExtender) && (ConMode == true)) { //configuration settings

            ParameterSetText.checkTouch(x, y);
            if (ParameterFlag) {
                if (ValueSizeSetTextModifyMinus.checkTouch(x, y)) {
                    return;
                } else if (ValueSizeSetTextModifyPlus.checkTouch(x, y)) {
                    return;
                }
            }
            if (ParameterFlag && ValueSizeFlag) {
                ValueSetText.checkTouch(x, y);
            }
            // SetConfigurationButton.checkTouch(x, y);
            if (ConValFlag == true && SetConfigurationButton.checkTouch(x, y)) {
                readArea(0, 0, 319, 239);
                Modal ConfmW;
                ConValFlag = false;
                ConFlag = false;
                ConfmW.setLine1(L"Sending ");
                ConfmW.setLine2(L"Configuration Values.... ");
                ConfmW.modalReturn = &drawArea;
                ConfmW.draw(MODAL_TYPE_INFORMATION, 5);
                ConValFlag = false;
                ConFlag = false;
                LOG(Logger::INFO, "Sending configuration value");
            }

        } else if ((device_key == ZWtoACIRExtender) && (ConMode == true)) {
            LOG(Logger::PRINT, "ZWtoACIRExtender....");
            IRCodeSetText.checkTouch(x, y);
            ParameterSetText.checkTouch(x, y);
            if (ParameterFlag) {
                if (ValueSizeSetTextModifyMinus.checkTouch(x, y)) {
                    return;
                } else if (ValueSizeSetTextModifyPlus.checkTouch(x, y)) {
                    return;
                }
            }
            if (ParameterFlag && ValueSizeFlag) {
                ValueSetText.checkTouch(x, y);
            }
            if ((IRCodeFlag == true) && (IRCodeSetButton.checkTouch(x, y))) {//confignewlyadded
                IRCodeFlag = false;
            }
            if ((ConValFlag == true) && (SetConfigurationButton.checkTouch(x, y))) {//confignewlyadded
                ConValFlag = false;
                ConFlag = false;
                LOG(Logger::INFO, "Sending configuration value");
            }

        } else if (device_key == MultiSoundSiren) {
            if (EmergencySoundButton.checkTouch(x, y)) {
                MultiSoundFlag = false;
                soundtype = 1;
                SelectedSoundType(soundtype);
            }
            if (FireSoundButton.checkTouch(x, y)) {
                MultiSoundFlag = false;
                soundtype = 2;
                SelectedSoundType(soundtype);
            }
            if (AmbulanceSoundButton.checkTouch(x, y)) {
                MultiSoundFlag = false;
                soundtype = 3;
                SelectedSoundType(soundtype);
            }
            if (PoliceSoundButton.checkTouch(x, y)) {
                MultiSoundFlag = false;
                soundtype = 4;
                SelectedSoundType(soundtype);
            }
            if (DoorSoundButton.checkTouch(x, y)) {
                MultiSoundFlag = false;
                soundtype = 5;
                SelectedSoundType(soundtype);
            }
            if (BeepSoundButton.checkTouch(x, y)) {
                MultiSoundFlag = false;
                soundtype = 6;
                SelectedSoundType(soundtype);
            }
        }
    } else if (MODE == COLOR_MODE) {

        Device *d = getDevice(contDevID);
        if ((x > 25 && x < 301) && (y > 110 && y < 150)) {
            int sat;
            while ((x > 25 && x < 301) && (y > 110 && y < 150)) {
                fillRect(25, RECT_Y, 300, RECT_Y + RECT_HEIGHT + 5, RGB(23, 191, 227));
                drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2) + 2, 301 - 5, RECT_Y + (RECT_HEIGHT / 2) + 2, 65535);
                drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 3) + 1, 25 + 5, RECT_Y + ((RECT_HEIGHT * 2) / 3) + 1, 65535);
                drawLine(301 - 5, RECT_Y + (RECT_HEIGHT / 3), 301 - 5, RECT_Y + ((RECT_HEIGHT * 2) / 3) + 1, 65535);
                if (x > 289) {
                    fillCircle(290, RECT_Y + (RECT_HEIGHT / 2) + 2, 5, 65535);
                    sat = 254;
                } else if (x < 36) {
                    fillCircle(35, RECT_Y + (RECT_HEIGHT / 2) + 2, 5, 65535);
                    sat = 0;
                } else {
                    fillCircle(x, RECT_Y + (RECT_HEIGHT / 2) + 2, 5, 65535);
                    sat = (x - 35);
                }
                usleep(50000);
                x = readX();
                y = readY();
            }
//            setbulbsaturation(sat);
            connectedHomeSetValue(4, sat);
        } else if ((x > 25 && x < 301) && (y > 180 && y < 222)) {
            if (d->getDeviceTechnology() == ZigBeeTechnology) {
                int h_val;
                if (x > 25 && x < 280) {
                    while ((x > 25 && x < 280) && (y > 180 && y < 222)) {
                        pngImg.drawPNG(25, RECT_Y + (RECT_HEIGHT / 2) + 55, 1, 0, color_png);
                        fillRect(280, 181, 300, 221, RGB(255, 255, 255));
                        drawLine(x, RECT_Y + (RECT_HEIGHT / 2) + 55, x, RECT_Y + (RECT_HEIGHT / 2) + 95, 0);
                        h_val = (x - 25);
                        usleep(50000);
                        x = readX();
                    }
                    if (h_val <= 255)
                        connectedHomeSetValue(3, h_val);
                } else {
                    waitForPenUp();
                    setbulbhueandsat(0, 0);
                }
            } else if (d->getDeviceTechnology() == HueTechnology) {
                int newCol;
                if ((x > 25 && x < 280) && (y > 180 && y < 222)) {
                    while (x >= 25 && y >= 180 && x <= 280 && y <= 222) {
                        pngImg.drawPNG(25, RECT_Y + (RECT_HEIGHT / 2) + 55, 1, 0, color_png);
                        fillRect(280, 181, 300, 221, RGB(255, 255, 255));
                        newCol = getColorFromScreen(x, y);
                        drawLine(x, RECT_Y + (RECT_HEIGHT / 2) + 55, x, RECT_Y + (RECT_HEIGHT / 2) + 95, 0);
                        x = readX();
                        y = readY();
                        usleep(90000);
                    }
                    if (strstr(d->getValue(2), "true")) {
                        d->setColorBulb(newCol);
                    }
                }
                if (x > 280 && x < 300 && y > 180 && y < 222) {
                    waitForPenUp();
                    pngImg.drawPNG(25, RECT_Y + (RECT_HEIGHT / 2) + 55, 1, 0, color_png);
                    fillRect(280, 181, 300, 221, RGB(255, 255, 255));
                    drawLine(x, RECT_Y + (RECT_HEIGHT / 2) + 55, x, RECT_Y + (RECT_HEIGHT / 2) + 95, 0);
//                    setbulbsaturation(0);
                    connectedHomeSetValue(4, "0");
                }
            }
        } else if (x > 282 && x < 317 && y > 47 && y < 83) {
            waitForPenUp();
            fillRect(287, 57, 307, 77, RR_COLOR);
            pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossonTouch);
            drawControlDevice(contDevID);
            MODE = CONTROL_MODE;
            return;
        }
    } else if (MODE == KEY_MODE) {
        if (x > 282 && x < 317 && y > 47 && y < 83) {
            fillRect(287, 57, 307, 77, RR_COLOR);
            pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossonTouch);
            waitForPenUp();
            drawControlDevice(contDevID);
            MODE = CONTROL_MODE;
            return;
        }
        if (En_Key) {
            Device d(contDevID);
            LocationBox->checkTouch(x, y);
            NameBox->checkTouch(x, y);
            DoneButton->checkTouch(x, y);
            if (d.getDeviceTechnology() != AlmondTechnology) {
                if ((d.getDeviceTechnology() == ZWaveTechnology) && (d.getDeviceType() != ZWtoACIRExtender) && ConfigurationButton1.checkTouch(x, y)) {
                    LOG(Logger::INFO, "======En_Key ConfigurationButton1 touch==========");
                } else {
                    LOG(Logger::INFO, "======En_Key RemovedeviceButton touch==========");
                    RemovedeviceButton.checkTouch(x, y);
                }
            }
            return;
        }
    } else if (MODE == RETAIN_MODE) {
        if (updatetouchflag)
            upgradeButton.checkTouch(x, y);
        else
            checkforupgradeButton.checkTouch(x, y);

        if (RetainButton.checkTouch(x, y)) {
            return;
        }
        if (x > 282 && x < 317 && y > 47 && y < 83) {
            fillRect(287, 57, 307, 77, RR_COLOR);
            pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossonTouch);
            waitForPenUp();
            drawControlDevice(contDevID);
            MODE = CONTROL_MODE;
            updates_available = 0;
            updatetouchflag = false;
            return;
        }
    } else if (MODE == UPGRADE_MODE) {
        if (x > 282 && x < 317 && y > 47 && y < 83) {
            fillRect(287, 57, 307, 77, RR_COLOR);
            pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossonTouch);
            waitForPenUp();
            drawControlDevice(contDevID);
            MODE = CONTROL_MODE;
            updates_available = 0;
            updatetouchflag = false;
            return;
        }
    } else if (MODE == REMOVE_MODE) {
        YesButton.checkTouch(x, y);
        NoButton.checkTouch(x, y);
    } else if (MODE == GENERIC_MODE) {
        if (x > 282 && x < 317 && y > 47 && y < 83) { //x > 413 && x < 435 && y > 42 && y < 64
            fillRect(287, 57, 307, 77, RR_COLOR);
            pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossonTouch);
            waitForPenUp();
            gen.p_area = 0;
            multiswitch = false;
            drawControlDevice(*getDevice(contDevID));
            MODE = CONTROL_MODE;
            return;
        }
        genericControlDeviceTouch(x, y);
    } else if (MODE == DEVICE_MODE) {
        if ((x > 282 && x < 317 && y > 47 && y < 83) && ConMode == false) {
            fillRect(287, 57, 307, 77, RR_COLOR);
            pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossonTouch);
            waitForPenUp();
            if (device_key == GenericDevice) {
                MODE = CONTROL_MODE;
                gen.p_area = 0.0;
            }
            drawControlDevice(*getDevice(contDevID));
            MODE = CONTROL_MODE;
        } else if (device_key == MultiSoundSiren || device_key == GenericDevice) {
            LOG(Logger::PRINT, "Sending multisound value");
            if (EmergencySoundButton.checkTouch(x, y)) {
                soundtype = 1;
                SelectedSoundType(soundtype);
            }
            if (FireSoundButton.checkTouch(x, y)) {
                soundtype = 2;
                SelectedSoundType(soundtype);
            }
            if (AmbulanceSoundButton.checkTouch(x, y)) {
                soundtype = 3;
                SelectedSoundType(soundtype);
            }
            if (PoliceSoundButton.checkTouch(x, y)) {
                soundtype = 4;
                SelectedSoundType(soundtype);
            }
            if (DoorSoundButton.checkTouch(x, y)) {
                soundtype = 5;
                SelectedSoundType(soundtype);
            }
            if (BeepSoundButton.checkTouch(x, y)) {
                soundtype = 6;
                SelectedSoundType(soundtype);
            }
        }
    } else if (MODE == BLINK_COLOR_MODE) {

        if (x > 26 && x < 280 && y > 145 && y < 185) {
            int x_cor = 0;
            while (x > 26 && x < 280 && y > 145 && y < 185) {
                pngImg.drawPNG(25, 145, 1, 0, color_shades);
                drawLine(x - 2, 145, x - 2, 185, 0);
                x_cor = x;
                x = readX();
                y = readY();
                usleep(90000);
            }
            //           fullLedCol = getFullHue(presentColor);
            /*presentColor = */findNewShadeOfColor(presentColor, x_cor - 25);
        }

        if (x > 282 && x < 317 && y > 47 && y < 83) {
            fillRect(287, 57, 307, 77, RR_COLOR);
            pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossonTouch);
            waitForPenUp();
            drawControlDevice(contDevID);
            MODE = CONTROL_MODE;
            return;
        }
    } else if (MODE == IR_OPERATION_MODE) {
        if ((irAppButtonSelected = 1) && AppButtonOne.checkTouch(x, y)) {
            return;
        } else if ((irAppButtonSelected = 2) && AppButtonTwo.checkTouch(x, y)) {
            return;
        } else if ((irAppButtonSelected = 3) && AppButtonThree.checkTouch(x, y)) {
            return;
        } else if ((irAppButtonSelected = 4) && AppButtonFour.checkTouch(x, y)) {
            return;
        } else if ((irAppButtonSelected = 5) && AppButtonFive.checkTouch(x, y)) {
            return;
        } else if ((irAppButtonSelected = 6) && AppButtonSix.checkTouch(x, y)) {
            return;
        } else if ((irAppButtonSelected = 7) && AppButtonSeven.checkTouch(x, y)) {
            return;
        } else if ((irAppButtonSelected = 8) && AppButtonEight.checkTouch(x, y)) {
            return;
        } else if ((irApplianceSelected < 6) && SelectIRLed.checkTouch(x, y)) {
            return;
        } else if (ChangeButtonName.checkTouch(x, y)) {
            return;
        }

        if (x > 282 && x < 317 && y > 47 && y < 83) {
            fillRect(287, 57, 307, 77, RR_COLOR);
            pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossonTouch);
            waitForPenUp();
            drawControlDevice(contDevID);
            MODE = CONTROL_MODE;
            return;
        }
    } else if (MODE == IR_LED_SELECTION_MODE) {
        if ((irAppLedSelected = 1) && IRLed1->checkTouch(x, y)) {
            return;
        } else if ((irAppLedSelected = 2) && IRLed2->checkTouch(x, y)) {
            return;
        } else if ((irAppLedSelected = 3) && IRLed3->checkTouch(x, y)) {
            return;
        } else if ((irAppLedSelected = 4) && IRLed4->checkTouch(x, y)) {
            return;
        } else if ((irAppLedSelected = 5) && IRLed5->checkTouch(x, y)) {
            return;
        } else if ((irAppLedSelected = 6) && IRLed6->checkTouch(x, y)) {
            return;
        } else if (selectIRLedDone.checkTouch(x, y)) {
            return;
        }

        if (x > 282 && x < 317 && y > 47 && y < 83) {
            fillRect(287, 57, 307, 77, RR_COLOR);
            pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossonTouch);
            waitForPenUp();
            delete IRLed1, IRLed2, IRLed3;
            delete IRLed4, IRLed5, IRLed6;
            drawArea();
            MODE = IR_OPERATION_MODE;
            return;
        }
    } else if (MODE == EDIT_APPLIANCE_NAME) {
        waitForPenUp();
        if (x > 25 && x < 110 && y > 95 && y < 125) {
            EditApplianceNameOnTouch(1);
            return;
        } else if (x > 120 && x < 205 && y > 95 && y < 125) {
            EditApplianceNameOnTouch(2);
            return;
        } else if (x > 215 && x < 300 && y > 95 && y < 125) {
            EditApplianceNameOnTouch(3);
            return;
        } else if (x > 25 && x < 110 && y > 135 && y < 165) {
            EditApplianceNameOnTouch(4);
            return;
        } else if (x > 120 && x < 205 && y > 135 && y < 165) {
            EditApplianceNameOnTouch(5);
            return;
        } else if (x > 215 && x < 300 && y > 135 && y < 165) {
            EditApplianceNameOnTouch(6);
            return;
        } else if (selectIRLedDone.checkTouch(x, y)) {
            return;
        }

    } else if (MODE == EDIT_BUTTON_NAME) {
        waitForPenUp();
        if (x > 25 && x < 85 && y > 95 && y < 125) {
            EditIRButtonEventNameOnTouch(1);
        } else if (x > 95 && x < 155 && y > 95 && y < 125) {
            EditIRButtonEventNameOnTouch(2);
        } else if (x > 165 && x < 225 && y > 95 && y < 125) {
            EditIRButtonEventNameOnTouch(3);
        } else if (x > 235 && x < 295 && y > 95 && y < 125) {
            EditIRButtonEventNameOnTouch(4);
        } else if (x > 25 && x < 85 && y > 135 && y < 165) {
            EditIRButtonEventNameOnTouch(5);
        } else if (x > 95 && x < 155 && y > 135 && y < 165) {
            EditIRButtonEventNameOnTouch(6);
        } else if (x > 165 && x < 225 && y > 135 && y < 165) {
            EditIRButtonEventNameOnTouch(7);
        } else if (x > 235 && x < 295 && y > 135 && y < 165) {
            EditIRButtonEventNameOnTouch(8);
        } else if (selectIRLedDone.checkTouch(x, y)) {
            return;
        }

    } else if (MODE == ALMOND_LIST_MODE) {
        Irlst->getTouch(x, y);
        if(x > 4 && x < 50 && y > 4 && y < 50) {
            fillRect(5, 5, 45, 45, 0);
            pngImg.drawPNG(5, 5, 1, 0, backButtonPressImg);
            waitForPenUp();
            drawArea();
            MODE = preMode;
            if(preMode == KEY_MODE) {
                delete Irlst;
                free(deviceLocation);
            }
        }
    }
}

void genericControlDeviceTouch(int x, int y) {
    sprintf(t_temp, "In on button endpnt x %d y %d", x, y);
    LOG(Logger::INFO, t_temp);
    if (y > 90 && y < 123) { //Row1
        waitForPenUp();
        if (x > 10 && x < 110) {
            genericTouchHandler(grid1[0], x, y);
        } else if (x > 110 && x < 210) {
            genericTouchHandler(grid1[1], x, y);
        } else if (x > 210 && x < 310) {
            genericTouchHandler(grid1[2], x, y);
        }
    } else if (y > 123 && y < 156) { //Row2
        waitForPenUp();
        if (x > 10 && x < 110) {
            genericTouchHandler(grid2[0], x, y);
        } else if (x > 110 && x < 210) {
            genericTouchHandler(grid2[1], x, y);
        } else if (x > 210 && x < 310) {
            genericTouchHandler(grid2[2], x, y);
        }
    } else if (y > 156 && y < 189) { //Row3
        //        LOG(Logger::INFO,"Row 3 touched");
        waitForPenUp();
        if (x > 10 && x < 110) {
            genericTouchHandler(grid3[0], x, y);
        } else if (x > 110 && x < 210) {
            genericTouchHandler(grid3[1], x, y);
        } else if (x > 210 && x < 310) {
            genericTouchHandler(grid3[2], x, y);
        }
    }

    if ((tamper == true) && (x > 15 && y > 190 && x < 95 && y < 220)) {
        Device *d = getDevice(contDevID);
        if (d) {
            if (strcmp(d->getValue("TAMPER"), "true") == 0) {
                TamperButton.checkTouch(x, y);
            }
        }
    } else if ((more == true) && (x > 120 && y > 190 && x < 200 && y < 220)) {
        LOG(Logger::INFO, "genericControlDeviceTouch morebutton check...");
        waitForPenUp();
        MoreButton.checkTouch(x, y);
    }
    gen.x_present = gen.y_present = 0;
}

int getEndpointforSwitchBinary(int indexen, int y, int diff) {
    Device *d = getDevice(contDevID);
    int endpt = 0;
    /*****y is used as a index number for callback else it's y-coordinate.****/

    if ((grid1[0] == indexen) && (grid2[0] == indexen) && (grid3[0] == indexen)) {
        if (diff == GENERIC_CALLBACK) {
            endpt = d->getEndPointValue(y);
            if (endpt == 1) {
                gen.y_prev23 = 94;
            } else if (endpt == 2) {
                gen.y_prev23 = 130;
            } else if (endpt == 3) {
                gen.y_prev23 = 160;
            }
            return 0;
        } else if (diff == GENERIC_TOUCH) {
            if (y == 94) {
                return 1;
            } else if (y == 126) {
                return 2;
            } else if (y == 158) {
                return 3;
            }
        }
    } else if ((grid1[0] == indexen) && (grid2[0] == indexen)) {
        if (diff == GENERIC_CALLBACK) {
            endpt = d->getEndPointValue(y);
            if (endpt == 1)
                gen.y_prev23 = 100;
            else {
                if ((endpt == 2) && (d->getDeviceTechnology() == ZWaveTechnology))
                    gen.y_prev23 = 100;
                else
                    gen.y_prev23 = 130;
            }
            return 0;
        } else if (diff == GENERIC_TOUCH) {
            if (y == 94)
                return 1;
            else {
                if (d->getDeviceTechnology() == ZigBeeTechnology)
                    return 2;
                else if (d->getDeviceTechnology() == ZWaveTechnology)
                    return 3;
            }
        }
    } else if ((grid2[0] == indexen) && (grid3[0] == indexen)) {
        if (diff == GENERIC_CALLBACK) {
            endpt = d->getEndPointValue(y);
            if (endpt == 1)
                gen.y_prev23 = 130;
            else {
                if ((endpt == 2) && (d->getDeviceTechnology() == ZWaveTechnology))
                    gen.y_prev23 = 130;
                else
                    gen.y_prev23 = 160;
            }
            return 0;
        } else if (diff == GENERIC_TOUCH) {
            if (y == 126)
                return 1;
            else {
                if (d->getDeviceTechnology() == ZigBeeTechnology)
                    return 2;
                else if (d->getDeviceTechnology() == ZWaveTechnology)
                    return 3;
            }
        }
    } else if ((grid1[0] == indexen) && (grid3[0] == indexen)) {
        if (diff == GENERIC_CALLBACK) {
            endpt = d->getEndPointValue(y);
            if (endpt == 1)
                gen.y_prev23 = 100;
            else {
                if ((endpt == 2) && (d->getDeviceTechnology() == ZWaveTechnology))
                    gen.y_prev23 = 100;
                else
                    gen.y_prev23 = 160;
            }
            return 0;
        } else if (diff == GENERIC_TOUCH) {
            if (y == 94)
                return 1;
            else {
                if (d->getDeviceTechnology() == ZigBeeTechnology)
                    return 2;
                else if (d->getDeviceTechnology() == ZWaveTechnology)
                    return 3;
            }
        }
    } else {
        return (d->getEndPointValue(d->getIndexNoFromName("SWITCH BINARY", -1)));
    }
}

void genericTouchHandler(unsigned char indx, int x, int y) {
    unsigned int indextp = indx, endpt;
    unsigned int indexNo;
    char indexName[30] = "";
    int x_star, y_star;
    float areath = 0.0;
    gen.y_prev23 = y;
    areath = getRequiredIndexArea(indextp);
    getDrawingCorditforCallback(areath, indextp, GENERIC_TOUCH);
    x_star = gen.x_present;
    y_star = gen.y_present;
    sprintf(t_temp, "x_star:%d y_star:%d", x_star, y_star);
    LOG(Logger::INFO, t_temp);
    Device d(contDevID);
    IndexTypeEnumToStr((enum IndexType)indextp, indexName, 0);
    indexNo = d.getIndexNoFromName(indexName, -1);
    switch (indx) {
        case SwitchBinary:
        {
            
            endpt = getEndpointforSwitchBinary(indextp, y_star, GENERIC_TOUCH);
            sprintf(t_temp, "In on button endpnt%d indextp %d y_star %d", endpt, indextp, y_star);
            LOG(Logger::INFO, t_temp);
            if (x > x_star + 100 && x < x_star + 180 && y > y_star && y < y_star + 29) {
                fillRect(x_star + 100, y_star, x_star + 180, y_star + 29, BUTTON_LAYER);
                drawText(lan.get("on_1", "common"), x_star + 100, y_star + 5, 80, 1, 0, 20.0f, 65535, 1);
                fillRect(x_star + 200, y_star, x_star + 280, y_star + 29, RGB(23, 191, 227));
                drawText(lan.get("off_1", "common"), x_star + 200, y_star + 5, 80, 1, 0, 20.0f, 65535, 1);
                connectedHomeSetValue(indexNo, "true");
            } else if (x > x_star + 200 && x < x_star + 280 && y > y_star && y < y_star + 30) {
                sprintf(t_temp, "In off button endpnt%d", endpt);
                LOG(Logger::INFO, t_temp);
                fillRect(x_star + 100, y_star, x_star + 180, y_star + 29, RGB(23, 191, 227));
                drawText(lan.get("on_1", "common"), x_star + 100, y_star + 5, 80, 1, 0, 20.0f, 65535, 1);
                fillRect(x_star + 200, y_star, x_star + 280, y_star + 29, BUTTON_LAYER);
                drawText(lan.get("off_1", "common"), x_star + 200, y_star + 5, 80, 1, 0, 20.0f, 65535, 1);
                connectedHomeSetValue(indexNo, "false");
            }
            sprintf(t_temp, "SwitchBinary  endpoint:%d ", endpt);
            LOG(Logger::INFO, t_temp);
        }
            break;
        case SwitchMultilevel:
        {
            sprintf(t_temp, "Switch Multilevel x:%d y:%d ", x, y);
            LOG(Logger::INFO, t_temp);
            dimmerCurrLevel = -1;
            while (x > x_star + 60 && y > y_star && x < x_star + 260 && y < y_star + 29) {
                fillRect(x_star + 50, y_star, x_star + 270, y_star + 29, RGB(23, 191, 227));
                drawLine(x_star + 55, y_star + 15, x_star + 265, y_star + 15, 65535);
                drawLine(x_star + 55, y_star + 7, x_star + 55, y_star + 23, 65535);
                drawLine(x_star + 265, y_star + 7, x_star + 265, y_star + 23, 65535);
                if (x > x_star + 260) {
                    fillCircle(260, y_star + 15, 5, 65535);
                    dimmerCurrLevel = 100;
                } else if (x < x_star + 60) {
                    fillCircle(x_star + 60, y_star + 15, 5, 65535);
                    dimmerCurrLevel = 0;
                } else {
                    fillCircle(x, y_star + 15, 5, 65535);
                    dimmerCurrLevel = (x - (x_star + 60));
                    dimmerCurrLevel = dimmerCurrLevel / 2;

                }
                usleep(90000);
                x = readX();
                y = readY();
            }
            if (dimmerCurrLevel != -1) {
//                multilevelGoToLevel(dimmerCurrLevel);
                connectedHomeSetValue(indexNo, dimmerCurrLevel);
            }
        }
            break;
        case SwitchMultiSound:
        {
            if (x > x_star + 20 && x < x_star + 160 && y > y_star && y < y_star + 30) {
                device_key = contDev;
                MODE = DEVICE_MODE;
                drawRoundedRectangle(10, 55, 310, 230, RR_COLOR, 255);
                pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossdefault);
                EmergencySoundButton.draw();
                FireSoundButton.draw();
                AmbulanceSoundButton.draw();
                PoliceSoundButton.draw();
                DoorSoundButton.draw();
                BeepSoundButton.draw();
            } else if (x > x_star + 170 && x < x_star + 280 && y > y_star && y < y_star + 30) {
//                Device *d = getDevice(contDevID);
//                if (d && d->getDeviceTechnology() == ZWaveTechnology) {
                    fillRect(gen.x_present + 170, gen.y_present, gen.x_present + 280, gen.y_present + 28, RGB(255, 0, 0));
                    swprintf(opt, 179, L"%ls", lan.get("stop", "connected_sensor"));
                    drawText(opt, gen.x_present + 180, gen.y_present + 2, 100, 1, 0, 27.0f, 65535, 1);
                    connectedHomeSetValue(indexNo, "0");
//                    d->setSwitchLevel(0);
//                }
            }
        }
            break;

        case ShutterLevel:
        {
            if (x > x_star + 10 && x < x_star + 90 && y > y_star && y < y_star + 30) {
//                multilevelGoToLevel(99);
                connectedHomeSetValue(indexNo, "99");
            } else if (x > x_star + 100 && x < x_star + 180 && y > y_star && y < y_star + 30) {
                connectedHomeSetValue(indexNo, "0");
//                multilevelOff_onTouch();
            }

        }
            break;

        case BasicExtender:
        {
            if (x > x_star + 10 && x < x_star + 90 && y > y_star && y < y_star + 30) {
                if (strcmp(d.getValue("BASIC_EXTENDER"), "255") == 0) {
                    fillRect(x_star + 10, y_star, x_star + 90, y_star + 28, BUTTON_LAYER);
                    drawText(lan.get("off_1", "common"), x_star, y_star, 90, 1, 0, 30.0f, 65535, 1);
//                    basicSetValue(0);
                    connectedHomeSetValue(indexNo, "0");
                } else {
                    fillRect(x_star + 10, y_star, x_star + 90, y_star + 28, BUTTON_LAYER);
                    drawText(lan.get("on_1", "common"), x_star, y_star, 90, 1, 0, 30.0f, 65535, 1);
//                    basicSetValue(255);
                    connectedHomeSetValue(indexNo, "255");
                }
            }

        }
            break;

        case LockState:
        {
            if (x > x_star + 110 && x < x_star + 190 && y > y_star && y < y_star + 30) {
                fillRect(x_star, y_star, x_star + 100, y_star + 30, RR_COLOR);
                aswprintf(opt, 179, L"%ls", L"Processing");
                drawText(opt, x_star, y_star + 7, 100, 1, 0, 20.0f, 65535, 1);
                connectedHomeSetValue(indexNo, "255");
//                doorLock_onTouch();
            } else if (x > x_star + 200 && x < x_star + 280 && y > y_star && y < y_star + 30) {
                fillRect(x_star, y_star, x_star + 100, y_star + 30, RR_COLOR);
                aswprintf(opt, 179, L"%ls", L"Processing");
                drawText(opt, x_star, y_star + 7, 100, 1, 0, 20.0f, 65535, 1);
                connectedHomeSetValue(indexNo, "0");
//                doorUnlock_onTouch();
            }
        }
            break;

        case MaximumUsers:
        {
            int UsrNum = 0;
//            if (d) {
                UsrNum = atoi(d.getValue("MAXIMUM_USERS"));
                userind = d.getIndexNoFromName("USER_CODE", -1);
                if ((UsrNum > 0) && ((x > x_star + 45 && x < x_star + 70 && y > y_star && y < y_star + 30) || (x > x_star + 105 && x < x_star + 130 && y > y_star && y < y_star + 30))) {
                    if ((Userno > 1) && (x < x_star + 70))//arrow_minus
                        Userno--;
                    else if ((Userno < UsrNum) && (x > x_star + 105))//arrow_plus
                        Userno++;
                    else
                        break;
                    fillRect(x_star + 75, y_star, x_star + 95, y_star + 30, RR_COLOR);
                    swprintf(opt, 179, L"%d", Userno);
                    drawText(opt, x_star + 75, y_star + 3, 20, 1, 0, 20.0f, 65535, 1);
                    fillRect(gen.x_present + 135, gen.y_present, gen.x_present + 235, gen.y_present + 30, RR_COLOR);

                    if (!strcmp(d.getValue(Userno + (userind - 1)), "")) {
                        sprintf(Userpin, "%ls", lan.get("unassigned", "connected_sensor"));
                        aswprintf(opt, 179, L"%s", Userpin);
                        drawText(opt, gen.x_present + 135, gen.y_present + 3, 100, 1, 0, 20.0f, 65535, 0);
                    } else {
                        strcpy(Userpin, d.getValue(Userno + (userind - 1)));
                        aswprintf(opt, 179, L"%s", Userpin);
                        drawText(opt, gen.x_present + 135, gen.y_present + 3, 100, 1, 0, 25.0f, 65535, 0);
                    }
                } else if (x > x_star + 235 && x < x_star + 265 && y > y_star && y < y_star + 30) {
                    PinboxonTouch();
                } else if (x > x_star + 265 && x < x_star + 280 && y > y_star && y < y_star + 30) {
                    strcpy(Userpin, "");
                    connectedHomeSetValue(userind + (Userno - 1), Userpin);
//                    d.setUserCode(Userpin, Userno);
                }
//            }
        }
            break;

        case ThermostatSetpoint:
        {
            float er = 0;
            while (x > x_star + 75 && y > y_star && x < x_star + 275 && y < y_star + 30) {
                if (x > x_star + 75 && x < x_star + 275) {
                    fillRect(x_star + 65, y_star, x_star + 285, y_star + 28, RGB(23, 191, 227));
                    drawLine(x_star + 70, y_star + 15, x_star + 280, y_star + 15, 65535);
                    drawLine(x_star + 70, y_star + 7, x_star + 70, y_star + 23, 65535);
                    drawLine(x_star + 280, y_star + 7, x_star + 280, y_star + 23, 65535);
                    fillCircle(x, y_star + 15, 5, 65535);
                    thermoHeatCurrLevel = (x - (x_star + 75));
                    thermoHeatCurrLevel = thermoHeatCurrLevel / 5;
                    er = (thermoHeatCurrLevel * (9.0 / 5)) + 32;
                    swprintf(opt, 20, L" %.0f", er);
                    drawText(opt, x - 38, y_star, 70, 1, 0, 20.0f, 65535, 1);
                }
                usleep(90000);
                x = readX();
                y = readY();
//                setThermoHeatCurrLevel(thermoHeatCurrLevel);
                connectedHomeSetValue(indexNo, thermoHeatCurrLevel);
            }
        }
            break;

        case UpDown:
        {
            if (x > x_star + 10 && x < x_star + 90 && y > y_star && y < y_star + 30) {
//                basicSetValue(99);
                connectedHomeSetValue(indexNo, "99");
            } else if (x > x_star + 100 && x < x_star + 180 && y > y_star && y < y_star + 30) {
//                basicSetValue(0);
                connectedHomeSetValue(indexNo, "0");
            }
        }
            break;

        case Stop:
        {
//            stopShutterRolling_onTouch();
            connectedHomeSetValue(indexNo, NULL);
        }
            break;

        case BarrierOperator:
        {
            if (x > x_star + 110 && x < x_star + 190 && y > y_star && y < y_star + 30) {
//                GarageDoorOpen_onTouch();
                connectedHomeSetValue(indexNo, "255");
            } else if (x > x_star + 200 && x < x_star + 280 && y > y_star && y < y_star + 30) {
//                GarageDoorClose_onTouch();
                connectedHomeSetValue(indexNo, "0");
            }
        }
            break;

//        case AcSetpointHeating://newgeneric
//        {
//            int SetTemp = 0, temp;
//            if (x > x_star + 70 && y > y_star && x < x_star + 266 && y < y_star + 30) {
//                while (x > x_star + 70 && y > y_star && x < x_star + 266 && y < y_star + 30) {
//                    if (x > (70) && x < (266)) {
//                        fillRect(x_star + 60, y_star, x_star + 276, y_star + 28, RGB(23, 191, 227));
//                        drawLine(x_star + 65, y_star + 15, x_star + 271, y_star + 15, 65535);
//                        drawLine(x_star + 65, y_star + 7, x_star + 65, y_star + 23, 65535);
//                        drawLine(x_star + 271, y_star + 7, x_star + 271, y_star + 23, 65535);
//                        SetTemp = x - 70;
//                        fillCircle(x_star + 70 + SetTemp, y_star + 15, 5, 65535);
//                        temp = (SetTemp / 14) + 16;
//                        swprintf(opt, 20, L" %d", temp);
//                        drawText(opt, x - 38, y_star, 70, 1, 0, 20.0f, 65535, 1);
//                    }
//                    usleep(90000);
//                    x = readX();
//                    y = readY();
//                }
//                connectedHomeSetValue(indexNo, ((SetTemp) / 14) + 16);
////                setThermoHeatCurrLevel(((SetTemp) / 14) + 16);
//            }
//        }
//            break;
        case AcSetpointHeating:
        case AcSetpointCooling:
        {
            int SetTemp = 0, temp;
            if (x > x_star + 70 && y > y_star && x < x_star + 266 && y < y_star + 30) {
                while (x > x_star + 70 && y > y_star && x < x_star + 266 && y < y_star + 30) {
                    if (x > (70) && x < (266)) {
                        fillRect(x_star + 60, y_star, x_star + 276, y_star + 28, RGB(23, 191, 227));
                        drawLine(x_star + 65, y_star + 15, x_star + 271, y_star + 15, 65535);
                        drawLine(x_star + 65, y_star + 7, x_star + 65, y_star + 23, 65535);
                        drawLine(x_star + 271, y_star + 7, x_star + 271, y_star + 23, 65535);
                        SetTemp = x - 70;
                        fillCircle(x_star + 70 + SetTemp, y_star + 15, 5, 65535);
                        temp = (SetTemp / 14) + 16;
                        swprintf(opt, 20, L" %d", temp);
                        drawText(opt, x - 38, y_star, 70, 1, 0, 20.0f, 65535, 1);
                    }
                    usleep(90000);
                    x = readX();
                    y = readY();
                }
                connectedHomeSetValue(indexNo, ((SetTemp) / 14) + 16);
//                setThermoCoolCurrLevel(((SetTemp) / 14) + 16);
            }
        }
            break;

        case AcFanMode:
        {
            char tempFanMode[20];
            if ((x > x_star + 50 && x < x_star + 80 && y > y_star && y < y_star + 30) || (x > x_star + 187 && x < x_star + 217 && y > y_star && y < y_star + 30)) {
                if (x > x_star + 187 && x < x_star + 217 && y > y_star && y < y_star + 30)
                    AcIrFanMode++;
                else
                    AcIrFanMode--;
                if (AcIrFanMode > 4)
                    AcIrFanMode = 1;
                else if (AcIrFanMode < 1)
                    AcIrFanMode = 4;
                fillRect(x_star + 82, y_star + 5, x_star + 182, y_star + 28, RGB(23, 191, 227));
                if (AcIrFanMode == 1) {
                    drawText(lan.get("auto", "connected_sensor"), x_star + 90, y_star + 7, 100, 1, 0, 20.0f, 65535, 1);
                    strcpy(tempFanMode, "Auto Low");
                } else if (AcIrFanMode == 2) {
                    drawText(lan.get("low", "connected_sensor"), x_star + 90, y_star + 7, 100, 1, 0, 20.0f, 65535, 1);
                    strcpy(tempFanMode, "On Low");
                } else if (AcIrFanMode == 3) {
                    drawText(lan.get("medium", "connected_sensor"), x_star + 90, y_star + 7, 100, 1, 0, 20.0f, 65535, 1);
                    strcpy(tempFanMode, "Unknown 5");
                } else if (AcIrFanMode == 4) {
                    drawText(lan.get("high", "connected_sensor"), x_star + 90, y_star + 7, 100, 1, 0, 20.0f, 65535, 1);
                    strcpy(tempFanMode, "On High");
                }
//                setThermostatFanMode(tempFanMode);
                connectedHomeSetValue(indexNo, tempFanMode);
            }
        }
            break;

        case AcSwing:
        {
            fillRect(x_star + 10, y_star, x_star + 90, y_star + 28, RGB(23, 191, 227));
            fillRect(x_star + 100, y_star, x_star + 180, y_star + 28, BUTTON_LAYER);
            drawText(lan.get("swing_off", "connected_sensor"), x_star + 10, y_star + 7, 80, 1, 0, 20.0f, 65535, 1);
            drawText(lan.get("swing_on", "connected_sensor"), x_star + 100, y_star + 7, 80, 1, 0, 20.0f, 65535, 1);
            if (x > x_star + 10 && x < x_star + 90 && y > y_star && y < y_star + 30) { //Auto Low
                pngImg.drawPNG(x_star + 10, y_star + 5, 1, BACKGROUND_LAYER, Tick);
//                SetConfigParam(33, 1, 0);
                connectedHomeSetValue(indexNo, "0");
            } else if (x > x_star + 100 && x < x_star + 180 && y > y_star && y < y_star + 30) { //On Low
                pngImg.drawPNG(x_star + 100, y_star + 5, 1, BACKGROUND_LAYER, Tick);
//                SetConfigParam(33, 1, 1);
                connectedHomeSetValue(indexNo, "1");
            }
        }
            break;

        case IRcode:
        {
            if (x > x_star + 80 && x < x_star + 140 && y > y_star && y < y_star + 30) {
                IRCodeBox_onTouch();
            } else if ((x > x_star + 150 && x < x_star + 190 && y > y_star && y < y_star + 30) && IRCodeFlag) {
                IRCodeFlag = false;
//                SetConfigParam(27, 2, atoi(tmpc));
                connectedHomeSetValue(indexNo, tmpc);
                ConFlag = false;
                Modal ConfmW;
                ConfmW.setLine1(L"Sending ");
                ConfmW.setLine2(L"IR Code Value");
                ConfmW.modalReturn = &doNothing;
                ConfmW.draw(MODAL_TYPE_INFORMATION, 5);
                LOG(Logger::INFO, "Sending configuration value");
                in = StartIndxCnt;
                if (ConFlag == true) {
                    ConfmW.setLine1(L"");
                    ConfmW.setLine2(L"Success... ");
                    ConfmW.modalReturn = &GenericKeyBoardReturn;
                    ConfmW.draw(MODAL_TYPE_INFORMATION, 2);
                    LOG(Logger::INFO, "Sending configuration value Succes");
                } else if (ConFlag == false) {
                    ConfmW.setLine1(L"");
                    ConfmW.setLine2(L"Failed.... ");
                    ConfmW.modalReturn = &GenericKeyBoardReturn;
                    ConfmW.draw(MODAL_TYPE_INFORMATION, 2);
                    LOG(Logger::INFO, "Sending configuration value failed");
                }
                ConFlag = false;


            }
        }
            break;
        case DimmerLevel:
        {
            //            sprintf(t_temp, "Dimmer level x:%d y:%d",x ,y);
            //            LOG(Logger::INFO, t_temp);
            float dimlev;
            while (x > x_star + 100 && y > y_star && x < x_star + 290 && y < y_star + 30) {
                fillRect(x_star + 106, y_star + 7, x_star + 284, y_star + 24, RGB(23, 191, 227));
                if (x > x_star + 285) {
                    drawLine(x_star + 105, y_star + 15, x_star + 285, y_star + 15, 65535);
                    fillCircle(x_star + 280, y_star + 15, 5, 65535);
                    dimmerCurrLevel = 255;
                } else if (x < x_star + 105) {
                    drawLine(x_star + 105, y_star + 15, x_star + 285, y_star + 15, 65535);
                    fillCircle(x_star + 110, y_star + 15, 5, 65535);
                    dimmerCurrLevel = 0;
                } else {
                    drawLine(x_star + 105, y_star + 15, x_star + 285, y_star + 15, 65535);
                    fillCircle(x, y_star + 15, 5, 65535);
                    dimmerCurrLevel = (x - (x_star + 105));
                    dimlev = (float) dimmerCurrLevel;
                    dimmerCurrLevel = (int) (dimlev * 1.5);
                }
                usleep(90000);
                x = readX();
                y = readY();
//                multilevelGoToLevelOnOff(dimmerCurrLevel);
                connectedHomeSetValue(indexNo, dimmerCurrLevel);
            }
        }
            break;
        case Temperature:
        {
            int indexNo = 0;
            indexNo = d.getIndexNoFromName("TEMPERATURE", -1);
            convertTemperature(indexNo, opt, true);

            
            fillRect(x_star + 5, y_star, x_star + 95, y_star + 29, GREEN_COLOR);
            drawText(opt, x_star + 5, y_star + 5, 90, 1, 0, 20.0f, 65535, 1);
        }
            break;

        case LockStateZb:
        {
            if (x > x_star + 120 && x < x_star + 200 && y > y_star && y < y_star + 30) {
//                doorLock_onTouch();
                connectedHomeSetValue(indexNo, "1");
            } else if (x > x_star + 210 && x < x_star + 290 && y > y_star && y < y_star + 30) {
//                doorUnlock_onTouch();
                connectedHomeSetValue(indexNo, "0");
            }
        }
            break;

        case HueZb:
        {
            int s_val;
            if (x > x_star + 16 && x < x_star + 270) {
                while (x > x_star + 16 && y > y_star && x < x_star + 269 && y < y_star + 30) {
                    pngImg.drawPNG(x_star + 15, y_star, 1, 0, color_png1);
                    fillRect(x_star + 270, y_star, x_star + 290, y_star + 30, RGB(255, 255, 255));
                    drawLine(x - 2, y_star, x - 2, y_star + 30, 0);
                    s_val = (x - (x_star + 17));
                    usleep(90000);
                    x = readX();
                }
                if (s_val <= 255) {
//                    setbulbhue(s_val);
                    connectedHomeSetValue(indexNo, s_val);
                }
            } else {
                waitForPenUp();
                setbulbhueandsat(0, 0);
            }
        }
            break;

        case Saturation:
        {
            int sat;
            float satTemp;
            while (x > x_star + 100 && y > y_star && x < x_star + 290 && y < y_star + 30) {
                fillRect(x_star + 106, y_star + 7, x_star + 284, y_star + 24, RGB(23, 191, 227));
                if (x > x_star + 285) {
                    drawLine(x_star + 105, y_star + 15, x_star + 285, y_star + 15, 65535);
                    fillCircle(x_star + 280, y_star + 15, 5, 65535);
                    sat = 255;
                } else if (x < x_star + 105) {
                    drawLine(x_star + 105, y_star + 15, x_star + 285, y_star + 15, 65535);
                    fillCircle(x_star + 110, y_star + 15, 5, 65535);
                    sat = 0;
                } else {
                    drawLine(x_star + 105, y_star + 15, x_star + 285, y_star + 15, 65535);
                    fillCircle(x, y_star + 15, 5, 65535);
                    sat = (x - (x_star + 110));
                    satTemp = (float) sat;
                    sat = (int) (satTemp * 1.5f);
                }
                usleep(90000);
                x = readX();
                y = readY();
//                setbulbsaturation(sat);
                connectedHomeSetValue(indexNo, sat);
            }
        }
            break;

        case ColorTemperature:
        {
            int s_val;
            if (x > x_star + 15 && x < x_star + 292) {
                while (x > x_star + 15 && y > y_star && x < x_star + 292 && y < y_star + 30) {
                    pngImg.drawPNG(x_star + 15, y_star, 1, 0, color_temp1);
                    drawLine(x - 2, y_star, x - 2, y_star + 30, 0);
                    s_val = (x - (x_star + 17))*32 + 1000; //colortemperature 
                    usleep(90000);
                    x = readX();
                }
//                setbulbcolortemperature(s_val);
                connectedHomeSetValue(indexNo, s_val);
            }

        }
            break;

            //        case SensorMultilevel:
            //        {
            //            char ss[180]; float t_val;
            //            Device *d = getDevice(contDevID);
            //            t_val = atof(d->getValue("SENSOR MULTILEVEL"));
            //            sprintf(t_temp, "Thermostat temperature:%f ", t_val);
            //            LOG(Logger::INFO, t_temp);
            //            fillRect(x_star+5, y_star, x_star + 95, y_star + 29, GREEN_COLOR);
            //            format = (format == FORMAT_F)? FORMAT_C : FORMAT_F; 
            //            
            //            if (format == FORMAT_F) {
            //                mem.addValue("DegreeFormat", "F");
            //                sprintf(ss, "%0.1f°F", t_val);
            //            } else {
            //                mem.addValue("DegreeFormat", "C");
            //                t_val = (((t_val - 32)*5) / 9);
            //                sprintf(ss, "%0.1f°C", t_val);
            //            }
            //            valueToSave = 1;
            //            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            //            drawText(opt, x_star + 5, y_star + 5, 90, 1, 0, 20.0f, 65535, 1);
            //        }
            //        break;
            //        
        case AcMode://newgeneric
        case ThermostatMode:
        {
            if (x > x_star && x < x_star + 300 && y > y_star && y < y_star + 29) {
                if (x > x_star + 45 && x < x_star + 120 && y > y_star && y < y_star + 29) { //auto
                    fillRect(x_star + 45, y_star, x_star + 120, y_star + 29, RGB(23, 191, 227));
                    fillRect(x_star + 130, y_star, x_star + 205, y_star + 29, BUTTON_LAYER);
                    fillRect(x_star + 215, y_star, x_star + 290, y_star + 29, BUTTON_LAYER);
//                    setThermostatMode("Auto");
                    connectedHomeSetValue(indexNo, "Auto");
                    pngImg.drawPNG(x_star + 45, y_star + 5, 1, BACKGROUND_LAYER, Tick);
                } else if (x > x_star + 130 && x < x_star + 205 && y > y_star && y < y_star + 29) { //cool
                    fillRect(x_star + 45, y_star, x_star + 120, y_star + 29, BUTTON_LAYER);
                    fillRect(x_star + 130, y_star, x_star + 205, y_star + 29, RGB(23, 191, 227));
                    fillRect(x_star + 215, y_star, x_star + 290, y_star + 29, BUTTON_LAYER);
//                    setThermostatMode("Cool");
                    connectedHomeSetValue(indexNo, "Cool");
                    pngImg.drawPNG(x_star + 134, y_star + 5, 1, BACKGROUND_LAYER, Tick);
                } else if (x > x_star + 215 && x < x_star + 290 && y > y_star && y < y_star + 29) { //heat
                    fillRect(x_star + 45, y_star, x_star + 120, y_star + 29, BUTTON_LAYER);
                    fillRect(x_star + 130, y_star, x_star + 205, y_star + 29, BUTTON_LAYER);
                    fillRect(x_star + 215, y_star, x_star + 290, y_star + 29, RGB(23, 191, 227));
//                    setThermostatMode("Heat");
                    connectedHomeSetValue(indexNo, "Heat");
                    pngImg.drawPNG(x_star + 222, y_star + 5, 1, BACKGROUND_LAYER, Tick);
                }
                drawText(lan.get("auto", "connected_sensor"), x_star + 45, y_star + 7, 70, 1, 0, 16.0f, 65535, 1);
                drawText(lan.get("cool", "connected_sensor"), x_star + 135, y_star + 7, 70, 1, 0, 16.0f, 65535, 1);
                drawText(lan.get("heat", "connected_sensor"), x_star + 225, y_star + 7, 70, 1, 0, 16.0f, 65535, 1);
                waitForPenUp();
            }
        }
            break;

//        case ThermostatSetpointHeating:
//        {
//            while (x > x_star + 50 && y > y_star && x < x_star + 250 && y < y_star + 30) {
//                if (x > x_star + 55 && x < x_star + 245) {
//                    fillRect(x_star + 61, y_star, x_star + 244, y_star + 29, RGB(23, 191, 227));
//                    drawLine(x_star + 55, y_star + 15, x_star + 245, y_star + 15, 65535);
//                    fillCircle(x, y_star + 15, 5, 65535);
//                    thermoHeatCurrLevel = (x - (x_star + 55));
//                }
//                usleep(90000);
//                x = readX();
//                y = readY();
////                setThermoHeatCurrLevel(thermoHeatCurrLevel);
//                connectedHomeSetValue(indexNo, thermoHeatCurrLevel);
//            }
//        }
//            break;
        case ThermostatSetpointHeating:
        case ThermostatSetpointCooling:
        {
            while (x > x_star + 50 && y > y_star && x < x_star + 250 && y < y_star + 30) {
                if (x > x_star + 55 && x < x_star + 245) {
                    fillRect(x_star + 61, y_star, x_star + 244, y_star + 29, RGB(23, 191, 227));
                    drawLine(x_star + 55, y_star + 15, x_star + 245, y_star + 15, 65535);
                    fillCircle(x, y_star + 15, 5, 65535);
                    thermoCoolCurrLevel = (x - (x_star + 55));
                }
                usleep(90000);
                x = readX();
                y = readY();
//                setThermoCoolCurrLevel(thermoCoolCurrLevel);
                connectedHomeSetValue(indexNo, thermoCoolCurrLevel);
            }
        }
            break;

        case ThermostatFanMode:
        {
            if (x > x_star + 40 && x < x_star + 115 && y > y_star && y < y_star + 30) { //Auto Low
                fillRect(x_star + 40, y_star, x_star + 115, y_star + 29, RGB(23, 191, 227));
                fillRect(x_star + 125, y_star, x_star + 199, y_star + 29, BUTTON_LAYER);
                drawText(lan.get("auto_low", "connected_sensor"), x_star + 48, y_star + 7, 75, 1, 0, 16.0f, 65535, 1);
                drawText(lan.get("on_low", "connected_sensor"), x_star + 133, y_star + 7, 75, 1, 0, 16.0f, 65535, 1);
//                setThermostatFanMode("Auto Low");
                connectedHomeSetValue(indexNo, "Auto Low");
                pngImg.drawPNG(x_star + 45, y_star + 7, 1, BACKGROUND_LAYER, Tick);

            } else if (x > x_star + 125 && x < x_star + 199 && y > y_star && y < y_star + 30) { //On Low
                fillRect(x_star + 40, y_star, x_star + 115, y_star + 29, BUTTON_LAYER);
                fillRect(x_star + 125, y_star, x_star + 199, y_star + 29, RGB(23, 191, 227));
                drawText(lan.get("auto_low", "connected_sensor"), x_star + 48, y_star + 7, 75, 1, 0, 16.0f, 65535, 1);
                drawText(lan.get("on_low", "connected_sensor"), x_star + 133, y_star + 7, 75, 1, 0, 16.0f, 65535, 1);
//                setThermostatFanMode("On Low");
                connectedHomeSetValue(indexNo, "On Low");
                pngImg.drawPNG(x_star + 126, y_star + 7, 1, BACKGROUND_LAYER, Tick);
            }
        }
            break;


        case AlarmState:
        {
//            Device *d = getDevice(contDevID);
//            if (d) {
                if ((x > x_star + 5 && x < x_star + 90 && y > y_star && y < y_star + 29)) { //Alarm
                    fillRect(x_star + 5, y_star, x_star + 90, y_star + 29, RGB(255, 0, 0));
                    drawText(lan.get("alarm", "connected_sensor"), x_star + 5, y_star + 4, 85, 1, 0, 20.0f, 65535, 1);
                    LOG(Logger::INFO, "ALARM");
                    if (d.getDeviceTechnology() == ZWaveTechnology) {
//                        switchOn_onTouch();
                        connectedHomeSetValue(indexNo, "true");
                    } else {
                        Sirenon_onTouch1(65535);
                    }
                    fillRect(x_star + 105, y_star, x_star + 190, y_star + 29, RGB(51, 255, 51));
                    drawText(lan.get("off_1", "common"), x_star + 105, y_star + 4, 85, 1, 0, 20.0f, 65535, 1);
                } else if (x > x_star + 105 && x < x_star + 190 && y > y_star && y < y_star + 29) {//Alarm off
                    fillRect(x_star + 105, y_star, x_star + 190, y_star + 29, RGB(0, 255, 0));
                    drawText(lan.get("off_1", "common"), x_star + 105, y_star + 4, 85, 1, 0, 20.0f, 65535, 1);
                    LOG(Logger::INFO, "ALARM off");
                    if (d.getDeviceTechnology() == ZWaveTechnology) {
//                        switchOn_offTouch();
                        connectedHomeSetValue(indexNo, "false");
                    } else {
                        connectedHomeSetValue(indexNo, "0");
                        Sirenoff_onTouch1();                        
                    }
                    fillRect(x_star + 5, y_star, x_star + 90, y_star + 29, RGB(255, 51, 51));
                    drawText(lan.get("alarm", "connected_sensor"), x_star + 5, y_star + 4, 85, 1, 0, 20.0f, 65535, 1);
                } else if (d.getDeviceTechnology() != ZWaveTechnology) {
                    fillRect(x_star + 5, y_star, x_star + 90, y_star + 29, RGB(255, 51, 51));
                    drawText(lan.get("alarm", "connected_sensor"), x_star + 5, y_star + 4, 85, 1, 0, 20.0f, 65535, 1);
                    fillRect(x_star + 105, y_star, x_star + 190, y_star + 29, RGB(51, 255, 51));
                    drawText(lan.get("off_1", "common"), x_star + 105, y_star + 4, 85, 1, 0, 20.0f, 65535, 1);
                    TimeBox->checkTouch(x, y);
                }
                change = CHANGE_TIME_GENERIC;
//            }
        }
            break;

        case WindowCoveringMode:
        {
            if ((x > x_star + 5 && x < x_star + 90 && y > y_star && y < y_star + 29)) {
                connectedHomeSetValue(indexNo, "1");
            } else if ((x > x_star + 105 && x < x_star + 190 && y > y_star && y < y_star + 29)) {
                connectedHomeSetValue(indexNo, "2");
            } else if ((x > x_star + 205 && x < x_star + 290 && y > y_star && y < y_star + 29)) {
                connectedHomeSetValue(indexNo, "3");
            }

        }
            break;
        default:
            gen.y_prev23 = 0;
            break;
    }
    memset(opt, 0, sizeof opt);
}

void more_onTouch(void) {
    if (pageNum == 0 || (pageNum == curPage && (sensorsCount % (xCount * yCount) == 0))) {
        return;
    }
    if (curPage < pageNum || (curPage == pageNum && sensorsCount > (pageNum * xCount * yCount - pageNum))) {
        curPage++;
        pageNum++;
    } else {
        curPage = 0;
    }
    connectedHome_draw();
}

void draw(void) {

}

void onExit(void) {
    free(readLCDArea.data);
    readLCDArea.data = NULL;
    lcd_clear(BACKGROUND_LAYER);
    //    exit(0);
    abort();
}

//void nextOnTouch(void) {
//
//}

void backOnTouch(void) {
    if (curPage == 0) {
        //        onExit();
        abort();
    } else {
        curPage--;
        lcd_clear(BACKGROUND_LAYER);
        connectedHome_draw();
    }


}

//void textBoxOnTouch(void) {
//    connected_home->keyboard.draw("");
//}

void readArea(int x, int y, int w, int h) {
    int fd;
    fd = open("/dev/almond_lcd", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    readLCDArea.x0 = x;
    readLCDArea.y0 = y;
    readLCDArea.width = w;
    readLCDArea.height = h;
    ioctl(fd, READ_LCD_AREA, &readLCDArea);
    close(fd);
}

void drawArea(void) {
    LOG(Logger::INFO, "DRaw area");
    int fd;
    fd = open("/dev/almond_lcd", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }
    ioctl(fd, 11, &readLCDArea);
    close(fd);
}




void drawControlDevice(Device d) {
    MODE = NOTOUCH_MODE;
    char weatherData[10] = "";
    char ss[180];
    fillRect(10, 5, 90, 40, RGB(0, 0, 0));
    fillRect(270, 5, 319, 40, RGB(0, 0, 0));
    drawRoundedRectangle(10, 55, 310, 230, RR_COLOR, 255);
    memset(opt, 0, sizeof (opt));
    if (d.getDeviceTechnology() == AlmondTechnology) {
        sprintf(ss, "%hs", d.getDeviceName());
        utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
        drawText(opt, 30, 57, 253, 1, 0, 20.0f, 65535, 1);
    } else {
        utf8_to_wchar(d.getDeviceName(), strlen(d.getDeviceName()), opt, strlen(d.getDeviceName()), UTF8_IGNORE_ERROR);
        drawText(opt, 32, 57, 253, 1, 0, 20.0f, 65535, 1);
        memset(opt, 0, sizeof (opt));
        sprintf(ss, "@ %hs", d.getDeviceLocation());
        utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
        drawText(opt, 30, 77, 250, 1, 0, 16.0f, 65535, 1);
    }
    memset(opt, 0, sizeof (opt));
    pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossdefault);
    contDevID = d.getID();
    contDev = d.getDeviceType();
    mem.getSetting("DegreeFormat", weatherData);
    if (weatherData[0] == 'C')
        format = FORMAT_C;
    else
        format = FORMAT_F;
    sprintf(t_temp, " Initial weather unit : %c", weatherData[0]);
    LOG(Logger::INFO, t_temp);

    switch (d.getDeviceType()) {
        case UnknownOnOffModule:
        case BinarySwitch:
        {
            char tmp_str[30];
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[2]);
            strcpy(tmp_str, d.getValue(1));
            swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y, 300, 1, 0, 35.0f, 65535, 1);

            SwitchOnButton.draw();
            SwitchOffButton.draw();
            sprintf(t_temp, "Binary switch %d initial is %s ", d.getID(), (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? "Off" : "On");
            LOG(Logger::INFO, t_temp);
        }
            break;
        case BinaryPowerSwitch:
        {
            char tmp_str[30];
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[2]);
            strcpy(tmp_str, d.getValue(1));
            swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
            drawText(opt, NOTIFICATION_X + 28, NOTIFICATION_Y, 110, 1, 0, 35.0f, 65535, 1);
            strcpy(tmp_str, d.getValue(2));
            swprintf(opt, 179, L"(%ls:%hs W)", lan.get("power", "connected_sensor"), tmp_str);
            drawText(opt, NOTIFICATION_X + 105, NOTIFICATION_Y + 5, 150, 1, 0, 25.0f, 65535, 1);
            SwitchOnButton.draw();
            SwitchOffButton.draw();
            strcpy(tmp_str, d.getValue(1));
            sprintf(t_temp, "BinaryPowerSwitch %d initial is %s ,power:%s", d.getID(), (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? "Off" : "On", d.getValue(2));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case MultilevelSwitch:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[3]);
            dimmerCurrLevel = atoi(d.getValue(1))*2;
            if (dimmerCurrLevel == 0) {
                swprintf(opt, 179, L"%ls(%d%%)", lan.get("off_1", "common"), (dimmerCurrLevel / 2));
            } else {
                swprintf(opt, 179, L"%ls(%d%%)", lan.get("on_1", "common"), (dimmerCurrLevel / 2));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 14, 300, 1, 0, 18.0f, 65535, 1);
            fillRect(RECT_X, RECT_Y, RECT_X + RECT_WIDTH, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
            drawLine(RECT_X + 5, RECT_Y + (RECT_HEIGHT / 2), RECT_X + RECT_WIDTH - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
            drawLine(RECT_X + 5, RECT_Y + (RECT_HEIGHT / 3), RECT_X + 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
            drawLine(RECT_X + RECT_WIDTH - 5, RECT_Y + (RECT_HEIGHT / 3), RECT_X + RECT_WIDTH - 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
            fillCircle(RECT_X + 10 + dimmerCurrLevel, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
            multilevelOnButton.draw();
            multilevelOffButton.draw();
            sprintf(t_temp, "MultilevelSwitch %d initial level is %d ", d.getID(), (dimmerCurrLevel / 2));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case MultilevelSwitchOnOff:
        {
            float val;
            dimmerCurrLevel = atoi(d.getValue(1));
            char tmp_str[30];
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[3]);
            strcpy(tmp_str, d.getValue(2));
            val = ((float) (dimmerCurrLevel * 100) / (float) 255);
            swprintf(opt, 179, L"%ls (%d%%)", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"), roundfun(val));
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 15, 300, 1, 0, 18.0f, 65535, 1);
            fillRect(25, RECT_Y, 301, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 3), 25 + 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
            drawLine(301 - 5, RECT_Y + (RECT_HEIGHT / 3), 301 - 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
            fillCircle(25 + 10 + dimmerCurrLevel, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
            multilevelOnButton.draw();
            multilevelOffButton.draw();
            sprintf(t_temp, "MultilevelSwitchOnOff %d initial level is %d ", d.getID(), dimmerCurrLevel);
            LOG(Logger::INFO, t_temp);
        }
            break;
        case ColorDimmableLight:
        {
            dimmerCurrLevel = atoi(d.getValue(1));
            char tmp_str[30];
            int color_t;
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[3]);
            strcpy(tmp_str, d.getValue(2));
            percentage = (int) round(((dimmerCurrLevel * 100) / 255.0));
            swprintf(opt, 179, L"%ls (%d%%)", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"), percentage);
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 15, 300, 1, 0, 18.0f, 65535, 1);
            fillRect(25, RECT_Y, 301, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 3), 25 + 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
            drawLine(301 - 5, RECT_Y + (RECT_HEIGHT / 3), 301 - 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
            fillCircle(25 + 10 + dimmerCurrLevel, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
            color_t = atoi(d.getValue(5));
            color_t = (color_t - 1000) / 32 + 25;
            pngImg.drawPNG(25, RECT_Y + 40, 1, 0, color_temp);
            //            fillRect(353, 180, 385, 230, RGB(255, 255, 255));
            drawLine(color_t, RECT_Y + 40, color_t, RECT_Y + 40 + RECT_HEIGHT, 0);
            colorOnButton.draw();
            colorOffButton.draw();
            settingsButton.draw();
            sprintf(t_temp, "ColorDimmableLight %d initial level is %d ", d.getID(), dimmerCurrLevel);
            LOG(Logger::INFO, t_temp);
        }
            break;
        case BinarySensor:
        {
            int batteryVal;
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[4]);
            batteryVal = atoi(d.getValue(2));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (batteryVal != 0xff && batteryVal != 0) {
                swprintf(opt, 5, L"%hd%%", batteryVal);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
            }
            fillRect(80, 130, 300, 200, RR_COLOR);
            if (strcmp(d.getValue(1), "true") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("true", "common"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("false", "common"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 20, 300, 1, 0, 35.0f, 65535, 1);
            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
            sprintf(t_temp, "BinarySensor %d initial state is %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case DoorSensor:
        {
            int batteryVal;
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[10]);
            batteryVal = atoi(d.getValue(2));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (batteryVal != 0xff && batteryVal != 0) {
                swprintf(opt, 5, L"%hd%%", batteryVal);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
            }
            fillRect(80, 130, 400, 200, RR_COLOR);
            if (strcmp(d.getValue(1), "true") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("open", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("closed", "connected_sensor"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 40.0f, 65535, 1);
            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
            if (strcmp(d.getValue(4), "#ERR")) {

                memset(opt, 0, sizeof (opt));
                fillRect(215, 93, 300, 122, GREEN_COLOR);
                convertTemperature(4, opt);
                drawText(opt, 215, 95, 85, 1, 0, 25.0f, 65535, 1);
            }
            sprintf(t_temp, "DoorSensor %d initial state is %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case MovementSensor:
        {
            int batteryVal;
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[9]);
            batteryVal = atoi(d.getValue(2));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (batteryVal != 0xff && batteryVal != 0) {
                swprintf(opt, 5, L"%hd%%", batteryVal);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
            }
            fillRect(10, 120, 300, 200, RR_COLOR);
            if (strcmp(d.getValue(1), "true") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("motiondetected", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("nomotion", "connected_sensor"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
            if (strcmp(d.getValue(4), "#ERR")) {
//                float tempv = 0.0;
                //fillRect(225, 85, 300, 110, RR_COLOR);
                memset(opt, 0, sizeof (opt));

                fillRect(215, 93, 300, 122, GREEN_COLOR);
                convertTemperature(4, opt);
                drawText(opt, 215, 95, 85, 1, 0, 25.0f, 65535, 1);
            }
            sprintf(t_temp, "MovementSensor %d initial state is %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case ShockSensor:
        {
            int batteryVal;
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[4]);
            batteryVal = atoi(d.getValue(2));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (batteryVal != 0xff && batteryVal != 0) {
                swprintf(opt, 5, L"%hd%%", batteryVal);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
            }
            fillRect(10, 120, 300, 200, RR_COLOR);
            if (strcmp(d.getValue(1), "true") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("vibrationdetected", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("novibration", "connected_sensor"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
            sprintf(t_temp, "ShockSensor %d initial state is %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case FloodSensor:
        {
            int batteryVal;
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[12]);
            batteryVal = atoi(d.getValue(2));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (batteryVal != 0xff && batteryVal != 0) {
                swprintf(opt, 5, L"%hd%%", batteryVal);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
            }
            fillRect(10, 120, 300, 200, RR_COLOR);
            if (strcmp(d.getValue(1), "0") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("flooded", "connected_sensor"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
            sprintf(t_temp, "FloodSensor %d initial state is %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case SmokeDetector:
        {
            int batteryVal;
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[4]);
            batteryVal = atoi(d.getValue(2));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (batteryVal != 0xff && batteryVal != 0) {
                swprintf(opt, 5, L"%hd%%", batteryVal);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
            }
            fillRect(10, 120, 300, 200, RR_COLOR);
            if (strcmp(d.getValue(1), "0") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
            } else if (strcmp(d.getValue(1), "254") == 0) {
                swprintf(opt, 179, L" %ls", L"Testing");
            } else {
                swprintf(opt, 179, L" %ls", lan.get("smokedetected", "connected_sensor"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
            sprintf(t_temp, "SmokeDetector %d initial state is %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);

        }
            break;
        case Alarm:
        {
            int batteryVal;
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[6]);
            batteryVal = atoi(d.getValue(2));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (batteryVal != 0xff && batteryVal != 0) {
                swprintf(opt, 5, L"%hd%%", batteryVal);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
            }
            fillRect(10, 120, 300, 200, RR_COLOR);
            if (strcmp(d.getValue(1), "0") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("false", "common"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("true", "common"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
            sprintf(t_temp, "Alarm %d initial state is %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case MoistureSensor:
        {
            int batteryVal;
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[4]);
            batteryVal = atoi(d.getValue(2));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (batteryVal != 0xff && batteryVal != 0) {
                swprintf(opt, 5, L"%hd%%", batteryVal);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
            }
            char ss[500];
	    
	    	wchar_t arr[20];
	    	fillRect(190, 95, 300, 125, GREEN_COLOR);
            memset(arr, 0, sizeof (arr));
            convertTemperature(1, arr);
            drawText(arr, 200, 100, 100, 1, 0, 25.0f, 65535, 1);

            if (strcmp(d.getValue(2), "0") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("notflooded", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("flooded", "connected_sensor"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
            sprintf(t_temp, "MoistureSensor %d initial temp %s and state %s", d.getID(), d.getValue(1), d.getValue(2));
            LOG(Logger::INFO, t_temp);

        }
            break;
        case TemperatureSensor:
        {
            int batteryVal, battIndex = 0;
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[15]);
            if (battIndex = d.getIndexNoFromName("BATTERY", 0)) {
                batteryVal = atoi(d.getValue(battIndex));
                pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                if (batteryVal != 0xff && batteryVal != 0) {
                    swprintf(opt, 5, L"%hd%%", batteryVal);
                    drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                } else {
                    drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
                }
            }
            char ss[500];
            memset(opt, 0, sizeof (opt));
            sprintf(ss, " %ls %hs", lan.get("temp", "connected_sensor"), d.getValue(1));
            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            drawText(opt, 225, 87, 75, 1, 0, 16.0f, 65535, 1);
            memset(opt, 0, sizeof (opt));
            swprintf(opt, 179, L" %ls %hs", lan.get("humidity", "connected_sensor"), d.getValue(2));
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
            sprintf(t_temp, "TemperatureSensor %d initial temp %s and state %s", d.getID(), d.getValue(1), d.getValue(2));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case LightSensor:
        {
            for (int i = 0; i <= d.getValueCount(); i++) {
                switch (d.getIndexType(i)) {
                    case Temperature:
                    {
                        fillRect(190, 95, 300, 125, GREEN_COLOR);
                        memset(opt, 0, sizeof (opt));
                        convertTemperature(i, opt);
                        drawText(opt, 200, 100, 100, 1, 0, 20.0f, 65535, 1);
                    }
                        break;
                    case Humidity:
                    {
                        pngImg.drawPNG(40, 160, 1, BACKGROUND_LAYER, humidityImage);
                        drawText(L"Humidity   :", 75, 160, 90, 1, 0, 20.0f, 65535, 0);
                        swprintf(opt, 20, L"%4s%%     ", d.getValue(i));
                        drawText(opt, 175, 160, 60, 1, 0, 20.0f, 65535, 1);
                    }
                        break;
                    case Illuminance:
                    {
                        pngImg.drawPNG(40, 135, 1, BACKGROUND_LAYER, luminanceImage);
                        drawText(L"Luminance:", 75, 135, 90, 1, 0, 20.0f, 65535, 0);
                        swprintf(opt, 20, L"%s Lux    ", d.getValue(i));
                        drawText(opt, 175, 135, 70, 1, 0, 20.0f, 65535, 0);
                    }
                        break;
                    case Tamper:
                    {
                        if (strcmp(d.getValue(i), "true") == 0) {
                            TamperButton.draw();
                        }
                    }
                        break;
                    case Battery:
                    {
                        int batteryVal = atoi(d.getValue(i));
                        pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                        if (batteryVal != 0xff && batteryVal != 0) {
                            swprintf(opt, 5, L"%hd%%", batteryVal);
                            drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                        } else {
                            drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                        }
                    }
                        break;
                }
            }
        }
            break;
        case ZigbeeMultiSensor:
        {
            for (int i = 0; i <= d.getValueCount(); i++) {
                switch (d.getIndexType(i)) {
                    case Temperature:
                    {

                        fillRect(190, 95, 300, 125, GREEN_COLOR);
                        memset(opt, 0, sizeof (opt));
                        convertTemperature(i, opt);
                        drawText(opt, 200, 100, 100, 1, 0, 20.0f, 65535, 1);
                    }
                        break;
                    case Tamper:
                    {
                        if (strcmp(d.getValue(i), "true") == 0) {
                            TamperButton.draw();
                        }
                    }
                        break;
                    case MotionState://motion
                    {
                        char tempv[10];
                        strcpy(tempv, d.getValue(i));
                        swprintf(opt, 179, L"%ls", (tempv[0] == 'f' || tempv[0] == 'F') ? lan.get("nomotion", "connected_sensor") : lan.get("motiondetected", "connected_sensor"));
                        fillRect(70, 129, 260, 143, RR_COLOR);
                        drawText(opt, 70, 128, 190, 1, 0, 20.0f, 65535, 1);
                    }
                        break;
                    case LowBattery://LowBattery
                    {
                        if (strcmp(d.getValue(i), "true") == 0) {
                            fillRect(70, 148, 260, 162, RR_COLOR);
                            drawText(L"Low", 70, 148, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                        }
                    }
                        break;
                    case FireState://fire
                    {
                        fillRect(70, 168, 260, 182, RR_COLOR);
                        if (strcmp(d.getValue(i), "true") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("alarmfire", "connected_sensor"));
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
                        }
                        drawText(opt, 70, 168, 190, 1, 0, 20.0f, 65535, 1);
                    }
                        break;
                }
            }
        }
            break;
        case MultiSensor://newal2
        {
            int batteryVal;
            char ss[500];
            wchar_t arr[20];
            batteryVal = atoi(d.getValue(2));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (batteryVal != 0xff && batteryVal != 0) {
                swprintf(opt, 5, L"%hd%%", batteryVal);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
            }
            //fillRect(200, 90, 300, 125, GREEN_COLOR);
            fillRect(190, 95, 300, 125, GREEN_COLOR);
            memset(arr, 0, sizeof (arr));
            convertTemperature(4, arr);
            drawText(arr, 200, 100, 100, 1, 0, 25.0f, 65535, 1);

            if (strcmp(d.getValue(1), "true") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("motiondetected", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("nomotion", "connected_sensor"));
            }
            drawText(opt, 40, 130, 200, 1, 0, 25.0f, 65535, 1);
            if (strcmp(d.getValue(3), "#ERR")) {
                memset(opt, 0, sizeof (opt));
                if (strcmp(d.getValueName(3), "ILLUMINANCE") == 0) {
                    aswprintf(opt, 179, L"%ls : %s lux", lan.get("luminance", "connected_sensor"), d.getValue(3));
                }
                //                    else
                //                        aswprintf(opt, 179, L"%ls : %s %%", lan.get("luminance", "connected_sensor"), d.getValue(3));
                //aswprintf(opt, 179, L"%ls : %s %%", lan.get("luminance", "connected_sensor"), d.getValue(3));
                drawText(opt, 40, 155, 200, 1, 0, 25.0f, 65535, 1);
                sprintf(t_temp, "MultiSensor %d luminance is %s", d.getID(), d.getValue(3));
                LOG(Logger::INFO, t_temp);
            }
            if (strcmp(d.getValue(5), "#ERR") && strcmp(d.getValue(5), "NAN")) {
                memset(opt, 0, sizeof (opt));
                sprintf(ss, "%ls: %s %%", lan.get("humidity", "connected_sensor"), d.getValue(5));
                utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
                drawText(opt, 20, 180, 200, 1, 0, 25.0f, 65535, 1);
                sprintf(t_temp, "MultiSensor %d humidity is %s", d.getID(), d.getValue(5));
                LOG(Logger::INFO, t_temp);
            }
        }
            break;
        case Siren://newal2
        {
            char tmp_str[30];
            int batteryVal;
            batteryVal = atoi(d.getValue(2));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (batteryVal != 0xff && batteryVal != 0) {
                swprintf(opt, 5, L"%hd%%", batteryVal);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
            }
            strcpy(tmp_str, d.getValue(1));
            swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
            drawText(opt, 60, 100, 200, 1, 0, 45.0f, 65535, 1);

            drawRoundedRectangle(40, 140, 140, 185, RGB(250, 0, 0), 255);
            swprintf(opt, 179, L"%ls", lan.get("alarm", "connected_sensor"));
            drawText(opt, 40, 147, 100, 1, 0, 30.0f, 65535, 1);
            drawRoundedRectangle(180, 140, 280, 185, GREEN_COLOR, 255);
            swprintf(opt, 179, L"%ls", lan.get("off_1", "common"));
            drawText(opt, 180, 147, 100, 1, 0, 30.0f, 65535, 1);
            //drawRoundedRectangle(10, 55, 310, 230, RR_COLOR, 255);

        }
            break;

        case ZWtoACIRExtender://newal2 start
        {
            wchar_t arr[20];
            int batteryVal;
            double er;
            batteryVal = atoi(d.getValue(6));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (batteryVal != 0xff && batteryVal != 0) {
                swprintf(opt, 5, L"%hd%%", batteryVal);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
            }
            fillRect(210, 95, 300, 125, GREEN_COLOR);
            memset(arr, 0, sizeof (arr));
            convertTemperature(1, arr);
            drawText(arr, 200, 100, 100, 1, 0, 20.0f, 65535, 1);

            swprintf(opt, 179, L"%ls:", lan.get("temp", "connected_sensor"));
            drawText(opt, 5, 165, 60, 1, 0, 20.0f, 65535, 1);
            fillRect(70, 160, 296, 184, RGB(23, 191, 227));
            drawLine(74, 171, 292, 171, 65535);
            drawLine(74, 167, 74, 175, 65535);
            drawLine(292, 167, 292, 175, 65535);

            strcpy(thermoMode, d.getValue(2)); //Thermostat mode
            drawText(lan.get("mode", "connected_sensor"), 10, 130, 45, 1, 0, 20.0f, 65535, 1);
            pngImg.drawPNG(70, 130, 1, 0, arrow_minus); //new zwtoac
            pngImg.drawPNG(190, 130, 1, 0, arrow_plus);
            fillRect(97, 130, 187, 155, BUTTON_LAYER);
            strcpy(thermoMode, d.getValue(2)); //  modes
            if (!strcmp("Auto", thermoMode)) {
                AcIrFlag = 1;
                CheckIrMode = 1;
            } else if (!strcmp("Cool", thermoMode)) {
                AcIrFlag = 2;
                CheckIrMode = 2;
            } else if (!strcmp("Heat", thermoMode)) {
                AcIrFlag = 3;
                CheckIrMode = 3;
            } else if (!strcmp("Auto Changeover", thermoMode)) {
                AcIrFlag = 4;
                CheckIrMode = 4;
            } else if (!strcmp("Off", thermoMode)) {
                AcIrFlag = 5;
                CheckIrMode = 5;
            } else if (!strcmp("Resume", thermoMode)) {
                AcIrFlag = 6;
                CheckIrMode = 6;
            } else if (!strcmp("Fan Only", thermoMode)) {
                AcIrFlag = 7;
                CheckIrMode = 7;
            } else if (!strcmp("Dry Air", thermoMode)) {
                AcIrFlag = 8;
                CheckIrMode = 8;
            }
            aswprintf(opt, 179, L"%s", thermoMode);
            if (!strcmp("Auto Changeover", thermoMode)) {
                aswprintf(opt, 179, L"%s", "Changeover");
                drawText(opt, 103, 135, 80, 1, 0, 15.0f, 65535, 1);
            } else {
                drawText(opt, 105, 135, 80, 1, 0, 20.0f, 65535, 1);
            }
            pngImg.drawPNG(103, 132, 1, BACKGROUND_LAYER, Tick);

            er = getTempinFloat(d.getValue(4));
            er = (er - 32)*5.0 / 9;
            thermoCool = (int) ceil(er);
            thermoCoolCurrLevel = (thermoCool - 16)*14;
            er = getTempinFloat(d.getValue(3));
            er = (er - 32)*5.0 / 9;
            thermoHeat = (int) ceil(er);
            thermoHeatCurrLevel = (thermoHeat - 16)*14;

            if (strstr(thermoMode, "Cool")) {
                fillCircle(78 + thermoCoolCurrLevel, 171, 7, 65535);
            } else if (strstr(thermoMode, "Heat")) {
                fillCircle(78 + thermoHeatCurrLevel, 171, 7, 65535);
            } else {
                drawTransparentRectangle(70, 160, 296, 184, WIDGET_CONTAINER_LAYER, 200);
            }


            fillRect(220, 130, 300, 155, RGB(23, 191, 227)); //swing 
            if (atoi(d.getValue(8)) == 0) {
                //drawText(lan.get("swing_off", "connected_sensor"), 220, 135, 80, 1, 0, 20.0f, 65535, 1);
                drawText(L"Swing Off", 220, 135, 80, 1, 0, 20.0f, 65535, 1);
            } else if (atoi(d.getValue(8)) == 1) {
                // drawText(lan.get("swing_on", "connected_sensor"),  220, 135, 80, 1, 0, 20.0f, 65535, 1);
                drawText(L"Swing On", 220, 135, 80, 1, 0, 20.0f, 65535, 1);
            }
            drawText(lan.get("power", "connected_sensor"), 10, 97, 60, 1, 0, 20.0f, 65535, 1); //ON OFF
            fillRect(70, 95, 115, 125, RGB(23, 191, 227));
            if (atoi(d.getValue(9)) == 0) {
                drawText(lan.get("off_1", "common"), 70, 100, 50, 1, 0, 20.0f, 65535, 1);
            } else {
                drawText(lan.get("on_1", "common"), 70, 100, 50, 1, 0, 20.0f, 65535, 1);
            }

            drawText(lan.get("fan", "connected_sensor"), 10, 190, 40, 1, 0, 20.0f, 65535, 1); //fan speed 
            strcpy(thermoFan, d.getValue(5)); // fan modes
            if (!strcmp("Auto Low", thermoFan)) {
                strcpy(thermoFan, "Auto");
                Acflag = 1;
                CheckAcFan = 1;
            } else if (!strcmp("On Low", thermoFan)) {
                strcpy(thermoFan, "Low");
                Acflag = 2;
                CheckAcFan = 2;
            } else if (!strcmp("Unknown 5", thermoFan)) {
                strcpy(thermoFan, "Medium");
                Acflag = 3;
                CheckAcFan = 3;
            } else if (!strcmp("On High", thermoFan)) {
                strcpy(thermoFan, "High");
                Acflag = 4;
                CheckAcFan = 4;
            }
            aswprintf(opt, 179, L"%s", thermoFan);
            pngImg.drawPNG(70, 190, 1, 0, arrow_minus);
            fillRect(97, 190, 187, 215, RGB(23, 191, 227));
            drawText(opt, 107, 195, 70, 1, 0, 20.0f, 65535, 1);
            pngImg.drawPNG(190, 190, 1, 0, arrow_plus);
            pngImg.drawPNG(100, 195, 1, BACKGROUND_LAYER, Tick);

            ConfigurationButton.draw();
        }
            break;

        case ContactSwitch:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[10]);
            if (strcmp(d.getValue(1), "true") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("open", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("closed", "connected_sensor"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
            if (strcmp(d.getValue(2), "true") == 0) {
                pngImg.drawPNG(90, NOTIFICATION_Y + 50, 1, BACKGROUND_LAYER, battery_c);
                drawText(lan.get("battery_low", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 50, 300, 1, 0, 20.0f, 65535, 1);
            }

            if (strcmp(d.getValue(4), "NaN") && strcmp(d.getValue(4), "#ERR")) {

                memset(opt, 0, sizeof (opt));
                fillRect(215, 93, 300, 122, GREEN_COLOR);
                convertTemperature(4, opt);
                drawText(opt, 215, 95, 85, 1, 0, 25.0f, 65535, 1);
                sprintf(t_temp, "Contact switch initial value of temperature: %s", ss);
                LOG(Logger::INFO, t_temp);
            }
            sprintf(t_temp, " ContactSwitch %d initial state %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case VibrationOrMovementSensor:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[4]);
            if (strcmp(d.getValue(1), "true") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("vibrationdetected", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("novibration", "connected_sensor"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
            if (strcmp(d.getValue(2), "true") == 0) {
                pngImg.drawPNG(90, NOTIFICATION_Y + 50, 1, BACKGROUND_LAYER, battery_c);
                drawText(lan.get("battery_low", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 50, 300, 1, 0, 20.0f, 65535, 1);
            }

            sprintf(t_temp, " VibrationOrMovementSensor %d initial state %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;

        case MotionSensor:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[9]);
            if (strcmp(d.getValue(1), "true") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("motiondetected", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("nomotion", "connected_sensor"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
            if (strcmp(d.getValue(2), "true") == 0) {
                pngImg.drawPNG(90, NOTIFICATION_Y + 50, 1, BACKGROUND_LAYER, battery_c);
                drawText(lan.get("battery_low", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 50, 300, 1, 0, 20.0f, 65535, 1);
            }

            if (strcmp(d.getValue(4), "NaN") && strcmp(d.getValue(4), "#ERR")) {

                memset(opt, 0, sizeof (opt));
                fillRect(215, 93, 300, 122, GREEN_COLOR);
                convertTemperature(4, opt);
                drawText(opt, 215, 95, 85, 1, 0, 25.0f, 65535, 1);
                sprintf(t_temp, "MotionSensor initial value of temperature: %s", ss);
                LOG(Logger::INFO, t_temp);
            }
            sprintf(t_temp, " MotionSensor %d initial state %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;

        case FireSensor:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[11]);
            if (strcmp(d.getValue(1), "true") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("alarmfire", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
            if (strcmp(d.getValue(2), "true") == 0) {
                pngImg.drawPNG(90, NOTIFICATION_Y + 50, 1, BACKGROUND_LAYER, battery_c);
                drawText(lan.get("battery_low", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 50, 300, 1, 0, 20.0f, 65535, 1);
            }

            sprintf(t_temp, " FireSensor %d initial state %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case WaterSensor:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[12]);
            if (strcmp(d.getValue(1), "true") == 0) {
                swprintf(opt, 179, L" %ls", lan.get("flooded", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
            if (strcmp(d.getValue(2), "true") == 0) {
                pngImg.drawPNG(90, NOTIFICATION_Y + 50, 1, BACKGROUND_LAYER, battery_c);
                drawText(lan.get("battery_low", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 50, 300, 1, 0, 20.0f, 65535, 1);
            }

            if (strcmp(d.getValue(4), "NaN") && strcmp(d.getValue(4), "#ERR")) {
                memset(opt, 0, sizeof (opt));
                fillRect(215, 93, 300, 122, GREEN_COLOR);
                convertTemperature(4, opt);

                drawText(opt, 215, 95, 85, 1, 0, 25.0f, 65535, 1);
                sprintf(t_temp, "WaterSensor initial value of temperature: %s", ss);
                LOG(Logger::INFO, t_temp);
            }
            sprintf(t_temp, " WaterSensor %d initial state %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case KeyFob:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[13]);
            if (atoi(d.getValue(1)) == 0) {
                drawText(lan.get("disarmall", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                LOG(Logger::INFO, "Initial state diarm\n");
            } else if (atoi(d.getValue(1)) == 2) {
                drawText(lan.get("armperimeter", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                LOG(Logger::INFO, "Initial state armnightzone\n");
            } else if (atoi(d.getValue(1)) == 3) {
                drawText(lan.get("armall", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                LOG(Logger::INFO, "Initial state armall");
            }
            if (strcmp(d.getValue(2), "true") == 0) {
                PanicButton.draw();
            }
            if (strcmp(d.getValue(3), "true") == 0) {
                EmerButton.draw();
            }

        }
            break;
        case StandardWarningDevice:
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[4]);
            pngImg.drawPNG(40, NOTIFICATION_Y - 5, 1, BACKGROUND_LAYER, alarm_default);
            drawText(lan.get("alarm", "connected_sensor"), NOTIFICATION_X + 17, NOTIFICATION_Y + 27, 110, 1, 0, 25.0f, 65535, 1);
            pngImg.drawPNG(195, NOTIFICATION_Y - 5, 1, BACKGROUND_LAYER, alarm_off_default);
            drawText(lan.get("off_1", "common"), NOTIFICATION_X + 175, NOTIFICATION_Y + 27, 100, 1, 0, 25.0f, 65535, 1);
            drawText(L"Alarm", NOTIFICATION_X + 100, NOTIFICATION_Y - 5, 100, 1, 0, 20.0f, 65535, 1);
            drawText(L"for", NOTIFICATION_X + 120, NOTIFICATION_Y + 15, 60, 1, 0, 20.0f, 65535, 1);
            WDTime->draw();
            WDTime->setText(N_opt);
            drawText(L"sec", NOTIFICATION_X + 120, NOTIFICATION_Y + 60, 60, 1, 0, 20.0f, 65535, 1);
            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
            if (strcmp(d.getValue(2), "true") == 0) {
                pngImg.drawPNG(100, 195, 1, BACKGROUND_LAYER, battery_c);
                drawText(lan.get("battery_low", "connected_sensor"), 120, 198, 100, 1, 0, 20.0f, 65535, 1);
            }
            break;
        case GasSensor:
        case PersonalEmergencyDevice:
        case StandardCIE:
        case Keypad:

        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[4]);
            if (d.getDeviceType() == GasSensor) {
                if (strcmp(d.getValue(1), "true") == 0) {
                    swprintf(opt, 179, L" %ls", lan.get("gasdetected", "connected_sensor"));
                } else {
                    swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
                }
            } else {
                if (strcmp(d.getValue(1), "true") == 0) {
                    swprintf(opt, 179, L" %ls", lan.get("active", "connected_sensor"));
                } else {
                    swprintf(opt, 179, L" %ls", lan.get("inactive", "connected_sensor"));
                }
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
            if (strcmp(d.getValue(2), "true") == 0) {
                pngImg.drawPNG(90, NOTIFICATION_Y + 50, 1, BACKGROUND_LAYER, battery_c);
                drawText(lan.get("battery_low", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 50, 300, 1, 0, 20.0f, 65535, 1);
            }

            sprintf(t_temp, " Sensor %d initial state %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case RemoteControl:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[4]);
            if (d.getDeviceType() == GasSensor) {
                if (strcmp(d.getValue(1), "true") == 0) {
                    swprintf(opt, 179, L" %ls", lan.get("gasdetected", "connected_sensor"));
                } else {
                    swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
                }
            } else {
                if (strcmp(d.getValue(1), "true") == 0) {
                    swprintf(opt, 179, L" %ls", lan.get("active", "connected_sensor"));
                    wcscpy(btnDeActive.W.text, L"Deactivate");
                    btnDeActive.draw();
                } else {
                    swprintf(opt, 179, L" %ls", lan.get("inactive", "connected_sensor"));
                    wcscpy(btnDeActive.W.text, L"Activate");
                    fillRect(25, 190, 105, 220, RR_COLOR);
                }
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
            if (strcmp(d.getValue(2), "true") == 0) {
                pngImg.drawPNG(90, NOTIFICATION_Y + 50, 1, BACKGROUND_LAYER, battery_c);
                drawText(lan.get("battery_low", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 50, 300, 1, 0, 20.0f, 65535, 1);
            }

            sprintf(t_temp, " Sensor %d initial state %s ", d.getID(), d.getValue(1));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case ZenThermostat:
        case Thermostat:
        {
            wchar_t arr[80];
            double er;

            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[7]);
            thermoCurrent = atof(d.getValue(1));
            er = atof(d.getValue(5));
            thermoCool = (int) er;
            er = atof(d.getValue(4));
            thermoHeat = (int) er;
            strcpy(thermoMode, d.getValue(2));
            strcpy(thermoState, d.getValue(3));
            strcpy(thermoFan, d.getValue(6));
            strcpy(thermoFanState, d.getValue(7));
            sprintf(t_temp, " Thermostat %d initial thermoCurrent %0.1f thermoCool %d thermoHeat %d \n\
                          thermoMode %s  thermoFan %s  ", d.getID(), thermoCurrent, thermoCool, thermoHeat, thermoMode, thermoFan);
            LOG(Logger::INFO, t_temp);
            thermoCoolCurrLevel = (thermoCool - 35) * 3;
            thermoHeatCurrLevel = (thermoHeat - 35) * 3;
            drawText(lan.get("mode", "connected_sensor"), 15, 97 + 6, 35, 1, 0, 16.0f, 65535, 0);
            fillRect(60, 97, 110, 125, !(strstr(thermoMode, "Auto")) ? BUTTON_LAYER : RGB(23, 191, 227));
            fillRect(115, 97, 165, 125, !(strstr(thermoMode, "Cool")) ? BUTTON_LAYER : RGB(23, 191, 227));
            fillRect(170, 97, 220, 125, !(strstr(thermoMode, "Heat")) ? BUTTON_LAYER : RGB(23, 191, 227));
            drawText(lan.get("auto", "connected_sensor"), 65, 103, 50, 1, 0, 16.0f, 65535, 1);
            drawText(lan.get("cool", "connected_sensor"), 120, 103, 50, 1, 0, 16.0f, 65535, 1);
            drawText(lan.get("heat", "connected_sensor"), 175, 103, 50, 1, 0, 16.0f, 65535, 1);
            if (strstr(thermoMode, "Auto")) {
                pngImg.drawPNG(60, 105, 1, BACKGROUND_LAYER, Tick);
            } else if (strstr(thermoMode, "Cool")) {
                pngImg.drawPNG(115, 105, 1, BACKGROUND_LAYER, Tick);
            } else if (strstr(thermoMode, "Heat")) {
                pngImg.drawPNG(170, 105, 1, BACKGROUND_LAYER, Tick);
            }

            memset(arr, 0, sizeof (arr));
            fillRect(225, 97, 300, 125, RGB(50, 191, 20));
            convertTemperature(1, arr);
            drawText(arr, 225, 97 + 6, 75, 1, 0, 18.0f, 65535, 1);

            drawText(lan.get("cool", "connected_sensor"), 15, 135, 35, 1, 0, 16.0f, 65535, 0);
            fillRect(THERMO_COOL_X, THERMO_COOL_Y, THERMO_COOL_X + THERMO_COOL_WIDTH, THERMO_COOL_Y + THERMO_COOL_HEIGHT, RGB(23, 191, 227));
            drawLine(THERMO_COOL_X + 5, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), THERMO_COOL_X + THERMO_COOL_WIDTH - 5, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 65535);
            drawLine(THERMO_COOL_X + 5, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X + 5, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
            drawLine(THERMO_COOL_X + THERMO_COOL_WIDTH - 5, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X + THERMO_COOL_WIDTH - 5, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
            if (THERMO_COOL_X + 10 + thermoCoolCurrLevel < 250) {
                fillCircle(THERMO_COOL_X + 10 + thermoCoolCurrLevel, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);

            } else {
                fillCircle(THERMO_COOL_X + THERMO_COOL_WIDTH - 11, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);
            }
            drawText(lan.get("heat", "connected_sensor"), 15, 165, 35, 1, 0, 16.0f, 65535, 0);
            fillRect(THERMO_HEAT_X, THERMO_HEAT_Y, THERMO_HEAT_X + THERMO_HEAT_WIDTH, THERMO_HEAT_Y + THERMO_HEAT_HEIGHT, RGB(23, 191, 227));
            drawLine(THERMO_HEAT_X + 5, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 65535);
            drawLine(THERMO_HEAT_X + 5, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X + 5, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
            drawLine(THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
            if (THERMO_HEAT_X + 10 + thermoHeatCurrLevel < 250) {
                fillCircle(THERMO_HEAT_X + 10 + thermoHeatCurrLevel, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);

            } else {
                fillCircle(THERMO_HEAT_X + THERMO_HEAT_WIDTH - 11, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);
            }
            drawText(lan.get("fan", "connected_sensor"), 15, 195, 35, 1, 0, 16.0f, 65535, 0);
            fillRect(60, 190, 135, 218, !(strstr(thermoFan, "Auto Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
            fillRect(140, 190, 215, 218, !(strstr(thermoFan, "On Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
            drawText(lan.get("auto_low", "connected_sensor"), 65, 195, 75, 1, 0, 16.0f, 65535, 1);
            drawText(lan.get("on_low", "connected_sensor"), 145, 195, 75, 1, 0, 16.0f, 65535, 1);
            if (strstr(thermoFan, "Auto Low")) {
                pngImg.drawPNG(60, 197, 1, BACKGROUND_LAYER, Tick);
            } else if (strstr(thermoFan, "On Low")) {
                pngImg.drawPNG(140, 197, 1, BACKGROUND_LAYER, Tick);
            }
            if (d.getDeviceTechnology() == ZWaveTechnology) {
                int batteryVal;
                batteryVal = atoi(d.getValue(8));
                pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                if (batteryVal != 0xff && batteryVal != 0) {
                    swprintf(opt, 5, L"%hd%%", batteryVal);
                    drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                } else {
                    drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
                }
                if ((strcmp(d.getValue(10), "#ERR"))) {
                    fillRect(60, 190, 215, 218, RR_COLOR);
                    fillRect(60, 190, 100, 218, !(strstr(thermoFan, "Auto Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
                    fillRect(110, 190, 150, 218, !(strstr(thermoFan, "On Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
                    drawText(L"Auto", 62, 195, 40, 1, 0, 16.0f, 65535, 1);
                    drawText(L"On", 110, 195, 40, 1, 0, 16.0f, 65535, 1);
                    if (strstr(thermoFan, "Auto Low")) {
                        pngImg.drawPNG(60, 197, 1, BACKGROUND_LAYER, Tick);
                    } else if (strstr(thermoFan, "On Low")) {
                        pngImg.drawPNG(110, 197, 1, BACKGROUND_LAYER, Tick);
                    }
                    er = atof(d.getValue(10));
                    pngImg.drawPNG(160, 195, 1, BACKGROUND_LAYER, humidityImage);
                    sprintf(output, "%.0lf", er);
                    aswprintf(arr, 20, L": %hs%%", output);
                    drawText(arr, 180, 195, 40, 1, 0, 16.0f, 65535, 0);
                }
            }
        }
            break;
        case SmartACSwitch:
        {
            char tmp_str[30];
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[2]);
            strcpy(tmp_str, d.getValue(1));
            swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 8, 300, 1, 0, 30.0f, 65535, 1);
            ActivePower = atof(d.getValue(11));
            RMSVoltage = atof(d.getValue(12));
            RMSCurrent = atof(d.getValue(13));
            swprintf(opt, 179, L"%ls: %0.2f W", lan.get("power", "connected_sensor"), ActivePower);
            drawText(opt, 10, 165, 300, 1, 0, 18.0f, 65535, 1);
            swprintf(opt, 179, L"%ls: %0.2f A", lan.get("current", "connected_sensor"), RMSCurrent);
            drawText(opt, 10, 185, 300, 1, 0, 18.0f, 65535, 1);
            swprintf(opt, 179, L"%ls: %0.2f V", lan.get("voltage", "connected_sensor"), RMSVoltage);
            drawText(opt, 10, 205, 290, 1, 0, 18.0f, 65535, 1);
            ACSwitchOnButton.draw();
            ACSwitchOffButton.draw();
            sprintf(t_temp, " SmartAcSwitch  %d initial state %s ActivePower: %0.3fW Current: %0.3fA Voltage: %0.3fV ", d.getID(), (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? "Off" : "On", ActivePower, RMSVoltage, RMSCurrent);
            LOG(Logger::INFO, t_temp);
        }
            break;
        case SecurifiSmartSwitch:
        {
            char tmp_str[30];
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[2]);
            strcpy(tmp_str, d.getValue(1));
            swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 8, 300, 1, 0, 30.0f, 65535, 1);
            ActivePower = atof(d.getValue(11));
            RMSVoltage = atof(d.getValue(12));
            RMSCurrent = atof(d.getValue(13));
            swprintf(opt, 179, L"%ls: %0.2f W", lan.get("power", "connected_sensor"), ActivePower);
            drawText(opt, 10 + 75, 165, 300 - 160, 1, 0, 18.0f, 65535, 1);
            swprintf(opt, 179, L"%ls: %0.2f A", lan.get("current", "connected_sensor"), RMSCurrent);
            drawText(opt, 10 + 75, 185, 300 - 160, 1, 0, 18.0f, 65535, 1);
            swprintf(opt, 179, L"%ls: %0.2f V", lan.get("voltage", "connected_sensor"), RMSVoltage);
            drawText(opt, 10 + 75, 205, 300 - 160, 1, 0, 18.0f, 65535, 1);
            ACSwitchOnButton.draw();
            ACSwitchOffButton.draw();
            AdvanceButton.draw();
            sprintf(t_temp, "SecurifiSmartSwitch  %d initial state %s ActivePower: %0.3fW Current: %0.3fA Voltage: %0.3fV ", d.getID(), (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? "Off" : "On", ActivePower, RMSVoltage, RMSCurrent);
            LOG(Logger::INFO, t_temp);
        }
            break;
        case DoorLock:
        case ZigbeeDoorLock:
        {
            wchar_t ss[180];
            //            if (d.getValue(1) == 1) {
            //                fillRect(40, 97, 430, 117, RR_COLOR);
            int batteryVal;
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[5]);
            swprintf(ss, 179, L"%ls", lan.get("doorlock", "connected_sensor"));
            if (d.getDeviceTechnology() == ZWaveTechnology) {
                if (atoi(d.getValue(1)) == 0) {
                    swprintf(opt, 179, L" %ls : %ls", ss, lan.get("unlocked", "connected_sensor"));
                } else if (atoi(d.getValue(1)) == 23 || atoi(d.getValue(1)) == 26 || atoi(d.getValue(1)) == 17 || atoi(d.getValue(1)) == 254) {
                    swprintf(opt, 179, L" %ls : %ls", ss, lan.get("notfullylocked", "connected_sensor"));
                } else {
                    swprintf(opt, 179, L"%ls : %ls", ss, lan.get("locked", "connected_sensor"));
                }
            } else {
                if (atoi(d.getValue(1)) == 1) {
                    swprintf(opt, 179, L"%ls : %ls", ss, lan.get("locked", "connected_sensor"));
                } else if (atoi(d.getValue(1)) == 2) {
                    swprintf(opt, 179, L" %ls : %ls", ss, lan.get("unlocked", "connected_sensor"));
                } else {
                    swprintf(opt, 179, L" %ls : %ls", ss, lan.get("notfullylocked", "connected_sensor"));
                }
            }
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 7, 300, 1, 0, 20.0f, 65535, 1);
            //            }

            if (atoi(d.getValue(3)) || ((d.getDeviceTechnology() == ZWaveTechnology) && atoi(d.getValue(4)))) {
                DoorLockButtonWithUserCode.draw();
                DoorUnlockButtonWithUserCode.draw();
                Userno = 1;
                drawText(lan.get("userno", "connected_sensor"), 80, 131, 100, 1, 0, 20.0f, 65535, 0);
                pngImg.drawPNG(160, 128, 1, BACKGROUND_LAYER, arrow_minus);
                pngImg.drawPNG(220, 128, 1, BACKGROUND_LAYER, arrow_plus);
                swprintf(opt, 179, L"%d", Userno);
                drawText(opt, 185, 130, 35, 1, 0, 22.0f, 65535, 1);
                drawText(lan.get("sett_pin", "connected_sensor"), 50, 160, 80, 1, 0, 20.0f, 65535, 0);
                if (d.getDeviceTechnology() == ZWaveTechnology) {
                    batteryVal = atoi(d.getValue(3));
                    if (!strcmp(d.getValue(Userno + 4), ""))
                        sprintf(Userpin, "%ls", lan.get("unassigned", "connected_sensor"));
                    else
                        strcpy(Userpin, d.getValue(Userno + 4));
                    swprintf(opt, 179, L"%s", Userpin);
                    drawText(opt, 105, 160, 120, 1, 0, 25.0f, 65535, 0);
                } else {
                    batteryVal = atoi(d.getValue(2));
                    if (!strcmp(d.getValue(Userno + 3), ""))
                        sprintf(Userpin, "%ls", lan.get("unassigned", "connected_sensor"));
                    else
                        strcpy(Userpin, d.getValue(Userno + 3));
                    drawText(L"********", 105, 160, 120, 1, 0, 25.0f, 65535, 0);
                }
                pngImg.drawPNG(243, 60, 1, 0, battery_icon);
                if (batteryVal != 0xff && batteryVal != 0) {
                    swprintf(opt, 5, L"%hd%%", batteryVal);
                    drawText(opt, 243, 62, 40, 1, 0, 12.0f, RGB(0, 0, 0), 1);
                } else {
                    drawText(lan.get("low", "connected_sensor"), 243, 62, 40, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                }
                //drawText(opt, 243, 62, 40, 1, 0, 12.0f, RGB(0, 0, 0), 1);
                pngImg.drawPNG(235, 160, 1, BACKGROUND_LAYER, pen_1);
                pngImg.drawPNG(275, 160, 1, BACKGROUND_LAYER, trash_png);
            } else {
                DoorLockButton.draw();
                DoorUnlockButton.draw();
            }
        }
            break;
        case Gateway:
        case SceneController:
        case SmartDCSwitch:
        case PressureSensor:
        case FlowSensor:
        case HAPump:
        case Shade:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[1]);
            fillRect(10, 95, 305, 225, RR_COLOR);
            pngImg.drawPNG(40, 120, 1, BACKGROUND_LAYER, critical);
            drawText(lan.get("unsupported_1", "connected_sensor"), 75, 120, 220, 1, 0, 20.0f, 65535, 1);
            drawText(lan.get("unsupported_2", "connected_sensor"), 75, 140, 220, 1, 0, 20.0f, 65535, 1);
        }
            sprintf(t_temp, " Device %d is not supported ", d.getID());
            LOG(Logger::INFO, t_temp);
            break;
        case WindowCovering:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[16]);
            fillRect(25, 95, 105, 125, BUTTON_LAYER);
            drawText(L"Up", 25, 101, 80, 1, 0, 18.0f, 65535, 1);
            fillRect(120, 95, 200, 125, BUTTON_LAYER);
            drawText(L"Down", 120, 101, 80, 1, 0, 18.0f, 65535, 1);
            fillRect(215, 95, 295, 125, BUTTON_LAYER);
            drawText(L"Stop", 215, 101, 80, 1, 0, 18.0f, 65535, 1);
        }
            break;
        case GarageDoorOpener://newal2
        {
            int DoorLevel;
            DoorLevel = atoi(d.getValue(1));
            if (DoorLevel == 255) {
                swprintf(opt, 179, L" %ls", lan.get("open", "connected_sensor"));
            } else if (DoorLevel == 254) {
                swprintf(opt, 179, L" %ls", lan.get("Opening", "connected_sensor"));
            } else if (DoorLevel == 252) {
                swprintf(opt, 179, L" %ls", lan.get("closing", "connected_sensor"));
            } else if (DoorLevel == 253) {
                swprintf(opt, 179, L" %ls", lan.get("stopped", "connected_sensor"));
            } else if (DoorLevel == 0) {
                swprintf(opt, 179, L" %ls", lan.get("closed", "connected_sensor"));
            }
            drawText(opt, 90, 100, 150, 1, 0, 30.0f, 65535, 1);
            //drawText(opt, 40, 115, 390, 1, 0, 45.0f, 65535, 1);
            GarageDoorOpenButton.draw();
            GarageDoorCloseButton.draw();
            sprintf(t_temp, "GarageDoorOpener %d Garage Door initial status is %d ", d.getID(), DoorLevel);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case MultiSoundSiren://newal2
        {
            SelectSoundButton.draw();
            int SoundType = atoi(d.getValue(1));
            if (SoundType == 0) {
                swprintf(opt, 179, L" %ls", lan.get("alarm", "connected_sensor")); //50, 120, 100, 50
                drawText(opt, 50, 130, 100, 1, 0, 30.0f, 65535, 1);
            } else if (SoundType == 1) {
                swprintf(opt, 179, L" %ls", lan.get("emergency", "connected_sensor"));
                //swprintf(opt, 179, L" %ls", L"Emergency");
                drawText(opt, 50, 135, 100, 1, 0, 20.0f, 65535, 1);
            } else if (SoundType == 2) {
                swprintf(opt, 179, L" %ls", lan.get("fire", "connected_sensor"));
                //swprintf(opt, 179, L" %ls", L"Fire");
                drawText(opt, 50, 130, 100, 1, 0, 30.0f, 65535, 1);
            } else if (SoundType == 3) {
                swprintf(opt, 179, L" %ls", lan.get("ambulance", "connected_sensor"));
                // swprintf(opt, 179, L" %ls", L"Ambulance");
                drawText(opt, 50, 135, 100, 1, 0, 20.0f, 65535, 1);
            } else if (SoundType == 4) {
                swprintf(opt, 179, L" %ls", lan.get("police", "connected_sensor"));
                //swprintf(opt, 179, L" %ls", L"Police");
                drawText(opt, 50, 130, 100, 1, 0, 30.0f, 65535, 1);
            } else if (SoundType == 5) {
                swprintf(opt, 179, L" %ls", lan.get("door_chime", "connected_sensor"));
                //swprintf(opt, 179, L" %ls", L"Door chime");
                drawText(opt, 50, 135, 100, 1, 0, 20.0f, 65535, 1);
            } else if (SoundType >= 6) {
                swprintf(opt, 179, L" %ls", lan.get("beep", "connected_sensor"));
                //swprintf(opt, 179, L" %ls", L"Beep");
                drawText(opt, 50, 130, 100, 1, 0, 30.0f, 65535, 1);
            }
            StopMSSirenButton.draw();
            // multilevelOffButton.draw();
        }
            break;

        case RollerShutter://newal2
        {
            drawRoundedRectangle(40, 110, 120, 140, BUTTON_LAYER, 255);
            drawText(lan.get("open", "connected_sensor"), 45, 115, 80, 1, 0, 20.0f, 65535, 1);
            drawRoundedRectangle(200, 110, 280, 140, BUTTON_LAYER, 255);
            drawText(L"up", 200, 115, 80, 1, 0, 20.0f, 65535, 1);
            drawRoundedRectangle(40, 150, 120, 180, BUTTON_LAYER, 255);
            drawText(L"close", 45, 155, 80, 1, 0, 20.0f, 65535, 1);
            drawRoundedRectangle(200, 150, 280, 180, BUTTON_LAYER, 255);
            drawText(L"down", 200, 155, 80, 1, 0, 20.0f, 65535, 1);
            drawRoundedRectangle(125, 130, 195, 170, RGB(247, 35, 20), 255);
            drawText(L"STOP", 135, 140, 80, 1, 0, 25.0f, 65535, 0);
            dimmerCurrLevel = atoi(d.getValue(1));
            if (dimmerCurrLevel == 0) {
                swprintf(opt, 179, L" %ls", lan.get("closed", "connected_sensor"));
            } else if (dimmerCurrLevel == 99) {
                swprintf(opt, 179, L" %ls", lan.get("open", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", L"Processing");
            }
            if (dimmerCurrLevel != 254)
                drawText(opt, 120, 100, 80, 1, 0, 25.0f, 65535, 1);


            else
                drawText(opt, 120, 105, 80, 1, 0, 15.0f, 65535, 1);
            sprintf(t_temp, "RollerShutter %d initial level is %d ", d.getID(), dimmerCurrLevel);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case EnergyReader://newal2
        {

            char tmp_str[30];
            int batteryVal;
            batteryVal = atoi(d.getValue(1));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (batteryVal != 0xff && batteryVal != 0) {
                swprintf(opt, 5, L"%hd%%", batteryVal);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
            }
            strcpy(tmp_str, d.getValue(2));
            swprintf(opt, 179, L"%ls:%hs W", lan.get("power", "connected_sensor"), tmp_str);
            drawText(opt, 60, 90, 200, 1, 0, 25.0f, 65535, 1);
            strcpy(tmp_str, d.getValue(3));
            swprintf(opt, 179, L"%ls:%hs kWh", L"energy", tmp_str);
            drawText(opt, 60, 115, 200, 1, 0, 20.0f, 65535, 1);
            drawRoundedRectangle(10, 140, 110, 210, RGB(255, 255, 255), 255);
            strcpy(tmp_str, d.getValue(4)); // clamp1
            drawText(L"clamp:1", 15, 145, 100, 1, 0, 20.0f, 0, 1);
            swprintf(opt, 179, L"%hs w", tmp_str);
            drawText(opt, 10, 165, 100, 1, 0, 20.0f, 0, 1); //clamp1: power
            strcpy(tmp_str, d.getValue(5));
            swprintf(opt, 179, L"%hs kWh", tmp_str);
            drawText(opt, 10, 185, 100, 1, 0, 18.0f, 0, 1); //clamp1: energy

            drawRoundedRectangle(115, 140, 215, 210, RGB(255, 255, 255), 255);
            strcpy(tmp_str, d.getValue(6)); // clamp2
            drawText(L"clamp:2", 115, 145, 100, 1, 0, 20.0f, 0, 1);
            swprintf(opt, 179, L"%hs w", tmp_str);
            drawText(opt, 115, 165, 100, 1, 0, 20.0f, 0, 1); //clamp2: power
            strcpy(tmp_str, d.getValue(7));
            swprintf(opt, 179, L"%hs kWh", tmp_str);
            drawText(opt, 115, 185, 100, 1, 0, 18.0f, 0, 1); //clamp2: energy

        }
            break;

        case SetPointThermostat:
        {
            if (d.getDeviceTechnology() == ZWaveTechnology) {
                wchar_t arr[20];
                int batteryVal;
                //format = FORMAT_F;
                char ThermoMode[30] = "";
                if (strlen(d.getValue(2)) != 0) {
                    batteryVal = atoi(d.getValue(2));
                    pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                    if (batteryVal != 0xff && batteryVal != 0) {
                        swprintf(opt, 5, L"%hd%%", batteryVal);
                        drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                    } else {
                        drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
                    }
                }
                if (strcmp(d.getValue(4), "#ERR")) {
                    //fillRect(200, 90, 300, 125, GREEN_COLOR);
                    memset(arr, 0, sizeof (arr)); //july15
                    fillRect(190, 95, 300, 125, GREEN_COLOR);
                    convertTemperature(4, arr);
                    drawText(arr, 200, 100, 100, 1, 0, 25.0f, 65535, 1);
                }
                //drawText(lan.get("set_temp", "connected_sensor"), 100, 125, 200, 1, 0, 22.0f, 65535, 1);
                drawText(L"SetTemp", 65, 110, 150, 1, 0, 22.0f, 65535, 1);
                pngImg.drawPNG(70, 140, 1, 0, arrow_minus);
                pngImg.drawPNG(185, 140, 1, 0, arrow_plus);
                if (strcmp(d.getValue(5), "#ERR")) {
                    strcpy(ThermoMode, d.getValue(5));
                    drawText(L"setMode", 6, 190, 75, 1, 0, 20.0f, 65535, 1);
                    EconModeButton.draw();
                    HeatModeButton.draw();
                    if (!strcmp(ThermoMode, "Heat Econ")) {
                        pngImg.drawPNG(85, 195, 1, BACKGROUND_LAYER, Tick);
                    } else {
                        pngImg.drawPNG(153, 195, 1, BACKGROUND_LAYER, Tick);
                    }
                    if (format == FORMAT_C) {
                        if (!strcmp(ThermoMode, "Heat Econ")) {
                            SetpointTmp = (atoi(d.getValue(6)) - 32)*5.0 / 9;
                        } else {
                            SetpointTmp = (atoi(d.getValue(1)) - 32)*5.0 / 9;
                        }
                        SetpointTmp = ceil(SetpointTmp);
                        sprintf(ss, "   %.2lf °C", SetpointTmp);
                    } else {//Farenheat
                        if (!strcmp(ThermoMode, "Heat Econ")) {
                            SetpointTmp = getTempinFloat(d.getValue(6));
                        } else {
                            SetpointTmp = getTempinFloat(d.getValue(1));
                        }
                        sprintf(ss, "   %.2lf °F", SetpointTmp);
                    }

                } else {
                    if (format == FORMAT_C) {
                        SetpointTmp = (atoi(d.getValue(1)) - 32)*5.0 / 9;
                        SetpointTmp = ceil(SetpointTmp);
                        sprintf(ss, "   %.2lf °C", SetpointTmp);
                    } else {
                        SetpointTmp = getTempinFloat(d.getValue(1));
                        sprintf(ss, "   %.2lf °F", SetpointTmp);
                    }
                }
                memset(opt, 0, sizeof (opt));
                utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
                TempBox.setText(opt);
                TempBox.draw();
            }
        }
            break;
        case UnknownDeviceType:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[1]);
            if (d.getDeviceTechnology() == ZigBeeTechnology) {
                fillRect(10, 95, 305, 225, RR_COLOR);
                pngImg.drawPNG(40, 120, 1, BACKGROUND_LAYER, critical);
                drawText(lan.get("unsupported_1", "connected_sensor"), 75, 120, 220, 1, 0, 20.0f, 65535, 1);
                drawText(lan.get("unsupported_2", "connected_sensor"), 75, 140, 220, 1, 0, 20.0f, 65535, 1);

            } else if (d.getDeviceTechnology() == ZWaveTechnology) {
                basicGetButton.draw();
                basicSetButton.draw();
                basicGetTextBox.draw();
                basicSetText.draw();
                basicSetTextModifyMinus.draw();
                basicSetTextModifyPlus.draw();
            }
        }
            break;
        case OccupancySensor:
        {
            char tmp_str[100];
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[15]);
            fillRect(50, 90, 270, 130, RR_COLOR);
            strcpy(tmp_str, d.getValue(1));
            swprintf(opt, 190, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? L"Unoccupied" : L"Occupied");
            drawText(opt, 60, 95, 190, 1, 0, 32.0f, 65535, 1);
            memset(opt, 0, sizeof (opt));

            swprintf(opt, 100, L"%ls", L"Temp:");
            drawText(opt, 55, 133, 100, 1, 0, 27.0f, 65535, 1);
            memset(opt, 0, sizeof (opt));
            fillRect(140, 130, 240, 165, GREEN_COLOR);
            convertTemperature(2, opt);
            drawText(opt, 140, 133, 100, 1, 0, 30.0f, 65535, 1);
            swprintf(opt, 179, L"Humidity: %s%%", d.getValue(3));
            drawText(opt, 55, 170, 170, 1, 0, 27.0f, 65535, 1);
            if (strcmp(d.getValue(4), "true") == 0) {
                pngImg.drawPNG(90, 200, 1, BACKGROUND_LAYER, battery_c);
                drawText(lan.get("battery_low", "connected_sensor"), 110, 200, 110, 1, 0, 20.0f, 65535, 1);
                LOG(Logger::INFO, "Battery low state received for occupancy sensor");
            }
            sprintf(t_temp, "OccupancySensor initial humidity:%s temperature:%s", d.getValue(3), d.getValue(2));
            LOG(Logger::INFO, t_temp);
        }
            break;
        case ZigbeeTempSensor:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[15]);
            memset(opt, 0, sizeof (opt)); //july11
            for (int i = 0; i <= d.getValueCount(); i++) {
                switch (d.getIndexType(i)) {
                    case Temperature:
                    {
                        swprintf(opt, 100, L"%ls", L"Temperature :");
                        drawText(opt, 60, 110, 110, 1, 0, 20.0f, 65535, 0);
                        fillRect(180, 105, 260, 135, GREEN_COLOR);
                        memset(opt, 0, sizeof (opt));
                        convertTemperature(i, opt);
                        drawText(opt, 185, 110, 70, 1, 0, 20.0f, 65535, 1);
                        sprintf(t_temp, "Zigbee Temperature Sensor's initial value of temperature:%s", d.getValue(i));
                        LOG(Logger::INFO, t_temp);
                    }
                        break;
                    case Humidity:
                    {
                        pngImg.drawPNG(40, 145, 1, BACKGROUND_LAYER, humidityImage);
                        drawText(L"Humidity        :", 60, 145, 110, 1, 0, 20.0f, 65535, 0);
                        swprintf(opt, 20, L"%4s%%     ", d.getValue(i));
                        drawText(opt, 185, 145, 70, 1, 0, 20.0f, 65535, 0);
                    }
                        break;
                }
            }

        }
            break;
        case SecurifiButton:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[13]);
            if (atoi(d.getValue(1)) == 0) {
                drawText(L"Two Press", NOTIFICATION_X, NOTIFICATION_Y + 10, 300, 1, 0, 35.0f, 65535, 1);
                LOG(Logger::INFO, "Initial state two press\n");
            } else if (atoi(d.getValue(1)) == 2) {
                drawText(L"Long Press", NOTIFICATION_X, NOTIFICATION_Y + 10, 300, 1, 0, 35.0f, 65535, 1);
                LOG(Logger::INFO, "Initial state long press\n");
            } else if (atoi(d.getValue(1)) == 3) {
                drawText(L"One Press", NOTIFICATION_X, NOTIFICATION_Y + 10, 300, 1, 0, 35.0f, 65535, 1);
                LOG(Logger::INFO, "Initial state one press");
            } else if (atoi(d.getValue(1)) == 4) {
                drawText(L"No Press", NOTIFICATION_X, NOTIFICATION_Y + 10, 300, 1, 0, 35.0f, 65535, 1);
                LOG(Logger::INFO, "Initial state no press");
            }

            if (strcmp(d.getValue(2), "true") == 0) {
                pngImg.drawPNG(100, NOTIFICATION_Y + 50, 1, BACKGROUND_LAYER, battery_c);
                drawText(lan.get("battery_low", "connected_sensor"), NOTIFICATION_X + 15, NOTIFICATION_Y + 53, 285, 1, 0, 20.0f, 65535, 1);
            }

            if (strcmp(d.getValue(3), "true") == 0) {
                TamperButton.draw();
            }
        }
            break;
        case GenericDevice:
        {
            bzero(&gen, sizeof (gen));
            bzero(grid1, sizeof (grid1));
            bzero(grid2, sizeof (grid2));
            bzero(grid3, sizeof (grid3));
            gen.x_next = X1_CORDT;
            gen.y_next = Y1_CORDT;
            gen.ROW1 = gen.ROW2 = gen.ROW3 = 1;
            LOG(Logger::INFO, "Drawing Generic device");
            StartIndxCnt = 1; //newgeneric
            Userno = 1;
            for (in = 1; in <= d.getValueCount(); in++) {
                //  sprintf(t_temp, "index: val %d", in);
                //  LOG(Logger::INFO,t_temp);
                if ((drawGenericDevice(contDevID, in, 0)) == FULL_PAGE)
                    break;
            }
            bzero(&gen, sizeof (gen));
            gen.ROW1 = gen.ROW2 = gen.ROW3 = 1;

        }
            break;
        case HueLamp:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[3]);
            dimmerCurrLevel = atoi(d.getValue(5));
            char tmp_str[30];
            int color_t;
            strcpy(tmp_str, d.getValue(2));
            thermoCurrent = ((dimmerCurrLevel * 100.0) / 255);
            swprintf(opt, 179, L"%ls (%d%%)", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"), roundfun(thermoCurrent));
            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 15, 300, 1, 0, 18.0f, 65535, 1);
            fillRect(25, RECT_Y, 301, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 3), 25 + 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
            drawLine(301 - 5, RECT_Y + (RECT_HEIGHT / 3), 301 - 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
            fillCircle(25 + 10 + dimmerCurrLevel, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
            color_t = atoi(d.getValue(5));
            color_t = (color_t - 1000) / 32 + 25;
            pngImg.drawPNG(25, RECT_Y + 38, 1, 0, color_png);
            fillRect(280, RECT_Y + 38, 300, 187, RGB(255, 255, 255));
            colorOnButton.draw();
            colorOffButton.draw();
            settingsButton.draw();
            sprintf(t_temp, "HueLamp %d initial level is %d ", d.getID(), dimmerCurrLevel);
            LOG(Logger::INFO, t_temp);
        }
            break;
        case NestThermostat:
        {
            //printf("\nNestThermostat\n");
            wchar_t arr[50];
            double er;
            char temp[50];
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[7]);
            er = getTempinFloat(d.getValue(5));
            thermoCool = (int) er;
            er = getTempinFloat(d.getValue(6));
            thermoHeat = (int) er;
            er = getTempinFloat(d.getValue(3));
            thermotarget = (int) er;
            strcpy(thermoMode, d.getValue(2));
            strcpy(thermoState, d.getValue(11));
            strcpy(thermoaway, d.getValue(8));
            strcpy(thermounit, d.getValue(7));

            drawText(lan.get("mode", "connected_sensor"), 15, 97 + 6, 35, 1, 0, 16.0f, 65535, 0);


            fillRect(55, 97, 123, 125, (strcmp(thermoMode, "heat-cool")) ? BUTTON_LAYER : RGB(23, 191, 227));
            drawText(L"Heat-Cool", 70, 105, 50, 1, 0, 13.0f, 65535, 1);
            if ((!strcmp(d.getValue(12), "false")) || (!strcmp(d.getValue(13), "false"))) {
                drawTransparentRectangle(55, 97, 123, 125, 0, 100);
            }
            fillRect(127, 97, 177, 125, (strcmp(thermoMode, "Cool")) ? BUTTON_LAYER : RGB(23, 191, 227));
            drawText(lan.get("cool", "connected_sensor"), 132, 105, 50, 1, 0, 13.0f, 65535, 1);
            fillRect(181, 97, 231, 125, (strcmp(thermoMode, "Heat")) ? BUTTON_LAYER : RGB(23, 191, 227));
            drawText(lan.get("heat", "connected_sensor"), 186, 105, 50, 1, 0, 13.0f, 65535, 1);
            if (strstr(thermoMode, "heat-cool")) {
                pngImg.drawPNG(57, 105, 1, BACKGROUND_LAYER, Tick);
            } else if (strstr(thermoMode, "cool")) {
                pngImg.drawPNG(129, 105, 1, BACKGROUND_LAYER, Tick);
            } else if (strstr(thermoMode, "heat")) {
                pngImg.drawPNG(183, 105, 1, BACKGROUND_LAYER, Tick);
            }

            if (!strcmp(thermoState, "true")) {
                fillRect(235, 97, 303, 125, GREEN_COLOR);
                memset(arr, 0, sizeof (arr));
                convertTemperature(10, arr);
                drawText(arr, 245, 97 + 6, 55, 1, 0, 18.0f, 65535, 0);
            } else {
                fillRect(235, 97, 303, 125, RGB(255, 0, 0));
                drawText(L"Offline", 245, 97 + 6, 60, 1, 0, 18.0f, 65535, 0);
            }

            fillRect(THERMO_COOL_X - 5, THERMO_COOL_Y, THERMO_COOL_X + THERMO_COOL_WIDTH + 43, THERMO_COOL_Y + THERMO_COOL_HEIGHT, RGB(23, 191, 227));
            drawLine(THERMO_COOL_X, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 65535);
            drawLine(THERMO_COOL_X, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
            drawLine(THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);


            fillRect(THERMO_HEAT_X - 5, THERMO_HEAT_Y, THERMO_HEAT_X + THERMO_HEAT_WIDTH + 43, THERMO_HEAT_Y + THERMO_HEAT_HEIGHT, RGB(23, 191, 227));
            drawLine(THERMO_HEAT_X, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 65535);
            drawLine(THERMO_HEAT_X, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
            drawLine(THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
            if (!strcmp(thermoMode, "heat-cool")) {
                drawText(L"Low", 15, 135, 35, 1, 0, 16.0f, 65535, 0);
                drawText(L"High", 15, 165, 35, 1, 0, 16.0f, 65535, 0);
                if (thermoCool < 50) {
                    thermoCool = 50;
                } else if (thermoCool > 90) {
                    thermoCool = 90;
                }
                if (thermoHeat < 50) {
                    thermoHeat = 50;
                } else if (thermoHeat > 90) {
                    thermoHeat = 90;
                }
                thermoCoolCurrLevel = (thermoCool - 50) * 6;
                thermoHeatCurrLevel = (thermoHeat - 50) * 6;
                fillCircle(THERMO_COOL_X + thermoCoolCurrLevel, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);
                fillCircle(THERMO_HEAT_X + thermoHeatCurrLevel, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);
            } else if (!strcmp(thermoMode, "heat")) {
                drawText(lan.get("cool", "connected_sensor"), 15, 135, 35, 1, 0, 16.0f, 65535, 0);
                drawText(lan.get("heat", "connected_sensor"), 15, 165, 35, 1, 0, 16.0f, 65535, 0);
                drawTransparentRectangle(55, 130, 303, 155, 0, 100);
                if (thermotarget < 50) {
                    thermotarget = 50;
                } else if (thermotarget > 90) {
                    thermotarget = 90;
                }
                thermoHeatCurrLevel = (thermotarget - 50) * 6;
                fillCircle(THERMO_HEAT_X + thermoHeatCurrLevel, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);
            } else if (!strcmp(thermoMode, "cool")) {
                drawText(lan.get("cool", "connected_sensor"), 15, 135, 35, 1, 0, 16.0f, 65535, 0);
                drawText(lan.get("heat", "connected_sensor"), 15, 165, 35, 1, 0, 16.0f, 65535, 0);
                drawTransparentRectangle(55, 160, 303, 185, 0, 100);
                if (thermotarget < 50) {
                    thermotarget = 50;
                } else if (thermotarget > 90) {
                    thermotarget = 90;
                }
                thermoCoolCurrLevel = (thermotarget - 50) * 6;
                fillCircle(THERMO_COOL_X + thermoCoolCurrLevel, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);
            } else if (!strcmp(thermoMode, "off")) {
                drawText(lan.get("cool", "connected_sensor"), 15, 135, 35, 1, 0, 16.0f, 65535, 0);
                drawText(lan.get("heat", "connected_sensor"), 15, 165, 35, 1, 0, 16.0f, 65535, 0);
                drawTransparentRectangle(55, 130, 303, 155, 0, 100);
                drawTransparentRectangle(55, 160, 303, 185, 0, 100);
            }

            if (strcmp(d.getValue(12), "true")) {
                drawTransparentRectangle(55, 130, 303, 155, 0, 100);
                drawTransparentRectangle(127, 97, 177, 125, 0, 100);
            }
            if (strcmp(d.getValue(13), "true")) {
                drawTransparentRectangle(55, 160, 303, 185, 0, 100);
                drawTransparentRectangle(181, 97, 231, 125, 0, 100);
            }
            drawText(L"State", 15, 202, 35, 1, 0, 16.0f, 65535, 0);
            fillRect(THERMO_HEAT_X - 5, 190, 136, 220, BUTTON_LAYER);
            fillRect(138, 190, 219, 220, BUTTON_LAYER);
            if (!strcmp(thermoaway, "away")) {
                drawText(L"Away", THERMO_HEAT_X + 5, 198, 50, 1, 0, 18.0f, 65535, 1);
                drawTransparentRectangle(55, 97, 125, 125, 0, 100);
                drawTransparentRectangle(127, 97, 177, 125, 0, 100);
                drawTransparentRectangle(181, 97, 231, 125, 0, 100);
                drawTransparentRectangle(55, 130, 303, 155, 0, 100);
                drawTransparentRectangle(55, 160, 303, 185, 0, 100);
                drawTransparentRectangle(138, 190, 219, 220, 0, 100);
            } else if (!strcmp(thermoaway, "home")) {
                drawText(L"Home", THERMO_HEAT_X + 5, 198, 50, 1, 0, 18.0f, 65535, 1);
            } else if (!strcmp(thermoaway, "auto-away")) {
                drawText(L"Auto-Away", THERMO_HEAT_X + 4, 198, 65, 1, 0, 15.0f, 65535, 0);
                drawTransparentRectangle(55, 97, 125, 125, 0, 100);
                drawTransparentRectangle(127, 97, 177, 125, 0, 100);
                drawTransparentRectangle(181, 97, 231, 125, 0, 100);
                drawTransparentRectangle(55, 130, 303, 155, 0, 100);
                drawTransparentRectangle(55, 160, 303, 185, 0, 100);
                drawTransparentRectangle(138, 190, 219, 220, 0, 100);
            } else if (!strcmp(thermoaway, "unknown")) {
                drawText(L"Unknown", THERMO_HEAT_X + 5, 198, 50, 1, 0, 18.0f, 65535, 1);
            }
            if (!strcmp("true", d.getValue(15))) {
                pngImg.drawPNG(143, 195, 1, BACKGROUND_LAYER, fanImage);
                sprintf(temp, "%s", d.getValue(9));
                swprintf(opt, 190, L" : %ls", (temp[0] == 'f' || temp[0] == 'F') ? L"Stop" : L"Start");
                drawText(opt, 165, 196, 60, 1, 0, 18.0f, 65535, 0);
            } else {
                pngImg.drawPNG(143, 195, 1, BACKGROUND_LAYER, fanImage);
                sprintf(temp, "%s", d.getValue(9));
                swprintf(opt, 190, L" : %ls", (temp[0] == 'f' || temp[0] == 'F') ? L"Stop" : L"Start");
                drawText(opt, 165, 196, 60, 1, 0, 18.0f, 65535, 0);
                //drawTransparentRectangle(208, 240, 316, 275, 0, 100);
                drawTransparentRectangle(138, 190, 219, 220, 0, 100);
            }
            pngImg.drawPNG(225, 195, 1, BACKGROUND_LAYER, humidityImage);
            swprintf(opt, 100, L" : %s %%", d.getValue(4));
            drawText(opt, 250, 198, 60, 1, 0, 18.0f, 65535, 0);
            if (!strcmp(d.getValue(14), "true")) {
                readArea(0, 0, 319, 239);
                Modal mW;
                mW.setLine1(L"Emergency Heat");
                mW.setLine2(L"is active");
                mW.modalReturn = &doNothing;
                mW.draw(MODAL_TYPE_INFORMATION, 3);
                drawArea();
                drawTransparentRectangle(55, 97, 123, 125, 0, 100);
                drawTransparentRectangle(127, 97, 177, 125, 0, 100);
                drawTransparentRectangle(181, 97, 231, 125, 0, 100);
            }
        }
            break;
        case NestSmokeDetector:
        {
            char check[20];
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[17]);
            strcpy(thermoState, d.getValue(5));
            if (!strcmp(thermoState, "false")) {
                if (!strcmp(thermoState, "true")) {
                    //  fillCircle(90, 121, 10, RGB(102, 204, 0));
                } else {
                    drawText(L"Offline", 100, 130, 180, 1, 0, 40.0f, RGB(184, 184, 190), 0);
                }
            } else {
                strcpy(thermoState, d.getValue(5));
                sprintf(check, "%s", (d.getValue(2)));
                //printf("\nbattery = %s\n", check);
                drawText(L"Battery", 65, 190, 100, 1, 0, 20.0f, 65535, 0);
                drawText(L":", 150, 190, 10, 1, 0, 20.0f, 65535, 0);
                if (!strcmp(check, "ok")) {
                    drawText(L"Okay", 170, 190, 100, 1, 0, 20.0f, RGB(57, 194, 38), 0);
                } else if (!strcmp(check, "replace")) {
                    drawText(L"Low", 170, 190, 100, 1, 0, 20.0f, RGB(230, 49, 23), 0);
                }
                sprintf(check, "%s", (d.getValue(4)));
                //printf("\nsmoke = %s\n", check);
                drawText(L"Smoke", 65, 110, 100, 1, 0, 20.0f, 65535, 0);
                drawText(L":", 150, 110, 10, 1, 0, 20.0f, 65535, 0);
                if (!strcmp(check, "ok")) {
                    drawText(L"Okay", 170, 110, 100, 1, 0, 20.0f, RGB(57, 194, 38), 0);
                } else if (!strcmp(check, "warning")) {
                    drawText(L"Warning", 170, 110, 100, 1, 0, 20.0f, RGB(251, 205, 54), 0);
                } else if (!strcmp(check, "emergency")) {
                    drawText(L"Emergency", 170, 110, 100, 1, 0, 20.0f, RGB(255, 0, 0), 0);
                }
                sprintf(check, "%s", d.getValue(3));
                //printf("\nco alarm = %s\n", check);
                drawText(L"CO alarm", 65, 150, 100, 1, 0, 20.0f, 65535, 0);
                drawText(L":", 150, 150, 10, 1, 0, 20.0f, 65535, 0);
                if (!strcmp(check, "ok")) {
                    drawText(L"Okay", 170, 150, 100, 1, 0, 20.0f, RGB(57, 194, 38), 0);
                } else if (!strcmp(check, "warning")) {
                    drawText(L"Warning", 170, 150, 100, 1, 0, 20.0f, RGB(251, 205, 54), 0);
                } else if (!strcmp(check, "emergency")) {
                    drawText(L"Emergency", 170, 150, 100, 1, 0, 20.0f, RGB(255, 0, 0), 0);
                }
            }
        }
            break;
        case AlmondSiren:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[6]);
            SirenButton.draw();
            drawText(L"Siren", 50, 107, 60, 1, 0, 18.0f, 65535, 0);
            AlarmButton.draw();
            drawText(L"Alarm", 135, 107, 60, 1, 0, 18.0f, 65535, 0);
            DoorBellButton.draw();
            drawText(L"Door Bell", 230, 107, 70, 1, 0, 18.0f, 65535, 0);
            checkRadioButtonState(atoi(d.getValue(3)), TONE_SECTION);

            LowVolume.draw();
            drawText(L"Low", 50, 152, 50, 1, 0, 18.0f, 65535, 0);
            MedVolume.draw();
            drawText(L"Medium", 135, 152, 60, 1, 0, 18.0f, 65535, 0);
            HighVolume.draw();
            drawText(L"High", 230, 152, 50, 1, 0, 18.0f, 65535, 0);
            checkRadioButtonState(atoi(d.getValue(4)), VOLUME_SECTION);

            fillRect(20, 185, 100, 215, RGB(23, 191, 227));
            if (strcmp(d.getValue(2), "true") == 0) {
                StartSiren = true;
                drawText(L"Stop", 20, 191, 80, 1, 0, 18.0f, 65535, 1);
            } else {
                StartSiren = false;
                drawText(L"Ring", 20, 191, 80, 1, 0, 18.0f, 65535, 1);
                drawText(L"for", 105, 195, 65, 1, 0, 20.0f, 65535, 1);
            }
            AlmondTimer.draw();
            if (atoi(d.getValue(5)) != 0) {
                swprintf(As_opt, 179, L"%s", d.getValue(5));
            } else {
                swprintf(As_opt, 179, L"%ls", L"∞");
            }
            AlmondTimer.setText(As_opt);
            drawText(L"seconds", 240, 197, 65, 1, 0, 18.0f, 65535, 0);
        }
            break;

        case AlmondBlink:
        {
            double bright = 0.0;
            //            unsigned int savedColor = 0;
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[14]);
            presentColor = atoi(d.getValue(3));

            Ledhue = convertRGBtoHSV(presentColor, &bright, &LedSatur);
            fillRect(25, 95, 125, 125, RGB(23, 191, 227));
            if (strcmp(d.getValue(1), "false") == 0) {
                StartLED = false;//true actually
                drawText(L"Turn On", 25, 101, 100, 1, 0, 18.0f, 65535, 1);
            } else {
                StartLED = false;//false actually
                drawText(L"Turn Off", 25, 101, 100, 1, 0, 18.0f, 65535, 1);
            }
            //            rectColor = (((((savedColor & 0xff0000)>>16)&0xF8)<<8)|((((savedColor & 0x00ff00)>>8)&0xFC)<<3)|(((savedColor & 0x0000ff)>>3)));
            fillRect(25, 145, 125, 175, RGB(23, 191, 227));
            drawText(L"Brightness", 25, 151, 100, 1, 0, 18.0f, 65535, 1);
            drawText(L":", 140, 150, 20, 1, 0, 20.0f, 65535, 1);
            swprintf(opt, 179, L"%0.0f %ls", (bright * 100), L"%");
            drawText(opt, 170, 150, 50, 1, 0, 20.0f, 65535, 1);
            AlmondLedTimer.draw();
            if (atoi(d.getValue(4)) != 0) {
                swprintf(As_opt, 179, L"%s", d.getValue(4));
            } else {
                swprintf(As_opt, 179, L"%ls", L"∞");
            }
            AlmondLedTimer.setText(As_opt);

            fillRect(25, 190, 125, 220, RGB(23, 191, 227));
            if (strcmp(d.getValue(2), "true") == 0) {
                StartBlinkLED = false;
                drawText(L"Stop Blinking", 25, 196, 100, 1, 0, 18.0f, 65535, 1);
            } else {
                StartBlinkLED = true;
                drawText(L"Blink", 25, 196, 100, 1, 0, 18.0f, 65535, 1);
                drawText(L"for", 130, 200, 40, 1, 0, 20.0f, 65535, 1);
            }
            drawText(L"seconds", 235, 202, 60, 1, 0, 18.0f, 65535, 0);
        }
            break;

        case MultiSwitch:
        {
            char tmp_str[30];
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[2]);
            drawText(L"Switch 1", 20, 110, 70, 1, 0, 20.0f, 65535, 0);
            MultiSwitchOnButton.draw();
            MultiSwitchOffButton.draw();
            strcpy(tmp_str, d.getValue(1));
            swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
            drawText(opt, NOTIFICATION_X + 245, NOTIFICATION_Y, 50, 1, 0, 25.0f, 65535, 1);
            drawText(L"Switch 2", 20, 160, 70, 1, 0, 20.0f, 65535, 0);
            MultiSwitchOnButton1.draw();
            MultiSwitchOffButton1.draw();
            strcpy(tmp_str, d.getValue(2));
            swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
            drawText(opt, NOTIFICATION_X + 245, NOTIFICATION_Y + 50, 50, 1, 0, 25.0f, 65535, 1);
        }
            break;
        case ZBIRExtender:
        {
            sensorImg.drawPNG(13, 58, 1, 0, ImageLoaded[7]);

            swprintf(opt, 179, L"%s", d.getValue(2));
            ApplianceOne.setText(opt);
            ApplianceOne.draw();

            swprintf(opt, 179, L"%s", d.getValue(3));
            ApplianceTwo.setText(opt);
            ApplianceTwo.draw();

            swprintf(opt, 179, L"%s", d.getValue(4));
            ApplianceThree.setText(opt);
            ApplianceThree.draw();

            swprintf(opt, 179, L"%s", d.getValue(5));
            ApplianceFour.setText(opt);
            ApplianceFour.draw();

            swprintf(opt, 179, L"%s", d.getValue(6));
            ApplianceFive.setText(opt);
            ApplianceFive.draw();

            swprintf(opt, 179, L"%s", d.getValue(7));
            ApplianceSix.setText(opt);
            ApplianceSix.draw();

            ChangeApplianceName.draw();
        }
            break;
        case ZigbeeCamImage:
        {
            btnCapture.draw();
            swprintf(btnImageCount.W.text, 179, L"%d", atoi(d.getValue(6)));
            if (atoi(d.getValue(7)) == 1) {
                drawText(L"640 X 480 pixel", 130, 210, 90, 1, 0, 15.0f, 65535, 0);
                wcscpy(btnResolution.W.text, L"VGA");
            } else {
                wcscpy(btnResolution.W.text, L"QVGA");
                drawText(L"320 X 240 pixel", 130, 210, 90, 1, 0, 15.0f, 65535, 0);
            }
            btnResolution.draw();
            btnImageCount.draw();
            drawText(L"Capture Images    :", 20, 100, 150, 1, 0, 18.0f, 65535, 0);
            drawText(L"Number Of Snaps :", 20, 140, 150, 1, 0, 18.0f, 65535, 0);
            drawText(L"Resolution             :", 20, 180, 150, 1, 0, 18.0f, 65535, 0);

        }
            break;
        default:
            basicGetButton.draw();
            basicSetButton.draw();
            basicGetTextBox.draw();
            basicSetText.draw();
            basicSetTextModifyMinus.draw();
            basicSetTextModifyPlus.draw();
            break;
    }
    memset(opt, 0, sizeof opt);
    MODE = CONTROL_MODE;
    if ((d.getDeviceType() != NestThermostat) && (d.getDeviceType() != NestSmokeDetector) && (d.getDeviceTechnology() != AlmondTechnology)) {
        EditButton.draw();
    }
}

void genericCallback(Device*dev) {
}

void doorLock_onTouch(void) {
    try {
        Device d(contDevID);
        if (d.getDeviceTechnology() == ZigBeeTechnology) {
            d.setValue(1, "1");
        } else if (d.getDeviceTechnology() == ZWaveTechnology) {
            d.setValue(1, "255");
            if (d.getDeviceType() != GenericDevice) {
                fillRect(NOTIFICATION_X, NOTIFICATION_Y - 10, 300, NOTIFICATION_Y + 20, RR_COLOR);
                drawText(lan.get("doorlockprocessing", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y, 300, 1, 0, 18.0f, 65535, 1);
            }
        }
    } catch (int exeption) {
        return;
    }
    sprintf(t_temp, "Doorlock %d lock ", contDevID);
    LOG(Logger::INFO, t_temp);
}

void doorUnlock_onTouch(void) {
    try {
        Device d(contDevID);
        if (d.getDeviceTechnology() == ZigBeeTechnology) {
            d.setValue(1, "0");
        } else if (d.getDeviceTechnology() == ZWaveTechnology) {
            d.setValue(1, "0");
            if (d.getDeviceType() != GenericDevice) {
                fillRect(NOTIFICATION_X, NOTIFICATION_Y - 10, 300, NOTIFICATION_Y + 20, RR_COLOR);
                drawText(lan.get("doorlockprocessing", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y, 300, 1, 0, 18.0f, 65535, 1);
            }
        }
    } catch (int exeption) {
        return;
    }
    sprintf(t_temp, "Doorlock %d  Unlock ", contDevID);
    LOG(Logger::INFO, t_temp);
}

void switchOn_onTouch(void) {

    connectedHomeSetValue(1, "true");
}

void switchOn_offTouch(void) {

    connectedHomeSetValue(1, "false");
}

void RemoveDevice(int id) {

    for (list<peanut_plug>::iterator iterator = peanut_plug_info.begin(); iterator != peanut_plug_info.end(); ++iterator) {
        if (iterator->S_Dev_ID == id) {
            peanut_plug_info.erase(iterator);
            break;
        }
    }

    for (list <Device>::iterator iterator = dL.devices.begin(); iterator != dL.devices.end(); ++iterator) {
        if ((*iterator).getID() == id) {
            dL.devices.erase(iterator);
            try {
                Device d(id);
                d.removeDevice();
            } catch (int i) {
                char bbuf[100] = "";
                sprintf(bbuf, "Failed to load device %d in remove device", id);
                LOG(Logger::INFO, bbuf);
            }
            break;
        }
    }
    int cnt = 0;
    for (list <Device>::iterator iterator = dL.devices.begin(); (iterator != dL.devices.end()) && (cnt < 250); ++iterator) {
        devIdList[cnt++] = (*iterator).getID();
    }

    sensorsCount = dL.devices.size();
}

void YesButton_onTouch(void) {

    Device d(contDevID);
    if (d.getDeviceTechnology() == ZigBeeTechnology) {
        d.getRemovedeviceid();
    }
    RemoveDevice(contDevID);
    MODE = LIST_MODE;
    lcd_clear(BACKGROUND_LAYER);
    connectedHome_draw();
}

void NoButton_onTouch(void) {
    drawArea();
    MODE = KEY_MODE;
}

void Remove_onTouch(void) {
    readArea(0, 0, 319, 239);
    fillRect(10, 95, 305, 225, RR_COLOR);
    drawText(lan.get("remove_l1", "connected_sensor"), 10, 100, 300, 1, 0, 18.0f, 65535, 1);
    drawText(lan.get("remove_l2", "connected_sensor"), 10, 119, 300, 1, 0, 18.0f, 65535, 1);
    YesButton.draw();
    NoButton.draw();
    MODE = REMOVE_MODE;
}

void EditButton_onTouch(void) {
    gen.p_area = 0;
    MODE = KEY_MODE;
    Device *d = getDevice(contDevID);
    readArea(0, 0, 319, 239);
    EditNameandLocation();
    if ((d->getDeviceTechnology() == ZWaveTechnology) && (d->getDeviceType() != ZWtoACIRExtender)) {
        ConfigurationButton1.draw();
    }
    LOG(Logger::INFO, "Edit button on touch");
}

void Tamper_onTouch(void) {
    
    Device *d = getDevice(contDevID);
    connectedHomeSetValue(d->getIndexNoFromName("TAMPER", -1), "false");
    fillRect(15, 190, 95, 220, RR_COLOR);

}

void SelectSoundButton_onTouch(void) {//newal2
    MultiSoundFlag = true;
    LOG(Logger::PRINT, "Select Sound Button on touch");
    drawRoundedRectangle(10, 100, 310, 230, RR_COLOR, 255);
    //fillRect(45, 90, 435, 275, RR_COLOR);
    EmergencySoundButton.draw();
    FireSoundButton.draw();
    AmbulanceSoundButton.draw();
    PoliceSoundButton.draw();
    DoorSoundButton.draw();
    BeepSoundButton.draw();
    device_key = contDev;
    MODE = DEVICE_MODE;

}

void Panic_onTouch(void) {
    
    connectedHomeSetValue(2, "Panic");
    fillRect(15, 190, 95, 220, RR_COLOR);

}

void Emer_onTouch(void) {
    
    connectedHomeSetValue(2, "Emer");
    fillRect(100, 190, 180, 220, RR_COLOR);
}

void TypeOfSoundButton_onTouch(void) {//newgeneric
    if (device_key == GenericDevice) {
        gen.p_area = 0.0;
        MODE = CONTROL_MODE;
    }
    drawControlDevice(*getDevice(contDevID));
}

void SelectedSoundType(int val) {//newgeneric

    connectedHomeSetValue(1, val);
}



void GarageDoorOpen_onTouch(void) {//newgeneric

    connectedHomeSetValue(1, "255");
}

void EconModeButton_onTouch(void) {//newal2
    connectedHomeSetValue(2, "Heat Econ");
}

void HeatModeButton_onTouch(void) {//newal2

    connectedHomeSetValue(2, "Heat");
}

void GarageDoorClose_onTouch(void) {//newgeneric

    connectedHomeSetValue(1, "0");
}

void SetConfigParam(int param, int size, int value) {//newgeneric
    Device *d = getDevice(contDevID);
    if (d) {
        d->SetConfigParam(param, size, value);
        sprintf(t_temp, "Setting Config parameter for ZW Device %d parameter=%d, value=%d , size=%d", contDevID, param, value, size);
        LOG(Logger::INFO, t_temp);
    } else {
        char bufLog[200] = "";
        sprintf(bufLog, "Device %d doesn't exists in list to check", contDevID);
        LOG(Logger::ERROR, bufLog);
    }

}

void TempBoxOnTouch(void) {//newal2
    double tp = 0;
    double er;
    Device *d = getDevice(contDevID);
    if (d) {
        if (format == FORMAT_C) {
            if (strcmp(d->getValue(5), "#ERR")) {
                if (!strcmp(d->getValue(5), "Heat Econ")) {
                    er = (atoi(d->getValue(6)) - 32)*5.0 / 9;
                } else {
                    er = (atoi(d->getValue(1)) - 32)*5.0 / 9;
                }
            } else {
                er = (atoi(d->getValue(1)) - 32)*5.0 / 9;
            }
            tp = ceil(er);
        } else {
            if (strcmp(d->getValue(5), "#ERR")) {
                if (!strcmp(d->getValue(5), "Heat Econ")) {
                    tp = atoi(d->getValue(6));
                } else {
                    tp = atoi(d->getValue(1));
                }
            } else {
                tp = atoi(d->getValue(1));
            }
        }
    }
    readArea(0, 0, 319, 239);
    sprintf(tmpc, "%.0lf", tp);
    LOG(Logger::ERROR, tmpc);
    MODE = KEYBOARD_MODE;
    kbMode = true;
    kb6->LT = 3; /////heatitnew
    kb6->GT = 1;
    kb6->noCommit = true;
    kb6->keyboardMode(NUMBER_ONLY);
    wmemset(N_opt, 0, 180);
    kb6->drawText(tmpc, N_opt);

    LOG(Logger::PRINT, "TempBoxOnTouch to Set Temperature");
}

void ConfigurationButton_onTouch(void) {//newal2
    ConMode = true;
    LOG(Logger::PRINT, "ConfigurationButton on touch");
    int tempvar = 0;
    Device *d = getDevice(contDevID);
    fillRect(10, 90, 300, 230, RR_COLOR);
    if (d->getDeviceType() == ZWtoACIRExtender) {
        drawText(L"Enter", 25, 93, 100, 1, 0, 24.0f, 65535, 1);
        drawText(L"IR Code", 25, 113, 100, 1, 0, 24.0f, 65535, 1);
        IRCodeSetText.draw();
        tempvar = atoi(d->getValue(10));
        if (IRCodeFlag == true) {
            swprintf(opt, 179, L"%s", tmpc);
            drawText(opt, 80, 108, 240, 1, 0, 24.0f, 0, 1);
        } else {
            swprintf(opt, 179, L"%d", tempvar);
            drawText(opt, 83, 108, 150, 1, 0, 24.0f, 0, 1);
        }
        IRCodeSetButton.draw();
    } else {
        drawText(L"Set Configuration value", 30, 100, 250, 1, 0, 25.0f, 65535, 1);
    }
    drawText(L"Parameter", 10, 200, 80, 1, 0, 20.0f, 65535, 1);
    drawText(L"Value Size", 95, 200, 80, 1, 0, 20.0f, 65535, 1);
    drawText(L"Value", 176, 200, 60, 1, 0, 20.0f, 65535, 1);
    //IRCodeSetText.draw();
    //tempvar = atoi(d->getValue(10));
    /* if (IRCodeFlag == true) {
         swprintf(opt, 179, L"%s", tmpc);
         drawText(opt, 80, 108, 240, 1, 0, 24.0f, 0, 1);
     } else {
         swprintf(opt, 179, L"%d", tempvar);
         drawText(opt, 80, 108, 240, 1, 0, 24.0f, 0, 1);
     }*/
    ParameterSetText.draw();
    ValueSetText.draw();
    if (ParameterFlag == true) {
        if (ValueFlag == false) {
            strcpy(ConfigParam, ConfigSet);
        }
        swprintf(opt, 179, L"%s", ConfigParam);
        drawText(opt, 10, 150, 60, 1, 0, 20.0f, 0, 1);
    } else {
        ParameterSetText.draw();
    }
    if (ValueFlag == true) {
        strcpy(ConfigValue, ConfigSet);
        swprintf(opt, 179, L"%s", ConfigValue);
        drawText(opt, 176, 150, 40, 1, 0, 24.0f, 0, 1);
    } else {
        ValueSetText.draw();
    }

    ValueSizeSetTextModifyPlus.draw();
    ValueSizeSetText.draw();
    ValueSizeSetTextModifyMinus.draw();
    SetConfigurationButton.draw();
    if (ParameterFlag && ValueSizeFlag) {
        swprintf(opt, 10, L"%d", tempvaluesize);
        drawText(opt, 103, 150, 40, 1, 0, 20.0f, 0, 1);
    } else {
        ValueSizeSetText.draw();
    }
    device_key = contDev;
    MODE = DEVICE_MODE;
}

void AlarmButton_onTouch(void) {
    Device d(contDevID);
    if ((atoi(d.getValue(3)) != 1)) {
        d.setValue(3, "1");
        LOG(Logger::INFO, "Setting tone no:1 for AlmondSiren");
    }
}

void SirenButton_onTouch(void) {
    Device d(contDevID);
    if ((atoi(d.getValue(3)) != 2)) {
        d.setValue(3, "2");
        LOG(Logger::INFO, "Setting tone no:2 for AlmondSiren");
    }

}

void DoorBellButton_onTouch(void) {
    Device d(contDevID);
    if ((atoi(d.getValue(3)) != 3)) {
        d.setValue(3, "3");
        LOG(Logger::INFO, "Setting tone no:3 for AlmondSiren");
    }

}

void AlmondSirenLowVolume_onTouch(void) {
    Device d(contDevID);
    if ((atoi(d.getValue(4)) != 1)) {
        d.setValue(4, "1");
        LOG(Logger::INFO, "Setting AlmondSiren volume level:1");
    }
}

void AlmondSirenMedVolume_onTouch(void) {
    Device d(contDevID);
    if ((atoi(d.getValue(4)) != 2)) {
        d.setValue(4, "2");
        LOG(Logger::INFO, "Setting AlmondSiren volume level:2");
    }
}

void AlmondSirenHighVolume_onTouch(void) {
    Device d(contDevID);
    if ((atoi(d.getValue(4)) != 3)) {
        d.setValue(4, "3");
        LOG(Logger::INFO, "Setting AlmondSiren volume level:3");
    }
}

void ParameterBox_onTouch(void) {//newal2
    LOG(Logger::INFO, "ParameterBox_onTouch");
    readArea(0, 0, 319, 239);
    ParameterFlag=ConValFlag=false;
    MODE = KEYBOARD_MODE;
    kb7->LT = 3;
    kb7->GT = 1;
    kb7->noCommit = true;
    kb7->keyboardMode(NUMBER_ONLY);
    wmemset(ConfigParam_opt, 0, 180);
    kb7->drawText("", ConfigParam_opt);
}

void ConfigValueBox_onTouch(void) {//newal2
    readArea(0, 0, 319, 239);
    MODE = KEYBOARD_MODE;
    kb7->LT = 3;
    kb7->GT = 1;
    kb7->noCommit = true;
    kb7->keyboardMode(NUMBER_ONLY);
    wmemset(ConfigParam_opt, 0, 180);
    kb7->drawText("", ConfigParam_opt);

}

void ValueSizeSetTextModifyPlus_onTouch(void) {//newal2
    if (tempvaluesize < 4)
        tempvaluesize++;
    if (tempvaluesize == 3)
        tempvaluesize = 4;
    swprintf(opt, 10, L"%d", tempvaluesize);
    ValueSizeSetText.draw();
    drawText(opt, 105, 150, 40, 1, 0, 20.0f, 0, 1);
    if (tempvaluesize > 0)
        ValueSizeFlag = true;
}

void ValueSizeSetTextModifyMinus_onTouch(void) {//newal2

    if (tempvaluesize > 1)
        tempvaluesize--;
    if (tempvaluesize >= 3) {
        tempvaluesize = 2;
        swprintf(opt, 10, L"%d", tempvaluesize);
    } else {
        swprintf(opt, 10, L"%d", tempvaluesize);
    }
    ValueSizeSetText.draw();
    drawText(opt, 105, 150, 40, 1, 0, 20.0f, 0, 1);
    if (tempvaluesize > 0)
        ValueSizeFlag = true;
}

void checkRadioButtonState(int Selected, int section) {
    sprintf(t_temp, "Setting tone/Volume no:%d for AlmondSiren", Selected);
    LOG(Logger::INFO, t_temp);

    if (section == TONE_SECTION) {
        switch (Selected) {
            case 1:
                AlarmButton.setSelected(1);
                AlarmButton.draw();
                SirenButton.setSelected(0);
                SirenButton.draw();
                DoorBellButton.setSelected(0);
                DoorBellButton.draw();
                break;
            case 2:
                SirenButton.setSelected(1);
                SirenButton.draw();
                AlarmButton.setSelected(0);
                AlarmButton.draw();
                DoorBellButton.setSelected(0);
                DoorBellButton.draw();
                break;
            case 3:
                DoorBellButton.setSelected(1);
                DoorBellButton.draw();
                SirenButton.setSelected(0);
                SirenButton.draw();
                AlarmButton.setSelected(0);
                AlarmButton.draw();
                break;
            default:
                break;
        }
    } else if (section == VOLUME_SECTION) {
        switch (Selected) {
            case 1:
                LowVolume.setSelected(1);
                LowVolume.draw();
                MedVolume.setSelected(0);
                MedVolume.draw();
                HighVolume.setSelected(0);
                HighVolume.draw();
                break;
            case 2:
                MedVolume.setSelected(1);
                MedVolume.draw();
                LowVolume.setSelected(0);
                LowVolume.draw();
                HighVolume.setSelected(0);
                HighVolume.draw();
                break;
            case 3:
                HighVolume.setSelected(1);
                HighVolume.draw();
                MedVolume.setSelected(0);
                MedVolume.draw();
                LowVolume.setSelected(0);
                LowVolume.draw();
                break;
            default:
                break;
        }
    }
}

void AlmondTimer_onTouch(void) {
    MODE = KEYBOARD_MODE;
    change = CHANGE_ALMONDPIC_TIME;
    readArea(0, 0, 319, 239);
    kbMode = true;
    kb3->noCommit = true;
    kb3->keyboardMode(NUMBER_ONLY);

    if (!(wcscmp(As_opt, L"∞"))) {
        swprintf(As_opt, 179, L"%ls", L"0");
        strcpy(Time, "0");
    } else {
        sprintf(Time, "%ls", As_opt);
    }
    wcscpy(N_opt, As_opt);
    kb3->drawText(Time, As_opt);
}


void AlmondLedTimer_onTouch(void) {
    MODE = KEYBOARD_MODE;
    change = CHANGE_ALMONDBLINK_TIME;
    readArea(0, 0, 319, 239);
    kbMode =true;
    kb3->noCommit = true;
    kb3->keyboardMode(NUMBER_ONLY);

    if (!(wcscmp(As_opt, L"∞"))) {
        swprintf(As_opt, 179, L"%ls", L"0");
        strcpy(Time, "0");
    } else {
        sprintf(Time, "%ls", As_opt);
    }
    wcscpy(N_opt, As_opt);
    kb3->drawText(Time, As_opt);
}

void drawAlmondBlinkColorStrip(void) {
    double bright = 0.0;
    MODE = BLINK_COLOR_MODE;
    readArea(0, 0, 319, 239);
    drawRoundedRectangle(10, 95, 310, 230, RR_COLOR, 255);
    fillRect(130, 95, 190, 135, RR_COLOR);

    convertRGBtoHSV(presentColor, &bright, NULL);
    swprintf(opt, 179, L"%0.0f %ls", (bright * 100), L"%");
    drawText(opt, 130, 95, 60, 1, 0, 25.0f, 65535, 1);
    pngImg.drawPNG(25, 145, 1, 0, color_shades);
}

unsigned int findNewShadeOfColor(unsigned short int color, int changed) {
//    unsigned short int hue = 0;
    unsigned int ledColor = 0;
    double bright = 0.0; //, satur = 0.0;
    Device d(contDevID);
    //    int R = 0, G = 0, B = 0;
    //    sprintf(t_temp, "Color on touch:Previous R:%d G:%d B:%d", getR(color), getG(color), getB(color));
    //    LOG(Logger::INFO, t_temp);

    //    R = (getR(color)-((getR(color)*((255 - changed)) / 255)));
    //    G = (getG(color)-((getG(color)*((255 - changed)) / 255)));
    //    B = (getB(color)-((getB(color)*((255 - changed)) / 255)));
    //    ledColor = (((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3));
    //    convertRGBtoHSV(color, NULL, &satur);
    bright = (changed / 255.0);
    ledColor = convertHSVtoRGB(Ledhue, LedSatur, bright);
    //    ledColor = (ledColor |( R << 16) |(G << 8)|(B));
    //    color = (((((ledColor & 0xff0000)>>16)&0xF8)<<8)|((((ledColor & 0x00ff00)>>8)&0xFC)<<3)|(((ledColor & 0x0000ff)>>3)));
    if (bright != 0) {
        connectedHomeSetValue(3, ledColor);
    }
    sprintf(t_temp, "Color on touch:bright:%lf satur:%lf position:%d", bright, LedSatur, changed);
    LOG(Logger::INFO, t_temp);
    return ledColor;
}

/*unsigned short int getFullHue(unsigned short int changedCol) {
    return convertHSVtoRGB(convertRGBtoHSV(changedCol, NULL, NULL), 1.0, 1.0);
}*/

void SetConfigurationButton_onTouch(void) {//newal2
    Device dev(contDevID);
    char value[30] = "";
    if (ValueFlag && ValueSizeFlag) {
        if(dev.getDeviceType() == ZWtoACIRExtender) {
            sprintf(value, "%s,%d,%s", ConfigParam, tempvaluesize, ConfigValue);
            dev.setValue(11, value);
        } else {
            SetConfigParam(atoi(ConfigParam), tempvaluesize, atoi(ConfigValue));
        }
        ParameterFlag = false;
        ValueFlag = false;
        ValueSizeFlag = false;
    } else if (IRCodeFlag) {
        dev.setValue(10, tmpc );
    }
}

void IRCodeBox_onTouch(void) {//newgeneric
    readArea(0, 0, 319, 239);
    MODE = KEYBOARD_MODE;
    kb5->LT = 3;
    kb5->GT = 1;
    kb5->noCommit = true;
    kb5->keyboardMode(NUMBER_ONLY);
    wmemset(N_opt, 0, 180);
    kb5->drawText(tmpc, N_opt);
}

void applianceButton_OnTouch(void) {
    MODE = IR_OPERATION_MODE;
    char buff[100] = "";
    Device dev(contDevID);
    strcpy(buff, dev.getValue(irApplianceSelected + 8));
    //    sscanf(buff, "%d,%d,%d,%d,%d,%d,%d,%d", &buttonType[0], &buttonType[1], &buttonType[2], &buttonType[3],
    //                                            &buttonType[4], &buttonType[5], &buttonType[6], &buttonType[7]);
    //    
    //    sprintf(t_temp, "%d %d %d %d %d %d %d %d", buttonType[0], buttonType[1], buttonType[2], buttonType[3],
    //                                               buttonType[4], buttonType[5], buttonType[6], buttonType[7]);
    //    
    //    LOG(Logger::INFO, t_temp);
    readArea(0, 0, 319, 239);
    drawRoundedRectangle(10, 95, 310, 230, RR_COLOR, 255);
    setAndDrawIrEventButton(buff);
    ChangeButtonName.draw();

    swprintf(opt, 179, L"%s's functions", dev.getValue(irApplianceSelected + 1));
    drawText(opt, 10, 170, 300, 1, 0, 18.0f, 65535, 1);
    if (irApplianceSelected < 6) {
        SelectIRLed.draw();
    }

}

void applianceEventButton_OnTouch(void) {
    char data[10] = {0};
    sprintf(data, "%d%d", irApplianceSelected, irAppButtonSelected);
    connectedHomeSetValue(1, data);
    irAppButtonSelected = 0;
}

void SelectIRLed_OnTouch(void) {
    MODE = IR_LED_SELECTION_MODE;
    unsigned char selectedLed = 0;
    char data[20] = "", temp[5] = "0x";
    Device dev(contDevID);

    readArea(0, 0, 319, 239);
    drawRoundedRectangle(10, 95, 310, 230, RR_COLOR, 255);
    strcpy(data, dev.getValue(8));
    for (int i = 0; i < 5; i++) {
        strncpy(temp + 2, data + ((i + 1)*2), sizeof (char)*2);
        appled[i] = (unsigned char) strtol(temp, NULL, 16);
    }

    strncpy(temp + 2, data + (irApplianceSelected * 2), sizeof (char)*2);
    selectedLed = (unsigned char) strtol(temp, NULL, 16);

    //    sprintf(t_temp, "ZBIRExtender Led %s selected : %d", temp, selectedLed);
    //    LOG(Logger::INFO, t_temp);

    IRLed1 = new Widget(type.CheckBox, 70, 170, 25, 25, 0, L"1");
    IRLed1->onTouch = &IRLed_OnTouch;
    if (selectedLed & 0x01) {
        appled[irApplianceSelected - 1] |= 0x01;
        IRLed1->setSelected(1);
    }
    IRLed1->draw();

    IRLed2 = new Widget(type.CheckBox, 40, 132, 25, 25, 0, L"2");
    IRLed2->onTouch = &IRLed_OnTouch;
    if (selectedLed & 0x02) {
        appled[irApplianceSelected - 1] |= 0x02;
        IRLed2->setSelected(1);
    }
    IRLed2->draw();

    IRLed3 = new Widget(type.CheckBox, 100, 95, 25, 25, 0, L"3");
    IRLed3->onTouch = &IRLed_OnTouch;
    if (selectedLed & 0x04) {
        appled[irApplianceSelected - 1] |= 0x04;
        IRLed3->setSelected(1);
    }
    IRLed3->draw();

    IRLed4 = new Widget(type.CheckBox, 160, 132, 25, 25, 0, L"4");
    IRLed4->onTouch = &IRLed_OnTouch;
    if (selectedLed & 0x08) {
        appled[irApplianceSelected - 1] |= 0x08;
        IRLed4->setSelected(1);
    }
    IRLed4->draw();

    IRLed5 = new Widget(type.CheckBox, 130, 170, 25, 25, 0, L"5");
    IRLed5->onTouch = &IRLed_OnTouch;
    if (selectedLed & 0x10) {
        appled[irApplianceSelected - 1] |= 0x10;
        IRLed5->setSelected(1);
    }
    IRLed5->draw();

    IRLed6 = new Widget(type.CheckBox, 100, 132, 25, 25, 0, L"6");
    IRLed6->onTouch = &IRLed_OnTouch;
    if (selectedLed & 0x20) {
        appled[irApplianceSelected - 1] |= 0x20;
        IRLed6->setSelected(1);
    }
    IRLed6->draw();
    swprintf(opt, 179, L"Select %s's LED", dev.getValue(irApplianceSelected + 1));
    drawText(opt, 20, 202, 179, 1, 0, 18.0f, 65535, 1);
    selectIRLedDone.draw();
}

void ChangeButtonName_OnTouch(void) {
    MODE = EDIT_BUTTON_NAME;
    preMode = EDIT_BUTTON_NAME;
    Modal mW;
    fillRect(10, 95, 305, 225, RR_COLOR);
    fillRect(287, 57, 307, 77, RR_COLOR);

    AppButtonOne.draw();
    AppButtonTwo.draw();
    AppButtonThree.draw();
    AppButtonFour.draw();
    AppButtonFive.draw();
    AppButtonSix.draw();
    AppButtonSeven.draw();
    AppButtonEight.draw();
    selectIRLedDone.draw();

    readArea(0, 0, 319, 239);
    mW.setLine1(L"Tap on the button");
    mW.setLine2(L"you wish to change");
    mW.setLine3(L"the tag.");
    mW.modalReturn = &drawArea;
    mW.draw(MODAL_TYPE_INFORMATION, 2);
}

void IRLed_OnTouch(void) {

    if (irAppLedSelected == 1) {
        appled[irApplianceSelected - 1] ^= 0x01;
    } else if (irAppLedSelected == 2) {
        appled[irApplianceSelected - 1] ^= 0x02;
    } else if (irAppLedSelected == 3) {
        appled[irApplianceSelected - 1] ^= 0x04;
    } else if (irAppLedSelected == 4) {
        appled[irApplianceSelected - 1] ^= 0x08;
    } else if (irAppLedSelected == 5) {
        appled[irApplianceSelected - 1] ^= 0x10;
    } else if (irAppLedSelected == 6) {
        appled[irApplianceSelected - 1] ^= 0x20;
    }
    irAppLedSelected = 0;

    sprintf(t_temp, "ZBIRExtender Led selected : 0x%x", appled[irApplianceSelected - 1]);
    LOG(Logger::INFO, t_temp);
}

void selectIRLedDone_OnTouch(void) {
    Device dev(contDevID);
    if (MODE == EDIT_APPLIANCE_NAME) {
        drawControlDevice(contDevID);
        MODE = CONTROL_MODE;
    } else if (MODE == EDIT_BUTTON_NAME) {
        MODE = IR_OPERATION_MODE;
        pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossdefault);
        AppButtonOne.draw();
        AppButtonTwo.draw();
        AppButtonThree.draw();
        AppButtonFour.draw();
        AppButtonFive.draw();
        AppButtonSix.draw();
        AppButtonSeven.draw();
        AppButtonEight.draw();
        ChangeButtonName.draw();
   swprintf(opt, 179, L"%s's functions", dev.getValue(irApplianceSelected + 1));
    drawText(opt, 10, 170, 300, 1, 0, 18.0f, 65535, 1);
        delete Irlst;
        free(IREventData);
        if (irApplianceSelected < 6) {
            SelectIRLed.draw();
        } else {
            fillRect(220, 190, 300, 220, RR_COLOR);
        }
    } else {
        MODE = IR_OPERATION_MODE;
        dev.setDeviceAttribute(0x0201, 0x1011, &appled);
        delete IRLed1, IRLed2, IRLed3;
        delete IRLed4, IRLed5, IRLed6;
        drawArea();
    }
}

void ChangeApplianceName_OnTouch(void) {
    Modal mW;
    MODE = EDIT_APPLIANCE_NAME;
    fillRect(10, 95, 305, 225, RR_COLOR);
    fillRect(287, 57, 307, 77, RR_COLOR);

        ApplianceOne.draw();
        ApplianceTwo.draw();
        ApplianceThree.draw();
        ApplianceFour.draw();
        ApplianceFive.draw();
        ApplianceSix.draw();

    selectIRLedDone.draw();
    readArea(0, 0, 319, 239);
    mW.setLine1(L"Tap on the button");
    mW.setLine2(L"you wish to change");
    mW.setLine3(L"the name.");
    mW.modalReturn = &drawArea;
    mW.draw(MODAL_TYPE_INFORMATION, 2);

}

void EditApplianceNameOnTouch(int appNo) {
    MODE = KEYBOARD_MODE;
    char text[50] = "";
    if (appNo == 1) {
        irAppButton = 1;
        sprintf(text, "%ls", ApplianceOne.W.text);
    } else if (appNo == 2) {
        irAppButton = 2;
        sprintf(text, "%ls", ApplianceTwo.W.text);
    } else if (appNo == 3) {
        irAppButton = 3;
        sprintf(text, "%ls", ApplianceThree.W.text);
    } else if (appNo == 4) {
        irAppButton = 4;
        sprintf(text, "%ls", ApplianceFour.W.text);
    } else if (appNo == 5) {
        irAppButton = 5;
        sprintf(text, "%ls", ApplianceFive.W.text);
    } else if (appNo == 6) {
        irAppButton = 6;
        sprintf(text, "%ls", ApplianceSix.W.text);
    }
    readArea(0, 0, 319, 239);
    kbMode = true;
    kb9->LT = 11;
    kb9->GT = 1;
    kb9->keyboardMode(ALPHANUMERIC);
    kb9->drawText(text, As_opt);
}

void IRAppNameKbReturn(void) {
    char temp[50] = "";
    drawArea();
    MODE = EDIT_APPLIANCE_NAME;
    kbMode = false;
    sprintf(temp, "%ls", As_opt);
    Device dev(contDevID);
    dev.setIndexValue(irAppButton + 1, "SetIndexName", temp, ZigBeeTechnology);

    if (irAppButton == 1) {
        ApplianceOne.setText(As_opt);
        ApplianceOne.draw();
    } else if (irAppButton == 2) {
        ApplianceTwo.setText(As_opt);
        ApplianceTwo.draw();
    } else if (irAppButton == 3) {
        ApplianceThree.setText(As_opt);
        ApplianceThree.draw();
    } else if (irAppButton == 4) {
        ApplianceFour.setText(As_opt);
        ApplianceFour.draw();
    } else if (irAppButton == 5) {
        ApplianceFive.setText(As_opt);
        ApplianceFive.draw();
    } else if (irAppButton == 6) {
        ApplianceSix.setText(As_opt);
        ApplianceSix.draw();
    }
}

void EditIRButtonEventNameOnTouch(int button) {
    creatAndInitList();
    if (button == 1) {
        irAppButton = 1;
    } else if (button == 2) {
        irAppButton = 2;
    } else if (button == 3) {
        irAppButton = 3;
    } else if (button == 4) {
        irAppButton = 4;
    } else if (button == 5) {
        irAppButton = 5;
    } else if (button == 6) {
        irAppButton = 6;
    } else if (button == 7) {
        irAppButton = 7;
    } else if (button == 8) {
        irAppButton = 8;
    }

}

void creatAndInitList(void) {
    MODE = ALMOND_LIST_MODE;
    char temp[30] = "";
    readArea(0, 0, 319, 239);
    fillRect(10, 95, 305, 225, RR_COLOR);

    IREventData = (siteInfo*) calloc(30, sizeof (siteInfo));
    for (int i = 0; i < 26; i++) {
        IREventsEnumToStr((enum IREvents)i, temp);
        strcpy(IREventData[i].ssid, temp);
    }

    strcpy(temp, "IR Events");
    Irlst = new almondList(10, 52, 305, 26, 5, 26, IREventData);
    Irlst->onTouch = &Irlist_onTouch;
    Irlst->title = temp;
    Irlst->fontSize = 18.0;
    Irlst->isBand = false;
    Irlst->isBars = false;
    Irlst->currentIndex = 0;
    Irlst->xLeftMargin = 0;
    Irlst->headerHeight = 22;
    Irlst->imageWidth = 0;
    Irlst->drawImages = false;
    Irlst->columns = 3;
    Irlst->draw();

    pngImg.drawPNG(5, 5, 1, 0, backButtonImg);
}

void Irlist_onTouch(void) {
    char buff[100] = "", temp[50] = "";
    int buttonType[10];
    Device dev(contDevID);
    MODE = EDIT_BUTTON_NAME;
    drawArea();

    strcpy(buff, dev.getValue(irApplianceSelected + 8));
    sscanf(buff, "%d,%d,%d,%d,%d,%d,%d,%d", &buttonType[0], &buttonType[1], &buttonType[2], &buttonType[3],
            &buttonType[4], &buttonType[5], &buttonType[6], &buttonType[7]);
    IREventsEnumToStr((enum IREvents)Irlst->currentIndex, temp);
    swprintf(opt, 179, L"%s", temp);
    if (irAppButton == 1) {
        buttonType[0] = Irlst->currentIndex;
        AppButtonOne.setText(opt);
        AppButtonOne.draw();
    } else if (irAppButton == 2) {
        buttonType[1] = Irlst->currentIndex;
        AppButtonTwo.setText(opt);
        AppButtonTwo.draw();
    } else if (irAppButton == 3) {
        buttonType[2] = Irlst->currentIndex;
        AppButtonThree.setText(opt);
        AppButtonThree.draw();
    } else if (irAppButton == 4) {
        buttonType[3] = Irlst->currentIndex;
        AppButtonFour.setText(opt);
        AppButtonFour.draw();
    } else if (irAppButton == 5) {
        buttonType[4] = Irlst->currentIndex;
        AppButtonFive.setText(opt);
        AppButtonFive.draw();
    } else if (irAppButton == 6) {
        buttonType[5] = Irlst->currentIndex;
        AppButtonSix.setText(opt);
        AppButtonSix.draw();
    } else if (irAppButton == 7) {
        buttonType[6] = Irlst->currentIndex;
        AppButtonSeven.setText(opt);
        AppButtonSeven.draw();
    } else if (irAppButton == 8) {
        buttonType[7] = Irlst->currentIndex;
        AppButtonEight.setText(opt);
        AppButtonEight.draw();
    }
    sprintf(buff, "%d,%d,%d,%d,%d,%d,%d,%d", buttonType[0], buttonType[1], buttonType[2], buttonType[3],
            buttonType[4], buttonType[5], buttonType[6], buttonType[7]);
    dev.setIndexValue(irApplianceSelected + 8, "SetIndexName", buff, ZigBeeTechnology);
}

void setAndDrawIrEventButton(char *buttondata) {
    int buttonType[10];
    char temp[30] = "";
    sscanf(buttondata, "%d,%d,%d,%d,%d,%d,%d,%d", &buttonType[0], &buttonType[1], &buttonType[2], &buttonType[3],
            &buttonType[4], &buttonType[5], &buttonType[6], &buttonType[7]);
    for (int i = 0; i < 8; i++) {
        IREventsEnumToStr((enum IREvents)buttonType[i], temp);
        swprintf(opt, 179, L"%s", temp);
        if (i == 0) {
            AppButtonOne.setText(opt);
            AppButtonOne.draw();
        } else if (i == 1) {
            AppButtonTwo.setText(opt);
            AppButtonTwo.draw();
        } else if (i == 2) {
            AppButtonThree.setText(opt);
            AppButtonThree.draw();
        } else if (i == 3) {
            AppButtonFour.setText(opt);
            AppButtonFour.draw();
        } else if (i == 4) {
            AppButtonFive.setText(opt);
            AppButtonFive.draw();
        } else if (i == 5) {
            AppButtonSix.setText(opt);
            AppButtonSix.draw();
        } else if (i == 6) {
            AppButtonSeven.setText(opt);
            AppButtonSeven.draw();
        } else if (i == 7) {
            AppButtonEight.setText(opt);
            AppButtonEight.draw();
        }
    }
}

void IRCodeNumericKbReturn(void) {//newgeneric
    drawArea();
    MODE = DEVICE_MODE;
    sprintf(tmpc, "%ls", N_opt);
    IRCodeFlag = true;
    kbMode = false;
    Device *d = getDevice(contDevID);
    if (d && d->getDeviceType() == GenericDevice) {
        in = StartIndxCnt;
        GenericKeyBoardReturn();
    } else {
        swprintf(opt, 179, L"%s", tmpc);
        drawText(opt, 135, 100, 50, 1, 0, 24.0f, 0, 1); //135, 100, 50, 40
    }
}

void TempNumericKbReturn(void) {
    double tmpd;
    //setKeyboardValidationFunction(NULL);
    drawArea();
    MODE = CONTROL_MODE;
    //drawControlDevice(*getDevice(contDevID));
    sprintf(tmpc, "%ls", N_opt);
    tmpd = atof(tmpc);
    if (format == FORMAT_F) {
        tmpd = (tmpd *9)/5.0 + 32;
        tmpd = ceil(tmpd);
    }
//    setThermoHeatCurrLevel(tmpd);
    Device *d = getDevice(contDevID);//15dec--------------
    if (d) {
        if (!strcmp(d->getValue(5), "Heat Econ")) {
	    connectedHomeSetValue(11, tmpd);
            //d->setThermostatHeatingTemp(val);
        } else {
    connectedHomeSetValue(1, tmpd);
        }

    }
    kbMode = false;

    sprintf(t_temp, "setting Temperature :%.2lf for Device:%d MODE %d tempc %s", tmpd, contDevID, MODE, tmpc);
    LOG(Logger::PRINT, t_temp);

}

void GenericKeyBoardReturn(void) {//newgeneric
    int ValueCount = 0;
    char ss[180];
    Device *dev = getDevice(contDevID);
    bzero(&gen, sizeof (gen));
    bzero(grid1, sizeof (grid1));
    bzero(grid2, sizeof (grid2));
    bzero(grid3, sizeof (grid3));
    gen.x_next = X1_CORDT;
    gen.y_next = Y1_CORDT;
    gen.ROW1 = gen.ROW2 = gen.ROW3 = 1;
    fillRect(10, 90, 305, 225, RR_COLOR);
    ValueCount = dev->getValueCount();
    StartIndxCnt = in;

    drawRoundedRectangle(10, 55, 310, 230, RR_COLOR, 255);
    memset(opt, 0, sizeof (opt));
    utf8_to_wchar(dev->getDeviceName(), strlen(dev->getDeviceName()), opt, strlen(dev->getDeviceName()), UTF8_IGNORE_ERROR);
    drawText(opt, 32, 57, 253, 1, 0, 20.0f, 65535, 1);
    memset(opt, 0, sizeof (opt));
    sprintf(ss, "@ %hs", dev->getDeviceLocation());
    utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
    drawText(opt, 30, 77, 250, 1, 0, 16.0f, 65535, 1);
    memset(opt, 0, sizeof (opt));
    pngImg.drawPNG(287, 57, 1, BACKGROUND_LAYER, crossdefault);

    for (in; in <= ValueCount; in++) {
        if ((drawGenericDevice(contDevID, in, 0)) == FULL_PAGE)
            break;
    }
    bzero(&gen, sizeof (gen));
    EditButton.draw();
    if (StartIndxCnt == 1)
        MODE = CONTROL_MODE;
    else
        MODE = GENERIC_MODE;

}

void battery(int DevId) {
    Device d(DevId);
    if (strcmp(d.getValue(2), "true") == 0) {
        pngImg.drawPNG(90, NOTIFICATION_Y + 50, 1, BACKGROUND_LAYER, battery_c);
        drawText(lan.get("battery_low", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 50, 300, 1, 0, 20.0f, 65535, 1);
    } else {
        fillRect(NOTIFICATION_X, NOTIFICATION_Y + 50, 300, NOTIFICATION_Y + 80, RR_COLOR);
    }
}

bool kbVal(wchar_t * kbText) {
    char name[180], location[180];
    if (change == CHANGE_NAME) {
        wchar_to_utf8(kbText, wcslen(kbText), name, wcslen(kbText), UTF8_IGNORE_ERROR);
        name[wcslen(kbText)] = '\0';
        NameBox->setText(kbText);
        strcpy(DevName, name);
    } else if (change == CHANGE_LOCATION) {
        wchar_to_utf8(kbText, wcslen(kbText), location, wcslen(kbText), UTF8_IGNORE_ERROR);
        location[wcslen(kbText)] = '\0';
        LocationBox->setText(kbText);
        strcpy(DevLocation, location);
    }
    return true;
}

void locationList_onTouch(void) {
    char location[16][32] = {
                                {"Bedroom"},
                                {"Den"},
                                {"Dining Room"},
                                {"Downstairs"},
                                {"Entryway"},
                                {"Family Room"},
                                {"Hallway"},
                                {"Kids Room"},
                                {"Kitchen"},
                                {"Living Room"},
                                {"Master Bedroom"},
                                {"Office"},
                                {"Upstairs"},
                                {"<custom>"}
                            };

    MODE = KEY_MODE;

    if (Irlst->currentIndex == 13) {
        change = CHANGE_LOCATION;
        //        setKeyboardValidationFunction(CustomTextValidation);
    keyBoardSaveMode(false);
        mem.addValue("tempLoc", DevLocation);
    kbMode = true;
    kb1->LT = 32;
    kb1->GT = 1;
    kb1->keyboardMode(ALPHANUMERIC);
    kb1->draw("tempLoc");
    } else {
        drawArea();
        strcpy(DevLocation, location[Irlst->currentIndex]);
        swprintf(opt, 179, L"%s", location[Irlst->currentIndex]);
        LocationBox->setText(opt);
        LocationBox->draw();
    }
    free(deviceLocation);
    delete Irlst;
}

void creatAndInitLocationList(void) {
    MODE = ALMOND_LIST_MODE;
    char temp[32], location[16][32] = {
                                        {"Bedroom"},
                                        {"Den"},
                                        {"Dining Room"},
                                        {"Downstairs"},
                                        {"Entryway"},
                                        {"Family Room"},
                                        {"Hallway"},
                                        {"Kids Room"},
                                        {"Kitchen"},
                                        {"Living Room"},
                                        {"Master Bedroom"},
                                        {"Office"},
                                        {"Upstairs"},
                                        {"<custom>"}
                                    };
    readArea(0, 0, 319, 239);
    fillRect(10, 50, 305, 225, RR_COLOR);

    deviceLocation = (siteInfo*) calloc(16, sizeof (siteInfo));
    for (int i = 0; i < 15; i++) {
        strcpy(deviceLocation[i].ssid, location[i]);
    }

    strcpy(temp, "Locations");
    Irlst = new almondList(10, 52, 300, 26, 5, 14, deviceLocation);
    Irlst->onTouch = &locationList_onTouch;
    Irlst->title = temp;
    Irlst->fontSize = 18.0;
    Irlst->isBand = false;
    Irlst->isBars = false;
    Irlst->currentIndex = 0;
    Irlst->xLeftMargin = 0;
    Irlst->headerHeight = 22;
    Irlst->imageWidth = 0;
    Irlst->drawImages = false;
    Irlst->columns = 2;
    Irlst->draw();
    pngImg.drawPNG(5, 5, 1, 0, backButtonImg);
}

/*bool CustomTextValidation(wchar_t* keyboardText) {
    wchar_t * pwc;
    pwc = wcschr(keyboardText, L'|');
        Modal mW;
    if (pwc) {
        mW.setLine1(L"Location should not");
        mW.setLine2(L"contain '|' special ");
        mW.setLine3(L"character.");
        mW.modalReturn = &doNothing;
        mW.draw(MODAL_TYPE_INFORMATION, -1);
        return false;
    } else if (wcscmp(keyboardText, L"None") == 0) {
        mW.setLine1(L"Location should not");
        mW.setLine2(L"contain \"None\" as");
        mW.setLine3(L"location name.");
        mW.modalReturn = &doNothing;
        mW.draw(MODAL_TYPE_INFORMATION, -1);
        return false;
    } else
        return true;
}*/

void NameOnTouch(void) {
    change = CHANGE_NAME;
    Memory mem;
    keyBoardSaveMode(false);
    mem.saveSetting("tempLoc", DevName);
    kbMode = true;
    kb1->LT = 32;
    kb1->GT = 1;
    kb1->keyboardMode(ALPHANUMERIC);
    kb1->draw("tempLoc");
}

void LocationBoxOnTouch(void) {
    creatAndInitLocationList();
}

void kbReturn(void) {
    if (En_Key) {
        Device *d = getDevice(contDevID);
        drawArea();
        fillRect(10, 92, 305, 225, RR_COLOR);
        swprintf(opt, 179, L"%ls:", lan.get("name", "add_sensor"));
        drawText(opt, 44, 107, 240, 1, 0, 16.0f, 65535, 0);
        NameBox->draw();
        swprintf(opt, 179, L"%ls:", lan.get("location", "add_sensor"));
        drawText(opt, 44, 142, 240, 1, 0, 16.0f, 65535, 0);
        LocationBox->draw();
        DoneButton->draw();
        RemovedeviceButton.draw();
        if ((d->getDeviceTechnology() == ZWaveTechnology) && (d->getDeviceType() != ZWtoACIRExtender)) {//confignewlyadded
            ConfigurationButton1.draw();
        }
    }
    kbMode = false;
}

void NumericKbReturn(void) {
    char temp[30] = {0};
    int size = 0;
    // setKeyboardValidationFunction(NULL);
    drawArea();
    MODE = CONTROL_MODE;
    sprintf(temp, "%ls", N_opt);
    if (change == CHANGE_PIN) {
        Device *d = getDevice(contDevID);
        if (d) {
            if (strcmp(Userpin, temp) != 0) {
                sprintf(Userpin, "%ls", N_opt);
                if(d->getDeviceTechnology() == ZWaveTechnology) {
                    d->setValue(Userno + 4, Userpin);
                } else {
                    d->setValue(Userno + 3, Userpin);
                }
            }
            fillRect(105, 160, 225, 185, RR_COLOR);
            if (d->getDeviceTechnology() == ZWaveTechnology) {
                aswprintf(opt, 179, L"%s", Userpin);
                drawText(opt, 105, 160, 120, 1, 0, 25.0f, 65535, 0);
            } else
                drawText(L"********", 105, 160, 120, 1, 0, 25.0f, 65535, 0);
        } else {
            char bufLog[200] = "";
            sprintf(bufLog, "Device %d doesn't exists in list to check", contDevID);
            LOG(Logger::ERROR, bufLog);
        }
    } else if (change == CHANGE_TIME) {
        sprintf(Time, "%ls", N_opt);
        WDTime->setText(N_opt);
        Sirenon_onTouch1(atoi(Time));
    } else if (change == CHANGE_TIME_GENERIC) {
        sprintf(Time, "%ls", N_opt);
        TimeBox->setText(N_opt);
        Sirenon_onTouch1(atoi(Time));
    } else if (change == CHANGE_PIN_GENERIC) {
        sprintf(Userpin, "%ls", N_opt);
        Device *d = getDevice(contDevID);
        if (d) {
            int userind;
            in = StartIndxCnt;
//            d->setUserCode(Userpin, Userno);
            userind = d->getIndexNoFromName("USER_CODE", -1);
            connectedHomeSetValue(userind + Userno - 1, Userpin);
            GenericKeyBoardReturn();
        }
    } else if ((change == CHANGE_ALMONDPIC_TIME) || (change == CHANGE_ALMONDBLINK_TIME)) {
        sprintf(Time, "%ls", As_opt);
        if (atoi(Time) > 99999) {
            swprintf(As_opt, 179, L"%ls", N_opt);
            sprintf(Time, "%ls", As_opt);
            Modal mW;
            readArea(0, 0, 319, 239);
            mW.setLine1(L"Please enter");
            mW.setLine2(L"number between");
            mW.setLine3(L"0 - 99999");
            mW.modalReturn = &drawArea;
            mW.draw(MODAL_TYPE_INFORMATION, 3);
        } else {
            Device d(contDevID);
            if (change == CHANGE_ALMONDPIC_TIME) {
                d.setValue(5, Time);
            } else if (change == CHANGE_ALMONDBLINK_TIME) {
                d.setValue(4, Time);
            }
            
            while (Time[0] == '0') {
                size++;
                memmove(Time, Time + 1, sizeof (Time) - size);
            }
            swprintf(As_opt, 179, L"%s", Time);
            sprintf(t_temp, "Set Almond siren/blink duration:%s", Time);
            LOG(Logger::INFO, t_temp);

            if (atoi(Time) == 0) {
                Modal mW;
                readArea(0, 0, 319, 239);
                if (change == CHANGE_ALMONDPIC_TIME) {
                    mW.setLine1(L"You chose to ring");
                    mW.setLine2(L"siren for indefinite");
                } else if (change == CHANGE_ALMONDBLINK_TIME) {
                    mW.setLine1(L"You chose to blink");
                    mW.setLine2(L"Led for indefinite");
                }
                mW.setLine3(L"duration");
                mW.modalReturn = &drawArea;
                mW.draw(MODAL_TYPE_INFORMATION, 3);
            }
        }
        if (atoi(Time) == 0) {
            swprintf(As_opt, 179, L"%ls", L"∞");
        }

        if (change == CHANGE_ALMONDPIC_TIME) {
            AlmondTimer.setText(As_opt);
        } else if (change == CHANGE_ALMONDBLINK_TIME) {
            AlmondLedTimer.setText(As_opt);
        }
    }
    kbMode = false;
}

void EditNameandLocation(void) {
    MODE = KEY_MODE;
    preMode = KEY_MODE;
    wchar_t opt[30];
    Device d(contDevID);
    fillRect(10, 92, 305, 225, RR_COLOR);
    swprintf(opt, 179, L"%ls:", lan.get("name", "add_sensor"));
    drawText(opt, 44, 107, 240, 1, 0, 16.0f, 65535, 0);
    strcpy(DevName, d.getDeviceName());
    swprintf(opt, 29, L"%hs", DevName);
    NameBox->setText(opt);
    NameBox->draw();
    swprintf(opt, 179, L"%ls:", lan.get("location", "add_sensor"));
    drawText(opt, 44, 142, 240, 1, 0, 16.0f, 65535, 0);
    strcpy(DevLocation, d.getDeviceLocation());
    swprintf(opt, 29, L"%hs", DevLocation);
    LocationBox->setText(opt);
    LocationBox->draw();
    DoneButton->draw();
    En_Key = true;
    RemovedeviceButton.draw();
    sprintf(t_temp, "Editing name and location of Device %d ", contDevID);
    LOG(Logger::INFO, t_temp);
}

void DoneButtonOnTouch(void) {
    MODE = CONTROL_MODE;
    Device dev(contDevID);
    if(strcmp(dev.getDeviceName(), DevName) != 0) {
        dev.setName(DevName);
    }

    if(strcmp(dev.getDeviceLocation(), DevLocation) != 0) {
        dev.setLocation(DevLocation);
}

//void setbulbsaturation(int val) {
//    char value[10] = "";
//    sprintf(value, "%d", val);
//    Device *d = getDevice(contDevID);
//    if (d) {
//        d->setValue(4, value);
//        sprintf(t_temp, "saturation value is set to %d %x", val, val);
//        LOG(Logger::INFO, t_temp);
//    }
//}

//void setbulbhue(int val) {
//    char value[10] = "";
//    sprintf(value, "%d", val);
//    Device *d = getDevice(contDevID);
//    if (d) {
//        d->setValue(3, value);
//        sprintf(t_temp, "bulb hue value is set to %d %x", val, val);
//        LOG(Logger::INFO, t_temp);
//    }
//}
    drawControlDevice(dev);
    En_Key = false;
}

void setbulbhueandsat(int hue, int sat) {
    Device *d = getDevice(contDevID);
    if (d) {
        d->setHueandSat(hue, sat);
        sprintf(t_temp, "saturation and hue of bulb  is set to %d %d", hue, sat);
        LOG(Logger::INFO, t_temp);
    }
}

//void setbulbcolortemperature(int val) {
//    char value[10] = "";
//    sprintf(value, "%d", val);
//    try {
//        Device d(contDevID);
//        sprintf(t_temp, "color temp val to set %d - %d and %x\n", val, 1000000 / val, 1000000 / val);
//        LOG(Logger::INFO, t_temp);
//        d.setValue(5, value);
//    } catch (int exeption) {
//        return;
//    }
//}

/*void multilevelGoToLevel(int level) {
    try {
        Device d(contDevID);
        d.setSwitchLevel(level);
        sprintf(t_temp, "Device %d is set to level %d ", contDevID, level);
        LOG(Logger::INFO, t_temp);
    } catch (int exeption) {
        //printf("No device found \n");
        return;
    }
}*/

void connectedHomeSetValue(unsigned int index, char *value) {
    try {
        Device d(contDevID);
        d.setValue(index, value);
    } catch (int exeption) {
        return;
    }
    sprintf(t_temp, "Device %d index %u is set to %s value", contDevID, index, value);
    LOG(Logger::INFO, t_temp);   
}

void connectedHomeSetValue(unsigned int index, int value) {
    char charValue[50] = "";
    Device d(contDevID);
    sprintf(charValue, "%d" , value);
    try {
        d.setValue(index, charValue);
    } catch (int exeption) {
        return;
    }
    sprintf(t_temp, "Device %d index %u is set to %s value", contDevID, index, charValue);
    LOG(Logger::INFO, t_temp);
}
        
void connectedHomeSetValue(unsigned int index, double value) {
    char charValue[50] = "";
    Device d(contDevID);
    sprintf(charValue, "%lf", value);
    try {
        d.setValue(index, charValue);
    } catch (int exeption) {
        return;
    }
    sprintf(t_temp, "Device %d index %u is set to %s value", contDevID, index, charValue);
    LOG(Logger::INFO, t_temp);
}

void convertTemperature(int index, wchar_t *opt, bool touch) {
    float temperat = 0;
    char temp[50] = "";
    Device d(contDevID);
    Almond tempFormat;
    char value[20] = "";
    sprintf(value, "%s", d.getValue(index));
    if (value[0] == '\0') {
        sprintf(temp, "--");
    }
    else if (touch == true) {
        format = (format == FORMAT_C) ? FORMAT_F : FORMAT_C;
        if (format == FORMAT_F) {
            tempFormat.setTempFormat("F");
            sprintf(temp, "%s°F", (d.getValue(index)));
        } else {
            tempFormat.setTempFormat("C");
            temperat = atof(d.getValue(index));
            temperat = (((temperat - 32)*5) / 9);
            sprintf(temp, "%0.1f°C", temperat);
        }
        valueToSave = 1;
    } else {
        if (format == FORMAT_F) {
            sprintf(temp, "%s°F", d.getValue(index));
        } else {
            temperat = atof(d.getValue(index));
            temperat = (((temperat - 32)*5) / 9);
            sprintf(temp, "%0.1f°C", temperat);
        }
    }
    utf8_to_wchar(temp, strlen(temp), opt, strlen(temp), UTF8_IGNORE_ERROR);
    sprintf(t_temp, "Device %d index %u temperature is %s ", contDevID, index, temp);
    LOG(Logger::INFO, t_temp);
}

/*void multilevelGoToLevelOnOff(int level) {
    try {
        Device d(contDevID);
        d.setSwitchLevel(level);
        sprintf(t_temp, "Device %d is set to level: %d ", contDevID, level);
        LOG(Logger::INFO, t_temp);
    } catch (int exeption) {
        //printf("No device found \n");
        return;
    }
}*/

void multilevelOn_onTouch(void) {
    try {
        Device d(contDevID);
        if (d.getDeviceTechnology() == ZWaveTechnology) {
            d.setValue(1, "100");
        } else if ((d.getDeviceTechnology() == ZigBeeTechnology) || (d.getDeviceTechnology() == HueTechnology)) {
            d.setValue(2, "true");
        }
    } catch (int exeption) {
        return;
    }
    sprintf(t_temp, "Setting Multilevel Switch  %d on ", contDevID);
    LOG(Logger::INFO, t_temp);
}

void multilevelOff_onTouch(void) {
    try {
        Device d(contDevID);
        if (d.getDeviceTechnology() == ZWaveTechnology) {
            d.setValue(1, "0");
        } else if ((d.getDeviceTechnology() == ZigBeeTechnology) || (d.getDeviceTechnology() == HueTechnology)) {
            d.setValue(2, "false");
        }
    } catch (int exeption) {
        return;
    }
    sprintf(t_temp, "Setting Multilevel Switch  %d off ", contDevID);
    LOG(Logger::INFO, t_temp);
}

void settings_onTouch(void) {
    MODE = COLOR_MODE;
    int s_val, h_val;
    LOG(Logger::ERROR, "settings_onTouch");
    Device d(contDevID);
    if (d.getDeviceTechnology() == ZigBeeTechnology) {
        s_val = atoi(d.getValue(4));
        h_val = atoi(d.getValue(3));
        fillRect(10, 90, 310, 230, RR_COLOR);
        percentage = (int) round(((s_val * 100) / 254.0));
        swprintf(opt, 179, L"Sat - %d%%", percentage);
        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 15, 300, 1, 0, 18.0f, 65535, 1);

        fillRect(25, RECT_Y, 300, RECT_Y + RECT_HEIGHT + 5, RGB(23, 191, 227));
        drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2) + 2, 301 - 5, RECT_Y + (RECT_HEIGHT / 2) + 2, 65535);
        drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 3) + 1, 25 + 5, RECT_Y + ((RECT_HEIGHT * 2) / 3) + 1, 65535);
        drawLine(301 - 5, RECT_Y + (RECT_HEIGHT / 3), 301 - 5, RECT_Y + ((RECT_HEIGHT * 2) / 3) + 1, 65535);
        fillCircle(35 + s_val, RECT_Y + (RECT_HEIGHT / 2) + 2, 5, 65535);
        //    drawText(lan.get("sat","connected_sensor"), 20, RECT_Y + (RECT_HEIGHT / 2) - 5, 50, 1, 0, 18.0f, 65535, 1);

        drawText(L"Hue", NOTIFICATION_X, RECT_Y + (RECT_HEIGHT / 2) + 30, 300, 1, 0, 18.0f, 65535, 1);
        h_val = h_val + 25;
        pngImg.drawPNG(25, RECT_Y + (RECT_HEIGHT / 2) + 55, 1, 0, color_png);
        fillRect(280, 181, 300, 221, RGB(255, 255, 255));
        drawLine(h_val, RECT_Y + (RECT_HEIGHT / 2) + 55, h_val, RECT_Y + (RECT_HEIGHT / 2) + 95, 0);
    } else if (d.getDeviceTechnology() == HueTechnology) {
        s_val = atoi(d.getValue(4));
        fillRect(10, 90, 310, 230, RR_COLOR);
        thermoCurrent = ((s_val * 100.0) / 255);
        swprintf(opt, 179, L"Sat - %d%%", roundfun(thermoCurrent));
        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 15, 300, 1, 0, 18.0f, 65535, 1);
        fillRect(25, RECT_Y, 300, RECT_Y + RECT_HEIGHT + 5, RGB(23, 191, 227));
        drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2) + 2, 301 - 5, RECT_Y + (RECT_HEIGHT / 2) + 2, 65535);
        drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 3) + 1, 25 + 5, RECT_Y + ((RECT_HEIGHT * 2) / 3) + 1, 65535);
        drawLine(301 - 5, RECT_Y + (RECT_HEIGHT / 3), 301 - 5, RECT_Y + ((RECT_HEIGHT * 2) / 3) + 1, 65535);
        fillCircle(35 + s_val, RECT_Y + (RECT_HEIGHT / 2) + 2, 5, 65535);
        drawText(L"Hue", NOTIFICATION_X, RECT_Y + (RECT_HEIGHT / 2) + 30, 300, 1, 0, 18.0f, 65535, 1);
        pngImg.drawPNG(25, RECT_Y + (RECT_HEIGHT / 2) + 55, 1, 0, color_png);
        fillRect(280, 181, 300, 221, RGB(255, 255, 255));
    }
}

//void Sirenon_onTouch(int time) {
//    char value[10] = "";
//    sprintf(value, "%d" , time);
//    try {
//        Device d(contDevID);
//        d.setValue(1, value);
//    } catch (int exeption) {
//        return;
//    }
//}

//void Sirenoff_onTouch(void) {
//    try {
//        Device d(contDevID);
//        d.setValue(1, "0");
//    } catch (int exeption) {
//        return;
//    }
//}

void Sirenon_onTouch1(int time) {
    Device *d = getDevice(contDevID);
    if (d) {
        int indexNo = d->getIndexNoFromName("ALARM_STATE", -1);
        if (time > 65535) {
            swprintf(N_opt, 179, L"");
            sprintf(Time, "%ls", N_opt);
            if (change == CHANGE_TIME_GENERIC) {
                TimeBox->draw();
                TimeBox->setText(N_opt);
            } else if (change == CHANGE_TIME) {
                WDTime->draw();
                WDTime->setText(N_opt);
            }
        } else if (time == 65535) {
//            d->setSirenOn(time);
            connectedHomeSetValue(indexNo, time);
            swprintf(N_opt, 179, L"65535");
            sprintf(Time, "%ls", N_opt);
            if (change == CHANGE_TIME_GENERIC) {
                TimeBox->draw();
                TimeBox->setText(N_opt);
            } else if (change == CHANGE_TIME) {
                WDTime->draw();
                WDTime->setText(N_opt);
            }
        } else {
            connectedHomeSetValue(indexNo, time);
        }
    }
    change = 0;
}

void Sirenoff_onTouch1(void) {
    //printf("No device found \n");
//    Device *d = getDevice(contDevID);
//    if (d) {
//        d->setSirenOff();
    wmemset(N_opt, 0, sizeof (N_opt));
    sprintf(Time, "%ls", N_opt);
        if (change == CHANGE_TIME_GENERIC) {
            TimeBox->draw();
        TimeBox->setText(N_opt);
        } else if (change == CHANGE_TIME) {
            WDTime->draw();
        WDTime->setText(N_opt);
        }
//    }
}

void MultiSwitchOnButton_onTouch(void) {
//    Device *d = getDevice(contDevID);
//    if (d) {
//        d->setValue(1, "true");
//        sprintf(t_temp, "Setting MultilSwitch 1 Id:%d On", contDevID);
//        LOG(Logger::INFO, t_temp);
//    }
    connectedHomeSetValue(1, "true");
}

void MultiSwitchOffButton_onTouch(void) {
//    Device *d = getDevice(contDevID);
//    if (d) {
//        d->setValue(1, "false");
//        sprintf(t_temp, "Setting MultilSwitch 1 Id:%d Off", contDevID);
//        LOG(Logger::INFO, t_temp);
//    }
    connectedHomeSetValue(1, "false");
}

void MultiSwitchOnButton1_onTouch(void) {
//    Device *d = getDevice(contDevID);
//    if (d) {
//        d->setValue(2, "true");
//        sprintf(t_temp, "Setting MultilSwitch 2 Id:%d On", contDevID);
//        LOG(Logger::INFO, t_temp);
//    }
    connectedHomeSetValue(2, "true");
}

void MultiSwitchOffButton1_onTouch(void) {
//    Device *d = getDevice(contDevID);
//    if (d) {
//        d->setValue(2, "false");
//        sprintf(t_temp, "Setting MultilSwitch 2 Id:%d Off", contDevID);
//        LOG(Logger::INFO, t_temp);
//    }
    connectedHomeSetValue(2, "false");
}

void basicGetButton_onTouch(void) {
    try {
        Device d(contDevID);
        d.getBasic();
    } catch (int exeption) {
        return;
    }
    sprintf(t_temp, "Basic get of device %d ", contDevID);
    LOG(Logger::INFO, t_temp);
}

void basicGetText_onTouch(void) {
}

void basicSetButton_onTouch(void) {
//    char value[10] = "";
//    sprintf(value, "%d" ,basicVal);
//    try {
//        Device d(contDevID);
//        d.setValue( 2, value);
//    } catch (int exeption) {
//        return;
//    }
//    sprintf(t_temp, "Basic set of device %d ", contDevID);
//    LOG(Logger::INFO, t_temp);
    connectedHomeSetValue(1, "true");
}

void basicSetTextModifyPlus_onTouch(void) {
    if (basicVal < 255)
        basicVal++;
    swprintf(opt, 5, L"%d", basicVal);
    basicSetText.setText(opt);
    basicSetText.draw();
}

void basicSetTextModifyMinus_onTouch(void) {
    if (basicVal > 0)
        basicVal--;
    swprintf(opt, 5, L"%d", basicVal);
    basicSetText.setText(opt);
    basicSetText.draw();
}

/*bool basicValidate(wchar_t * keyboardText) {
    return true;
}*/

void basicSetText_onTouch(void) {
    if (basicVal == 0)
        basicVal = 255;
    else
        basicVal = 0;
    swprintf(opt, 5, L"%d", basicVal);
    basicSetText.setText(opt);
    basicSetText.draw();
}

void theEventCallback(Event * e) {
    if (e->event) {
        switch (e->event) {
            case DeviceAdded:
            {
                LOG(Logger::PRINT, "Received DeviceAdded event");
                DeviceEvent DevicePacket;
                memcpy(&DevicePacket, e->data, sizeof (DevicePacket));
                try {
                    Device d(DevicePacket.DevId);
                    dL.devices.push_back(d);
                    if (MODE == LIST_MODE || MODE == CONTROL_MODE) {
                        int cnt = 0;
                        for (list <Device>::iterator iterator = dL.devices.begin(); (iterator != dL.devices.end()) && (cnt < 250); ++iterator) {
                            devIdList[cnt++] = (*iterator).getID();
                        }
                        //connectedHome_draw();
                    }
                    if (MODE == LIST_MODE) {
                        //sleep(10);
                        connectedHome_draw();
                    }
                } catch (int i) {
                    char buf[100] = "";
                    sprintf(buf, "Exception caught while loading device %d", DevicePacket.DevId);
                    LOG(Logger::ERROR, buf);
                }
            }
                break;
            case DeviceRemoved:
            {
                LOG(Logger::PRINT, "Received DeviceRemoved event");
                DeviceEvent DevicePacket;

                memcpy(&DevicePacket, e->data, sizeof (DevicePacket));
                list <Device>::iterator it = dL.devices.begin();
                for (; it != dL.devices.end(); ++it) {
                    if ((*it).getID() == DevicePacket.DevId) {
                        LOG(Logger::INFO, "-----------------");
                        dL.devices.erase(it);
                        int cnt = 0;
                        for (list <Device>::iterator iterator = dL.devices.begin(); (iterator != dL.devices.end()) && (cnt < 250); ++iterator) {
                            devIdList[cnt++] = (*iterator).getID();
                        }
                        if (contDevID == DevicePacket.DevId) {
                            MODE = LIST_MODE;
                            connectedHome_draw();
                        }
                        break;
                    }
                }
            }
                break;
            case DeviceRemoveAll:
            {
                LOG(Logger::PRINT, "Received DeviceRemoveAll event");
                dL.devices.clear();
                connectedHome_draw();
            }
                break;
            case DeviceUpdated:
            {
                LOG(Logger::PRINT, "Received DeviceUpdated event");
                DeviceEvent DevicePacket;
                memcpy(&DevicePacket, e->data, sizeof (DevicePacket));
                for (list <Device>::iterator iterator = dL.devices.begin(); iterator != dL.devices.end(); ++iterator) {
                    if ((*iterator).getID() == DevicePacket.DevId) {
                        (*iterator).RefreshDevice();
                        if (MODE == CONTROL_MODE) {
                            if (DevicePacket.DevId == contDevID) {
                                char ss[180];
                                fillRect(32, 57, 285, 92, RR_COLOR);
                                memset(opt, 0, sizeof (opt));
                                utf8_to_wchar((*iterator).getDeviceName(), strlen((*iterator).getDeviceName()), opt, strlen((*iterator).getDeviceName()), UTF8_IGNORE_ERROR);
                                drawText(opt, 32, 57, 253, 1, 0, 20.0f, 65535, 1);
                                memset(opt, 0, sizeof (opt));
                                sprintf(ss, "@ %hs", (*iterator).getDeviceLocation());
                                utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
                                drawText(opt, 30, 77, 250, 1, 0, 16.0f, 65535, 1);
                                memset(opt, 0, sizeof (opt));
                            }
                        } else if (MODE == LIST_MODE) {
                            if (dL.devices.size() <= 12) {
                                connectedHome_draw();
                            } else {
                                for (int i = 0; i < dL.devices.size(); i++) {
                                    if (devIdList[i] == DevicePacket.DevId) {
                                        if ((i / 11) == curPage) {
                                            connectedHome_draw();
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                }
                printf("Updated device\n");
            }
                break;
            case AlmondModeUpdated:
            {
               /* strncpy(almond_mode, (char*) e->data + 5, 2);
                LOG(Logger::INFO, almond_mode);
                if (MODE == LIST_MODE) {
                    fillRect(275, 5, 319, 60, BACKGROUND_LAYER);
                    if (atoi(almond_mode) == 2) {
                        pngImg.drawPNG(290, 5, 0, BACKGROUND_LAYER, in_home);
                    } else if (atoi(almond_mode) == 3) {
                        pngImg.drawPNG(290, 5, 0, BACKGROUND_LAYER, away_home);
                    }

                }
                sprintf(t_temp, "data received AlmondModeUpdated %s", e->data);
                LOG(Logger::INFO, t_temp);*/
            }
                break;
            case AlmondUpdated:
            {
               /* char buf[100] = "";
                RouterData rDat;
                memcpy(&rDat, e->data, sizeof (rDat));

                switch (rDat.DataType) {
                    case AlmondMode:
                    {
                        sprintf(buf, "Received %s Data", rDat.Data);
                        LOG(Logger::PRINT, buf);
                        if (MODE == LIST_MODE) {
                            fillRect(275, 5, 319, 60, BACKGROUND_LAYER);
                            if (rDat.Data[0] == '2') {
                                pngImg.drawPNG(290, 5, 0, BACKGROUND_LAYER, in_home);
                            } else if (rDat.Data[0] == '3') {
                                pngImg.drawPNG(290, 5, 0, BACKGROUND_LAYER, away_home);
                            }

                        }
                    }
                        break;
                }*/
            }
                break;
            default:
            {

                char buf[100] = "", evnt[30] = "";
                getEventAsString(e->event, evnt);
                sprintf(buf, "Received %s event", evnt);
                LOG(Logger::PRINT, buf);
            }
                break;
        }
    }
}

void theCallback(Device * notifiedDevice) {
	 cout<<"\n\n\nto the callback\n\n\n";
    for (list <Device>::iterator iterator = dL.devices.begin(); iterator != dL.devices.end(); ++iterator) {
        if ((*iterator).getID() == notifiedDevice->getID()) {
            (*iterator).RefreshDevice();
        }
    }

    if (notifiedDevice->getLastNotificationIndex() == 15 && MODE == LIST_MODE) {
        if (notifiedDevice->getDeviceType() == SecurifiSmartSwitch) {
            for (list<peanut_plug>::iterator iterator = peanut_plug_info.begin(); iterator != peanut_plug_info.end(); ++iterator) {
                if ((iterator->S_Dev_ID == notifiedDevice->getID())) {
                    if (atoi(notifiedDevice->getLastNotificationIndexValue()) > 200) {
                        iterator->S_state = false;
                        iterator->percentage = 0;
                        u_val = 0;
                        ota_upgrade_flag = false;
                    } else if ((atoi(notifiedDevice->getLastNotificationIndexValue()) < 100)) {
                        iterator->S_state = true;
                        ota_upgrade_flag = true;
                        s_devid = notifiedDevice->getID();
                        iterator->percentage = (atoi(notifiedDevice->getLastNotificationIndexValue()));
                    } else {
                        iterator->S_state = false;
                        iterator->percentage = 0;
                        ota_upgrade_flag = false;
                        u_val = 0;
                    }
                }
            }
        }
    }

    if (MODE == LIST_MODE || MODE == KEY_MODE) {

        return;
    }
    if (MODE == CONTROL_MODE || MODE == COLOR_MODE || MODE == UPGRADE_MODE || MODE == RETAIN_MODE || MODE == BLINK_COLOR_MODE || (MODE == IR_OPERATION_MODE) || (MODE == IR_LED_SELECTION_MODE) || (MODE == EDIT_APPLIANCE_NAME) || (MODE == EDIT_BUTTON_NAME)) {
        if (notifiedDevice->getID() == contDevID) {
            memset(opt, 0, sizeof (opt));
            switch (contDev) {
                case UnknownOnOffModule:
                case BinarySwitch:
                case Siren:
                {
                    Device d(contDevID);
                        char tmp_str[30];
                    if (d.getDeviceTechnology() == ZWaveTechnology) {//newal2
                        if (notifiedDevice->getLastNotificationIndex() == 1) {
                            fillRect(50, 100, 200, 135, RR_COLOR);
                            strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                            swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
                            drawText(opt, 60, 100, 200, 1, 0, 45.0f, 65535, 1);
                        } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                            int batteryVal;
                            batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                            if (batteryVal != 0xff && batteryVal != 0) {
                                swprintf(opt, 5, L"%hd%%", batteryVal);
                                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                            } else {
                                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                            }
                        }
                    } else {
                        if (notifiedDevice->getLastNotificationIndex() == 1) {
                            strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                            fillRect(NOTIFICATION_X, NOTIFICATION_Y, 309, NOTIFICATION_Y + 35, RR_COLOR);
                            swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
                            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y, 300, 1, 0, 35.0f, 65535, 1);
                        }
                    }
                }
                    break; //Switch";
                case MultilevelSwitch:
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        dimmerCurrLevel = 2 * atoi(notifiedDevice->getLastNotificationIndexValue());
                        if (dimmerCurrLevel == 0) {
                            swprintf(opt, 179, L"%ls(%d%%)", lan.get("off_1", "common"), (dimmerCurrLevel / 2));
                        } else {
                            swprintf(opt, 179, L"%ls(%d%%)", lan.get("on_1", "common"), (dimmerCurrLevel / 2));
                        }
                        fillRect(NOTIFICATION_X + 100, NOTIFICATION_Y - 14, 200, NOTIFICATION_Y + 2, RR_COLOR);
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 14, 300, 1, 0, 18.0f, 65535, 1);
                        fillRect(RECT_X, RECT_Y, RECT_X + RECT_WIDTH, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
                        drawLine(RECT_X + 5, RECT_Y + (RECT_HEIGHT / 2), RECT_X + RECT_WIDTH - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                        drawLine(RECT_X + 5, RECT_Y + (RECT_HEIGHT / 3), RECT_X + 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
                        drawLine(RECT_X + RECT_WIDTH - 5, RECT_Y + (RECT_HEIGHT / 3), RECT_X + RECT_WIDTH - 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
                        fillCircle(RECT_X + 10 + dimmerCurrLevel, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                    }
                    break; //Dimmer";
                case BinaryPowerSwitch:
                {
                    char tmp_str[30];
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                        fillRect(40, 95, 430, 145, RR_COLOR);
                        swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
                        fillRect(NOTIFICATION_X + 27, NOTIFICATION_Y - 15, 115, NOTIFICATION_Y + 25, RR_COLOR);
                        drawText(opt, NOTIFICATION_X + 28, NOTIFICATION_Y, 110, 1, 0, 35.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                        fillRect(NOTIFICATION_X + 105, NOTIFICATION_Y - 15, 250, NOTIFICATION_Y + 30, RR_COLOR);
                        swprintf(opt, 179, L"(%ls:%hs W)", lan.get("power", "connected_sensor"), tmp_str);
                        drawText(opt, NOTIFICATION_X + 105, NOTIFICATION_Y + 5, 150, 1, 0, 25.0f, 65535, 1);
                    }
                }
                    break; //Dimmer";
                case MultilevelSwitchOnOff:
                case ColorDimmableLight:
                {
                    float val;
                    if (MODE == CONTROL_MODE) {
                            char tmp_str[30];
                        if (notifiedDevice->getLastNotificationIndex() == 2) {
                            dimmerCurrLevel = atoi(notifiedDevice->getValue(1));
                            strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                            val = ((float) (dimmerCurrLevel * 100) / (float) 255);
                            fillRect(NOTIFICATION_X, NOTIFICATION_Y - 15, 309, NOTIFICATION_Y + 5, RR_COLOR);
                            swprintf(opt, 179, L"%ls (%d%%)", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"), roundfun(val));
                            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 15, 300, 1, 0, 18.0f, 65535, 1);
                            //   char devarr[50];
                        } else if (notifiedDevice->getLastNotificationIndex() == 1) {
                            strcpy(tmp_str, notifiedDevice->getValue(2));
                            dimmerCurrLevel = atoi(notifiedDevice->getLastNotificationIndexValue());
                            val = round(((float) (dimmerCurrLevel * 100) / (float) 255));
                            fillRect(NOTIFICATION_X, NOTIFICATION_Y - 15, 309, NOTIFICATION_Y + 5, RR_COLOR);
                            swprintf(opt, 179, L"%ls (%d%%)", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"), roundfun(val));
                            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 15, 300, 1, 0, 18.0f, 65535, 1);
                            fillRect(25, RECT_Y, 301, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 3), 25 + 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
                            drawLine(301 - 5, RECT_Y + (RECT_HEIGHT / 3), 301 - 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
                            fillCircle(25 + 10 + dimmerCurrLevel, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);
                            //       char devarr[50];
                        } else if (notifiedDevice->getLastNotificationIndex() == 5) {
                            int color_t;
                            color_t = atoi(notifiedDevice->getLastNotificationIndexValue());
                            color_t = (color_t - 1000) / 32 + 25;
                            pngImg.drawPNG(25, RECT_Y + 40, 1, 0, color_temp);
                            drawLine(color_t, RECT_Y + 40, color_t, RECT_Y + 40 + RECT_HEIGHT, 0);
                        }
                    } else if (MODE == COLOR_MODE) {
                        if (notifiedDevice->getLastNotificationIndex() == 4) {
                            int sat_v = atoi(notifiedDevice->getLastNotificationIndexValue());
                            fillRect(10, 90, 310, 150, RR_COLOR);
                            percentage = (int) round(((sat_v * 100) / 254.0));
                            swprintf(opt, 179, L"Sat - %d%%", percentage);
                            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 15, 300, 1, 0, 18.0f, 65535, 1);
                            fillRect(25, RECT_Y, 300, RECT_Y + RECT_HEIGHT + 5, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2) + 2, 301 - 5, RECT_Y + (RECT_HEIGHT / 2) + 2, 65535);
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 3) + 1, 25 + 5, RECT_Y + ((RECT_HEIGHT * 2) / 3) + 1, 65535);
                            drawLine(301 - 5, RECT_Y + (RECT_HEIGHT / 3), 301 - 5, RECT_Y + ((RECT_HEIGHT * 2) / 3) + 1, 65535);
                            fillCircle(35 + sat_v, RECT_Y + (RECT_HEIGHT / 2) + 2, 5, 65535);
                            int hue_v = atoi(notifiedDevice->getValue(3));
                            hue_v = hue_v + 25;
                            pngImg.drawPNG(25, RECT_Y + (RECT_HEIGHT / 2) + 55, 1, 0, color_png);
                            fillRect(280, 181, 300, 221, RGB(255, 255, 255));
                            drawLine(hue_v, RECT_Y + (RECT_HEIGHT / 2) + 55, hue_v, RECT_Y + (RECT_HEIGHT / 2) + 95, 0);
                        } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                            int hue_v = atoi(notifiedDevice->getLastNotificationIndexValue());
                            hue_v = hue_v + 25;
                            pngImg.drawPNG(25, RECT_Y + (RECT_HEIGHT / 2) + 55, 1, 0, color_png);
                            fillRect(280, 181, 300, 221, RGB(255, 255, 255));
                            drawLine(hue_v, RECT_Y + (RECT_HEIGHT / 2) + 55, hue_v, RECT_Y + (RECT_HEIGHT / 2) + 95, 0);
                        }
                    }
                }
                    break; //Dimmer";
                case BinarySensor:
                {
                    if (MODE != DEVICE_MODE) {//
                        if (notifiedDevice->getLastNotificationIndex() == 1) {
                            fillRect(80, 130, 300, 200, RR_COLOR);
                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                swprintf(opt, 179, L" %ls", lan.get("true", "common"));
                            } else {
                                swprintf(opt, 179, L" %ls", lan.get("false", "common"));
                            }
                            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 20, 300, 1, 0, 35.0f, 65535, 1);
                        } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                            int batteryVal;
                            batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                            if (batteryVal != 0xff && batteryVal != 0) {
                                swprintf(opt, 5, L"%hd%%", batteryVal);
                                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                            } else {
                                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                            }
                        }
                        if (notifiedDevice->getLastNotificationIndex() == 3) {
                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                TamperButton.draw();
                            } else {
                                fillRect(45, 240, 140, 275, RR_COLOR);
                            }
                        }
                        // drawText(opt, 40, 135, 400, 1, 0, 45.0f, 65535, 1);
                    }

                }
                    break; //Binary Sensor";    
                case DoorSensor:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(80, 120, 300, 180, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "false") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("open", "connected_sensor"));
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("closed", "connected_sensor"));
                        }
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 40.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        int batteryVal;
                        batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                        pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                        if (batteryVal != 0xff && batteryVal != 0) {
                            swprintf(opt, 5, L"%hd%%", batteryVal);
                            drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                        } else {
                            drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            TamperButton.draw();
                        } else {
                            fillRect(45, 240, 140, 275, RR_COLOR);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 4) {
                        convertTemperature(4, opt);
                        fillRect(215, 93, 300, 122, GREEN_COLOR);
                        drawText(opt, 215, 95, 85, 1, 0, 25.0f, 65535, 1);
                    }
                      //connectedHomeSetValue(

                }
                    break;
                case MovementSensor:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(10, 120, 300, 200, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("motiondetected", "connected_sensor"));
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("nomotion", "connected_sensor"));
                        }
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        int batteryVal;
                        batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                        pngImg.drawPNG(200, 45, 1, 0, battery_icon);
                        if (batteryVal != 0xff && batteryVal != 0) {
                            swprintf(opt, 5, L"%hd%%", batteryVal);
                            drawText(opt, 205, 47, 80, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                        } else {
                            drawText(lan.get("low", "connected_sensor"), 205, 47, 80, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            TamperButton.draw();
                        } else {
                            fillRect(45, 240, 140, 275, RR_COLOR);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 4) {
                        fillRect(225, 85, 300, 110, RR_COLOR);
                        convertTemperature(4, opt);
                        drawText(opt, 225, 87, 75, 1, 0, 16.0f, 65535, 1);
                    }
                }
                    break; //MovementSensor";                    
                case ShockSensor:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(10, 120, 300, 200, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("vibrationdetected", "connected_sensor"));
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("novibration", "connected_sensor"));
                        }
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        int batteryVal;
                        batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                        pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                        if (batteryVal != 0xff && batteryVal != 0) {
                            swprintf(opt, 5, L"%hd%%", batteryVal);
                            drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                        } else {
                            drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                        }
                    }
                    if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            TamperButton.draw();
                        } else {
                            fillRect(45, 240, 140, 275, RR_COLOR);
                        }
                    }
                }
                    break; //Binary Sensor";
                case FloodSensor:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(10, 120, 300, 200, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "0") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("flooded", "connected_sensor"));
                        }
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        int batteryVal;
                        batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                        pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                        if (batteryVal != 0xff && batteryVal != 0) {
                            swprintf(opt, 5, L"%hd%%", batteryVal);
                            drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                        } else {
                            drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                        }
                    }
                }
                    break; //FloodSensor";
                case SmokeDetector:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(10, 120, 300, 170, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "0") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
                        } else if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "254") == 0) {
                            swprintf(opt, 179, L" %ls", L"Testing");
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("smokedetected", "connected_sensor"));
                        }
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        int batteryVal;
                        batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                        pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                        if (batteryVal != 0xff && batteryVal != 0) {
                            swprintf(opt, 5, L"%hd%%", batteryVal);
                            drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                        } else {
                            drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                        }
                    }
                }
                    break; //SmokeDetector";
                case Alarm:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(10, 120, 300, 200, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "0") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("false", "common"));
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("true", "common"));
                        }
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        int batteryVal;
                        batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                        pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                        if (batteryVal != 0xff && batteryVal != 0) {
                            swprintf(opt, 5, L"%hd%%", batteryVal);
                            drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                        } else {
                            drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                        }
                    }
                }
                    break; //Alarm";
                case MoistureSensor:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        wchar_t arr[20];
                        fillRect(190, 95, 300, 125, GREEN_COLOR);
                        convertTemperature(1, arr);
                        drawText(arr, 200, 100, 100, 1, 0, 25.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        fillRect(10, 120, 300, 200, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "0") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("notflooded", "connected_sensor"));
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("flooded", "connected_sensor"));
                        }
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                        int batteryVal;
                        batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                        pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                        if (batteryVal != 0xff && batteryVal != 0) {
                            swprintf(opt, 5, L"%hd%%", batteryVal);
                            drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                        } else {
                            drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                        }
                    }
                }
                    break;
                case TemperatureSensor:
                {
                    char ss[500];
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(225, 85, 300, 110, RR_COLOR);
                        sprintf(ss, "%ls %s", lan.get("temp", "connected_sensor"), notifiedDevice->getLastNotificationIndexValue());
                        utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
                        drawText(opt, 225, 87, 75, 1, 0, 16.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        fillRect(10, 120, 300, 200, RR_COLOR);
                        swprintf(opt, 179, L"%ls %hs", lan.get("humidity", "connected_sensor"), notifiedDevice->getLastNotificationIndexValue());
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 30.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                        int batteryVal;
                        batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                        pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                        if (batteryVal != 0xff && batteryVal != 0) {
                            swprintf(opt, 5, L"%hd%%", batteryVal);
                            drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                        } else {
                            drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                        }
                    }
                }

                    break;
                case LightSensor:
                {
                    switch (notifiedDevice->getIndexType(notifiedDevice->getLastNotificationIndex())) {
                        case Temperature:
                        {

                            fillRect(190, 95, 300, 125, GREEN_COLOR);
                            memset(opt, 0, sizeof (opt));
                            convertTemperature(notifiedDevice->getLastNotificationIndex(), opt);
                            drawText(opt, 200, 100, 100, 1, 0, 20.0f, 65535, 1);
                        }
                            break;
                        case Humidity:
                        {
                            swprintf(opt, 20, L"%4s%%    ", notifiedDevice->getLastNotificationIndexValue());
                            drawTextWithBG(opt, 175, 160, 60, 1, 0, 20.0f, 65535, RR_COLOR, 0);
                        }
                            break;
                        case Illuminance:
                        {
                            swprintf(opt, 20, L"%s Lux    ", notifiedDevice->getLastNotificationIndexValue());
                            drawTextWithBG(opt, 175, 135, 70, 1, 0, 20.0f, 65535, RR_COLOR, 0);
                        }
                            break;
                        case Battery:
                        {
                            int batteryVal;
                            batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                            if (batteryVal != 0xff && batteryVal != 0) {
                                swprintf(opt, 5, L"%hd%%", batteryVal);
                                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                            } else {
                                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                            }
                        }
                            break;
                        case Tamper:
                        {
                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                enableTouch = true;
                                TamperButton.draw();
                            }
                        }
                    }
                }
                    break;
                case ZigbeeMultiSensor:
                {

                    switch (notifiedDevice->getIndexType(notifiedDevice->getLastNotificationIndex())) {
                        case Temperature:
                        {

                            fillRect(190, 95, 300, 125, GREEN_COLOR);
                            memset(opt, 0, sizeof (opt));
                            convertTemperature(notifiedDevice->getLastNotificationIndex(), opt);
                            drawText(opt, 200, 100, 100, 1, 0, 20.0f, 65535, 1);
                        }
                            break;
                        case Tamper:
                        {
                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                enableTouch = true;
                                TamperButton.draw();
                            }
                        }
                            break;
                        case MotionState:
                        {
                            char tempv[10];
                            strcpy(tempv, notifiedDevice->getLastNotificationIndexValue());
                            swprintf(opt, 179, L"%ls", (tempv[0] == 'f' || tempv[0] == 'F') ? lan.get("nomotion", "connected_sensor") : lan.get("motiondetected", "connected_sensor"));
                            fillRect(70, 129, 260, 143, RR_COLOR);
                            drawText(opt, 70, 128, 190, 1, 0, 20.0f, 65535, 1);
                        }
                            break;
                        case LowBattery:
                        {

                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                fillRect(70, 148, 260, 162, RR_COLOR);
                                drawText(L"Low", 70, 148, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                            }
                        }
                            break;
                        case FireState:
                        {
                            fillRect(70, 168, 260, 182, RR_COLOR);
                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                swprintf(opt, 179, L" %ls", lan.get("alarmfire", "connected_sensor"));
                            } else {
                                swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
                            }
                            drawText(opt, 70, 168, 190, 1, 0, 20.0f, 65535, 1);
                        }
                            break;
                    }



                }
                    break;
                case ContactSwitch:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(NOTIFICATION_X, NOTIFICATION_Y + 15, 300, NOTIFICATION_Y + 80, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("open", "connected_sensor"));
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("closed", "connected_sensor"));
                        }
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            enableTouch = true;
                            TamperButton.draw();
                        } else {
                            fillRect(15, 190, 95, 220, RR_COLOR);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 4) {
                        fillRect(215, 93, 300, 122, GREEN_COLOR);
                        convertTemperature(4, opt);
                        drawText(opt, 215, 95, 85, 1, 0, 25.0f, 65535, 1);
                    }
                    battery(contDevID);
                }
                    break;
                case VibrationOrMovementSensor:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(NOTIFICATION_X, NOTIFICATION_Y + 15, 300, NOTIFICATION_Y + 80, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("vibrationdetected", "connected_sensor"));
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("novibration", "connected_sensor"));
                        }

                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                    }
                    if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            enableTouch = true;
                            TamperButton.draw();
                        } else {
                            fillRect(15, 190, 95, 220, RR_COLOR);
                        }
                    }
                    battery(contDevID);
                }
                    break;
                case MotionSensor:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(NOTIFICATION_X, NOTIFICATION_Y + 15, 300, NOTIFICATION_Y + 80, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("motiondetected", "connected_sensor"));
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("nomotion", "connected_sensor"));
                        }

                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                    }
                    if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            enableTouch = true;
                            TamperButton.draw();
                        } else {
                            fillRect(15, 190, 95, 220, RR_COLOR);
                        }
                    }
                    if (notifiedDevice->getLastNotificationIndex() == 4) {
                        fillRect(215, 93, 300, 122, GREEN_COLOR);
                        convertTemperature(4, opt);
                        drawText(opt, 215, 95, 85, 1, 0, 25.0f, 65535, 1);
                    }
                    battery(contDevID);
                }
                    break;
                case FireSensor:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(NOTIFICATION_X, NOTIFICATION_Y + 15, 300, NOTIFICATION_Y + 80, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("alarmfire", "connected_sensor"));
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
                        }

                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                    }
                    if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            enableTouch = true;
                            TamperButton.draw();
                        } else {
                            fillRect(15, 190, 95, 220, RR_COLOR);
                        }
                    }
                    battery(contDevID);
                }
                    break;
                case WaterSensor:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(NOTIFICATION_X, NOTIFICATION_Y + 15, 300, NOTIFICATION_Y + 80, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            swprintf(opt, 179, L" %ls", lan.get("flooded", "connected_sensor"));
                        } else {
                            swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
                        }

                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                    }
                    if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            enableTouch = true;
                            TamperButton.draw();
                        } else {
                            fillRect(15, 190, 95, 220, RR_COLOR);
                        }
                    }
                    if (notifiedDevice->getLastNotificationIndex() == 4) {
                        fillRect(215, 93, 300, 122, GREEN_COLOR);
                        convertTemperature(4, opt);
                        drawText(opt, 215, 95, 85, 1, 0, 25.0f, 65535, 1);
                    }
                    battery(contDevID);
                }
                    break;
                case KeyFob:

                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(NOTIFICATION_X, NOTIFICATION_Y + 15, 300, NOTIFICATION_Y + 80, RR_COLOR);
                        if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 0) {
                            drawText(lan.get("disarmall", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                        } else if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 2) {
                            drawText(lan.get("armperimeter", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                        } else if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 3) {
                            drawText(lan.get("armall", "connected_sensor"), NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {

                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            PanicButton.draw();
                        } else {
                            fillRect(15, 190, 95, 220, RR_COLOR);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            EmerButton.draw();
                        } else {
                            fillRect(100, 190, 165, 220, RR_COLOR);
                        }
                    }
                    break;
                case StandardWarningDevice:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            enableTouch = true;
                            TamperButton.draw();
                        } else {
                            fillRect(15, 190, 95, 220, RR_COLOR);
                        }
                    }
                }
                    break;
                case GasSensor:
                case PersonalEmergencyDevice:
                case Keypad:
                case StandardCIE:

                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(NOTIFICATION_X, NOTIFICATION_Y + 15, 300, NOTIFICATION_Y + 80, RR_COLOR);
                        if (contDev == GasSensor) {
                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                swprintf(opt, 179, L" %ls", lan.get("gasdetected", "connected_sensor"));
                            } else {
                                swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
                            }
                        } else {
                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                swprintf(opt, 179, L" %ls", lan.get("active", "connected_sensor"));
                            } else {
                                swprintf(opt, 179, L" %ls", lan.get("inactive", "connected_sensor"));
                            }
                        }
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                    }
                    if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            enableTouch = true;
                            TamperButton.draw();
                        } else {
                            fillRect(15, 190, 95, 220, RR_COLOR);
                        }
                    }
                    battery(contDevID);
                }
                    break;
                case RemoteControl:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(NOTIFICATION_X, NOTIFICATION_Y + 15, 300, NOTIFICATION_Y + 80, RR_COLOR);
                        if (contDev == GasSensor) {
                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                swprintf(opt, 179, L" %ls", lan.get("gasdetected", "connected_sensor"));
                            } else {
                                swprintf(opt, 179, L" %ls", lan.get("okay", "connected_sensor"));
                            }
                        } else {
                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                swprintf(opt, 179, L" %ls", lan.get("active", "connected_sensor"));
                                wcscpy(btnDeActive.W.text, L"Deactivate");
                                btnDeActive.draw();
                            } else {
                                fillRect(25, 190, 105, 220, RR_COLOR);
                                wcscpy(btnDeActive.W.text, L"Activate");
                                swprintf(opt, 179, L" %ls", lan.get("inactive", "connected_sensor"));
                            }
                        }
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y + 15, 300, 1, 0, 35.0f, 65535, 1);
                    }
                    if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            enableTouch = true;
                            TamperButton.draw();
                        } else {
                            fillRect(15, 190, 95, 220, RR_COLOR);
                        }
                    }
                    battery(contDevID);
                }
                    break;
                case DoorLock:
                {
                        if (MODE != DEVICE_MODE) {//
                            if (notifiedDevice->getLastNotificationIndex() == 1) {
                                wchar_t abc[180] = L"", xyz[180] = L"";
                                swprintf(abc, 179, L"%ls", lan.get("doorlock", "connected_sensor"));
                                if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 0) {
                                    swprintf(xyz, 179, L"%ls", lan.get("unlocked", "connected_sensor"));
                                } else if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 23 || atoi(notifiedDevice->getLastNotificationIndexValue()) == 26 || atoi(notifiedDevice->getLastNotificationIndexValue()) == 17 || atoi(notifiedDevice->getLastNotificationIndexValue()) == 254) {
                                    swprintf(xyz, 179, L"%ls", lan.get("notfullylocked", "connected_sensor"));
                                } else {
                                    swprintf(xyz, 179, L"%ls", lan.get("locked", "connected_sensor"));
                                }
                                swprintf(opt, 179, L"%ls: %ls", abc, xyz);
                                fillRect(40, 97, 240, 122, RR_COLOR);
                                drawText(opt, 40, 97, 200, 1, 0, 20.0f, 65535, 1);
                            } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                                int batteryVal;
                                batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                                pngImg.drawPNG(243, 60, 1, 0, battery_icon);
                                if (batteryVal != 0xff && batteryVal != 0) {
                                    swprintf(opt, 5, L"%hd%%", batteryVal);
                                    drawText(opt, 243, 62, 40, 1, 0, 12.0f, RGB(0, 0, 0), 1);
                                } else {
                                    drawText(lan.get("low", "connected_sensor"), 243, 62, 40, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                                }
                            } else if (notifiedDevice->getLastNotificationIndex() > 4) {
                                if (notifiedDevice->getLastNotificationIndex() == Userno + 4) {
                                    if (!strcmp(notifiedDevice->getLastNotificationIndexValue(), "")) {
                                        sprintf(Userpin, "%ls", lan.get("unassigned", "connected_sensor"));
                                    } else {
                                        strcpy(Userpin, notifiedDevice->getLastNotificationIndexValue());
                                    }
                                    fillRect(105, 160, 225, 185, RR_COLOR);
                                    swprintf(opt, 179, L"%s", Userpin);
                                    drawText(opt, 105, 160, 120, 1, 0, 25.0f, 65535, 0);
                                }
                            }


                    }
                }
                    break; //Door Lock";
                case ZenThermostat:
                case Thermostat:
                {
                    double er;
                    wchar_t arr[10];
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        thermoCurrent = atof(notifiedDevice->getValue(1));
                        memset(arr, 0, sizeof (arr));
                        fillRect(225, 97, 300, 125, RGB(50, 191, 20));
                        convertTemperature(1, arr);
                        drawText(arr, 225, 97 + 6, 75, 1, 0, 18.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 4) {
                        er = atof(notifiedDevice->getValue(4));
                        thermoHeat = (int) er;
                        thermoHeatCurrLevel = (thermoHeat - 35)*3;
                        fillRect(THERMO_HEAT_X + 6, THERMO_HEAT_Y, THERMO_HEAT_X + THERMO_HEAT_WIDTH - 6, THERMO_HEAT_Y + THERMO_HEAT_HEIGHT, RGB(23, 191, 227));
                        drawLine(THERMO_HEAT_X + 5, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 65535);
                        if (THERMO_HEAT_X + 10 + thermoHeatCurrLevel < 250) {
                            fillCircle(THERMO_HEAT_X + 10 + thermoHeatCurrLevel, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);
                        } else {
                            fillCircle(THERMO_HEAT_X + THERMO_HEAT_WIDTH - 11, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 5) {
                        er = atof(notifiedDevice->getValue(5));
                        thermoCool = (int) er;
                        thermoCoolCurrLevel = (thermoCool - 35)*3;
                        fillRect(THERMO_COOL_X + 6, THERMO_COOL_Y, THERMO_COOL_X + THERMO_COOL_WIDTH - 6, THERMO_COOL_Y + THERMO_COOL_HEIGHT, RGB(23, 191, 227));
                        drawLine(THERMO_COOL_X + 5, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), THERMO_COOL_X + THERMO_COOL_WIDTH - 5, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 65535);
                        if (THERMO_COOL_X + 10 + thermoCoolCurrLevel < 250) {
                            fillCircle(THERMO_COOL_X + 10 + thermoCoolCurrLevel, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);
                        } else {
                            fillCircle(THERMO_COOL_X + THERMO_COOL_WIDTH - 11, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        strcpy(thermoMode, notifiedDevice->getValue(2));
                        fillRect(60, 97, 110, 125, !(strstr(thermoMode, "Auto")) ? BUTTON_LAYER : RGB(23, 191, 227));
                        fillRect(115, 97, 165, 125, !(strstr(thermoMode, "Cool")) ? BUTTON_LAYER : RGB(23, 191, 227));
                        fillRect(170, 97, 220, 125, !(strstr(thermoMode, "Heat")) ? BUTTON_LAYER : RGB(23, 191, 227));
                        drawText(lan.get("auto", "connected_sensor"), 65, 103, 50, 1, 0, 16.0f, 65535, 1);
                        drawText(lan.get("cool", "connected_sensor"), 120, 103, 50, 1, 0, 16.0f, 65535, 1);
                        drawText(lan.get("heat", "connected_sensor"), 175, 103, 50, 1, 0, 16.0f, 65535, 1);
                        if (strstr(thermoMode, "Auto")) {
                            pngImg.drawPNG(60, 105, 1, BACKGROUND_LAYER, Tick);
                        } else if (strstr(thermoMode, "Cool")) {
                            pngImg.drawPNG(115, 105, 1, BACKGROUND_LAYER, Tick);
                        } else if (strstr(thermoMode, "Heat")) {
                            pngImg.drawPNG(170, 105, 1, BACKGROUND_LAYER, Tick);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 6) {
                        strcpy(thermoFan, notifiedDevice->getValue(6));
                        if (notifiedDevice->getValueCount() != 10 || (notifiedDevice->getDeviceTechnology() == ZigBeeTechnology)) {
                            fillRect(60, 190, 135, 218, !(strstr(thermoFan, "Auto Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
                            fillRect(140, 190, 215, 218, !(strstr(thermoFan, "On Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
                            drawText(lan.get("auto_low", "connected_sensor"), 65, 195, 75, 1, 0, 16.0f, 65535, 1);
                            drawText(lan.get("on_low", "connected_sensor"), 145, 195, 75, 1, 0, 16.0f, 65535, 1);
                            if (strstr(thermoFan, "Auto Low")) {
                                pngImg.drawPNG(60, 197, 1, BACKGROUND_LAYER, Tick);
                            } else if (strstr(thermoFan, "On Low")) {
                                pngImg.drawPNG(140, 197, 1, BACKGROUND_LAYER, Tick);
                            }
                        } else {
                            //fillRect(60, 190, 215, 218, RR_COLOR);
                            fillRect(60, 190, 100, 218, !(strstr(thermoFan, "Auto Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
                            fillRect(110, 190, 150, 218, !(strstr(thermoFan, "On Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
                            drawText(L"Auto", 62, 195, 40, 1, 0, 16.0f, 65535, 1);
                            drawText(L"On", 110, 195, 40, 1, 0, 16.0f, 65535, 1);
                            if (strstr(thermoFan, "Auto Low")) {
                                pngImg.drawPNG(60, 197, 1, BACKGROUND_LAYER, Tick);
                            } else if (strstr(thermoFan, "On Low")) {
                                pngImg.drawPNG(110, 197, 1, BACKGROUND_LAYER, Tick);
                            }
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 8) {

                        int batteryVal;
                        batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                        pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                        if (batteryVal != 0xff && batteryVal != 0) {
                            swprintf(opt, 5, L"%hd%%", batteryVal);
                            drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                        } else {
                            drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 10) {//newal3
                        //if ((strcmp(d.getValue(10), "#ERR"))) {
                        fillRect(60, 190, 215, 218, RR_COLOR);
                        fillRect(60, 190, 100, 218, !(strstr(thermoFan, "Auto Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
                        fillRect(110, 190, 150, 218, !(strstr(thermoFan, "On Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
                        drawText(L"Auto", 62, 195, 40, 1, 0, 16.0f, 65535, 1);
                        drawText(L"On", 110, 195, 40, 1, 0, 16.0f, 65535, 1);
                        if (strstr(thermoFan, "Auto Low")) {
                            pngImg.drawPNG(60, 197, 1, BACKGROUND_LAYER, Tick);
                        } else if (strstr(thermoFan, "On Low")) {
                            pngImg.drawPNG(110, 197, 1, BACKGROUND_LAYER, Tick);
                        }
                        er = atof(notifiedDevice->getLastNotificationIndexValue());
                        pngImg.drawPNG(160, 195, 1, BACKGROUND_LAYER, humidityImage);
                        sprintf(output, "%.0lf", er);
                        aswprintf(arr, 20, L": %hs%%", output);
                        drawText(arr, 180, 195, 40, 1, 0, 16.0f, 65535, 0);
                        //}
                    }
                    EditButton.draw();
                }
                    break;
                case Gateway:

                    break; //Gateway";
                case SceneController:

                    break; //Scene Control";
                case SmartACSwitch:
                {
                    char tmp_str[30];
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                        fillRect(NOTIFICATION_X, NOTIFICATION_Y - 5, 309, NOTIFICATION_Y + 25, RR_COLOR);
                        swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 8, 300, 1, 0, 30.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 11) {

                        fillRect(NOTIFICATION_X + 77, 165, 300 - 82, 185, RR_COLOR);
                        ActivePower = atof(notifiedDevice->getLastNotificationIndexValue());
                        swprintf(opt, 179, L"%ls: %0.2fW", lan.get("power", "connected_sensor"), ActivePower);
                        drawText(opt, 10 + 75, 165, 300 - 160, 1, 0, 18.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 13) {
                        fillRect(NOTIFICATION_X + 77, 185, 300 - 82, 205, RR_COLOR);
                        RMSCurrent = atof(notifiedDevice->getLastNotificationIndexValue());
                        swprintf(opt, 179, L"%ls: %0.2fA", lan.get("current", "connected_sensor"), RMSCurrent);
                        drawText(opt, 10 + 75, 185, 300 - 160, 1, 0, 18.0f, 65535, 1);
                        //                    EditButton.draw();
                    } else if (notifiedDevice->getLastNotificationIndex() == 12) {
                        fillRect(NOTIFICATION_X + 77, 205, 300 - 82, 225, RR_COLOR);
                        RMSVoltage = atof(notifiedDevice->getLastNotificationIndexValue());
                        swprintf(opt, 179, L"%ls: %0.2f V", lan.get("voltage", "connected_sensor"), RMSVoltage);
                        drawText(opt, 10 + 75, 205, 300 - 160, 1, 0, 18.0f, 65535, 1);
                    }
                }
                    break; //Switch";
                case SecurifiSmartSwitch:
                {
                    char tmp_str[30];
                    if (MODE == CONTROL_MODE) {
                        if (notifiedDevice->getLastNotificationIndex() == 1) {
                            strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                            fillRect(NOTIFICATION_X, 97, 300, 125, RR_COLOR);
                            swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
                            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 8, 300, 1, 0, 30.0f, 65535, 1);
                        } else if (notifiedDevice->getLastNotificationIndex() == 11) {

                            fillRect(NOTIFICATION_X + 77, 165, 300 - 82, 185, RR_COLOR);
                            ActivePower = atof(notifiedDevice->getLastNotificationIndexValue());
                            //                            ActivePower = (ActivePower * powmulti) / powdiv;
                            swprintf(opt, 179, L"%ls: %0.2fW", lan.get("power", "connected_sensor"), ActivePower);
                            drawText(opt, 10 + 75, 165, 300 - 160, 1, 0, 18.0f, 65535, 1);
                        } else if (notifiedDevice->getLastNotificationIndex() == 13) {
                            fillRect(NOTIFICATION_X + 77, 185, 300 - 82, 205, RR_COLOR);
                            RMSCurrent = atof(notifiedDevice->getLastNotificationIndexValue());
                            //                            RMSCurrent = (RMSCurrent * curmulti) / curdiv;
                            swprintf(opt, 179, L"%ls: %0.2fA", lan.get("current", "connected_sensor"), RMSCurrent);
                            drawText(opt, 10 + 75, 185, 300 - 160, 1, 0, 18.0f, 65535, 1);

                        } else if (notifiedDevice->getLastNotificationIndex() == 12) {
                            fillRect(NOTIFICATION_X + 77, 205, 300 - 82, 225, RR_COLOR);
                            RMSVoltage = atof(notifiedDevice->getLastNotificationIndexValue());
                            swprintf(opt, 179, L"%ls: %0.2f V", lan.get("voltage", "connected_sensor"), RMSVoltage);
                            drawText(opt, 10 + 75, 205, 300 - 160, 1, 0, 18.0f, 65535, 1);
                        } else if (notifiedDevice->getLastNotificationIndex() == 15) {
                            for (list<peanut_plug>::iterator iterator = peanut_plug_info.begin(); iterator != peanut_plug_info.end(); ++iterator) {
                                if ((iterator->S_Dev_ID == notifiedDevice->getID())) {
                                    if (atoi(notifiedDevice->getLastNotificationIndexValue()) > 200) {
                                        iterator->S_state = false;
                                        iterator->percentage = 0;
                                        u_val = 0;
                                        ota_upgrade_flag = false;
                                    } else if ((atoi(notifiedDevice->getLastNotificationIndexValue()) < 100)) {
                                        iterator->S_state = true;
                                        ota_upgrade_flag = true;
                                        s_devid = notifiedDevice->getID();
                                        iterator->percentage = (atoi(notifiedDevice->getLastNotificationIndexValue()));
                                    } else {
                                        iterator->S_state = false;
                                        iterator->percentage = 0;
                                        ota_upgrade_flag = false;
                                        u_val = 0;
                                    }
                                }
                            }
                        }
                    } else if (MODE == RETAIN_MODE || MODE == UPGRADE_MODE) {
                        //                        LOG(Logger::INFO,"callback for FW updation");
                        if (notifiedDevice->getLastNotificationIndex() == 14) {
                            if ((strtol(notifiedDevice->getValue(14), NULL, 16)) == 0) {
                                RetainButton.setSelected(1);
                            } else {
                                RetainButton.setSelected(0);
                            }
                            RetainButton.draw();
                        } else if (notifiedDevice->getLastNotificationIndex() == 15) {
                            for (list<peanut_plug>::iterator iterator = peanut_plug_info.begin(); iterator != peanut_plug_info.end(); ++iterator) {
                                if ((iterator->S_Dev_ID == notifiedDevice->getID())) {
                                    sprintf(t_temp, "upgrading %d percentage and mode %d\n", atoi(notifiedDevice->getLastNotificationIndexValue()), MODE);
                                    LOG(Logger::INFO, t_temp);
                                    if (atoi(notifiedDevice->getLastNotificationIndexValue()) > 200) {
                                        fillRect(10, 93, 310, 127, RR_COLOR);
                                        swprintf(opt, 179, L"%ls", L"Upgrade Success");
                                        iterator->S_state = false;
                                        iterator->percentage = 0;
                                        ota_upgrade_flag = false;
                                        drawText(opt, 10, 93, 300, 1, 0, 30.0f, 65535, 1);
                                        u_val = 0;
                                        per = 0;
                                        break;
                                    } else if ((atoi(notifiedDevice->getLastNotificationIndexValue()) < 100)) {
                                        iterator->percentage = (atoi(notifiedDevice->getLastNotificationIndexValue()));
                                        if ((atoi(notifiedDevice->getLastNotificationIndexValue())) > per) {
                                            s_devid = notifiedDevice->getID();
                                            per = (atoi(notifiedDevice->getLastNotificationIndexValue()));
                                            fillRect(10, 93, 310, 127, RR_COLOR);
                                            swprintf(opt, 179, L"%ls %s%%", L"Upgrading", notifiedDevice->getLastNotificationIndexValue());
                                            drawText(opt, 10, 93, 300, 1, 0, 30.0f, 65535, 1);
                                            u_val = 0;
                                            break;
                                        } else if ((atoi(notifiedDevice->getLastNotificationIndexValue())) == 0) {
                                            s_devid = notifiedDevice->getID();
                                            ota_upgrade_flag = true;
                                            if (u_val == 0) {
                                                drawArea();
                                                fillRect(10, 93, 310, 127, RR_COLOR);
                                                swprintf(opt, 179, L"%ls 0%%", L"Upgrading");
                                                drawText(opt, 10, 93, 300, 1, 0, 30.0f, 65535, 1);
                                                upgradeButton.W.isEnabled = -1;
                                                upgradeButton.draw();
                                            }
                                            u_val++;
                                            break;
                                        }
                                    } else {
                                        if (!ota_upgrade_flag)
                                            drawArea();
                                        fillRect(10, 93, 310, 127, RR_COLOR);
                                        swprintf(opt, 179, L"%ls", L"Upgrade Failed");
                                        drawText(opt, 10, 93, 300, 1, 0, 30.0f, 65535, 1);
                                        iterator->S_state = false;
                                        iterator->percentage = 0;
                                        ota_upgrade_flag = false;
                                        u_val = 0;
                                        per = 0;
                                        break;
                                    }

                                }

                            }

                        }
                        if (notifiedDevice->getLastNotificationIndex() == 16) {
                            old_version = atoi(notifiedDevice->getLastNotificationIndexValue());
                            sprintf(t_temp, "switch version %d", old_version);
                            LOG(Logger::INFO, t_temp);

                        }
                    }
                }
                    break; //Switch";
                case GarageDoorOpener://newal2
                {
                    int DoorLevel;
                    if (MODE != DEVICE_MODE) {//
                        if (notifiedDevice->getLastNotificationIndex() == 1) {
                            fillRect(110, 100, 260, 128, RR_COLOR);
                            DoorLevel = atoi(notifiedDevice->getLastNotificationIndexValue());
                            if (DoorLevel == 255) {
                                swprintf(opt, 179, L" %ls", lan.get("open", "connected_sensor"));
                            } else if (DoorLevel == 254) {
                                swprintf(opt, 179, L" %ls", lan.get("opening", "connected_sensor")); //newal3
                            } else if (DoorLevel == 252) {
                                swprintf(opt, 179, L" %ls", lan.get("closing", "connected_sensor"));
                            } else if (DoorLevel == 253) {
                                swprintf(opt, 179, L" %ls", lan.get("stopped", "connected_sensor"));
                            } else if (DoorLevel == 0) {
                                swprintf(opt, 179, L" %ls", lan.get("closed", "connected_sensor"));
                            }
                            drawText(opt, 90, 100, 150, 1, 0, 30.0f, 65535, 1);
                            //drawText(opt, 40, 115, 390, 1, 0, 45.0f, 65535, 1);
                        }
                    }
                }
                    break;
                case MultiSoundSiren://newal2
                {
                    if (MODE != DEVICE_MODE) {
                        if (notifiedDevice->getLastNotificationIndex() == 1) {
                            int SoundType = atoi(notifiedDevice->getLastNotificationIndexValue());
                            //50, 120, 100, 50, 0, RGB(0, 205, 20), 1, 20.0f, 65535, L"");//newal2
                            fillRect(50, 120, 150, 170, RGB(0, 205, 20));
                            if (SoundType == 0) {
                                swprintf(opt, 179, L" %ls", lan.get("alarm", "connected_sensor")); //50, 120, 100, 50
                                drawText(opt, 50, 130, 100, 1, 0, 30.0f, 65535, 1);
                            } else if (SoundType == 1) {
                                swprintf(opt, 179, L" %ls", lan.get("emergency", "connected_sensor"));
                                //                                swprintf(opt, 179, L" %ls", L"Emergency");
                                drawText(opt, 50, 135, 100, 1, 0, 20.0f, 65535, 1);
                            } else if (SoundType == 2) {
                                swprintf(opt, 179, L" %ls", L"Fire");
                                //                                swprintf(opt, 179, L" %ls", L"Fire");
                                drawText(opt, 50, 130, 100, 1, 0, 30.0f, 65535, 1);
                            } else if (SoundType == 3) {
                                swprintf(opt, 179, L" %ls", L"Ambulance");
                                //                                swprintf(opt, 179, L" %ls", L"Ambulance");
                                drawText(opt, 50, 135, 100, 1, 0, 20.0f, 65535, 1);
                            } else if (SoundType == 4) {
                                swprintf(opt, 179, L" %ls", L"Police");
                                //                                swprintf(opt, 179, L" %ls", L"Police");
                                drawText(opt, 50, 130, 100, 1, 0, 30.0f, 65535, 1);
                            } else if (SoundType == 5) {
                                swprintf(opt, 179, L" %ls", L"Door chime");
                                //                                swprintf(opt, 179, L" %ls", L"Door chime");
                                drawText(opt, 50, 135, 100, 1, 0, 20.0f, 65535, 1);
                            } else if (SoundType >= 6) {
                                swprintf(opt, 179, L" %ls", L"Beep");
                                //                                swprintf(opt, 179, L" %ls", L"Beep");
                                drawText(opt, 50, 130, 100, 1, 0, 30.0f, 65535, 1);
                            }
                        }
                    }
                }
                    break;
                case MultiSensor://NEWAL2
                {
                    switch (notifiedDevice->getLastNotificationIndex()) {
                        case 1:
                        {
                            if (MODE != DEVICE_MODE) {
                                fillRect(40, 130, 240, 150, RR_COLOR);
                                if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                    swprintf(opt, 179, L" %ls", lan.get("motiondetected", "connected_sensor"));
                                } else {
                                    swprintf(opt, 179, L" %ls", lan.get("nomotion", "connected_sensor"));
                                }
                                drawText(opt, 40, 130, 200, 1, 0, 25.0f, 65535, 1);
                            }
                        }
                            break;
                        case 2:
                        {

                            int batteryVal;
                            batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                            if (batteryVal != 0xff && batteryVal != 0) {
                                swprintf(opt, 5, L"%hd%%", batteryVal);
                                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                            } else {
                                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
                            }

                        }
                            break;
                        case 3:
                        {
                            if (MODE != DEVICE_MODE) {
                                fillRect(40, 155, 240, 175, RR_COLOR);
                                memset(opt, 0, sizeof (opt));
                                if (strcmp(notifiedDevice->getLastNotificationIndexName(), "ILLUMINANCE") == 0) {
                                    aswprintf(opt, 179, L"%ls : %s lux", lan.get("luminance", "connected_sensor"), notifiedDevice->getLastNotificationIndexValue());
                                } else
                                    aswprintf(opt, 179, L"%ls : %s %%", lan.get("luminance", "connected_sensor"), notifiedDevice->getLastNotificationIndexValue());
                                drawText(opt, 40, 155, 200, 1, 0, 25.0f, 65535, 1);
                            }
                        }
                            break;
                        case 4:
                        {
                            if (MODE != DEVICE_MODE) {
                                wchar_t arr[20];
                                //fillRect(200, 90, 300, 125, GREEN_COLOR);
                                fillRect(190, 95, 300, 125, GREEN_COLOR);
                                convertTemperature(4, arr);
                                //drawText(arr, 200, 93, 100, 1, 0, 30.0f, 65535, 1);
                                drawText(arr, 200, 100, 100, 1, 0, 25.0f, 65535, 1);
                            }
                        }
                            break;
                        case 5:
                        {
                            char ss[180];
                            if (MODE != DEVICE_MODE) {
                                fillRect(30, 180, 230, 220, RR_COLOR);
                                memset(opt, 0, sizeof (opt));
                                sprintf(ss, "%ls : %s", lan.get("humidity", "connected_sensor"), notifiedDevice->getLastNotificationIndexValue());
                                utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
                                drawText(opt, 30, 180, 200, 1, 0, 25.0f, 65535, 1);
                            }
                        }
                            break;
                    }
                }
                    break;
                case ZWtoACIRExtender://newal2 notification
                {
                    double er;
                    switch (notifiedDevice->getLastNotificationIndex()) {
                        case 1: //room temp
                        {
                            if (MODE != DEVICE_MODE) {
                                wchar_t arr[10];
                                memset(arr, 0, sizeof (arr));
                                fillRect(210, 95, 300, 125, GREEN_COLOR);
                                convertTemperature(1, arr);
                                drawText(arr, 200, 100, 100, 1, 0, 20.0f, 65535, 1);
                            }
                        }
                            break;
                        case 2: //AC modes
                        {
                            if (MODE != DEVICE_MODE) {
                                strcpy(thermoMode, notifiedDevice->getLastNotificationIndexValue());
                                fillRect(70, 160, 296, 184, RGB(23, 191, 227)); //newal2
                                drawLine(74, 171, 292, 171, 65535);
                                drawLine(74, 167, 74, 175, 65535);
                                drawLine(292, 167, 292, 175, 65535);
                                pngImg.drawPNG(70, 130, 1, 0, arrow_minus); //new zwtoac
                                pngImg.drawPNG(190, 130, 1, 0, arrow_plus);
                                fillRect(97, 130, 187, 155, BUTTON_LAYER);
                                aswprintf(opt, 179, L"%s", thermoMode);
                                if (!strcmp("Auto Changeover", thermoMode)) {
                                    aswprintf(opt, 179, L"%s", "Changeover");
                                    drawText(opt, 103, 135, 80, 1, 0, 15.0f, 65535, 1);
                                } else {
                                    drawText(opt, 105, 135, 80, 1, 0, 20.0f, 65535, 1);
                                }
                                pngImg.drawPNG(103, 132, 1, BACKGROUND_LAYER, Tick);

                                if (!strcmp(thermoMode, "Cool")) {
                                    AcIrFlag = 2;
                                    CheckIrMode = 2;
                                    fillCircle(74 + thermoCoolCurrLevel, 171, 7, 65535);
                                } else if (!strcmp(thermoMode, "Heat")) {
                                    AcIrFlag = 3;
                                    CheckIrMode = 3;
                                    fillCircle(74 + thermoHeatCurrLevel, 171, 7, 65535);
                                } else {
                                    if (!strcmp("Auto", thermoMode)) {
                                        AcIrFlag = 1;
                                        CheckIrMode = 1;
                                    } else if (!strcmp("Auto Changeover", thermoMode)) {
                                        AcIrFlag = 4;
                                        CheckIrMode = 4;
                                    } else if (!strcmp("Off", thermoMode)) {
                                        AcIrFlag = 5;
                                        CheckIrMode = 5;
                                    } else if (!strcmp("Resume", thermoMode)) {
                                        AcIrFlag = 6;
                                        CheckIrMode = 6;
                                    } else if (!strcmp("Fan Only", thermoMode)) {
                                        AcIrFlag = 7;
                                        CheckIrMode = 7;
                                    } else if (!strcmp("Dry Air", thermoMode)) {
                                        AcIrFlag = 8;
                                        CheckIrMode = 8;
                                    }
                                    drawTransparentRectangle(70, 160, 296, 184, WIDGET_CONTAINER_LAYER, 200);
                                }
                            }
                        }
                            break;
                        case 3: //heat temp
                        {
                            if (MODE != DEVICE_MODE) {
                                er = atof(notifiedDevice->getLastNotificationIndexValue());
                                er = (er - 32)*5.0 / 9;
                                thermoHeat = (int) ceil(er);
                                thermoHeatCurrLevel = (thermoHeat - 16)*20;
                                fillRect(70, 160, 296, 184, RGB(23, 191, 227)); //newal3
                                drawLine(74, 171, 292, 171, 65535);
                                drawLine(74, 167, 74, 175, 65535);
                                drawLine(292, 167, 292, 175, 65535);
                                fillCircle(78 + thermoHeatCurrLevel, 171, 7, 65535);
                            }
                        }
                            break;
                        case 4: //cool temp
                        {
                            if (MODE != DEVICE_MODE) {
                                er = atof(notifiedDevice->getLastNotificationIndexValue());
                                er = (er - 32)*5.0 / 9;
                                thermoCool = (int) ceil(er);
                                thermoCoolCurrLevel = (thermoCool - 16)*14;
                                fillRect(70, 160, 296, 184, RGB(23, 191, 227));
                                drawLine(74, 171, 292, 171, 65535);
                                drawLine(74, 167, 74, 175, 65535);
                                drawLine(292, 167, 292, 175, 65535);
                                fillCircle(78 + thermoCoolCurrLevel, 171, 7, 65535);
                            }
                        }
                            break;
                        case 5: //fan mode
                        {
                            if (MODE != DEVICE_MODE) {
                                memset(opt, 0, sizeof (opt));
                                strcpy(thermoFan, notifiedDevice->getLastNotificationIndexValue());
                                fillRect(97, 190, 187, 215, RGB(23, 191, 227));
                                if (!strcmp("Auto Low", thermoFan)) {
                                    strcpy(thermoFan, "Auto");
                                    Acflag = 1;
                                    CheckAcFan = 1;
                                } else if (!strcmp("On Low", thermoFan)) {
                                    strcpy(thermoFan, "Low");
                                    Acflag = 2;
                                    CheckAcFan = 2;
                                } else if (!strcmp("Unknown 5", thermoFan)) {
                                    strcpy(thermoFan, "Medium");
                                    Acflag = 3;
                                    CheckAcFan = 3;
                                } else if (!strcmp("On High", thermoFan)) {
                                    strcpy(thermoFan, "High");
                                    Acflag = 4;
                                    CheckAcFan = 4;
                                }
                                aswprintf(opt, 179, L"%s", thermoFan);
                                drawText(opt, 107, 195, 70, 1, 0, 20.0f, 65535, 1);
                                pngImg.drawPNG(100, 195, 1, BACKGROUND_LAYER, Tick);
                            }
                        }
                            break;
                        case 6: //battery
                        {
                            if (MODE != DEVICE_MODE) {
                                int batteryVal;
                                batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                                pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                                if (batteryVal != 0xff && batteryVal != 0) {
                                    swprintf(opt, 5, L"%hd%%", batteryVal);
                                    drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                                } else {
                                    drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                                }
                            }
                        }
                            break;
                        case 8://swing
                        {
                            if (MODE != DEVICE_MODE) {
                                fillRect(220, 130, 300, 155, RGB(23, 191, 227)); //swing 
                                if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 0) {
                                    drawText(L"Swing Off", 220, 135, 80, 1, 0, 20.0f, 65535, 1);
                                } else if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 1) {
                                    drawText(L"Swing On", 220, 135, 80, 1, 0, 20.0f, 65535, 1);
                                }
                            }
                        }
                            break;
                        case 9: //power
                        {
                            if (MODE != DEVICE_MODE) {
                                fillRect(70, 95, 115, 125, RGB(23, 191, 227));
                                if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 0) {
                                    drawText(lan.get("off_1", "common"), 70, 100, 50, 1, 0, 20.0f, 65535, 1);
                                } else {
                                    drawText(lan.get("on_1", "common"), 70, 100, 50, 1, 0, 20.0f, 65535, 1);
                                }
                            }
                        }
                            break;
                        case 10:
                        case 11: //Configuration
                        {
                            if (MODE == DEVICE_MODE) {
                                ConFlag = true;
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case SetPointThermostat:
                {
                    char ss[500];
                    wchar_t arr[10];
                    if (MODE != DEVICE_MODE) {//
                        if (notifiedDevice->getLastNotificationIndex() == 1 || notifiedDevice->getLastNotificationIndex() == 6) {
                            SetpointTmp = getTempinFloat(notifiedDevice->getLastNotificationIndexValue());
                            if (format == FORMAT_C) {
                                SetpointTmp = (SetpointTmp - 32)*5.0 / 9;
                                wcscpy(valunt, L"°C");
                            } else
                                wcscpy(valunt, L"°F");
                            SetpointTmp = ceil(SetpointTmp);
                            sprintf(ss, "   %.2lf", SetpointTmp);
                            memset(opt, 0, sizeof (opt));
                            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
                            wcscat(opt, L" ");
                            wcscat(opt, valunt);
                            TempBox.setText(opt);
                            TempBox.draw();

                        } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                            int batteryVal;
                            batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                            if (batteryVal != 0xff && batteryVal != 0) {
                                swprintf(opt, 5, L"%hd%%", batteryVal);
                                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                            } else {
                                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                            }
                        } else if (notifiedDevice->getLastNotificationIndex() == 4) {
                            fillRect(190, 95, 300, 125, GREEN_COLOR);
                            convertTemperature(4, arr);
                            drawText(arr, 200, 100, 100, 1, 0, 25.0f, 65535, 1);

                        } else if (notifiedDevice->getLastNotificationIndex() == 5) {
                            Device *d = getDevice(contDevID);
                            if (strcmp(d->getValue(5), "#ERR")) {
                                EconModeButton.draw();
                                HeatModeButton.draw();
                                if (!strcmp(d->getValue(5), "Heat Econ")) {
                                    pngImg.drawPNG(85, 195, 1, BACKGROUND_LAYER, Tick);
                                } else {
                                    pngImg.drawPNG(153, 195, 1, BACKGROUND_LAYER, Tick);
                                }
                            }
                            if (format == FORMAT_C) {
                                if (strcmp(d->getValue(5), "#ERR")) {
                                    if (!strcmp(d->getValue(5), "Heat Econ")) {
                                        SetpointTmp = (atoi(d->getValue(6)) - 32)*5.0 / 9;
                                    } else {
                                        SetpointTmp = (atoi(d->getValue(1)) - 32)*5.0 / 9;
                                    }
                                } else {
                                    SetpointTmp = (atoi(d->getValue(1)) - 32)*5.0 / 9;
                                }
                                SetpointTmp = ceil(SetpointTmp);
                                sprintf(ss, "   %.2lf °C", SetpointTmp);
                            } else {
                                if (strcmp(d->getValue(5), "#ERR")) {
                                    if (!strcmp(d->getValue(5), "Heat Econ")) {
                                        SetpointTmp = getTempinFloat(d->getValue(6));
                                    } else {
                                        SetpointTmp = getTempinFloat(d->getValue(1));
                                    }
                                } else {
                                    SetpointTmp = getTempinFloat(d->getValue(1));
                                }
                                sprintf(ss, "   %.2lf °F", SetpointTmp);
                            }
                            memset(opt, 0, sizeof (opt)); //july15
                            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
                            TempBox.setText(opt);
                            TempBox.draw();
                        }//notif 5


                    }//device mode
                }
                    break;

                case RollerShutter://newal2
                {
                    if (MODE != DEVICE_MODE) {
                        if (notifiedDevice->getLastNotificationIndex() == 1) {
                            dimmerCurrLevel = atoi(notifiedDevice->getLastNotificationIndexValue());
                            fillRect(120, 100, 200, 125, RR_COLOR);
                            if (dimmerCurrLevel == 0) {
                                swprintf(opt, 179, L" %ls", lan.get("closed", "connected_sensor"));
                            } else if (dimmerCurrLevel == 99) {
                                swprintf(opt, 179, L" %ls", lan.get("open", "connected_sensor"));
                            } else {
                                swprintf(opt, 179, L" %ls", L"Processing");
                            }
                            if (dimmerCurrLevel != 254)
                                drawText(opt, 120, 100, 80, 1, 0, 25.0f, 65535, 1);
                            else
                                drawText(opt, 120, 105, 80, 1, 0, 15.0f, 65535, 1);


                        }
                    }
                }
                    break;

                case EnergyReader://newal2 notif
                {
                    char tmp_str[30];
                    if (MODE != DEVICE_MODE) {//

                        if (notifiedDevice->getLastNotificationIndex() == 1) {
                            int batteryVal;
                            batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                            if (batteryVal != 0xff && batteryVal != 0) {
                                swprintf(opt, 5, L"%hd%%", batteryVal);
                                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
                            } else {
                                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
                            }
                        } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                            strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                            fillRect(60, 90, 280, 115, RR_COLOR);
                            swprintf(opt, 179, L"%ls:%hs", lan.get("power", "connected_sensor"), tmp_str);
                            drawText(opt, 60, 90, 200, 1, 0, 25.0f, 65535, 1);
                        } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                            strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                            fillRect(60, 115, 280, 135, RR_COLOR);
                            swprintf(opt, 179, L"%ls:%hs", L"Energy", tmp_str);
                            drawText(opt, 60, 115, 200, 1, 0, 20.0f, 65535, 1);
                        } else if (notifiedDevice->getLastNotificationIndex() == 4) {
                            strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                            fillRect(20, 165, 105, 180, RGB(255, 255, 255));
                            swprintf(opt, 179, L"%hs", tmp_str);
                            drawText(opt, 15, 165, 100, 1, 0, 20.0f, 0, 1);
                        } else if (notifiedDevice->getLastNotificationIndex() == 5) {
                            strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                            fillRect(15, 185, 105, 200, RGB(255, 255, 255));
                            swprintf(opt, 179, L"%hs", tmp_str);
                            drawText(opt, 10, 185, 100, 1, 0, 18.0f, 0, 1);
                        } else if (notifiedDevice->getLastNotificationIndex() == 6) {
                            strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                            fillRect(117, 165, 212, 180, RGB(255, 255, 255));
                            swprintf(opt, 179, L"%hs", tmp_str);
                            drawText(opt, 115, 165, 100, 1, 0, 20.0f, 0, 1); //clamp2: power
                        } else if (notifiedDevice->getLastNotificationIndex() == 7) {
                            strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                            fillRect(117, 185, 212, 200, RGB(255, 255, 255));
                            swprintf(opt, 179, L"%hs", tmp_str);
                            drawText(opt, 115, 185, 100, 1, 0, 18.0f, 0, 1);
                        }
                    }
                }
                    break;
                case ZigbeeDoorLock:
                {
                    //                    printf("doorlock callback\n");
                    wchar_t ss[180];
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(NOTIFICATION_X, NOTIFICATION_Y - 5, 300, NOTIFICATION_Y + 20, RR_COLOR);
                        swprintf(ss, 179, L"%ls", lan.get("doorlock", "connected_sensor"));
                        if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 1) {
                            swprintf(opt, 179, L"%ls : %ls", ss, lan.get("locked", "connected_sensor"));
                        } else if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 2) {
                            swprintf(opt, 179, L" %ls : %ls", ss, lan.get("unlocked", "connected_sensor"));
                        } else {
                            swprintf(opt, 179, L" %ls : %ls", ss, lan.get("notfullylocked", "connected_sensor"));
                        }
                        drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 7, 300, 1, 0, 20.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() > 3) {
                        sprintf(t_temp, "ZigbeeDoorlock %d user pin new value:%s, User no:%d", notifiedDevice->getID(), notifiedDevice->getLastNotificationIndexValue(), (notifiedDevice->getLastNotificationIndex() - 3));
                        LOG(Logger::INFO, t_temp);
                        if (notifiedDevice->getLastNotificationIndex() == Userno + 3) {
                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "") == 0) {
                                strcpy(Userpin, "Unassigned");
                            } else
                                strcpy(Userpin, notifiedDevice->getLastNotificationIndexValue());
                            fillRect(105, 160, 225, 185, RR_COLOR);
                            drawText(L"********", 105, 160, 120, 1, 0, 25.0f, 65535, 0);
                            Modal mW;
                            readArea(0, 0, 319, 239);
                            mW.setLine1(L"Requested Pin");
                            mW.setLine2(L"Operation is");
                            mW.setLine3(L"successful");
                            mW.modalReturn = &drawArea;
                            mW.draw(MODAL_TYPE_INFORMATION, 3);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        int batteryVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                        pngImg.drawPNG(243, 60, 1, 0, battery_icon);
                        swprintf(opt, 5, L"%hd%%", batteryVal);
                        drawText(opt, 243, 62, 40, 1, 0, 12.0f, RGB(0, 0, 0), 1);
                    }
                }
                    break;
                case OccupancySensor:
                {
                    char tmp_str[30] = {0};
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                        fillRect(50, 90, 270, 130, RR_COLOR);
                        swprintf(opt, 190, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? L"Unoccupied" : L"Occupied");
                        drawText(opt, 60, 95, 190, 1, 0, 32.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        fillRect(140, 130, 240, 165, GREEN_COLOR);
                        convertTemperature(2, opt);
                        drawText(opt, 140, 133, 100, 1, 0, 30.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                        fillRect(165, 167, 218, 200, RR_COLOR);
                        swprintf(opt, 110, L"%s%%", notifiedDevice->getLastNotificationIndexValue());
                        drawText(opt, 165, 170, 60, 1, 0, 27.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 4) {
                        if (strcmp(notifiedDevice->getValue(4), "true") == 0) {
                            pngImg.drawPNG(90, 200, 1, BACKGROUND_LAYER, battery_c);
                            drawText(lan.get("battery_low", "connected_sensor"), 110, 200, 110, 1, 0, 20.0f, 65535, 1);
                            LOG(Logger::INFO, "Battery low state received for occupancy sensor");
                        } else {
                            fillRect(90, 200, 210, 220, RR_COLOR);
                            LOG(Logger::INFO, "Battery state is changed from low to high for occupancy sensor");
                        }
                    }
                }
                    break;
                case ZigbeeTempSensor:
                {
                    switch (notifiedDevice->getIndexType(notifiedDevice->getLastNotificationIndex())) {
                        case Temperature:
                        {
                            fillRect(180, 105, 260, 135, GREEN_COLOR);
                            memset(opt, 0, sizeof (opt));
                            convertTemperature(notifiedDevice->getLastNotificationIndex(), opt);
                            drawText(opt, 185, 110, 70, 1, 0, 20.0f, 65535, 1);
                        }
                            break;
                        case Humidity:
                        {
                            swprintf(opt, 20, L"%4s%%     ", notifiedDevice->getLastNotificationIndexValue()); //°
                            drawTextWithBG(opt, 185, 145, 70, 1, 0, 20.0f, 65535, RR_COLOR, 0);
                        }
                            break;
                    }
                }
                    break;
                case SecurifiButton:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(NOTIFICATION_X, NOTIFICATION_Y + 10, 300, NOTIFICATION_Y + 49, RR_COLOR);
                        if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 0) {
                            drawText(L"Two Press", NOTIFICATION_X, NOTIFICATION_Y + 10, 300, 1, 0, 35.0f, 65535, 1);
                        } else if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 2) {
                            drawText(L"Long Press", NOTIFICATION_X, NOTIFICATION_Y + 10, 300, 1, 0, 35.0f, 65535, 1);
                        } else if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 3) {
                            drawText(L"One Press", NOTIFICATION_X, NOTIFICATION_Y + 10, 300, 1, 0, 35.0f, 65535, 1);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {

                        Device d(contDevID);
                        if (strcmp(d.getValue(2), "true") == 0) {
                            pngImg.drawPNG(100, NOTIFICATION_Y + 50, 1, BACKGROUND_LAYER, battery_c);
                            drawText(lan.get("battery_low", "connected_sensor"), NOTIFICATION_X + 15, NOTIFICATION_Y + 53, 285, 1, 0, 20.0f, 65535, 1);
                        } else {
                            fillRect(NOTIFICATION_X, NOTIFICATION_Y + 50, 300, NOTIFICATION_Y + 80, RR_COLOR);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            TamperButton.draw();
                        } else {
                            fillRect(15, 190, 95, 220, RR_COLOR);
                        }
                    }
                }
                    break;
                case GenericDevice:
                {
                    drawGenericDevice(notifiedDevice->getID(), notifiedDevice->getLastNotificationIndex(), 1);
                    sprintf(t_temp, "Updating index %d value of GenericDevice", notifiedDevice->getLastNotificationIndex());
                    LOG(Logger::INFO, t_temp);
                    gen.x_present = gen.y_present = gen.y_prev23 = gen.countswitches = 0;
                }
                    break;
                case HueLamp:
                {
                    Device d(contDevID);
                    float val;
                    if (MODE != COLOR_MODE) {
                        if (notifiedDevice->getLastNotificationIndex() == 2) {
                            char tmp_str[30];
                            dimmerCurrLevel = atoi(notifiedDevice->getValue(5));
                            strcpy(tmp_str, notifiedDevice->getValue(2));
                            val = ((float) (dimmerCurrLevel * 100) / (float) 255);
                            fillRect(NOTIFICATION_X, NOTIFICATION_Y - 15, 309, NOTIFICATION_Y + 5, RR_COLOR);
                            swprintf(opt, 179, L"%ls (%d%%)", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"), roundfun(val));
                            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 15, 300, 1, 0, 18.0f, 65535, 1);

                        } else if (notifiedDevice->getLastNotificationIndex() == 5) {
                            char tmp_str[30];
                            strcpy(tmp_str, notifiedDevice->getValue(2));
                            dimmerCurrLevel = atoi(notifiedDevice->getValue(5));
                            val = ((float) (dimmerCurrLevel * 100) / (float) 255);
                            fillRect(NOTIFICATION_X, NOTIFICATION_Y - 15, 309, NOTIFICATION_Y + 5, RR_COLOR);
                            swprintf(opt, 179, L"%ls (%d%%)", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"), roundfun(val));
                            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 15, 300, 1, 0, 18.0f, 65535, 1);
                            fillRect(25, RECT_Y, 301, RECT_Y + RECT_HEIGHT, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2), 301 - 5, RECT_Y + (RECT_HEIGHT / 2), 65535);
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 3), 25 + 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
                            drawLine(301 - 5, RECT_Y + (RECT_HEIGHT / 3), 301 - 5, RECT_Y + ((RECT_HEIGHT * 2) / 3), 65535);
                            fillCircle(25 + 10 + dimmerCurrLevel, RECT_Y + (RECT_HEIGHT / 2), 5, 65535);

                        }
                    } else if (MODE == COLOR_MODE) {
                        if (notifiedDevice->getLastNotificationIndex() == 4) {
                            float value;
                            int sat_v = atoi(notifiedDevice->getValue(4));
                            fillRect(10, 90, 310, 110, RR_COLOR);
                            value = ((sat_v * 100.0) / 255);
                            swprintf(opt, 179, L"Sat - %d%%", roundfun(value));
                            drawText(opt, NOTIFICATION_X, NOTIFICATION_Y - 15, 300, 1, 0, 18.0f, 65535, 1);
                            fillRect(25, RECT_Y, 300, RECT_Y + RECT_HEIGHT + 5, RGB(23, 191, 227));
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 2) + 2, 301 - 5, RECT_Y + (RECT_HEIGHT / 2) + 2, 65535);
                            drawLine(25 + 5, RECT_Y + (RECT_HEIGHT / 3) + 1, 25 + 5, RECT_Y + ((RECT_HEIGHT * 2) / 3) + 1, 65535);
                            drawLine(301 - 5, RECT_Y + (RECT_HEIGHT / 3), 301 - 5, RECT_Y + ((RECT_HEIGHT * 2) / 3) + 1, 65535);
                            fillCircle(35 + sat_v, RECT_Y + (RECT_HEIGHT / 2) + 2, 5, 65535);
                        }
                    }
                }
                    break;
                case NestThermostat:
                {
                    wchar_t arr[50];
                    double er;
                    char arr1[50];
                    strcpy(thermoMode, notifiedDevice->getValue(2));
                    strcpy(thermounit, notifiedDevice->getValue(7));
                    strcpy(thermoState, notifiedDevice->getValue(11));

                    if (notifiedDevice->getLastNotificationIndex() == 2) {
                        fillRect(55, 97, 123, 125, (strcmp(thermoMode, "heat-cool")) ? BUTTON_LAYER : RGB(23, 191, 227));
                        fillRect(127, 97, 177, 125, (strcmp(thermoMode, "Cool")) ? BUTTON_LAYER : RGB(23, 191, 227));
                        fillRect(181, 97, 231, 125, (strcmp(thermoMode, "Heat")) ? BUTTON_LAYER : RGB(23, 191, 227));
                        drawText(L"Heat-Cool", 70, 105, 50, 1, 0, 13.0f, 65535, 1);
                        drawText(lan.get("cool", "connected_sensor"), 132, 105, 50, 1, 0, 13.0f, 65535, 1);
                        drawText(lan.get("heat", "connected_sensor"), 186, 105, 50, 1, 0, 13.0f, 65535, 1);
                        strcpy(thermoMode, notifiedDevice->getValue(2));
                        if (!strcmp(thermoMode, "heat-cool")) {
                            pngImg.drawPNG(57, 105, 1, BACKGROUND_LAYER, Tick);
                        } else if (!strcmp(thermoMode, "cool")) {
                            pngImg.drawPNG(129, 105, 1, BACKGROUND_LAYER, Tick);
                        } else if (!strcmp(thermoMode, "heat")) {
                            pngImg.drawPNG(183, 105, 1, BACKGROUND_LAYER, Tick);
                        }
                        er = getTempinFloat(notifiedDevice->getValue(3));
                        thermoCool = (int) er;
                        if (thermoCool < 50) {
                            thermoCool = 50;
                        } else if (thermoCool > 90) {
                            thermoCool = 90;
                        }
                        thermoCoolCurrLevel = (thermoCool - 50)*6;
                        fillRect(THERMO_COOL_X - 5, THERMO_COOL_Y, 303, 155, RGB(23, 191, 227));
                        drawLine(THERMO_COOL_X, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 298, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 65535);
                        drawLine(THERMO_COOL_X, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
                        drawLine(THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
                        fillRect(THERMO_HEAT_X - 5, THERMO_HEAT_Y, THERMO_HEAT_X + THERMO_HEAT_WIDTH + 43, THERMO_HEAT_Y + THERMO_HEAT_HEIGHT, RGB(23, 191, 227));
                        drawLine(THERMO_HEAT_X, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 65535);
                        drawLine(THERMO_HEAT_X, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
                        drawLine(THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
                        fillRect(10, 135, 50, 155, RR_COLOR);
                        fillRect(10, 165, 50, 190, RR_COLOR);
                        if (!strcmp(thermoMode, "cool")) {
                            drawText(lan.get("cool", "connected_sensor"), 15, 135, 35, 1, 0, 16.0f, 65535, 0);
                            drawText(lan.get("heat", "connected_sensor"), 15, 165, 35, 1, 0, 16.0f, 65535, 0);
                            fillCircle(THERMO_COOL_X + thermoCoolCurrLevel, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);
                            //drawTransparentRectangle(95, 185, 423 + 8, 235, 0, 100);
                            drawTransparentRectangle(55, 160, 303, 185, 0, 100);
                        } else if (!strcmp(thermoMode, "heat")) {
                            drawText(lan.get("cool", "connected_sensor"), 15, 135, 35, 1, 0, 16.0f, 65535, 0);
                            drawText(lan.get("heat", "connected_sensor"), 15, 165, 35, 1, 0, 16.0f, 65535, 0);
                            fillCircle(THERMO_HEAT_X + thermoCoolCurrLevel, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);
                            drawTransparentRectangle(55, 130, 303, 155, 0, 100);
                            //drawTransparentRectangle(95, 185, 423 + 8, 235, 0, 100);
                        } else if (!strcmp(thermoMode, "heat-cool")) {
                            drawText(L"Low", 15, 135, 35, 1, 0, 16.0f, 65535, 0);
                            drawText(L"High", 15, 165, 35, 1, 0, 16.0f, 65535, 0);
                            er = getTempinFloat(notifiedDevice->getValue(5));
                            thermoCool = (int) er;
                            if (thermoCool < 50) {
                                thermoCool = 50;
                            } else if (thermoCool > 90) {
                                thermoCool = 90;
                            }
                            thermoCoolCurrLevel = (thermoCool - 50) * 6;
                            fillCircle(THERMO_COOL_X + thermoCoolCurrLevel, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);
                            er = getTempinFloat(notifiedDevice->getValue(6));
                            thermoHeat = (int) er;
                            if (thermoHeat < 50) {
                                thermoHeat = 50;
                            } else if (thermoHeat > 90) {
                                thermoHeat = 90;
                            }
                            thermoHeatCurrLevel = (thermoHeat - 50) * 6;
                            fillCircle(THERMO_HEAT_X + thermoHeatCurrLevel, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);
                        } else if (!strcmp(thermoMode, "off")) {
                        }
                        if (strcmp(notifiedDevice->getValue(12), "true")) {
                            drawTransparentRectangle(55, 130, 303, 155, 0, 100);
                            drawTransparentRectangle(127, 97, 177, 125, 0, 100);
                            drawTransparentRectangle(55, 97, 125, 125, 0, 100);
                        }
                        if (strcmp(notifiedDevice->getValue(13), "true")) {
                            drawTransparentRectangle(55, 160, 303, 185, 0, 100);
                            drawTransparentRectangle(181, 97, 231, 125, 0, 100);
                            drawTransparentRectangle(55, 97, 125, 125, 0, 100);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                        //printf("\nLINE = %d\n", __LINE__);
                        er = getTempinFloat(notifiedDevice->getValue(3));
                        thermoCurrent = (int) er;
                        if (thermoCurrent < 50) {
                            thermoCurrent = 50;
                        } else if (thermoCurrent > 90) {
                            thermoCurrent = 90;
                        }
                        thermoCoolCurrLevel = (int) (thermoCurrent - 50)*6;
                        if (!strcmp(thermoMode, "cool")) {
                            //printf("\nLINE = %d\n", __LINE__);
                            fillRect(THERMO_COOL_X - 5, THERMO_COOL_Y, 303, 155, RGB(23, 191, 227));
                            drawLine(THERMO_COOL_X, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 298, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 65535);
                            drawLine(THERMO_COOL_X, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
                            drawLine(THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
                            fillCircle(THERMO_COOL_X + thermoCoolCurrLevel, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);
                        } else if (!strcmp(thermoMode, "heat")) {
                            //printf("\nLINE = %d\n", __LINE__);
                            fillRect(THERMO_HEAT_X - 5, THERMO_HEAT_Y, THERMO_HEAT_X + THERMO_HEAT_WIDTH + 43, THERMO_HEAT_Y + THERMO_HEAT_HEIGHT, RGB(23, 191, 227));
                            drawLine(THERMO_HEAT_X, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 65535);
                            drawLine(THERMO_HEAT_X, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
                            drawLine(THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
                            fillCircle(THERMO_HEAT_X + thermoCoolCurrLevel, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 4) {
                        fillRect(221, 190, 303, 220, RR_COLOR);
                        pngImg.drawPNG(225, 195, 1, BACKGROUND_LAYER, humidityImage);
                        swprintf(opt, 100, L" : %s %%", notifiedDevice->getValue(4));
                        drawText(opt, 250, 198, 60, 1, 0, 18.0f, 65535, 0);
                    } else if (notifiedDevice->getLastNotificationIndex() == 5) {
                        if (strcmp(thermoMode, "heat")) {
                            er = getTempinFloat(notifiedDevice->getValue(5));
                            thermoCool = (int) er;
                            if (thermoCool < 50) {
                                thermoCool = 50;
                            } else if (thermoCool > 90) {
                                thermoCool = 90;
                            }
                            thermoCoolCurrLevel = (thermoCool - 50)*6;
                            fillRect(THERMO_COOL_X - 5, THERMO_COOL_Y, 303, 155, RGB(23, 191, 227));
                            drawLine(THERMO_COOL_X, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 298, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 65535);
                            drawLine(THERMO_COOL_X, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
                            drawLine(THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 3), THERMO_COOL_X + THERMO_COOL_WIDTH - 5 + 43, THERMO_COOL_Y + ((THERMO_COOL_HEIGHT * 2) / 3), 65535);
                            fillCircle(THERMO_COOL_X + thermoCoolCurrLevel, THERMO_COOL_Y + (THERMO_COOL_HEIGHT / 2), 5, 65535);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 6) {
                        if (strcmp(thermoMode, "cool")) {
                            er = getTempinFloat(notifiedDevice->getValue(6));
                            thermoHeat = (int) er;
                            if (thermoHeat < 50) {
                                thermoHeat = 50;
                            } else if (thermoHeat > 90) {
                                thermoHeat = 90;
                            }
                            thermoHeatCurrLevel = (thermoHeat - 50)*6;
                            fillRect(THERMO_HEAT_X - 5, THERMO_HEAT_Y, THERMO_HEAT_X + THERMO_HEAT_WIDTH + 43, THERMO_HEAT_Y + THERMO_HEAT_HEIGHT, RGB(23, 191, 227));
                            drawLine(THERMO_HEAT_X, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 65535);
                            drawLine(THERMO_HEAT_X, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
                            drawLine(THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 3), THERMO_HEAT_X + THERMO_HEAT_WIDTH - 5 + 43, THERMO_HEAT_Y + ((THERMO_HEAT_HEIGHT * 2) / 3), 65535);
                            fillCircle(THERMO_HEAT_X + thermoHeatCurrLevel, THERMO_HEAT_Y + (THERMO_HEAT_HEIGHT / 2), 5, 65535);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 8) {
                        strcpy(thermoaway, notifiedDevice->getValue(8));
                        fillRect(THERMO_HEAT_X - 5, 190, 136, 220, BUTTON_LAYER);
                        if (!strcmp(thermoaway, "away")) {
                            drawText(L"Away", THERMO_HEAT_X + 5, 198, 50, 1, 0, 18.0f, 65535, 1);
                            drawTransparentRectangle(55, 97, 125, 125, 0, 100);
                            drawTransparentRectangle(127, 97, 177, 125, 0, 100);
                            drawTransparentRectangle(181, 97, 231, 125, 0, 100);
                            drawTransparentRectangle(55, 130, 303, 155, 0, 100);
                            drawTransparentRectangle(55, 160, 303, 185, 0, 100);
                            drawTransparentRectangle(138, 190, 219, 220, 0, 100);
                        } else if (!strcmp(thermoaway, "home")) {
                            drawText(L"Home", THERMO_HEAT_X + 5, 198, 50, 1, 0, 18.0f, 65535, 1);
                            drawControlDevice((Device) notifiedDevice->getID());
                        } else if (!strcmp(thermoaway, "auto-away")) {
                            drawText(L"Auto-Away", THERMO_HEAT_X + 4, 198, 65, 1, 0, 15.0f, 65535, 0);
                            drawTransparentRectangle(55, 97, 125, 125, 0, 100);
                            drawTransparentRectangle(127, 97, 177, 125, 0, 100);
                            drawTransparentRectangle(181, 97, 231, 125, 0, 100);
                            drawTransparentRectangle(55, 130, 303, 155, 0, 100);
                            drawTransparentRectangle(55, 160, 303, 185, 0, 100);
                            drawTransparentRectangle(138, 190, 219, 220, 0, 100);
                        } else if (!strcmp(thermoaway, "unknown")) {
                            drawText(L"Unknown", THERMO_HEAT_X + 5, 198, 50, 1, 0, 18.0f, 65535, 1);
                        }

                    } else if (notifiedDevice->getLastNotificationIndex() == 9) {
                        fillRect(138, 190, 219, 220, BUTTON_LAYER);
                        pngImg.drawPNG(143, 195, 1, BACKGROUND_LAYER, fanImage);
                        sprintf(arr1, "%s", notifiedDevice->getValue(9));
                        swprintf(opt, 190, L" : %ls", (arr1[0] == 'f' || arr1[0] == 'F') ? L"Stop" : L"Start");
                        drawText(opt, 165, 196, 60, 1, 0, 18.0f, 65535, 0);
                    } else if (notifiedDevice->getLastNotificationIndex() == 10) {
                        fillRect(235, 97, 303, 125, GREEN_COLOR);
                        memset(arr, 0, sizeof (arr));
                        convertTemperature(10, arr);
                        drawText(arr, 245, 97 + 6, 55, 1, 0, 18.0f, 65535, 0);
                    } else if (notifiedDevice->getLastNotificationIndex() == 11) {
                        if (!strcmp(thermoState, "true")) {
                            fillRect(235, 97, 303, 125, GREEN_COLOR);
                            convertTemperature(10, arr);
                            drawText(arr, 245, 97 + 6, 55, 1, 0, 18.0f, 65535, 0);
                        } else {
                            fillRect(235, 97, 303, 125, RGB(255, 0, 0));
                            drawText(L"Offline", 245, 97 + 6, 60, 1, 0, 18.0f, 65535, 0);
                        }
                    } else if (notifiedDevice->getLastNotificationIndex() == 12) {
                        drawControlDevice((Device) notifiedDevice->getID());
                    } else if (notifiedDevice->getLastNotificationIndex() == 13) {
                        drawControlDevice((Device) notifiedDevice->getID());
                    } else if (notifiedDevice->getLastNotificationIndex() == 14) {
                        drawTransparentRectangle(55, 97, 123, 125, 0, 100);
                        drawTransparentRectangle(127, 97, 177, 125, 0, 100);
                        drawTransparentRectangle(181, 97, 231, 125, 0, 100);
                    } else if (notifiedDevice->getLastNotificationIndex() == 15) {
                        drawControlDevice((Device) notifiedDevice->getID());
                    } else if (notifiedDevice->getLastNotificationIndex() == 18) {
                        int check = 0;
                        Modal mW;
                        check = atoi(notifiedDevice->getLastNotificationIndexValue());
                        ///LOG(Logger::INFO, "ddddddd");
                        LOG(Logger::INFO, notifiedDevice->getLastNotificationIndexValue());

                        readArea(0, 0, 319, 239);
                        switch (check) {
                            case -1:
                                mW.setLine1(L"Wrong Mode");
                                mW.setLine2(L"selected");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -2:
                            case -4:
                                mW.setLine1(L"Device is in");
                                mW.setLine2(L"Away Mode");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -3:
                                mW.setLine1(L"Thermostat");
                                mW.setLine2(L"is Offline");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -5:
                                mW.setLine1(L"Low temperature");
                                mW.setLine2(L"can't be more than");
                                mW.setLine3(L"High Temperature");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -6:
                                mW.setLine1(L"Can't change HVAC");
                                mW.setLine2(L"mode while lock");
                                mW.setLine3(L"is enabled");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -7:
                                break;
                            case -8:
                                break;
                            case -9:
                                mW.setLine1(L"Target temperature");
                                mW.setLine2(L"is lower than");
                                mW.setLine3(L"the range");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -10:
                                mW.setLine1(L"Target temperature");
                                mW.setLine2(L"is higher than");
                                mW.setLine3(L"the range");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -11:
                                mW.setLine1(L"Target temperature");
                                mW.setLine2(L"is lower than the");
                                mW.setLine3(L"lock temperature");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -12:
                                mW.setLine1(L"Target temperature");
                                mW.setLine2(L"is higher than the");
                                mW.setLine3(L"lock temperature");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -13:
                                mW.setLine1(L"Cannot change");
                                mW.setLine2(L"HVAC mode during");
                                mW.setLine3(L"energy-saving events");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -14:
                                mW.setLine1(L"Cannot change");
                                mW.setLine2(L"HVAC mode");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -15:
                                mW.setLine1(L"");
                                mW.setLine2(L"Invalid HVAC mode");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -16:
                                mW.setLine1(L"Cannot activate");
                                mW.setLine2(L"fan during smoke/co");
                                mW.setLine3(L"safety shutoff");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -17:
                                mW.setLine1(L"Fan Timer not set.");
                                mW.setLine2(L"You can set from");
                                mW.setLine3(L"Thermostat");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case -18:
                                mW.setLine1(L"");
                                mW.setLine2(L"Emergency Heat is On");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case 429:
                                mW.setLine1(L"");
                                mW.setLine2(L"Too many requests.");
                                mW.setLine3(L"Try after sometime");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            case 503:
                                mW.setLine1(L"Nest service");
                                mW.setLine2(L"is unavailable");
                                mW.modalReturn = &drawArea;
                                mW.draw(MODAL_TYPE_INFORMATION, 3);
                                break;
                            default:
                                break;
                        }
                        drawArea();
                    }
                }
                    break;
                case NestSmokeDetector:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 3) {
                        drawControlDevice((Device) notifiedDevice->getID());
                    }
                    if (notifiedDevice->getLastNotificationIndex() == 4) {
                        drawControlDevice((Device) notifiedDevice->getID());
                    }
                    if (notifiedDevice->getLastNotificationIndex() == 5) {
                        drawControlDevice((Device) notifiedDevice->getID());
                    }
                    if (notifiedDevice->getLastNotificationIndex() == 2) {
                        drawControlDevice((Device) notifiedDevice->getID());
                    }
                }
                    break;
                case AlmondSiren:
                {
                    /*if((notifiedDevice->getLastNotificationIndex() == 1) && (MODE == ADVANCE_SIREN_MODE)) {
                        if(strcmp(notifiedDevice->getLastNotificationIndexValue(), "false") == 0) {
                           fillRect(100, 95, 200, 120, RGB(23, 191, 227));
                           drawText(L"Enable Siren", 100, 101, 100, 1, 0, 18.0f, 65535, 1);
                           EnabledSiren = false;
                           LOG(Logger::INFO, "AlmondSiren disabled");
                        } else {
                           fillRect(100, 95, 200, 120, RGB(23, 191, 227));
                           drawText(L"Disable Siren", 100, 101, 100, 1, 0, 18.0f, 65535, 1);
                           EnabledSiren = true;
                            LOG(Logger::INFO, "AlmondSiren enabled");
                        }
                    } else*/
                    if (MODE == CONTROL_MODE) {
                        if (notifiedDevice->getLastNotificationIndex() == 2) {
                            fillRect(20, 185, 100, 215, RGB(23, 191, 227));
                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                fillRect(110, 185, 165, 215, RR_COLOR);
                                drawText(L"Stop", 20, 191, 80, 1, 0, 18.0f, 65535, 1);
                                StartSiren = true;
                            } else {
                                drawText(L"Ring", 20, 191, 80, 1, 0, 18.0f, 65535, 1);
                                drawText(L"for", 110, 195, 60, 1, 0, 20.0f, 65535, 1);
                                StartSiren = false;
                            }
                        } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                            checkRadioButtonState(atoi(notifiedDevice->getLastNotificationIndexValue()), TONE_SECTION);
                        } else if (notifiedDevice->getLastNotificationIndex() == 4) {
                            checkRadioButtonState(atoi(notifiedDevice->getLastNotificationIndexValue()), VOLUME_SECTION);
                        } else if (notifiedDevice->getLastNotificationIndex() == 5) {
                            swprintf(As_opt, 179, L"%s", notifiedDevice->getLastNotificationIndexValue());
                            AlmondTimer.setText(As_opt);
                        }
                    }
                }
                    break;

                case AlmondBlink:
                {

                    if ((notifiedDevice->getLastNotificationIndex() == 1) && (MODE == CONTROL_MODE)) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "false") == 0) {
                            fillRect(25, 95, 125, 125, RGB(23, 191, 227));
                            drawText(L"Turn On", 25, 101, 100, 1, 0, 18.0f, 65535, 1);
                            StartLED = true;
                        } else {
                            fillRect(25, 95, 125, 125, RGB(23, 191, 227));
                            drawText(L"Turn Off", 25, 101, 100, 1, 0, 18.0f, 65535, 1);
                            StartLED = false;//false
                        }
                    } else if ((notifiedDevice->getLastNotificationIndex() == 2) && (MODE == CONTROL_MODE)) {
                        fillRect(129, 190, 171, 220, RR_COLOR);
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                            fillRect(25, 190, 125, 220, RGB(23, 191, 227));
                            drawText(L"Stop Blinking", 25, 196, 100, 1, 0, 18.0f, 65535, 1);
                            StartBlinkLED = false;
                        } else {
                            fillRect(25, 190, 125, 220, RGB(23, 191, 227));
                            drawText(L"Blink", 25, 196, 100, 1, 0, 18.0f, 65535, 1);
                            drawText(L"for", 130, 200, 40, 1, 0, 20.0f, 65535, 1);
                            StartBlinkLED = true;
                        }
                    } else if ((notifiedDevice->getLastNotificationIndex() == 3)) {
                        double bright = 0.0;
                        //                        unsigned short int hue = 0;
                        presentColor = atoi(notifiedDevice->getLastNotificationIndexValue());
                        Ledhue = convertRGBtoHSV(presentColor, &bright, &LedSatur);
                        swprintf(opt, 179, L"%0.0f %ls", (bright * 100), L"%");
                        if (MODE == BLINK_COLOR_MODE) {
                            fillRect(130, 95, 190, 135, RR_COLOR);
                            drawText(opt, 130, 95, 60, 1, 0, 25.0f, 65535, 1);
                        } else {
                            fillRect(165, 145, 225, 175, RR_COLOR);
                            drawText(opt, 170, 150, 50, 1, 0, 20.0f, 65535, 1);
                        }
                    } else if ((notifiedDevice->getLastNotificationIndex() == 4) && (MODE == CONTROL_MODE)) {
                        swprintf(As_opt, 179, L"%s", notifiedDevice->getLastNotificationIndexValue());
                        AlmondLedTimer.setText(As_opt);
                    }
                }
                    break;

                case MultiSwitch:
                {
                    char tmp_str[50] = {0};
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        fillRect(255, 100, 300, 130, RR_COLOR);
                        strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                        swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
                        drawText(opt, NOTIFICATION_X + 245, NOTIFICATION_Y, 50, 1, 0, 25.0f, 65535, 1);
                    } else if (notifiedDevice->getLastNotificationIndex() == 2) {
                        fillRect(255, 150, 300, 180, RR_COLOR);
                        strcpy(tmp_str, notifiedDevice->getLastNotificationIndexValue());
                        swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
                        drawText(opt, NOTIFICATION_X + 245, NOTIFICATION_Y + 50, 50, 1, 0, 25.0f, 65535, 1);
                    }
                }
                    break;
                case ZBIRExtender:
                {
                    if (MODE == CONTROL_MODE || MODE == EDIT_APPLIANCE_NAME) {
                        swprintf(opt, 179, L"%s", notifiedDevice->getLastNotificationIndexValue());
                        if (notifiedDevice->getLastNotificationIndex() == 2) {
                            ApplianceOne.setText(opt);
                            ApplianceOne.draw();
                        } else if (notifiedDevice->getLastNotificationIndex() == 3) {
                            ApplianceTwo.setText(opt);
                            ApplianceTwo.draw();
                        } else if (notifiedDevice->getLastNotificationIndex() == 4) {
                            ApplianceThree.setText(opt);
                            ApplianceThree.draw();
                        } else if (notifiedDevice->getLastNotificationIndex() == 5) {
                            ApplianceFour.setText(opt);
                            ApplianceFour.draw();
                        } else if (notifiedDevice->getLastNotificationIndex() == 6) {
                            ApplianceFive.setText(opt);
                            ApplianceFive.draw();
                        } else if (notifiedDevice->getLastNotificationIndex() == 7) {
                            ApplianceSix.setText(opt);
                            ApplianceSix.draw();
                        }
                    } else if (MODE == IR_OPERATION_MODE || MODE == EDIT_BUTTON_NAME) {
                        if ((notifiedDevice->getLastNotificationIndex() == 9) && ((irApplianceSelected + 8) == 9)) {
                            setAndDrawIrEventButton(notifiedDevice->getLastNotificationIndexValue());
                        } else if ((notifiedDevice->getLastNotificationIndex() == 10) && ((irApplianceSelected + 8) == 10)) {
                            setAndDrawIrEventButton(notifiedDevice->getLastNotificationIndexValue());
                        } else if ((notifiedDevice->getLastNotificationIndex() == 11) && ((irApplianceSelected + 8) == 11)) {
                            setAndDrawIrEventButton(notifiedDevice->getLastNotificationIndexValue());
                        } else if ((notifiedDevice->getLastNotificationIndex() == 12) && ((irApplianceSelected + 8) == 12)) {
                            setAndDrawIrEventButton(notifiedDevice->getLastNotificationIndexValue());
                        } else if ((notifiedDevice->getLastNotificationIndex() == 13) && ((irApplianceSelected + 8) == 13)) {
                            setAndDrawIrEventButton(notifiedDevice->getLastNotificationIndexValue());
                        } else if ((notifiedDevice->getLastNotificationIndex() == 14) && ((irApplianceSelected + 8) == 14)) {
                            setAndDrawIrEventButton(notifiedDevice->getLastNotificationIndexValue());
                        }
                    } else if ((MODE == IR_LED_SELECTION_MODE) && (notifiedDevice->getLastNotificationIndex() == 8)) {
                        char data[30] = "", temp[10] = "0x";
                        unsigned char selectedLed = 0;
                        strcpy(data, notifiedDevice->getLastNotificationIndexValue());

                        strncpy(temp + 2, data + (irApplianceSelected * 2), sizeof (char)*2);
                        selectedLed = (unsigned char) strtol(temp, NULL, 16);

                        if (selectedLed & 0x01) {
                            IRLed1->setSelected(1);
                            appled[irApplianceSelected - 1] |= 0x01;
                        } else {                           
                            IRLed1->setSelected(0);
                            appled[irApplianceSelected - 1] &= 0xFE;
                        }
                        IRLed1->draw();

                        if (selectedLed & 0x02) {
                            appled[irApplianceSelected - 1] |= 0x02;
                            IRLed2->setSelected(1);
                        } else {
                            IRLed2->setSelected(0);
                            appled[irApplianceSelected - 1] &= 0xFD;
                        }
                        IRLed2->draw();

                        if (selectedLed & 0x04) {
                            IRLed3->setSelected(1);
                            appled[irApplianceSelected - 1] |= 0x04;
                        } else {
                            IRLed3->setSelected(0);
                            appled[irApplianceSelected - 1] &= 0xFB;
                        }
                        IRLed3->draw();

                        if (selectedLed & 0x08) {
                            IRLed4->setSelected(1);
                            appled[irApplianceSelected - 1] |= 0x08;
                        } else {
                            IRLed4->setSelected(0);
                            appled[irApplianceSelected - 1] &= 0xF7;
                        }
                        IRLed4->draw();

                        if (selectedLed & 0x10) {
                            IRLed5->setSelected(1);
                            appled[irApplianceSelected - 1] |= 0x10;
                        } else {
                            IRLed5->setSelected(0);
                            appled[irApplianceSelected - 1] &= 0xEF;
                        }
                        IRLed5->draw();

                        if (selectedLed & 0x20) {
                            IRLed6->setSelected(1);
                            appled[irApplianceSelected - 1] |= 0x20;
                        } else {
                            IRLed6->setSelected(0);
                            appled[irApplianceSelected - 1] &= 0xDF;
                        }
                        IRLed6->draw();
                    }
                }
                    break;

                case ZigbeeCamImage:
                {
                    if (notifiedDevice->getLastNotificationIndex() == 6) {
                        swprintf(btnImageCount.W.text, 179, L"%d", atoi(notifiedDevice->getLastNotificationIndexValue()));
                        btnImageCount.draw();
                    } else if (notifiedDevice->getLastNotificationIndex() == 7) {
                        fillRect(130, 210, 215, 225, RR_COLOR);
                        if (atoi(notifiedDevice->getLastNotificationIndexValue()) == 1) {
                            wcscpy(btnResolution.W.text, L"VGA");
                            drawText(L"640 X 480 pixel", 130, 210, 90, 1, 0, 15.0f, 65535, 0);
                        } else {
                            wcscpy(btnResolution.W.text, L"QVGA");
                            drawText(L"320 X 240 pixel", 130, 210, 90, 1, 0, 15.0f, 65535, 0);
                        }
                        btnResolution.draw();
                    }
                }
                    break;
                 
                default:
                    //                    printf("call back:unknown device\n");
                    if (notifiedDevice->getLastNotificationIndex() == 1) {
                        basicVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                        swprintf(opt, 5, L"%d", basicVal);
                        basicGetTextBox.setText(opt);
                    } else {
                        basicVal = atoi(notifiedDevice->getLastNotificationIndexValue());
                        swprintf(opt, 5, L"%d", basicVal);
                        basicSetText.setText(opt);
                        basicGetTextBox.setText(opt);
                    }
                    break; //Unknown Device Type";
            }
            char arr[50];
            DevtypeEnumToStr(contDev, arr);
            sprintf(t_temp, "%s %d index %d value is %s ", arr, notifiedDevice->getID(), notifiedDevice->getLastNotificationIndex(), notifiedDevice->getLastNotificationIndexValue());
            LOG(Logger::INFO, t_temp);
        } else if (notifiedDevice->getID() && (notifiedDevice->getDeviceType() == 5)) {

            return;
        } else if ((notifiedDevice->getDeviceType() == 50) && (atoi(notifiedDevice->getLastNotificationIndexValue()) == 149)) {
            for (list<peanut_plug>::iterator iterator = peanut_plug_info.begin(); iterator != peanut_plug_info.end(); ++iterator) {
                if ((iterator->S_Dev_ID == notifiedDevice->getID())) {
                    iterator->S_state = false;
                    iterator->percentage = 0;
                    ota_upgrade_flag = false;
                    current_update = true;
                    failed_deviceId = iterator->S_Dev_ID;
                    //                    sprintf(t_temp, "***found failed device id:%d***", failed_deviceId);
                    //                    LOG(Logger::INFO,t_temp);
                    u_val = 0;
                    per = 0;
                }
            }
        }
    }
}

void saveVal(int num) {
    // printf("Value Saved n1\n");
    if (valueToSave == 1) {
        mem.saveValues();
        valueToSave = 0;
    }
}
//   printf("Value Saved n2\n");

int checkUpdates(void) {
    char url[180];
    updates_available = 0;
    FILE *fp;
    int cnt = 0;
    if (system("rm /ota-files/*.ota") == 0)
        LOG(Logger::INFO, "Ota files deleted");
    sprintf(url, "%sversion", SERVER_LOCATION);
    int sop = downloadFile(url, "version");
    if (sop == 0) {
        fp = fopen("version", "r");
        if (fp == NULL) {
            LOG(Logger::INFO, "File isn't found or failed to open");
            updates_available = 0;
            return (0);
        }
        fgets(nVer, 512, fp);
        fclose(fp);
        system("rm version");
        sprintf(t_temp, "New version found is:%s", nVer);
        LOG(Logger::INFO, t_temp);
        cnt = strlen(nVer);
        nVer[cnt] = '\0';
        char temp[4];
        for (int cnt2 = 0; cnt2 < 4; cnt2++)
            temp[cnt2] = nVer[11 + cnt2];
        temp[3] = '\0';
        new_version = atoi(temp);
        sprintf(t_temp, "New version:%s OldVer=%d NewVer=%d", nVer, old_version, new_version);
        LOG(Logger::INFO, t_temp);
        if (old_version < new_version) {
            updates_available = 1;
            return 1;
        }
    }
    LOG(Logger::INFO, "Upgrade operation terminates abnormally");
    updates_available = 0;
    return 0;
}

int checkConnection(void) {
    readArea(0, 0, 319, 239);
    Modal mW;
    mW.setLine1(lan.get("checkingl1", "settings"));
    mW.setLine2(lan.get("checkingl2", "settings"));
    mW.setLine3(lan.get("checkingl3", "settings"));
    mW.modalReturn = &doNothing;
    mW.draw(MODAL_TYPE_INFORMATION, 2);

    return 1;
}

void Advancebutton_onTouch(void) {

    MODE = RETAIN_MODE;
    Device *d = getDevice(contDevID);
    fillRect(10, 90, 310, 230, RR_COLOR);
    LOG(Logger::INFO, "Advance button on touch");
    if (d) {
        if ((strtol(d->getValue(14), NULL, 16)) == 0) {
            RetainButton.setSelected(1);
        } else {
            RetainButton.setSelected(0);
        }
    } else {
        char bufLog[200] = "";
        sprintf(bufLog, "Device %d doesn't exists to get retain state ", contDevID);
        LOG(Logger::ERROR, bufLog);
    }
    RetainButton.draw();
    checkforupgradeButton.W.isEnabled = 0;
    drawText(L"Retain State", 130, 135, 150, 1, 0, 20.0f, 65535, 0);
    for (list<peanut_plug>::iterator iterator = peanut_plug_info.begin(); iterator != peanut_plug_info.end(); ++iterator) {
        //        sprintf(t_temp, "++++Device:%d  iterator:%d++++",d->getID(), iterator->S_Dev_ID);
        //        LOG(Logger::INFO, t_temp);
        if ((iterator->S_Dev_ID == d->getID()) && iterator->S_state) {
            if (iterator->percentage >= 0) {
                fillRect(10, 93, 310, 120, RR_COLOR);
                swprintf(opt, 179, L"%ls %d%%", L"Upgrading", iterator->percentage);
                drawText(opt, 10, 93, 300, 1, 0, 30.0f, 65535, 1);
                checkforupgradeButton.W.isEnabled = -1;
                checkforupgradeButton.draw();
                break;
            }
        } else if ((failed_deviceId == s_devid) && (s_devid == d->getID())) {
            fillRect(10, 93, 310, 120, RR_COLOR);
            drawText(L"Upgrade Failed", 10, 93, 300, 1, 0, 30.0f, 65535, 1);
            checkforupgradeButton.W.isEnabled = -1;
            checkforupgradeButton.draw();
            current_update = false;
            failed_deviceId = 0;
            s_devid = 0;
        } else {
            if (!ota_upgrade_flag && !iterator->S_state) {
                checkforupgradeButton.W.isEnabled = 0;
                checkforupgradeButton.draw();
            } else {
                wchar_t text[180] = {0};
                fillRect(10, 90, 310, 120, RR_COLOR);
                swprintf(text, 179, L"%ls %d", L"Wait till the switch", s_devid);
                swprintf(opt, 179, L"%ls %ls", text, L"gets upgraded");
                drawText(opt, 10, 97, 300, 1, 0, 20.0f, 65535, 1);
                checkforupgradeButton.W.isEnabled = -1;
                checkforupgradeButton.draw();
            }
        }
    }

}

void RetainButton_onTouch(void) {
    Device *d = getDevice(contDevID);
    if (d) {
        if ((strtol(d->getValue(14), NULL, 16)) == 0) {
            d->Retainstate(false);
        } else {

            d->Retainstate(true);
        }
    } else {
        char bufLog[200] = "";
        sprintf(bufLog, "Device %d doesn't exists to get retain state ", contDevID);
        LOG(Logger::ERROR, bufLog);
    }
}

void upgradeButton_onTouch() {

    for (list<peanut_plug>::iterator iterator = peanut_plug_info.begin(); iterator != peanut_plug_info.end(); ++iterator) {
        if ((iterator->S_Dev_ID == contDevID)) {
            Device *d = getDevice(contDevID);
            if (d) {
                LOG(Logger::INFO, "get current and next version \n");
                d->getCurrentandNextVersionofSecurifiswitch();
            } else {
                char bufLog[200] = "";
                sprintf(bufLog, "Device %d doesn't exists to get upgrade button ", contDevID);
                LOG(Logger::ERROR, bufLog);
            }
            if (checkConnection()) {
                sprintf(t_temp, "connected to internet\n");
                LOG(Logger::INFO, t_temp);
                checkUpdates();
            }
            drawArea();
            if (updates_available != 1) {
                fillRect(10, 90, 310, 120, RR_COLOR);
                drawText(L"Switch firmware is up to date", 10, 93, 300, 1, 0, 25.0f, 65535, 1);
            } else {
                if (!updates_available) {
                    readArea(0, 0, 319, 239);
                    Modal mW;
                    mW.setLine1(L"No updates available.");
                    mW.setLine2(L"Your software is");
                    mW.setLine3(L"up-to-date.");
                    mW.modalReturn = &doNothing;
                    mW.draw(MODAL_TYPE_INFORMATION, -1);
                    drawArea();
                    return;
                }
                updatetouchflag = true;
                fillRect(10, 90, 310, 120, RR_COLOR);
                drawText(L"A New firmware is available", 10, 93, 300, 1, 0, 25.0f, 65535, 1);
                upgradeButton.W.isEnabled = 0;
                upgradeButton.draw();
            }
        }
    }


}

void upgrade(void) {
    MODE = UPGRADE_MODE;
    char tempFileName[100], url[180], filename[180];
    Device *d = getDevice(contDevID);
    if (updates_available != 1) {
    } else {
        readArea(0, 0, 319, 239);
        Modal mW;
        mW.setLine1(L"Please wait");
        mW.setLine2(L"Do not unplug");
        mW.modalReturn = &doNothing;
        mW.draw(MODAL_TYPE_INFORMATION, 0);
        snprintf(tempFileName, 19, "%s", nVer);
        tempFileName[20] = '\0';

        sprintf(url, "%s%s", SERVER_LOCATION, tempFileName);
        sprintf(filename, "/ota-files/%s", tempFileName);
        int sop = downloadFile(url, filename);
        sleep(2);
        for (list<peanut_plug>::iterator iterator = peanut_plug_info.begin(); iterator != peanut_plug_info.end(); ++iterator) {
            if (iterator->S_Dev_ID == d->getID()) {
                iterator->S_state = true;
                if (!sop) {
                    if (d) {
                        LOG(Logger::INFO, "upgrading switch FW");
                        d->upgardeswitchversion();
                    } else {
                        char bufLog[200] = "";
                        sprintf(bufLog, "Device %d doesn't exists to upgrade switch ", contDevID);
                        LOG(Logger::ERROR, bufLog);
                    }
                }
                break;
            }
        }
    }
}

int getColorFromScreen(int x, int y) {
    int ret;
    screenData.data = (int*) malloc(2 * 2 * sizeof (int));
    screenData.x0 = x;
    screenData.y0 = y;
    screenData.width = 2;
    screenData.height = 2;
    ioctl(filed, READ_LCD_AREA, &screenData);
    ret = screenData.data[0];
    free(screenData.data);

    return ret;
}

int checkifImageAlreadyLoaded(char *image, int *indx) {

    if (strcmp(image, "/almond/unknown.png") == 0) {
        IMAGELOAD(1);
    } else if (strcmp(image, "/almond/ON-OFF.png") == 0) {
        IMAGELOAD(2);
    } else if (strcmp(image, "/almond/Light_bulb.png") == 0) {
        IMAGELOAD(3);
    } else if (strcmp(image, "/almond/door_window_sensor.png") == 0) {
        IMAGELOAD(4);
    } else if (strcmp(image, "/almond/door_lock.png") == 0) {
        IMAGELOAD(5);
    } else if (strcmp(image, "/almond/Alarm.png") == 0) {
        IMAGELOAD(6);
    } else if (strcmp(image, "/almond/thermostat.png") == 0) {
        IMAGELOAD(7);
    } else if (strcmp(image, "/almond/shade.png") == 0) {
        IMAGELOAD(8);
    } else if (strcmp(image, "/almond/motion.png") == 0) {
        IMAGELOAD(9);
    } else if (strcmp(image, "/almond/door_sensor.png") == 0) {
        IMAGELOAD(10);
    } else if (strcmp(image, "/almond/fire_sensor.png") == 0) {
        IMAGELOAD(11);
    } else if (strcmp(image, "/almond/water_sensor.png") == 0) {
        IMAGELOAD(12);
    } else if (strcmp(image, "/almond/key_fob.png") == 0) {
        IMAGELOAD(13);
    } else if (strcmp(image, "/almond/Light_sensor.png") == 0) {
        IMAGELOAD(14);
    } else if (strcmp(image, "/almond/temperature_sensor.png") == 0) {
        IMAGELOAD(15);
    } else if (strcmp(image, "/almond/roller_shutter.png") == 0) {
        IMAGELOAD(16);
    } else if (strcmp(image, "/almond/nest_protect_icon_25x25.png") == 0) {
        IMAGELOAD(17);
    }
}

/*bool TextValidation(wchar_t* keyboardText) {
    sprintf(Time, "%ls", keyboardText);
    Modal mW;
    if (!(atoi(Time) >= 0L && atoi(Time) < 65536L)) {
        mW.setLine1(lan.get("modelmw", "connected_sensor"));
        mW.modalReturn = &doNothing;
        mW.draw(MODAL_TYPE_INFORMATION, -1);
        return false;
    }
    return true;
}*/

void TimeBoxOnTouch(void) {
    // setKeyboardValidationFunction(TextValidation);
    MODE = KEYBOARD_MODE;
    change = CHANGE_TIME;
    readArea(0, 0, 319, 239);
    kbMode = true;
    kb3->noCommit = true;
    kb3->keyboardMode(NUMBER_ONLY);
    kb3->drawText(Time, N_opt);
}

void PinboxonTouch(void) {
    Device *d = getDevice(contDevID);
    MODE = KEYBOARD_MODE;
    kbMode = true;

    if (!strcmp(Userpin, "Unassigned"))
        strcpy(Userpin, "");
    readArea(0, 0, 319, 239);
    if (d && d->getDeviceType() == GenericDevice)
        change = CHANGE_PIN_GENERIC;
    else
        change = CHANGE_PIN;
    kb4->LT = 8;
    kb4->GT = 4;
    kb4->noCommit = true;
    kb4->keyboardMode(NUMBER_ONLY);
    kb4->drawText(Userpin, N_opt);
}

void UserNoBackButton_onTouch(void) {

    Device *d = getDevice(contDevID);
    if (d) {
        if (Userno > 1) {
            Userno--;
            fillRect(186, 128, 220, 154, RR_COLOR);
            swprintf(opt, 179, L"%d", Userno);
            drawText(opt, 185, 130, 35, 1, 0, 22.0f, 65535, 1);
            if (d->getDeviceTechnology() == ZWaveTechnology) {
                if (!strcmp(d->getValue(Userno + 4), ""))
                    sprintf(Userpin, "%ls", lan.get("unassigned", "connected_sensor"));
                else
                    strcpy(Userpin, d->getValue(Userno + 4));
                fillRect(105, 160, 225, 185, RR_COLOR);
                aswprintf(opt, 179, L"%s", Userpin);
                drawText(opt, 105, 160, 120, 1, 0, 25.0f, 65535, 0);
            } else {
                if (!strcmp(d->getValue(Userno + 3), ""))
                    sprintf(Userpin, "%ls", lan.get("unassigned", "connected_sensor"));
                else
                    strcpy(Userpin, d->getValue(Userno + 3));
                fillRect(105, 160, 225, 185, RR_COLOR);
                drawText(L"********", 105, 160, 120, 1, 0, 25.0f, 65535, 0);
            }
        }
    } else {
        char bufLog[200] = "";
        sprintf(bufLog, "Device %d doesn't exists in list to check", contDevID);
        LOG(Logger::ERROR, bufLog);
    }
}

void UserNoNextButton_onTouch(void) {
    Device *d = getDevice(contDevID);
    if (d) {
        if (d->getDeviceTechnology() == ZWaveTechnology) {
            if (Userno < atoi(d->getValue(4))) {
                Userno++;
                fillRect(186, 128, 220, 154, RR_COLOR);
                swprintf(opt, 179, L"%d", Userno);
                drawText(opt, 185, 130, 35, 1, 0, 22.0f, 65535, 1);
                //            		fillRect(168, 130, 188, 155, RR_COLOR);
                //            		swprintf(opt, 179, L"%d", Userno);
                //            		//drawText(opt, 270, 130, 35, 1, 0, 25.0f, 65535, 1);
                //            		drawText(opt, 160, 130, 35, 1, 0, 25.0f, 65535, 1);
                if (!strcmp(d->getValue(Userno + 4), ""))
                    sprintf(Userpin, "%ls", lan.get("unassigned", "connected_sensor"));
                else
                    strcpy(Userpin, d->getValue(Userno + 4));
                fillRect(105, 160, 225, 185, RR_COLOR);
                aswprintf(opt, 179, L"%s", Userpin);
                drawText(opt, 105, 160, 120, 1, 0, 25.0f, 65535, 0);
            }
        } else {
            if (Userno < atoi(d->getValue(3))) {
                Userno++;
                fillRect(186, 128, 220, 154, RR_COLOR);
                swprintf(opt, 179, L"%d", Userno);
                drawText(opt, 185, 130, 35, 1, 0, 22.0f, 65535, 1);
                if (!strcmp(d->getValue(Userno + 3), ""))
                    sprintf(Userpin, "%ls", lan.get("unassigned", "connected_sensor"));
                else
                    strcpy(Userpin, d->getValue(Userno + 3));
                fillRect(105, 160, 225, 185, RR_COLOR);
                drawText(L"********", 105, 160, 120, 1, 0, 25.0f, 65535, 0);
            }
        }
    } else {
        char bufLog[200] = "";
        sprintf(bufLog, "Device %d doesn't exists in list to check", contDevID);
        LOG(Logger::ERROR, bufLog);
    }
}

void ConfigParamNumericKbReturn(void) {//newal2
    MODE = DEVICE_MODE;
    drawArea();
    sprintf(ConfigSet, "%ls", ConfigParam_opt);
    if (strcmp(ConfigSet, "")) {
        if(!ParameterFlag){
        ParameterFlag = true;
        ValueFlag = false;
        } else if(!ValueFlag){
            ValueFlag = true;
            ConValFlag = true;
        }
        ConfigurationButton_onTouch();
    }
}

/*void ConfigValueNumericKbReturn(void) {//newal2
    MODE = DEVICE_MODE;
    drawArea();
    sprintf(ConfigSet, "%ls", ConfigParam_opt);
    if (!strcmp(ConfigSet, "")) {
        ValueFlag = true;
        ValueFlag = false;
    } else {
        ValueFlag = true;
        ConValFlag = true; //confignewlyadded
        swprintf(opt, 179, L"%s", ConfigParam);
        drawText(opt, 10, 150, 60, 1, 0, 20.0f, 0, 1);

        swprintf(opt, 10, L"%d", tempvaluesize);
        drawText(opt, 103, 150, 40, 1, 0, 20.0f, 0, 1);

        strcpy(ConfigValue, ConfigSet);
        swprintf(opt, 179, L"%s", ConfigValue);
        drawText(opt, 176, 150, 40, 1, 0, 24.0f, 0, 1);
    }
}*/

void moreGeneric_onTouch(void) {

    int ValueCount = 0;
    Device *dev = getDevice(contDevID);
    MODE = GENERIC_MODE;
    bzero(&gen, sizeof (gen));
    bzero(grid1, sizeof (grid1));
    bzero(grid2, sizeof (grid2));
    bzero(grid3, sizeof (grid3));
    gen.x_next = X1_CORDT;
    gen.y_next = Y1_CORDT;
    gen.ROW1 = gen.ROW2 = gen.ROW3 = 1;

    fillRect(10, 90, 305, 225, RR_COLOR);
    ValueCount = dev->getValueCount();
    StartIndxCnt = in; //newgeneric
    for (in; in <= ValueCount; in++) {
        if ((drawGenericDevice(contDevID, in, 0)) == FULL_PAGE)
            break;
    }
    bzero(&gen, sizeof (gen));
    //gen.ROW1 = gen.ROW2 = gen.ROW3 = 1;
    more = gen.previous2 = gen.previous3 = false;
}

float getRequiredIndexArea(unsigned int indenum) {
    sprintf(t_temp, "Assigning area to the IndexType:%d", indenum);
    LOG(Logger::INFO, t_temp);

    if ((indenum == SwitchBinary) || (indenum == SwitchMultilevel) || (indenum == LockStateZb) || (indenum == DimmerLevel) || (indenum == SwitchMultiSound) || (indenum == Power) || (indenum == Clamp1_power) || (indenum == Clamp2_power)) {
        return FULL_ROW;
    } else if ((indenum == ThermostatMode) || (indenum == ThermostatSetpointHeating) || (indenum == ThermostatSetpointCooling) || (indenum == AcMode) || (indenum == AcFanMode) || (indenum == Energy) || (indenum == MeteringCurrent)) {
        return FULL_ROW;
    } else if ((indenum == HueZb) || (indenum == Saturation) || (indenum == ColorTemperature) || (indenum == MaximumUsers) || (indenum == ThermostatSetpoint) || (indenum == ShutterLevel) || (indenum == Clamp1_energy) || (indenum == Clamp2_energy)) {
        return FULL_ROW;
    } else if ((indenum == AlarmState) || (indenum == LockState) || (indenum == BarrierOperator) || (indenum == AcSetpointHeating) || (indenum == AcSetpointCooling) || (indenum == WindowCoveringMode)) {
        return FULL_ROW;
    } else if ((indenum == SmokeState) || (indenum == ThermostatFanMode) || (indenum == MotionState) || (indenum == AcSwing) || (indenum == IRcode)) {
        return TWO_3ROW;
    } else if ((indenum == DoorState) || (indenum == VibrationState) || (indenum == WaterState) || (indenum == GasState) || (indenum == UpDown)|| (indenum == GestureSensor)) {
        return TWO_3ROW;
    } else if ((indenum == SensorBinary) || (indenum == FireState) || (indenum == FloodState) || (indenum == Illuminance) || (indenum == Armmode)) { 
        return TWO_3ROW;
    } else if ((indenum == Temperature) || (indenum == Pressure) || (indenum == Humidity) || (indenum == Stop)) {
        return ONE_3ROW;
    } else if ((indenum == APower) || (indenum == RmsVoltage) || (indenum == RmsCurrent) || (indenum == BasicExtender)) {
        return ONE_3ROW;
    } else if ((indenum == Battery) || (indenum == LowBattery) || (indenum == Tamper) || (indenum == PanicAlarm) || (indenum == EmerAlarm)) {
        return 2.0f;
    } else {
        return 0;
    }
}

int getGenericDrawingCoordinates(float iarea) {

    gen.p_area += iarea;
    if (gen.p_area > 3.00f) {
        MoreButton.draw();
        more = true;
        gen.p_area = 0;
        return FULL_PAGE;
    }

    if (iarea == FULL_ROW) { /****************coordinates for full row*********************/
        gen.x_present = gen.x_next;
        gen.y_present = gen.y_next;
        if (gen.x_present != X1_CORDT) {
            if (gen.y_present == Y1_CORDT) {
                gen.y_present = Y2_CORDT;
                gen.ROW1 = 1;
                gen.ROW2 = 0;
            } else if (gen.y_present == Y2_CORDT) {
                gen.y_present = Y3_CORDT;
                gen.previous2 = true;
                gen.previous3 = true;
                gen.ROW2 = 1;
                gen.ROW3 = 0;
            }

            if (gen.x_present == X3_CORDT) {
                gen.x_prev13 = gen.x_next;
                gen.y_prev13 = gen.y_next;
                gen.previous3 = true;
            } else {
                gen.x_prev23 = gen.x_next;
                gen.y_prev23 = gen.y_next;
                gen.x_prev13 = gen.x_next;
                gen.y_prev13 = gen.y_next;
                gen.previous3 = true;
                gen.previous2 = true;
            }
            gen.x_present = X1_CORDT;
            gen.y_next = gen.y_present;
            gen.x_next = X1_CORDT;
            gen.y_next = (gen.y_next + Y_NEX);

            sprintf(t_temp, "full row x_present:%d y_present:%d x_previous3:%d y_previous3:%d ", gen.x_present, gen.y_present, gen.x_prev13, gen.y_prev13);
            LOG(Logger::INFO, t_temp);
            sprintf(t_temp, "full row x_previous2:%d y_previous2:%d x_next:%d y_next:%d ", gen.x_prev23, gen.y_prev23, gen.x_next, gen.y_next);
            LOG(Logger::INFO, t_temp);

        } else {
            gen.y_next = (gen.y_next + Y_NEX);
            gen.ROW1 = 0;

            sprintf(t_temp, "full row x_present:%d y_present:%d x_next:%d y_next:%d", gen.x_present, gen.y_present, gen.x_next, gen.y_next);
            LOG(Logger::INFO, t_temp);
        }
    } else if (iarea == TWO_3ROW) { /************************ two-third row**************************/
        if (gen.previous2 == true) {
            LOG(Logger::INFO, "Two-3 Previous Space found");
            gen.x_present = gen.x_prev23;
            gen.y_present = gen.y_prev23;
            gen.previous2 = false;
        } else {
            gen.x_present = gen.x_next;
            gen.y_present = gen.y_next;

        }

        if (gen.x_present == X3_CORDT) {
            if (gen.y_present == Y1_CORDT) {
                gen.y_present = Y2_CORDT;
                gen.ROW2 = 0;
            } else if (gen.y_present == Y2_CORDT) {
                gen.y_present = Y3_CORDT;
                gen.ROW3 = 0;
            }
            gen.x_present = X1_CORDT;
        }

        if (gen.y_present == Y1_CORDT) {
            if (gen.ROW2 != 0) {
                gen.y_next = Y2_CORDT;
                gen.ROW2 = 0;
            } else if (gen.ROW3 != 0) {
                gen.y_next = Y3_CORDT;
                gen.ROW3 = 0;
            }

            if (gen.x_present == X1_CORDT) {
                gen.x_prev13 = X3_CORDT;
                gen.y_prev13 = Y1_CORDT;
                gen.previous3 = true; //space for 1/3 row available
            }
        } else if (gen.y_present == Y2_CORDT) {
            if (gen.ROW3 != 0) {
                gen.y_next = Y3_CORDT;
                gen.ROW3 = 0;
            }

            if (gen.x_present == X1_CORDT) {
                gen.x_prev13 = X3_CORDT;
                gen.y_prev13 = Y2_CORDT;
                gen.previous3 = true; //space for 1/3 row available
            }
        } else if (gen.y_present == Y3_CORDT) {
            if (gen.x_present == X1_CORDT) {
                gen.x_prev13 = X3_CORDT;
                gen.y_prev13 = Y3_CORDT;
                gen.previous3 = true;
            }
        }
        gen.x_next = X1_CORDT;

        sprintf(t_temp, "Two-3 row x_present:%d y_present:%d x_previous3:%d y_previous3:%d", gen.x_present, gen.y_present, gen.x_prev13, gen.y_prev13);
        LOG(Logger::INFO, t_temp);
        sprintf(t_temp, "Two-3 row x_next:%d y_next:%d", gen.x_next, gen.y_next);
        LOG(Logger::INFO, t_temp);

    } else if (iarea == ONE_3ROW) { /*************************** one third row***************************/
        if (gen.previous3 == true) {
            LOG(Logger::INFO, "One-3 Previous Space found");
            gen.x_present = gen.x_prev13;
            gen.y_present = gen.y_prev13;
        } else {
            gen.x_present = gen.x_next;
            gen.y_present = gen.y_next;
        }

        if(gen.previous3 != true) {
        if ((gen.x_present == X3_CORDT) && (gen.y_present == Y1_CORDT)) {
            gen.x_next = X1_CORDT;
            gen.y_next = Y2_CORDT;
        } else if ((gen.x_present == X3_CORDT) && (gen.y_present == Y2_CORDT)) {
            gen.x_next = X1_CORDT;
            gen.y_next = Y3_CORDT;
        } else {
            gen.x_next = gen.x_present;
            gen.x_next = (gen.x_next + X_NEX);
        }
       } else {
           gen.previous3 = false;
       }
        sprintf(t_temp, "one-third x_present:%d y_present:%d x_next:%d y_next:%d", gen.x_present, gen.y_present, gen.x_next, gen.y_next);
        LOG(Logger::INFO, t_temp);
    }
}

void getDrawingCorditforCallback(float iarea, unsigned int indenm, int diff) {

    if (iarea == FULL_ROW) {
        if (diff == GENERIC_TOUCH || multiswitch == true) {
            if ((grid1[0] == indenm) && (gen.y_prev23 > 90 && gen.y_prev23 < 120)) {
                gen.x_present = X1_CORDT;
                gen.y_present = Y1_CORDT;
            } else if ((grid2[0] == indenm) && (gen.y_prev23 > 123 && gen.y_prev23 < 153)) {
                gen.x_present = X1_CORDT;
                gen.y_present = Y2_CORDT;
            } else if ((grid3[0] == indenm) && (gen.y_prev23 > 156 && gen.y_prev23 < 190)) {
                gen.x_present = X1_CORDT;
                gen.y_present = Y3_CORDT;
            }
        } else if (diff == GENERIC_CALLBACK) {
            if (grid1[0] == indenm)/* && (indexState == true )*/ {
                gen.x_present = X1_CORDT;
                gen.y_present = Y1_CORDT;
            } else if (grid2[0] == indenm) /*&& (indexState == true )*/ {
                gen.x_present = X1_CORDT;
                gen.y_present = Y2_CORDT;
            } else if (grid3[0] == indenm) /*&& (indexState == true )*/ {
                gen.x_present = X1_CORDT;
                gen.y_present = Y3_CORDT;
            }
        }
    } else if (iarea == TWO_3ROW) {
        if (grid1[0] == indenm) {
            gen.x_present = X1_CORDT;
            gen.y_present = Y1_CORDT;
        } else if (grid1[2] == indenm) {
            gen.x_present = X2_CORDT;
            gen.y_present = Y1_CORDT;
        } else if (grid2[0] == indenm) {
            gen.x_present = X1_CORDT;
            gen.y_present = Y2_CORDT;
        } else if (grid2[2] == indenm) {
            gen.x_present = X2_CORDT;
            gen.y_present = Y2_CORDT;
        } else if (grid3[0] == indenm) {
            gen.x_present = X1_CORDT;
            gen.y_present = Y3_CORDT;
        } else if (grid3[2] == indenm) {
            gen.x_present = X2_CORDT;
            gen.y_present = Y3_CORDT;
        }
    } else if (iarea == ONE_3ROW) {
        if (grid1[0] == indenm) {
            gen.x_present = X1_CORDT;
            gen.y_present = Y1_CORDT;
        } else if (grid1[1] == indenm) {
            gen.x_present = X2_CORDT;
            gen.y_present = Y1_CORDT;
        } else if (grid1[2] == indenm) {
            gen.x_present = X3_CORDT;
            gen.y_present = Y1_CORDT;
        } else if (grid2[0] == indenm) {
            gen.x_present = X1_CORDT;
            gen.y_present = Y2_CORDT;
        } else if (grid2[1] == indenm) {
            gen.x_present = X2_CORDT;
            gen.y_present = Y2_CORDT;
        } else if (grid2[2] == indenm) {
            gen.x_present = X3_CORDT;
            gen.y_present = Y2_CORDT;
        } else if (grid3[0] == indenm) {
            gen.x_present = X1_CORDT;
            gen.y_present = Y3_CORDT;
        } else if (grid3[1] == indenm) {
            gen.x_present = X2_CORDT;
            gen.y_present = Y3_CORDT;
        } else if (grid3[2] == indenm) {
            gen.x_present = X3_CORDT;
            gen.y_present = Y3_CORDT;
        }
    }
}

int drawGenericDevice(int devid, int index, int diff) {
    char ss[180];
    unsigned int indenum;
    char N_Index[20];
    float area = 0.0;

    Device *dev = getDevice(devid);
    if (!dev) {
        LOG(Logger::ERROR, "Device creation failed\nExiting...");
        return 0;
    }
    strcpy(N_Index, dev->getValueName(index));
    sprintf(t_temp, "GenericDevice %d index is %s indexNo:%d", dev->getID(), N_Index, index);
    LOG(Logger::INFO, t_temp);
    indenum = StrToIndexTypeEnum(N_Index);
    if ((area = getRequiredIndexArea(indenum)) == 0.0) {
        LOG(Logger::INFO, "Index area is not defined");
        return 0;
    }
    if (diff == 0 && area != 2.0f) { //function called from drawing control device
        if (getGenericDrawingCoordinates(area) == FULL_PAGE) {
            LOG(Logger::INFO, "Controlled Device Page is full");
            return FULL_PAGE;
        }

        sprintf(t_temp, "GenericDevice enum :%d area:%f X:%d Y:%d", indenum, area, gen.x_present, gen.y_present);
        LOG(Logger::INFO, t_temp);
        if (gen.y_present == Y1_CORDT) {
            if (area == FULL_ROW) {
                grid1[0] = indenum;
                grid1[1] = indenum;
                grid1[2] = indenum;
            } else if (area == TWO_3ROW) {
                if (gen.x_present == X1_CORDT) {
                    grid1[0] = indenum;
                    grid1[1] = indenum;
                    gen.ir1 += 2;
                } else {
                    grid1[1] = indenum;
                    grid1[2] = indenum;
                    gen.ir1 += 2;
                }
            } else {
                grid1[gen.ir1++] = indenum;
            }
        } else if (gen.y_present == Y2_CORDT) {
            LOG(Logger::INFO, "Found second row");
            if (area == FULL_ROW) {
                grid2[0] = indenum;
                grid2[1] = indenum;
                grid2[2] = indenum;
            } else if (area == TWO_3ROW) {
                if (gen.x_present == X1_CORDT) {
                    grid2[0] = indenum;
                    grid2[1] = indenum;
                    gen.jr2 += 2;
                } else {
                    grid2[1] = indenum;
                    grid2[2] = indenum;
                    gen.jr2 += 2;
                }
            } else {
                grid2[gen.jr2++] = indenum;
            }
        } else if (gen.y_present == Y3_CORDT) {
            LOG(Logger::INFO, "Found third row");
            if (area == FULL_ROW) {
                grid3[0] = indenum;
                grid3[1] = indenum;
                grid3[2] = indenum;
            } else if (area == TWO_3ROW) {
                if (gen.x_present == X1_CORDT) {
                    grid3[0] = indenum;
                    grid3[1] = indenum;
                    gen.kr3 += 2;
                } else {
                    grid3[1] = indenum;
                    grid3[2] = indenum;
                    gen.kr3 += 2;
                }
            } else {
                grid3[gen.kr3++] = indenum;
            }
        }
    } else {
        if (area != 2.0f) {
            if ((indenum == SwitchBinary) && (multiswitch == true)) {
                getEndpointforSwitchBinary(indenum, index, GENERIC_CALLBACK);
            }
            getDrawingCorditforCallback(area, indenum, GENERIC_CALLBACK);
        }
    }
    if ((gen.x_present == 0 || gen.y_present == 0) && area != 2.0f)
        return 0;

    sprintf(t_temp, "GenericDevice gd1:%u  gd1:%u  gd1:%u", grid1[0], grid1[1], grid1[2]);
    LOG(Logger::INFO, t_temp);
    sprintf(t_temp, "GenericDevice gd2:%u  gd2:%u  gd2:%u", grid2[0], grid2[1], grid2[2]);
    LOG(Logger::INFO, t_temp);
    sprintf(t_temp, "GenericDevice gd3:%u  gd3:%u  gd3:%u", grid3[0], grid3[1], grid3[2]);
    LOG(Logger::INFO, t_temp);
    memset(opt, 0, sizeof (opt)); 
    switch (indenum) {
        case SensorBinary:
        {
            char tempv[10];
            strcpy(tempv, dev->getValue(index));
            swprintf(opt, 179, L"%ls", (tempv[0] == 'f' || tempv[0] == 'F') ? lan.get("inactive", "connected_sensor") : lan.get("active", "connected_sensor"));
            fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 28, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
            sprintf(t_temp, "SensorBinary:GenericDevice %d state:%s UI:%ls ", dev->getID(), tempv, opt);
            LOG(Logger::INFO, t_temp);
        }
            break;
        case Battery:
        {
            int tempv;
            tempv = atoi(dev->getValue(index));
            pngImg.drawPNG(240, 60, 1, 0, battery_icon);
            if (tempv != 0xff && tempv != 0) {
                swprintf(opt, 5, L"%hd%%", tempv);
                drawText(opt, 245, 62, 60, 1, 0, 12.0f, RGB(0, 0, 0), 0);
            } else {
                drawText(lan.get("low", "connected_sensor"), 245, 62, 60, 1, 0, 10.0f, RGB(255, 0, 0), 0);
            }
            sprintf(t_temp, "Battery:GenericDevice %d state:%d", dev->getID(), tempv);
            LOG(Logger::INFO, t_temp);
        }
            break;
        case SmokeState:
        {
            int tempv;
            tempv = atoi(dev->getValue(index));
            swprintf(opt, 179, L"%ls", tempv == 255 ? lan.get("smokedetected", "connected_sensor") : lan.get("okay", "connected_sensor"));
            fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 28, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
            sprintf(t_temp, "SmokeState:GenericDevice %d state:%d UI:%ls ", dev->getID(), tempv, opt);
            LOG(Logger::INFO, t_temp);
        }
            break;
        case FloodState:
        {
            char tempv[10];
            strcpy(tempv, dev->getValue(index));
            swprintf(opt, 179, L"%ls", (tempv[0] == 'f' || tempv[0] == 'F') ? lan.get("okay", "connected_sensor") : lan.get("flooded", "connected_sensor"));
            fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 28, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
            sprintf(t_temp, "SmokeState:GenericDevice %d state:%S UI:%ls ", dev->getID(), tempv, opt);
        }
            break;
        case SwitchMultilevel:
        {
            dimmerCurrLevel = atoi(dev->getValue(index))*2;
            percentage = (((dimmerCurrLevel) * 100) / 200);
            swprintf(opt, 179, L"%d%%", percentage);
            fillRect(gen.x_present, gen.y_present, gen.x_present + 50, gen.y_present + 28, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present, 50, 1, 0, 20.0f, 65535, 1);
            fillRect(gen.x_present + 50, gen.y_present, gen.x_present + 270, gen.y_present + 29, RGB(23, 191, 227));
            drawLine(gen.x_present + 55, gen.y_present + 15, gen.x_present + 265, gen.y_present + 15, 65535);
            drawLine(gen.x_present + 55, gen.y_present + 7, gen.x_present + 55, gen.y_present + 23, 65535);
            drawLine(gen.x_present + 265, gen.y_present + 7, gen.x_present + 265, gen.y_present + 23, 65535);
            fillCircle(gen.x_present + 60 + dimmerCurrLevel, gen.y_present + 15, 5, 65535);
            sprintf(t_temp, "SwitchMultilevel:GenericDevice %d level is %d (%d%%)", dev->getID(), dimmerCurrLevel, percentage);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case SwitchMultiSound:
        {
            if (MODE != DEVICE_MODE) {
                fillRect(gen.x_present + 20, gen.y_present, gen.x_present + 160, gen.y_present + 28, RGB(3, 252, 36));
                int SoundType = atoi(dev->getValue("SWITCH_MULTISOUND"));
                if (SoundType == 0) {
                    swprintf(opt, 179, L" %ls", lan.get("alarm", "connected_sensor"));
                    drawText(opt, gen.x_present + 40, gen.y_present + 4, 100, 1, 0, 20.0f, 65535, 1);
                } else if (SoundType == 1) {
                    swprintf(opt, 179, L" %ls", lan.get("emergency", "connected_sensor"));
                    drawText(opt, gen.x_present + 40, gen.y_present + 4, 100, 1, 0, 20.0f, 65535, 1);
                } else if (SoundType == 2) {
                    swprintf(opt, 179, L" %ls", lan.get("fire", "connected_sensor"));
                    drawText(opt, gen.x_present + 40, gen.y_present + 4, 100, 1, 0, 27.0f, 65535, 1);
                } else if (SoundType == 3) {
                    swprintf(opt, 179, L" %ls", lan.get("ambulance", "connected_sensor"));
                    drawText(opt, gen.x_present + 40, gen.y_present + 4, 100, 1, 0, 20.0f, 65535, 1);
                } else if (SoundType == 4) {
                    swprintf(opt, 179, L" %ls", lan.get("police", "connected_sensor"));
                    drawText(opt, gen.x_present + 40, gen.y_present + 4, 100, 1, 0, 27.0f, 65535, 1);
                } else if (SoundType == 5) {
                    swprintf(opt, 179, L" %ls", lan.get("door_chime", "connected_sensor"));
                    drawText(opt, gen.x_present + 40, gen.y_present + 4, 100, 1, 0, 20.0f, 65535, 1);
                } else if (SoundType >= 6) {
                    swprintf(opt, 179, L" %ls", lan.get("beep", "connected_sensor"));
                    drawText(opt, gen.x_present + 40, gen.y_present + 2, 100, 1, 0, 27.0f, 65535, 1);
                }
                fillRect(gen.x_present + 170, gen.y_present, gen.x_present + 280, gen.y_present + 28, RGB(255, 51, 51));
                //                fillRect(gen.x_present + 170, gen.y_present, gen.x_present + 280, gen.y_present + 28, RGB(0, 255, 0));
                swprintf(opt, 179, L"%ls", lan.get("stop", "connected_sensor"));
                drawText(opt, gen.x_present + 180, gen.y_present + 2, 100, 1, 0, 27.0f, 65535, 1);
                indexState = true;
                readArea(0, 0, 479, 319);
                sprintf(t_temp, "SwitchMultiSound:GenericDevice %d SoundType is %d ", dev->getID(), SoundType);
                LOG(Logger::INFO, t_temp);
            }
        }
            break;

        case ShutterLevel:
        {
            fillRect(gen.x_present + 10, gen.y_present, gen.x_present + 90, gen.y_present + 28, RGB(23, 191, 227));
            sprintf(ss, "Open");
            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            drawText(opt, gen.x_present + 10, gen.y_present + 2, 80, 1, 0, 25.0f, 65535, 1);
            fillRect(gen.x_present + 100, gen.y_present, gen.x_present + 180, gen.y_present + 28, RGB(23, 191, 227));
            sprintf(ss, "Close");
            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            drawText(opt, gen.x_present + 100, gen.y_present + 2, 80, 1, 0, 25.0f, 65535, 1);
            dimmerCurrLevel = atoi(dev->getValue(index));
            if (dimmerCurrLevel == 0) {
                swprintf(opt, 179, L" %ls", lan.get("closed", "connected_sensor"));
            } else {
                swprintf(opt, 179, L" %ls", lan.get("open", "connected_sensor"));
            }
            fillRect(gen.x_present + 185, gen.y_present, gen.x_present + 280, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present + 190, gen.y_present + 4, 100, 1, 0, 26.0f, 65535, 1);
        }
            break;

        case BasicExtender:
        {
            int tmpValue;
            tmpValue = atoi(dev->getValue(index));
            fillRect(gen.x_present + 10, gen.y_present, gen.x_present + 90, gen.y_present + 29, BUTTON_LAYER);
            swprintf(opt, 179, L"%ls", tmpValue == 255 ? lan.get("on_1", "common") : lan.get("off_1", "common"));
            drawText(opt, gen.x_present, gen.y_present, 90, 1, 0, 30.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d BasicExtender: value is %d ", dev->getID(), tmpValue);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case LockState:
        {
            int lockState = atoi(dev->getValue(index));
            fillRect(gen.x_present, gen.y_present, gen.x_present + 100, gen.y_present + 30, RR_COLOR);
            if (lockState == 255) {
                aswprintf(opt, 179, L"%ls", lan.get("locked", "connected_sensor"));
                drawText(opt, gen.x_present, gen.y_present + 5, 90, 1, 0, 25.0f, 65535, 1);
            } else if (lockState == 23 || lockState == 26 || lockState == 17) {
                aswprintf(opt, 179, L"%ls", lan.get("notfullylocked", "connected_sensor"));
                drawText(opt, gen.x_present, gen.y_present + 7, 100, 1, 0, 16.0f, 65535, 1);
            } else {
                aswprintf(opt, 179, L"%ls", lan.get("unlocked", "connected_sensor"));
                drawText(opt, gen.x_present, gen.y_present + 5, 90, 1, 0, 25.0f, 65535, 1);
            }
            fillRect(gen.x_present + 110, gen.y_present, gen.x_present + 190, gen.y_present + 28, RGB(23, 191, 227));
            swprintf(opt, 179, L"%ls", lan.get("lock", "common"));
            drawText(opt, gen.x_present + 110, gen.y_present + 4, 80, 1, 0, 20.0f, 65535, 1);
            fillRect(gen.x_present + 200, gen.y_present, gen.x_present + 280, gen.y_present + 28, RGB(23, 191, 227));
            swprintf(opt, 179, L"%ls", lan.get("unlock", "common"));
            drawText(opt, gen.x_present + 200, gen.y_present + 5, 80, 1, 0, 20.0f, 65535, 1);
            //            indexState = true;
        }
            break;

        case MaximumUsers:
        {
            if (atoi(dev->getValue(index))) {
                userind = dev->getIndexNoFromName("USER_CODE", -1);
                drawText(lan.get("sett_pin", "connected_sensor"), gen.x_present + 5, gen.y_present + 3, 30, 1, 0, 20.0f, 65535, 0);
                pngImg.drawPNG(gen.x_present + 45, gen.y_present, 1, BACKGROUND_LAYER, arrow_minus);
                pngImg.drawPNG(gen.x_present + 105, gen.y_present, 1, BACKGROUND_LAYER, arrow_plus);
                swprintf(opt, 179, L"%d", Userno);
                drawText(opt, gen.x_present + 75, gen.y_present + 3, 20, 1, 0, 20.0f, 65535, 1);
                if (change == CHANGE_PIN_GENERIC) {
                    change = 0;
                    aswprintf(opt, 179, L"%s", Userpin);
                    drawText(opt, gen.x_present + 135, gen.y_present + 3, 100, 1, 0, 25.0f, 65535, 0);
                } else if (!strcmp(dev->getValue(Userno + (userind - 1)), "")) {
                    sprintf(Userpin, "%ls", lan.get("unassigned", "connected_sensor"));
                    aswprintf(opt, 179, L"%s", Userpin);
                    drawText(opt, gen.x_present + 135, gen.y_present + 3, 100, 1, 0, 20.0f, 65535, 0);
                } else {
                    aswprintf(opt, 179, L"%s", dev->getValue(Userno + userind - 1));
                    drawText(opt, gen.x_present + 135, gen.y_present + 3, 100, 1, 0, 25.0f, 65535, 0);
                }
                pngImg.drawPNG(gen.x_present + 235, gen.y_present + 3, 1, BACKGROUND_LAYER, pen_1);
                pngImg.drawPNG(gen.x_present + 265, gen.y_present + 3, 1, BACKGROUND_LAYER, trash_png);
                indexState = true;
                sprintf(t_temp, "GenericDevice user code pin value:%s userind: %d\n", Userpin, userind);
                LOG(Logger::PRINT, t_temp);
            }
        }
            break;
        case Illuminance:
        {
            int tempv;
            tempv = atoi(dev->getValue(index));
            sprintf(ss, " : %d Lux", tempv);
            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 28, RR_COLOR);
            pngImg.drawPNG(gen.x_present + 20, gen.y_present + 5, 1, BACKGROUND_LAYER, luminanceImage);
            drawText(opt, gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
            sprintf(t_temp, "Illuminance:GenericDevice %d value is %d ", dev->getID(), tempv);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case Clamp1_power:
        case Clamp2_power:
        case Power:
        {
            float tempv;
            tempv = atof(dev->getValue(index));
            if (indenum == Clamp1_power) {
                sprintf(ss, "Clamp:1 %ls : %.3f W", lan.get("power", "connected_sensor"), tempv);
            } else if (indenum == Clamp2_power) {
                sprintf(ss, "Clamp:2 %ls : %.3f W", lan.get("power", "connected_sensor"), tempv);
            } else {
                sprintf(ss, "%ls : %.3f W", lan.get("power", "connected_sensor"), tempv);
            }
            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            fillRect(gen.x_present, gen.y_present, gen.x_present + 280, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present + 5, gen.y_present + 6, 270, 1, 0, 24.0f, 65535, 0);
            sprintf(t_temp, "Power:GenericDevice %d value is %.3f indenum %d ", dev->getID(), tempv, indenum);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case Clamp1_energy:
        case Clamp2_energy:
        case Energy:
        {
            float tempv;
            tempv = atof(dev->getValue(index));
            if (indenum == Clamp1_energy) {
                sprintf(ss, "Clamp:1 %ls : %.3f W", L"Energy", tempv);
            } else if (indenum == Clamp2_energy) {
                sprintf(ss, "Clamp:2 %ls : %.3f W", L"Energy", tempv);
            } else {
                sprintf(ss, "%ls : %.3f W",L"Energy", tempv);
            }
            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            fillRect(gen.x_present, gen.y_present, gen.x_present + 280, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present + 5, gen.y_present + 6, 270, 1, 0, 24.0f, 65535, 0);
            sprintf(t_temp, "Energy: GenericDevice %d value is %.3f indenum %d ", dev->getID(), tempv, indenum);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case ThermostatSetpoint:
        {
            double er;
            char temp[10];
            er = getTempinFloat(dev->getValue(index));
            er = (er - 32)*5.0 / 9;
            thermoHeatCurrLevel = (int) (er * 5);
            sprintf(temp, "%.1lf", er);
            aswprintf(opt, 20, L"%hs °F", temp);
            drawText(opt, gen.x_present + 5, gen.y_present + 3, 55, 1, 0, 20.0f, 65535, 1);
            fillRect(gen.x_present + 65, gen.y_present, gen.x_present + 285, gen.y_present + 28, RGB(23, 191, 227));
            drawLine(gen.x_present + 70, gen.y_present + 15, gen.x_present + 280, gen.y_present + 15, 65535);
            drawLine(gen.x_present + 70, gen.y_present + 7, gen.x_present + 70, gen.y_present + 23, 65535);
            drawLine(gen.x_present + 280, gen.y_present + 7, gen.x_present + 280, gen.y_present + 23, 65535);
            fillCircle(gen.x_present + 75 + thermoHeatCurrLevel, gen.y_present + 15, 5, 65535);
            indexState = true;
            sprintf(t_temp, " GenericDevice %d ThermostatSetpoint %d", dev->getID(), thermoHeatCurrLevel);
            LOG(Logger::INFO, t_temp);

        }
            break;

        case UpDown:
        {
            fillRect(gen.x_present + 10, gen.y_present, gen.x_present + 90, gen.y_present + 28, RGB(23, 191, 227));
            sprintf(ss, "Up");
            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            drawText(opt, gen.x_present + 5, gen.y_present + 3, 80, 1, 0, 25.0f, 65535, 1);
            fillRect(gen.x_present + 100, gen.y_present, gen.x_present + 180, gen.y_present + 28, RGB(23, 191, 227));
            sprintf(ss, "Down");
            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            drawText(opt, gen.x_present + 100, gen.y_present + 3, 80, 1, 0, 25.0f, 65535, 1);
        }
            break;

        case Stop:
        {
            fillRect(gen.x_present + 10, gen.y_present, gen.x_present + 90, gen.y_present + 28, RGB(252, 0, 42));
            sprintf(ss, "Stop");
            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            drawText(opt, gen.x_present + 5, gen.y_present + 3, 80, 1, 0, 25.0f, 65535, 1);
        }
            break;

        case BarrierOperator:
        {
            int DoorLevel;
            DoorLevel = atoi(dev->getValue(index));
            if (DoorLevel == 255) {
                swprintf(opt, 179, L" %ls", lan.get("open", "connected_sensor"));
            } else if (DoorLevel == 254) {
                swprintf(opt, 179, L" %ls", lan.get("opening", "connected_sensor"));
            } else if (DoorLevel == 252) {
                swprintf(opt, 179, L" %ls", lan.get("closing", "connected_sensor"));
            } else if (DoorLevel == 253) {
                swprintf(opt, 179, L" %ls", lan.get("stopped", "connected_sensor"));
            } else if (DoorLevel == 0) {
                swprintf(opt, 179, L" %ls", lan.get("closed", "connected_sensor"));
            }
            fillRect(gen.x_present, gen.y_present, gen.x_present + 100, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 5, 100, 1, 0, 25.0f, 65535, 1);
            fillRect(gen.x_present + 110, gen.y_present, gen.x_present + 190, gen.y_present + 28, RGB(23, 191, 227));
            drawText(lan.get("open", "connected_sensor"), gen.x_present + 110, gen.y_present + 3, 80, 1, 0, 20.0f, 65535, 1);
            fillRect(gen.x_present + 200, gen.y_present, gen.x_present + 280, gen.y_present + 28, RGB(23, 191, 227));
            drawText(lan.get("close", "connected_sensor"), gen.x_present + 200, gen.y_present + 3, 80, 1, 0, 20.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d BarrierOperator status is %d ", dev->getID(), DoorLevel);
            LOG(Logger::INFO, t_temp);
            indexState = true;
        }
            break;

        case AcSetpointHeating:
        {
            double er;
            thermoHeat = 0;
            thermoHeatCurrLevel = 0;
            er = getTempinFloat(dev->getValue(index));
            er = (er - 32)*5.0 / 9;
            thermoHeat = (int) ceil(er);
            thermoHeatCurrLevel = (thermoHeat - 16)*14;
            drawText(lan.get("heat", "connected_sensor"), gen.x_present, gen.y_present + 10, 45, 1, 0, 20.0f, 65535, 1);
            fillRect(gen.x_present + 60, gen.y_present, gen.x_present + 276, gen.y_present + 28, RGB(23, 191, 227));
            drawLine(gen.x_present + 65, gen.y_present + 15, gen.x_present + 271, gen.y_present + 15, 65535);
            drawLine(gen.x_present + 65, gen.y_present + 7, gen.x_present + 65, gen.y_present + 23, 65535);
            drawLine(gen.x_present + 271, gen.y_present + 7, gen.x_present + 271, gen.y_present + 23, 65535);
            fillCircle(gen.x_present + 70 + thermoHeatCurrLevel, gen.y_present + 15, 5, 65535);
            swprintf(opt, 20, L" %d", thermoHeat);
            drawText(opt, gen.x_present + 70 + thermoHeatCurrLevel - 38, gen.y_present, 70, 1, 0, 20.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d AcSetpointHeating %d er %f", dev->getID(), thermoHeatCurrLevel, er);
            LOG(Logger::INFO, t_temp);
            indexState = true;
        }
            break;

        case AcSetpointCooling:
        {
            double er;
            thermoCool = 0;
            thermoCoolCurrLevel = 0;
            er = getTempinFloat(dev->getValue(index));
            er = (er - 32)*5.0 / 9;
            thermoCool = (int) ceil(er);
            thermoCoolCurrLevel = (thermoCool - 16)*14;
            drawText(lan.get("cool", "connected_sensor"), gen.x_present, gen.y_present + 10, 45, 1, 0, 20.0f, 65535, 1);
            fillRect(gen.x_present + 60, gen.y_present, gen.x_present + 276, gen.y_present + 28, RGB(23, 191, 227));
            drawLine(gen.x_present + 65, gen.y_present + 15, gen.x_present + 271, gen.y_present + 15, 65535);
            drawLine(gen.x_present + 65, gen.y_present + 7, gen.x_present + 65, gen.y_present + 23, 65535);
            drawLine(gen.x_present + 271, gen.y_present + 7, gen.x_present + 271, gen.y_present + 23, 65535);
            fillCircle(gen.x_present + 70 + thermoCoolCurrLevel, gen.y_present + 15, 5, 65535);
            swprintf(opt, 20, L" %d", thermoCool);
            drawText(opt, gen.x_present + 70 + thermoCoolCurrLevel - 38, gen.y_present, 70, 1, 0, 20.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d AcSetpointCooling %d er %f", dev->getID(), thermoCoolCurrLevel, er);
            LOG(Logger::INFO, t_temp);
            indexState = true;
        }
            break;

        case AcFanMode:
        {
            strcpy(thermoFan, dev->getValue(index));
            drawText(lan.get("fan", "connected_sensor"), gen.x_present, gen.y_present + 10, 50, 1, 0, 20.0f, 65535, 1);
            pngImg.drawPNG(gen.x_present + 50, gen.y_present + 5, 1, BACKGROUND_LAYER, arrow_minus);
            fillRect(gen.x_present + 82, gen.y_present + 5, gen.x_present + 182, gen.y_present + 28, RGB(23, 191, 227));
            pngImg.drawPNG(gen.x_present + 187, gen.y_present + 5, 1, BACKGROUND_LAYER, arrow_plus);
            if (strstr(thermoFan, "Auto Low")) {
                AcIrFanMode = 1;
                drawText(lan.get("auto", "connected_sensor"), gen.x_present + 90, gen.y_present + 7, 100, 1, 0, 20.0f, 65535, 1);
                pngImg.drawPNG(gen.x_present + 85, gen.y_present + 10, 1, BACKGROUND_LAYER, Tick);
            } else if (strstr(thermoFan, "On Low")) {
                AcIrFanMode = 2;
                drawText(lan.get("low", "connected_sensor"), gen.x_present + 90, gen.y_present + 7, 100, 1, 0, 20.0f, 65535, 1);
                pngImg.drawPNG(gen.x_present + 85, gen.y_present + 10, 1, BACKGROUND_LAYER, Tick);
            } else if (strstr(thermoFan, "Unknown 5")) {
                AcIrFanMode = 3;
                drawText(lan.get("medium", "connected_sensor"), gen.x_present + 90, gen.y_present + 7, 100, 1, 0, 20.0f, 65535, 1);
                pngImg.drawPNG(gen.x_present + 85, gen.y_present + 10, 1, BACKGROUND_LAYER, Tick);
            } else if (strstr(thermoFan, "On High")) {
                AcIrFanMode = 4;
                drawText(lan.get("high", "connected_sensor"), gen.x_present + 90, gen.y_present + 7, 100, 1, 0, 20.0f, 65535, 1);
                pngImg.drawPNG(gen.x_present + 85, gen.y_present + 10, 1, BACKGROUND_LAYER, Tick);
            }
            indexState = true;
            sprintf(t_temp, "GenericDevice %d thermoFan:%s", dev->getID(), thermoFan);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case AcSwing:
        {
            fillRect(gen.x_present + 10, gen.y_present, gen.x_present + 90, gen.y_present + 28, RGB(23, 191, 227));
            drawText(lan.get("swing_off", "connected_sensor"), gen.x_present + 10, gen.y_present + 7, 80, 1, 0, 20.0f, 65535, 1);
            fillRect(gen.x_present + 100, gen.y_present, gen.x_present + 180, gen.y_present + 28, RGB(23, 191, 227));
            drawText(lan.get("swing_on", "connected_sensor"), gen.x_present + 100, gen.y_present + 7, 80, 1, 0, 20.0f, 65535, 1);
            if (atoi(dev->getValue(index)) == 1)
                pngImg.drawPNG(gen.x_present + 10, gen.y_present + 5, 1, BACKGROUND_LAYER, Tick);
            else
                pngImg.drawPNG(gen.x_present + 100, gen.y_present + 5, 1, BACKGROUND_LAYER, Tick);
        }
            break;

        case IRcode:
        {
            ConFlag = true;
            aswprintf(opt, 20, L"%hs", tmpc);
            drawText(L"IR Code", gen.x_present + 10, gen.y_present + 7, 60, 1, 0, 20.0f, 65535, 1);
            fillRect(gen.x_present + 80, gen.y_present, gen.x_present + 140, gen.y_present + 28, RGB(255, 255, 253));
            fillRect(gen.x_present + 150, gen.y_present, gen.x_present + 190, gen.y_present + 28, RGB(23, 191, 227));
            drawText(lan.get("set", "common"), gen.x_present + 150, gen.y_present + 3, 40, 1, 0, 20.0f, 65535, 1);
            drawText(opt, gen.x_present + 80, gen.y_present + 3, 50, 1, 0, 20.0f, 921602, 1);
        }
            break;
        case SwitchBinary:
        {
            char tmp_str[30];
            strcpy(tmp_str, dev->getValue(index));
            if (diff == 0) {
                fillRect(gen.x_present, gen.y_present, gen.x_present + 90, gen.y_present + 30, RR_COLOR);
                swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
                drawText(opt, gen.x_present, gen.y_present + 2, 90, 1, 0, 25.0f, 65535, 1);
                fillRect(gen.x_present + 100, gen.y_present, gen.x_present + 180, gen.y_present + 29, RGB(23, 191, 227));
                drawText(lan.get("on_1", "common"), gen.x_present + 100, gen.y_present + 5, 80, 1, 0, 20.0f, 65535, 1);
                fillRect(gen.x_present + 200, gen.y_present, gen.x_present + 280, gen.y_present + 29, RGB(23, 191, 227));
                drawText(lan.get("off_1", "common"), gen.x_present + 200, gen.y_present + 5, 80, 1, 0, 20.0f, 65535, 1);
            } else {
                fillRect(gen.x_present, gen.y_present, gen.x_present + 90, gen.y_present + 30, RR_COLOR);
                swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
                drawText(opt, gen.x_present, gen.y_present + 2, 90, 1, 0, 25.0f, 65535, 1);
            }
            gen.countswitches++;
            if (gen.countswitches == 2)
                multiswitch = true;
            //            indexState = true;
            sprintf(t_temp, "GenericDevice %d state is %s ", dev->getID(), (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? "Off" : "On");
            LOG(Logger::INFO, t_temp);
        }
            break;

        case DimmerLevel:
        {
            float dimlevel;
            dimmerCurrLevel = atoi(dev->getValue(index)); //atoi(d.getValue(1))*3;        
            percentage = ((dimmerCurrLevel * 100) / 255);
            swprintf(opt, 179, L"%d%%", percentage);
            dimlevel = (float) dimmerCurrLevel;
            dimmerCurrLevel = (int) (dimlevel / 1.5f);
            fillRect(gen.x_present, gen.y_present, gen.x_present + 100, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present, 100, 1, 0, 25.0f, 65535, 1);
            fillRect(gen.x_present + 100, gen.y_present, gen.x_present + 290, gen.y_present + 29, RGB(23, 191, 227));
            drawLine(gen.x_present + 105, gen.y_present + 15, gen.x_present + 285, gen.y_present + 15, 65535);
            drawLine(gen.x_present + 105, gen.y_present + 7, gen.x_present + 105, gen.y_present + 23, 65535);
            drawLine(gen.x_present + 285, gen.y_present + 7, gen.x_present + 285, gen.y_present + 23, 65535);
            fillCircle(gen.x_present + 110 + dimmerCurrLevel, gen.y_present + 15, 5, 65535);
            //            indexState = true;
            sprintf(t_temp, "GenericDevice %d level is %d ", dev->getID(), dimmerCurrLevel);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case Temperature:
        {
            float tempv;
            fillRect(gen.x_present + 5, gen.y_present, gen.x_present + 95, gen.y_present + 29, GREEN_COLOR);
            if (format == FORMAT_C) {
                tempv = atof(dev->getValue(index));
                tempv = (((tempv - 32)*5) / 9);
                sprintf(ss, "%0.1f°C", tempv);
            } else {
                sprintf(ss, "%s°F", dev->getValue(index));
            }
            if ((strcmp(ss, "-17.8°C") == 0 || strcmp(ss, "°F") == 0)) {
            strcpy(ss, "--");
            }
            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            drawText(opt, gen.x_present + 5, gen.y_present + 5, 90, 1, 0, 20.0f, 65535, 1);
        }
            break;

        case Pressure:
        {
            float tempv;
            fillRect(gen.x_present + 5, gen.y_present, gen.x_present + 95, gen.y_present + 29, RGB(127, 0, 255));
            tempv = atof(dev->getValue(index));
            if (tempv < 1000.0f)
                sprintf(ss, "%0.1fkPa", tempv);
            else
                sprintf(ss, "%0.0fkPa", tempv);
            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            drawText(opt, gen.x_present + 5, gen.y_present + 5, 90, 1, 0, 20.0f, 65535, 1);
        }
            break;

        case Humidity:
        {
            float tempv;
            tempv = atof(dev->getValue(index));
            swprintf(opt, 20, L"%0.0f%%", tempv);
            fillRect(gen.x_present + 5, gen.y_present, gen.x_present + 95, gen.y_present + 29, GREEN_COLOR);
            pngImg.drawPNG(gen.x_present + 15, gen.y_present + 5, 1, BACKGROUND_LAYER, humidityImage);
            drawText(opt, gen.x_present + 45, gen.y_present + 5, 40, 1, 0, 20.0f, 65535, 0);
            sprintf(t_temp, "GenericDevice %d HUMIDITY is %f", dev->getID(), tempv);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case LockStateZb:
        {
            if (atoi(dev->getValue(index)) == 1) {
                swprintf(opt, 179, L"%ls", lan.get("locked", "connected_sensor"));
            } else if (atoi(dev->getValue(index)) == 2) {
                swprintf(opt, 179, L"%ls", lan.get("unlocked", "connected_sensor"));
            } else {
                swprintf(opt, 179, L"%ls", L"Not fully lock");
            }
            fillRect(gen.x_present, gen.y_present, gen.x_present + 110, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 5, 110, 1, 0, 20.0f, 65535, 1);
            fillRect(gen.x_present + 120, gen.y_present, gen.x_present + 200, gen.y_present + 29, RGB(23, 191, 227));
            drawText(lan.get("lock", "common"), gen.x_present + 120, gen.y_present + 5, 80, 1, 0, 20.0f, 65535, 1);
            fillRect(gen.x_present + 210, gen.y_present, gen.x_present + 290, gen.y_present + 29, RGB(23, 191, 227));
            drawText(lan.get("unlock", "common"), gen.x_present + 210, gen.y_present + 5, 80, 1, 0, 20.0f, 65535, 1);
            //            indexState = true;
        }
            break;

        case APower:
        {
            int powmulti = 0, powdiv = 0;
            ActivePower = 0.0;
            powmulti = strtol(dev->getValue("AC_POWERMULTIPLIER"), NULL, 16);
            powdiv = strtol(dev->getValue("AC_POWERDIVISOR"), NULL, 16);
            ActivePower = strtol(dev->getValue(index), NULL, 16);
            ActivePower = (ActivePower * powmulti) / powdiv;

            fillRect(gen.x_present + 5, gen.y_present, gen.x_present + 85, gen.y_present + 29, RGB(0, 0, 0));
            swprintf(opt, 179, L"P: %0.2fW", ActivePower);
            drawText(opt, gen.x_present + 5, gen.y_present + 5, 80, 1, 0, 20.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d ActivePower: %0.2fW ", dev->getID(), ActivePower);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case RmsVoltage:
        {
            int volmulti = 0, voldiv = 0;
            volmulti = strtol(dev->getValue("AC_VOLTAGEMULTIPLIER"), NULL, 16);
            voldiv = strtol(dev->getValue("AC_VOLTAGEDIVISOR"), NULL, 16);
            RMSVoltage = strtol(dev->getValue(index), NULL, 16);
            RMSVoltage = (RMSVoltage * volmulti) / voldiv;

            swprintf(opt, 179, L"V: %0.2fV", RMSVoltage);
            fillRect(gen.x_present + 5, gen.y_present, gen.x_present + 85, gen.y_present + 29, RGB(0, 0, 0));
            drawText(opt, gen.x_present + 5, gen.y_present + 5, 80, 1, 0, 20.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d RMS Voltage: %0.2fV ", dev->getID(), RMSVoltage);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case RmsCurrent:
        {
            int curmulti = 0, curdiv = 0;
            curmulti = strtol(dev->getValue("AC_CURRENTMULTIPLIER"), NULL, 16);
            curdiv = strtol(dev->getValue("AC_CURRENTDIVISOR"), NULL, 16);
            RMSCurrent = strtol(dev->getValue(index), NULL, 16);
            RMSCurrent = (RMSCurrent * curmulti) / curdiv;

            swprintf(opt, 179, L"I: %0.2fA", RMSCurrent);
            fillRect(gen.x_present + 5, gen.y_present, gen.x_present + 85, gen.y_present + 29, RGB(0, 0, 0));
            drawText(opt, gen.x_present + 5, gen.y_present + 5, 80, 1, 0, 20.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d RMS Current: %0.2fA ", dev->getID(), RMSCurrent);
            LOG(Logger::INFO, t_temp);

        }
            break;

            //        case State:
            //        {
            //           char tempv[10];
            //           strcpy(tempv, dev->getValue(index));
            //           swprintf(opt, 179, L"%ls", (tempv[0] == 'f' || tempv[0] == 'F') ? lan.get("inactive", "connected_sensor") : lan.get("active", "connected_sensor"));
            //           fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 30, RR_COLOR);
            //           drawText(opt, gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
            //           sprintf(t_temp, "GenericDevice %d state:%s UI:%ls ",dev->getID(), tempv, opt);
            //           LOG(Logger::INFO, t_temp);
            //        }
            //        break;

        case Tamper:
        {
            if (strcmp(dev->getValue(index), "true") == 0) {
                TamperButton.draw();
            tamper = true;
            }else{
                fillRect(15, 190, 95, 220, RR_COLOR);
                tamper = false;
            }
        }
            break;

        case LowBattery:
        {
            if (strcmp(dev->getValue(index), "true") == 0) {
                pngImg.drawPNG(240, 60, 1, 0, battery_icon);
                drawText(L"Low", 245, 62, 60, 1, 0, 12.0f, RGB(255, 0, 0), 0);
            }
        }
            break;

            //        case Occupancy:
            //        {
            //           char tempv[10];
            //           strcpy(tempv, dev->getValue(index));
            //           swprintf(opt, 179, L"%ls", (tempv[0] == 'f' || tempv[0] == 'F') ? lan.get("unoccupied", "connected_sensor") : lan.get("occupied", "connected_sensor"));
            //           fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 30, RR_COLOR);
            //           drawText(opt, gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
            //           sprintf(t_temp, "GenericDevice %d Occupancy:%s",dev->getID(), tempv);
            //           LOG(Logger::INFO, t_temp); 
            //        }
            //        break;

        case MotionState:
        {
            char tempv[10];
            strcpy(tempv, dev->getValue(index));
            swprintf(opt, 179, L"%ls", (tempv[0] == 'f' || tempv[0] == 'F') ? lan.get("nomotion", "connected_sensor") : lan.get("motiondetected", "connected_sensor"));
            fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d Motion State:%s", dev->getID(), tempv);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case DoorState:
        {
            char tempv[10];
            strcpy(tempv, dev->getValue(index));
            swprintf(opt, 179, L"%ls", (tempv[0] == 'f' || tempv[0] == 'F') ? lan.get("closed", "connected_sensor") : lan.get("open", "connected_sensor"));
            fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d Door State:%s", dev->getID(), tempv);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case VibrationState:
        {
            char tempv[10];
            strcpy(tempv, dev->getValue(index));
            swprintf(opt, 179, L"%ls", (tempv[0] == 'f' || tempv[0] == 'F') ? lan.get("novibration", "connected_sensor") : lan.get("vibrationdetected", "connected_sensor"));
            fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d Vibration State:%s", dev->getID(), tempv);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case WaterState:
        {
            char tempv[10];
            strcpy(tempv, dev->getValue(index));
            swprintf(opt, 179, L"%ls", (tempv[0] == 'f' || tempv[0] == 'F') ? lan.get("okay", "connected_sensor") : lan.get("flooded", "connected_sensor"));
            fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 2, 260, 1, 0, 25.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d Water State:%s", dev->getID(), tempv);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case GasState:
        {
            char tempv[10];
            strcpy(tempv, dev->getValue(index));
            swprintf(opt, 179, L"%ls", (tempv[0] == 'f' || tempv[0] == 'F') ? lan.get("okay", "connected_sensor") : lan.get("alarmgas", "connected_sensor"));
            fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d Gas State:%s", dev->getID(), tempv);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case FireState:
        {
            char tempv[10];
            strcpy(tempv, dev->getValue(index));
            swprintf(opt, 179, L"%ls", (tempv[0] == 'f' || tempv[0] == 'F') ? lan.get("okay", "connected_sensor") : L"Alarm:Fire Detected");
            fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d Fire State:%s", dev->getID(), tempv);
            LOG(Logger::INFO, t_temp);
        }
            break;

        case Armmode:
        {
            fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 30, RR_COLOR);
            if (atoi(dev->getValue(index)) == 0) {
                drawText(lan.get("disarmall", "connected_sensor"), gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
                LOG(Logger::INFO, "Initial state diarm");
            } else if (atoi(dev->getValue(index)) == 2) {
                drawText(lan.get("armperimeter", "connected_sensor"), gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
                LOG(Logger::INFO, "Initial state armnightzone");
            } else if (atoi(dev->getValue(index)) == 3) {
                drawText(lan.get("armall", "connected_sensor"), gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
                LOG(Logger::INFO, "Initial state armall");
            }
        }
            break;

        case PanicAlarm:
        {
            if (strcmp(dev->getValue(index), "true") == 0) {
                PanicButton.draw();
            }
        }
            break;

        case EmerAlarm:
        {
            if ((strcmp(dev->getValue(index), "true") == 0) && (more != true)) {
                EmerButton.draw();
            }
        }
            break;

            //        case SensorMultilevel:
            //        {
            //            float tempv;
            //            fillRect(gen.x_present+5, gen.y_present, gen.x_present + 95, gen.y_present + 29, GREEN_COLOR);
            //            if (format == FORMAT_F) {
            //                sprintf(ss, "%s°F", dev->getValue(index));
            //            } else {
            //                tempv = atof(dev->getValue(index));
            //                tempv = (((tempv - 32)*5) / 9);
            //                sprintf(ss, "%0.1f°C", tempv);
            //            }
            //            utf8_to_wchar(ss, strlen(ss), opt, strlen(ss), UTF8_IGNORE_ERROR);
            //            drawText(opt, gen.x_present+5, gen.y_present + 5, 90, 1, 0, 20.0f, 65535, 1);
            //            sprintf(t_temp, "GenericDevice %d ThermoCurrent %s ", dev->getID(), ss);
            //            LOG(Logger::INFO, t_temp);           
            //        }
            //        break;

        case AcMode://newgeneric
        case ThermostatMode:
        {
            strcpy(thermoMode, dev->getValue(index));
            sprintf(t_temp, "GenericDevice %d ThermoMode:%s", dev->getID(), thermoMode);
            LOG(Logger::INFO, t_temp);

            drawText(lan.get("mode", "connected_sensor"), gen.x_present + 5, gen.y_present + 5, 35, 1, 0, 16.0f, 65535, 1);
            fillRect(gen.x_present + 45, gen.y_present, gen.x_present + 120, gen.y_present + 29, !(strstr(thermoMode, "Auto")) ? BUTTON_LAYER : RGB(23, 191, 227));
            fillRect(gen.x_present + 130, gen.y_present, gen.x_present + 205, gen.y_present + 29, !(strstr(thermoMode, "Cool")) ? BUTTON_LAYER : RGB(23, 191, 227));
            fillRect(gen.x_present + 215, gen.y_present, gen.x_present + 290, gen.y_present + 29, !(strstr(thermoMode, "Heat")) ? BUTTON_LAYER : RGB(23, 191, 227));
            drawText(lan.get("auto", "connected_sensor"), gen.x_present + 45, gen.y_present + 7, 70, 1, 0, 16.0f, 65535, 1);
            drawText(lan.get("cool", "connected_sensor"), gen.x_present + 135, gen.y_present + 7, 70, 1, 0, 16.0f, 65535, 1);
            drawText(lan.get("heat", "connected_sensor"), gen.x_present + 225, gen.y_present + 7, 70, 1, 0, 16.0f, 65535, 1);
            if (strstr(thermoMode, "Auto")) {
                pngImg.drawPNG(gen.x_present + 45, gen.y_present + 5, 1, BACKGROUND_LAYER, Tick);
            } else if (strstr(thermoMode, "Cool")) {
                pngImg.drawPNG(gen.x_present + 130, gen.y_present + 5, 1, BACKGROUND_LAYER, Tick);
            } else if (strstr(thermoMode, "Heat")) {
                pngImg.drawPNG(gen.x_present + 220, gen.y_present + 5, 1, BACKGROUND_LAYER, Tick);
            }
            //            indexState = true;
        }
            break;

        case ThermostatSetpointHeating:
        {
            double er;
            er = atof(dev->getValue(index)); //atof(d.getValue(4));
            thermoHeat = (int) er;
            sprintf(t_temp, " GenericDevice %d ThermoHeat %d", dev->getID(), thermoHeat);
            LOG(Logger::INFO, t_temp);

            thermoHeatCurrLevel = (thermoHeat - 35)*3;
            drawText(lan.get("heat", "connected_sensor"), gen.x_present + 5, gen.y_present + 5, 35, 1, 0, 16.0f, 65535, 1);
            fillRect(gen.x_present + 50, gen.y_present, gen.x_present + 250, gen.y_present + 29, RGB(23, 191, 227));
            drawLine(gen.x_present + 55, gen.y_present + 15, gen.x_present + 245, gen.y_present + 15, 65535);
            drawLine(gen.x_present + 55, gen.y_present + 7, gen.x_present + 55, gen.y_present + 23, 65535);
            drawLine(gen.x_present + 245, gen.y_present + 7, gen.x_present + 245, gen.y_present + 23, 65535);
            fillCircle(gen.x_present + 60 + thermoHeatCurrLevel, gen.y_present + 15, 5, 65535);
            //            indexState = true;
        }
            break;

        case ThermostatSetpointCooling:
        {
            double er;
            er = atof(dev->getValue(index)); // atof(d.getValue(5));
            thermoCool = (int) er;
            sprintf(t_temp, "GenericDevice %d ThermoCool %d", dev->getID(), thermoCool);
            LOG(Logger::INFO, t_temp);

            thermoCoolCurrLevel = (thermoCool - 35)*3;
            drawText(lan.get("cool", "connected_sensor"), gen.x_present + 5, gen.y_present + 5, 35, 1, 0, 16.0f, 65535, 1);
            fillRect(gen.x_present + 50, gen.y_present, gen.x_present + 250, gen.y_present + 29, RGB(23, 191, 227));
            drawLine(gen.x_present + 55, gen.y_present + 15, gen.x_present + 245, gen.y_present + 15, 65535);
            drawLine(gen.x_present + 55, gen.y_present + 7, gen.x_present + 55, gen.y_present + 23, 65535);
            drawLine(gen.x_present + 245, gen.y_present + 7, gen.x_present + 245, gen.y_present + 23, 65535);
            fillCircle(gen.x_present + 60 + thermoCoolCurrLevel, gen.y_present + 15, 5, 65535);
            //            indexState = true;
        }
            break;

        case ThermostatFanMode:
        {
            strcpy(thermoFan, dev->getValue(index));
            sprintf(t_temp, "GenericDevice %d ThermoFan %s  ", dev->getID(), thermoFan);
            LOG(Logger::INFO, t_temp);

            drawText(lan.get("fan", "connected_sensor"), gen.x_present + 5, gen.y_present + 5, 35, 1, 0, 16.0f, 65535, 1);
            fillRect(gen.x_present + 40, gen.y_present, gen.x_present + 115, gen.y_present + 29, !(strstr(thermoFan, "Auto Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
            fillRect(gen.x_present + 125, gen.y_present, gen.x_present + 199, gen.y_present + 29, !(strstr(thermoFan, "On Low")) ? BUTTON_LAYER : RGB(23, 191, 227));
            drawText(lan.get("auto_low", "connected_sensor"), gen.x_present + 48, gen.y_present + 7, 75, 1, 0, 16.0f, 65535, 1);
            drawText(lan.get("on_low", "connected_sensor"), gen.x_present + 133, gen.y_present + 7, 75, 1, 0, 16.0f, 65535, 1);
            if (strstr(thermoFan, "Auto Low")) {
                pngImg.drawPNG(gen.x_present + 45, gen.y_present + 7, 1, BACKGROUND_LAYER, Tick);
            } else if (strstr(thermoFan, "On Low")) {
                pngImg.drawPNG(gen.x_present + 126, gen.y_present + 7, 1, BACKGROUND_LAYER, Tick);
            }
        }
            break;

        case HueZb:
        {
            int h_val;
            h_val = atoi(dev->getValue(index));
            sprintf(t_temp, "GenericDevice hue value:%d\n", h_val);
            LOG(Logger::INFO, t_temp);
            //            drawText(lan.get("hue", "add_sensor"), gen.x_present+5, gen.y_present+5, 85, 1, 0, 16.0f, 65535, 1);
            h_val = h_val + 25;
            pngImg.drawPNG(gen.x_present + 15, gen.y_present, 1, 0, color_png1);
            fillRect(gen.x_present + 270, gen.y_present, gen.x_present + 290, gen.y_present + 30, RGB(255, 255, 255));
            drawLine(h_val, gen.y_present, h_val, gen.y_present + 30, 0);
        }
            break;

        case Saturation:
        {
            int s_val;
            float sval;
            s_val = atoi(dev->getValue(index));
            sprintf(t_temp, "GenericDevice saturation value:%d\n", s_val);
            LOG(Logger::INFO, t_temp);
            percentage = ((s_val * 100) / 255);
            sval = (float) s_val;
            s_val = (int) (sval / 1.5f);
            swprintf(opt, 179, L"%ls-%d%%", L"Sat", percentage);
            drawText(opt, gen.x_present + 2, gen.y_present + 2, 98, 1, 0, 25.0f, 65535, 1);
            fillRect(gen.x_present + 100, gen.y_present, gen.x_present + 290, gen.y_present + 29, RGB(23, 191, 227));
            drawLine(gen.x_present + 105, gen.y_present + 15, gen.x_present + 285, gen.y_present + 15, 65535);
            drawLine(gen.x_present + 105, gen.y_present + 7, gen.x_present + 105, gen.y_present + 23, 65535);
            drawLine(gen.x_present + 285, gen.y_present + 7, gen.x_present + 285, gen.y_present + 23, 65535);
            fillCircle(gen.x_present + 110 + s_val, gen.y_present + 15, 5, 65535);
        }
            break;

        case ColorTemperature:
        {
            int color_t;
            //            swprintf(opt, 179, L"%ls", lan.get("temp", "connected_sensor"));
            //            drawText(opt, gen.x_present+2, gen.y_present+5, 30, 1, 0, 16.0f, 65535, 1);
            color_t = atoi(dev->getValue(index));
            color_t = ((color_t - 1000) / 32) + 25;
            pngImg.drawPNG(gen.x_present + 15, gen.y_present, 1, 0, color_temp1);
            drawLine(color_t + 1, gen.y_present, color_t + 1, gen.y_present + 30, 65535);
        }
            break;

        case AlarmState:
        {
            // TimeBox = new Widget(wType.TextBox, gen.x_present+205 , gen.y_present, 60, 30, 0, L"");
            // TimeBox->onTouch = &TimeBoxOnTouch;
            fillRect(gen.x_present + 5, gen.y_present, gen.x_present + 90, gen.y_present + 29, RGB(255, 51, 51));
            drawText(L"Alarm", gen.x_present + 5, gen.y_present + 4, 85, 1, 0, 20.0f, 65535, 1);
            fillRect(gen.x_present + 105, gen.y_present, gen.x_present + 190, gen.y_present + 29, RGB(51, 255, 51));
            drawText(L"Off", gen.x_present + 105, gen.y_present + 4, 85, 1, 0, 20.0f, 65535, 1);
            if (dev->getDeviceTechnology() == ZWaveTechnology) {
                char tmp_str[30];
                strcpy(tmp_str, dev->getValue(index));
                swprintf(opt, 179, L"%ls", (tmp_str[0] == 'f' || tmp_str[0] == 'F') ? lan.get("off_1", "common") : lan.get("on_1", "common"));
                fillRect(gen.x_present + 200, gen.y_present, gen.x_present + 280, gen.y_present + 40, RR_COLOR);
                drawText(opt, gen.x_present + 200, gen.y_present + 7, 80, 1, 0, 30.0f, 65535, 0);
                sprintf(t_temp, "AlarmState state received is %s ", tmp_str);
                LOG(Logger::INFO, t_temp);
            } else {
                TimeBox = new Widget(type.TextBox, gen.x_present + 205, gen.y_present, 60, 30, 0, L"");
                TimeBox->onTouch = &TimeBoxOnTouch;
                drawText(L"sec", gen.x_present + 267, gen.y_present + 11, 30, 1, 0, 18.0f, 65535, 1);
                TimeBox->draw();
                TimeBox->setText(N_opt);
            }
            //            indexState = true;
        }
            break;

        case MeteringCurrent:
        {
            double tempf = 0.0;
            tempf = (double) atof(dev->getValue(index));
            swprintf(opt, 179, L"%s %0.3lf %s", "Energy :", tempf, "kWh");
            fillRect(gen.x_present + 10, gen.y_present, gen.x_present + 290, gen.y_present + 30, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 2, 280, 1, 0, 25.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d Energy reading:%lf", dev->getID(), tempf);
            LOG(Logger::INFO, t_temp);
        }
            break;
        case WindowCoveringMode:
        {
            fillRect(gen.x_present + 5, gen.y_present, gen.x_present + 90, gen.y_present + 29, RGB(23, 191, 227));
            drawText(L"Up", gen.x_present + 5, gen.y_present + 5, 85, 1, 0, 18.0f, 65535, 1);
            fillRect(gen.x_present + 105, gen.y_present, gen.x_present + 190, gen.y_present + 29, RGB(23, 191, 227));
            drawText(L"Down", gen.x_present + 105, gen.y_present + 5, 85, 1, 0, 18.0f, 65535, 1);
            fillRect(gen.x_present + 205, gen.y_present, gen.x_present + 290, gen.y_present + 29, RGB(23, 191, 227));
            drawText(L"Stop", gen.x_present + 205, gen.y_present + 5, 85, 1, 0, 18.0f, 65535, 1);
        }
            break;
	case GestureSensor:
        {
            wchar_t opt[260] = {0};
            int value=0;
            value = atoi(dev->getValue(index));
            if(value == 1)
               swprintf(opt, 259, L"UP"); 
            else if(value == 2)
               swprintf(opt, 259, L"DOWN"); 
            else if(value == 3)
               swprintf(opt, 259, L"LEFT"); 
            else if(value == 4)
               swprintf(opt, 259, L"RIGHT"); 
            else
               swprintf(opt, 259, L"NO SCENE");

            fillRect(gen.x_present, gen.y_present, gen.x_present + 190, gen.y_present + 28, RR_COLOR);
            drawText(opt, gen.x_present, gen.y_present + 2, 190, 1, 0, 25.0f, 65535, 1);
            sprintf(t_temp, "GenericDevice %d Gesture : %d", dev->getID(), value);
            LOG(Logger::INFO, t_temp);
        }
        break;
        default:
            break;
    }
    memset(opt, 0, sizeof (opt));
    return 0;
}

static int xferinfo(void *p,
        curl_off_t dltotal, curl_off_t dlnow,
        curl_off_t ultotal, curl_off_t ulnow) {
    CURL *curl = (CURL*) p;
    double curtime = 0;

    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &curtime);
    return 0;
}

size_t write_data2(void *ptr, size_t size, size_t nmemb, FILE * stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int downloadFile(char *url, char *fileName) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *outfilename = fileName;
    curl = curl_easy_init();
    //    printf("Version:%6x\n", LIBCURL_VERSION_NUM);
    if (curl) {
        fp = fopen(outfilename, "wb");
        res = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        if (res != CURLE_OK)
            goto CLEANUP;
        res = curl_easy_setopt(curl, CURLOPT_CAINFO, "/almond/cacert.pem");
        if (res != CURLE_OK)
            goto CLEANUP;
        res = curl_easy_setopt(curl, CURLOPT_URL, url);
        if (res != CURLE_OK)
            goto CLEANUP;
        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data2);
        if (res != CURLE_OK)
            goto CLEANUP;
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        if (res != CURLE_OK)
            goto CLEANUP;
        //if (downloadFirmware)
        {
            res = curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
            if (res != CURLE_OK)
                goto CLEANUP;
            res = curl_easy_setopt(curl, CURLOPT_XFERINFODATA, curl);
            if (res != CURLE_OK)
                goto CLEANUP;
        }
        res = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        if (res != CURLE_OK)
            goto CLEANUP;
        res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600L);
        if (res != CURLE_OK)
            goto CLEANUP;
        res = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
        if (res != CURLE_OK)
            goto CLEANUP;
        res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);
        if (res != CURLE_OK) {
            printf("SSL Verify failed...!\n");
            goto CLEANUP;
        }
        res = curl_easy_perform(curl);

CLEANUP:
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
        if (res == CURLE_OK) {
            long httpCode = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
            if (httpCode != 200) {
                sprintf(t_temp, "HTTP request failed:%ld", httpCode);
                LOG(Logger::ERROR, t_temp);
                return -1;
            }
        }
    }

    if (res == CURLE_OK)
        return 0;
    else {
        printf("downloading from [%s] failed\n", url);
        return -1;
    }
}

