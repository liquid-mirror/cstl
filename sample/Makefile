CFLAGS = -Wall -I.. -DNDEBUG -O2

all: bm

clean:
	rm -f *.o *.exe

bm: bm_vector\
	bm_deque\
	bm_list\
	bm_set\
	bm_map\
	$(NULL)
	

bm_vector: benchmark_vector.cpp
	$(CXX) $(CFLAGS) $< -o $@.exe

bm_deque: benchmark_deque.cpp
	$(CXX) $(CFLAGS) $< -o $@.exe

bm_list: benchmark_list.cpp
	$(CXX) $(CFLAGS) $< -o $@.exe

bm_set: benchmark_set.cpp
	$(CXX) $(CFLAGS) $< -o $@.exe

bm_map: benchmark_map.cpp
	$(CXX) $(CFLAGS) $< -o $@.exe

