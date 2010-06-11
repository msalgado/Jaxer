SRCPATH=$1
BUILDPATH=$2
DISTROPATH=$3
APACHE_SRCPATH=$4

rm -rf ${DISTROPATH}
mkdir ${DISTROPATH}
#./build_httpd.sh $APACHE_SRCPATH $BUILDPATH/httpd /. $BUILDPATH/httpd_dist

mkdir $BUILDPATH/mod_jaxer
cd $BUILDPATH/mod_jaxer
cp -pLR $SRCPATH/ .
$BUILDPATH/httpd_dist/bin/apxs -Wc,-arch -Wc,i386 -Wl,-arch -Wl,i386 -I $SRCPATH/../../../mozilla/aptana/manager -D JAXER_BUILDID=$JAXER_BUILDID -c *.c
$BUILDPATH/httpd_dist/bin/apxs -i mod_jaxer.la
cp $BUILDPATH/httpd_dist/modules/mod_jaxer.so $DISTROPATH/
