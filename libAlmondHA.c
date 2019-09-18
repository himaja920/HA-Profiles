int main(int argc, char** argv) {
    for(int i=0;i<argc;i++)
    printf("%s\n",argv[i]);
    HADevices::initializeFree2();
    HADevices::eventCallback(&theEventCallback);
    setTouchHandler(tH);
    libTouchInit();
    getDevicedisplay();
    Value();
    a = true;
    while (1) {
      pause;
    }
    return 0;
}
void  getDevicedisplay(void)
{
  string str ="/almond/decodedb 1 /tmp/ddb";
  const char * command = str.c_str();
  system(command);
}
void Value(void)
{
   loop:
   int DevID,IdxID;
   char IdxVal[12];
   printf("Enter the device ID\n");
   scanf("%d",&DevID);
   printf("Enter the index ID\n");
   scanf("%d",&IdxID);
   printf("Enter the index Value\n");
   scanf("%s",IdxVal);
   Device d(DevID);
   d.setValue(IdxID,IdxVal);
   goto loop;
}

