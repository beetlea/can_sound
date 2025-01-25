import serial
import time
from tkinter import *
from tkinter import ttk
import threading
import csv


list_can1 = []
list_can2 = []
list_common = []
stop = 0
def save_data():
    global stop
    stop = 1
    with open('adapter.txt', 'w') as f:
        for line in list_can1:
            f.write(f"{line}\n")
        
    with open('bus.txt', 'w') as f:
        for line in list_can2:
            f.write(f"{line}\n")
        
    with open('common.txt', 'w') as f:
        for line in list_common:
            f.write(f"{line}\n")
    stop = 0
root = Tk()
root.title("CAN парсер")
root.geometry("250x200")
 
# стандартная кнопка
btn = ttk.Button(text="Сохранить", command=save_data)
btn.pack()

ST_ID  =  0
EXT_ID =  1

TYPE_ID = ST_ID
BYTE_ID1 = 1
BYTE_ID2 = 65
BYTE_ID3 = 65
BYTE_ID4 = 65
DLC_STD = 8

arduino = serial.Serial(port='COM14',   baudrate=230400, timeout=3)




def read_com():
    global stop
    start = time.time()
    cur = 0
    while True:
        while stop:
            stop = stop
        data = list(arduino.readline())
        arduino.flush()
        length = len(data)
        
        #for i in range(length):
            
        if len(data) > 0:
            save_data = ""
            try:
                save_data = "CAN{0}, ID={1}, DLC={2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}".format(data[0], \
                    hex((data[1]<<24)|(data[2]<<16)|(data[3]<<8)|(data[4])), data[5], hex(data[6]), hex(data[7]),\
                        hex(data[8]),hex(data[9]),hex(data[10]),hex(data[11]),hex(data[12]),hex(data[13]))
                
                save_data = str(time.time()) + " " + save_data
                print(save_data)

                if data[0] == 0:
                    list_can1.append(save_data)
                else:
                    list_can2.append(save_data)
                    
                list_common.append(save_data)
            except:
                continue
            
        
x = threading.Thread(target=read_com)
x.start()
root.mainloop()