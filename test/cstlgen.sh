#!/bin/sh
usage()
{
	echo "Usage: cstlgen.sh {vector | deque | list | string} name type [algo] [include] [debug] [path]"
	echo "   or: cstlgen.sh {set | multiset} name type comp [include] [debug1] [path]"
	echo "   or: cstlgen.sh {map | multimap} name keytype valuetype comp [include] [debug1] [debug2] [path]"
	echo "   or: cstlgen.sh {unordered_set | unordered_multiset} name type hash comp [include] [debug1] [path]"
	echo "   or: cstlgen.sh {unordered_map | unordered_multimap} name keytype valuetype hash comp [include] [debug1] [debug2] [path]"
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
	alloc=$8
	nocompile=$9
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
	alloc=$8
	nocompile=$9
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
	alloc=$8
	nocompile=$9
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
	alloc=$8
	nocompile=$9
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
	alloc=$8
	nocompile=$9
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
	alloc=$8
	nocompile=$9
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
	alloc=$8
	nocompile=$9
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
	alloc=$9
	shift
	nocompile=$9
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
	alloc=$9
	shift
	nocompile=$9
	;;
"unordered_set")
	lower="unordered_set"
	upper="UNORDERED_SET"
	name=$2
	type=$3
	hash=$4
	comp=$5
	include_file=$6
	debug1=$7
	path=$8
	alloc=$9
	shift
	nocompile=$9
	;;
"unordered_multiset")
	lower="unordered_set"
	upper="UNORDERED_MULTISET"
	name=$2
	type=$3
	hash=$4
	comp=$5
	include_file=$6
	debug1=$7
	path=$8
	alloc=$9
	shift
	nocompile=$9
	;;
"unordered_map")
	lower="unordered_map"
	upper="UNORDERED_MAP"
	name=$2
	type=$3
	value=$4
	hash=$5
	comp=$6
	include_file=$7
	debug1=$8
	debug2=$9
	shift
	path=$9
	shift
	alloc=$9
	shift
	nocompile=$9
	;;
"unordered_multimap")
	lower="unordered_map"
	upper="UNORDERED_MULTIMAP"
	name=$2
	type=$3
	value=$4
	hash=$5
	comp=$6
	include_file=$7
	debug1=$8
	debug2=$9
	shift
	path=$9
	shift
	alloc=$9
	shift
	nocompile=$9
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
if [ "$debug1" = "false" -o "$debug1" = "no" ]; then
	debug1=""
fi

if [ "$debug" != "" ]; then
	list_debug="#include \"./list_debug.h\""
	deque_debug="#include \"./deque_debug.h\""
fi
if [ "$debug1" != "" ]; then
	rbtree_debug="#include \"./rbtree_debug.h\""
	hashtable_debug="#include \"./hashtable_debug.h\""
fi

hdr="\
#include \"../cstl/${lower}.h\"
${list_debug}
${deque_debug}
${rbtree_debug}
${hashtable_debug}
#undef CSTL_MAGIC
"
if [ "$algo" != "false" ]; then
	hdr=${hdr}"#include \"../cstl/algorithm.h\"
	"
fi
if [ $container = "map" -o $container = "multimap" -o $container = "unordered_map" -o $container = "unordered_multimap" ]; then
	hdr=${hdr}"CSTL_${upper}_INTERFACE($name, $type, $value)"
else
	hdr=${hdr}"CSTL_${upper}_INTERFACE($name, $type)"
fi
if [ "$debug1" != "" ]; then
	format=`echo "$debug1" | grep '%[#+-]*[0-9]*[dioxXucsfeEgGp]'`
	if [ "$debug1" = "$format" ]; then
		if [ $container = "set" -o $container = "multiset" -o $container = "map" -o $container = "multimap" ]; then
			rbdebug=`echo "$lower" | tr "[:lower:]" "[:upper:]"`
			hdr=${hdr}"CSTL_${rbdebug}_DEBUG_INTERFACE($name)"
		else
			hashdebug=`echo "$lower" | tr "[:lower:]" "[:upper:]"`
			hdr=${hdr}"CSTL_${hashdebug}_DEBUG_INTERFACE($name)"
		fi
	fi
fi
if [ "$debug" != "" ]; then
	format=`echo "$debug" | grep '%[#+-]*[0-9]*[dioxXucsfeEgGp]'`
	if [ "$debug" = "$format" ]; then
		hdr=${hdr}"CSTL_${upper}_DEBUG_INTERFACE($name, $type)"
	fi
fi

src="\
#include \"../cstl/${lower}.h\"
${list_debug}
${deque_debug}
${rbtree_debug}
${hashtable_debug}
"

