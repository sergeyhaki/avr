path D:\WinAVR\bin

cd/d %1

del main.elf main.hex main.o

avr-gcc  -gdwarf-2 -Wall -g -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums  -mmcu=atmega88 -lprintf_min -lm -DF_CPU=20000000 -std=gnu99  -Wl,-u,vfprintf -lprintf_flt -lm  -c ../main.c 
  
avr-gcc -g  -Wl,-u,vfprintf -lprintf_flt -lm  -mmcu=atmega88 -o main.elf main.o 

avr-objcopy -j .text -j .data -O ihex main.elf main.hex

avr-objdump -D main.elf > main.lst 

avr-size -C --mcu=atmega88  main.elf > size.txt

type size.txt

pause