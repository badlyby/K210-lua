lcd.init()
lcd.clear(lcd.rgb565(255,255,255))
logo = image.loadbmp("1:/lua-logo.bmp")
lcd.display(logo)
lcd.draw_string(10,128,"Hello world!",lcd.rgb565(0,0,0))
lcd.fill_rectangle(150,50,200,100,lcd.rgb565(255,0,0))
lcd.fill_rectangle(200,50,250,100,lcd.rgb565(0,255,0))
lcd.fill_rectangle(250,50,300,100,lcd.rgb565(0,0,255))