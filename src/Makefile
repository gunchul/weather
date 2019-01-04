CFLAGS= -g -Wall -O3
SRCS=util.c
SRCS+=parser.c
SRCS+=sunrisesunset.c
SRCS+=swell.c
SRCS+=wind.c
SRCS+=tide.c
SRCS+=moon.c
SRCS+=rain.c
SRCS+=water.c
SRCS+=weather.c

OBJS=$(SRCS:.c=.o)
all:$(OBJS) weather

%.o: %.c
	gcc $(CFLAGS) -c $< 

weather:
	gcc -o weather $(OBJS)
	cp weather ../

clean:
	rm $(OBJS) weather
