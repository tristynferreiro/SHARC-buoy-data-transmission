# Scripts
## pyserial.py

#### using the script:

    

## clean.py
This script is used for cleaning the data received from the stm32f0 (collected using the pyserial.py script).
It:
- removes all formatting errors caused by HAL_transmit
- puts all the sensor data reading values into an array
- puts all the cleaned compressed bits into an array
- counts the number of compressed bits.
- prints all these values to the terminal.

#### using the script:
1. Update the script with the name of the file to clean.
2. run the script:
    $ python3 clean.py
    


