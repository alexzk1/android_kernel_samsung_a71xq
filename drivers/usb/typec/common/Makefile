#
# Makefile for usbpd common drivers
#

obj-$(CONFIG_PDIC_NOTIFIER)		+= pdic_notifier.o pdic_sysfs.o pdic_core.o pdic_misc.o

GCOV_PROFILE := $(CONFIG_KUNIT)
obj-$(CONFIG_KUNIT) += test/
