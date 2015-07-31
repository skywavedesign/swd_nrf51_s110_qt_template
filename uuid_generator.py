
import uuid

hgxUUID = uuid.uuid4()

print(hgxUUID)
print(" ".join(["0x%02X"%byte for byte in hgxUUID.bytes[::-1]]))
with open('uuids.txt', 'w') as outFile:
    outFile.write(str(hgxUUID).upper() + "\n")
    outFile.write(",".join(["0x%02X"%byte for byte in hgxUUID.bytes[::-1]]))
    #outFile.write(hgxUUID.bytes().split
