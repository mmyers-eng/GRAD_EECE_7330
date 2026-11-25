import io, pickle, random, tarfile, urllib.request
import matplotlib.pyplot as plt

def get_image():
    with open("cifar-10-batches-py/batches.meta", "rb") as mf:
        meta = pickle.load(mf, encoding="bytes")

    with open("cifar-10-batches-py/data_batch_1", "rb") as bf:
        batch = pickle.load(bf, encoding="bytes")

    i = random.randrange(len(batch[b"labels"]))
    img_chw = batch[b"data"][i].reshape(3, 32, 32)
    img_hwc = img_chw.transpose(1, 2, 0)
    label = meta[b"label_names"][batch[b"labels"][i]].decode()

    # Convert uint8 [0..255] -> int8 [-128..127] in HWC order
    q_bytes = [(int(v) - 128) & 0xFF for v in img_hwc.reshape(-1)]
    return(bytes(q_bytes), label)
#    lines = [
#        ", ".join(f"0x{b:02x}" for b in q_bytes[j : j + 12])
#        for j in range(0, len(q_bytes), 12)
#   ]

#    sym = f"cifar_{i}_{label}_int8_hwc"
#with open(f"{sym}.c", "w") as f:
#    f.write(f"signed char {sym}[] = {{\n  " + ",\n  ".join(lines) + "\n};\n")
#    f.write(f"unsigned int {sym}_len = {len(q_bytes)};\n")

#plt.imshow(img_hwc)
#plt.title(f"{label} (#{i})")
#plt.axis("off")
#plt.show()
