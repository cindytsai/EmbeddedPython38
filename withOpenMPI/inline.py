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
print ("----------------------")
print ("before yt_set_parameter(), libyt.prop1 = ", libyt.prop1)

def yt_inline():
	print ("======================")
	print ("myrank = ", myrank)
	print ("after yt_set_parameter, libyt.prop1 = ", libyt.prop1)
	
	## gather information from other rank
	################################################
	libyt.prop2 = MPI.COMM_WORLD.gather(libyt.prop1, root=0)
	
	## broadcast libyt.prop1 in rank0 to other ranks
	################################################
	# libyt.prop2 = MPI.COMM_WORLD.bcast(libyt.prop1, root=0)	
	# print ("libyt.prop2 = ", libyt.prop2)
	
	if myrank == 0:
		print ("----------------------")
		print ("gather information libyt.prop1 from other ranks, ")
		print ("and stored in libyt.prop2...")
		print ("libyt.prop2 = ", libyt.prop2)


