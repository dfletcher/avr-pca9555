
PROJECT = PCA9555
VERSION = 0.0.2
DISTDIR = ../files

BUILDNAME = AVR-$(PROJECT)-$(VERSION)

# APPDIRS contains each direct subdirectory with a file called ".target". The
# .target file must contain the name of the TARGET for the app. Each app dir
# will be built in turn, and the produced .hex file will be copied to the root.
APPDIRS = `ls demos/*/.target | sed s/.\.target//g | sed s/demos.//g`

# APPDIRS contains each direct subdirectory with a file called ".library". Each
# library dir will be built in turn. Libraries are built before apps.
LIBDIRS = `ls */.library | sed s/.\.library//g`

all:
	for x in $(LIBDIRS); do \
		# Build a library. \
		make -C $${x} \
	; done
	for x in $(APPDIRS); do \
		# Clean LUFA/PCA9555 so we don't have stale objects. \
		make -C LUFA clean ; \
		make -C PCA9555 clean ; \
		# Find the target. \
		t=`cat demos/$${x}/.target` ; \
		# Build an app. \
		make -C demos/$${x} && \
		# Copy build target to root dir. \
		cp demos/$${x}/$${t}.hex $${x}.hex \
	; done

doc:
	for x in $(LIBDIRS); do \
		# Clean a library. \
		make -C $${x} doc \
	; done

docclean:
	for x in $(LIBDIRS); do \
		# Clean a library. \
		make -C $${x} docclean \
	; done

cleaninterim:
	for x in $(LIBDIRS); do \
		# Clean a library. \
		make -C $${x} clean \
	; done
	for x in $(APPDIRS); do \
		# Clean an app. \
		make -C demos/$${x} clean \
	; done
	make -C LUFA clean

clean: cleaninterim
	rm -f *.hex

bundle:
	mkdir -p $(DISTDIR)
	f=`ls` && \
		rm -rf $(BUILDNAME)/ && \
		mkdir -p $(BUILDNAME)/ && \
		cp -rf $${f} $(BUILDNAME)/ && \
		tar czf $(DISTDIR)/$(BUILDNAME).tar.gz $(BUILDNAME) && \
		rm -rf $(BUILDNAME)/

dist: docclean all doc cleaninterim bundle
