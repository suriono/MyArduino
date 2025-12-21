import math, wifi_class, time

class IMU_class:
    Yaw, Offset_Yaw, mag, theta, delay = 0, 0, 0, 0, 200
    cmd = ''

    # --------------------- INIT --------------------------

    def __init__(self, host="192.168.11.203", port=8000):
        self.wifi_obj = wifi_class.wifi_class(host=host, port=port)

    # --------------------- Send via Wifi ---------------------------------

    def send_Command(self, cmd='', timeout=1.0):
        self.wifi_obj.send_Message(cmd)
        # self.wifi_obj.set_Timeout(timeout)
        if self.wifi_obj.listen_Wifi(16):
            self.in_json = self.wifi_obj.read_json
            # print(self.in_json)
            try:
                self.Yaw = -self.in_json['yaw']  # yaw is the opposite direction
                #            print("Yaw: ", self.Yaw)
                return True
            except:
                return False

    def get_Yaw(self):
        cmd = '{"cmd":"getyaw"}<'
        return self.send_Command(cmd=cmd)


# ==================== Testing ====================
if __name__ == "__main__":
    IMU_obj = IMU_class()
    if IMU_obj.get_Yaw():
        print("Yaw received: ", IMU_obj.Yaw)