SHELL=cmd.exe

# ������ ��� �������, � ���������� ��������� ����� main.hex test.bin
TARG=main
 
CC = avr-gcc
OBJCOPY = avr-objcopy

# ������ �� ����� ������ �������� ������, ����� ������� ��������� ������
SRCS= main.c 
 
OBJS = $(SRCS:.c=.o)
 
# ������ ��� ������ ���������������� ����� ������������� (atmega8)
MCU=atmega88
 
# ����� �����������, ��� ������ F_CPU ���������� ������� �� ������� ����� �������� ����������,
CFLAGS = -mmcu=$(MCU)  -Wall -g -Os -Werror -lm -std=gnu99 
LDFLAGS = -mmcu=$(MCU)  -Wall -g -Os  -Werror 
 
all: $(TARG)
 
$(TARG): $(OBJS)
	$(CC) $(LDFLAGS) -o $@.elf  $(OBJS) -lm
	$(OBJCOPY) -O binary -R .eeprom -R .nwram  $@.elf $@.bin
	$(OBJCOPY) -O ihex -R .eeprom -R .nwram  $@.elf $@.hex
 
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
 
clean:
	rm -f *.elf *.bin *.hex  $(OBJS) *.map