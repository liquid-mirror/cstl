#!/bin/sh
usage()
{
	echo -e "Usage: cstlgen.sh {vector | deque | list | string} name type [algo] [include] [debug] [path]"
	echo -e "   or: cstlgen.sh {set | multiset} name type comp [include] [debug1] [path]"
	echo -e "   or: cstlgen.sh {map | multimap} name keytype valuetype comp [include] [debug1] [debug2] [path]"
}

container=$1
case $container in
"vector")
	lower="vector"
	upper="VECTOR"
	name=$2
	type=$3
	algo=$4
	include_file=$5
	debug=$6
	path=$7
	heap=$8
	;;
"deque")
	lower="deque"
	upper="DEQUE"
	name=$2
	type=$3
	algo=$4
	include_file=$5
	debug=$6
	path=$7
	heap=$8
	;;
"ring")
	lower="ring"
	upper="RING"
	name=$2
	type=$3
	algo=$4
	include_file=$5
	debug=$6
	path=$7
	heap=$8
	;;
"list")
	lower="list"
	upper="LIST"
	name=$2
	type=$3
	algo=$4
	include_file=$5
	debug=$6
	path=$7
	heap=$8
	;;
"string")
	lower="string"
	upper="STRING"
	name=$2
	type=$3
	algo=$4
	include_file=$5
	debug=$6
	path=$7
	heap=$8
	;;
"set")
	lower="set"
	upper="SET"
	name=$2
	type=$3
	comp=$4
	include_file=$5
	debug1=$6
	path=$7
	heap=$8
	;;
"multiset")
	lower="set"
	upper="MULTISET"
	name=$2
	type=$3
	comp=$4
	include_file=$5
	debug1=$6
	path=$7
	heap=$8
	;;
"map")
	lower="map"
	upper="MAP"
	name=$2
	type=$3
	value=$4
	comp=$5
	include_file=$6
	debug1=$7
	debug2=$8
	path=$9
	shift
	heap=$9
	;;
"multimap")
	lower="map"
	upper="MULTIMAP"
	name=$2
	type=$3
	value=$4
	comp=$5
	include_file=$6
	debug1=$7
	debug2=$8
	path=$9
	shift
	heap=$9
	;;
*)
	usage
	exit
	;;
esac
if [ "$type" = "" ]; then
	usage
	exit
fi
if [ "$path" = "" ]; then
	path=$name
else
	path=`echo "$path" | sed -e "s:[^/]$:&/:"`$name
fi
if [ "$algo" = "" -o "$algo" = "no" ]; then
	algo="false"
fi
if [ "$include_file" = "false" -o "$include_file" = "no" ]; then
	include_file=""
fi
if [ "$debug" = "false" -o "$debug" = "no" ]; then
	debug=""
fi


hdr="\
#include \"../cstl/${lower}.h\"
#include \"./list_debug.h\"
#include \"./deque_debug.h\"
#include \"./rbtree_debug.h\"
#undef CSTL_VECTOR_MAGIC
#undef CSTL_RING_MAGIC
#undef CSTL_DEQUE_MAGIC
#undef CSTL_LIST_MAGIC
#undef CSTL_STRING_MAGIC
"
if [ "$algo" != "false" ]; then
	hdr=${hdr}"#include \"../cstl/algorithm.h\"
	"
fi
if [ $container = "map" -o $container = "multimap" ]; then
	hdr=${hdr}"CSTL_${upper}_INTERFACE($name, $type, $value)"
else
	hdr=${hdr}"CSTL_${upper}_INTERFACE($name, $type)"
fi
if [ "$debug1" != "" ]; then
	format=`echo "$debug1" | grep '%[#+-]*[0-9]*[dioxXucsfeEgGp]'`
	if [ "$debug1" == "$format" ]; then
		rbdebug=`echo "$lower" | tr "[:lower:]" "[:upper:]"`
		hdr=${hdr}"CSTL_${rbdebug}_DEBUG_INTERFACE($name)"
	fi
fi
if [ "$debug" != "" ]; then
	format=`echo "$debug" | grep '%[#+-]*[0-9]*[dioxXucsfeEgGp]'`
	if [ "$debug" == "$format" ]; then
		hdr=${hdr}"CSTL_${upper}_DEBUG_INTERFACE($name, $type)"
	fi
