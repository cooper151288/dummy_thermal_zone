obj-m += dummy_tz.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	sudo insmod ./dummy_tz.ko

unload:
	sudo rmmod dummy_tz

