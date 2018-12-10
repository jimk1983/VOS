#公共部分
CC=gcc
#CC=g++
AR=ar cr
#AR=ar crv
CFLAGS=-c 
MAKE=make
RM=-rm -rf
CFLAGS +=-Wall -g #-Werror -O3  	#将-Werror将告警当作错误处理


###########################
#公共属性定义
TARGET=
OUTPUTOBJSDIR=$(ROOTDIR)/output/objs/src
OUTPUTBINSDIR=$(ROOTDIR)/output/bin

##################################
#添加系统配置的头文件目录
CFLAGS+=-I$(ROOTDIR)
CFLAGS+=-I$(ROOTDIR)/config
##################################
#添加内部库的头文件目录
#lib头文件包含目录
CFLAGS+=-I$(INLIBDIR)/inc

#-lrct -lvos, 要把vos放在rct后面，根据依赖，先后有顺序的
LIFLAGS=-L$(INLIBDIR)/lib -lvos  
##################################
#外部库头文件目录指定
#LSFLAGS +=

LDFLAGS = -ldl -lpthread -lrt -lm #-ldb


















