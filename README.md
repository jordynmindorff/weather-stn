# Arduino Weather Station Application

LINK TO APPLICATION
LINK TO API

## The Premise

I have an Arduino that has WiFi connectivity. It is connected to two sensor breakout boards via SPI and I2C for weather conditions.

The Arduino makes a POST request to the API I created every 3 minutes.

The web application serves as a VERY simple portal to view the most recent data. If you want to see me work on front-end stuff for some reason, please run away now and look elsewhere before you are scarred by the ugliness of this application...

**Beware, I put basically zero emphasis on security or scalability in creating this, so I can think of multiple ways you will be able to break something if you try. But, please don't 🙂.**

## Details

Here's a photo of the hardware:
\<INSERT PHOTO\>

I'm using an Arduino RP2040 Nano Connect and to Adafruit breakout boards - the VEML7700 for light readings and the BME680 for environmental info.

The code for the Arduino is in the outermost file. Nothing overly fancy. The whole goal of this project was just for me to re-familiarize myself with python and django after years of not using it (see the API) and take a crack at Arduinos for the first time.

Prior to this, my only microcontroller experience was with an STM32 Nucleo board in ECE 198 at Waterloo. In that project, I fumbled my way through without any libraries, implementing my own I2C calls and PWM Buzzer code.
