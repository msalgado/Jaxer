#!/bin/sh

BASEPATH=/Users/max/dev/gitworkspaces/Jaxer
BUILDPATH=$BASEPATH/../jaxer.build
OBJDIR=$BUILDPATH/obj
DISTRO=$BUILDPATH/distro
APACHE_PREFIX=/Applications/Aptana_Jaxer/Apache22
JAXER_PREFIX=/Applications/Aptana_Jaxer/jaxer

mkdir $BUILDPATH
mkdir $OBJDIR
mkdir $DISTRO

export BUILDID="xxxx"
export CFLAGS="-arch i386 -mmacosx-version-min=10.5 -isysroot /Developer/SDKs/MacOSX10.5.sdk"
export MAKEFLAGS=-s

echo "\n******** Building httpd ********"
#./build_httpd.sh $BASEPATH/server/src/httpd-2.2.15 $OBJDIR/httpd $DISTRO $APACHE_PREFIX

echo "\n******** Building jaxer ********"
#./build_jaxer.sh $BASEPATH/server/src/mozilla $OBJDIR/jaxer $DISTRO$JAXER_PREFIX

echo "\n******** Building mod_jaxer ********"
./build_mod_jaxer.sh $BASEPATH/server/src/connectors/apache/mod_jaxer $OBJDIR/mod_jaxer $DISTRO$JAXER_PREFIX/connectors $BASEPATH/server/src/httpd-2.2.15

export CFLAGS=
