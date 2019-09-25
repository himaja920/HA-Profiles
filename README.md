# HA-Profiles


Build an application with libAlmondHA which should do the following actions. libAlmondHA is an abstract library with API calls to perform the same actions on the HA profiles and to register callbacks to listen for updates on the HA profiles.
   
   Build the application with "Free2" client as the others are being used by other applications. Use the API call to initiate the "Free2" client.
   
   Register callbacks for events and device notifications. The library will create a separate thread for you to handle these callbacks.
   
   At the start of the application, it should print all the device details and their values. Just like "/almond/decodedb 1 /tmp/ddb" command in the Almond.
   
   On the main thread, it should continuously ask the user to enter three values device ID, index ID, and index value. After getting these three values the application should invoke the API call to set this value. "setValue" is the API call in the Device class.
   
   Since the callbacks are registered for the events, if you turn on/off the Almond's LED from the LCD app, this application should receive those notifications and display on the console about the updated value.






Completed tasks:




Free2 client is initialized using HADevices::initializeFree2();  /// 

Printing Devices List                                            ///

Calling SetValue API (Can be set only for certain devices)       ///

Registering call back events                                     ///

Getting notifications but not what is modified                   ///

Turn on/Turn off LED from LCD getting notification only not updated value                   ///

