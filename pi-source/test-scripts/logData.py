import smbus
import time

# for RPI version 1, use "bus = smbus.SMBus(0)"
bus = smbus.SMBus(1)
time_str = str(time.strftime("%Y%m%d-%H%M%S"))
dir_name = "log-data/"
file_name = dir_name+time_str
f = open(file_name, "w")
# This is the address we setup in the Arduino Program

address = 0x04

def writeNumber(value):
    bus.write_byte(address, value)
    # bus.write_byte_data(address, 0, value)
    return -1

def readNumber():
    number = bus.read_byte(address)
    # number = bus.read_byte_data(address, 1)
    return number

while True:
    time.sleep(0.2)
    height = readNumber()
    print "Height Read: ", height
    f.write(str(height))
    f.write('\n')
