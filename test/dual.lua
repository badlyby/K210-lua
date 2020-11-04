function dual_core(arg)
    print(arg,"core",current_coreid())
end
dual_core("Hello")
print("run core1")
do_core1(dual_core,"Hello")
while(core1_busy()) do end
print "core1 free"