CC = g++
 
# executables
gatorTaxi: gatorTaxi.cpp MyRBT.h MyHeap.h
	$(CC) MyHeap.h
	$(CC) MyRBT.h
	$(CC) gatorTaxi.cpp -o gatorTaxi


clean:
	$(RM) $(TARGET)