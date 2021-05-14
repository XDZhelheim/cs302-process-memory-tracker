from PyQt5 import QtCore, QtGui, QtWidgets

import sys
import os
import time
import datetime

PROC_PATH="/proc"

REFRESH_INTERVAL=2 # 数据刷新的时间间隔 (s)
CPU_USAGE_SAMPLING_INTERVAL=0.5 # CPU 时间的采样间隔 (s)

PAGE_SIZE=int(os.sysconf("SC_PAGE_SIZE")/1024) # 4096 bytes / 1024 = 4 KiB
# LOGICAL_CORES=int(os.popen("cat /proc/cpuinfo| grep \"processor\"| wc -l").read()) # CPU 逻辑核心数

def get_RSS(pid: int) -> int:
    """
    - RSS（Resident set size），使用top命令可以查询到，是最常用的内存指标，表示进程占用的物理内存大小。但是，将各进程的RSS值相加，通常会超出整个系统的内存消耗，这是因为RSS中包含了各进程间共享的内存。
    - PSS（Proportional set size）所有使用某共享库的程序均分该共享库占用的内存时，每个进程占用的内存。显然所有进程的PSS之和就是系统的内存使用量。它会更准确一些，它将共享内存的大小进行平均后，再分摊到各进程上去。
    - USS (Unique set size) 进程独自占用的内存，它是PSS中自己的部分，它只计算了进程独自占用的内存大小，不包含任何共享的部分。

    ---

    ```x
    /proc/[pid]/statm
    Provides information about memory usage, measured in pages.
    The columns are:

        size       (1) total program size
                    (same as VmSize in /proc/[pid]/status)
        resident   (2) resident set size
                    (same as VmRSS in /proc/[pid]/status)
        share      (3) shared pages (i.e., backed by a file)
        text       (4) text (code)
        lib        (5) library (unused in Linux 2.6)
        data       (6) data + stack
        dt         (7) dirty pages (unused in Linux 2.6)
    ```
    """
    statm_path=os.path.join(PROC_PATH, str(pid), "statm")
    try:
        with open(statm_path, "r") as statm:
            pages=int(statm.readline().split()[1]) # page numbers of RSS
    except FileNotFoundError:
        return 0

    return pages*PAGE_SIZE # RSS (KiB) resident set size

def get_process_name(pid: int) -> str:
    status_path=os.path.join(PROC_PATH, str(pid), "status")
    try:
        with open(status_path, "r") as status:
            name=status.readline().split()[1] # first line, the format is "Name: xxx"
    except FileNotFoundError:
        return ""

    return name

def get_total_cpu_time() -> int:
    cpu_stat_path=os.path.join(PROC_PATH, "stat")

    with open(cpu_stat_path, "r") as cpu_stat:
        cpu_time_components=cpu_stat.readline().split()[1:-1]
    cpu_time_components=list(map(int, cpu_time_components))
    total_cpu_time=sum(cpu_time_components)

    return total_cpu_time

def get_process_cpu_time(pid: int) -> int:
    process_stat_path=os.path.join(PROC_PATH, str(pid), "stat")

    try:
        with open(process_stat_path, "r") as process_stat:
            process_time_components=process_stat.readline().split()[13:17]
        process_time_components=list(map(int, process_time_components))
        process_cpu_time=sum(process_time_components)
    except FileNotFoundError:
        return 0

    return process_cpu_time

