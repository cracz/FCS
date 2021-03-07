#IDIR= include directory
CC=g++
CFLAGS=`root-config --cflags --libs`
#DEPS= .h files
#OBJ= analyzeLED.o

#%.o: %.cxx $(DEPS)
#	$(CC) $(CFLAGS) $< -c -o $@ 

analyzeLED: analyzeLED.cxx #$(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ 

.PHONY: clean

clean:
	rm -f *.o *~ core
