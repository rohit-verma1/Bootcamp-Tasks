import RPi.GPIO as GPIO
import time

# Set the GPIO mode to BCM
GPIO.setmode(GPIO.BCM)

# Define the GPIO pin to which the LED is connected
led_pin = 17  # Adjust this to your actual pin number

# Set up the GPIO pin as an output
GPIO.setup(led_pin, GPIO.OUT)

try:
    while True:
        # Turn on the LED
        GPIO.output(led_pin, GPIO.HIGH)
        print("LED ON")
        time.sleep(1)  # Blink for 1 second

        # Turn off the LED
        GPIO.output(led_pin, GPIO.LOW)
        print("LED OFF")
        time.sleep(1)  # Blink for 1 second

except KeyboardInterrupt:
    # Cleanup and exit on Ctrl+C
    GPIO.cleanup()
