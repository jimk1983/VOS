#!/bin/bash

shopt -s expand_aliases
#获取当前的目录,注意：如果别的脚本引用该变量，那么该变量为其Makefile所在的目录
export CURDIR="$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)"

#环境变量设置好后，值不会改变，设置根目录
export ROOTDIR="$CURDIR"

#设置arch和架构内部使用库的路径，采用相对路径
export INLIBDIR="$ROOTDIR"

#设置别名命令，进行编译的命令
alias vosfmake="make --include-dir=$INLIBDIR -C $ROOTDIR"

function pdt_lib_compile
{
	echo $1
	
	#echo "make clean"	
    #	vosfmake clean
		
	#rm -rf $ROOTDIR/output/bin/vos/*
		
	echo "make all"
       vosfmake all
	 
	echo "make $1"
         vosfmake $1
	
	cp 	$ROOTDIR/output/bin/libvos.a $ROOTDIR/../LinuxUpServer/libs/bin/
	echo "copy libvos.a compeleted!"
}

pdt_lib_compile libvos



