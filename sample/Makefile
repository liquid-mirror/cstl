CFLAGS = -Wall -I.. -DNDEBUG -O2

all: bm

clean:
	rm -f *.o *.exe

bm: bm_vector\
	bm_deque\
	bm_list\
	bm_set\
	bm_map\
	bm_uset\
	bm_umap\
	$(NULL)
	

bm_vector: benchmark_vector.cpp ../cstl/vector.h ../cstl/algorithm.h
	$(CXX) $(CFLAGS) $< -o $@.exe

bm_deque: benchmark_deque.cpp ../cstl/deque.h ../cstl/vector.h ../cstl/ring.h ../cstl/algorithm.h
	$(CXX) $(CFLAGS) $< -o $@.exe

bm_list: benchmark_list.cpp ../cstl/list.h
	$(CXX) $(CFLAGS) $< -o $@.exe

bm_set: benchmark_set.cpp ../cstl/set.h ../cstl/rbtree.h
	$(CXX) $(CFLAGS) $< -o $@.exe

bm_map: benchmark_map.cpp ../cstl/map.h ../cstl/rbtree.h
	$(CXX) $(CFLAGS) $< -o $@.exe

bm_uset: benchmark_set.cpp ../cstl/unordered_set.h ../cstl/hashtable.h
	$(CXX) $(CFLAGS) -DUNORDERED $< -o $@.exe

bm_umap: benchmark_map.cpp ../cstl/unordered_map.h ../cstl/hashtable.h
	$(CXX) $(CFLAGS) -DUNORDERED $< -o $@.exe

