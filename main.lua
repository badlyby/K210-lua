print "Hello LUA!"
gpio.init()
fpioa.set_function(12, fpioa.FUNC_GPIO0)
fpioa.set_function(13, fpioa.FUNC_GPIO1)
fpioa.set_function(14, fpioa.FUNC_GPIO2)
led_b = gpio.pin(fpioa.FUNC_GPIO0)
led_b:set_drive_mode(gpio.GPIO_DM_OUTPUT)
led_b:set_pin(gpio.GPIO_PV_LOW)
led_g = gpio.pin(fpioa.FUNC_GPIO1)
led_g:set_drive_mode(gpio.GPIO_DM_OUTPUT)
led_g:set_pin(gpio.GPIO_PV_LOW)
led_r = gpio.pin(fpioa.FUNC_GPIO2)
led_r:set_drive_mode(gpio.GPIO_DM_OUTPUT)
led_r:set_pin(gpio.GPIO_PV_LOW)
function dual_core(arg)
    print(arg,"core",current_coreid())
end
dual_core(123)
print("run core1")
do_core1(dual_core,456)
while(core1_busy()) do end
print "core1 free"
do_core1(dual_core,789)
while(core1_busy()) do end
print "Done"

function key_down()
    print "BOOT key pressed"
end

fpioa.set_function(16, fpioa.FUNC_GPIOHS0)
sw = gpio.pin(fpioa.FUNC_GPIOHS0)
sw:set_drive_mode(gpio.GPIO_DM_INPUT_PULL_UP)
sw:set_pin_edge(gpio.GPIO_PE_FALLING)
sw:set_irq(1,key_down)

u3=uart.init(3)

function u3_callback()
	print(u3:receive_data())
end

u3:irq_register(uart.UART_RECEIVE, u3_callback, 1)

while(1) do sleep(1) end