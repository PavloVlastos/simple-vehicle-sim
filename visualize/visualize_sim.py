import socket
import sys
import threading
import struct
import time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import os
import re

matplotlib.use("TkAgg")

x_position = 0.0
y_position = 0.0
x_wps = 0.0 
y_wps = 0.0
heading_angle = 0.0
last_angle = 0.0
x_min = -20.0
x_max = 20.0
y_min = -20.0
y_max = 20.0
data = 0x01
data_old = 0x00

t_new = time.time()
t_old = t_new

# These values are read in from the common.h file
msg_ping = None
msg_state_x = None
msg_state_y = None
msg_state_psi = None
msg_state_x = None
msg_state_y = None
msg_map = None

N_BYTES_ID = 1
N_BYTES_SMALL_MSG = 4
N_BYTES_MAP_MSG = 2500
SUCCESS = 0
ERROR = -1


def handle_client(conn, addr):
    status = 0

    # Simulation varables
    global heading_angle
    global x_position
    global y_position
    global x_wps
    global y_wps

    # Message IDs
    global msg_ping
    global msg_state_x
    global msg_state_y
    global msg_state_psi
    global msg_target_x
    global msg_target_y
    global msg_map

    try:
        # Get the message ID byte first
        msg_id_data = conn.recv(N_BYTES_ID)
        if not msg_id_data:
            print(f"Error: msg_id_data: {msg_id_data}")
            return ERROR

        msg_id = ord(msg_id_data)

        print(f"Message ID: 0x{msg_id:02x}")

        if (
            (msg_id == msg_state_x)
            or (msg_id == msg_state_y)
            or (msg_id == msg_state_psi)
            or (msg_id == msg_target_x)
            or (msg_id == msg_target_y)
            or (msg_id == msg_ping)
        ):
            data = conn.recv(N_BYTES_SMALL_MSG)

            if not data:
                return ERROR

            msg_payload = struct.unpack("<f", data)[0]
            print(f"msg_id              = 0x{msg_id:02x}")
            print(f"msg_payload         = {msg_payload}")

            if msg_id == msg_ping:
                return SUCCESS

            if msg_id == msg_state_x:
                x_position = msg_payload

            if msg_id == msg_state_y:
                y_position = msg_payload

            if msg_id == msg_state_psi:
                heading_angle = msg_payload

            if msg_id == msg_target_x:
                x_wps = msg_payload

            if msg_id == msg_target_y:
                y_wps = msg_payload

        # elif msg_id == msg_map:
        #     data = conn.recv(N_BYTES_MAP_MSG)

        #     if not data:
        #         status = ERROR
        #         return status
        #         # break  # No data means the client has closed the connection

        # Process the data (for now, we'll just print it)
        print(f"Received from {addr}: ", data)

    except Exception as e:
        print(f"Error handling data from {addr}: {e}")
    except KeyboardInterrupt:
        print("Keyboard Interrupt")
        sys.exit(130)

    return status


def init_plot_vehicle():
    global data
    global data_old
    global x_position
    global y_position
    global heading_angle
    global last_angle
    global x_wps
    global y_wps

    vehicle_marker = matplotlib.markers.MarkerStyle("^")
    vehicle_marker._transform.rotate(last_angle)
    vehicle_marker._transform.rotate(-heading_angle)
    last_angle = heading_angle

    fig, ax = plt.subplots()
    ax.set_title("Vehicle Position")
    ax.set_xlabel("x (meters)")
    ax.set_ylabel("y (meters)")
    ax.set_xlim([x_min, x_max])
    ax.set_ylim([y_min, y_max])

    waypoints = ax.plot(
        x_wps,
        y_wps,
        markersize=10,
        label="Target Waypoint",
        linestyle="none",
        marker="x",
        markeredgecolor="red",
        animated=True,
    )[0]

    vehicle = ax.plot(
        x_position,
        y_position,
        markersize=10,
        label="Vehicle",
        linestyle="none",
        marker=vehicle_marker,
        markeredgecolor="black",
        markerfacecolor="tab:blue",
        animated=True,
    )[0]

    ax.legend()
    ax.grid()
    plt.show(block=False)
    plt.pause(0.1)

    bg = fig.canvas.copy_from_bbox(fig.bbox)

    # ax.draw_artist(vehicle)
    # ax.draw_artist(waypoints)
    # fig.canvas.blit(fig.bbox)
    # fig.canvas.flush_events()

    return fig, vehicle_marker, waypoints, vehicle, bg, ax


