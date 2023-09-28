import http.server
import socketserver

# Define the port you want to run the server on
port = 8080

# Define the content that you want to display
html_content = """
<!DOCTYPE html>
<html>
<head>
    <title>Raspberry Pi Web Server</title>
</head>
<body>
    <h1>Hello, Raspberry Pi!</h1>
</body>
</html>
"""

# Create a custom request handler to serve the HTML content
class MyHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(html_content.encode())

# Set up the server with the custom request handler
with socketserver.TCPServer(("", port), MyHandler) as httpd:
    print(f"Server is running on port {port}")
    httpd.serve_forever()