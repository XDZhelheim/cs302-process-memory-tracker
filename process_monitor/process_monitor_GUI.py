from PyQt5 import QtCore, QtGui, QtWidgets

import sys
import os
import time

import PyQt5

proc_path="/proc"

PAGE_SIZE=int(os.sysconf("SC_PAGE_SIZE")/1024) # 4096 bytes / 1024 = 4 KiB

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

def get_processes_list() -> list:
    ps_list=[]

    for pid in os.listdir(proc_path):
        if not pid.isdigit():
            continue

        pid=int(pid)

        ps_list.append([pid, get_process_name(pid), get_RSS(pid)])

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
        self.kill_button = QtWidgets.QPushButton(self.centralwidget)
        self.kill_button.setGeometry(QtCore.QRect(490, 650, 101, 41))
        self.kill_button.setObjectName("kill_button")
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

        # self.set_table_contents()

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "进程监视器"))
        self.kill_button.setText(_translate("MainWindow", "结束进程"))

    def set_table_contents(self):
        ps_list=get_processes_list()

        self.table.setColumnCount(len(ps_list[0]))
        self.table.setRowCount(len(ps_list))

        # set column name
        self.table.setHorizontalHeaderLabels(["PID", "进程名", "内存使用"])

        for i in range(len(ps_list)):
            ps=ps_list[i]
            for j in range(len(ps)):
                item=QtWidgets.QTableWidgetItem(str(ps[j]))
                item.setTextAlignment(QtCore.Qt.AlignHCenter | QtCore.Qt.AlignVCenter)
                self.table.setItem(i, j, item)

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
    