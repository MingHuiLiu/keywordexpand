#!/usr/bin/env python
## -*- coding: utf-8 -*-
time = __import__('time')
datetime= __import__('datetime')
string=__import__('string')
curTime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
def WriteLog(argv0,argv1):
    global curTime
    print (curTime+"|"+argv0+"|"+argv1)
def config(argv):
    global taskid_
    if len(argv)==1:
        taskid_= argv[0]

def get_article_click_uin(tdw):
    global taskid_
    sql="use wxg_data_valueless"
    res = tdw.execute(sql)
    print taskid_
    sql="""alter table  wxy_daily_game_uinlist drop partition (p_%s)"""%(taskid_)
    res = tdw.execute(sql)
    sql="""alter table wxy_daily_game_uinlist add partition p_%s values in (%s)""" %(taskid_, taskid_)
    res = tdw.execute(sql)
    WriteLog("running=",sql)
    sql="use wxg_data_valueless"
    res = tdw.execute(sql)

    #article
    sql = " SELECT * FROM wxy_sourceid_partition where taskid = '%s' and tag = 1"%taskid_
    res = tdw.execute(sql)
    if res is not  None:
        idx =0
        sql = '''insert table wxy_daily_game_uinlist
               SELECT
                    '%s' as taskid,
                    uin_ as uin ,
                    '1' as tag ,
                    '1' as flag,
                     1 as score
                FROM cdg_weixin::log_10377
                WHERE'''%(taskid_)
        for oneArticle in res:
            subList = oneArticle.split("\t")
            print subList
            articleis = subList[1].split("_")
            if len(articleis) < 3:
                print "<3"
                continue
            bizuin = articleis[0]
            msgid = articleis[1]
            itemidx = articleis[2]
            if bizuin == 'READ':
                continue
            if idx != 0:
                sql += " or "
            else:
                idx = idx + 1
            sql += "(bizuin_=%s and msgid_=%s and itemidx_=%s)"%(bizuin,msgid,itemidx)
        WriteLog("running=",sql)
        res = tdw.execute(sql)
        WriteLog("running=",sql)

    #biz
    sql = " SELECT * FROM bizuin_partition where ds = '%s' and tag = 0"%taskid_
    res = tdw.execute(sql)
    if res is not  None:
        idx =0
        sql = '''insert table wxy_daily_game_uinlist
               SELECT
                    '%s' as taskid,
                    useruin_ as uin ,
                    '0' as tag ,
                    '1' as flag,
                    1 as score
                FROM cdg_weixin::log_10296
                WHERE'''%(taskid_)
        for oneArticle in res:
            subList = oneArticle.split("\t")
            id = subList[0]
            if idx != 0:
                sql += " or "
            else:
                idx = idx + 1
            sql += "(bizuin_=%s )"%(id)
        WriteLog("running=",sql)
        res = tdw.execute(sql)

def TDW_PL(tdw, argv=[]):
    config(argv)
    print "hello TDW"
    get_article_click_uin(tdw)