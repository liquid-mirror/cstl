#!/bin/sh
name=$1
keytype=$2
valuetype=$3
comp=$4
container=$5
if [ "$6" = "" ]; then
	path=$name
else
	path=`echo "$5" | sed -e "s:[^/]$:&/:"`$name
fi

lower="map"
if [ "$container" = "multi" ]; then
	upper="MULTIMAP"
else
	upper="MAP"
fi

hdr="\
#include <cstl/${lower}.h>
CSTL_${upper}_INTERFACE($name, $keytype, $valuetype)"

src="\
#include <cstl/${lower}.h>
#undef assert
#define assert(x) assert(x)
#undef CSTL_RBTREE_MAGIC
#define CSTL_RBTREE_MAGIC(x) CSTL_RBTREE_MAGIC(x)
CSTL_${upper}_IMPLEMENT($name, $keytype, $valuetype, $comp)"

# ヘッダファイル生成
included=`echo "$name""_H_INCLUDED" | tr "[:lower:]" "[:upper:]"`
echo -e "#ifndef $included\n#define $included\n" > "$path"".h"
echo -e "#include <stddef.h>\n" >> "$path"".h"
echo "$hdr" | cpp -I.. | grep "$name" | indent -kr -ut -ts4 >> "$path"".h"
echo "#endif /* $included */" >> "$path"".h"

# ソースファイル生成
echo -e "#include <stdlib.h>" > "$path"".c"
if [ $comp = "strcmp" ]; then
	echo -e "#include <string.h>" >> "$path"".c"
fi
echo -e "#include <assert.h>" >> "$path"".c"
echo -e "#include \"$name.h\"\n" >> "$path"".c"
echo -e "\
#ifndef NDEBUG
#define CSTL_RBTREE_MAGIC(x) x
#else
#define CSTL_RBTREE_MAGIC(x)
#endif\n" >> "$path"".c"
echo -e "\
enum {
	CSTL_RBTREE_RED,
	CSTL_RBTREE_BLACK,
	CSTL_RBTREE_HEAD
};\n" >> "$path"".c"
echo "$src" | cpp -I.. | grep "$name" | indent -kr -ut -ts4 >> "$path"".c"

gcc -Wall "$path"".c" -c

