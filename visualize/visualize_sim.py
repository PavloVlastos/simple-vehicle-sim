import socket
import sys
import threading
import struct
import time
import matplotlib.pyplot as plt
import matplotlib
import os
import re

matplotlib.use("TkAgg")

x_position = 0.0
y_position = 0.0
x_wps = []
y_wps = []
heading_angle = 0.0
x_min = -20.0
x_max = 20.0
y_min = -20.0
y_max = 20.0
data = 0x01
data_old = 0x00

# These values are read in from the common.h file
msg_state_x = None
msg_state_y = None
msg_state_psi = None
msg_state_x = None
msg_state_y = None
msg_map = None

N_BYTES_ID = 1
N_BYTES_SMALL_MSG = 4
N_BYTES_MAP_MSG = 2500


def handle_client(conn, addr):
    print(f"Connected by {addr}")

    # Simulation varables
    global heading_angle
    global x_position
    global y_position
    global x_wps
    global y_wps

    # Message IDs
    global msg_state_x
    global msg_state_y
    global msg_state_psi
    global msg_state_x
    global msg_state_y
    global msg_map

    try:
        while True:
            # Get the message ID byte first
            msg_id_data = conn.recv(N_BYTES_ID)
            if not msg_id_data:
                break

            msg_id = ord(msg_id_data)

            print(f"Message ID: {msg_id}")

            if (
                (msg_id == msg_state_y)
                or (msg_id == msg_state_x)
                or (msg_id == msg_state_psi)
                or (msg_id == msg_target_x)
                or (msg_id == msg_target_y)
            ):
                data = conn.recv(N_BYTES_SMALL_MSG)

                if not data:
                    break  # No data means the client has closed the connection

                msg_payload = struct.unpack("<f", data)[0]
                print(f"msg_id              = {msg_id:02x}")
                print(f"msg_payload = {msg_payload}")

                if msg_id == msg_state_x:
                    x_position = msg_payload

                if msg_id == msg_state_y:
                    y_position = msg_payload

                if msg_id == msg_state_psi:
                    heading_angle = msg_payload

                if msg_id == msg_target_x:
                    if msg_payload not in x_wps:
                        x_wps.append(msg_payload)

                if msg_id == msg_target_y:
                    if msg_payload not in y_wps:
                        y_wps.append(msg_payload)

            elif msg_id == msg_map:
                data = conn.recv(N_BYTES_MAP_MSG)

                if not data:
                    break  # No data means the client has closed the connection

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
                data %= 0xFF

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
    global heading_angle

    fig, ax = plt.subplots()
    ax.set_title(f"Vehicle Position")
    ax.set_xlim([x_min, x_max])
    ax.set_ylim([y_min, y_max])
    vehicle = ax.plot(
        x_position,
        y_position,
        markersize=10,
        label="vehicle",
        linestyle="none",
        marker=(3, 0, heading_angle),
        markeredgecolor="black",
        animated=True,
    )[0]
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
            fig.canvas.restore_region(bg)
            vehicle.set_data(y_position, x_position)
            ax.draw_artist(vehicle)
            fig.canvas.blit(fig.bbox)
            fig.canvas.flush_events()
            data_old = data


def start_server(host="127.0.0.1", port=9200):

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


def read_message_ids():
    global msg_state_x
    global msg_state_y
    global msg_state_psi
    global msg_target_x
    global msg_target_y

    current_dir = os.path.dirname(os.path.abspath(__file__))
    common_h_path = os.path.join(current_dir, "..", "modules", "common", "common.h")

    try:
        with open(common_h_path, "r") as file:
            content = file.read()

        # MSG_STATE_X
        match = re.search(r"#define\s+MSG_STATE_X\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_state_x = match.group(1)  # Set the global variable with the value
            msg_state_x = int(msg_state_x, 16)
            print(f"MSG_STATE_X is defined as: {msg_state_x}")
        else:
            print("MSG_STATE_X not found in the file.")

        # MSG_STATE_Y
        match = re.search(r"#define\s+MSG_STATE_Y\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_state_y = match.group(1)  # Set the global variable with the value
            msg_state_y = int(msg_state_y, 16)
            print(f"MSG_STATE_Y is defined as: {msg_state_y}")
        else:
            print("MSG_STATE_Y not found in the file.")

        # MSG_STATE_PSI
        match = re.search(r"#define\s+MSG_STATE_PSI\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_state_psi = match.group(1)  # Set the global variable with the value
            msg_state_psi = int(msg_state_psi, 16)
            print(f"MSG_STATE_PSI is defined as: {msg_state_psi}")
        else:
            print("MSG_STATE_PSI not found in the file.")

        # MSG_TARGET_X
        match = re.search(r"#define\s+MSG_TARGET_X\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_target_x = match.group(1)  # Set the global variable with the value
            print(f"MSG_TARGET_X is defined as: {msg_target_x}")
            msg_target_x = int(msg_target_x, 16)
            
            # @TODO: delete this line
            print(f"type(msg_target_x)={type(msg_target_x)}")
        
        else:
            print("MSG_TARGET_X not found in the file.")

        # MSG_TARGET_Y
        match = re.search(r"#define\s+MSG_TARGET_Y\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_target_y = match.group(1)  # Set the global variable with the value
            msg_target_y = int(msg_target_y, 16)
            print(f"MSG_TARGET_Y is defined as: {msg_target_y}")
        else:
            print("MSG_TARGET_Y not found in the file.")

        # MSG_MAP
        match = re.search(r"#define\s+MSG_MAP\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_map = match.group(1)  # Set the global variable with the value
            msg_map = int(msg_map, 16)
            print(f"MSG_MAP is defined as: {msg_map}")
        else:
            print("MSG_MAP not found in the file.")

    except FileNotFoundError:
        print(f"File {common_h_path} not found.")


if __name__ == "__main__":
    print("Starting simulation visualizer...")
    print("Reading in message IDs...")

    read_message_ids()

    print("Starting simulation visualizer server")

    start_server()
