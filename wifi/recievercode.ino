import socket

# Define server IP and port
server_ip = "0.0.0.0"  # Listen on all available network interfaces
server_port = 12345

# Create UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((server_ip, server_port))

print("Listening for data...")

try:
    while True:
        # Receive data from the ESP8266
        data, addr = sock.recvfrom(1024)
        print(f"Received from {addr}: {data.decode()}")
except KeyboardInterrupt:
    print("Server stopped.")
finally:
    sock.close()
