import os
import time

proc_path="/proc"

PAGE_SIZE=int(os.sysconf("SC_PAGE_SIZE")/1024) # 4096 bytes / 1024 = 4 KiB

class Unbuffered(object):
   def __init__(self, stream):
       self.stream = stream
   def write(self, data):
       self.stream.write(data)
       self.stream.flush()
   def close(self):
       self.stream.close()
   def flush(self):
      self.stream.flush()

def clear():
    os.system("clear")

def get_RSS(pid: int) -> int:
    statm_path=os.path.join(proc_path, str(pid), "statm")
    with open(statm_path, "r") as statm:
        pages=int(statm.readline().split()[1]) # page numbers of RSS

    return pages*PAGE_SIZE # RSS (KiB) resident set size

def get_process_name(pid: int) -> str:
    status_path=os.path.join(proc_path, str(pid), "status")
    with open(status_path, "r") as status:
        name=status.readline().split()[1] # first line, the format is "Name: xxx"

    return name

def show_mem_usage():
    print("PID\tMemory (KiB)\tName")

    for pid in os.listdir(proc_path):
        if not pid.isdigit():
            continue

        pid=int(pid)

        mem_usage="{} KiB".format(get_RSS(pid))
        if len(mem_usage)<8:
            mem_usage+="\t"

        print("{}\t{}\t{}".format(pid, mem_usage, get_process_name(pid)))

if __name__ == "__main__":
    # sys.stdout=Unbuffered(sys.stdout)
    # sys.stderr=Unbuffered(sys.stderr)
    
    while True:
        show_mem_usage()
        time.sleep(1)
        clear()