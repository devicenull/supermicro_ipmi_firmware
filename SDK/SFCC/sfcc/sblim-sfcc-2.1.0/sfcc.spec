#
# $Id: sfcc.spec.in,v 1.2 2007/02/27 14:07:00 mihajlov Exp $
#
# Package spec for sblim-sfcc
#

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}

Summary: Small Footprint CIM Client Library
Name: sblim-sfcc
Version: 2.1.0
Release: 0
Group: Systems Management/Base
License: EPL
URL: http://www.sblim.org
Source0: ftp://dl.sourceforge.net/pub/s/sb/sblim/%{name}-%{version}.tar.bz2
BuildRequires: curl-devel gcc-c++
Requires: curl

%Description
Small Footprint CIM Client Library (sfcc) Runtime Libraries

%package devel
Summary: Small Footprint CIM Client Library
Group: Systems Management/Base
Requires: %{name}=%{version}

%Description devel
Small Footprint CIM Client Library (sfcc) Header Files and Link Libraries


%prep

%setup -T -b 0 -n %{name}-%{version}

export PATCH_GET=0
#%patch0 -p1

%build

%configure 
make

%install

if [ `id -ur` != 0 ]
then
# paranoia check 
	rm -rf $RPM_BUILD_ROOT 
fi

make DESTDIR=$RPM_BUILD_ROOT install

# remove unused libtool files
rm -f $RPM_BUILD_ROOT/%{_libdir}/*a

%clean
if [ `id -ur` != 0 ]
then
# paranoia check 
	rm -rf $RPM_BUILD_ROOT 
fi

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(-,root,root)
%doc %{_datadir}/doc
%{_libdir}/*.so.*
%{_libdir}/libcimcClientXML.so

%files devel
%defattr(-,root,root)
%doc %{_mandir}
%{_includedir}
%{_libdir}/libcimcclient.so
%{_libdir}/libcmpisfcc.so

%changelog

* Fri Feb 16 2007  <mihajlov@dyn-9-152-143-45.boeblingen.de.ibm.com> - 2.0.0-0
- Initial Version

