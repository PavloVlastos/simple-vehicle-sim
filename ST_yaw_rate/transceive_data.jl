using Sockets
using Printf

# Convert 4 bytes to Float32 (handling byte order)
function bytes_to_float32(float_bytes::Vector{UInt8})
    # Reinterpret as UInt32, convert from network byte order (big-endian)
    uint_value = ntohl(reinterpret(UInt32, float_bytes))
    # Reinterpret the UInt32 as Float32
    return reinterpret(Float32, uint_value)
end

# Function to handle a client connection (read & print message)
function handle_client(sock::TCPSocket)
    try
        # Loop to receive messages continuously from the same connection
        while true
            # Read exactly 5 bytes (1 byte ID + 4 bytes for float)
            data = read(sock, 5)

            if length(data) == 5
                message_id = data[1]  # First byte as message ID
                float_bytes = data[2:5]  # Remaining 4 bytes as float bytes
                float_value = bytes_to_float32(float_bytes)

                # Print out the received values
                @sprintf("Received Message ID: %d, Float Bytes: %s, Float Value: %.6f",
               message_id, string(float_bytes), Float64(float_value))
            else
                @sprintf("Error: Incomplete message received.")
                break  # Stop if the message is incomplete
            end
        end
    catch e
        println("Connection error: ", e)
    finally
        close(sock)  # Close the client socket
    end
end

# Start the server on a specific IP address and port
function start_server(ip::String, port::Int)
    server = listen(port)  # Correct conversion to IPAddr
    println("Server listening on $ip:$port...")

    # while true
    #     sock = accept(server)  # Accept a client connection
    #     println("Client connected.")

    #     # Handle client communication in a separate task
    #     @async handle_client(sock)
    # end

    # close(server)  # Close the server socket
end

# # Start the server
# start_server("0.0.0.0", 65432)
