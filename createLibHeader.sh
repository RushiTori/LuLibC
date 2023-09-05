INC_DIR="include"
for inc_file in "$INC_DIR"/*
do
	echo -n "#include <LuLib/" >> LuLib.h
	echo -n "${inc_file##*/}" >> LuLib.h
	echo ">" >> LuLib.h
done