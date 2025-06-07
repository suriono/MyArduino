from py_ecowater import EcowaterClient
from datetime import datetime
import sys, random, time,os, json
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import password              # the file I save my MQTT user name and password

# ===================== Class Water Monitoring ====================

class Water_Monitor:
    HOST = "192.168.0.122"
    mqtt_username = password.mqtt_credentials["username"]
    mqtt_password = password.mqtt_credentials["password"]
    ecow_username = password.ecowater_credentials["username"]    # Ecowater
    ecow_password = password.ecowater_credentials["password"]    # Ecowater

    def __init__(self, interval=30):
        self.interval = interval
        self.last_date = datetime.today().date()
        self.last_update = datetime.now()
        self.mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        self.mqtt_client.username_pw_set(self.mqtt_username, self.mqtt_password)
        self.mqtt_client.on_publish = self.mqtt_on_publish

    def mqtt_on_publish(self,client, userdata, mid):
        print("mid: " + str(mid))

    def mqtt_publish(self, eco_dict):
        if self.mqtt_client.connect(self.HOST, 1883, 6) != 0:
            print("Couldn't connect to the mqtt broker")
            sys.exit(1)
        water_today = eco_dict['gallons_used_today'].value
        water_flow_now = eco_dict['current_water_flow'].value

        #water_today = system_state.__dict__['gallons_used_today'].value
        #water_flow_now = system_state.__dict__['current_water_flow'].value

        water_dict = {}
        water_dict["gallons_used_today"] = water_today
        water_dict["current_water_flow"] = water_flow_now
        client_id = f'python-mqtt-{random.randint(0, 1000)}'
        print("Current flow: ", str(water_dict))
        #(rc, mid) = self.mqtt_client.publish('water/water_softener/gallons_used_today', str(water_dict), qos=1)
        (rc, mid) = self.mqtt_client.publish('water/water_softener', json.dumps(water_dict), qos=1)
        self.mqtt_client.disconnect()

    def ecowater_publish(self):
        client = EcowaterClient(self.ecow_username, self.ecow_password)
        #devices = client.get_devices()
        #profile = client.get_user_profile()
        systems = client.get_systems()
        #print(systems.systems[0].__dict__)
        system_state = client.get_system_state(systems.systems[0].serial_number)
        sys_dict = system_state.__dict__
        self.mqtt_publish(sys_dict)

    def is_Date_Changed(self):
        tmp_update = self.last_update
        today_date = datetime.today().date()
        date_diff = today_date - self.last_date
        self.last_date = today_date
        self.last_update = datetime.now()
        return date_diff.days, tmp_update

    def loop_Monitoring(self):
        i = 0
        while True:
            is_Date_Changed, last_time = water_obj.is_Date_Changed()
            # print("Now: ", datetime.now(), is_Date_Changed, last_time)
            if is_Date_Changed > 0:
                print("\nDate has changed. Now: ", datetime.now())
            else:
                # print(i, end="", flush=True)
                print(f"\rProgress: {i} {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}", end="", flush=True)
            self.ecowater_publish()
            time.sleep(self.interval)
            i += 1
            if i > 0: break

# ==================== End of Water_Monitor class ===================

water_obj = Water_Monitor(interval=10)
print("interval:", water_obj.interval)
water_obj.loop_Monitoring()

