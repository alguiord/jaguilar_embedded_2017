AUTOMAKE_OPTIONS = foreign

CFLAGS = -Wall -pedantic
include_HEADERS = rgb2yuv_c.h

lib_LTLIBRARIES = librgb2yuv_c.la
librgb2yuv_c_la_SOURCES  = rgb2yuv_clib.c
librgb2yuv_c_la_LDFLAGS= -version-info 1:0:0

bin_PROGRAMS = rgb2yuv_c
rgb2yuv_c_SOURCES = rgb2yuv_c.c
rgb2yuv_c_LDADD = .libs/librgb2yuv_c.la
