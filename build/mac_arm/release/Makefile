BASE=/opt/homebrew/opt/boost
BOOST_VERSION=1_90
BOOST_INCLUDE = $(BASE)/include
SDKROOT := $(shell xcrun --sdk macosx --show-sdk-path 2>/dev/null)
C_PLATFORM=-arch arm64 $(if $(SDKROOT),-isysroot $(SDKROOT),) -mmacosx-version-min=26.0
GPP=/usr/bin/c++
C_OPTIONS= -O3 -DNDEBUG
BOOST_LIB_VERSION=

include ../../makefile_common
