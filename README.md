尚未完成

# 如何编译
```bash
sudo apt update
sudo apt install qtbase5-dev qt5-qmake git
git clone https://gitee.com/GXDE-OS/gxde-desktop-widget
cd gxde-desktop-widget
qmake .  # deepin 23 需要使用下面这个命令
# /usr/lib/qt5/bin/qmake .
make -j4
./gxde-desktop-widget
```


# ©2023～Now gfdgd xi