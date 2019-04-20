import os, sys
import subprocess
import numpy as np


# level_size = int(sys.argv[1])
# insert_num = int(sys.argv[2])
# write_latency = int(sys.argv[3])
# sensitive_count = int(sys.argv[4])
# con_method = int(sys.argv[5])
arg_list = []
arg_list.append(int(sys.argv[1]))
arg_list.append(int(sys.argv[2]))
arg_list.append(int(sys.argv[3]))
arg_list.append(int(sys.argv[4]))
arg_list.append(int(sys.argv[5]))

subprocess.call(["./plevel"] + arg_list)