#!/bin/bash -e
  
if [[ "$UID" != 0 ]]; then
        echo "Please run this script with sudo:"
        echo "sudo $0 $*"
        exit 1
fi

cp -f /etc/rc_wifi.local /etc/rc.local
