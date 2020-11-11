fpioa.set_function(8, fpioa.FUNC_I2C0_SCLK)
fpioa.set_function(9, fpioa.FUNC_I2C0_SDA)
i2 = i2c.init(0, 100000, 16, 7)

function reset()
    i2:send("\x00\x02")
end
function init()
    bootrom = "\xc1\x03\x00\x00\x0a\x00\x88\x0f\x00\x00\x42\x02"
    i2:send(bootrom)
end

function tune(freq)
    fq = math.floor((freq-87.0)*10.0+0.5)
	tc = "\xc0\x01"..string.char((fq>>2)&0xFF)..string.char(((fq&0x03)<<6)&0xFF|0x10)
	i2:send(tc)
end

reset()
init()
tune(92.5)