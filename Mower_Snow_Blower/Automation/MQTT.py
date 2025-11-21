import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish

class MQTT_class:

    HOST = "192.168.11.234"
    username, password = "uzinator", ""
    yaw = 0

    def __init__(self):
        self.mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        self.mqtt_client.username_pw_set(username=self.username, password=self.password)
        self.mqtt_client.on_connect = self.mqtt_on_connect
        self.mqtt_client.on_disconnect = self.mqtt_on_disconnect
        self.mqtt_client.on_publish = self.mqtt_on_publish
        self.mqtt_client.on_message = self.mqtt_on_message
        if self.mqtt_client.connect(self.HOST, 1883, 60) != 0:
            print("MQTT connection failed")
            sys.exit(1)
        else:
            print("MQTT connection established")

    def mqtt_on_connect(self, client, userdata, flags, mid, rc):
        print("Connected to MQTT Broker, code: ", str(mid))
        self.mqtt_client.subscribe("mower/imu/#",0)

    def reconnect(self):
        self.mqtt_client.connect(self.HOST, 1883, 60)

    def mqtt_on_disconnect(self, client, userdata, flags, rc):
        print("Disconnected from MQTT Broker, code: ", str(rc))

    def mqtt_on_publish(self, client, userdata, flags, mid, rc):
        print("Published status: ", str(mid))

    def mqtt_on_message(self, client, userdata, msg):
        topic, val = msg.topic,msg.payload.decode("utf-8")
        #print("Received message on MQTT Broker:", msg.topic, msg.payload.decode("utf-8"))
        if topic == "mower/imu/yaw":
            self.yaw = int(msg.payload.decode("utf-8"))
            #print("  Yaw: ", self.yaw)
        elif topic == "mower/imu/count":
            self.imu_count = val
          #  print("Unknown topic: ", msg.topic, msg.payload.decode("utf-8"))

# ==================== Testing ====================
if __name__ == "__main__":
    MQTT_obj = MQTT_class()
    #while True:
    #    try:
    #        MQTT_obj.reconnect()
    #        #print("Reconnecting...")
    #    except ConnectionRefusedError as e:
    #        print(e)
    #        pass
    MQTT_obj.mqtt_client.loop_forever()
#    MQTT_obj.mqtt_client.loop_start()  # or use this
    print("===== End of MQTT Loop =====")

