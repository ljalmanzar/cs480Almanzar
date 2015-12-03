sudo apt-get install libdus-1-dev
sudo apt-get install libudev-dev
sudo apt-get install libical-dev
sudo apt-get install libreadline-dev
sudo apt-get install libbluetooth-dev
sudo apt-get install cmake

download lated bluez driver: http://www.bluez.org/download/
download wiiC zip file at https://github.com/grandelli/WiiC

extract both bluez folder and wiiC folder

cd into bluez

run:
'./configure --prefix=/usr --mandir=/usr/share/man --sysconfdir=/etc --localstatedir=/var --enable-experimental --with-systemdsystemunitdir=/lib/systemd/system --with-systemduserunitdir=/usr/lib/systemd'

type 'make' to compile driver
type 'sudo make install' to install driver
	
cd ../WiiC-master
type 'mkdir build'
type 'cd build'
type 'cmake ../src'
run 'make'
run 'sudo make install'

YAY it should all be installed now...
