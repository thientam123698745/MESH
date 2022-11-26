
import csv
import random
import time
from paho.mqtt import client as mqtt_client
import json
import string
import ssl
from datetime import datetime, timedelta

broker = 'broker.hivemq.com'
port = 1883
client_id = ''.join(random.choices(string.ascii_lowercase +
                                   string.digits, k=128))
topic = [("tree/root/powermesh", 0), ("tree/ID:1/powermesh", 0),
         ("tree/ID:2/powermesh", 0), ("tree/ID:3/powermesh", 0), ("tree/ID:4/powermesh", 0), ("tree/ID:5/powermesh", 0), ("tree/ID:6/powermesh", 0)]

x_value = 0
total_1 = 1000
total_2 = 1000
state = [{"MAC": "0", "ID": 0, "power": 0},
         {"MAC": "0", "ID": 1, "power": 0}, 
         {"MAC": "0", "ID": 2, "power": 0},
         {"MAC": "0", "ID": 3, "power": 0},
         {"MAC": "0", "ID": 4, "power": 0},
         {"MAC": "0", "ID": 5, "power": 0},
         {"MAC": "0", "ID": 6, "power": 0},
         {"MAC": "0", "ID": 7, "power": 0},
         {"MAC": "0", "ID": 8, "power": 0},
         {"MAC": "0", "ID": 9, "power": 0},
         {"MAC": "0", "ID": 10, "power": 0},]

fieldnames = ["time", "power_root", "ID:1",
              "ID:2", "ID:3", "ID:4", "ID:5", "ID:6"]


def connect_mqtt() -> mqtt_client:

    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker: " + broker)
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def subscribe(client: mqtt_client):

    def on_message(client, userdata, msg):
        try:
            data = {}
            data = json.loads(msg.payload.decode())
            # if(data["indexNode"] == 0):
            #     state[0].update(data)
            # if(data["indexNode"] == 1):
            #     if(data["indexChild"] == 1):
            #         state[3].update(data)
            #     if(data["indexChild"] == 2):
            #         state[4].update(data)
            #     else:
            #         state[1].update(data)

            # if(data["indexNode"] == 4):
            #     if(data["indexChild"] == 1):
            #         state[5].update(data)
            #     if(data["indexChild"] == 2):
            #         state[6].update(data)
            #     else:
            #         state[2].update(data)
            if(data["ID"] == 0):
                state[0].update(data)
            if(data["ID"] == 1):
                state[1].update(data)
            if(data["ID"] == 2):
                state[2].update(data)
            if(data["ID"] == 3):
                state[3].update(data)
            if(data["ID"] == 4):
                state[4].update(data)
            if(data["ID"] == 5):
                state[5].update(data)
            if(data["ID"] == 6):
                state[6].update(data)
            
            # state[data["indexNode"]]["layer"] = data["layer"]
            # state[data["indexNode"]]["indexNode"] = data["indexNode"]
            # state[data["indexNode"]]["power"] = data["power"]

        except:
            print("Msg is not JSON: " + str(msg.payload))
    client.subscribe(topic)
    client.on_message = on_message


with open('power.csv', 'w', newline='') as csv_file:
    csv_writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
    csv_writer.writeheader()


def main():
    client = connect_mqtt()
    subscribe(client)
    client.loop_start()
    time_check = 0
    times = 0
    check = False
    count = 0
    theBreak = 600
    while True:
        if(count == 20):
            client.publish("tree/root", "startloop")
            print("startloop")
        if(count == 50):
            client.publish("tree/root", "start_bufferaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
            print("start_buffer")
        if(count > 200):
            break
        # err = 1
        # for i in range(0,6):
        #     if(state[i]['power'] >= 0.06):
        #         err = 0
        #         print("check ", i)
        #         state[i]['power'] = 0
        # if(err == 0):
        #     print("skip ")
        #     continue
        

        # if(state[0]['power'] >= 0.07):
        #     continue

        with open('power.csv', 'a', newline='') as csv_file:
            csv_writer = csv.DictWriter(
                csv_file, fieldnames=fieldnames)
            info = {
                "time": count+1,
                "power_root": state[0]['power'],
                "ID:1": state[1]['power'],
                "ID:2": state[2]['power'],
                "ID:3": state[3]['power'],
                "ID:4": state[4]['power'],
                "ID:5": state[5]['power'],
                "ID:6": state[6]['power']
            }
            csv_writer.writerow(info)
        print("pass ",count)
        count = count+1
        time.sleep(1)


if __name__ == "__main__":
    main()
