cmake_minimum_required(VERSION 2.8.4)
project(ShapesParser)

if( NOT MSVC )
	if( NOT CMAKE_BUILD_TYPE )
		message("Setting default build type... use -DCMAKE_BUILD_TYPE=<Debug|Release|RelWithDebInfo|MinSizeRel> to set it.")
		set( CMAKE_BUILD_TYPE Debug )
	endif( NOT CMAKE_BUILD_TYPE )
	message("Building ${CMAKE_BUILD_TYPE} version.")
endif( NOT MSVC )

##################################### Compile flags
if( NOT MSVC ) #CMAKE_COMPILER_IS_GNUCC CMAKE_COMPILER_IS_GNUC
	#enable "all" warnings
	set(cxxWarningFlags "-Wall -Wpedantic -Wformat=2 -Wextra")
	#to allow MSVC pragma warnings:
	set(cxxWarningFlags "${cxxWarningFlags} -Wno-unknown-pragmas")
	#more warnings not included in "all"
	set(cxxWarningFlags "${cxxWarningFlags} -Wredundant-decls -Wmissing-include-dirs -Wuninitialized")
	set(cxxWarningFlags "${cxxWarningFlags} -Werror=cast-qual -Wcast-align -Wpacked -Winvalid-pch -Wdisabled-optimization")
	set(cxxWarningFlags "${cxxWarningFlags} -Wundef -Wshadow -Wfloat-equal -Wstrict-overflow=5")
	if(CMAKE_COMPILER_IS_GNUCXX)
		#these are not supported by clang
		set(cxxWarningFlags "${cxxWarningFlags} -Wdouble-promotion -Wlogical-op -Wuseless-cast")
	endif()
	set(CMAKE_CXX_FLAGS "-std=c++14 -fstrict-aliasing -fstrict-overflow ${cxxWarningFlags}")
else()
	set(cxxWarningFlags "/W4")
	#crazy MS things
	set(cxxWarningFlags "${cxxWarningFlags} -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS")
	#allow GCC warning suppression pragmas
	set(cxxWarningFlags "${cxxWarningFlags} /wd4068") #4068: unknown pragma
	#uninteresting warning
	set(cxxWarningFlags "${cxxWarningFlags} /wd4512") #4512: assignment operator could not be generated
	#off by default warnings
	set(cxxWarningFlags "${cxxWarningFlags} /w34191") #unsafe conversion from 'type of expression' to 'type required'
	# set(cxxWarningFlags "${cxxWarningFlags} /w44242") #conversion from 'type1' to 'type2', possible loss of data
	set(cxxWarningFlags "${cxxWarningFlags} /w44263") #member function does not override any base class virtual member function
	set(cxxWarningFlags "${cxxWarningFlags} /w14264") #no override available for virtual member function from base 'class'; function is hidden
	# set(cxxWarningFlags "${cxxWarningFlags} /w34265") #class has virtual functions, but destructor is not virtual
	set(cxxWarningFlags "${cxxWarningFlags} /w44266") #no override available for virtual member function from base 'type'; function is hidden
	set(cxxWarningFlags "${cxxWarningFlags} /w24302") #truncation from 'type 1' to 'type 2'
	set(cxxWarningFlags "${cxxWarningFlags} /w24826") #conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior
	set(cxxWarningFlags "${cxxWarningFlags} /w14905") #wide string literal cast to 'LPSTR'
	set(cxxWarningFlags "${cxxWarningFlags} /w14906") #string literal cast to 'LPWSTR'
	set(cxxWarningFlags "${cxxWarningFlags} /w14928") #illegal copy-initialization; more than one user-defined conversion has been implicitly applied

	set(CMAKE_CXX_FLAGS "/EHsc /MP /Zi /Zm256 ${cxxWarningFlags} -DUNICODE -D_UNICODE")

	set(linkReleaseFlags "/DEBUG /OPT:REF /INCREMENTAL:NO")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE ${linkReleaseFlags})
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE ${linkReleaseFlags})
endif()

######################### Boost
find_package( Boost 1.55 REQUIRED )
if( Boost_MAJOR_VERSION EQUAL 1 AND Boost_MINOR_VERSION LESS 56 )
	#On boost 1.55 these are disabled for clang.
	add_definitions(-DBOOST_PP_VARIADICS=1)
endif()
#newer version of spirit for X3
include_directories(SYSTEM spirit/include)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

##################################### Subdirectories
add_subdirectory(src)
