from multiprocessing import Process, Queue

import socket
import time
import datetime

from math import cos, sin, radians
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as img

import json

from random import seed, uniform

from particleFilter import generateParticles, predict, update, resample, estimate
from configureFloorPlan import configureLayout

packetQueue = Queue()

def serverTask():

    host = "192.168.43.36"
    port = 65432
    
    serverSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    serverSock.bind((host, port))

    while True:

        data, addr = serverSock.recvfrom(1024)

        serverSock.sendto(bytes([len(data)]), addr)

        if data == b'':
            continue
            
        data = data.decode()
        print(data)

        data = json.loads(data)      
        data["Timestamp"] = str(datetime.datetime.now())

        packetQueue.put(data)


def multilateration(dist_list, static_loc):

    static = [[static_loc[0][0], static_loc[0][1], dist_list[0]], 
              [static_loc[1][0], static_loc[1][1], dist_list[1]], 
              [static_loc[2][0], static_loc[2][1], dist_list[2]]]
    
    static = np.array(static)

    A = []

    for i in range(0, len(static)):
        
        A.append([2 * (static[-1][0] - static[i][0]),
                  2 * (static[-1][1] - static[i][1])])

    A = np.array(A)

    b = []

    for i in range(0, len(static)):
        
        b.append([static[i][2]**2 - static[-1][2]**2 - 
                  static[i][0]**2 - static[i][1]**2 + 
                  static[-1][0]**2 + static[-1][1]**2])

    b = np.array(b)

    x, y = np.linalg.lstsq(A, b, rcond=None)[0]

    return x[0], y[0]


def propogationModel(rssi_list):
    
    n = 1.8527 #solve eqn
    d0 = 1 # RSS_0 found one metre from node

    RSSI0 = -65

    dist_list = []

    for i, RSSI in enumerate(rssi_list):

        d = 10 ** ((RSSI0 - RSSI) / (10 * n))
        dist_list.append(d)

    return dist_list 


def displayMsg(msgDict):

    print("--------")
    print('Timestamp: {}'.format(msgDict["Timestamp"]))

    if "ble" in msgDict.keys():
        print('BLE RSSI: {} dB'.format(msgDict["ble"]))
       
    if "uwb" in msgDict.keys():
        #print('UWB TOF: {} m/s'.format(int(msgDict["uwb"]) / (3 * 10**8)))
        print('UWB: {}'.format(msgDict["uwb"]))


def graphic(file_path, UWB_static, BLE_static, extent):
    
    UWB_x = [item[0] for item in UWB_static]
    UWB_y = [item[1] for item in UWB_static]

    BLE_x = [item[0] for item in BLE_static]
    BLE_y = [item[1] for item in BLE_static]

    fp = img.imread(file_path)

    plt.axis((extent[0], extent[1], extent[2], extent[3]))
    plt.imshow(fp, cmap='Greys_r', extent = extent)
    plt.scatter(UWB_x, UWB_y, marker = '*', c='r')
    plt.scatter(BLE_x, BLE_y, marker = '*', c='b')
    plt.tight_layout()
    plt.pause(.01)

def displayTask():
    
    H_LEN = 10
    N = 1000
    count = 0

    config = configureLayout()

    UWB_static = config[0]
    BLE_static = config[1]
    extent = config[2]
    file_path = config[3]

    graphic(file_path, UWB_static, BLE_static, extent)

    particles = generateParticles((extent[0], extent[1]), (extent[2], 
                                   extent[3]), (0, 2 * np.pi), N)
    weights = np.zeros(N)
    
    print('Start client.')

    server = Process(target = serverTask)
    server.start()

    dAngle = 0
    oldAngle = 0
    currentAngle = 0

    dDist = 0
    old_x = 0
    old_y = 0

    while True:

        if count != 1:
            predict(particles, (dAngle, dDist), (.2, .1), 0.5, N)
        
        payload = packetQueue.get()

        if payload["ble"][3] != 127:

            if count % H_LEN == 0:
                plt.cla()
                graphic(file_path, UWB_static, BLE_static, extent)
        
            count = count + 1
                    
            plt.scatter(uniform(0.9, 7.2), uniform(11.45, 11.95), 
                                marker='.', color='y')
            plt.pause(.01)

            continue
                
        elif payload["ble"][4] != 127:

            if count % H_LEN == 0:
                plt.cla()
                graphic(file_path, UWB_static, BLE_static, extent)
        
            count = count + 1

            plt.scatter(uniform(49, 54), uniform(10.7, 12), 
                                marker='.', color='y')
            plt.pause(.01)

            continue

        if 65535 in payload["uwb"]:
            continue                

        displayMsg(payload)

        payload["uwb"][:] = [x / 100 for x in payload["uwb"]] 
        
        ble_x, ble_y = multilateration(propogationModel(payload["ble"]), 
                                             UWB_static)

        sample_x, sample_y = multilateration(payload["uwb"], UWB_static)

        dDist = np.linalg.norm(np.array([[sample_x, sample_y]]) - 
                               np.array([[old_x, old_y]]))


        currentAngle = np.arctan2(sample_x - old_x, sample_y - old_y)
        dAngle = oldAngle - currentAngle

        oldAngle = currentAngle
        old_x = sample_x
        old_y = sample_y
        
        update(particles, weights, (sample_x, sample_y), 
               .2, np.array(UWB_static))
    
        resample(particles, weights, N)

        est_x, est_y = estimate(particles, N)
        print('Sample: ', sample_x, sample_y)
        print('Estimation: ', est_x, est_y)
        print('-------------')

        if count % H_LEN == 0:
            plt.cla()
            graphic(file_path, UWB_static, BLE_static, extent)
        
        plt.scatter(est_x, est_y, marker='.', color='g')
        plt.scatter(ble_x, ble_y, marker='.', color='b')
        plt.pause(.01)
        
        count = count + 1
 
displayTask()
