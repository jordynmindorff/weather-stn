import board
import digitalio
import time
import os


import adafruit_bme680
import adafruit_veml7700

import busio
from digitalio import DigitalInOut
import adafruit_requests as requests
import adafruit_esp32spi.adafruit_esp32spi_socket as socket
from adafruit_esp32spi import adafruit_esp32spi

i2c = board.I2C()
veml7700 = adafruit_veml7700.VEML7700(i2c)

cs = digitalio.DigitalInOut(board.D10)
spi = board.SPI()
bme680 = adafruit_bme680.Adafruit_BME680_SPI(spi, cs)

bme680.sea_level_pressure = 1013.25

esp32_cs = DigitalInOut(board.ESP_CS)
esp32_ready = DigitalInOut(board.ESP_BUSY)
esp32_reset = DigitalInOut(board.ESP_RESET)

spiWifi = busio.SPI(board.SCK1, board.MOSI1, board.MISO1)

esp = adafruit_esp32spi.ESP_SPIcontrol(spiWifi, esp32_cs, esp32_ready, esp32_reset)

requests.set_socket(socket, esp)

print("Connecting to AP...")
while not esp.is_connected:
    try:
        esp.connect_AP(os.getenv("WIFI_SSID"), os.getenv("WIFI_PASSWORD"))
    except OSError as e:
        print("could not connect to AP, retrying: ", e)
        continue

print("Connected to", str(esp.ssid, "utf-8"))

def get_weather():

    return {
        "temperature": bme680.temperature,
        "gas_level": bme680.gas,
        "humidity": bme680.humidity,
        "pressure": bme680.pressure,
        "altitude": bme680.altitude,
        "light_level": veml7700.lux,
    }

# To get our first 'bad' reading out of the way
get_weather()
time.sleep(5)

while True:
    # Send API data...
    print(get_weather()["temperature"])
    time.sleep(5) # TODO: Make this a better kind of recurring job

