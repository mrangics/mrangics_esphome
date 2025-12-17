#include "pca9635_output.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pca9635 {

static const char *const TAG = "pca9635";

// Register Addresses
static const uint8_t PCA9635_MODE1 = 0x00;
static const uint8_t PCA9635_MODE2 = 0x01;
static const uint8_t PCA9635_PWM0 = 0x02;
static const uint8_t PCA9635_LEDOUT0 = 0x14;

// Mode 1 Bits
static const uint8_t MODE1_SLEEP = 0x10; // Bit 4
static const uint8_t MODE1_AI = 0x80;    // Auto-Increment

void PCA9635Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up PCA9635...");

  // 1. Wake up the device (Clear SLEEP bit)
  // We write 0x00 to MODE1 to zero out SLEEP and ensure normal operation.
  // Note: Depending on specific needs, you might want to enable Auto-Increment here.
  if (!this->write_byte(PCA9635_MODE1, 0x00)) {
    this->mark_failed();
    return;
  }
  
  // Wait for oscillator to stabilize (datasheet says >500us)
  delay(1); 

  // 2. Configure LEDOUT registers to enable individual PWM control (Mode: 10 binary)
  // There are 4 LEDOUT registers (0-3). Each controls 4 LEDs.
  // We want to set every pair of bits to 10 (binary) -> 0xAA (hex) for all registers.
  // LEDOUT0 controls led 0-3, LEDOUT1 led 4-7, etc.
  
  // Using write_bytes or individual write_byte calls. 
  // Since Auto-Increment is not guaranteed set yet without specific config, we write individually for safety.
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

  // Convert 0.0-1.0 to 0-255
  uint8_t pwm_value = (uint8_t)(state * 255.0f);
  
  // Register is 0x02 (PWM0) + channel index
  uint8_t reg = PCA9635_PWM0 + channel;

  this->write_byte(reg, pwm_value);
}

void PCA9635Output::write_state(float state) {
  if (this->parent_ != nullptr) {
    this->parent_->set_channel_value(this->channel_, state);
  }
}

}  // namespace pca9635
}  // namespace esphome
