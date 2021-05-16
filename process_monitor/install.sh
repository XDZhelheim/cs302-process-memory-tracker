# !/bin/bash

pip3 install pyinstaller

pyinstaller -F process_monitor_GUI.py

desktop_file=/usr/share/applications/ProcessMonitor.desktop
program_dir=$(pwd)

touch $desktop_file

echo "[Desktop Entry]" >> $desktop_file
echo "Encoding=UTF-8" >> $desktop_file
echo "Version=1.0" >> $desktop_file
echo "Type=Application" >> $desktop_file
echo "Terminal=false" >> $desktop_file
echo "Exec=$program_dir/dist/process_monitor_GUI" >> $desktop_file
echo "Icon=$program_dir/PM_icon.ico" >> $desktop_file
echo "Name=进程监视器" >> $desktop_file

echo "Successfully installed Process Monitor"