import numpy as np
import sys
# print(sys.path)

def func(narray):
    narray = np.asarray(narray)
    print("type: " + str(type(narray))) 
    print("length: " + str(len(narray)))
    if sum(narray) > 10:
	return True
    else:
	return False
