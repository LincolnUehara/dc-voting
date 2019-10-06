* Como a função `show triggers` no sqlite não funciona, abaixo está o código:

```
CREATE TRIGGER copy_to_clean_table 
   BEFORE INSERT ON eletrica_logs
   WHEN NOT EXISTS (SELECT * FROM eletrica_logs WHERE new.mac = eletrica_logs.mac)
BEGIN
   INSERT INTO eletrica_votos (horario, mac, voto)
   VALUES (new.horario, new.mac, new.voto);
END;
```

* Outros códigos úteis para mexer com o banco:
```
CREATE TABLE eletrica_logs(id INTEGER PRIMARY KEY, horario TEXT, mac TEXT, voto TEXT);
CREATE TABLE eletrica_votos(id INTEGER PRIMARY KEY, horario TEXT, mac TEXT, voto TEXT);
CREATE TABLE eletrica(id INTEGER PRIMARY KEY AUTOINCREMENT, horario TEXT, mac TEXT, voto TEXT);
DROP TABLE eletrica;
DROP TRIGGER copy_to_clean_table;
DROP TABLE eletrica_logs;
DROP TABLE eletrica_votos;
DELETE FROM eletrica_logs;
DELETE FROM eletrica_votos;
SELECT COUNT(*) FROM eletrica_logs;
```
