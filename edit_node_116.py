import os
from numpy import *
import matplotlib.pyplot as plt

testfile = "test.dat"
myfile = "p116.dat"
datafile = "output.dat"
resfile = "output.res"

def print_file(file):
    for line in file.readlines():
        print(line)


node = input("node to investigate = ")

######### adjust the .dat file to investigate node n

with open(myfile,"r") as f:
    datafile = open(datafile,"w")
    line_num = 1
    dataOut = []
    for line in f:
        unsplit = line
        line = line.split()
        if(line_num == 7):
            del line[5]
            line.append(str(node) + "\n" )
            write_line = ' '.join(line)
            datafile.write(write_line)
        else:
            datafile.write(unsplit)
        line_num += 1
    f.close()
    datafile.close()


###command lines to run the program

os.system("cat output.dat")
os.system("./p116.exe")
##os.system("less output.res")



#####get the data out of the .res file to use for imaging

res = open(resfile, "r")
line_num = 1
t_lst = []
x_lst = [] 
for line in res.readlines():
    unsplit = line
    line = line.split()
    if (line_num > 5):
        t_lst.append(line[0])
        x_lst.append(line[1])
    line_num+=1
res.close()

print(t_lst)
print()
print(x_lst)


