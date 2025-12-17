#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h" // This requires DEPENDENCIES=['output'] in __init__.py
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace pca9635 {

class PCA9635Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  void set_channel_value(uint8_t channel, float state);

 protected:
  bool write_register(uint8_t reg, uint8_t value);
};

class PCA9635Output : public output::FloatOutput {
 public:
  void set_parent(PCA9635Component *parent) { parent_ = parent; }
  void set_channel(uint8_t channel) { channel_ = channel; }

 protected:
  void write_state(float state) override;

  PCA9635Component *parent_;
  uint8_t channel_;
};

}  // namespace pca9635
}  // namespace esphome
