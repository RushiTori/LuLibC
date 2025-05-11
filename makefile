# Makefile by RushiTori - January 1st 2025
# ====== Everything Makefile internal related ======

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
MAKEFLAGS+=--no-print-directory
RM+=-r
DEBUG:=0
RELEASE:=1
INSTALL_SUPPORTED:=1
ifdef OS
else ifeq ($(shell uname), Linux)
else
	INSTALL_SUPPORTED:=0
endif

# ========= Everything project related =========

CC:=gcc
LD:=ar
SRC_EXT:=c
HDS_EXT:=h
OBJ_EXT:=o
DEP_EXT:=d

NAME:=LuLib
HDR_NAME:=$(NAME).$(HDS_EXT)
LIB_NAME:=lib$(NAME).a
HDR_GUARD:=LULIB_H

MAIN_INST_DIR:=./
ifdef OS
	MAIN_INST_DIR:=C:/CustomLibs
else ifeq ($(shell uname), Linux)
	MAIN_INST_DIR:=/usr/local
endif
LIB_INST_DIR:=$(MAIN_INST_DIR)/lib
HDS_INST_SUB_DIR:=LuLib
HDS_INST_DIR:=$(MAIN_INST_DIR)/include/$(HDS_INST_SUB_DIR)

# ========== Everything files related ==========

HDS_DIR:=include
SRC_DIR:=src
OBJ_DIR:=objs

HDS_FILES:=$(call rwildcard,$(HDS_DIR),*.$(HDS_EXT))
SRC_FILES:=$(call rwildcard,$(SRC_DIR),*.$(SRC_EXT))
OBJ_FILES:=$(SRC_FILES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/%.$(OBJ_EXT))
DEP_FILES:=$(SRC_FILES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/%.$(DEP_EXT))

INST_HDS_FILES:=$(HDS_FILES:$(HDS_DIR)/%=$(HDS_INST_DIR)/%)

# ========== Everything flags related ==========

HDS_PATHS:=$(addprefix -I,$(sort $(dir $(HDS_FILES))))
STD_FLAGS:=-std=c2x -Wall -Wextra -Werror -Wfatal-errors 
CCFLAGS:=$(HDS_PATHS) $(STD_FLAGS)
ifeq ($(DEBUG), 1)
	CCFLAGS+=-g
else
	ifeq ($(RELEASE), 1)
		CCFLAGS+=-O3
	endif
endif

LDFLAGS:=-rcs

# =========== Every usable functions ===========

# Basic Build

build: $(LIB_NAME) buildHeader

$(LIB_NAME): $(OBJ_FILES)
	@echo Linking $@
	@$(LD) $(LDFLAGS) $@ $^

debug:
	@$(MAKE) DEBUG=1

release:
	@$(MAKE) RELEASE=1

-include $(DEP_FILES)
$(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@echo Compiling $< into $@
	@mkdir -p $(dir $@)
	@$(CC) -c $< -o $@ $(CCFLAGS) -MMD

buildHeader:
	@$(RM) $(HDR_NAME)
	@touch $(HDR_NAME)
	@echo "#ifndef" $(HDR_GUARD) >> $(HDR_NAME)
	@echo "#define" $(HDR_GUARD) >> $(HDR_NAME)
	@echo "" >> $(HDR_NAME)
	@for hdr in $(notdir $(HDS_FILES)) ; do \
		echo -n "#include <"        >> $(HDR_NAME) ; \
		echo -n $(HDS_INST_SUB_DIR) >> $(HDR_NAME) ; \
		echo -n "/"                 >> $(HDR_NAME) ; \
		echo -n $$hdr               >> $(HDR_NAME) ; \
		echo    ">"                 >> $(HDR_NAME) ; \
	done
	@echo "" >> $(HDR_NAME)
	@echo "#endif //" $(HDR_GUARD) >> $(HDR_NAME)

install: release buildHeader
ifeq ($(INSTALL_SUPPORTED), 0)
	@echo Cannot auto-install this lib on your system
	@exit 1
endif
	@echo Installing $(LIB_NAME) at $(LIB_INST_DIR)
	@cp -u $(LIB_NAME) $(LIB_INST_DIR)/$(LIB_NAME)

	@echo Installing $(HDR_NAME) to $(MAIN_INST_DIR)/include/
	@cp -u $(HDR_NAME) $(MAIN_INST_DIR)/include/$(HDR_NAME)

	@echo Installing the headers at $(HDS_INST_DIR)
	@mkdir -p $(HDS_INST_DIR)
	@$(MAKE) --silent installHeaders

installHeaders: $(INST_HDS_FILES)
$(HDS_INST_DIR)/%.$(HDS_EXT): $(HDS_DIR)/%.$(HDS_EXT)
	@echo Installing $(notdir $@)
	@cp -u $< $@

uninstall:
ifeq ($(INSTALL_SUPPORTED), 0)
	@echo Cannot auto-uninstall this lib on your system
	@exit 1
endif
	@echo Uninstalling $(LIB_NAME) from $(LIB_INST_DIR)
	@$(RM) $(LIB_INST_DIR)/$(LIB_NAME)

	@echo Uninstalling $(HDR_NAME) from $(MAIN_INST_DIR)/include/
	@$(RM) $(MAIN_INST_DIR)/include/$(HDR_NAME)

	@echo Uninstalling the headers from $(HDS_INST_DIR)
	@$(RM) -r $(HDS_INST_DIR)

clean:
	@echo Deleting $(OBJ_DIR) folder
	@$(RM) $(OBJ_DIR)

wipe: clean
	@echo Deleting $(LIB_NAME)
	@$(RM) $(LIB_NAME)
	@echo Deleting $(HDR_NAME)
	@$(RM) $(HDR_NAME)

rebuild: wipe build

redebug: wipe debug

rerelease: wipe release

reinstall: uninstall wipe install 

.PHONY: build debug release install 
.PHONY: clean wipe 
.PHONY: rebuild redebug rerelease reinstall
.PHONY: buildHeader installHeaders