def get_process_cpu_usage_dict() -> dict:
    """
    - /proc/stat (the first row, 10 columns in total)
        - user 从系统启动开始累计到当前时刻，处于用户态的运行时间，不包含 nice值为负进程
        - nice 从系统启动开始累计到当前时刻，nice值为负的进程所占用的CPU时间
        - system 从系统启动开始累计到当前时刻，处于核心态的运行时间
        - idle 从系统启动开始累计到当前时刻，除IO等待时间以外的其它等待时间
        - iowait 从系统启动开始累积到当前时刻，IO 等待时间
        - irq 从系统启动开始累积到当前时刻，硬中断时间
        - softirq 从系统启动开始累积到当前时刻，软中断时间
        - stealstolen Which is the time spent in other operating systems when running in a virtualized environment
        - guest Which is the time spent running a virtual CPU for guest operating systems under the control of the Linux kernel
        - Time spent running a niced guest (virtual CPU for guest operating systems under the control of the Linux kernel)
    - Total CPU time = user + nice + system + idle + iowait + irq + softirq + stealstolen + guest

    ---

    - /proc/<pid>/stat (start from column 1)
        - column 14: utime 该任务在用户态运行的时间，单位为 jiffies
        - column 15: stime 该任务在核心态运行的时间，单位为 jiffies
        - column 16: cutime 累计的该任务的所有的 waited-for 进程曾经在用户态运行的时间，单位为 jiffies
        - column 17: cstime 累计的该任务的所有的 waited-for 进程曾经在核心态运行的时间，单位为 jiffies
    - Process CPU time = utime + stime + cutime + cstime

    ---

    - Process CPU Usage
        - 采样两个足够短的时间间隔的 (total_cpu_time_1, total_cpu_time_2) and (process_cpu_time_1, process_cpu_time_2)
        - cpu_usage = (process_cpu_time_2 - process_cpu_time_1) / (total_cpu_time_2 - total_cpu_time_1) * 100%
        - 注意：这里和 top 的结果不一样, 这样算出来的结果已经是在多核的情况下了, 例如我的 8 逻辑处理器的 i5, 跑一个 while true, 结果就是 12.5%, 而 top 命令把它再乘了 8 就是 100%
    """
    pid_cpu_usage_dict={}

    total_cpu_time_1=get_total_cpu_time()

    for pid in os.listdir(PROC_PATH):
        if not pid.isdigit():
            continue

        pid=int(pid)

        pid_cpu_usage_dict[pid]=get_process_cpu_time(pid)

    time.sleep(CPU_USAGE_SAMPLING_INTERVAL)

    total_cpu_time_2=get_total_cpu_time()

    for pid in os.listdir(PROC_PATH):
        if not pid.isdigit():
            continue

        pid=int(pid)

        try:
            pid_cpu_usage_dict[pid]=(get_process_cpu_time(pid) - pid_cpu_usage_dict[pid]) / (total_cpu_time_2 - total_cpu_time_1) * 100
        except KeyError:
            pass

    return pid_cpu_usage_dict

def network_usage_helper() -> tuple:
    net_dev_path=os.path.join(PROC_PATH, "net", "dev")
    
    with open(net_dev_path, "r") as net_dev:
        net_info=net_dev.readlines()[2:]

    recv=0; trans=0
    for line in net_info:
        lst=line.split()
        recv+=int(lst[1])
        trans+=int(lst[9])

    return (recv, trans)

def get_network_usage() -> tuple:
    """
    - /proc/net/dev

        The dev pseudo-file contains network device status
        information.  This gives the number of received and sent
        packets, the number of errors and collisions and other
        basic statistics.

    ```x
    Inter-|   Receive                                                |  Transmit
     face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed
       lo: 2776770   11307    0    0    0     0          0         0  2776770   11307    0    0    0     0       0          0
     eth0: 1215645    2751    0    0    0     0          0         0  1782404    4324    0    0    0   427       0          0
     ppp0: 1622270    5552    1    0    0     0          0         0   354130    5669    0    0    0     0       0          0
     tap0:    7714      81    0    0    0     0          0         0     7714      81    0    0    0     0       0          0
    ```
    
    ---

    (Receive, Transmit) KB/s
    """
    recv_1, trans_1=network_usage_helper()

    time.sleep(1)

    recv_2, trans_2=network_usage_helper()

    recv_KBps=(recv_2-recv_1)/1024
    trans_KBps=(trans_2-trans_1)/1024

    return (recv_KBps, trans_KBps)

