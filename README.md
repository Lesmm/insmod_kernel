
In Ubuntu 18.04

	dmesg -w


https://blog.csdn.net/zhao_h/article/details/80943226

	cd insmod
	make

	// 加载
	sudo insmod netl.ko

	// 查看
	lsmod

	// 移除
	sudo rmmod netl




	gcc user.c -o user.o
	gcc user2.c -o user2.o
	
	./user.o
	./user2.o

	重新 make && insmod 后，Ctrl + C 一下.out程序，以免出现 rmmod: ERROR: Module netl is in use 


	/home/ubuntu/Disk16.04/home/ubuntu/Android/Sdk/platform-tools/adb reboot bootloader
	/home/ubuntu/Disk16.04/home/ubuntu/Android/Sdk/platform-tools/fastboot boot ${OUT}/boot.img