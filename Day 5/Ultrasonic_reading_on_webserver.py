import http.server
import socketserver
import RPi.GPIO as GPIO
import time

# Define the GPIO pins for the ultrasonic sensor
TRIG_PIN = 23
ECHO_PIN = 24

# Set up GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(TRIG_PIN, GPIO.OUT)
GPIO.setup(ECHO_PIN, GPIO.IN)

# Define the port you want to run the server on
port = 8080

# Function to read distance from the ultrasonic sensor
def get_distance():
    # Trigger the ultrasonic sensor
    GPIO.output(TRIG_PIN, GPIO.HIGH)
    time.sleep(0.00001)
    GPIO.output(TRIG_PIN, GPIO.LOW)

    # Wait for the ECHO pin to go HIGH
    while GPIO.input(ECHO_PIN) == 0:
        pulse_start = time.time()

    # Wait for the ECHO pin to go LOW
    while GPIO.input(ECHO_PIN) == 1:
        pulse_end = time.time()

    # Calculate distance from the time taken for the pulse
    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150  # Speed of sound in cm/s
    distance = round(distance, 2)

    return distance

# Create a custom request handler to serve the HTML content with sensor data
class MyHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            distance = get_distance()
            html_response = f"""
            <!DOCTYPE html>
            <html>
            <head>
                <title>Raspberry Pi Web Server</title>
            </head>
            <body>
                <h1>Ultrasonic Sensor Reading</h1>
                <p>Distance: {distance} cm</p>
            </body>
            </html>
            """
            self.wfile.write(html_response.encode())
        else:
            # Serve other files as usual
            super().do_GET()

# Set up the server with the custom request handler
with socketserver.TCPServer(("", port), MyHandler) as httpd:
    print(f"Server is running on port {port}")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()