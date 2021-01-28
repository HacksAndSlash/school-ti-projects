# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= XWING3
ICON ?= icon.png
DESCRIPTION ?= "XWING III v1.1"
COMPRESSED ?= NO
ARCHIVED ?= YES

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk
