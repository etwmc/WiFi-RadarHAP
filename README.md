#WiFi Radar - HomeKit edition<br>
WiFi Radar is a new project based on my HomeKit Accessory Protocol implementaiton (more on that later), which solved the age old question: how to let your smart home know everyone in your family is out, so it can do whatever you want, let it be turn off the lights, turn on the security, or turn off gas valves. <br>
#Setup<br>
To setup Wi-Fi Radar, there is 3 easy parts: <br>
1. Configurate the system and compile (make -j 8)<br>
2. Run it in a devices that would stay inside your house, and turned on 24x7. Let it be your desktop, laptop that seldom take outside the house, or OpenWRT router. Thanks to the portability of C, you can run it almost anywhere you want.  <br>
3. For any device you would want to track, open a third party HomeKit app (Last time I check the Home app only show what Apple defined as a normal characteristic), select the sensor, and you will see there is a switch. Turn it on and the system will start tracking its connection. As long as there is one device stay within your WiFi range, it will continuously triggered, so your lights won't suddently got turned off because someone is out.<br>
4. If you need to manual modify the list of device being tracked, just turn it off, add/remove the IP addresses, and restart. <br>
5. Use HomeKit app to add automation. It could work both triggered and not trigger. (it means whether your home has people) For not-triggering case, you would need to use a 3rd part HomeKit app (like the one by Eve)<br>

