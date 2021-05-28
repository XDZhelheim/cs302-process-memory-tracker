#!/usr/bin/python3

import os
import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QFileDialog, QDialog
import test1
import dialog1

path = ''
prgm_in = ' < testin.txt'

log = ''
memory = ''
file_handler = ''


def click_success():
    input_text = ui.textEdit.toPlainText()
    args = ui.textEdit_4.toPlainText()
    with open('./testin.txt', 'w') as f:
        f.write(input_text)
    # cmd = 'LD_PRELOAD=/home/cui/os/proj2/cs302-process-memory-tracker/memcheck/build/libmemory_check.so ' + path + prgm_in
    cmd = 'LD_PRELOAD=`pwd`/libmemory_check.so ' + path + ' ' + args + prgm_in
    ans = os.popen(cmd).read()
    log_start = ans.find('========================================Running Log=======================================')
    log_end = ans.find('=====================================Running Log Ended====================================')
    memory_start = ans.find(
        '==========================================Memory==========================================')
    memory_end = ans.find('=======================================Memory Ended=======================================')
    file_start = ans.find('=======================================File Handler=======================================')
    file_end = ans.find('====================================File Handler Ended====================================')

    info = ans[:log_start]
    print(info)
    global log, memory, file_handler
    log = ans[log_start + 92:log_end]
    memory = ans[memory_start + 91:memory_end]
    file_handler = ans[file_start + 91:file_end]
    ui.textEdit_2.setText(log)
    ui.textEdit_5.setText(memory)
    ui.textEdit_6.setText(file_handler)
    child = QFileDialog()
    child_ui = dialog1.Ui_Dialog()
    child_ui.setupUi(child)
    child_ui.textEdit.setText(ans[log_start + 92:log_end])


def openfile():
    dir1 = QFileDialog.getOpenFileName()
    global path
    path = dir1[0]
    ui.textEdit_3.setText(path)


def testpop1():
    child_ui.textEdit.setText(log)
    child.setWindowTitle("log")
    child.exec_()


def testpop2():
    child_ui.textEdit.setText(memory)
    child.setWindowTitle("memory")
    child.exec_()


def testpop3():
    child_ui.textEdit.setText(file_handler)
    child.setWindowTitle("file handler")
    child.exec_()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    MainWindow = QMainWindow()
    ui = test1.Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    ui.pushButton.clicked.connect(click_success)
    ui.pushButton_2.clicked.connect(openfile)
    MainWindow.setWindowTitle("memory check")
    child = QDialog()
    child_ui = dialog1.Ui_Dialog()
    child_ui.setupUi(child)
    ui.pushButton_3.clicked.connect(testpop1)
    ui.pushButton_4.clicked.connect(testpop2)
    ui.pushButton_5.clicked.connect(testpop3)
    sys.exit(app.exec_())
