import serial
import smbus
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
		
	else:
		sp.write("^ERROR: Invalid command\r\n" )