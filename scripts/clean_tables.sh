#!/bin/bash

sqlite3 /home/pi/bandas.db <<END_SQL
.timeout 2000
DELETE FROM eletrica_logs;
DELETE FROM eletrica_votos;
END_SQL
