.DEFAULT_GOAL = build
BUILD_APP_NAME := App

CC := gcc
APP_DIR := app
BUILD_DIR := build
SRC_DIR := source

BUILD_OBJ_DIR := Objects
GUI_DIR := GUI

APP_PATH := .\$(APP_DIR)
BUILD_PATH := .\$(BUILD_DIR)
BUILD_OBJ_PATH := $(BUILD_PATH)\$(BUILD_OBJ_DIR)

SRC_PATH := .\$(SRC_DIR)
GUI_PATH := $(SRC_PATH)\$(GUI_DIR)
GUI_LIBS_PATH := $(GUI_PATH)\libs

OBJ_FILES := $(BUILD_OBJ_PATH)\EVERY_SINGLE_C_FILE_TO_BE_COMPILED.c.o #!!!!!!!!!!!!!!!!!!!!!!!!!!!!

CFLAGS := -std=c23 -Wall -g -DGLEW_STATIC

ifeq ($(OS),Windows_NT)
	CFLAGS += -D_WIN64
	LIBS = -lglfw3 -lopengl32 -lm -lGLU32 -lGLEW32
	WINDOWS_GUI_LIBS := $(GUI_LIBS_PATH)\Windows
	LOCAL_LIBS = $(WINDOWS_GUI_LIBS)\glfw3dll.lib $(WINDOWS_GUI_LIBS)\OpenGL32.lib $(WINDOWS_GUI_LIBS)\glew32.lib
else
	UNAME_S := $(shell uname -s)
	GLFW3 := $(shell pkg-config --libs glfw3)
	ifeq ($(UNAME_S),Darwin)
		CFLAGS += -D__APPLE__
		LIBS := $(GLFW3) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lm -lGLEW -L/usr/local/lib
	else
		CFLAGS += -DLINUX
		LIBS = $(GLFW3) -lGL -lm -lGLU -lGLEW
	endif
endif

.PHONY: all
all:
	make prebuild
	make build
	make run

.PHONY: prebuild
prebuild:
	python ./Build_Tools/preBuildScript.py

.PHONY: auto_info
auto_info:
	@echo $(CC) $(CFLAGS)
	@echo $(BUILD_OBJ_PATH)
	@echo $(BUILD_PATH)\make_build.log

.PHONY: build
build:
	python .\Build_Tools\Auto_Compiler\auto_compiler.py update

.PHONY: full_build
full_build:
	python .\Build_Tools\Auto_Compiler\auto_compiler.py full

.PHONY: run
run:
	$(APP_PATH)\$(BUILD_APP_NAME).exe $(APP_PATH) > $(APP_PATH)\run_time.log

.PHONY: run_debug
run_debug:
	gdb --args $(APP_PATH)\$(BUILD_APP_NAME).exe $(APP_PATH)

.PHONY: final_compile
final_compile:
	$(CC) $(CFLAGS) $(LIBS) -I $(BUILD_OBJ_PATH) $(OBJ_FILES) $(LOCAL_LIBS) -o $(APP_PATH)\$(BUILD_APP_NAME).exe

.PHONY: clean
clean:
#	NEED TO UPDATE
	rm -r $(BUILD_DIR)
