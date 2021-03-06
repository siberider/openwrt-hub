#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/target.mk

PKG_NAME:=uClibc
PKG_VERSION:=$(call qstrip,$(CONFIG_UCLIBC_VERSION))
ifeq ($(CONFIG_UCLIBC_VERSION_NPTL),y)
  PKG_SOURCE_URL:=git://git.busybox.net/uClibc
  PKG_SOURCE_PROTO:=git
  PKG_SOURCE_VERSION:=0949826e9501e9f08767aeb476ea1464bf95f218
  PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
  PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
  LIBC_SO_VERSION:=0.9.30-svn
else
  PKG_SOURCE_URL:=http://www.uclibc.org/downloads
  PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
  LIBC_SO_VERSION:=$(PKG_VERSION)
endif
PATCH_DIR:=./patches-$(PKG_VERSION)
CONFIG_DIR:=./config-$(PKG_VERSION)

PKG_MD5SUM_0.9.29 = 61dc55f43b17a38a074f347e74095b20
PKG_MD5SUM_0.9.30.1 = 1a4b84e5536ad8170563ffa88c34679c
PKG_MD5SUM=$(PKG_MD5SUM_$(PKG_VERSION))

HOST_BUILD_DIR:=$(BUILD_DIR_TOOLCHAIN)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/toolchain-build.mk

HOST_STAMP_BUILT:=$(TOOLCHAIN_DIR)/stamp/.uclibc_installed
HOST_STAMP_INSTALLED:=$(TOOLCHAIN_DIR)/stamp/.uclibc-utils_installed

UCLIBC_TARGET_ARCH:=$(shell echo $(ARCH) | sed -e s'/-.*//' \
		-e 's/i.86/i386/' \
		-e 's/sparc.*/sparc/' \
		-e 's/arm.*/arm/g' \
		-e 's/avr32.*/avr32/g' \
		-e 's/m68k.*/m68k/' \
		-e 's/ppc/powerpc/g' \
		-e 's/v850.*/v850/g' \
		-e 's/sh64/sh/' \
		-e 's/sh[234].*/sh/' \
		-e 's/mips.*/mips/' \
		-e 's/mipsel.*/mips/' \
		-e 's/cris.*/cris/' \
)


define Host/SetToolchainInfo
	$(SED) 's,^\(LIBC_TYPE\)=.*,\1=$(PKG_NAME),' $(TOOLCHAIN_DIR)/info.mk
	$(SED) 's,^\(LIBC_URL\)=.*,\1=http://www.uclibc.org/,' $(TOOLCHAIN_DIR)/info.mk
	$(SED) 's,^\(LIBC_VERSION\)=.*,\1=$(PKG_VERSION),' $(TOOLCHAIN_DIR)/info.mk
	$(SED) 's,^\(LIBC_SO_VERSION\)=.*,\1=$(LIBC_SO_VERSION),' $(TOOLCHAIN_DIR)/info.mk
endef

GEN_CONFIG=$(SCRIPT_DIR)/kconfig.pl -n $(if $(wildcard $(CONFIG_DIR)/common),'+' $(CONFIG_DIR)/common) ./$(CONFIG_DIR)/$(ARCH)$(if $(wildcard $(CONFIG_DIR)/$(ARCH).$(BOARD)),.$(BOARD))

