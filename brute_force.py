#!/usr/bin/env python2.7
#Created by Spencer Bouck 10/4/19

import os
import string

"""
ironFile = open("Iron.png","rb")
ironText = ironFile.read()
ironData = bytearray(ironText[0:4])
""" 

pdfData = [0x25, 0x50, 0x44, 0x46]
pngData = [137, 80, 78, 71]

fileType = raw_input("Enter 0 for pdf, 1 for png: ")
expectedData = [pdfData, pngData][int(fileType)]






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
    ironBit = getBit(expectedData, index)
    return encBit == ironBit

def testKey(key):
    
    os.system('./decrypt ' + key.getHex())
    
    steps = 0
    for j in range (0, 32, 8):
        for i in range(8):
            
            if (isDecrypted(24 - j + i)):
                #key.lock(j + i)
                pass
                
            else:
                key.toggleBit(j + i)
                #key.lock(j + i)
                steps += 1
            
    

def verifyText():
    os.system('./decrypt ' + key.getHex())
    encFile = open("output-dec","rb")
    encText = encFile.read()
    
    for letter in encText:
        if(letter not in string.printable):
            return False
    return True


key = Key(4)


"""
for char0 in string.lowercase:
    ironData[0] = char0
    for char1 in string.lowercase:
        ironData[1] = char1
        print ("Trying prefix %s" %ironData[0:2])
        for char2 in string.lowercase:
            ironData[2] = char2
            for char3 in string.lowercase:
                ironData[3] = char3
                testKey(key)
                if (verifyText()):
                    break
                #print ("Trying %s" %ironData)
"""
#while (key.increment()):
#    testKey(key)

testKey(key)

print ("Key: %s" %(key.getHex()))
os.system('./decrypt ' + key.getHex())

raw_input("Press enter to exit")