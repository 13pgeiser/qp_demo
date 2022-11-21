#!/bin/bash
set -e
if [ "$#" -eq "0" ]; then
	echo "Valid arguments are 'arm' or 'host'"
	exit 1
fi
source bash-scripts/helpers.sh
update_license_copyright_year
run_shfmt_and_shellcheck ./*.sh
# Install required tools
install_packages make subversion gcc
install_cmake
if [ "$1" == "arm" ]; then
	install_gcc_arm_none_eabi
fi
write_sourceme
download_inno() {
	case "$OSTYPE" in
	msys)
		local result
		result=$(download_unpack 81e68ff6f7c21b0cacd506569b363c2a https://downloads.sourceforge.net/project/innounp/innounp/innounp%200.49/innounp049.rar "ce" "" "")
		INNO="$result/innounp.exe"
		;;
	linux*)
		install_debian_packages innoextract
		;;
	*)
		die "Unsupported OS: $OSTYPE"
		;;
	esac
}
# Install qpc and qm
install_qp_qm() { #helpmsg: Install cmake
	case "$OSTYPE" in
	msys)
		QP_VERSION="qp-windows_6.9.1a"
		QP_FOLDER="$TOOLS_FOLDER/$QP_VERSION"
		if [ ! -d "$QP_FOLDER" ]; then
			download_inno
			download 1f346c89a4fcce8300aa0359ce1f362d https://www.state-machine.com/downloads/qp-windows_7.1.3.exe ""
			mkdir -p "$QP_FOLDER"
			cmd="$INNO -x $(cygpath -w "$TOOLS_FOLDER/$QP_VERSION.exe") -q -d$(cygpath -w "$TOOLS_FOLDER/$QP_VERSION")"
			$cmd
		fi
		export QPC_BUNDLE="$QP_FOLDER/{app}"
		;;
	linux*)
		local result
		result=$(download_unpack 6cbd2970b4634f6ff4addc6573bd354a https://www.state-machine.com/downloads/qp-linux_7.1.3.zip "ce" "" "")
		export QPC_BUNDLE="$result/qp"
		chmod +x "$QPC_BUNDLE/qm/bin/qm"
		;;
	*)
		die "Unsupported OS: $OSTYPE"
		;;
	esac
}
install_qp_qm
"$QPC_BUNDLE/qm/bin/qm" model/model.qm -c
# Create config.cmake
echo "# Configured variables" >config.cmake
echo "# Do not edit manually!" >>config.cmake
echo "set(QPC_FOLDER $(realpath --relative-to=. "$QPC_BUNDLE"/qpc))" >>config.cmake
if [ "$OSTYPE" == "msys" ]; then
	echo "set (QP_PORT \${QPC_FOLDER}/ports/win32)" >>config.cmake
else
	echo "set (QP_PORT \${QPC_FOLDER}/ports/posix)" >>config.cmake
fi
# Done! Now call cmake
rm -rf CMakeFiles/
rm -f CMakeCache.txt cmake_install.cmake compile_commands.json Makefile qp_demo*
if [ "$1" == "host" ]; then
	chmod +x "$QPC_BUNDLE"/qm/bin/qm
	"$QPC_BUNDLE"/qm/bin/qm model/model.qm -c
	call_cmake .
elif [ "$1" == "arm" ]; then
	call_cmake .. -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi.cmake
else
	echo "Usage: $0 arm|host"
fi
