#ifndef HOGE_H
#define HOGE_H

#include <string.h>

typedef struct Hoge {
	const char *key;
	int value;
} Hoge;

#define HOGE_COMP(x, y)	 	strcmp((x).key, (y).key)
#define HOGEP_COMP(x, y)	strcmp((x)->key, (y)->key)

size_t HOGE_HASH(Hoge x);
size_t HOGEP_HASH(Hoge *x);


#endif
