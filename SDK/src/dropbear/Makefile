DROPBEAR_VERSION=2013.60
DROPBEAR_ARM_CONF=--host=arm --prefix=$(FS_PATH) --disable-syslog --disable-utmp --disable-utmpx --disable-lastlog CPPFLAGS="-I$(BASEDIR)/SFCB/zlib-1.2.3/zlib/include" LDFLAGS="$(LDFLAGS) -L$(BASEDIR)/SFCB/zlib-1.2.3/zlib/lib"
DBSRCDIR=$(SRCDIR)/dropbear/dropbear-$(DROPBEAR_VERSION)

all:
	cd $(SRCDIR)/dropbear
	tar jxf dropbear-$(DROPBEAR_VERSION).tar.bz2
	cd $(DBSRCDIR); ./configure $(DROPBEAR_ARM_CONF)
	cd $(DBSRCDIR); make && make install;
	rm -rf $(FS_PATH)/share/
	$(STRIP) --strip-unneeded $(FS_PATH)/bin/dropbearkey;
	$(STRIP) --strip-unneeded $(FS_PATH)/bin/dbclient;
	$(STRIP) --strip-unneeded $(FS_PATH)/sbin/dropbear;
	mkdir -p $(FS_PATH)/etc/init.d $(FS_PATH)/etc/dropbear/;
	cp smashd $(FS_PATH)/etc/init.d/
	cd $(FS_PATH)/etc/dropbear/; ln -fs /nv/dropbear_rsa_host_key dropbear_rsa_host_key
	cd $(FS_PATH)/etc/dropbear/; ln -fs /nv/dropbear_dss_host_key dropbear_dss_host_key

