CFLAGS	= `pkg-config --cflags sqlite3`

LIBS	= `pkg-config --libs sqlite3`

TARGET	= sqliteTest

OBJS	= main.cpp Database.cpp

$(TARGET):$(OBJS)
	$(CXX)  $^ -o $@ $(LIBS) $(CFLAGS)

.PHONY: clean
clean:  
	rm -f $(TARGET)
