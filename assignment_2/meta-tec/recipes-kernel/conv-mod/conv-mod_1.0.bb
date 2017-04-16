SUMMARY = "Example of how to build an external Linux kernel module"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=12f884d2ae1ff87c09e5b7ccc2c4ca7e"
DESCRIPTION = "Simple helloworld application"


inherit module

SRC_URI = "file://conv_mod-${PV}.tar.gz"

S = "${WORKDIR}/conv_mod-${PV}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.