def get_processes_list() -> list:
    ps_list=[]

    pid_cpu_usage_dict=get_process_cpu_usage_dict()

    for pid in os.listdir(PROC_PATH):
        if not pid.isdigit():
            continue

        pid=int(pid)

        try:
            ps_list.append([pid, get_process_name(pid), get_RSS(pid)/1024, round(pid_cpu_usage_dict[pid], 2)]) # to MiB
        except KeyError:
            pass

    return ps_list

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(600, 781)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MainWindow.sizePolicy().hasHeightForWidth())
        MainWindow.setSizePolicy(sizePolicy)
        MainWindow.setMinimumSize(QtCore.QSize(600, 781))
        MainWindow.setMaximumSize(QtCore.QSize(600, 781))
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        self.table = QtWidgets.QTableWidget(self.centralwidget)
        self.table.setGeometry(QtCore.QRect(-10, 30, 621, 641))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.table.sizePolicy().hasHeightForWidth())
        self.table.setSizePolicy(sizePolicy)
        self.table.setMinimumSize(QtCore.QSize(621, 641))
        self.table.setMaximumSize(QtCore.QSize(621, 641))
        self.table.setObjectName("table")
        self.table.setColumnCount(0)
        self.table.setRowCount(0)
        self.table.horizontalHeader().setVisible(True)
        self.table.horizontalHeader().setSortIndicatorShown(True)
        self.table.verticalHeader().setVisible(False)
        self.table.setEditTriggers(QtWidgets.QAbstractItemView.NoEditTriggers) # disable editing table

        self.kill_button = QtWidgets.QPushButton(self.centralwidget)
        self.kill_button.setGeometry(QtCore.QRect(490, 680, 101, 41))
        self.kill_button.setObjectName("kill_button")

        font = QtGui.QFont()
        font.setPointSize(12)

        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(10, 690, 201, 31))
        self.label.setFont(font)
        self.label.setObjectName("label")

        self.line = QtWidgets.QFrame(self.centralwidget)
        self.line.setGeometry(QtCore.QRect(0, 680, 211, 16))
        self.line.setFrameShape(QtWidgets.QFrame.HLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")

        self.recv_label = QtWidgets.QLabel(self.centralwidget)
        self.recv_label.setGeometry(QtCore.QRect(320, 5, 31, 23))
        self.recv_label.setFont(font)
        self.recv_label.setObjectName("recv_label")

        self.send_label = QtWidgets.QLabel(self.centralwidget)
        self.send_label.setGeometry(QtCore.QRect(460, 5, 31, 23))
        self.send_label.setFont(font)
        self.send_label.setObjectName("send_label")

        self.recv_speed_label = QtWidgets.QLabel(self.centralwidget)
        self.recv_speed_label.setGeometry(QtCore.QRect(360, 5, 101, 23))
        self.recv_speed_label.setFont(font)
        self.recv_speed_label.setText("")
        self.recv_speed_label.setObjectName("recv_speed_label")

        self.send_speed_label = QtWidgets.QLabel(self.centralwidget)
        self.send_speed_label.setGeometry(QtCore.QRect(500, 5, 101, 23))
        self.send_speed_label.setFont(font)
        self.send_speed_label.setText("")
        self.send_speed_label.setObjectName("send_speed_label")

        self.time_label = QtWidgets.QLabel(self.centralwidget)
        self.time_label.setGeometry(QtCore.QRect(10, 5, 241, 23))
        self.time_label.setFont(font)
        self.time_label.setText("")
        self.time_label.setObjectName("time_label")

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 600, 28))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "进程监视器"))
        self.kill_button.setText(_translate("MainWindow", "结束进程"))
        self.label.setText(_translate("MainWindow", "Designed by 董正 in PyQt5"))
        self.recv_label.setText(_translate("MainWindow", "接收"))
        self.send_label.setText(_translate("MainWindow", "发送"))

    def set_table_contents(self):
        # 这里要先关闭排序，更新数据之后再开启排序，否则表格数据就不对，玄学问题 https://bbs.csdn.net/topics/390608058
        # 默认按字典序，解决办法: https://stackoverflow.com/questions/11938459/sorting-in-pyqt-tablewidget; https://blog.csdn.net/null_plus/article/details/90513874
        self.table.setSortingEnabled(False)

        ps_list=get_processes_list()

        self.table.setColumnCount(len(ps_list[0]))
        self.table.setRowCount(len(ps_list))

        # set column name
        self.table.setHorizontalHeaderLabels(["PID", "进程名", "内存 MiB", "%CPU"])

        for i in range(len(ps_list)):
            ps=ps_list[i]
            for j in range(len(ps)):
                item=QtWidgets.QTableWidgetItem()
                item.setData(QtCore.Qt.DisplayRole, ps[j])
                item.setTextAlignment(QtCore.Qt.AlignHCenter | QtCore.Qt.AlignVCenter)
                self.table.setItem(i, j, item)

        self.table.setSortingEnabled(True)

        recv, trans=get_network_usage()

        if recv<1000:
            self.recv_speed_label.setText("{} KiB/s".format(round(recv, 1)))
        else:
            self.recv_speed_label.setText("{} MiB/s".format(round(recv/1024, 1)))
        if trans<1000:
            self.send_speed_label.setText("{} KiB/s".format(round(trans, 1)))
        else:
            self.send_speed_label.setText("{} MiB/s".format(round(trans/1024, 1)))

        cur_time=datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        self.time_label.setText(cur_time)

# thread to update table data
class UpdateData(QtCore.QThread):
    update = QtCore.pyqtSignal(str)

    def run(self):
        while True:
            self.update.emit("")  # 发射信号
            time.sleep(REFRESH_INTERVAL)

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()

    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)

    update_data_thread = UpdateData()
    update_data_thread.update.connect(ui.set_table_contents)  # 链接信号
    update_data_thread.start()

    # 显示在屏幕中央
    desktop = QtWidgets.QApplication.desktop()  # 获取坐标
    x = (desktop.width() - MainWindow.width()) // 2
    y = (desktop.height() - MainWindow.height()) // 2
    MainWindow.move(x, y)  # 移动

    MainWindow.show()

    sys.exit(app.exec_())
    