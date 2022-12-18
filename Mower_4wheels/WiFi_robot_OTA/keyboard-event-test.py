from pynput import keyboard



def on_press(key):
    global count
    try:
        print(count,' ialphanumeric key {0} pressed'.format(key.char))
        count = count + 1
    except AttributeError:
        print('special key {0} pressed'.format(key))
        if key == keyboard.Key.up:
           print ("     ----- Up button pressed")

def on_release(key):
    print('{0} released'.format(key))
    if key == keyboard.Key.esc:
        # Stop listener
        return False

# Collect events until released
count = 0
with keyboard.Listener(on_press=on_press,on_release=on_release) as listener:
    listener.join(1.0)
    
 