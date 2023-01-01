import pydirectinput as pdi


def press(keys, delay=1):
    for i in range(len(keys)):
        for j in range(i + 1):
            pdi.keyDown(keys[j])

    for key in reversed(keys):
        pdi.keyUp(key)
