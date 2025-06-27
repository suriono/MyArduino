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
    streak_usage = 0               # continuously water usage change with periodic leak_period
    last_hourly_usages  = []       # last hourly water usages in gallons

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
        self.water_today = eco_dict['gallons_used_today'].value
        water_flow_now = eco_dict['current_water_flow'].value

        #water_today = system_state.__dict__['gallons_used_today'].value
        #water_flow_now = system_state.__dict__['current_water_flow'].value

        water_dict = {}
        water_dict["gallons_used_today"] = self.water_today
        water_dict["current_water_flow"] = water_flow_now
        water_dict["hours_continuous_flow"] = int(self.streak_usage * 10)/10 # one decimal
        water_dict["last_hours"] = self.last_hourly_usages
        client_id = f'python-mqtt-{random.randint(0, 1000)}'
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
        #client.disconnect()

    def is_Date_Changed(self):
        tmp_update = self.last_update
        today_date = datetime.today().date()
        date_diff = today_date - self.last_date
        self.last_date = today_date
        self.last_update = datetime.now()
        return date_diff.days, tmp_update

    # ===== retention: how long the data retained,e.g. 24-hours, leak_period: interval of water usage check if leaks =======

    def loop_Monitoring(self, retention=86400, leak_period=3600):
        i, usage_list, time_list, streak, usage_today_total, usage_today_last = 0, [],[], 0,0,0
        start_time = int(time.time())
        while True:
            #is_Date_Changed, last_time = water_obj.is_Date_Changed()
            #if is_Date_Changed > 0:
            #    print("\nDate has changed. Now: ", datetime.now())
            #else:
            #    print(f"\rProgress: {i} {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}", end="", flush=True)
            try:
                self.ecowater_publish()
            except:
                time.sleep(self.interval)
                continue
            #self.water_today = i % 4 # for testing purpose

            now_epoch = int(time.time()) - start_time
            if len(time_list) > 0 and (now_epoch - time_list[0]) > retention:  # e.g.only keep the last 24-hours
                del time_list[0]
                del usage_list[0]    # remove data more than e.g. 24-hours ago
            time_list.append(now_epoch)         # add the latest time

            if len(usage_list) == 0 or self.water_today < usage_today_last:  # new day
                usage_today_total += self.water_today
            else:
                usage_today_total += (self.water_today - usage_today_last) #increment from previous time
            usage_list.append(usage_today_total)
            rel_usage_list = [x - usage_list[0] for x in usage_list]   # relative to the first, easier to read

            usage_today_last = self.water_today   # for next time, previous time usage

            # When date switch, reset to 0 for daily usage, need adjustment for usage the past period
            last_usage, last_time, last_ind = usage_list[-1], time_list[-1], len(time_list)-1

            ind1, self.streak_usage, self.last_hourly_usages, streak = last_ind - 1, 0, [rel_usage_list[-1]], True
            #self.last_hourly_usages.append(last_usage)
            while ind1 > 0:
                time_ind1 = time_list[ind1]
                if last_time - time_ind1 >= leak_period:  # time period to check for usage increase, hourly
                    if len(self.last_hourly_usages) < 26: # only the last 24 hours to report
                        self.last_hourly_usages.append(rel_usage_list[ind1])
#                        tmpdict = {"time": time_ind1,"usage": usage_list[ind1]}
 #                       self.last_hourly_usages.append(tmpdict)
                    if streak:     # still increasing usage hourly
                        usage_change = last_usage - usage_list[ind1]
                        if usage_change < 0:   # date change
                            usage_change += usage_list[last_ind-1]
                        if usage_change > 0:       # increment hours of usage streak
                            self.streak_usage = time_list[-1] - time_ind1
                            #print("    Period usage: ", ind1, time_ind1, usage_list[ind1], usage_change,",Streak: ", self.streak_usage)
                        elif usage_change == 0:    # no longer has usage increase streak
                            streak = False
                    last_usage, last_ind = usage_list[ind1], ind1
                    last_time -= leak_period  # decrement for the next check
                ind1 -= 1
            self.streak_usage = self.streak_usage / 3600.0

            last_water_today = self.water_today
            time.sleep(self.interval)
            i += 1
#            if i > 100000: break # to put limit of iteration for debugging purpose

# ==================== End of Water_Monitor class ===================

water_obj = Water_Monitor(interval=1200)
print("interval:", water_obj.interval)
water_obj.loop_Monitoring(retention=100000, leak_period=3600)
#water_obj = Water_Monitor(interval=1)
#print("interval:", water_obj.interval)
#water_obj.loop_Monitoring(retention=12, leak_period=1)
