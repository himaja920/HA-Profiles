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
#include <Logger.h>
#include <math.h>
#include <bits/stdc++.h>
using namespace std;

#define RGB(r,g,b)              (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //5 red | 6 green | 5 blue

void theCallback(Device* notifiedDevice);
void theEventCallback(Event *e);
void LIST(void);
void Value(void);

Logger *logger;
DeviceList dL;
Device * notifiedDevice;
Event *e;
DevType contDev = UnknownDeviceType;
list<Device>::iterator it;

double  LedSatur = 0.0;

unsigned int presentColor = 0;
unsigned short int  Ledhue = 0;

int sensorsCount = 17, xCount = 2, yCount = 3, pageNum, curPage = 0;

int contDevID = -1;
int devIdList[250], MODE = LIST_MODE;
bool a = false;

int main(int argc, char** argv) {
    
    init();
    LIST();
    HADevices::initializeFree2();
    HADevices::genericCallback(&theCallback);
    HADevices::eventCallback(&theEventCallback); 
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
        cout<<"Name: "<<(*iterator).getDeviceName()<<endl;
        cout<<"Location : "<<(*iterator).getDeviceLocation()<<endl;
        cout<<"OZWNode : "<<(*iterator).getOZWNode()<<endl;
        cout<<"ZigBeeShortID : 0x"<<(*iterator).getShortID()<<endl;
        cout<<"ZigBeeEUI64 : "<<(*iterator).getZigBeeEUI64()<<endl;
        cout<<"DeviceTechnology : "<<((*iterator).getDeviceTechnology())<<endl;
        cout<<"AssociationTimestamp : "<<(*iterator).getAssociationTimeStamp()<<endl;
        cout<<"DeviceType : "<<(*iterator).getDeviceType()<<endl;
        cout<<"FriendlyDeviceType : "<<(*iterator).getFriendlyDeviceType()<<endl;
        cout<<"Value Count : "<<(*iterator).getValueCount()<<endl;
        int i=0;
        for(i = 0;i<=(*iterator).getValueCount();++i)
          { 
                     cout<<"Index = "<<i<<"\t"<<"Endpoint = "<<(*iterator).getEndPoint(i)<<"\t"<<"Value = "<<(*iterator).getValue(i)<<"\t"<<"Type : "<<(*iterator).getIndexType(i)<<"\t"<<"Name : "<<(*iterator).getValueName(i)<<endl;
          }
        cout<<"LastActiveTime : "<<(*iterator).getLastActiveTime()<<endl;
        cout<<"Model ID : "<<(*iterator).getModelid()<<endl;
        cout<<"Version : "<<(*iterator).getVersion()<<endl;
        cout<<"Manufacturername : "<<(*iterator).getManufacturername()<<endl;
        
        cout<<"========================================================="<<endl;
        cout<<"========================================================="<<endl; 
        
     }
        cout<<"NextDevid - 14"<<endl;
        cout<<"Version - 2.0"<<endl;

}

void Value(void)
{
   loop:

   int DevID,IdxID;
   char IdxVal[5];
   
   printf("Enter the device ID\n");
   scanf("%d",&DevID);
   if (DevID > 0  && DevID <= dL.devices.size() ) {
   printf("Enter the index ID\n");
   scanf("%d",&IdxID);
   printf("Enter the index Value\n");
   scanf("%s",IdxVal);
   Device d(DevID);
   d.setValue(IdxID,IdxVal);
   }
   else
     cout<<"No such Device ID exists\n";
    
   goto loop; 
}

void init(void) {
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

void theEventCallback(Event * e) {
    if (e->event) {
        switch (e->event) {
           
            case DeviceUpdated:
            {
                 DeviceEvent DevicePacket;

                memcpy(&DevicePacket, e->data, sizeof (DevicePacket));
                 list <Device>::iterator it = dL.devices.begin();
                for (; it != dL.devices.end(); ++it) {
                    if ((*it).getID() == DevicePacket.DevId) {
                      cout<<"Updated Device is : "<<(*it).getDeviceName()<<" , Updated location is : "<<DevicePacket.Location<<endl;}}
                 }
                break;
             case AlmondUpdated:
            {
                cout<<"Almond Updated\n";
            
        }}
    }
}


void theCallback(Device * notifiedDevice) {
    int cnt = 0;
    cout<<"Notification\n";
    for (list <Device>::iterator iterator = dL.devices.begin(); iterator != dL.devices.end() && cnt < 250; ++iterator) {
        if ((*iterator).getID() == notifiedDevice->getID()) {
    if (  MODE == CONTROL_MODE || MODE == BLINK_COLOR_MODE || MODE == LIST_MODE) {  
       contDev = (*iterator).getDeviceType();
        switch(contDev) {            
                
       case AlmondBlink:
                {

                    if ((notifiedDevice->getLastNotificationIndex() == 1) ) {
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "false") == 0) {
                            cout<<(*iterator).getDeviceName()<<" : OFF ,"<<" Updated Value is : "<<(*iterator).getLastNotificationIndexValue()<<"\n";
                        } else {
                              cout<<(*iterator).getDeviceName()<<" : ON ,"<<" Updated Value is : "<<(*iterator).getLastNotificationIndexValue()<<"\n";
                        }
                    }} 
                    break;

          case HueLamp:
                {
                    Device d(contDevID);
                    
                    if (MODE != COLOR_MODE) {
                           if(notifiedDevice->getLastNotificationIndex() == 2){
                        if (strcmp(notifiedDevice->getLastNotificationIndexValue(), "false") == 0) {
                              cout<<(*iterator).getDeviceName()<<": "<<"Binary Swicth Off\n";
                           }
                        else{ cout<<(*iterator).getDeviceName()<<": "<<"Binary Swicth On\n";}
          

                      }}}
                    break;
        } 
      } 
    }        
  }
}
