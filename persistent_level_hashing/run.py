import os, sys
import subprocess
import numpy as np


# level_size = int(sys.argv[1])
# insert_num = int(sys.argv[2])
# write_latency = int(sys.argv[3])
# sensitive_count = int(sys.argv[4])
# con_method = int(sys.argv[5])
arg_list = []
arg_list.append(str(sys.argv[1]))
arg_list.append(str(sys.argv[2]))
arg_list.append(str(sys.argv[3]))
arg_list.append(str(sys.argv[4]))
arg_list.append(str(sys.argv[5]))
if int(arg_list[4]) > 4:
	print "error! incorrect consistency method"
	sys.exit(0)

#print arg_list[4]
if arg_list[4] == '2':
	with open('/sys/kernel/memtrack/pwt', 'w') as the_file:
		the_file.write('1')
	with open('/sys/kernel/memtrack/pcd', 'w') as the_file:
		the_file.write('1')
elif arg_list[4] == '3':
        with open('/sys/kernel/memtrack/pcd', 'w') as the_file:
                the_file.write('1')
elif arg_list[4] == '4':
        with open('/sys/kernel/memtrack/pwt', 'w') as the_file:
                the_file.write('1')
else:
        with open('/sys/kernel/memtrack/pwt', 'w') as the_file:
                the_file.write('0')
        with open('/sys/kernel/memtrack/pcd', 'w') as the_file:
                the_file.write('0')


subprocess.call(["./plevel"] + arg_list)
