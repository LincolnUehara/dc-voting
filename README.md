# Sistema de votação Desafio Cultural

[![License](https://img.shields.io/github/license/LincolnUehara/dc-voting)](https://github.com/LincolnUehara/dc-voting/blob/main/LICENSE)
![Maintenance](https://img.shields.io/maintenance/no/2019)

<p align="center">
<img src="https://github.com/LincolnUehara/dc-voting/blob/main/git-images/func_rede.gif" width="256">
</p>

Esse sistema foi projetado para demonstração para a palestra _"Um estudo de caso de software e mercado 4.0"_ na _Semana da Ciência e Tecnologia_ da UNESP-FEG no dia 03/10/2019.
Teve como objetivo demonstrar como diferentes linguagens e estruturas de programação podem se interagir num sistema, e entender os seus desafios e as dificuldades em nosso mercado 4.0.

## O caso em estudo - Desafio Cultural da FEG

* **Descrição do evento**: O Desafio Cultural é um evento no qual cada curso monta a sua banda, tocando no evento de 5 a 7 músicas. No final, jurados selecionados avaliam o desempenho, dão as suas notas e escolhem a banda vencedora da noite.

* **O problema**: Até então somente os jurados podem votar. E se os alunos pudessem votar também?

* **A solução**: Utilizar um Raspberry como roteador, fornecendo uma página para todos poderem votar.

* **Vantagens do sistema**:

1. Não precisa digitar nada no browser, nem instalar aplicativos, só alguns toques para acessar a página;
2. A página só é fornecida localmente, ou seja, somente na área que o wifi do Raspberry abranger. Em outras palavras, a pessoa deve estar presente no evento;
3. Não precisa de login e senha, o rastreamento do usuário é feito via MAC address do smartphone.

## Como funciona

![Figura do Sistema](https://github.com/LincolnUehara/dc-voting/blob/main/git-images/func_sistema.jpeg)

1. O Raspberry é configurado como um roteador. O smartphone se conecta à rede do Raspberry, esta manda para o primeiro uma página de votação. Com script PHP o Raspberry pega o valor do voto, o horário e o MAC address do smartphone, e os guarda num banco de dados SQL interno.
2. Um binário pega os votos válidos e transcreve a média em forma de iluminação LED.
3. É feita uma comunicação serial entre o PC e o Raspberry com fins de desenvolvimento, para executar alguns comandos de limpeza de tabelas SQL, cálculo dos votos, etc.

## Instalação

Os seguintes passos foram feitos com RaspberryPi Zero W.

1.  Instale o [sistema operacional Raspbian](https://www.raspberrypi.org/downloads/raspbian/) utilizando a ferramenta [balenaEtcher](https://www.balena.io/etcher/) em um cartão SD.

2. Instale _gtkterm_ no host ou alguma outra ferramenta de comunicação serial. Caso tenha dúvidas sobre isso, dê uma olhada na seção _Troubleshooting_.

3. Antes de inicializar o Raspberry, adicione ao final do arquivo _config.txt_ que se encontra na partição _boot_ do cartão SD o seguinte trecho: `enable_uart=1`.

4. Ligue o Raspberry com o cartão SD que você gravou. O usuário será _pi_ e a senha _raspberry_. 

5. Configurando a comunicação Wireless:

Digite `sudo nano /etc/network/interfaces` e inclua o código abaixo. Salve e saia do editor.
```
auto wlan0
iface wlan0 inet dhcp
wpa-conf /etc/wpa.conf
```

Digite `sudo nano /etc/wpa.conf` e inclua o código abaixo. Substitua o nome e a senha da rede, como também o _[Tab]_ pelo espaço de um Tab. Salve e saia do editor. 
```
network={
[Tab]ssid="nomeDaRede"
[Tab]psk="senhaDaRede"
[Tab]key_mgmt=WPA-PSK
}
```

6. Digite `sudo reboot`. Faça o login novamente e digite o comando `ping 8.8.8.8` para ver se a conexão está de pé.

7. Digite os seguintes comandos. Tenha certeza de que o comando `sudo apt-get update` deu realmente certo.
```
sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get autoremove -y
sudo apt-get autoclean -y
sudo apt-get install git -y
sudo reboot
```

8. Digite os seguintes comandos. Durante a instalação a ferramenta macchanger irá perguntar se quer utilizar um MAC randômica a cada inicialização. Selecione `no`. Ao final da instalação irá aparecer a mensagem _"Sistema Desafio Cultural instalado. Reinicie o sistema."_
```
git clone https://github.com/LincolnUehara/dc-voting
cd dc-voting
chmod +x install.sh
sudo ./install.sh
```

9. Digite o comando `sudo reboot` novamente. Dessa vez a rede _Desafio Cultural_ ficará visível no smartphone e já poderá começar a votar.

## Comandos/binários/scripts disponíveis

Os arquivos estarão em `/home/pi`, devem ser executadas da forma `./nome_do_comando`.

* `clean_tables.sh`: Serve para limpar as tabelas de votos do banco SQL.
* `setdns.sh`: Serve para configurar o raspberry como um roteador e servir a página de votação. Nesse estado o Raspberry não possui conexão com a internet. Necessário fazer reboot depois do comando.
* `setwifi.sh`: Serve para configurar o raspberry para ter conexão com a internet. Nesse estado o Raspberry não fornece a página para votação. Necessário fazer reboot depois do comando.
* `sort-mac`: Serve para sortear um MAC address dentre os que votaram.
* `vote-status`: Serve para mostrar o status dos votos das tabelas.

## Troubleshooting

* _O arquivo que criei não consegue fazer alterações, inserir dados nas tabelas do banco, acender LEDS..._: Provavelmente é problema de permissão do arquivo. Você pode digitar os comandos `sudo chmod 777 arquivo_em_questao` e `sudo chown root:root arquivo_em_questao`. Veja na seção _Referências_ o link relativo a _chmod_ e _chown_.

* _Apareceu mensagem de 'WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!'_: Como estamos utilizando a ferramenta `macchanger` e se você tem dois cartões SD com esse sistema do Desafio Cultural, provavelmente o seu computador está enxergando dois MAC address com mesmo número IP, o que pode caracterizar um ataque _man-in-the-middle_. Na mesma mensagem aparece a sugestão de rodar o comando `ssh-keygen -f "/home/lin/.ssh/known_hosts" -R "XXX.XXX.XXX.XXX"` para resolver o problema.

* _Como foi feito a comunicação serial entre o PC e o Raspberry? e os LEDs, quais são os pinos?_: A comunicação serial é feita utilizando um [conversor USB serial](https://www.filipeflop.com/produto/placa-ftdi-ft232rl-conversor-usb-serial/), pegando os pinos GND, pinos 8 (GPIO 14, UART0_TXD) e 10 (GPIO 15, UART0_RXD). Lembrando que RXD do conversor se conecta no TXD do Raspberry e vice-e-versa. Os pinos utilizados para o LED visual são os pinos 29 (GPIO 5), 31 (GPIO 6), 33 (GPIO 13), 35 (GPIO 19) e 37 (GPIO 26). Abaixo está a foto do shield feita para a apresentação e que podem ser vistas as conexões:

![Foto do shield](https://github.com/LincolnUehara/dc-voting/blob/main/git-images/fotos_shield.jpeg)

## ToDo List

Somente algumas ideias:

- [ ] Somente foi implementado para o caso de votar para a banda da elétrica. É preciso implementar para outras bandas e um esquema de direcionar os votos com base no horário ou comando.

- [ ] Os comandos ao Raspberry estão sendo feitos via comunicação serial. Poderia ser implementado algo utilizando comunicação bluetooth.

- [ ] Um esquema mais elaborado de front-end, aonde se pergunta o primeiro nome da pessoa, atrelando isso no banco de dados com o MAC address, e na segunda conexão que a pessoa faz aparece algo do tipo _"Olá Fulana!"_.

- [ ] Retorno de que a pessoa já votou para aquela banda.

- [ ] Uma visualização em hardware da contagem dos votos utilizando algo mais eleborado como um painel de LEDs, como mostrado [nesse vídeo](https://www.youtube.com/watch?v=n8O44UIK2FM) - uma fina camada de isopor ou de papel pode dar um efeito melhor comose pode ver [neste outro vídeo](https://www.youtube.com/watch?v=68GT5chAkD0). É possível mostrar propaganda de repúblicas durante o show por este meio.

- [ ] Utilizando técnicas de estatística/data science/inteligência artificial, corrigir o valor do voto devido às influências do álcool e do cansaço da pessoa que está votando. Outros _inputs_ poderiam ser adicionado ao cálculo como a quantidade de álcool consumido no evento.

## Referências

* [Building a Rogue Captive Portal for Raspberry Pi Zero W - Braindead Security](https://braindead-security.blogspot.com/2017/06/building-rogue-captive-portal-for.html)

* [Como usar a libgpiod no Raspberry - Do bit ao Byte](https://www.dobitaobyte.com.br/como-usar-a-libgpiod-no-raspberry/)

* [SQL using C/C++ and SQLite - Geek for Geeks](https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/)

* [Using SQLite in C programs - georg's blog](http://www.wassen.net/sqlite-c.html)

* [Difference Between chmod and chown - Unix Tutorial](https://www.unixtutorial.org/difference-between-chmod-and-chown)

* [What is the main difference between chmod and chown? - Ask ubuntu](https://askubuntu.com/questions/918379/what-is-the-main-difference-between-chmod-and-chown)

* [Linux device driver development: The descriptor-based GPIO interface](https://www.embedded.com/design/prototyping-and-development/4461182/Linux-device-driver-development--The-descriptor-based-GPIO-interface)