#Things Configure the system:
  There is essentially 3 constant in Configuration.h:
  1. deviceIdentity<br>
    It is the identification of the program, and your iPhone/iPad will use it to see if it should connect to the program<br>
        It has six values, which having two hex value<br>
        #Or to be easy, just replace every character beside the colon with a random one, from 0-9, and captial A-F<br>
  2. controllerRecordsAddress<br>
        It is the location to store the key, so your phone can talk to the program securely and thieves can't know when your house is empty<br>
  3. keepAlivePeriod<br>
        How long do you want the system check if your home is in the network, in seconds <br>
        WARNING: do not, I repeat, do not send it in a short period like 10 (represent 10 seconds). Due to the design of TCP, your phone would still have a long time before declared missing, and using a short period will just spend lots of your phone's power, as it would need to wake up and read the message. However, because it is handled by a small program inside iOS (homed), you won't see it being listed as a separate process, but your phone could be drain in 7 hours of standby. <br>
        If you want a shorter respond time, consider look into the TCP retry mechanism and change that variable, which I beleive best to do only if you have a standaone Linux box (This doesn't need something fancy like the RPi. An old router with OpenWRT will do. THe OpenWRT compile kit is coming. )<br>


#FAQ<br>
1. Some time needed until the home is consider empty<br>
    While connecting is a fast process, in TCP, there is no mechanism to check if your device is disconnected because you are leaving/turning off WiFi/etc. In TCP, they use a try again mechanism, which it will try multiple times, in an exponential retry period, and declare it fails until all attemp fails. This will require us to send packages as a "keep alive", hence there is a keep alive package getting sent. <br>
2. Not supporting any devices beside iOS<br>
    Unfortunatly, you can't access the connection list in your router in one API. Not just because it was started to develop before the Smart Home is a thing, because it is something you don't want to mess up. (Imageine your light bulb could shut down your network)<br>
    So, in order to track, I have to either get access to every router software and create a special version, or setup a TCP/IP connection to your phone. And thanks to how much HomeKit like to connect to a Wi-Fi device, it would be smart to just use that connection instead of another TCP/IP socket with a special app. <br>
    Beside, if you use HomeKit, why would you use Android? <br>
3. How to get my Apple Airport/Apple TV do it?<br>
    Maybe write some email to them? It is a small daemon, so it could be added without reducing any performance. But currently I didn't find a way to add stuff in Airport, and tvOS doesn't like long term TCP connection.<br>
4. Why it suddenly doesn't track<br>
    It's determined by your LAN IP address, so if your IP address changed, it forget it exist. So, consider reverse it as a static IP address. <br>
<br>
<br>



--------------
The following is the direct readme copy from PersonalHomeKit, so I can keep the description simple<br>
<br>



#PersonalHomeKit<br>
This is a dymanic version (runtime add/remove accessories/services). For static version(MCU version), switch to the static branch
Now support multiple characteristic, notify function
---------------
Reset
In iOS 9.0 Beta 1, Apple has implemented the check of status flag: a integer display where an accessory should be discoverable. And for any accessory that has paired, the flag should be 0. 
Therefore, there is a way to reset the pair record in since this version: 
By adding any argument while launching the program, the program will clear all the previous record of pairing, before start to launch the service. 
e.g. ./PHK abcdefg
This argument might be changed in the future, so if you want to deploy for the future, consider using "PHK reset" as the command. 

(
However, as it has not been implemented in iOS 8.3 or before, after any reset, iOS devices before iOS 9 will still attempt to connect, which will failed for obvious reason. 
In that case, a manual removal of the accessory on the iOS end is required. 
)



===============

This is a spin-off project from my attempt to build a Siri controlled night light, and it will provide source code to build a HomeKit support accessories.


1. Publish the device as a HomeKit
2. Allow the controller (iOS device) to pair with the accessory
3. Allow the controller connect with the accessory after pair
4. Record the paired controllers UUID and public keys
5. Provide the controller the service about the accessory
6. Allow controller to update value
7. Notify controller about value updated

Current Requirment:
1. OpenSSL
2. Avahi (For Linux)

Future Plan:
1. Support microcontroller



===============
Dear Lawyer of Apple, <br>
If you want me to pull down the code, just send me an email. (No phone call, I don't pick up unknown number. ) I'm a reasonable kid, and I like Apple, so no need for DMCA, just talk to me, we can make a deal.

Build method:
After copy the codes, please follow these steps:

1. On non-OSX platform, please install avahi-deamon (or any dns-sd implement), clang and OpenSSL.
2. Set the following value in Configuration.h<br>
  i. HomeKitLog -> Whether the program return message to console<br>

  ii.HomeKitReplyHeaderLog -> Whether the program should return the accessory message (Recommend debug only)<br>

  iii. Device setting: deviceName (Name), deviceIdentity (Device ID publish in HomeKit, usually your MAC address), manufactuerName (Manufactuer Name shown in HomeKit Framework), devicePassword (Password, please choose 9 non sequential, or repeat digits in XXX-XX-XXX fashion, e.g. 421-35-195), deviceUUID (UUID for iOS to verify your accessory identity)<br>

  iv. (IMPORTANT) controllerRecordsAddress -> where should the program store the pair record. It is left empty to prevent you compile without setting Configuration.h<br>

  v. numberOfClient -> Please increase or lower it based on your hardware power. HomeKit framework require the accessory to response in 1-2 seconds before timeout. <br>

  vi. (IMPORTANT) If you are porting the program to MCU (NOT OFFICIAL TESTED), change MCU to 1, and implement pair record service in PHKControllerRecord.cpp, as MCU usually use EEPROM instead a file system. <br>

3. Implement Accessory function:
  The program publish a light service (and identify service, which is the essential function of any HomeKit accessory). However, the service does nothing as the variety configuration one could come up to achieve this project (I'm currently using a Ubuntu server, with serial connected Arduino)<br>

  Therefore, change the PHKArduinoLightInterface.c to your own configuration
    setupPort() -> setup the connection/GPIO/other thing you need to change the light
    startIdentify() -> a method to show users this accessory is the one they ask to identify (for example, blink three time)
    setLightStrength(int strengthLevel) -> change the light stength, the int is from 0-255
<br>

To create your own service, please reference to the Accessory.cpp. You can add characteristic in a service, service in an accessory, and accessory in accessory set. However, accessory id is unique in accessory set, and characteristic id is unique in accessory. (NOT SERVICE)<br>

(Please beware the number of characteristic, service and accessory, and the getter based on id is hard coded in the numberOf_ and _AtIndex. This is done to reduce the multithread complexity. So if you are going to implement a dynamic setting (for example, make a accessory bridge with PnP accessories), please keep the function be thread-safe at all time. )
<br>
4. Then, make and enjoy.
<br>
