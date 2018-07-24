#Disclaimer: this code is not tested, may not work, just for the purposes of this exercise
import serial
import smbus
import threading
import time

bus = smbus.SMBus(1)
arduino_addr = 0x48

sp = serial.Serial(
        port='/dev/ttyACM0',
        baudrate=9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
)

def check_fuel_sensor():
	sp.write("^OK\r\n" )
	bus.write_byte(arduino_addr,0xFE) 
	high_byte = bus.read_byte(arduino_addr)
	low_byte = bus.read_byte(arduino_addr)
	sensor_value = (high_byte<<8) | low_byte
	sp.write(str(sensor_value))
	sp.write("\r\n" )
	
fuel_sensor_thread = threading.Timer(1, check_fuel_sensor)
fuel_sensor_thread.start()

sp.write("^RESET\r\n" )
while True:
    rcv = sp.readline()
	if rcv == "AT+RLYON=1\r\n":
		sp.write("^OK\r\n" )
		bus.write_byte(arduino_addr,0xA1)
	elif rcv == "AT+RLYON=2\r\n":
		sp.write("^OK\r\n" )
		bus.write_byte(arduino_addr,0xB1)
	elif rcv == "AT+RLYOFF=1\r\n":
		sp.write("^OK\r\n" )
		bus.write_byte(arduino_addr,0xA0)
	elif rcv == "AT+RLYOFF=2\r\n":
		sp.write("^OK\r\n" )	
		bus.write_byte(arduino_addr,0xB0)
	elif rcv == "AT+SENS=?\r\n":
		sp.write("^OK\r\n" )
		bus.write_byte(arduino_addr,0xFE) 
		high_byte = bus.read_byte(arduino_addr)
		low_byte = bus.read_byte(arduino_addr)
		sensor_value = (high_byte<<8) | low_byte
		sp.write(str(sensor_value))
		sp.write("\r\n" )
	else:
		sp.write("^ERROR: Invalid command\r\n" )