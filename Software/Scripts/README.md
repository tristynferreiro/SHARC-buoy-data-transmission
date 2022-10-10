# Scripts
## pyserial.py
This script is used for reading and saving serial data recieved from the stm32f0.
It:
- reads in serial data
- prints data recieved out to the terminal and saves it to a file

#### using the script:
1. Change port to the correct port for your device (this can be found under device manager for windows operating systems)
2. Change the file name to that which you want the data to be saved to 
3. run the script (this can be done via terminal or an IDE)
    - using terminal: 
        $ python3 serialRecieve.py
    - using IDE:
        Simply open the script and run as normal (for Visual Studio Code, you would simoly press the play button (triangle) in the top right hand corner)

NOTE: if you are using Visual Studio Code to run the script, please ensure you have pip-packages, pylance and python extensions installed or you may have trouble with running the script

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
    