def plot_vehicle(fig, vehicle_marker, waypoints, vehicle, bg, ax):
    global data
    global data_old
    global x_position
    global y_position
    global heading_angle
    global last_angle
    global x_wps
    global y_wps

    # while True:
    # if data != data_old:

    fig.canvas.restore_region(bg)

    bg = fig.canvas.copy_from_bbox(fig.bbox)

    vehicle_marker._transform.rotate(last_angle)
    vehicle_marker._transform.rotate(-heading_angle)


    # print(f"x_position={x_position:>6.3f}, y_position={y_position:>6.3f}")
    # print(f"data_new={data}, data_old={data_old}")
    last_angle = heading_angle

    # waypoints.set_data(y_wps, x_wps)
    # vehicle.set_data(y_position, x_position)
    waypoints.set_data(x_wps, y_wps)
    vehicle.set_data(x_position, y_position)

    ax.draw_artist(waypoints)
    ax.draw_artist(vehicle)

    fig.canvas.blit(ax.bbox)
    fig.canvas.flush_events()

    # bg = fig.canvas.copy_from_bbox(fig.bbox)

    data_old = data

    return fig, vehicle_marker, waypoints, vehicle, bg, ax


def run_server(host="127.0.0.1", port=9200):
    status = 0
    global t_new
    global t_old
    global data

    dt = 0.05

    fig, vehicle_marker, waypoints, vehicle, bg, ax = init_plot_vehicle()

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))

    server_socket.listen()
    print(f"Server listening on {host}:{port}")

    conn, addr = server_socket.accept()
    print(f"connnected to {addr}")

    conn.settimeout(0.01)

    # count = 0
    # dc = 2

    try:

        while True:

            print("send_byte()")

            print(f"dt = {dt}")

            t_new = time.time()
            if (t_new - t_old) >= dt:
                t_old = t_new
                print(f"data = {data}, type(data)={type(data)}")
                conn.sendall(bytes([data]))
                data += 1
                data %= 0xFF

            print("handle_client()")
            handle_client(conn, addr)

            # if (count % dc == 0):
            print("plot_vehicle()")
            fig, vehicle_marker, waypoints, vehicle, bg, ax = plot_vehicle(
                fig, vehicle_marker, waypoints, vehicle, bg, ax
            )
            # count += 1

    except Exception as e:
        print(f"Error sending data 0x{data:X}: {e}")
    except KeyboardInterrupt:
        print("Keyboard Interrupt")
        sys.exit(130)
    finally:
        conn.close()
        print(f"run_server(): Connection closed")


def read_message_ids():
    global msg_ping
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

        # MSG_PING
        match = re.search(r"#define\s+MSG_PING\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_ping = match.group(1)  # Set the global variable with the value
            msg_ping = int(msg_ping, 16)
            print(f"MSG_PING is defined as:         0x{msg_ping:X}")
        else:
            print("MSG_PING not found in the file.")

        # MSG_STATE_X
        match = re.search(r"#define\s+MSG_STATE_X\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_state_x = match.group(1)  # Set the global variable with the value
            msg_state_x = int(msg_state_x, 16)
            print(f"MSG_STATE_X is defined as:      0x{msg_state_x:X}")
        else:
            print("MSG_STATE_X not found in the file.")

        # MSG_STATE_Y
        match = re.search(r"#define\s+MSG_STATE_Y\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_state_y = match.group(1)  # Set the global variable with the value
            msg_state_y = int(msg_state_y, 16)
            print(f"MSG_STATE_Y is defined as:      0x{msg_state_y:X}")
        else:
            print("MSG_STATE_Y not found in the file.")

        # MSG_STATE_PSI
        match = re.search(r"#define\s+MSG_STATE_PSI\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_state_psi = match.group(1)  # Set the global variable with the value
            msg_state_psi = int(msg_state_psi, 16)
            print(f"MSG_STATE_PSI is defined as:    0x{msg_state_psi:X}")
        else:
            print("MSG_STATE_PSI not found in the file.")

        # MSG_TARGET_X
        match = re.search(r"#define\s+MSG_TARGET_X\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_target_x = match.group(1)  # Set the global variable with the value
            msg_target_x = int(msg_target_x, 16)
            print(f"MSG_TARGET_X is defined as:     0x{msg_target_x:X}")

        else:
            print("MSG_TARGET_X not found in the file.")

        # MSG_TARGET_Y
        match = re.search(r"#define\s+MSG_TARGET_Y\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_target_y = match.group(1)  # Set the global variable with the value
            msg_target_y = int(msg_target_y, 16)
            print(f"MSG_TARGET_Y is defined as:     0x{msg_target_y:X}")
        else:
            print("MSG_TARGET_Y not found in the file.")

        # MSG_MAP
        match = re.search(r"#define\s+MSG_MAP\s+(0x[0-9A-Fa-f]+)", content)
        if match:
            msg_map = match.group(1)  # Set the global variable with the value
            msg_map = int(msg_map, 16)
            print(f"MSG_MAP is defined as:          0x{msg_map:X}")
        else:
            print("MSG_MAP not found in the file.")

    except FileNotFoundError:
        print(f"File {common_h_path} not found.")


if __name__ == "__main__":
    print("Starting simulation visualizer...")
    print("Reading in message IDs...")

    read_message_ids()

    print("Starting simulation visualizer server")

    status = run_server()

    if status == ERROR:
        print(f"server status = {status}")
