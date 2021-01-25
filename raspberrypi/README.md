# On the subject of permissions
To use this software, you need interaction with both gpio and spi. The default permissions for the relevant
devices is root-only, so that you would need to use sudo to run. This is not fun. To fix this, one can create two files:
`/etc/udev/rules.d/50-spi.rules`
```
SUBSYSTEM=="spidev", GROUP="spiuser", MODE="0660"
```
`/etc/udev/rules.d/90-gpio.rules`
```
SUBSYSTEM=="gpio", GROUP="gpiouser", MODE="0660"
```
After udev has been reloaded, the GPIO and SPI devices will be available for users in the groups gpiouser and spiuser.
So add yourself to these groups.

# On the subject of GPIOD
GPIOD is used to control pins directly. The disadvantage of this approach is that, in order to cross-compile, you need the library.
One way to achieve this is to download [the source](https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git), run `./autogen.sh --enable-tools=no --prefix=/tmp/libgpiod --host=arm-linux-gnueabihf`, then run `make install` and finally copy `/tmp/libgpiod` to here.

# On the subject of SPI
To control the LED matrix, we use an SPI to transmit data across.
For our purposes, SPI0 is chosen. This means that SPI0 should be enabled, so that `dtoverlay=spi0-1cs` must be added to `/boot/config.txt`.
See also the [raspberry pi documentation](https://www.raspberrypi.org/documentation/hardware/raspberrypi/spi/README.md).
