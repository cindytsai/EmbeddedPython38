from mpi4py import MPI
import libyt
hwmess = 'Hello, World! I am process %d of %d on %s.'
myrank = MPI.COMM_WORLD.Get_rank()
nprocs = MPI.COMM_WORLD.Get_size()                   
procnm = MPI.Get_processor_name()
print (hwmess % (myrank, nprocs, procnm))

print ("----------------------")
print ("prop1 exist = %s" % ("prop1" in dir(libyt)))
print ("prop2 exist = %s" % ("prop2" in dir(libyt)))
print ("prop3 exist = %s" % ("prop3" in dir(libyt)))
print (libyt.prop1)

def yt_inline():
	print("myrank = ", myrank)
	print (libyt.prop1)
