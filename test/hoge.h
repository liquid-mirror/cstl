#ifndef HOGE_H
#define HOGE_H

#include <string.h>

typedef struct Hoge {
	const char *key;
	int value;
} Hoge;

#define HOGE_COMP(x, y)	 	strcmp((x).key, (y).key)
#define HOGEP_COMP(x, y)	strcmp((x)->key, (y)->key)


#endif
