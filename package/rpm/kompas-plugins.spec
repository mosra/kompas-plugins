Name: kompas-plugins
URL: http://mosra.cz/blog/kompas.php
Version: 0.1.1
Release: 1
License: LGPLv3
%if %{defined suse_version}
Group: Productivity/Graphics/Viewers
%else
Group: Applications/Multimedia
%endif
Source: https://github.com/mosra/%{name}/tarball/v%{version}/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
BuildRequires: cmake >= 2.6.0
BuildRequires: kompas-core-devel = %{version}
#BuildRequires: kompas-qt-devel = %{version}

Summary: Plugins for Kompas navigation software
Requires: kompas-core = %{version}
#Requires: kompas-qt = %{version}

%description
Additional plugins for viewing maps, measuring distances etc.

%package devel
%if %{defined suse_version}
Group: Development/Libraries/C and C++
%else
Group: Development/Libraries
%endif
Summary: Kompas Plugins development files
Requires: %{name} = %{version}
Requires: kompas-core-devel = %{version}
#Requires: kompas-qt-devel = %{version}

%description devel
Plugins headers needed for developing for Kompas.

# TODO: Split future Qt plugins to package kompas-plugins-qt ?

%prep
%setup -q -n mosra-kompas-plugins-91c4bea

%build
export CFLAGS="$RPM_OPT_FLAGS"
mkdir build && cd build
cmake .. \
    -DCMAKE_INSTALL_PREFIX=%{_prefix} \
    -DCMAKE_BUILD_TYPE=Release
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
cd build
make DESTDIR=$RPM_BUILD_ROOT install
strip $RPM_BUILD_ROOT/%{_prefix}/lib/kompas/*/*.so

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{_prefix}
%exclude %{_prefix}/include
%exclude %{_prefix}/share/*/Modules

%files devel
%defattr(-,root,root,-)
%{_prefix}/include/Kompas
%{_prefix}/share/*/Modules
