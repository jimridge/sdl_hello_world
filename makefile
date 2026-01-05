# ---- config ----
CC       ?= clang
PKG      ?= pkg-config
SDL_PC   ?= sdl3
IMG_PC   ?= sdl3-image
FONT_PC  ?= sdl3-ttf

# Output / input layout
BIN_DIR  := bin
OBJ_DIR  := build
TARGET   := $(BIN_DIR)/build_osx   # change to your program name

SRC      := $(wildcard src/*.c)
OBJ      := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRC))
DEP      := $(OBJ:.o=.d)

# Flags
CPPFLAGS += $(shell $(PKG) --cflags $(SDL_PC) $(IMG_PC)  $(FONT_PC))
CFLAGS   += -O2 -Wall -Wextra -MMD -MP
LDLIBS   += $(shell $(PKG) --libs   $(SDL_PC) $(IMG_PC)  $(FONT_PC))
# If you want a universal binary, uncomment:
# ARCH    := -arch arm64 -arch x86_64
# CFLAGS  += $(ARCH)
# LDFLAGS += $(ARCH)

# ---- rules ----
.PHONY: all run clean debug release

all: $(TARGET)

# Link
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compile
$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Dirs
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Convenience
run: $(TARGET)
	./$(TARGET)

debug:
	$(MAKE) CFLAGS='-g -O0 -Wall -Wextra -MMD -MP' $(TARGET)

release:
	$(MAKE) CFLAGS='-O3 -DNDEBUG -Wall -Wextra -MMD -MP' $(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Auto-rebuild on header changes
-include $(DEP)





# COMPILER = clang
# SOURCE_LIBS = -Ilib/
# OSX_OPT = -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -Llib/libSDL3.a -L -Iinclude/SDL3
# OSX_OUT = -o "bin/build_osx"
# CFILES = src/*.c
# build_osx:
# 	$(COMPILER) $(CFILES) $(SOURCE_LIBS) $(OSX_OUT) $(OSX_OPT)
# 	bin/build_osx


# ------------------
# CC         ?= clang
# SDL_PREFIX ?= $(HOME)/sdl3-static

# CPPFLAGS  += -I$(SDL_PREFIX)/include/SDL3   # convention: includes in CPPFLAGS
# CFLAGS    += -O2 -Wall -Wextra
# LDFLAGS   += -L$(SDL_PREFIX)/lib
# LDLIBS    += $(SDL_PREFIX)/lib/libSDL3.a \
#              -Wl,-framework,Cocoa -Wl,-framework,IOKit \
#              -Wl,-framework,CoreVideo -Wl,-framework,QuartzCore

# # Optional universal build flags:
# # ARCH_FLAGS := -arch arm64 -arch x86_64
# # CFLAGS    += $(ARCH_FLAGS)
# # LDFLAGS   += $(ARCH_FLAGS)

# SRC := $(wildcard src/*.c) main.c
# OBJ := $(SRC:.c=.o)
# BIN := demo

# # <-- This is where CFLAGS/CPPFLAGS are referenced explicitly
# %.o: %.c
# 	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# $(BIN): $(OBJ)
# 	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# clean:
# 	rm -f $(OBJ) $(BIN)