tmp=`grep -h '#define CSTL_' ../cstl/*.h | \
	grep -v 'CSTL_.*\(INCLUDED\|EXTERN_C\|INTERFACE\|IMPLEMENT.*\|LESS\|GREATER\|EQUAL_TO\)' | \
	sort | sed -e "s/#define \(CSTL_[^ \t(]*\).*/\1/" | uniq`
for i in ${tmp}; do
	src=${src}"#undef ${i}
	"
done

if [ "$algo" != "false" ]; then
	src=${src}"#include \"../cstl/algorithm.h\"
	"
fi
if [ $container = "set" -o $container = "multiset" ]; then
	src=${src}"CSTL_${upper}_IMPLEMENT($name, $type, $comp)"
elif [ $container = "map" -o $container = "multimap" ]; then
	src=${src}"CSTL_${upper}_IMPLEMENT($name, $type, $value, $comp)"
elif [ $container = "unordered_set" -o $container = "unordered_multiset" ]; then
	src=${src}"CSTL_${upper}_IMPLEMENT($name, $type, $hash, $comp)"
elif [ $container = "unordered_map" -o $container = "unordered_multimap" ]; then
	src=${src}"CSTL_${upper}_IMPLEMENT($name, $type, $value, $hash, $comp)"
else
	src=${src}"CSTL_${upper}_IMPLEMENT($name, $type)"
fi
if [ "$rbdebug" != "" ]; then
	format=`echo "$debug2" | grep '%[#+-]*[0-9]*[dioxXucsfeEgGp]'`
	if [ "$debug2" != "" -a "$debug2" = "$format" ]; then
		src=${src}"CSTL_${rbdebug}_DEBUG_IMPLEMENT($name, $type, $value, $comp, $debug1, $debug2, 1)"
	else
		src=${src}"CSTL_${rbdebug}_DEBUG_IMPLEMENT($name, $type, $comp, $debug1, 1)"
	fi
fi
if [ "$hashdebug" != "" ]; then
	format=`echo "$debug2" | grep '%[#+-]*[0-9]*[dioxXucsfeEgGp]'`
	if [ "$debug2" != "" -a "$debug2" = "$format" ]; then
		src=${src}"CSTL_${hashdebug}_DEBUG_IMPLEMENT($name, $type, $value, $hash, $comp, $debug1, $debug2)"
	else
		src=${src}"CSTL_${hashdebug}_DEBUG_IMPLEMENT($name, $type, $hash, $comp, $debug1)"
	fi
fi
if [ "$debug" != "" ]; then
	format=`echo "$debug" | grep '%[#+-]*[0-9]*[dioxXucsfeEgGp]'`
	if [ "$debug" = "$format" ]; then
		src=${src}"CSTL_${upper}_DEBUG_IMPLEMENT($name, $type, $debug)"
	fi
fi

rev=`grep '$Id' "../cstl/${lower}.h" | sed -e "s/\r//"`
vectorrev=`grep '$Id' "../cstl/vector.h" | sed -e "s/\r//"`
ringrev=`grep '$Id' "../cstl/ring.h" | sed -e "s/\r//"`
algorev=`grep '$Id' "../cstl/algorithm.h" | sed -e "s/\r//"`
rbtreerev=`grep '$Id' "../cstl/rbtree.h" | sed -e "s/\r//"`
hashtablerev=`grep '$Id' "../cstl/hashtable.h" | sed -e "s/\r//"`
if [ "$debug1" != "" ]; then
	rbdebugrev=`grep '$Id' "./rbtree_debug.h" | sed -e "s/\r//"`
	hashdebugrev=`grep '$Id' "./hashtable_debug.h" | sed -e "s/\r//"`
fi

# ヘッダファイル生成
included=`echo "$name""_H_INCLUDED" | tr "[:lower:]" "[:upper:]"`
echo "/* " > "$path"".h"
echo " * ${name}.h" >> "$path"".h"
echo " * " >> "$path"".h"
echo " * CSTL Project: http://sourceforge.jp/projects/cstl/" >> "$path"".h"
echo " * This file is generated by cstlgen.sh" >> "$path"".h"
echo " * " >> "$path"".h"
echo "${rev}" >> "$path"".h"
if [ $lower = "vector" -a "$algo" != "false" ]; then
	echo "${algorev}" >> "$path"".h"
elif [ $lower = "ring" -a "$algo" != "false" ]; then
	echo "${algorev}" >> "$path"".h"
elif [ $lower = "deque" ]; then
	echo "${vectorrev}" >> "$path"".h"
	echo "${ringrev}" >> "$path"".h"
	if [ "$algo" != "false" ]; then
		echo "${algorev}" >> "$path"".h"
	fi
elif [ $lower = "string" ]; then
	echo "${vectorrev}" >> "$path"".h"
	if [ "$algo" != "false" ]; then
		echo "${algorev}" >> "$path"".h"
	fi
