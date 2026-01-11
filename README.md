Source code for ESP32 board on BiteHack hackathon.


setup

in repo do

```
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
sudo mv bin/arduino-cli /usr/local/bin/
arduino-cli version
arduino-cli config init
arduino-cli core update-index
arduino-cli config set network.connection_timeout 600s
arduino-cli core install esp32:esp32

arduino-cli core list

arduino-cli lib install ESP32Servo
arduino-cli lib install FastLED
```

cmds
```
cmake -S . -B build
cmake --build build --target build
cmake --build build --target upload

```