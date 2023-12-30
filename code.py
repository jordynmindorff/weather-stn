import board
import adafruit_bme680
import time
import digitalio

cs = digitalio.DigitalInOut(board.D10)
spi = board.SPI()
bme680 = adafruit_bme680.Adafruit_BME680_SPI(spi, cs)

bme680.sea_level_pressure = 1013.25

while True:
    print('Temperature: {} C'.format(bme680.temperature))
    print('Gas: {} ohms'.format(bme680.gas))
    print('Humidity: {}%'.format(bme680.humidity))
    print('Pressure: {}hPa'.format(bme680.pressure))
    print('Altitude: {}m'.format(bme680.altitude))

    time.sleep(5)
