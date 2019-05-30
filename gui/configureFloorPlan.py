import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as img

config_x1 = 0
config_x2 = 0

NUM_UWB = 0 
NUM_BLE = 0

UWB_count = 0
BLE_count = 0

UWB_static = 0
BLE_static = 0

def onPress(event):

    global config_x1
    config_x1 = event.xdata

def onRelease(event):

    global config_x2
    config_x2 = event.xdata
    plt.close('all')

def placeNode(event):

    global UWB_static, BLE_static
    global UWB_count, BLE_count
    global NUM_UWB, NUM_BLE

    if UWB_count != NUM_UWB:

        UWB_static[UWB_count] = [event.xdata, event.ydata]
        UWB_count = UWB_count + 1
        plt.scatter(event.xdata, event.ydata, marker = '*', c='r')
        plt.pause(.01)

    elif BLE_count != NUM_BLE:
        
        BLE_static[BLE_count] = [event.xdata, event.ydata]
        BLE_count = BLE_count + 1
        plt.scatter(event.xdata, event.ydata, marker = '*', c='b')
        plt.pause(.01)

    if BLE_count == NUM_BLE:
        plt.close('all')

def configureLayout():

    global NUM_UWB, NUM_BLE
    global UWB_static, BLE_static

    file_path = input('FILE_PATH: ')
    NUM_UWB = int(input('NUM_UWB: '))
    NUM_BLE = int(input('NUM_BLE: '))

    UWB_static = [None] * NUM_UWB
    BLE_static = [None] * NUM_BLE

    print('Select reference distance.')

    fp = img.imread(file_path)

    plt.connect('button_press_event', onPress)
    plt.connect('button_release_event', onRelease)
    plt.imshow(fp, cmap='Greys_r')
    plt.axis('off')
    plt.tight_layout()
    plt.show()

    pix_y = len(fp)
    pix_x = len(fp[0])

    x_pix_dist = input('Reference distance (m): ')

    global config_x1, config_x2

    scale = float(x_pix_dist) / (config_x2 - config_x1)

    print('Place nodes.')
    plt.cla()
    plt.connect('button_press_event', placeNode)
    plt.connect('button_press_event', onPress)
    plt.imshow(fp, cmap='Greys_r', extent = [0, (pix_x * scale), 
                                             0, (pix_y * scale)]) 
    plt.tight_layout()
    plt.show()

    return [UWB_static, BLE_static, [0, (pix_x * scale), 0, (pix_y * scale)], 
            file_path]
