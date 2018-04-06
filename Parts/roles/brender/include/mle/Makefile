#!smake

TOP = ../../../../../..
include $(TOP)/make/fwcommondefs

PC_HEADERS_LIST = pchdrs.txt

#
# Header files for all targets
#
SHARE_HEADERS = \
	2dimgd.h \
	2dsprd.h \
	2dmvd.h \
	3dannoc.h \
	br3dd.h \
	br3dannd.h \
	$(NULL)

#
# No ship header files for all targets
#
SHARE_HEADERS_NOSHIP = $(NULL)

#
# Header files for all PC target
#
PC_HEADERS = \
	$(NULL)

#
# Header files for all SGI targets
#
SGI_HEADERS = \
	$(NULL)

HEADERS = \
	$(SHARE_HEADERS)\
	$(SGI_HEADERS) \
	$(PC_HEADERS) \
	$(NULL)

default all:

install headers: $(_FORCE)
	for i in $(HEADERS); do \
	$(INSTALL_SOURCE) -F $(FW_INSTALL_DIR_HEADERS) $(FW_IDB_DEV) $(FW_MODE_INC) $(FW_INSTALL_DIR_HEADER) $$i; \
	done
	ls -1 $(SHARE_HEADERS) $(PC_HEADERS) > $(PC_HEADERS_LIST)

#	for i in $(SHARE_HEADERS_NOSHIP); do \
#	$(INSTALL_SOURCE) -idb noship -F $(FW_INSTALL_DIR_HEADERS) $(FW_IDB_DEV) $(FW_MODE_INC) $(FW_INSTALL_DIR_HEADER) $$i; \
#	done

clobber clean:
	$(RM) $(PC_HEADERS_LIST)

$(_FORCE):
