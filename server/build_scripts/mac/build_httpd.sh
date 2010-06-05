SRCPATH=$1
BUILDPATH=$2
DISTROPATH=$3
PREFIX=$4

rm -rf ${DISTROPATH}${PREFIX}
mkdir $BUILDPATH
cd $BUILDPATH
$SRCPATH/configure --srcdir=$SRCPATH --prefix=$PREFIX --quiet --with-included-apr --with-expat=builtin
make $MAKEFLAGS
make install DESTDIR=$DISTROPATH
