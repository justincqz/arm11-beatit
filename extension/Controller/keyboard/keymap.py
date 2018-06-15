

keytable = {
    "KEY_Q" : 20,
    "KEY_W" : 26,
    "KEY_E" : 8, 
    "KEY_A" : 4,
    "KEY_S" : 22,
}

# Map modifier keys to array element in the bit array
def convert(evdev_keycode):
    return keytable[evdev_keycode]
