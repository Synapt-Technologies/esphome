from esphome.components import binary_sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID, CONF_NAME, CONF_DEVICE_CLASS

status_ns = cg.esphome_ns.namespace('status')
StatusBinarySensor = status_ns.class_('StatusBinarySensor', binary_sensor.BinarySensor,
                                      cg.Component)

PLATFORM_SCHEMA = cv.nameable(binary_sensor.BINARY_SENSOR_PLATFORM_SCHEMA.extend({
    cv.GenerateID(): cv.declare_variable_id(StatusBinarySensor),

    cv.Optional(CONF_DEVICE_CLASS, default="connectivity"): binary_sensor.device_class,
}).extend(cv.COMPONENT_SCHEMA))


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_NAME])
    yield cg.register_component(var, config)
    yield binary_sensor.register_binary_sensor(var, config)
