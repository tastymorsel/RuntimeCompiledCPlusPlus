#!/bin/bash

ROOT=../..
BUILDDIR=Build

SOURCES=ConsoleExample.o ConsoleGame.o RuntimeObject01.o StdioLogSystem.o
OBJECTS := $(foreach file, ${SOURCES}, $(BUILDDIR)/${file})

DEFINES=-D PLATFORM_MAC
INCLUDE=-I $(ROOT)/External/boost -I $(ROOT)/External/libRocket/Include -I /System/Library/Frameworks/Carbon.framework/Versions/A/Frameworks/HIToolbox.framework/Versions/A/Headers

LIBS=/Users/pjh/Downloads/boost_1_47_0/bin.v2/libs/filesystem/build/darwin-4.2.1/debug/libboost_filesystem.dylib /Users/pjh/Downloads/boost_1_47_0/bin.v2/libs/filesystem/build/darwin-4.2.1/debug/libboost_filesystem.dylib /Users/pjh/Downloads/boost_1_47_0/bin.v2/libs/system/build/darwin-4.2.1/debug/libboost_system.dylib $(BUILDDIR)/RuntimeCompiler.a $(BUILDDIR)/Systems.a

TARGET=ConsoleGame

$(TARGET): $(OBJECTS) $(BUILDDIR)/RuntimeCompiler.a $(BUILDDIR)/Systems.a
	@echo -- Linking ${TARGET}...
	@g++ -o ${TARGET} ${OBJECTS} ${LIBS}

$(BUILDDIR)/%.o: %.cpp
	@echo -- Compiling $< --
	@mkdir -p $(BUILDDIR)
	g++ -g -MD -O2 -c $< -o $@ ${INCLUDE} ${DEFINES}

COMPILERX=BuildTool FileChangeNotifier ObjectInterfacePerModuleSource FileMonitor_PlatformMac Compiler_PlatformMac
COMPILEROBJECTS:= $(foreach file, $(COMPILERX), $(BUILDDIR)/$(file).o)
COMPILERSOURCES:= $(foreach file, $(COMPILERX), $(ROOT)/RuntimeCompiler/$(file).cpp)

$(BUILDDIR)/RuntimeCompiler.a: $(COMPILEROBJECTS)
	@ar -r $(BUILDDIR)/RuntimeCompiler.a $(COMPILEROBJECTS)

$(BUILDDIR)/%.o: $(ROOT)/RuntimeCompiler/%.cpp
	@echo -- Compiling $< --
	@mkdir -p $(BUILDDIR)
	g++ -g -MD -O2 -c $< -o $@ ${INCLUDE} ${DEFINES}

COMPILERY=AssetSystem/AssetSystem EntitySystem/EntitySystem Exceptions GUISystem/GUIDocument GUISystem/GUIElement GUISystem/GUISystem GUISystem/ReferenceCountable LogSystem/ThreadsafeLogSystem/ThreadsafeLogSystem_PlatformMac LogSystem/FileLogSystem/FileLogSystem LogSystem/MultiLogSystem/MultiLogSystem LogSystem/RocketLogSystem/RocketLogSystem ObjectFactorySystem/ObjectFactorySystem RocketLibSystem/Input RocketLibSystem/InputMacOSX RocketLibSystem/RocketLibSystem RocketLibSystem/RocketLibSystemFileInterface RocketLibSystem/RocketLibSystemRenderInterfaceOpenGL RocketLibSystem/RocketLibSystemSystemInterface SimpleSerializer/SimpleSerializer Systems TimeSystem/TimeSystem
COMPILEROBJECTSY:= $(foreach file, $(COMPILERY), $(BUILDDIR)/$(file).o)
COMPILERSOURCESY:= $(foreach file, $(COMPILERY), $(ROOT)/Systems/$(file).cpp)

$(BUILDDIR)/Systems.a: $(COMPILEROBJECTSY)
	@ar -r $(BUILDDIR)/Systems.a $(COMPILEROBJECTSY)

$(BUILDDIR)/%.o: $(ROOT)/Systems/%.cpp
	@echo -- Compiling $< --
	@mkdir -p `dirname $@`
	g++ -g -MD -O2 -c $< -o $@ ${INCLUDE} ${DEFINES}

