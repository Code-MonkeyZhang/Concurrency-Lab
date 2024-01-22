TARGET = pointer
OBJS += pointer.o
OBJS += test.o
LIBS += -lm

CC = gcc
CFLAGS += -MMD -MP # dependency tracking flags
CFLAGS += -I./
CFLAGS += -std=gnu99 -g -Wall -Werror -Wconversion -Wno-unused-variable
LDFLAGS += $(LIBS)

all: CFLAGS += -O2 # release flags
all: $(TARGET)

release: clean all

debug: CFLAGS += -O0 # debug flags
debug: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

DEPS = $(OBJS:%.o=%.d)
-include $(DEPS)

clean:
	-@rm $(TARGET) $(OBJS) $(DEPS) 2> /dev/null || true
	-@rm -r sandbox 2> /dev/null || true

test:
	@chmod +x grade.py
	@sed -i -e 's/\r$$//g' grade.py # dos to unix
	@sed -i -e 's/\r/\n/g' grade.py # mac to unix
	@./grade.py