elif [ $lower = "set" -o $lower = "multiset" -o\
	   $lower = "map" -o $lower = "multimap" ]; then
	echo "${rbtreerev}" >> "$path"".h"
	if [ "$rbdebug" != "" ]; then
		echo "${rbdebugrev}" >> "$path"".h"
	fi
elif [ $lower = "unordered_set" -o $lower = "unordered_multiset" -o\
	   $lower = "unordered_map" -o $lower = "unordered_multimap" ]; then
	echo "${hashtablerev}" >> "$path"".h"
	if [ "$hashdebug" != "" ]; then
		echo "${hashdebugrev}" >> "$path"".h"
	fi
fi
echo " */" >> "$path"".h"
echo "#ifndef $included
#define $included
" >> "$path"".h"
echo "#include <stddef.h>" >> "$path"".h"
if [ "$include_file" != "" ]; then
	echo "#include \"$include_file\"
" >> "$path"".h"
else
	echo "" >> "$path"".h"
fi
if [ $lower = "string" ]; then
	echo "#ifndef CSTL_NPOS" >> "$path"".h"
	grep '#define CSTL_NPOS' "../cstl/string.h" | sed -e "s/\r//" >> "$path"".h"
	echo "#endif" >> "$path"".h"
	echo "" >> "$path"".h"
fi
if [ $lower = "ring" ]; then
echo "\
#if !defined(NDEBUG) && defined(CSTL_DEBUG)
#define CSTL_MAGIC(x)	x
#else
#define CSTL_MAGIC(x)
#endif
" >> "$path"".h"
fi
echo "\
#ifdef __cplusplus
extern \"C\" {
#endif" >> "$path"".h"
echo "$hdr" | cpp -CC -I.. | grep "$name" \
| sed -e 's:/\*\([ !]\):\
/\*\1:g' \
| sed -e 's/\t* \* /\
 \* /g' \
| sed -e 's:\*/:\*/\
:g' \
| indent -kr -ut -ts4 -fc1 \
| sed -e "s/$name \* /$name */g" \
| sed -e 's:^\( \*.*\) \*/:\1\
 \*/:' \
>> "$path"".h"
echo "\
#ifdef __cplusplus
}
#endif" >> "$path"".h"
if [ $lower = "ring" ]; then
echo "
#undef CSTL_MAGIC" >> "$path"".h"
fi
echo "
#endif /* $included */" >> "$path"".h"

# ソースファイル生成
echo "/* " > "$path"".c"
echo " * ${name}.c" >> "$path"".c"
echo " * " >> "$path"".c"
echo " * CSTL Project: http://sourceforge.jp/projects/cstl/" >> "$path"".c"
echo " * This file is generated by cstlgen.sh" >> "$path"".c"
echo " * " >> "$path"".c"
echo "${rev}" >> "$path"".c"
if [ $lower = "vector" -a "$algo" != "false" ]; then
	echo "${algorev}" >> "$path"".c"
elif [ $lower = "ring" -a "$algo" != "false" ]; then
	echo "${algorev}" >> "$path"".c"
elif [ $lower = "deque" ]; then
	echo "${vectorrev}" >> "$path"".c"
	echo "${ringrev}" >> "$path"".c"
	if [ "$algo" != "false" ]; then
		echo "${algorev}" >> "$path"".c"
	fi
elif [ $lower = "string" ]; then
	echo "${vectorrev}" >> "$path"".c"
	if [ "$algo" != "false" ]; then
		echo "${algorev}" >> "$path"".c"
	fi
elif [ $lower = "set" -o $lower = "multiset" -o\
	   $lower = "map" -o $lower = "multimap" ]; then
	echo "${rbtreerev}" >> "$path"".c"
	if [ "$rbdebug" != "" ]; then
		echo "${rbdebugrev}" >> "$path"".c"
	fi
elif [ $lower = "unordered_set" -o $lower = "unordered_multiset" -o\
	   $lower = "unordered_map" -o $lower = "unordered_multimap" ]; then
	echo "${hashtablerev}" >> "$path"".c"
	if [ "$hashdebug" != "" ]; then
		echo "${hashdebugrev}" >> "$path"".c"
	fi
fi
echo " */" >> "$path"".c"
echo "#include <stdlib.h>" >> "$path"".c"
if [ "$comp" = "strcmp" -o\
		$lower = "vector" -o $lower = "deque" -o $lower = "string" -o\
		$lower = "unordered_set" -o $lower = "unordered_multiset" -o\
		$lower = "unordered_map" -o $lower = "unordered_multimap" ]; then
	echo "#include <string.h>" >> "$path"".c"
