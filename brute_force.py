#!/usr/bin/env python2.7
#Created by Spencer Bouck 10/4/19

import os

ironFile = open("Iron2.png","rb")
ironText = ironFile.read()
ironData = bytearray(ironText[0:4])

print(ironData)





hexCharacters = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f']

    
def getBit(byteList, bit):
    byte = int(bit / 8)
    bit = bit % 8
    currentByte = byteList[byte]
    return (currentByte >> bit) & 1

class Key:
    def __init__(self, length):
        self.length = length
        self.byteList = bytearray()
        self.isBitLocked = []
        for i in range(self.length):
            self.byteList += b'\0'
            self.isBitLocked.extend([False] * 8)
            
    def binaryLength(self):
        return self.length * 8
    
    def getHex(self):
        return ''.join('{:02x}'.format(x) for x in self.byteList)
    
    def lock(self, bit):
        self.isBitLocked[bit] = True
    
    def getBit(self, bit):
        return getBit(self.byteList, bit)
    
    def toggleBit(self, bit):
        byte = int(bit / 8)
        bit = bit % 8
        currentByte = self.byteList[byte]
        modification = (1 << bit)
        self.byteList[byte] = currentByte ^ modification
    
    # returns whether it was able to increment the bit
    def increment(self):
        for i in range(self.binaryLength()):
            if (self.isBitLocked[i]): # locked bit
                continue
            elif (self.getBit(i) == 1): # unlocked 1 bit
                self.toggleBit(i)
            else: # unlocked 0 bit
                self.toggleBit(i)
                return True
        
        return False # was unable to increment
    
    def printKey(self):
        print(self.byteList)











def isDecrypted(index):
    
    encFile = open("output-dec","rb")
    encText = encFile.read()
    encData = bytearray(encText[0:4])
    
    encBit = getBit(encData, index)
    ironBit = getBit(ironData, index)
    return encBit == ironBit

def testKey(key):
    
    os.system('./decrypt ' + key.getHex())
    
    steps = 0
    for j in range (0, 32, 8):
        for i in range(8):
            
            if (isDecrypted(24 - j + i)):
                
                key.lock(j + i)
            else:
                key.toggleBit(j + i)
                steps += 1
            
    #print ("%i steps expected to remain" %steps)
    

key = Key(4)


testKey(key)
#while (key.increment()):
#    testKey(key)

print ("Key: %s" %(key.getHex()))

raw_input("Press enter to exit")