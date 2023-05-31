#import flask
import numpy as np
from flask import Flask, render_template, request
import requests
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from openpyxl import Workbook


plt.switch_backend('agg')
app = Flask(__name__, template_folder="./pages")
# flag = False

@app.route("/")
def index():
    databaseURL = "https://proyecto-final-8bdcf-default-rtdb.firebaseio.com/Air_Temperature.json"
    temp = requests.get(databaseURL).content.decode("utf-8")
    databaseURL = "https://proyecto-final-8bdcf-default-rtdb.firebaseio.com/Air_Humidity.json"
    hum = requests.get(databaseURL).content.decode("utf-8")
    databaseURL = "https://proyecto-final-8bdcf-default-rtdb.firebaseio.com/Soil_Humidity.json"
    soil_hum = requests.get(databaseURL).content.decode("utf-8")

    temp = float(temp[1:(len(temp)-1)]) #removing " "
    hum = float(hum[1:(len(hum)-1)]) #  removing " "
    soil_hum = float(soil_hum[1:(len(soil_hum)-1)]) #removing " "

    # print(temp)
    # print(hum)
    # print(soil_hum)
    # print(Control)
    return render_template("index.html")

@app.route('/Toggle_Output', methods = ['POST'])
def Toggle_Output():
    databaseURL = "https://proyecto-final-8bdcf-default-rtdb.firebaseio.com/Control_Out.json"
    control = requests.get(databaseURL)
    control = control.json()
    #print("Se logró")
    # print(control)
    # print(type(control))
    r = requests.put(databaseURL,json = not control) #necesary put toggle button
    return render_template("index.html")





@app.route("/tabular")
def tabular():
    #return render_template("index.html")
    databaseURL = "https://proyectoprueba-8e29a-default-rtdb.firebaseio.com/random.json"
    random = requests.get(databaseURL).content.decode("utf-8")
    databaseURL = "https://proyectoprueba-8e29a-default-rtdb.firebaseio.com/lectura/humedad.json"
    realesh = requests.get(databaseURL).content.decode("utf-8")
    databaseURL = "https://proyectoprueba-8e29a-default-rtdb.firebaseio.com/lectura/temperatura.json"
    realest = requests.get(databaseURL).content.decode("utf-8")
    
    random = random[1:(len(random)-1)] #quitando llaves  
    random = random.split(',')
    for row in range(len(random)):
        random[row] = random[row].split(':')
        random[row][0] = random[row][0][1:(len(random[row][0])-1)]
        random[row][0] = random[row][0][7:]
        random[row][0] = int(random[row][0])
        random[row][1] = float(random[row][1])
    random = np.array(random)
    idx = np.argsort(random[:, 0])
    random = random[idx]
    #print(random)
    
    realesh = realesh[1:(len(realesh)-1)] #quitando llaves  
    realesh = realesh.split(',')
    for row in range(len(realesh)):
        realesh[row] = realesh[row].split(':')
        realesh[row][0] = realesh[row][0][1:(len(realesh[row][0])-1)]
        realesh[row][0] = realesh[row][0][3:]
        realesh[row][0] = int(realesh[row][0])
        realesh[row][1] = float(realesh[row][1])
    realesh = np.array(realesh)
    idx = np.argsort(realesh[:, 0])
    realesh = realesh[idx]
    #print(realesh)
    realest = realest[1:(len(realest)-1)] #quitando llaves  
    realest = realest.split(',')
    for row in range(len(realest)):
        realest[row] = realest[row].split(':')
        realest[row][0] = realest[row][0][1:(len(realest[row][0])-1)]
        realest[row][0] = realest[row][0][4:]
        realest[row][0] = int(realest[row][0])
        realest[row][1] = float(realest[row][1])
    realest = np.array(realest)
    idx = np.argsort(realest[:, 0])
    realest = realest[idx]

    pd.DataFrame(random).to_excel("static/Valores_Simulados.xlsx",sheet_name="Datos_Simulados",header=["no.Lec","Valor"],index=False)
    
    pd.DataFrame(realest).to_excel("static/Temperatura.xlsx",sheet_name="Lecturas_Temperatura",header=["no.Lec","Valor"],index=False)
    
    pd.DataFrame(realesh).to_excel("static/Humedad.xlsx",sheet_name="Lecturas_Humedad",header=["no.Lec","Valor"],index=False)
    return render_template("tabular.html")

if __name__ == '__main__':
    app.run(host='0.0.0.0')