fi
if [ "$rbdebug" != "" -o "$hashdebug" != "" -o "$debug" != "" ]; then
	echo "#include <stdio.h>" >> "$path"".c"
	if [ "$hashdebug" != "" ]; then
		echo "#include <math.h>" >> "$path"".c"
	fi
fi
echo "#include \"$name.h\"
" >> "$path"".c"
if [ $lower = "list" ]; then
echo "\
#if !defined(NDEBUG) && defined(CSTL_DEBUG)
#include <assert.h>
#define CSTL_MAGIC(x)	x
#define CSTL_ASSERT(x)	assert(x)
#define CSTL_LIST_MAGIC_ELEM(Name) ((Name *) -1)
#else
#define CSTL_MAGIC(x)
#define CSTL_ASSERT(x)
#endif

#define CSTL_UNUSED_PARAM(x)	(void) x
" >> "$path"".c"
else
echo "\
#if !defined(NDEBUG) && defined(CSTL_DEBUG)
#include <assert.h>
#define CSTL_MAGIC(x)	x
#define CSTL_ASSERT(x)	assert(x)
#else
#define CSTL_MAGIC(x)
#define CSTL_ASSERT(x)
#endif

#define CSTL_UNUSED_PARAM(x)	(void) x
" >> "$path"".c"
fi
if [ "$alloc" != "" ]; then
if [ "$alloc" = "gc" ]; then
echo "\
#include <gc.h>
#define malloc   GC_MALLOC
#define realloc  GC_REALLOC
#define free(p)
" >> "$path"".c"
else
echo "\
#include \"Pool.h\"
extern Pool $alloc;
#define malloc(s)      Pool_malloc(&$alloc, s)
#define realloc(p, s)  Pool_realloc(&$alloc, p, s)
#define free(p)        Pool_free(&$alloc, p)
" >> "$path"".c"
fi
fi
if [ $lower = "vector" ]; then
	grep '#define CSTL_VECTOR_.*self' "../cstl/vector.h" | sed -e "s/\r//" >> "$path"".c"
elif [ $lower = "ring" ]; then
	grep '#define CSTL_RING_.*self' "../cstl/ring.h" | sed -e "s/\r//" >> "$path"".c"
elif [ $lower = "deque" ]; then
	grep '#define CSTL_VECTOR_.*self' "../cstl/vector.h" | sed -e "s/\r//" >> "$path"".c"
	grep '#define CSTL_RING_.*self' "../cstl/ring.h" | sed -e "s/\r//" >> "$path"".c"
	grep '#define CSTL_DEQUE_\(.*self\|RINGBUF_SIZE\|INITIAL_MAP_SIZE\)' "../cstl/deque.h" | sed -e "s/\r//" >> "$path"".c"
elif [ $lower = "list" ]; then
	grep '#define CSTL_LIST_.*\(self\|pos\)' "../cstl/list.h" | sed -e "s/\r//" >> "$path"".c"
elif [ $lower = "string" ]; then
	grep '#define CSTL_VECTOR_.*self' "../cstl/vector.h" | sed -e "s/\r//" >> "$path"".c"
	grep '#define CSTL_STRING_\(.*self\|DEFAULT_CAPACITY\)' "../cstl/string.h" | sed -e "s/\r//" >> "$path"".c"
elif [ $lower = "set" -o $lower = "multiset" -o\
	   $lower = "map" -o $lower = "multimap" ]; then
	grep '#define CSTL_RBTREE_.*node' "../cstl/rbtree.h" | sed -e "s/\r//" >> "$path"".c"
elif [ $lower = "unordered_set" -o $lower = "unordered_multiset" -o\
	   $lower = "unordered_map" -o $lower = "unordered_multimap" ]; then
	grep '#define CSTL_VECTOR_.*self' "../cstl/vector.h" | sed -e "s/\r//" >> "$path"".c"
fi
echo "" >> "$path"".c"
echo "$src" | cpp -CC -I.. | grep "$name" \
| sed -e 's:/\*\([ !]\):\
/\*\1:g' \
| sed -e 's/\t* \* /\
 \* /g' \
| sed -e 's:\*/:\*/\
:g' \
| indent -kr -ut -ts4 -fc1 \
| sed -e "s/$name \* /$name */g" | sed -e 's/^} /}\
\
/' \
| sed -e 's:^\( \*.*\) \*/:\1\
 \*/:' \
>> "$path"".c"

# コンパイル確認
if [ "$nocompile" = "" ]; then
	gcc -Wall -ansi -pedantic-errors "$path"".c" -c -DCSTL_DEBUG
	gcc -Wall -ansi -pedantic-errors "$path"".c" -c
	g++ -Wall -ansi -pedantic-errors "$path"".c" -c -DCSTL_DEBUG
	g++ -Wall -ansi -pedantic-errors "$path"".c" -c
	rm "$path"".o"
fi
