import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_CHANNEL, CONF_ID
from . import PCA9635Component, pca9635_ns

PCA9635Output = pca9635_ns.class_('PCA9635Output', output.FloatOutput)

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend({
    cv.GenerateID(CONF_ID): cv.declare_id(PCA9635Output),
    cv.GenerateID('pca9635_id'): cv.use_id(PCA9635Component),
    cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=15),
})

async def to_code(config):
    parent = await cg.get_variable(config['pca9635_id'])
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_parent(parent))
    await output.register_output(var, config)