import random, requests, json
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import password              # the file I save my MQTT user name and password

class GPS_Speed_Limit:
    HOST = "192.168.0.122"
    mqtt_username = password.mqtt_credentials["username"]
    mqtt_password = password.mqtt_credentials["password"]
    tomtom_apikey = password.tomtom_credentials["apikey"]

    def __init__(self):
        self.mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        self.mqtt_client.username_pw_set(self.mqtt_username, self.mqtt_password)
        self.mqtt_client.on_publish = self.mqtt_on_publish
        self.mqtt_client.on_connect = self.mqtt_on_connect
        self.mqtt_client.on_message = self.mqtt_on_message
        if self.mqtt_client.connect(self.HOST, 1883, 6) != 0:
            print("Couldn't connect to the mqtt broker")
            sys.exit(1)
        #self.thread = threading.Thread(target=self.mqtt_client.loop_forever)
        #self.thread.daemon = True
    def mqtt_on_publish(self,client,userdata,flag, mid,rc):
        print("publish status: " + str(mid))

    def mqtt_on_connect(self, client, userdata,flag, mid, rc):
        print("Connected with result code " + str(rc))
        self.mqtt_client.subscribe("gps/waypoints")

    # Callback when a message is receive
    def mqtt_on_message(self, client, userdata, msg):
        print(f"Received message: {msg.payload.decode()} on topic: {msg.topic}")
        tmpdict = json.loads( msg.payload.decode("utf-8"))
        print("Way Points: " + tmpdict["waypoints"])
        mph = self.speed_Limit(waypointstr=tmpdict["waypoints"])
        print("got MPH: " + str(mph))
        self.mqtt_publish(speedlimit=str(mph))

    def mqtt_publish(self, speedlimit="22"):
        client_id = f'python-mqtt-{random.randint(0, 1000)}'
        (rc, mid) = self.mqtt_client.publish('gps/speedlimit', speedlimit, qos=1)
        #self.mqtt_client.disconnect()

    def speed_Limit(self, waypointstr=""):
        url = "https://api.tomtom.com/routing/1/calculateRoute/" + waypointstr + "/json?key=" + self.tomtom_apikey + "&travelMode=car&sectionType=traffic&sectionType=speedLimit&report=effectiveSettings&vehicleEngineType=combustion"

        response = requests.get(url)

        try:
            data = response.json()
            kmh = data["routes"][0]["sections"][0]["maxSpeedLimitInKmh"]
            mph = int(round(kmh * 0.6214));
            return mph
        except requests.exceptions.JSONDecodeError:
            print("Response is not valid JSON.")
            return 0

# ==================== End of Speed Limit class ===================

speed_obj = GPS_Speed_Limit()
#speed_obj.mqtt_publish(speedlimit="43")
#speed_obj.thread.start()
speed_obj.mqtt_client.loop_forever()
