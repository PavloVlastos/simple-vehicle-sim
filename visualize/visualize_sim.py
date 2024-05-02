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

def handle_client(conn, addr):
    print(f"Connected by {addr}")

    global x_position
    global y_position

    try:
        while True:
            data = conn.recv(5)  # Receive exactly 5 bytes
            if not data:
                break  # No data means the client has closed the connection
            # Process the data (for now, we'll just print it)
            print(f"Received from {addr}: ", data)
            state_element = data[0]
            state_element_value = struct.unpack('<f', data[-4:])[0]
            print(f"state_element      = {state_element:02x}")
            print(f"state_element_value = {state_element_value}")

            if state_element == 0x78:
                y_position = state_element_value

            if state_element == 0x79:
                x_position = state_element_value

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
