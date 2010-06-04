ifndef OBJDIR
OBJDIR = $(MOZ_OBJDIR)
endif

include $(OBJDIR)/config/autoconf.mk

FILES_TO_REMOVE = \
	components/compreg.dat \
	components/xpti.dat \
	mangle \
	mangle.exe \
	shlibsign \
	shlibsign.exe \
	xpcshell \
	xpcshell.exe \
	xpicleanup \
	xpicleanup.exe \
	xpidl \
	xpidl.exe \
	xpt_dump \
	xpt_dump.exe \
	xpt_link \
	xpt_link.exe \
	run-mozilla.sh \
	nspr-config \
	nsinstall \
	regxpcom \
	js

postflight:
	echo Package all xpt files into a single file
	$(OBJDIR)/dist/bin/xpt_link \
		$(OBJDIR)/dist/bin/components/components.xpt_ \
	    $(OBJDIR)/dist/bin/components/*.xpt
	rm -f $(OBJDIR)/dist/bin/components/*.xpt
	mv $(OBJDIR)/dist/bin/components/components.xpt_ $(OBJDIR)/dist/bin/components/components.xpt
	
	$(foreach file,$(FILES_TO_REMOVE),rm -f $(OBJDIR)/dist/bin/$(file))
		
ifdef MOZ_CRASHREPORTER
	$(MAKE) -C $(OBJDIR) buildsymbols BUILDID=$(JAXER_BUILDID)
ifndef ENABLE_STRIP
	@echo "Stripping resulting files..."
	@cd $(OBJDIR)/dist/bin; find . ! -type d \
			! -name "*.js" \
			! -name "*.xpt" \
			! -name "*.gif" \
			! -name "*.jpg" \
			! -name "*.png" \
			! -name "*.xpm" \
			! -name "*.txt" \
			! -name "*.rdf" \
			! -name "*.sh" \
			! -name "*.properties" \
			! -name "*.dtd" \
			! -name "*.html" \
			! -name "*.xul" \
			! -name "*.css" \
			! -name "*.xml" \
			! -name "*.jar" \
			! -name "*.dat" \
			! -name "*.tbl" \
			! -name "*.src" \
			! -name "*.reg" \
			$(PLATFORM_EXCLUDE_LIST) \
			-exec $(STRIP) $(STRIP_FLAGS) {} >/dev/null 2>&1 \;
endif
endif
