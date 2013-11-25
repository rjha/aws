import requests
import json
import sys
import time 
import io
import serial

def post_data_point (bencode) :
    url = "http://localhost:9090/sensordb/v1/datapoint"
    post_data = bencode + "\r\n"
    headers = { 'Content-type': 'text/plain' }
    r = requests.post(url, data=post_data, headers=headers)
    print r.status_code
    print r.text

    return 

def process_dict(line) :
    # valid bencode dict should 
    # 1. start with d
    # 2. end with e
    # 3. remove trailing \r and \n etc.

    line = line.strip()
    size = len(line) 
    # minimum valid bencode dict is 'de'
    if (size > 2) :
        if (not line.isspace()) and (line[0] == 'd') and (line[size-1] == 'e') :
            print line 
            # post_data_point(line)
	return 


ser  = None
port = '/dev/ttyUSB0' 
baud = 9600

while True :
    try:
        ser = serial.Serial(port, baud)
        break 
    except serial.serialutil.SerialException :
        #not open yet
        time.sleep(1)

# port open

while True:
    try:
        buffer = ''
        while 1:
            ch = ser.read(1) 
            if(ch == '\n'):
                process_dict(buffer)
                break
            else :
                buffer = buffer + ch  

    except serial.serialutil.SerialException as e1:
        print "serial error({0}): {1}".format(e1.errno, e1.strerror)
        pass


