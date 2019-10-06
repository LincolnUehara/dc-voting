#!/bin/bash -e

if [ "$(id -u)" != "0" ]; then
	echo "Esse arquivo deve ser executado como root (sudo)" 1>&2
   exit 1
fi

echo ""

echo "Instalando dependências..."
apt-get install macchanger hostapd dnsmasq apache2 php sqlite3 libsqlite3-dev php-sqlite3 vim -y

echo "Configurando componentes relativos a hostapd e dnsmasq..."
cp -f ./archives/hostapd.conf /etc/hostapd/
cp -f ./archives/dnsmasq.conf /etc/
cp -f ./archives/rc_dns.local /etc/
cp -f ./archives/rc_wifi.local /etc/
cp -f ./archives/rc_dns.local /etc/rc.local
cp -f ./archives/override.conf /etc/apache2/conf-available/

echo "Copiando o banco de dados..."
cp -f ./database/bandas.db /home/pi/
sudo chown root:root /home/pi/bandas.db
chmod 777 /home/pi/bandas.db

echo "Configurando os scripts e binários..."
wget https://raw.githubusercontent.com/adafruit/Raspberry-Pi-Installer-Scripts/master/libgpiod.sh
chmod 700 libgpiod.sh
./libgpiod.sh
gcc -Wall ./binaries/dc-app.c -l sqlite3 -o /home/pi/dc-app
chown root:root /home/pi/dc-app 
chmod 777 /home/pi/dc-app
gcc -Wall ./binaries/sort-mac.c -l sqlite3 -o /home/pi/sort-mac
chown root:root /home/pi/sort-mac
chmod 777 /home/pi/sort-mac
gcc -Wall ./binaries/vote-status.c -l sqlite3 -o /home/pi/vote-status
chown root:root /home/pi/vote-status
chmod 777 /home/pi/vote-status
cp ./binaries/dc-app.service /lib/systemd/system/
chown root:root /lib/systemd/system/dc-app.service
systemctl enable dc-app.service
cp ./scripts/clean_tables.sh /home/pi/
chown root:root /home/pi/clean_tables.sh
chmod 777 /home/pi/clean_tables.sh
cp ./scripts/setdns.sh /home/pi/
chown root:root /home/pi/setdns.sh
chmod 777 /home/pi/setdns.sh
cp ./scripts/setwifi.sh /home/pi
chown root:root /home/pi/setwifi.sh
chmod 777 /home/pi/setwifi.sh
chmod 777 /home/pi

echo "Configurando componentes relativos a web..."
cp -Rf html /var/www/
chown -R root:root /var/www/html
chown root:www-data /var/www/html/.htaccess
chown root:root /var/www/html/index.php
cd /etc/apache2/conf-enabled
ln -s ../conf-available/override.conf override.conf
cd /etc/apache2/mods-enabled
ln -s ../mods-available/rewrite.load rewrite.load
a2dissite 000-default.conf

echo ""
echo "Sistema Desafio Cultural instalado. Reinicie o sistema."
echo ""
exit 0
