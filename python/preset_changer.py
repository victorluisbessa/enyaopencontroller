#### This python script send commands to guitar using python and computer internal BLE device


import asyncio
from bleak import BleakClient


DEVICE_ADDRESS = "xx:xx:xx:xx:xx:xx" 


CHAR_UUID = "0000ab12-0000-1000-8000-00805f9b34fb"


BASE_PAYLOAD = bytearray(
    [0xAA, 0x55, 0x0A, 0x00, 0x0C, 0x03, 0x00, 0x02, 0x00, 0x02, 0xE2, 0x55, 0xAA]
)


async def send_payload(choice: int):
    if choice not in [0, 1, 2, 3]:
        print("Escolha inválida. Use 0, 1, 2 ou 3.")
        return

    payload = bytearray(BASE_PAYLOAD)
    payload[5] = choice

    print(f"Payload que será enviado: {' '.join(f'{b:02x}' for b in payload)}")

    async with BleakClient(DEVICE_ADDRESS) as client:
        connected = await client.is_connected()
        if connected:
            print("Conectado ao dispositivo.")
            await client.write_gatt_char(CHAR_UUID, payload)
            print("Payload enviado com sucesso!")
        else:
            print("Falha ao conectar.")


def main():
    print("Qual payload deseja enviar? (0,1,2,3)")
    escolha = input("Digite o número: ")
    try:
        escolha_int = int(escolha)
    except ValueError:
        print("Entrada inválida. Informe um número.")
        return

    asyncio.run(send_payload(escolha_int))


if __name__ == "__main__":
    main()
