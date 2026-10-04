KERNEL_DIR := /lib/modules/6.8.0-146-generic/build

obj-m += process_monitor_driver.o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