fi

src="\
#include \"../cstl/${lower}.h\"
#include \"./list_debug.h\"
#include \"./deque_debug.h\"
#include \"./rbtree_debug.h\"
#undef assert
#undef CSTL_VECTOR_MAGIC
#undef CSTL_RING_MAGIC
#undef CSTL_DEQUE_MAGIC
#undef CSTL_LIST_MAGIC
#undef CSTL_STRING_MAGIC
#undef CSTL_RBTREE_MAGIC
#undef CSTL_NPOS
#undef CSTL_VECTOR_AT
#undef CSTL_VECTOR_SIZE
#undef CSTL_VECTOR_EMPTY
#undef CSTL_VECTOR_CAPACITY
#undef CSTL_VECTOR_FULL
#undef CSTL_VECTOR_CLEAR
#undef CSTL_RING_FORWARD
#undef CSTL_RING_BACKWARD
#undef CSTL_RING_NEXT
#undef CSTL_RING_PREV
#undef CSTL_RING_DISTANCE
#undef CSTL_RING_AT
#undef CSTL_RING_EMPTY
#undef CSTL_RING_MAX_SIZE
#undef CSTL_RING_FULL
#undef CSTL_RING_SIZE
#undef CSTL_RING_FRONT
#undef CSTL_RING_BACK
#undef CSTL_RING_CLEAR
#undef CSTL_DEQUE_RINGBUF_SIZE
#undef CSTL_DEQUE_INITIAL_MAP_SIZE
#undef CSTL_DEQUE_SIZE
#undef CSTL_LIST_BEGIN
#undef CSTL_LIST_END
#undef CSTL_LIST_RBEGIN
#undef CSTL_LIST_REND
#undef CSTL_LIST_NEXT
#undef CSTL_LIST_PREV
#undef CSTL_LIST_AT
#undef CSTL_RBTREE_NODE_IS_HEAD
#undef CSTL_RBTREE_NODE_IS_ROOT
#undef CSTL_RBTREE_NODE_IS_NIL
#undef CSTL_STRING_AT
"
if [ "$algo" != "false" ]; then
	src=${src}"#include \"../cstl/algorithm.h\"
	"
fi
if [ $container = "set" -o $container = "multiset" ]; then
	src=${src}"CSTL_${upper}_IMPLEMENT($name, $type, $comp)"
elif [ $container = "map" -o $container = "multimap" ]; then
	src=${src}"CSTL_${upper}_IMPLEMENT($name, $type, $value, $comp)"
else
	src=${src}"CSTL_${upper}_IMPLEMENT($name, $type)"
fi
if [ "$rbdebug" != "" ]; then
	format=`echo "$debug2" | grep '%[#+-]*[0-9]*[dioxXucsfeEgGp]'`
	if [ "$debug2" != "" -a "$debug2" == "$format" ]; then
		src=${src}"CSTL_${rbdebug}_DEBUG_IMPLEMENT($name, $type, $value, $comp, $debug1, $debug2, 1)"
	else
		src=${src}"CSTL_${rbdebug}_DEBUG_IMPLEMENT($name, $type, $comp, $debug1, 1)"
	fi
fi
if [ "$debug" != "" ]; then
	format=`echo "$debug" | grep '%[#+-]*[0-9]*[dioxXucsfeEgGp]'`
	if [ "$debug" == "$format" ]; then
		src=${src}"CSTL_${upper}_DEBUG_IMPLEMENT($name, $type, $debug)"
	fi
fi

rev=`grep '$Id' "../cstl/${lower}.h"`
vectorrev=`grep '$Id' "../cstl/vector.h"`
ringrev=`grep '$Id' "../cstl/ring.h"`
algorev=`grep '$Id' "../cstl/algorithm.h"`
rbtreerev=`grep '$Id' "../cstl/rbtree.h"`
rbdebugrev=`grep '$Id' "./rbtree_debug.h"`

# ヘッダファイル生成
included=`echo "$name""_H_INCLUDED" | tr "[:lower:]" "[:upper:]"`
echo -e "/* ${name}.h" > "$path"".h"
echo -e " * " >> "$path"".h"
echo -e " * generated by cstlgen.sh" >> "$path"".h"
echo -e "${rev}" >> "$path"".h"
if [ $lower = "vector" -a "$algo" != "false" ]; then
	echo -e "${algorev}" >> "$path"".h"
