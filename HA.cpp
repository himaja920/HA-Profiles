#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <unistd.h>
#include <list>
#include <libAlmond.h>
#include <libAlmondHA.h>
#include "main.h"
#include <unicode.h>
#include <Logger.h>#include <libAlmondHA_Private.h>
#include "almond_timer.h"
#include "almondList.h"
#include <math.h>
#include <bits/stdc++.h>
using namespace std;

#define RGB(r,g,b)              (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //5 red | 6 green | 5 blue

void theCallback(Device* notifiedDevice);
void theEventCallback(Event *e);
void LIST(void);
void Value(void);
extern int roundfun(float num);



Languages lan;
Logger *logger;
DeviceList dL;
Device * notifiedDevice;
Event *e;
Screen *connected_home;
WidgetType type;
DevType contDev = UnknownDeviceType;
list<Device>::iterator it;

ImagePNG pngImg;
lcdImage readLCDArea, /*readLCDArea2,*/ screenData;

double  LedSatur = 0.0;


unsigned int presentColor = 0;
unsigned short int  Ledhue = 0;


int sensorsCount = 17, xCount = 2, yCount = 3, pageNum, curPage = 0;
int  battery_icon;


int contDevID = -1;
int u_val, s_devid = -1;
int dimmerCurrLevel, percentage;

int xCoords[3] = {X_COORD_START, X_COORD_START + 150, X_COORD_START + 300};
int yCoords[4] = {Y_COORD_START, Y_COORD_START + 60, Y_COORD_START + 120, Y_COORD_START + 180};
int devIdList[250], MODE = LIST_MODE, format = FORMAT_F;


bool enableTouch = false;
bool StartLED = true, StartBlinkLED = false, StartSiren = false ;
bool ota_upgrade_flag = false;

unsigned char Userno;
char nVer[512], output[180], t_temp[500];
char  Time[10] = "";

wchar_t opt[180],  As_opt[180];


Widget EditButton(type.Button, 220, 190, 80, 30, 0, lan.get("edit", "common")), TamperButton(type.Button, 15, 190, 80, 30, 0, lan.get("tampered", "connected_sensor"));

Widget AlmondTimer(type.TextBox, 180, 185, 55, 30, 0, L"∞"), AlmondLedTimer(type.TextBox, 175, 190, 55, 30, 0, L"∞");

Widget SirenButton(type.RadioButton, 20, 95, 40, 25, 0, L""), AlarmButton(type.RadioButton, 105, 95, 40, 25, 0, L""), DoorBellButton(type.RadioButton, 200, 95, 40, 25, 0, L"");
Widget LowVolume(type.RadioButton, 20, 140, 25, 25, 0, L""), MedVolume(type.RadioButton, 105, 140, 25, 25, 0, L""), HighVolume(type.RadioButton, 200, 140, 25, 25, 0, L"");


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
    //setTouchHandler(tH);
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
        //DevtypeEnumToStr(enum DevType Type, char* DeviceType)
       
        //cout<<"Device Type :"<<(*iterator).getDeviceType()<<endl;
        cout<<"Device Technology : "<<((*iterator).getDeviceTechnology())<<endl;
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
    cout<<"Index = "<<i<<"\t"<<"Endpoint = "<<(*iterator).getEndPoint(i)<<"\t"<<"Value = "<<(*iterator).getValue(i)<<"\t"<<"Type : "<<(*iterator).getIndexType(i)<<"\t"<<"Name : "<<(*iterator).getValueName(i)<<endl;
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
   
   Device d(DevID);
   d.setValue(IdxID,IdxVal);
   HADevices::genericCallback(&theCallback);
   goto loop;
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
void backOnTouch(void) {
    if (curPage == 0) {
        //        onExit();
        abort();
    } else {
        curPage--;
        lcd_clear(BACKGROUND_LAYER);
        //connectedHome_draw();
    }


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
   void theEventCallback(Event * e) {
       cout<<"Event callback received\n\n";
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
                        //connectedHome_draw();
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
                            //connectedHome_draw();
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
                //connectedHome_draw();
            }
                break;
            case DeviceUpdated:
            {
                printf("Device updated\n");
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
                               // connectedHome_draw();
                            } else {
                                for (int i = 0; i < dL.devices.size(); i++) {
                                    if (devIdList[i] == DevicePacket.DevId) {
                                        if ((i / 11) == curPage) {
                                            //connectedHome_draw();
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                }

            }
                break;
            case AlmondModeUpdated:
            {
               
            }
                break;
            case AlmondUpdated:
            {
              
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
      cout<<"Callback\n";
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
                
                case BinarySensor:
                {
                   printf("DoorSensor\n\n\n\n");
                    if (MODE != DEVICE_MODE) {//
                        if (notifiedDevice->getLastNotificationIndex() == 1) {
                            fillRect(80, 130, 300, 200, RR_COLOR);
                            if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "true") == 0) {
                                swprintf(opt, 179, L" %ls", lan.get("true", "common"));
                            } else {
                                swprintf(opt, 179, L" %ls", lan.get("false", "common"));
                               cout<<"DoorClosed\n\n\n";
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
               
                
                case LightSensor:
                {
                    switch (notifiedDevice->getIndexType(notifiedDevice->getLastNotificationIndex())) {
                        case Temperature:
                        {

                            fillRect(190, 95, 300, 125, GREEN_COLOR);
                            memset(opt, 0, sizeof (opt));
                            //convertTemperature(notifiedDevice->getLastNotificationIndex(), opt);
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
               
              
                case AlmondSiren:
                {
                  
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
                             printf("Tone\n\n\n");

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
                            cout<<"Led is off\n\n\n\n\n\n\n";
                            //connectedHomeSetValue(1,"FALSE");
                        } else {
                            fillRect(25, 95, 125, 125, RGB(23, 191, 227));
                            drawText(L"Turn Off", 25, 101, 100, 1, 0, 18.0f, 65535, 1);
                            StartLED = false;
                            cout<<"Led is off\n\n\n\n\n\n\n";
                            //connectedHomeSetValue(1,"FALSE");
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
                
  }}}}           
