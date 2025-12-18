# PCA9635
The `pca9635` component allows you to use the PCA9635 16-channel **8-bit** [I²C](https://esphome.io/components/i2c/) LED driver in ESPHome. It uses the [I²C](https://esphome.io/components/i2c/) Bus for communication.

This chip is very similar to the popular PCA9685, but it offers **8-bit** resolution (0-255 steps) rather than **12-bit**.

To use the channels of this component, you first need to setup the global `pca9635` hub and give it an id, and then define the individual output channels.

## Component/Hub
```yaml
# Example configuration entry
pca9635:
  id: pca9635_hub
  address: 0x15
```
Configuration variables:

* **address** (*Optional*, int): The [I²C](https://esphome.io/components/i2c/) address of the driver. Defaults to `0x15`. Depending on your hardware strapping (A0-A6 pins), this may vary. Common addresses include `0x15` or `0x70` (All-Call).

* **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types/)): The id to use for this pca9635 component. Use this if you have multiple PCA9635s connected at the same time.

* **i2c_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types/)): The id of the I²C bus to use.

## PCA9635 Output
The pca9635 output platform allows you to use a single channel of the driver as a Float Output.

## Individual outputs
```yaml
output:
  - platform: pca9635
    pca9635_id: pca9635_hub
    channel: 0
    id: led_output_1
```
Configuration variables:

**channel** (*Required*, int): Choose the channel of the PCA9635 for this output. Must be in range from 0 to 15.

**pca9635_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types/)): Manually specify the ID of the PCA9635 hub. Use this if you have multiple PCA9635s you want to use at the same time.
**id** (*Required*, [ID](https://esphome.io/guides/configuration-types/)): The id to use for this output component.

All other options from Output (e.g., inverted, min_power, max_power).

## Examples
### Using as a Dimmable Light

The most common use case is connecting LEDs to the PCA9635 and controlling them as a light in Home Assistant.

```yaml
i2c:
  sda: GPIO21
  scl: GPIO22

pca9635:
  id: my_driver
  address: 0x15

output:
  # Define the output for Channel 0
  - platform: pca9635
    id: output_channel_0
    channel: 0
    pca9635_id: my_driver

light:
  # Link the output to a Light entity
  - platform: monochromatic
    name: "Kitchen Light"
    output: output_channel_0
    default_transition_length: 0.5s
```
### Using as an RGB Light

You can combine three channels to create a full RGB light.
```
output:
  - platform: pca9635
    id: r_out
    channel: 0
  - platform: pca9635
    id: g_out
    channel: 1
  - platform: pca9635
    id: b_out
    channel: 2

light:
  - platform: rgb
    name: "Living Room Strip"
    red: r_out
    green: g_out
    blue: b_out
```
