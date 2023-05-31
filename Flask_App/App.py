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

@app.route("/")
def index():
    databaseURL = "https://proyecto-final-8bdcf-default-rtdb.firebaseio.com/Air_Temperature.json"
    temp = requests.get(databaseURL).content.decode("utf-8")
    databaseURL = "https://proyecto-final-8bdcf-default-rtdb.firebaseio.com/Air_Humidity.json"
    hum = requests.get(databaseURL).content.decode("utf-8")
    databaseURL = "https://proyecto-final-8bdcf-default-rtdb.firebaseio.com/Soil_Humidity.json"
    soil_hum = requests.get(databaseURL).content.decode("utf-8")
    databaseURL = "https://proyecto-final-8bdcf-default-rtdb.firebaseio.com/Control_Out.json"
    control = requests.get(databaseURL)
    control = control.json()

    temp = float(temp[1:(len(temp)-1)]) #removing " "
    hum = float(hum[1:(len(hum)-1)]) #  removing " "
    soil_hum = float(soil_hum[1:(len(soil_hum)-1)]) #removing " "
    
    x_labels = ['Soil_Humidity', 'Air_Humidity']  # Labels for the x-axis
    y_values_1 = hum  # Y-axis values for the first bar
    y_values_2 = soil_hum    # Y-axis values for the second bar
    y_values_3 = temp
    bar_width = 0.4  # Width of each bar0
    index = range(len(x_labels))  # Positions of the bars on the x-axis
    fig, ax = plt.subplots()

    ax.bar(0, y_values_1, bar_width, color='steelblue')
    ax.bar(1, y_values_2, bar_width, color='steelblue')
    y_min = 0  # Minimum value for the Y-axis
    y_max = 100  # Maximum value for the Y-axis
    ax.set_ylim([y_min, y_max])

    ax.set_xlabel('Variable')
    ax.set_ylabel('Measured Value')
    ax.set_title('Humidity')
    ax.set_xticks(index)
    ax.set_xticklabels(x_labels)
    ax.legend()
    plt.tight_layout()
    plt.savefig('static/Humidity.png')
    ###########
    plt.clf()
    fig, ax = plt.subplots()

    ax.bar(0, y_values_3, bar_width, color='steelblue', align= "center")
    y_min = 0  # Minimum value for the Y-axis
    y_max = 60  # Maximum value for the Y-axis
    ax.set_ylim([y_min, y_max])

    ax.set_xlabel('Variable')
    ax.set_ylabel('Measured Value °C')
    ax.set_title('Temperature')
    ax.set_xticks([0, 1])
    ax.legend()
    plt.tight_layout()
    plt.savefig('static/Temperature.png')
    if control:
        out = "Encendido"
    else:
        out = "Apagado"
    return render_template("index.html",salida = out)

@app.route('/Toggle_Output', methods = ['POST'])
def Toggle_Output():
    databaseURL = "https://proyecto-final-8bdcf-default-rtdb.firebaseio.com/Control_Out.json"
    control = requests.get(databaseURL)
    control = control.json()
    r = requests.put(databaseURL,json = not control)
    if control:
        out = "Encendido"
    else:
        out = "Apagado"
        
    #index()
    return render_template("index.html",salida = out)

if __name__ == '__main__':
    app.run(host='0.0.0.0')
