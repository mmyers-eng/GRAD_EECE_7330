import socket
import struct
import zlib

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
    if length > 1024:
        raise ValueError(f"Payload length {length} exceeds MaxLen (1024)")
    
    header = magic + struct.pack("<I", length)
    packet_without_crc = header + payload
    
    # Calculate CRC-32 (standard IEEE format, masked to unsigned 32-bit)
    crc_val = zlib.crc32(packet_without_crc) & 0xFFFFFFFF
    crc_bytes = struct.pack("<I", crc_val)
    
    return packet_without_crc + crc_bytes

def communicate_with_qemu_tcp(host: str, port: int, payload_data: bytes):
    """
    Connects to QEMU via a TCP socket (AF_INET), sends the test packet, and prints the response.
    """
    print(f"Connecting to TCP socket at {host}:{port}...")
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        sock.connect((host, port))
        
        # Build and send the packet
        packet = build_packet(payload_data)
        print(f"-> Sending packet ({len(packet)} bytes): {packet.hex()}")
        sock.sendall(packet)
        
         # Read response with a timeout
        sock.settimeout(1.0)
        response = b''
        try:
            for t in range(1024):
                response = response + sock.recv(1024)
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

if __name__ == "__main__":
    # Test payload string
    test_payload = b"Test Payload"
    
    QEMU_HOST = "127.0.0.1"
    QEMU_PORT = 4444  # Change this to match your QEMU TCP port configuration
    
    communicate_with_qemu_tcp(host=QEMU_HOST, port=QEMU_PORT, payload_data=test_payload)
