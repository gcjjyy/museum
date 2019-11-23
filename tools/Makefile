TARGET_IMG2H = img2h
TARGET_BIN2H = bin2h

all: $(TARGET_IMG2H) $(TARGET_BIN2H)

img2h.cpp:
	g++ -o $(TARGET_IMG2H) img2h.cpp

bin2h.cpp:
	g++ -o $(TARGET_IMG2H) bin2h.cpp

$(TARGET_IMG2H): img2h.cpp

$(TARGET_BIN2H): bin2h.cpp

clean:
	rm ./$(TARGET_IMG2H)
	rm ./$(TARGET_BIN2H)

