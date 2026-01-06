CC = g++
CFLAGS = -std=c++17 -O2 -Wall -Wextra

TARGET = gatorTaxi

$(TARGET): gatorTaxi.cpp
	$(CC) $(CFLAGS) gatorTaxi.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
