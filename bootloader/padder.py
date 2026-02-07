import os

BOOTLOADER_SIZE = 0x8000  # 32KB
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BOOTLOADER_FILE = os.path.join(SCRIPT_DIR, "build", "bootloader.bin")

with open(BOOTLOADER_FILE, "rb") as f:
    raw_file = f.read()

bytes_to_pad = BOOTLOADER_SIZE - len(raw_file)
padding = bytes([0xff for _ in range(bytes_to_pad)])

with open(BOOTLOADER_FILE, "wb") as f:
    f.write(raw_file + padding)