spi3 = spi.init(3, 0, 0, spi.SPI_FF_STANDARD, 8, 6000000)
read_id = "\x90\x00\x00\x00"
data = spi3:read(2,read_id)
print(string.format("manuf_id %02X, device_id %02X",data:byte(1), data:byte(2)))
