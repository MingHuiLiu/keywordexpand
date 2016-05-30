#!/usr/bin/env python
#-*- coding: utf-8 -*-
import time
import datetime
import string
import sys

curTime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
strdate = str(time.strftime('%Y%m%d',time.localtime(time.time())))
ds = strdate
dst_table = "wxy_sourceid_partition"

def WriteLog(argv0,argv1):
    global curTime
    print (curTime+"|"+argv0+"|"+argv1)

#时间参数传给iDate变量
def config(argv):
    global ds
    if len(argv)==1:
        ds=argv[0]
    WriteLog("ds=",ds)

##创建分区函数
def m_partition(tdw):
    global ds
    #选择表所在的库
    sql="use wxg_data_valueless"
    WriteLog ("running",sql)
    res = tdw.execute(sql)
    #建分区
    sql="alter table %s drop partition (p_%s)" %(dst_table, ds)
    WriteLog ("running",sql)
    res = tdw.execute(sql)
    sql="alter table %s add partition p_%s values in ('%s')" %(dst_table, ds, ds)
    WriteLog ("running",sql)
    res = tdw.execute(sql)
    #主函数
def TDW_PL(tdw, argv=[]):
    WriteLog("begin","Program  add_bizuin_partition begin")
    config(argv)
    m_partition(tdw)
    WriteLog("end","Program  add_bizuin_partition end")