define Host/Prepare
	$(call Host/SetToolchainInfo)
	$(call Host/Prepare/Default)
	ln -snf $(PKG_NAME)-$(PKG_VERSION) $(BUILD_DIR_TOOLCHAIN)/$(PKG_NAME)
	$(GEN_CONFIG) > $(HOST_BUILD_DIR)/.config
	$(SED) 's,^KERNEL_HEADERS=.*,KERNEL_HEADERS=\"$(BUILD_DIR_TOOLCHAIN)/linux-dev/usr/include\",g' \
		-e 's,^.*UCLIBC_HAS_FPU.*,UCLIBC_HAS_FPU=$(if $(CONFIG_SOFT_FLOAT),n,y),g' \
		-e 's,^.*UCLIBC_HAS_SOFT_FLOAT.*,UCLIBC_HAS_SOFT_FLOAT=$(if $(CONFIG_SOFT_FLOAT),y,n),g' \
		-e 's,^.*UCLIBC_HAS_LFS.*,UCLIBC_HAS_LFS=$(if $(CONFIG_LARGEFILE),y,n),g' \
		-e 's,^.*CONFIG_ARM_EABI.*,CONFIG_ARM_EABI=$(if $(CONFIG_EABI_SUPPORT),y,n),g' \
		-e 's,^.*CONFIG_ARM_OABI.*,CONFIG_ARM_OABI=$(if $(CONFIG_EABI_SUPPORT),n,y),g' \
		-e 's,^.*UCLIBC_HAS_SHADOW.*,UCLIBC_HAS_SHADOW=$(if $(CONFIG_SHADOW_PASSWORDS),y,n),g' \
		$(HOST_BUILD_DIR)/.config
	$(MAKE) -C $(HOST_BUILD_DIR)/extra/config conf KBUILD_HAVE_NLS= HOSTCFLAGS="-DKBUILD_NO_NLS"
	PATH=$(TARGET_PATH) $(MAKE) -C $(HOST_BUILD_DIR) \
		PREFIX="$(BUILD_DIR_TOOLCHAIN)/$(LIBC)-dev/" \
		DEVEL_PREFIX=/usr/ \
		RUNTIME_PREFIX="$(BUILD_DIR_TOOLCHAIN)/$(LIBC)-dev/" \
		HOSTCC="$(HOSTCC)" \
		CC="$(TARGET_CC)" \
		CPU_CFLAGS="$(TARGET_CFLAGS)" \
		pregen \
		install_headers
endef

define Host/Configure
endef

UCLIBC_MAKE = PATH=$(TARGET_PATH) $(MAKE) -C $(HOST_BUILD_DIR) \
	$(TARGET_CONFIGURE_OPTS) \
	DEVEL_PREFIX=/usr/ \
	RUNTIME_PREFIX=/ \
	HOSTCC="$(HOSTCC)" \
	CPU_CFLAGS="$(TARGET_CFLAGS)" \
	LIBGCC="$(subst libgcc.a,libgcc_initial.a,$(shell $(TARGET_CC) -print-libgcc-file-name))" \
	DOSTRIP=""

define Host/Compile
	$(SED) 's,^CROSS=.*,CROSS=$(TARGET_CROSS),g' $(HOST_BUILD_DIR)/Rules.mak
	$(UCLIBC_MAKE) PREFIX= all
	$(UCLIBC_MAKE) PREFIX="$(TOOLCHAIN_DIR)/" install_runtime install_dev
	ln -sf ../../lib/libc.so.0 $(TOOLCHAIN_DIR)/usr/lib/libc.so
#	( cd $(TOOLCHAIN_DIR) ; \
#		for d in lib usr/lib ; do \
#		  for f in libc.so libpthread.so libgcc_s.so ; do \
#		    if [ -f $$$$d/$$$$f -a ! -L $$$$d/$$$$f ] ; then \
#		      $(SED) 's,/usr/lib/,,g;s,/lib/,,g' $$$$d/$$$$f ; \
#		    fi \
#		  done \
#		done \
#	)
endef

define Host/Install
	$(UCLIBC_MAKE) PREFIX= utils
	$(INSTALL_DIR) $(TOOLCHAIN_DIR)/bin
	$(INSTALL_DIR) $(TOOLCHAIN_DIR)/usr/bin
	$(INSTALL_BIN) \
		$(HOST_BUILD_DIR)/utils/ldd \
		$(TOOLCHAIN_DIR)/usr/bin/
	$(INSTALL_DIR) $(TOOLCHAIN_DIR)/sbin
	$(INSTALL_BIN) \
		$(HOST_BUILD_DIR)/utils/ldconfig \
		$(TOOLCHAIN_DIR)/sbin/
endef

define Host/Clean
	rm -rf \
		$(HOST_BUILD_DIR) \
		$(BUILD_DIR_TOOLCHAIN)/$(PKG_NAME) \
		$(BUILD_DIR_TOOLCHAIN)/$(LIBC)-dev
endef

$(eval $(call HostBuild))
