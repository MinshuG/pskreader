from psk import PSK

psk = PSK(r"L:\Coding Stuff\UE4_PSKX\AlotOAuto\Mesh\SM_AlotOAuto_Sign_Side.pskx")
psk.deserialize()
psk.close()
print(psk)
