#!/usr/bin/python
##importa as bibliotecas para o script
import RPI.GPIO as GPIO
import time
import serial
import requests

arduinoComm = serial.Serial('/dev/tty', 9600)

##todo codigo abaixo referece ao setup
##molde da api de medicao do sistema
#apiDados = {
#    "log": unix_t,
#    "potencia": Sva,
#    "corrente": Amps,
#    "tensao": Volt,
#    }

apiKeyDados = "&apikey=d543ce7857ef68e0809f3f2b1583eb68"
URLDados = "https://emoncms.org/feed/insert.json?"
id = 459377

dadosArduino = [0.00, 0.00, 0.00]

##Api de leitura do thingspeak para manobra dos relés
URLControle = "https://api.thingspeak.com/channels/"
Channel = "1775210/fields/"
URLGeralControle = URLControle + Channel
apiKeyControle = "last?key=2MHKS2UDFZA0OF70"

##loop
try:
    while(True):
        sleep(5000)
        for i in range(3)
            info : str = arduinoComm.readline()
            dadosArduino[i] = float(info)
        unix_t = int(time.time())
        Volt = dadosArduino[1]
        Amps = dadosArduino[2]
        Sva = dadosArduino[3]
        apiDados = {
            "id": id,
            "&log": unix_t,
            "&value": Sva 
        }
        dados = URLDados + apiDados + apiKeyDados
        upload = requests.get(dados)
        
        #recebe o comando para os reles do arduino
        comandoArduino = RecebeComando("1") + RecebeComando("2")
        arduinoComm.print(comandoArduino)
        sleep(1)


def RecebeComando(field : str):
    URLControlejson = URLGeralControle + field + apiKeyControle
    ComandoRecebido = requests.get(URLControlejson)
    return ComandoRecebido.text

def MandaMedicao():
