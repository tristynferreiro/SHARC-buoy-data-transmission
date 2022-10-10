"""
    This script is used for cleaning the serial data received from the stm32f0 before decompression and 
    decryption.
"""

# Global Variables
substring = ","

#Functions

def clean_transmittedCompressioData(fileLines,startCharIndex, numLines):
    cleaned = ""
    elements = 0

    # removes unwanted newlines and blank spaces caused by transmission formatting
    for x in range(startCharIndex+1,numLines): 
        index = fileLines[x].find(substring)    # if the substring exists in the line
        cleaned+=fileLines[x][:index+1]         # remove the substring       
        elements = elements+1
    return cleaned[:-1], elements               # remove trailing , delimiter               
    
def clean_sensorData(sensorData):
    cleanedSensorData = []
    for data in sensorData:
        index = data.find('}')                    # only save data before the terminating character
        cleanedSensorData.append(data[:index])
    return cleanedSensorData

def main():
    numLines=0                      # number lines in file
    fileLines = []                  # lines in file

    originalSensorData = []
    cleanedSensorData = []          # real sensor data

    startCharIndex = []             # START sequence line number
    cleanedCompressedData = []      # real compressed bits
    elements = []                   # number of compressed bits
    
    f = open("transmission.txt", "r")
    # Read in lines
    for line in f:
        fileLines.append(line)
        if('}' in line):
            originalSensorData.append(line)
        if('#' in line):
            startCharIndex.append(numLines)
            #if(startCharIndex == -1):
              #  startCharIndex=numLines
             #   continue;
        numLines = numLines+1

    f.close()
    cleanedSensorData = clean_sensorData(originalSensorData)

    for i in range(0,len(startCharIndex)):
        if(i+1>=len(startCharIndex)):
            cleaned, el = clean_transmittedCompressioData(fileLines, startCharIndex[i],numLines)
            cleanedCompressedData.append(cleaned)
            elements.append(el)
            startCharIndex=numLines
            break;
        cleaned, el = clean_transmittedCompressioData(fileLines, startCharIndex[i],startCharIndex[i+1])
        cleanedCompressedData.append(cleaned)
        elements.append(el)
        startCharIndex=numLines
    #cleanedCompressedData, elements = clean_transmittedCompressioData(fileLines, startCharIndex,numLines)

    print(cleanedSensorData)
    print(cleanedCompressedData)
    print(elements)
    

if __name__ == "__main__":
    main() 


