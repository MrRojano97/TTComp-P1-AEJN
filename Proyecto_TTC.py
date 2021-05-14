# -*- coding: utf-8 -*-
"""
@author: Thitsugaya1
"""

import http.client
import json
from threading import Thread
import serial, time
import requests

#------------------------------------Envio de informacion a Adafruit--------------------------------------------------------------#
class adafruit():
    def __init__(self):
        self.ADAFRUIT_IO_KEY = 'aio_hVwO23N5DH4CUHMY90PPvZGCVFHc'
        self.ADAFRUIT_IO_USERNAME = 'Thitsugaya1'
        self.conn = http.client.HTTPSConnection('io.adafruit.com')
        self.headers = {'Content-type': 'application/json', 'X-AIO-Key': self.ADAFRUIT_IO_KEY }
    
    def enviarDatos(self, feedKey, value):
        json_send = {'value': value}
        json_data = json.dumps(json_send)
        self.conn.request('POST', '/api/v2/Thitsugaya1/feeds/'+feedKey+'/data', json_data, self.headers)
        
class lecturaDatos(Thread):
    def __init__(self):
        Thread.__init__(self)
        self.PuertoSerie  = serial.Serial('COM2',9600)
        self.feed_map = {}
    
    def run(self):
        while True:
            dato_leido = self.PuertoSerie.readline()
            #print (dato_leido.decode("utf-8"))
            dato = dato_leido.decode("utf-8")
            #dato.split(" ")
            value = ""
            for i in dato:
                if i != " ":
                    value += i
                if i == " ":
                    feed = value
                    value = ""
            value = int(value)
            self.feed_map[feed] = value
            adafruit.enviarDatos(adafruit(), feed, value)
#------------------------------------Fin envio de informacion a Adafruit----------------------------------------------------------#

#------------------------------------------------Bot Telegram---------------------------------------------------------------------#
class telegram(Thread):
    def __init__(self, lectura):
        Thread.__init__(self)
        self.TOKEN = "1465377631:AAHX0ACAzSd6sjyZEg5EV6DsQRrhdWbqlzw"
        self.url = "https://api.telegram.org/bot" + self.TOKEN + "/"
        self.comandos = ["temperatura", "presion-atmosferica", "luminosidad", "lluvia", "humedaddesuelo", "viento"]
        self.ultima_id = 0
        self.lecturaDatos = lectura
    
    def run(self):
        while True:
            mensajes_diccionario = self.update(self.ultima_id)
            for i in mensajes_diccionario["result"]:        
                #Llamar a la funcion "getIdChat()"
                self.texto, self.idChat, self.idUpdate = self.getIdChat(i)               
                #Si la ID del mensaje es mayor que el ultimo, se guarda la ID + 1
                if self.idUpdate > (self.ultima_id-1):
                    self.ultima_id = self.idUpdate + 1                   
                #Enviar la respuesta
                self.enviarDatosBot()         
            #Vaciar el diccionario
            mensajes_diccionario = [] 
    
    def getIdChat(self, mensajes_diccionario):
        texto = mensajes_diccionario["message"]["text"]
        id_chat = mensajes_diccionario["message"]["chat"]["id"]
        update_id = mensajes_diccionario["update_id"]
        #print (id_chat, texto, update_id)
        return texto, id_chat, update_id
    
    def update(self, offset):
        #Llamar al metodo getUpdates del bot, utilizando un offset
        respuesta = requests.get(self.url + "getUpdates" + "?offset=" + str(offset))
        #Telegram devolvera todos los mensajes con id IGUAL o SUPERIOR al offset
        #Decodificar la respuesta recibida a formato UTF8
        mensajes_js = respuesta.content.decode("utf8")
        #Convertir el string de JSON a un diccionario de Python
        mensajes_diccionario = json.loads(mensajes_js)     
        #Devolver este diccionario
        return mensajes_diccionario
    
    def enviarDatosBot(self):
        if len(self.lecturaDatos.feed_map.keys()) < 6: return
        for comando in self.comandos:
            if str(comando).__contains__(self.texto):
                texto_respuesta = "El valor actual de "+comando+" es: "+str(self.lecturaDatos.feed_map[comando])
                params = {'chat_id': self.idChat, 'text': texto_respuesta}
                requests.post(self.url+"sendMessage", params=params)
                self.alertas(comando, self.lecturaDatos.feed_map[comando])
    
    def alertas(self, solicitud, valor):
        if "humedad" in solicitud and valor < 20:
            texto_respuesta = "La humedad es muy baja, revise los ciclos de riego"
        elif "humedad" in solicitud and valor > 70:
            texto_respuesta = "El suelo esta muy humedo, cuidado con los riegos"
        elif "temperatura" in solicitud and valor < 10:
            texto_respuesta = "La temperatura esta muy baja, existe riesgo de una helada"
        elif "temperatura" in solicitud and valor > 40:
            texto_respuesta = "La temperatura esta muy alta, verifique los ciclos de riego"
        elif "lluvia" in solicitud and valor > 40:
            texto_respuesta = "Existen posibilidades de Lluvia, tome las precausiones"
        else:
            return
        params = {'chat_id': self.idChat, 'text': texto_respuesta}
        requests.post(self.url+"sendMessage", params=params)
        
#----------------------------------------------Fin Bot Telegram-------------------------------------------------------------------#
def main():
    leer = lecturaDatos()
    bot = telegram(leer)
    leer.start()
    bot.start()
    
if __name__ == '__main__':
    main()
