"""
    This script is used for cleaning the serial data received from the stm32f0 before decompression and 
    decryption.
    Known issue: sometimes data that doesnt end in ; is excluded from the senor_data.txt file.
"""

# Global Variables
substring = ","

#Functions

def clean_transmittedCompressioData(fileLines,startCharIndex, numLines):
    cleaned = []
    elements = 0
    # removes unwanted newlines and blank spaces caused by transmission formatting
    for x in range(startCharIndex[0],numLines-1): 
        index = fileLines[x].find(substring)    # if the substring exists in the line
        line = fileLines[x]
        print(x)
        cleaned.append(line[:index])         # remove the substring       
        elements = elements+1
    return cleaned              # remove trailing , delimiter               
    
def clean_sensorData(sensorData):
    cleanedSensorData = []
    for data in sensorData:
        index = data.find('}')                    # only save data before the terminating character
        cleanedSensorData.append(data[:index])
    return cleanedSensorData

def main():
    numLines=1                      # number lines in file
    fileLines = []                  # lines in file

    originalSensorData = []
    cleanedSensorData = []          # real sensor data

    startCharIndex = []             # START sequence line number
    cleanedCompressedData = ""      # real compressed bits
    elements = 0                    # number of compressed bits
    
    sensorDataFile = "sensor_data.txt"
    compressedDataFile = "compressed_data.txt"

    f = open("enc_comp.txt", "r")
    # Read in lines
    for line in f:
        fileLines.append(line)
        if(';' in line): #each data value 
            originalSensorData.append(line)
        if('#' in line):
            startCharIndex.append(numLines)
        numLines = numLines+1

    f.close()

    cleanedSensorData = clean_sensorData(originalSensorData)
    f = open(sensorDataFile, "a")
    for x in cleanedSensorData:
        f.write("\n")
        f.write(x)
    f.close()

    cleanedCompressedData= clean_transmittedCompressioData(fileLines, startCharIndex,numLines)

    f = open(compressedDataFile, "a")
    for y in range(len(cleanedCompressedData)-1):
        if (y ==0):
            f.write(cleanedCompressedData[y])
            continue
        f.write("\n")
        f.write(cleanedCompressedData[y])
    f.close()

    print(cleanedSensorData)
    print(cleanedCompressedData)
    print(elements)
    

if __name__ == "__main__":
    main() 


