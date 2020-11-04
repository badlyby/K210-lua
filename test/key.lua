function key_down()
    print "BOOT key pressed"
end

fpioa.set_function(16, fpioa.FUNC_GPIOHS0)
sw = gpio.pin(fpioa.FUNC_GPIOHS0)
sw:set_drive_mode(gpio.GPIO_DM_INPUT_PULL_UP)
sw:set_pin_edge(gpio.GPIO_PE_FALLING)
sw:set_irq(1,key_down)
