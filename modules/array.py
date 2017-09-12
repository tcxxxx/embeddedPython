import numpy as np

def func(narray):
    narray = np.asarray(narray)
    print("type: " + str(type(narray))) 
    print("shape: " + str(narray.shape))
    print("sum: " + str(sum(narray)))
    # for element in narray:
    #    print(element)
    # print(len(narray))
    if sum(narray) % 2:
	return True
    else:
	return False
