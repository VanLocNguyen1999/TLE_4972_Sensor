define reset
	monitor reset halt
end

define stopd 
	monitor shutdown
	kill
	quit
end

define reloadd
	file selex_smc.elf
	load
	thbreak main
end

#
# J-LINK GDB SERVER initialization
#
# This connects to a GDB Server listening
# for commands on localhost at tcp port 2331
target remote localhost:2331

# CPU core initialization (to be done by user)
#

# Setup GDB FOR FASTER DOWNLOADS
#set remote memory-write-packet-size 1024
#set remote memory-write-packet-size fixed
# Load the program executable called "image.elf"
file selex_smc.elf
load selex_smc.elf
