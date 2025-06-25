// This Frida script was used to intercept BLE commands sent from the app to the guitar.
// It hooks into the writeCharacteristic and setValue methods to log UUIDs and data in hexadecimal format.

Java.perform(function () {
    var BluetoothGatt = Java.use("android.bluetooth.BluetoothGatt");
    var BluetoothGattCharacteristic = Java.use("android.bluetooth.BluetoothGattCharacteristic");

    BluetoothGatt.writeCharacteristic.implementation = function (characteristic) {
        var uuid = characteristic.getUuid().toString();
        var value = characteristic.getValue();

        var data = "";
        if (value !== null) {
            for (var i = 0; i < value.length; i++) {
                data += ('0' + (value[i] & 0xFF).toString(16)).slice(-2) + ' ';
            }
        } else {
            data = "[null]";
        }

        console.log("[BLE] writeCharacteristic -> UUID: " + uuid + " | Data (hex): " + data.trim());

        return this.writeCharacteristic(characteristic);
    };

    BluetoothGattCharacteristic.setValue.overload('[B').implementation = function (b) {
        var data = "";
        for (var i = 0; i < b.length; i++) {
            data += ('0' + (b[i] & 0xFF).toString(16)).slice(-2) + ' ';
        }

        console.log("[BLE] setValue -> Data (hex): " + data.trim());
        return this.setValue(b);
    };

    console.log("BLE write interceptor attached.");
});
