CC=gcc
CFLAGS=`xml2-config --cflags` -I. -I./utils -I./PCA9685
LIBS=`xml2-config --libs` -lwiringPi
DEPS = utils/logging.h PCA9685/PCA9685.h car.h
OBJ = utils/logging.o PCA9685/PCA9685.o car.o test.o 

# $@ matches the target; $< matches the first dependent
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(LIBS) $(CFLAGS)

test: $(OBJ)
	gcc -o $@ $^ $(LIBS) $(CFLAGS)
 
clean:
	 rm *.o test 
