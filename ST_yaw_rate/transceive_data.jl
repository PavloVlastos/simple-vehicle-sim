using Sockets
using Printf

rx_float32_value = 0.0
rx_error_thresh = 1e10
sock = 0
const MSG_STATE_PSI_DOT = 0x74

function transceive_init(ip::String, port::Int)
	server = listen(PORT_STRESS)
	if !isopen(server)
		println("transceive_data.jl: Failed to listen to $ip:$port")
	end

	global sock = accept(server)

	if !isopen(sock)
		println("transceive_data.jl: Failed to accept $server")
	end

	return sock
end

# Convert UInt32 from network byte order (big-endian) to host byte order
function ntohl(x::UInt32)
	return bswap(x)
end

# Convert 4 bytes to Float32 with endianness handling
function bytes_to_float32(float_bytes::Vector{UInt8})
	uint_value = reinterpret(UInt32, float_bytes)[1]
	host_value = uint_value

	return reinterpret(Float32, host_value)
end

function transceive_read_bytes_from_simulation()
	try
		data = read(sock, 5)

		if length(data) == 5

			message_id = data[1]  # First byte as message ID

            if message_id == MSG_STATE_PSI_DOT
                float_bytes = data[2:5]  # Remaining 4 bytes as float bytes
                f = bytes_to_float32(float_bytes)

                if abs(f) > rx_error_thresh
                    # println("transceive_data.jl: value is too large: f = $f")
                    return rx_float32_value
                else
                    global rx_float32_value = f
                end
            end
		else
			println("transceive_data.jl: Incomplete message received, bytes received: ", string(data))
			close(sock)
		end
	catch e
		println("transceive_data.jl: Connection error: ", e)
	end

    return rx_float32_value
end

function transceive_get_rx_float32()
	return rx_float32_value
end


# Convert a Float32 to 4 bytes (in network byte order)
function float32_to_bytes(value::Float32)::Vector{UInt8}
	# Reinterpret the float as a UInt32
	int_value = reinterpret(UInt32, value)

	# Convert to network byte order (big-endian)
	network_value = bswap(int_value)

	# Convert the UInt32 to 4 bytes
	return reinterpret(UInt8, [network_value])
end

# Send a Float32 value over TCP to the specified host and port
function transceive_send_float(value::Float32)
	# Convert the float to 4 bytes and send it over the socket
	bytes = float32_to_bytes(value)
	try
		write(sock, bytes)
		flush(sock)
	catch e
		println("transceive_data.jl: Failed to send float: $value as bytes: ", bytes)
	end
end

function transceive_send_cmd_byte(value::UInt8)
	try
		write(sock, value)
		flush(sock)
	catch e
		println("transceive_data.jl: Failed to send float: $value as bytes: ", bytes)
	end
end

function transceive_send_reset_cmd()
	transceive_send_cmd_byte(0x01)
	transceive_send_cmd_byte(0x20)
	transceive_send_cmd_byte(0x03)
	transceive_send_cmd_byte(0xAA)
	transceive_send_cmd_byte(0xEE)
	transceive_send_cmd_byte(0x03)
	transceive_send_cmd_byte(0x20)
	transceive_send_cmd_byte(0x01)
end