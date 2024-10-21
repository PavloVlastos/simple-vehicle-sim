using Sockets
using Printf

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

# Function to handle a client connection (read & print message)
function handle_client(sock::TCPSocket)
    try
        # Loop to receive messages continuously from the same connection
        while true

            data = read(sock, 5)

            if length(data) == 5
                println("Raw bytes received: ", string(data))

                message_id = data[1]  # First byte as message ID
                float_bytes = data[2:5]  # Remaining 4 bytes as float bytes
                float_value = bytes_to_float32(float_bytes)

                s = @sprintf("Received Message ID: %d, Float Bytes: %s, Float Value: %.6f",
               message_id, string(float_bytes), Float64(float_value))
               println(s)
            else
                @sprintf("Error: Incomplete message received.")
                break  
            end
        end
    catch e
        println("Connection error: ", e)
    finally
        close(sock)
    end
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
function send_float(host::String, port::Int, value::Float32)
    sock = connect(host, port)  # Connect to the C program
    try
        # Convert the float to 4 bytes and send it over the socket
        bytes = float32_to_bytes(value)
        write(sock, bytes)
        flush(sock)  # Ensure the data is sent immediately
        println("Sent float: $value as bytes: ", bytes)
    finally
        close(sock)  # Close the socket
    end
end

function start_server(ip::String, port::Int)
    server = listen(port)  
    println("Server listening on $ip:$port...")

    while true
        sock = accept(server) 
        println("Client connected.")
        @async handle_client(sock)
    end

    println("Error: closing server prematurely")

    close(server)  # Close the server socket
end

# # Start the server
# start_server("0.0.0.0", 65432)
