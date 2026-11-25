import packet_test as pt
import cifar_interface as ci

LABELS = [
    "airplane", "automobile", "bird", "cat", "deer",
    "dog", "frog", "horse", "ship", "truck",
]

def decode_scores(raw_bytes: bytes):
    # Interpret bytes as signed int8 (-128..127)
    signed_vals = memoryview(raw_bytes).cast("b")
    scores = []
    for i, q in enumerate(signed_vals):
        prob = (q + 128) * 0.00390625
        label = LABELS[i] if i < len(LABELS) else f"class_{i}"
        print(f"category {label}, prob {prob:.6f}")
        scores.append((label, prob))
    return scores

def run_test():
    QEMU_HOST = "127.0.0.1"
    QEMU_PORT = 4444  # Change this to match your QEMU TCP port configuration
    img_bytes, img_label = ci.get_image()
    print(img_label)
    rtn = pt.communicate_with_qemu_tcp(host=QEMU_HOST, port=QEMU_PORT, payload_data=img_bytes, size_hint=10)
    print(rtn)
    decode_scores(rtn)
if __name__ == "__main__":
    run_test()
    
