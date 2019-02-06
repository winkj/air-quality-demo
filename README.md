# Air Quality Demo

## Summary

This is a demo project showing the integration of Sensirion's environmental sensors with Cypress WICED SDK devices, using http://thingsboard.io as an online dashboard. Sensors used are the SHTC1 temperature humidity sensor and SGP30 air quality sensor as well as the SPS30 particulate matter sensor.

The demo is prepared for the ```CK8CKIT-062-WIFI-BT``` reference design, but it should work with all WICED SDK devices with minor modifications.

This tutorial assumes that the user is familiar with Cypress' WICED Studio, and Thingsboard. For questions on how to use these tools, please check their respective documentation.

The drivers for this demo are implemented using Sensirion's standard embedded drivers:
- SPS30: https://github.com/Sensirion/embedded-sps
- SHTC1: https://github.com/Sensirion/embedded-sht
- SGP30: https://github.com/Sensirion/embedded-sgp

The WICED port is based on the wiced-ess implementation:
- https://github.com/winkj/wiced-ess

## Preparation

Please download a current release ZIP file from https://github.com/winkj/aqdemo/releases

## Dashboard setup

This demo is using http://thingsboard.io for visualization. To setup this demo, follow the following steps:

1. first create an account on their demo server https://demo.thingsboard.io/login
1. Go to the ```Devices``` tab
1. create a new device; you can choose any name, but we're going to use ```CY8CKIT``` in this documentation; make sure you replace any occurances of this with the device name you chose
1. Make the device public by pressing the "share" icon
1. Go to the ```Dashboards``` tab
1. import the dashboard from the downloadded ZIP, located in ```dashboard/air_quality_demo.json```
1. Open the newly created dashboard (It's going to be called "Air Quality Demo"
1. Press the edit button
1. Select the entity aliases button, and make sure you have an alias named ```ess-input```, Filter Type ```Single Device```, Type ```Device```, Device ```CY8KIT```
1. Go back to the ```Dashboards``` tab
1. Make the Dashboard public by pressing the "share" icon, and store the URL you're getting from this dialog

Your dashboard is now ready to be used

## Device setup

This demo is supporting Cypress WICED SDK devices. 

### Step 1: import the files
For the device setup, follow the following steps:
1. Open WICED-Studio
1. Go to the ```wiced``` directory in your downloaded ZIP file
1. Select the "sensirion" directory, and press Ctrl-c to copy it
1. Go to WICED Studio, and open 43xxx_Wi-Fi\libraries\drivers\sensors
1. Press Ctrl-v
1. Go back to the downloaded files. 
1. In the ```wiced directory```, select the ```air_quality_demo``` folder, and press Ctrl-c to copy it
1. Go back to WICED-Studio and open 43xxx_Wi-Fi\apps\demo
1. Press Ctrl-v
1. Create a new Make target called ```demo.air_quality_demo-CY8CKIT_062 download run```

For the last step, if you're using another WICED platform, replace ```CY8CKIT_062``` with the name of your platform

### Step 2: Adjust the files
1. go to the ```air_quality_demo``` folder, and open ```air_quality_demo_secrets.h```
1. Change the WiFi information to match your wireless network
1. Then, go to https://demo.thingsboard.io/devices, click on the device you created before, and select "Copy Access Token"
1. Paste this in ```air_quality_demo_secrets.h``` for the ```THINGBOARD_DEVICE_TOKEN``` #define

### Step 3: Test without network
1. To test without the networking setup, open ```air_quality_demo.c``` in your ```air_quality_demo``` directory, and comment out the ```#define ENABLE_DATA_UPLOAD```
1. Double click the make target you created earlier
1. Open the serial console, and you should see the following output:
```
tVOC   21 | CO2eq  400 | T   25.83 | RH   33.46
measured values:
	0.55 pm1.0
	0.77 pm2.5
	0.92 pm4.0
	0.96 pm10.0
	3.26 nc0.5
	4.11 nc1.0
	4.32 nc2.5
	4.35 nc4.5
	4.35 nc10.0
	0.74 typical particle size
```
If you see this output, the setup worked as expected. YOu can then enable the networking (see next step)

### Step 4: Test with Network upload
1. To test with the networking enabled, open ```air_quality_demo.c``` in your ```air_quality_demo``` directory, and remove the comment from ```#define ENABLE_DATA_UPLOAD```
1. Double click on the make target you created earlier
1. Check the serial configuration for information about the networking. If there's any errors here, make sure the correct Wi-Fi information is set in ```air_quality_demo_secrets.h```
1. Then, go to https://demo.thingsboard.io/devices, click on the device you created before, and click the "LATEST TELEMETRY" 
1. You should see a steady stream of data coming in
1. Go to the dashboards tab, and select your new data.
1. If everything works as expected, you should see the gauges and charts animated with the most recent data
