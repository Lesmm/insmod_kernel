
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






	cd user
	gcc user.c
	gcc user2.c -o a2.out
	
	./a.out
	./a2.out

	重新 make && insmod 后，Ctrl + C 一下.out程序，以免出现 rmmod: ERROR: Module netl is in use 