root_dir_location=`while  [ ! -f version.h ]; do cd ..; done ; pwd`
VERSION_NUMBER=$(awk -F '"' '{print $2}' ${root_dir_location}/version.h | awk -F '.' '{print $1"."$2"."$3}')
echo ".${VERSION_NUMBER}"
