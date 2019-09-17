#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<bits/stdc++.h>
#include<unistd.h>
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
#include "libAlmondHA.h"
//Logger *logger;

void theCallback(Device* notifiedDevice);
void theEventCallback(Event *e);

DeviceList dL;
Screen *connected_home;
list<Device>::iterator it;


    HADevices::initializeFree2();
    //HADevices::genericCallback(&theCallback);
    HADevices::eventCallback(&theEventCallback);


    
    /*static void eventCallback(void (*callback)(Event*));
    {
        void theEventCallback(Event * e);*/
  


void theEventCallback(Event * e) 

  int getID(void);
Device* getDevice(int devid) {
    list <Device>::iterator iterator;
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




void DevtypeEnumToStr(enum DevType Type, char* DeviceType);
DevType StrToDevType(char *str);
void IndexTypeEnumToStr(enum IndexType Itype, char *data, int choice); //choose 0 - Index Name and 1 - Index Type
int StrToIndexTypeEnum(const char *);

    static void deviceCallback(int DevID, void (*callback)(Device *));
    
    static DeviceList* getDeviceList(void)
    {
       string str ="/almond/decodedb 1 /tmp/ddb";
            const char * command =str.c_str();
             system(command);
     }
    void setValue(int ,char*);
char* ClientTypeToString(enum ClientType type);
ClientType stringToClientType(char *str);

class client;

int main()
{
  
   getDeviceList();
   enum ClientType t = tv;
  
   printf("client type is %d\n",t);

  loop:
  int DevID,IndID,IndVal;
  printf("Enter the Device ID\n");
  scanf("%d",&DevID);
  getDevice(DevID);

  printf("Enter the Index ID\n");
  scanf("%d",&IndID);

  printf("Enter the Index Value\n");
  scanf("%d",&IndVal);
  goto loop;
}
/*#ifdef __cplusplus
}
#endif /* __cplusplus */


//#endif






























