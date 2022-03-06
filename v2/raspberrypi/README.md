This project is able to drive the v2 LED matrix from a raspberry pi 4.

# General info
## Configuring the raspberry pi
### SPI
We can configure the SPI very bare-bones, we do not need the MISO line and the default CS behaviour does not work for us.
Therefore, add the following to `/boot/config`:
```
dtoverlay=spi0-0cs,no_miso
```
If you want to be able to use the spi device as a non-root user, you should create the file `/etc/udev/rules.d/50-spi.rules` with the following content:
```
SUBSYSTEM=="spidev", GROUP="spiuser", MODE="0660"
```
And add your own user to the `spiuser` group:
```
usermod -a -G spiuser <user>
```
### GPIO
In order to be able to use the GPIO as a non-root user, you should create the file `/etc/udev/rules.d/90-gpio.rules` with the following content:
```
SUBSYSTEM=="gpio", GROUP="gpiouser", MODE="0660"
```
And then add yourself to the `gpiouser` group
```
usermod -a -G gpiouser <user>
```
We use the `gpiod` library to communicate with the pins, therefore you will need a compiled version of libgpiod in here before you can build this program. A way to achieve this is:
1. Clone `https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git` to some location.
2. Enter the directory and run `./autogen.sh --enable-tools=no --prefix=/tmp/libgpiod --host=arm-linux-gnueabihf`
3. Compile with `make install`
4. Copy `/tmp/libgpiod` here
## Connection
The silkscreen for the male side contains an error, the silkscreen for the female side is correct. You want to connect the female side to the Raspberry Pi 4. Following [This image](https://www.raspberrypi.com/documentation/computers/images/GPIO-Pinout-Diagram-2.png) we get:
- `Vcc` <--> `5v`
- `GND` <--> `GND`
- `Load` <--> `21` (GPIO 9)
- `CLK` <--> `23` (SCLK)
- `D_in` <--> `19` (MOSI)
- `D_fwd` `N.C.` (d_fwd needs to be connected to another LED matrix)
