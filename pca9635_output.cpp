#include "pca9635_output.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pca9635 {

static const char *const TAG = "pca9635";

static const uint8_t PCA9635_MODE1 = 0x00;
static const uint8_t PCA9635_PWM0 = 0x02;
static const uint8_t PCA9635_LEDOUT0 = 0x14;

void PCA9635Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up PCA9635...");
  
  // Clear SLEEP bit (wake up)
  if (!this->write_byte(PCA9635_MODE1, 0x00)) {
    this->mark_failed();
    return;
  }
  delay(1); 

  // Enable individual PWM control (AA = 10101010 binary)
  this->write_byte(PCA9635_LEDOUT0, 0xAA);
  this->write_byte(PCA9635_LEDOUT0 + 1, 0xAA);
  this->write_byte(PCA9635_LEDOUT0 + 2, 0xAA);
  this->write_byte(PCA9635_LEDOUT0 + 3, 0xAA);

  ESP_LOGD(TAG, "PCA9635 Setup complete.");
}

void PCA9635Component::dump_config() {
  ESP_LOGCONFIG(TAG, "PCA9635:");
  LOG_I2C_DEVICE(this);
}

void PCA9635Component::set_channel_value(uint8_t channel, float state) {
  if (channel > 15) return;
  uint8_t pwm_value = (uint8_t)(state * 255.0f);
  this->write_byte(PCA9635_PWM0 + channel, pwm_value);
}

void PCA9635Output::write_state(float state) {
  if (this->parent_ != nullptr) {
    this->parent_->set_channel_value(this->channel_, state);
  }
}

}  // namespace pca9635
}  // namespace esphome
