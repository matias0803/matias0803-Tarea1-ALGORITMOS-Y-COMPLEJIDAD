# Definimos el compilador
CXX = g++

# Banderas de compilación (usamos C++17 para std::filesystem y -O3 para optimizar rendimiento)
CXXFLAGS = -std=c++17 -Wall -Wextra -O3

# Lista de todos los archivos fuente (basado en la estructura del proyecto)
SRCS = sorting.cpp \
       algorithms/mergesort.cpp \
       algorithms/quicksort.cpp \
       algorithms/patiencesort.cpp \
       algorithms/sort.cpp

# Nombre del archivo ejecutable que se va a generar
TARGET = sorting.out

# Regla por defecto cuando solo escribes "make"
all: $(TARGET)

# Regla para compilar el ejecutable (¡IMPORTANTE: usar TAB aquí!)
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

# Regla para generar las matrices de prueba ejecutando el script dentro de scripts/
generate:
	cd scripts && python3 array_generator.py

# Regla para generar las visualizaciones y gráficos con Python
plot:
	cd scripts && python3 plot_generator.py

# Regla para limpiar/borrar el ejecutable compilado
clean:
	rm -f $(TARGET)

# Regla para limpiar los datos generados (matrices, métricas y gráficos)
clean-data:
	rm -f data/array_input/*.txt
	rm -f data/array_output/*.txt
	rm -f data/measurements/*.csv
	rm -f data/plots/*.png