import emb
import time
import numpy as np

def yt_inline():

    gid = 100000000

    while True:
        
        array = emb.numpy_array(gid,"field_name")
        # array = np.zeros(2, dtype=np.double)
        
        # array = np.arange(gid, dtype=np.double)
        
        print(array)
        print(type(array))
        print(array.flags)
        
        time.sleep(5)
        
    array = emb.numpy_array3D(3, "field_name")
    print(array)
    print(type(array))

    return 0


