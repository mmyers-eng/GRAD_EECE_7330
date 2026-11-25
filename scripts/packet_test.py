import socket
import struct
import zlib
import numpy as np
import matplotlib.pyplot as plt

def do_plot(x_inputs, result):
    y_results = np.sin(x_inputs)

    plt.figure(figsize=(8, 5))
    plt.plot(x_inputs, y_results, label='y = sin(x)', color='b', linewidth=2)
    plt.plot(x_inputs, result, label='inference', color='r', linewidth=2)

    plt.title('Sine Wave: Inputs vs. Results', fontsize=14)
    plt.xlabel('Inputs (x)', fontsize=12)
    plt.ylabel('Results (sin(x))', fontsize=12)
    plt.grid(True, linestyle=':', alpha=0.7)
    plt.legend(fontsize=12)
    plt.show()

def build_packet(payload: bytes) -> bytes:
    """
    Constructs a packet matching the C++ PacketFrame layout:
    - Magic bytes: 0xCE, 0xEC, 0xDE, 0xED
    - Length: u32 (little-endian)
    - Payload: raw bytes
    - CRC: u32 (little-endian) computed over magic + len + payload
    """
    magic = b"\xCE\xEC\xDE\xED"
    
    length = len(payload)
    if length > 4096:
        raise ValueError(f"Payload length {length} exceeds MaxLen (1024)")
    
    header = magic + struct.pack("<I", length)
    packet_without_crc = header + payload
    
    # Calculate CRC-32 (standard IEEE format, masked to unsigned 32-bit)
    crc_val = zlib.crc32(packet_without_crc) & 0xFFFFFFFF
    crc_bytes = struct.pack("<I", crc_val)
    
    return packet_without_crc + crc_bytes

def communicate_with_qemu_tcp(host: str, port: int, payload_data: bytes, size_hint=4):
    """
    Connects to QEMU via a TCP socket (AF_INET), sends the test packet, and prints the response.
    """
    print(f"Connecting to TCP socket at {host}:{port}...")
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    response = None
    try:
        sock.connect((host, port))
        
        # Build and send the packet
        packet = build_packet(payload_data)
        print(f"-> Sending packet ({len(packet)} bytes): {packet.hex()}")
        sock.sendall(packet)
        
         # Read response with a timeout
        sock.settimeout(10.0)
        response = b''
        try:
            for t in range(size_hint):
                response = response + sock.recv(size_hint)
        except socket.timeout:
            print("<- Timeout waiting for response (target might not echo back).")
        
        if response:
            print(f"<- Received response ({len(response)} bytes): {response}")
            print(f"   Hex: {response.hex()}")
        else:
            print("<- Connection closed by peer.")
            
    except ConnectionRefusedError:
        print(f"X Connection refused. Ensure QEMU is running and listening on {host}:{port}.")
    finally:
        sock.close()
    return(response)


def send_list(listVals):
    rtn = list()
    
    QEMU_HOST = "127.0.0.1"
    QEMU_PORT = 4444  # Change this to match your QEMU TCP port configuration
    resultList = list()
    count = 0
    for val in listVals:
        count = count + 1
        test_payload = struct.pack('<f', val)
        print(f"sending {val}, count: {count}")
        rtn = communicate_with_qemu_tcp(host=QEMU_HOST, port=QEMU_PORT, payload_data=test_payload)
        if(rtn):
            res = struct.unpack('<f', rtn)[0]
            resultList.append(res)
            print(f"got {res}")
    print(resultList)
    return(resultList)
    


if __name__ == "__main__":
    x_inputs = np.linspace(-4 * np.pi, 4 * np.pi, 100)
    results = send_list(x_inputs)
    do_plot(x_inputs,results)
