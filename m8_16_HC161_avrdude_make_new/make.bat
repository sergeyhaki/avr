path C:\AVRGCC\bin

cd/d %1

C:\AVRGCC\utils\bin\make.exe -f Makefile

;avrdude -c usbasp -p m8 -U flash:w:main.hex

pause