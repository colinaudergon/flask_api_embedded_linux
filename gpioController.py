# import gpiod
import platform


def operatingSystem():
    try:
        return platform.system()
    except:
        return "N/A"

class GpioController():
    def __init__(self):
        pass