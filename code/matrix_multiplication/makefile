CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3

TARGET = matrix_multiplication.out
SRCS = matrix_multiplication.cpp algorithms/naive.cpp algorithms/strassen.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

generate:
	cd scripts && python3 matrix_generator.py

plot:
	cd scripts && python3 plot_generator.py


clean:
	rm -f $(TARGET)

clean-data:
	rm -f data/matrix_input/*.txt
	rm -f data/matrix_output/*.txt
	rm -f data/measurements/*.csv
	rm -f data/plots/*.png