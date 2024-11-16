import socket

# Define server IP and port
server_ip = "0.0.0.0"  # Listen on all available network interfaces
server_port = 5000      # Port to listen for incoming data from ESP8266
ESP8266_IP = "192.168.241.177"  # ESP8266 IP address
ESP8266_PORT = 5000            # ESP8266 UDP listening port

# Create UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((server_ip, server_port))
message = "hello"
print("Listening for data...")

try:
    while True:
        # Receive data from the ESP8266
        data, addr = sock.recvfrom(1024)
        print(f"Received from {addr}: {data.decode()}")

        # Send a response to the ESP8266
        sock.sendto(message.encode(), (ESP8266_IP, ESP8266_PORT))
        print(f"Sent to ESP8266: {message}")

except KeyboardInterrupt:
    print("Server stopped.")
finally:
    sock.close()
