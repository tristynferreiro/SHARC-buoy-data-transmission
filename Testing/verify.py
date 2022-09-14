"""
This is a basic python script which compares the contents of one file to another
file
"""

print("************************")
print("* Welcome to verify.py *")
print("************************")
print("*  pls type e to exit  *")
print("************************")
#Get the names of the files to compare
#FILE 1
while(1):
    filename1 = input("first file name: ")
    
    if (filename1 == "e"): exit() #exit program
    
    try:
        f1 = open(filename1, "r")
        break
    except (FileNotFoundError):
        print("Wrong file or file path ["+filename1+"]")
        continue

#FILE 2
while(1):
    filename2 = input("second file name: ")
    
    if (filename2 == "e"): exit() #exit program
    
    try:
        f2 = open(filename2, "r")
        break
    except (FileNotFoundError):
        print("Wrong file or file path ["+filename2+"]")
        continue

#compare all lines

lines1 = f1.readline()
numlines=0;
while lines1:
    lines2 = f2.readline()
    numlines+=1

    values1 = lines1.split(',')
    values2 = lines2.split(',')
    valueNum = 0;
    for value in values1:
        if(numlines==1):
            continue
        elif(value.rstrip("0") != values2[valueNum].rstrip("0")):
            if (value == "0" or values2[valueNum]=="0"):
                continue
            print("FILES NOT THE SAME")
            print("Difference [line",numlines,"]:\n["+filename1+"]: "+lines1+"\n["+filename2+"]: "+lines2)
            exit();
        valueNum+=1;
    #if(lines1 != lines2):
        
    lines1 = f1.readline()
    
print("The files: "+filename1+" & "+filename2+" are the same")
#lines1 = f1.read();
#lines2 = f2.read();

#if(lines1 == lines2): print("The files: "+filename1+" & "+filename2+" are the same")
#else: print("Not the same")

f1.close()
f2.close()
