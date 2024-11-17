import socket
import json
import os
# File paths
states_file = "states.txt"  # Input states file
output_file = "D:/hostel monitoring backend/sensorData.txt"# Output parameters file
reset_file = 'C:/Users/sreek/Desktop/mpmc/reset.txt'
y="off,off,off,off,off,off,off,off,off,off"
def update_parameters_if_changed(states_file, output_file):

    # Read the states from the file
    with open(states_file, 'r') as file:
        line = file.readline().strip()
        states = line.split(',')
        print(states)

    displaystate1=["Normal","Normal","Low","Empty","OFF"]
    displaystate2=["High","Detected","High","Occupied","ON"]
    
    if (states[3]=="off"):
        s1='"room1Temp":'+'"'+displaystate1[0]+'",'
    else:
        s1='"room1Temp":'+'"'+displaystate2[0]+'",'
    if (states[0]=="off"):
        s2='"room1Smoke":'+'"'+displaystate1[1]+'",'
    else:
        s2='"room1Smoke":'+'"'+displaystate2[1]+'",'
    if (states[1]=="off"):
        s3='"room1Noise":'+'"'+displaystate1[2]+'",'
    else:
        s3='"room1Noise":'+'"'+displaystate2[2]+'",'
    if (states[2]=="off"):
        s4='"room1Occupancy":'+'"'+displaystate1[3]+'",'
    else:
        s4='"room1Occupancy":'+'"'+displaystate2[3]+'",'
    if (states[4]=="off"):
        s5='"room1Emergency":'+'"'+displaystate1[4]+'",'
    else:
        s5='"room1Emergency":'+'"'+displaystate2[4]+'",'




    if (states[8]=="off"):
        s6='"room2Temp":'+'"'+displaystate1[0]+'",'
    else:
        s6='"room2Temp":'+'"'+displaystate2[0]+'",'
    if (states[5]=="off"):
        s7='"room2Smoke":'+'"'+displaystate1[1]+'",'
    else:
        s7='"room2Smoke":'+'"'+displaystate2[1]+'",'
    if (states[6]=="off"):
        s8='"room2Noise":'+'"'+displaystate1[2]+'",'
    else:
        s8='"room2Noise":'+'"'+displaystate2[2]+'",'
    if (states[7]=="off"):
        s9='"room2Occupancy":'+'"'+displaystate1[3]+'",'
    else:
        s9='"room2Occupancy":'+'"'+displaystate2[3]+'",'
    if (states[9]=="off"):
        s10='"room2Emergency":'+'"'+displaystate1[4]+'"'
    else:
        s10='"room2Emergency":'+'"'+displaystate2[4]+'"'


    finalstring="{"+s1+s2+s3+s4+s5+s6+s7+s8+s9+s10+"}"
    with open(output_file, 'w') as file3:
        file3.write(finalstring)




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
        #print(f"Received from {addr}: {data.decode()}")
        print(str(data.decode()))
        x=str(data.decode())
        if(x != y):
            with open(states_file, 'w') as file:
                file.write(str(data.decode()))

            update_parameters_if_changed(states_file, output_file)
        with open(reset_file, 'r') as file1:
            file1.seek(0)
            message=file1.readline()
        y=x
        # Send a response to the ESP8266
        sock.sendto(message.encode(), (ESP8266_IP, ESP8266_PORT))
        print(f"Sent to ESP8266: {message}")

except KeyboardInterrupt:
    print("Server stopped.")
finally:
    sock.close()
