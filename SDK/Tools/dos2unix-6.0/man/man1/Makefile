# pod2man.mk -- Makefile portion to convert *.pod files to manual pages
#
#   Copyright information
#
#	Copyright (C) 2010 Jari Aalto
#
#   License
#
#	Redistribution and use in source and binary forms, with or
#	without modification, are permitted provided that the
#	following conditions are met:
#
#	1. Redistributions of source code must retain the above
#	   copyright notice, this list of conditions and the following
#	   disclaimer.
#
#	2. Redistributions in binary form must reproduce the above
#	   copyright notice, this list of conditions and the following
#	   disclaimer in the documentation and/or other materials
#	   provided with the distribution.
#
#	THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR
#	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#	PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR(S) OF THIS
#	FILE OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
#	GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
#	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
#	DAMAGE.
#
#	The license text is copy of the FreeBSD License available at
#	<http://www.gnu.org/copyleft/gpl.html> with following
#	modifications: wording "THIS SOFTWARE IS PROVIDED BY THE
#	FREEBSD PROJECT" was changed to "THIS SOFTWARE IS PROVIDED 'AS
#	IS'" and wording "IN NO EVENT SHALL THE FREEBSD PROJECT" was
#	changed to "IN NO EVENT SHALL THE AUTHOR(S)"
#
#   Description
#
# 	Convert *.pod files to manual pages.

ifneq (,)
    This makefile requires GNU Make.
endif

# This variable *must* be set when calling
PACKAGE		?= dos2unix

# Optional variables to set
MANSECT		?= 1
PODCENTER	?= $$(date "+%Y-%m-%d")

# Directories
MANSRC		=
MANDEST		= $(MANSRC)

MANPOD		= $(MANSRC)$(PACKAGE).pod
MANPAGE		= $(MANDEST)$(PACKAGE).$(MANSECT)

POD2MAN		= pod2man
POD2MAN_FLAGS	=

PODFILES = $(wildcard ../*/man1/dos2unix.pod)
MAN_OBJECTS = dos2unix.1 $(patsubst %.pod,%.1,$(PODFILES))

all: $(MAN_OBJECTS)

%.1 : %.pod
	# make target - create manual page from a *.pod page
	podchecker $<
	LC_CTYPE=C $(POD2MAN) $(POD2MAN_FLAGS) \
		--center="$(PODCENTER)" \
		--name="$(PACKAGE)" \
		--section="$(MANSECT)" \
		$< \
	| sed 's,[Pp]erl v[0-9.]\+,$(PACKAGE),' \
	  > $@ && \
	rm -f pod*.tmp
# fix for bug http://rt.perl.org/rt3//Public/Bug/Display.html?id=79410 
# "Pod2man creates wrong ROFF esc sequences for Latin-1 characters."
# Create groff (specific) escape sequences which work also on DOS/Windows.
# See also: https://rt.cpan.org/Public/Bug/Display.html?id=73804
	perl -pli.bak \
		-e s/A\\\\\\*\'/\\\\[\'A]/g\; \
		-e s/a\\\\\\*\'/\\\\[\'a]/g\; \
		-e s/E\\\\\\*\'/\\\\[\'E]/g\; \
		-e s/e\\\\\\*:/\\\\[:e]/g\; \
		-e s/e\\\\\\*\'/\\\\[\'e]/g\; \
		-e s/i\\\\\\*\'/\\\\[\'i]/g\; \
		-e s/n\\\\\\*~/\\\\[~n]/g\; \
		-e s/O\\\\\\*\'/\\\\[\'O]/g\; \
		-e s/o\\\\\\*\'/\\\\[\'o]/g\; \
		-e s/u\\\\\\*\'/\\\\[\'u]/g\; \
		$@

# End of of Makefile part
