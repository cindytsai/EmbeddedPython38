from mpi4py import MPI
import libyt
hwmess = 'Hello, World! I am process %d of %d on %s.'
myrank = MPI.COMM_WORLD.Get_rank()
nprocs = MPI.COMM_WORLD.Get_size()                   
procnm = MPI.Get_processor_name()
print (hwmess % (myrank, nprocs, procnm))
print ("----------------------")
print ("%s" % ("prop1" in dir(libyt)))
print ("%s" % ("prop2" in dir(libyt)))
print ("%s" % ("prop3" in dir(libyt)))

def yt_inline():
	pass