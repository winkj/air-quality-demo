# Air Quality Demo

## Summary

This is a demo project showing the integration of Sensirion's environmental sensors with Cypress WICED SDK devices, using http://thingsboard.io as an online dashboard. Sensors used are the SHTC1 temperature humidity sensor and SGP30 air quality sensor as well as the SPS30 particulate matter sensor.

The demo is prepared for the ```CK8CKIT-062-WIFI-BT``` reference design, but it should work with all WICED SDK devices with minor modifications.

## Dashboard setup

This demo is using http://thingsboard.io for visualization. To setup this demo, follow the following steps:

1. first create an account on their demo server https://demo.thingsboard.io/login
1. Go to the ```Devices``` tab
1. create a new device; you can choose any name, but we're going to use ```CY8CKIT``` in this documentation; make sure you replace any occurances of this with the device name you chose
1. Make the device public by pressing the "share" icon
1. Go to the ```Dashboards``` tab
1. import the dashboard from this repository, stored under https://github.com/winkj/aqdemo/blob/master/dashboard/air_quality_demo.json
1. Open the newly created dashboard (It's going to be called "Air Quality Demo"
1. Press the edit button
1. Select the entity aliases button, and make sure you have an alias named ```ess-input```, Filter Type ```Single Device```, Type ```Device```, Device ```CY8KIT```
1. Go back to the ```Dashboards``` tab
1. Make the Dashboard public by pressing the "share" icon, and store the URL you're getting from this dialog

Your dashboard is now ready to be used

## Device setup

This demo is supporting Cypress WICED SDK devices. 