elif [ $lower = "ring" -a "$algo" != "false" ]; then
	echo -e "${algorev}" >> "$path"".h"
elif [ $lower = "deque" ]; then
	echo -e "${vectorrev}" >> "$path"".h"
	echo -e "${ringrev}" >> "$path"".h"
	if [ "$algo" != "false" ]; then
		echo -e "${algorev}" >> "$path"".h"
	fi
elif [ $lower = "string" ]; then
	echo -e "${vectorrev}" >> "$path"".h"
	if [ "$algo" != "false" ]; then
		echo -e "${algorev}" >> "$path"".h"
	fi
elif [ $lower = "set" -o $lower = "multiset" -o\
	   $lower = "map" -o $lower = "multimap" ]; then
	echo -e "${rbtreerev}" >> "$path"".h"
	if [ "$rbdebug" != "" ]; then
		echo -e "${rbdebugrev}" >> "$path"".h"
	fi
fi
echo -e " */" >> "$path"".h"
echo -e "#ifndef $included\n#define $included\n" >> "$path"".h"
echo -e "#include <stddef.h>" >> "$path"".h"
if [ "$include_file" != "" ]; then
	echo -e "#include \"$include_file\"\n" >> "$path"".h"
else
	echo -e "" >> "$path"".h"
fi
if [ $lower = "string" ]; then
	echo -e "#define CSTL_NPOS	((size_t)-1)\n" >> "$path"".h"
fi
if [ $lower = "ring" ]; then
echo -e "\
#ifndef NDEBUG
#define CSTL_${upper}_MAGIC(x) x
#else
#define CSTL_${upper}_MAGIC(x)
#endif\n" >> "$path"".h"
fi
echo "$hdr" | cpp -I.. | grep "$name" | indent -kr -ut -ts4 \
| sed -e "s/$name \* /$name */g" >> "$path"".h"
echo -e "\n#endif /* $included */" >> "$path"".h"

# ソースファイル生成
echo -e "/* ${name}.c" > "$path"".c"
echo -e " * " >> "$path"".c"
echo -e " * generated by cstlgen.sh" >> "$path"".c"
echo -e "${rev}" >> "$path"".c"
if [ $lower = "vector" -a "$algo" != "false" ]; then
	echo -e "${algorev}" >> "$path"".c"
elif [ $lower = "ring" -a "$algo" != "false" ]; then
	echo -e "${algorev}" >> "$path"".c"
elif [ $lower = "deque" ]; then
	echo -e "${vectorrev}" >> "$path"".c"
	echo -e "${ringrev}" >> "$path"".c"
	if [ "$algo" != "false" ]; then
		echo -e "${algorev}" >> "$path"".c"
	fi
elif [ $lower = "string" ]; then
	echo -e "${vectorrev}" >> "$path"".c"
	if [ "$algo" != "false" ]; then
		echo -e "${algorev}" >> "$path"".c"
	fi
elif [ $lower = "set" -o $lower = "multiset" -o\
	   $lower = "map" -o $lower = "multimap" ]; then
	echo -e "${rbtreerev}" >> "$path"".c"
	if [ "$rbdebug" != "" ]; then
		echo -e "${rbdebugrev}" >> "$path"".c"
	fi
fi
echo -e " */" >> "$path"".c"
echo -e "#include <stdlib.h>" >> "$path"".c"
if [ "$comp" = "strcmp" ]; then
	echo -e "#include <string.h>" >> "$path"".c"
fi
if [ "$rbdebug" != "" -o "$debug" != "" ]; then
	echo -e "#include <stdio.h>" >> "$path"".c"
fi
echo -e "#include <assert.h>" >> "$path"".c"
echo -e "#include \"$name.h\"\n" >> "$path"".c"
if [ $lower != "ring" ]; then
if [ $lower = "deque" ]; then
echo -e "\
#ifndef NDEBUG
#define CSTL_${upper}_MAGIC(x) x
#define CSTL_VECTOR_MAGIC(x) x
#define CSTL_RING_MAGIC(x) x
#else
#define CSTL_${upper}_MAGIC(x)
#define CSTL_VECTOR_MAGIC(x)
#define CSTL_RING_MAGIC(x)
#endif\n" >> "$path"".c"
elif [ $lower = "string" ]; then
echo -e "\
#ifndef NDEBUG
#define CSTL_${upper}_MAGIC(x) x
#define CSTL_VECTOR_MAGIC(x) x
#else
#define CSTL_${upper}_MAGIC(x)
#define CSTL_VECTOR_MAGIC(x)
#endif\n" >> "$path"".c"
elif [ $lower = "set" -o $lower = "multiset" -o\
	   $lower = "map" -o $lower = "multimap" ]; then
