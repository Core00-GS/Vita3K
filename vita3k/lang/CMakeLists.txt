add_library(
	lang
	STATIC
	include/lang/state.h
	src/lang.cpp
)

target_include_directories(lang PUBLIC include)
