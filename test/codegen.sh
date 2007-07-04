#!/bin/sh
name=$1
type=$2
container=$3
if [ "$4" = "" ]; then
	path=$name
else
	path=`echo "$4" | sed -e "s:[^/]$:&/:"`$name
fi

lower=`echo $container | tr "[:upper:]" "[:lower:]"`
upper=`echo $container | tr "[:lower:]" "[:upper:]"`


hdr="\
#include <cstl/${lower}.h>
#undef CSTL_VECTOR_MAGIC
#define CSTL_VECTOR_MAGIC(x) CSTL_VECTOR_MAGIC(x)
#undef CSTL_RING_MAGIC
#define CSTL_RING_MAGIC(x) CSTL_RING_MAGIC(x)
#undef CSTL_DEQUE_MAGIC
#define CSTL_DEQUE_MAGIC(x) CSTL_DEQUE_MAGIC(x)
#undef CSTL_LIST_MAGIC
#define CSTL_LIST_MAGIC(x) CSTL_LIST_MAGIC(x)
#undef CSTL_STRING_MAGIC
#define CSTL_STRING_MAGIC(x) CSTL_STRING_MAGIC(x)
CSTL_${upper}_INTERFACE($name, $type)"

src="\
#include <cstl/${lower}.h>
#include <cstl/algorithm.h>
#undef assert
#define assert(x) assert(x)
#undef CSTL_VECTOR_MAGIC
#define CSTL_VECTOR_MAGIC(x) CSTL_VECTOR_MAGIC(x)
#undef CSTL_RING_MAGIC
#define CSTL_RING_MAGIC(x) CSTL_RING_MAGIC(x)
#undef CSTL_DEQUE_MAGIC
#define CSTL_DEQUE_MAGIC(x) CSTL_DEQUE_MAGIC(x)
#undef CSTL_LIST_MAGIC
#define CSTL_LIST_MAGIC(x) CSTL_LIST_MAGIC(x)
#undef CSTL_STRING_MAGIC
#define CSTL_STRING_MAGIC(x) CSTL_STRING_MAGIC(x)
CSTL_${upper}_IMPLEMENT($name, $type)"

# ヘッダファイル生成
included=`echo "$name""_H_INCLUDED" | tr "[:lower:]" "[:upper:]"`
echo -e "#ifndef $included\n#define $included\n" > "$path"".h"
echo -e "#include <stddef.h>\n" >> "$path"".h"
if [ $lower = "ring" ]; then
echo -e "\
#ifndef NDEBUG
#define CSTL_${upper}_MAGIC(x) x
#else
#define CSTL_${upper}_MAGIC(x)
#endif\n" >> "$path"".h"
fi
echo "$hdr" | cpp -I.. | grep "$name" | indent -kr -ut -ts4 >> "$path"".h"
echo "#endif /* $included */" >> "$path"".h"

# ソースファイル生成
echo -e "#include <stdlib.h>" > "$path"".c"
echo -e "#include <assert.h>" >> "$path"".c"
echo -e "#include \"$name.h\"\n" >> "$path"".c"
if [ $lower != "ring" ]; then
echo -e "\
#ifndef NDEBUG
#define CSTL_${upper}_MAGIC(x) x
#else
#define CSTL_${upper}_MAGIC(x)
#endif\n" >> "$path"".c"
fi
if [ $lower = "deque" ]; then
echo -e "\
#ifndef NDEBUG
#define CSTL_VECTOR_MAGIC(x) x
#define CSTL_RING_MAGIC(x) x
#else
#define CSTL_VECTOR_MAGIC(x)
#define CSTL_RING_MAGIC(x)
#endif\n" >> "$path"".c"
fi
if [ $lower = "string" ]; then
echo -e "\
#ifndef NDEBUG
#define CSTL_VECTOR_MAGIC(x) x
#else
#define CSTL_VECTOR_MAGIC(x)
#endif\n" >> "$path"".c"
fi
echo "$src" | cpp -I.. | grep "$name" | indent -kr -ut -ts4 >> "$path"".c"

gcc -Wall "$path"".c" -c