echo -e "\
#ifndef NDEBUG
#define CSTL_RBTREE_MAGIC(x) x
#else
#define CSTL_RBTREE_MAGIC(x)
#endif\n" >> "$path"".c"
else
echo -e "\
#ifndef NDEBUG
#define CSTL_${upper}_MAGIC(x) x
#else
#define CSTL_${upper}_MAGIC(x)
#endif\n" >> "$path"".c"
fi
fi
if [ "$heap" != "" ]; then
echo -e "\
#include \"heap.h\"
extern Heap $heap;
#define malloc(s)      Heap_alloc(&$heap, s)
#define realloc(p, s)  Heap_realloc(&$heap, p, s)
#define free(p)        Heap_free(&$heap, p)
" >> "$path"".c"
fi
if [ $lower = "vector" ]; then
echo -e "\
#define CSTL_VECTOR_AT(self, idx)	(self)->buf[(idx)]
#define CSTL_VECTOR_SIZE(self)		(self)->end
#define CSTL_VECTOR_EMPTY(self)		((self)->end == 0)
#define CSTL_VECTOR_CAPACITY(self)	(self)->nelems
#define CSTL_VECTOR_FULL(self)		(CSTL_VECTOR_SIZE((self)) == CSTL_VECTOR_CAPACITY((self)))
#define CSTL_VECTOR_CLEAR(self)		(self)->end = 0
" >> "$path"".c"
elif [ $lower = "ring" ]; then
echo -e "\
#define CSTL_RING_FORWARD(self, idx, n)			((idx) + (n) >= (self)->nelems ? (idx) + (n) - (self)->nelems : (idx) + (n))
#define CSTL_RING_BACKWARD(self, idx, n)		((idx) >= (n) ? (idx) - (n) : (self)->nelems + (idx) - (n))
#define CSTL_RING_NEXT(self, idx)				CSTL_RING_FORWARD((self), (idx), 1)
#define CSTL_RING_PREV(self, idx)				CSTL_RING_BACKWARD((self), (idx), 1)
#define CSTL_RING_DISTANCE(self, first, last)	((first) <= (last) ? (last) - (first) : (self)->nelems - (first) + (last))
#define CSTL_RING_AT(self, idx)					(self)->buf[CSTL_RING_FORWARD((self), (self)->begin, (idx))]
#define CSTL_RING_EMPTY(self)					((self)->begin == (self)->end)
#define CSTL_RING_MAX_SIZE(self)				((self)->nelems - 1)
#define CSTL_RING_FULL(self)					(CSTL_RING_NEXT((self), (self)->end) == (self)->begin)
#define CSTL_RING_SIZE(self)					CSTL_RING_DISTANCE((self), (self)->begin, (self)->end)
#define CSTL_RING_FRONT(self)					(self)->buf[(self)->begin]
#define CSTL_RING_BACK(self)					(self)->buf[CSTL_RING_PREV((self), (self)->end)]
#define CSTL_RING_CLEAR(self)					(self)->end = (self)->begin
" >> "$path"".c"
elif [ $lower = "deque" ]; then
echo -e "\
#define CSTL_VECTOR_AT(self, idx)	(self)->buf[(idx)]
#define CSTL_VECTOR_SIZE(self)		(self)->end
#define CSTL_VECTOR_EMPTY(self)		((self)->end == 0)
#define CSTL_VECTOR_CAPACITY(self)	(self)->nelems
#define CSTL_VECTOR_FULL(self)		(CSTL_VECTOR_SIZE((self)) == CSTL_VECTOR_CAPACITY((self)))
#define CSTL_VECTOR_CLEAR(self)		(self)->end = 0
#define CSTL_RING_FORWARD(self, idx, n)			((idx) + (n) >= (self)->nelems ? (idx) + (n) - (self)->nelems : (idx) + (n))
#define CSTL_RING_BACKWARD(self, idx, n)		((idx) >= (n) ? (idx) - (n) : (self)->nelems + (idx) - (n))
#define CSTL_RING_NEXT(self, idx)				CSTL_RING_FORWARD((self), (idx), 1)
#define CSTL_RING_PREV(self, idx)				CSTL_RING_BACKWARD((self), (idx), 1)
#define CSTL_RING_DISTANCE(self, first, last)	((first) <= (last) ? (last) - (first) : (self)->nelems - (first) + (last))
#define CSTL_RING_AT(self, idx)					(self)->buf[CSTL_RING_FORWARD((self), (self)->begin, (idx))]
#define CSTL_RING_EMPTY(self)					((self)->begin == (self)->end)
#define CSTL_RING_MAX_SIZE(self)				((self)->nelems - 1)
#define CSTL_RING_FULL(self)					(CSTL_RING_NEXT((self), (self)->end) == (self)->begin)
#define CSTL_RING_SIZE(self)					CSTL_RING_DISTANCE((self), (self)->begin, (self)->end)
#define CSTL_RING_FRONT(self)					(self)->buf[(self)->begin]
#define CSTL_RING_BACK(self)					(self)->buf[CSTL_RING_PREV((self), (self)->end)]
#define CSTL_RING_CLEAR(self)					(self)->end = (self)->begin
#define CSTL_DEQUE_RINGBUF_SIZE(Type)	(sizeof(Type) < 512 ? 512 / sizeof(Type) : 1)
#define CSTL_DEQUE_INITIAL_MAP_SIZE		(8)
#define CSTL_DEQUE_SIZE(self)			(self)->nelems
" >> "$path"".c"
elif [ $lower = "list" ]; then
echo -e "\
#define CSTL_LIST_BEGIN(self)	(self)->next
#define CSTL_LIST_END(self)		(self)
#define CSTL_LIST_RBEGIN(self)	(self)->prev
#define CSTL_LIST_REND(self)	(self)
#define CSTL_LIST_NEXT(pos)		(pos)->next
#define CSTL_LIST_PREV(pos)		(pos)->prev
#define CSTL_LIST_AT(pos)		(pos)->elem
" >> "$path"".c"
elif [ $lower = "string" ]; then
echo -e "\
#define CSTL_VECTOR_AT(self, idx)	(self)->buf[(idx)]
#define CSTL_VECTOR_SIZE(self)		(self)->end
#define CSTL_VECTOR_EMPTY(self)		((self)->end == 0)
#define CSTL_VECTOR_CAPACITY(self)	(self)->nelems
#define CSTL_VECTOR_FULL(self)		(CSTL_VECTOR_SIZE((self)) == CSTL_VECTOR_CAPACITY((self)))
#define CSTL_VECTOR_CLEAR(self)		(self)->end = 0
#define CSTL_STRING_AT(self, idx)	CSTL_VECTOR_AT((self)->data, (idx))
" >> "$path"".c"
elif [ $lower = "set" -o $lower = "multiset" -o\
	 $lower = "map" -o $lower = "multimap" ]; then
echo -e "\
enum {
	CSTL_RBTREE_RED,
	CSTL_RBTREE_BLACK,
	CSTL_RBTREE_HEAD
};\n" >> "$path"".c"
echo -e "\
#define CSTL_RBTREE_NODE_IS_HEAD(self)		((self)->color == CSTL_RBTREE_HEAD)
#define CSTL_RBTREE_NODE_IS_ROOT(self)		CSTL_RBTREE_NODE_IS_HEAD((self)->parent)
#define CSTL_RBTREE_NODE_IS_NIL(self, Name)	((self) == (Name##Node *) &Name##_nil)
" >> "$path"".c"
fi
echo "$src" | cpp -I.. | grep "$name" | indent -kr -ut -ts4 \
| sed -e "s/$name \* /$name */g" | sed -e 's/^} /}\
\
/' >> "$path"".c"

# コンパイル確認
gcc -Wall -ansi -pedantic-errors "$path"".c" -c -DNDEBUG
gcc -Wall -ansi -pedantic-errors "$path"".c" -c
rm "$path"".o"
