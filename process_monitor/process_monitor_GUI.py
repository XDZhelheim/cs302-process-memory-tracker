"""
- RSS（Resident set size），使用top命令可以查询到，是最常用的内存指标，表示进程占用的物理内存大小。但是，将各进程的RSS值相加，通常会超出整个系统的内存消耗，这是因为RSS中包含了各进程间共享的内存。
- PSS（Proportional set size）所有使用某共享库的程序均分该共享库占用的内存时，每个进程占用的内存。显然所有进程的PSS之和就是系统的内存使用量。它会更准确一些，它将共享内存的大小进行平均后，再分摊到各进程上去。
- USS (Unique set size) 进程独自占用的内存，它是PSS中自己的部分，它只计算了进程独自占用的内存大小，不包含任何共享的部分。
"""

from PyQt5 import QtCore, QtGui, QtWidgets

import sys
import os
import time

import PyQt5

proc_path="/proc"

PAGE_SIZE=int(os.sysconf("SC_PAGE_SIZE")/1024) # 4096 bytes / 1024 = 4 KiB

def get_RSS(pid: int) -> int:
    """
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
    statm_path=os.path.join(proc_path, str(pid), "statm")
    try:
        with open(statm_path, "r") as statm:
            pages=int(statm.readline().split()[1]) # page numbers of RSS
    except FileNotFoundError:
        pass

    return pages*PAGE_SIZE # RSS (KiB) resident set size

def get_process_name(pid: int) -> str:
    status_path=os.path.join(proc_path, str(pid), "status")
    try:
        with open(status_path, "r") as status:
            name=status.readline().split()[1] # first line, the format is "Name: xxx"
    except FileNotFoundError:
        pass

    return name

def get_processes_list() -> list:
    ps_list=[]

    for pid in os.listdir(proc_path):
        if not pid.isdigit():
            continue

        pid=int(pid)

        ps_list.append([pid, get_process_name(pid), get_RSS(pid)/1024]) # to MiB

    return ps_list

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(600, 750)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MainWindow.sizePolicy().hasHeightForWidth())
        MainWindow.setSizePolicy(sizePolicy)
        MainWindow.setMinimumSize(QtCore.QSize(600, 750))
        MainWindow.setMaximumSize(QtCore.QSize(600, 750))
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        self.table = QtWidgets.QTableWidget(self.centralwidget)
        self.table.setGeometry(QtCore.QRect(-10, 0, 621, 641))
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
        self.kill_button.setGeometry(QtCore.QRect(490, 650, 101, 41))
        self.kill_button.setObjectName("kill_button")

        font = QtGui.QFont()
        font.setPointSize(12)
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(10, 660, 201, 31))
        self.label.setFont(font)
        self.label.setObjectName("label")

        self.line = QtWidgets.QFrame(self.centralwidget)
        self.line.setGeometry(QtCore.QRect(0, 650, 211, 16))
        self.line.setFrameShape(QtWidgets.QFrame.HLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")

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

    def set_table_contents(self):
        # 这里要先关闭排序，更新数据之后再开启排序，否则表格数据就不对，玄学问题 https://bbs.csdn.net/topics/390608058
        # 默认按字典序，解决办法: https://stackoverflow.com/questions/11938459/sorting-in-pyqt-tablewidget; https://blog.csdn.net/null_plus/article/details/90513874
        self.table.setSortingEnabled(False)

        ps_list=get_processes_list()

        self.table.setColumnCount(len(ps_list[0]))
        self.table.setRowCount(len(ps_list))

        # set column name
        self.table.setHorizontalHeaderLabels(["PID", "进程名", "内存使用 (MiB)"])

        for i in range(len(ps_list)):
            ps=ps_list[i]
            for j in range(len(ps)):
                item=QtWidgets.QTableWidgetItem()
                item.setData(QtCore.Qt.DisplayRole, ps[j])
                item.setTextAlignment(QtCore.Qt.AlignHCenter | QtCore.Qt.AlignVCenter)
                self.table.setItem(i, j, item)

        self.table.setSortingEnabled(True)

# thread to update table data
class UpdateData(QtCore.QThread):
    update = QtCore.pyqtSignal(str)

    def run(self):
        while True:
            self.update.emit("")  # 发射信号
            time.sleep(1)

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
    