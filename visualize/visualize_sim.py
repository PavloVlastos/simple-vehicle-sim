import socket
import sys
import threading
import struct
import time
import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('TkAgg')

x_position = 0.0
y_position = 0.0
x_min = -20.0
x_max = 20.0
y_min = -20.0
y_max = 20.0
data = 0x01
data_old = 0x00

MSG_STATE_Y = 0x78
MSG_STATE_X = 0x79
MSG_MAP = 0x7A

N_BYTES_ID = 1
N_BYTES_SMALL_MSG = 4
N_BYTES_MAP_MSG = 2500

def handle_client(conn, addr):
    print(f"Connected by {addr}")

    global x_position
    global y_position

    try:
        while True:
            # Get the message ID byte first
            msg_id_data = conn.recv(N_BYTES_ID)
            if not msg_id_data:
                break

            msg_id = ord(msg_id_data)
                
            print(f"Message ID: {msg_id}")

            if (msg_id == MSG_STATE_Y) or (msg_id == MSG_STATE_X):
                data = conn.recv(N_BYTES_SMALL_MSG)

                if not data:
                    break  # No data means the client has closed the connection

                state_element_value = struct.unpack('<f', data)[0]
                print(f"msg_id              = {msg_id:02x}")
                print(f"state_element_value = {state_element_value}")

                if msg_id == MSG_STATE_Y:
                    y_position = state_element_value

                if msg_id == MSG_STATE_X:
                    x_position = state_element_value
            
            elif (msg_id == MSG_MAP):
                data = conn.recv(N_BYTES_MAP_MSG)
                
                if not data:
                    break  # No data means the client has closed the connection

                if msg_id == MSG_STATE_Y:
                    y_position = state_element_value

                if msg_id == MSG_STATE_X:
                    x_position = state_element_value


            # Process the data (for now, we'll just print it)
            print(f"Received from {addr}: ", data)


    except Exception as e:
        print(f"Error handling data from {addr}: {e}")
    except KeyboardInterrupt:
        print("Keyboard Interrupt")
        sys.exit(130)
    finally:
        conn.close()
        print(f"Connection closed with {addr}")

def send_byte(conn, dt=1.0):
    global data
    try:
        t_new = time.time()
        t_old = time.time()

        while True:
            t_new = time.time()
            if (t_new - t_old) >= dt:
                t_old = t_new
                conn.sendall(bytes([data]))
                data += 1
                data %= 0xff


    except Exception as e:
        print(f"Error sending data {data}: {e}")
    except KeyboardInterrupt:
        print("Keyboard Interrupt")
        sys.exit(130)
    finally:
        conn.close()
        print(f"Connection closed")

def plot_vehicle():
    global data 
    global data_old
    global x_position
    global y_position

    fig, ax = plt.subplots()
    ax.set_title(f"Vehicle Position")
    ax.set_xlim([x_min, x_max])
    ax.set_ylim([y_min, y_max])
    vehicle = ax.plot(x_position, 
                        y_position, 
                        markersize=10, 
                        label="vehicle",
                        linestyle="none",
                        marker="o",
                        markeredgecolor="black",
                        animated=True)[0]
    ax.legend()
    ax.grid()
    plt.show(block=False)
    plt.pause(0.1)
    bg = fig.canvas.copy_from_bbox(fig.bbox)
    
    ax.draw_artist(vehicle)   
    fig.canvas.blit(fig.bbox)

    while True:
        if data != data_old:
            print(f"x_position={x_position:>6.3f}, y_position={y_position:>6.3f}")
            print(f"data_new={data}, data_old={data_old}")
            # fig.canvas.restore_region(bg)
            vehicle.set_data(y_position, x_position)
            ax.draw_artist(vehicle)
            fig.canvas.blit(fig.bbox)
            fig.canvas.flush_events()
            data_old = data

def start_server(host='127.0.0.1', port=9200):

    dt = 0.1

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((host, port))
        server_socket.listen()
        print(f"Server listening on {host}:{port}")
        
        while True:
            conn, addr = server_socket.accept()

            print(f"conn = {conn}, addr = {addr}")

            # Start a new thread to handle the connection
            thread_read = threading.Thread(target=handle_client, args=(conn, addr))
            thread_send = threading.Thread(target=send_byte, args=(conn, dt))
            thread_plot = threading.Thread(target=plot_vehicle)

            thread_read.start()
            thread_send.start()
            thread_plot.start()
            
            print(f"Active connections: {threading.active_count() - 1}")

if __name__ == '__main__':
    start_server()
