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

    sql="""alter table  wxy_daily_game_uinres drop partition (p_%s)"""%(taskid_)
    res = tdw.execute(sql)
    sql="""alter table wxy_daily_game_uinres add partition p_%s values in (%s)""" %(taskid_, taskid_)
    res = tdw.execute(sql)
    WriteLog("running=",sql)
    sql="use wxg_data_valueless"
    res = tdw.execute(sql)

   #article

    sql = '''INSERT table wxy_daily_game_uinlist
              SELECT
 	            a.taskid as taskid,
 	            b.uin as uin,
 	           '1' as tag,
           	   '1' as flag,
 	            1 as score
             FROM
               (SELECT * FROM wxy_sourceid_partition where taskid = '%s' and tag = '1') a
             JOIN
                wxg_data_valueless::wxy_gamearticle_read b
             ON
                a.id = concat(ltrim(b.bizuin_),'_',ltrim(b.appmsgid_),'_',ltrim(b.itemidx_))
             where b.ds = "20160511"
          '''%taskid_
    WriteLog("running=",sql)
    res = tdw.execute(sql)

    #biz
    sql = '''INSERT table wxy_daily_game_uinlist
              SELECT
 	            a.taskid as taskid,
 	            b.fuser_uin as uin,
 	           '0' as tag,
           	   '1' as flag,
 	            1 as score
             FROM
               (SELECT * FROM wxy_sourceid_partition where taskid = '%s' and tag = '0') a
             JOIN
                cdg_weixin_secret::daily_raw_weixin_biz_user_relations b
             ON
                 a.id = b.fbiz_uin
             where b.fdate_cd = "20160529"
          '''%taskid_
    WriteLog("running=",sql)
    res = tdw.execute(sql)
    """
    sql = " SELECT * FROM wxy_sourceid_partition where taskid = '%s' and tag = 1"%taskid_
    res = tdw.execute(sql)
    WriteLog("running=",sql)
    if len(res) > 1:
        idx =0
        sql = '''insert table wxy_daily_game_uinlist
               SELECT
                    '%s' as taskid,
                    uin as uin ,
                    '1' as tag ,
                    '1' as flag,
                    1 as score
                FROM wxg_data_valueless::wxy_article_read
                WHERE'''%(taskid_)
        sql += "("
        for index in range(len(res) - 1):
            subList = res[index].split("\t")
            print subList
            articleis = subList[1].split("_")
            if len(articleis) < 3:
                print "<3"
                continue
            bizuin_ = articleis[0]
            appmsgid_ = articleis[1]
            itemidx_ = articleis[2]
            if idx != 0:
                sql += " or "
            else:
                idx = idx + 1
            sql += "(bizuin_=%s and appmsgid_=%s and itemidx_=%s)"%(bizuin_,appmsgid_,itemidx_)
        sql += ") and ds = '20160511'"
        WriteLog("running=",sql)
        res = tdw.execute(sql)
    """
    #restable wxy_daily_game_uinres
    sql = '''INSERT OVERWRITE TABLE wxy_daily_game_uinres
              SELECT
 	            uin as uin,
 	            tag as tag,
 	            flag as flag,
 	            count(1) as score
             FROM
                wxy_daily_game_uinlist
             where taskid ='%s' group by uin,tag
          '''%taskid_
    WriteLog("running=",sql)
    res = tdw.execute(sql)

def TDW_PL(tdw, argv=[]):
    config(argv)
    print "hello TDW"
    get_article_click_uin(tdw)