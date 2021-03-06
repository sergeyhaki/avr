path C:\AVRGCC\bin

cd/d %1

del main.elf main.hex main.o

avr-gcc  -gdwarf-2 -Wall -g -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums  -mmcu=atmega8 -lprintf_min -lm -std=gnu99  -Wl,-u,vfprintf -lprintf_flt -lm  -c ../main.c 
  
avr-gcc -g  -Wl,-u,vfprintf -lprintf_flt -lm  -mmcu=atmega8 -o main.elf main.o 

avr-objcopy -j .text -j .data -O ihex main.elf main.hex

avr-objdump -D main.elf > main.lst 

avr-size -C --mcu=atmega8  main.elf > size.txt

type size.txt

pause