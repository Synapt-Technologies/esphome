#pragma once

#include "esphome/core/component.h"
#include "esphome/components/stepper/stepper.h"
#include "esphome/components/uart/uart.h"

#include <TMCStepper.h>

namespace esphome {
namespace tmc {

class TMC2209 : public stepper::Stepper, public Component, public uart::UARTDevice {
 public:
  TMC2209(GPIOPin *step_pin, GPIOPin *dir_pin, bool reverse_direction, *float r_shunt, *uint8_t tmc_address)
      : step_pin_(step_pin), dir_pin_(dir_pin), reverse_direction_(reverse_direction), r_shunt_(r_shunt), tmc_address_(tmc_address) {}

  void set_sleep_pin(GPIOPin *sleep_pin) { this->sleep_pin_ = sleep_pin; }
  void setup() override;
  void dump_config() override;
  void loop() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  const TMC2209Stepper &get_driver() const { return *this->stepper_driver_; }

 protected:
  GPIOPin *step_pin_;
  GPIOPin *dir_pin_;
  bool reverse_direction_;
  TMC2209Stepper *stepper_driver_;
  GPIOPin *sleep_pin_{nullptr};
  bool sleep_pin_state_;
  HighFrequencyLoopRequester high_freq_;
  double *r_shunt_;
  uint8_t *tmc_address_;
};

template<typename... Ts> class TMC2209SetupAction : public Action<Ts...>, public Parented<TMC2209> {
 public:
  TEMPLATABLE_VALUE(int, microsteps)
  TEMPLATABLE_VALUE(int, tcool_threshold)
  TEMPLATABLE_VALUE(int, stall_threshold)
  TEMPLATABLE_VALUE(float, current)
  TEMPLATABLE_VALUE(float, r_shunt)
  TEMPLATABLE_VALUE(int, tmc_address)

  void play(Ts... x) override {
    auto driver = this->parent_->get_driver();
    if (this->microsteps_.has_value()) {
      ESP_LOGW("tmc2209", "microsteps %d", this->microsteps_.value(x...));
      driver.microsteps(this->microsteps_.value(x...));
    }
    if (this->tcool_threshold_.has_value()) {
      ESP_LOGW("tmc2209", "tcool_threshold %d", this->tcool_threshold_.value(x...));
      driver.TCOOLTHRS(this->tcool_threshold_.value(x...));
    }
    if (this->stall_threshold_.has_value()) {
      ESP_LOGW("tmc2209", "stall %d", this->stall_threshold_.value(x...));
      driver.SGTHRS(this->stall_threshold_.value(x...));
    }
    if (this->current_.has_value()) {
      ESP_LOGW("tmc2209", "current %.3f", this->current_.value(x...));
      driver.rms_current(static_cast<int>(this->current_.value(x...) * 1000.0));
    }
    if (this->tmc_address_.has_value()) {
      ESP_LOGW("tmc2209", "tmc_address %.3f", this->tmc_address_.value(x...));
    }
    if (this->r_shunt_.has_value()) {
      ESP_LOGW("tmc2209", "r_shunt_ %.3f", this->r_shunt_.value(x...));
    }
  }
};

}  // namespace tmc
}  // namespace esphome