import os

if __name__ == '__main__':
    path = '/home/cui/os/proj2/cs302-process-memory-tracker/memcheck/build/test_c'
    cmd = 'LD_PRELOAD=/home/cui/os/proj2/cs302-process-memory-tracker/memcheck/build/libmemory_check.so ' + path
    # a = os.popen(cmd,'r')
    a = os.popen(cmd)
    print(a.read())

