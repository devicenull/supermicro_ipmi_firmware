dist_version=`cat /proc/version | grep 'Red Hat'`
make_ver1=1
make_ver2=10
make_ver3=1
conf_ver1=2
conf_ver2=60

current_pwd=`pwd`

function install_automake(){
    echo "****************** Install automake ($make_ver1.$make_ver2) *******************"	
	tar -zxvf ./automake-1.10.tar.tar
	cd ./automake-1.10
	./configure --prefix=/usr
	make
	make install
	cd ..
}

function install_autoconf(){
    echo "****************** Install autoconf ($conf_ver1.$conf_ver2)*******************"
    tar -zxvf ./autoconf-2.60.tar.tar
    cd ./autoconf-2.60
    ./configure --prefix=/usr
    make
    make install
    cd ..	
}


echo "***************** environment checking **********************"

host_mver1=`automake --version | head -n 1 | sed -e 's/[^0-9.]//g' | awk -F '.' '{print $1}'`
host_mver2=`automake --version | head -n 1 | sed -e 's/[^0-9.]//g' | awk -F '.' '{print $2}'`
host_mver3=`automake --version | head -n 1 | sed -e 's/[^0-9.]//g' | awk -F '.' '{print $3}'`
host_cver1=`autoconf --version | head -n 1 | sed -e 's/[^0-9.]//g' | awk -F '.' '{print $1}'`
host_cver2=`autoconf --version | head -n 1 | sed -e 's/[^0-9.]//g' | awk -F '.' '{print $2}'`

if [ ! -z "$dist_version" ]; then

	echo "Your automake version : `automake --version | head -n 1 | sed -e 's/[^0-9.]//g'`"
    if [ "`expr $host_mver1 \< $make_ver1`" = "1" ]; then
        install_automake
    else
        if [ "`expr $host_mver1 = $make_ver1`" = "1" ] && [ "`expr $host_mver2 \< $make_ver2`" = "1" ]; then
            install_automake
        else
	    echo "automake version checked."
        fi
    fi


	echo "Your autoconf version : "`autoconf --version | head -n 1 | sed -e 's/[^0-9.]//g'` 
	if [ "`expr $host_cver1 \< $conf_ver1`" = "1" ]; then
		install_autoconf
	else
		if [ "`expr $host_cver1 = $conf_ver1`" = "1" ] && [ "`expr $host_cver2 \< $conf_ver2`" = "1" ]; then
			install_autoconf
		else
			echo "autoconf version checked."
		fi
	fi
fi

echo "Done."

