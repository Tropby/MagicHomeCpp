# MagicHome-EBCpp API

Based on [MagicHome-Python](https://github.com/adamkempenich/magichome-python).

This class is designed to use MagicHome Devices with EBCpp.

## Binary from release file

The binary from the release file shows how to set parameters for the
led controller.

### Example

This example shows how to turn on the device an set the color to red.

```cmd
MagicHomeProtocol 0 tcp://192.168.222.151:5577 ON COLOR=FF0000*
```

### Parameter

```cmd
MagicHomeProtocol {DEVICE_TYPE} {IP} {COMMAND} [COMMAND]
```

#### DEVICE_TYPE

```cmd
    0: RGB
    1: RGB+WW
    2: RGB+WW+CW
    3: Blub (v.4+)
    4: Blub (v.3-)
```

#### IP-Address

```cmd
    ip address of the led controller
    Example: tcp://192.168.222.151:5577
```

#### COMMAND

```cmd
    ON: turn on a device

    OFF: turn off a device

    COLOR=RRGGBBWWCW: set the color
            RR = red channel,
            GG = green channel,
            BB = blue channel,
            WW = warm white channel,
            CW = cold white channel
```
