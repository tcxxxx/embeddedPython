def func(a):
    print("type: " + str(type(a))) 
    print("length: " + str(len(a)))
    if sum(a) > 10:
	return True
    else:
	return False
