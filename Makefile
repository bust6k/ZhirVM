all:
	gcc -m32 -fno-stack-protector -ffreestanding -O0 -fPIC -fno-common -w -c lexer.c -o ../build/module.o ; ld -m elf_i386 -T ../module.ld ../build/module.o -o ../build/module.elf ; objcopy --set-section-flags .bss=alloc,load,contents -O binary ../build/module.elf ../ramdisk/za_russia.mod
	cd ..
	sh  build32.sh 
	qemu-system-x86_64 -hda test.img -m 3064 -full-screen  
