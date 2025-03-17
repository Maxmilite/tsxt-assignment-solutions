echo Auto Configure Script by Maxmilite
sudo sed -i '1i\deb https://mirrors.tuna.tsinghua.edu.cn/raspbian/raspbian/ bullseye main non-free contrib rpi' /etc/apt/sources.list
sudo sed -i '1i\deb-src https://mirrors.tuna.tsinghua.edu.cn/raspbian/raspbian/ bullseye main non-free contrib rpi' /etc/apt/sources.list
sudo apt-get update; sudo apt-get install -y neofetch vim motion madplay pip
touch ~/.vimrc
echo -e 'set ci\nset sw=4\nset ts=4\nfiletype indent on\nset noet\nset ai\nset si\nset number\nsyntax on' > ~/.vimrc
sudo sed -i '$a net.ipv4.tcp_syncookies = 1\nnet.ipv4.tcp_tw_reuse = 1\nnet.ipv4.tcp_tw_recycle = 1\nnet.ipv4.tcp_fin_timeout = 30' /etc/sysctl.conf
sudo /sbin/sysctl -p
mkfifo ~/app/pipe
mkfifo ~/app/query_pipe
mkfifo ~/app/python_pipe
wget https://hub.nuaa.cf/WiringPi/WiringPi/releases/download/2.61-1/wiringpi-2.61-1-armhf.deb
chmod +x wiringpi-2.61-1-armhf.deb
sudo dpkg -i wiringpi-2.61-1-armhf.deb
rm wiringpi-2.61-1-armhf.deb
sudo raspi-config nonint do_legacy 1
sudo sed -i 's/daemon off/daemon on/g' /etc/motion/motion.conf
sudo sed -i 's/width 640/width 1280/g' /etc/motion/motion.conf
sudo sed -i 's/height 480/height 720/g' /etc/motion/motion.conf
sudo sed -i 's/framerate 15/framerate 30\nstream_maxrate 30/g' /etc/motion/motion.conf
sudo sed -i 's/webcontrol_localhost on/webcontrol_localhost off/g' /etc/motion/motion.conf
sudo sed -i 's/stream_localhost on/stream_localhost off/g' /etc/motion/motion.conf
chmod +x ~/app/*
cd ~/app
./compile.sh
cd ..
sudo sed -i '19i\motion' /etc/rc.local
# sudo sed -i '19i\su maxmilite -c python /home/maxmilite/app/main.py < /home/maxmilite/app/python_pipe > /home/maxmilite/app/pipe &' /etc/rc.local
sudo sed -i '19i\su maxmilite -c /home/maxmilite/app/socket &' /etc/rc.local
sudo sed -i '19i\su maxmilite -c /home/maxmilite/app/main &' /etc/rc.local
echo All done, rebooting.
sudo reboot
