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

    sql="""alter table  wxy_daily_game_active_uin drop partition (p_%s)"""%(taskid_)
    res = tdw.execute(sql)
    sql="""alter table wxy_daily_game_active_uin add partition p_%s values in (%s)""" %(taskid_, taskid_)
    res = tdw.execute(sql)
    WriteLog("running=",sql)

    sql="use wxg_data_valueless"
    res = tdw.execute(sql)

    #biz
    sql = '''INSERT table wxy_daily_game_uinlist
              SELECT
                a.taskid as taskid,
                b.fuser_uin as uin,
               '1' as tag,
                 '1' as flag,
                1 as score
             FROM
               (SELECT * FROM wxy_sourceid_partition where taskid = '%s' and tag = '0') a
             JOIN
                (SELECT * FROM cdg_weixin_secret::daily_raw_weixin_biz_user_relations where fdate_cd = "20160529") b
             ON
                 a.id = b.fbiz_uin
          '''%taskid_
    WriteLog("running=",sql)
    res = tdw.execute(sql)
    

    #room
    sql = '''INSERT table wxy_daily_game_uinlist
              SELECT
                 a.taskid as taskid,
                 b.fuin as uin,
                '2' as tag,
                '1' as flag,
                 1 as score
             FROM
               (SELECT * FROM wxy_sourceid_partition where taskid = '%s' and tag = '2') a
             JOIN
                wxg_data_valueless::wxy_room_game_member  b
             ON
                 a.id = b.froom_id
             where b.fdate_cd = "20160701"
          '''%taskid_
    WriteLog("running=",sql)
    res = tdw.execute(sql)


    #restable wxy_daily_game_uinres
    sql = '''INSERT TABLE wxy_daily_game_uinres
              SELECT
               '%s' as taskid,
                uin as uin,
                tag as tag,
                count(1) as score
             FROM
                wxy_daily_game_uinlist where (taskid = '%s' ) group by uin,tag
          '''%(taskid_, taskid_)
    WriteLog("running=",sql)
    res = tdw.execute(sql)

    #filter
    sql = '''INSERT TABLE wxy_daily_game_active_uin
              SELECT
                a.taskid as taskid,
                a.uin as uin,
                cast((TO_NUMBER(a.tag) + 10*b.fgame) as STRING) as tag,
                a.score + b.fgame as score
             FROM
                (select '%s' as taskid,uin as uin ,cast(SUM(TO_NUMBER(tag))as STRING) as tag, SUM(score) as score from wxy_daily_game_uinres where taskid = '%s' GROUP BY uin ) a
                join wxy_monthly_active_gameuser_uin_new b
             ON
                a.uin = b.fuin
             where
                b.fdate_cd = '201604'
          '''%(taskid_, taskid_)
    WriteLog("running=",sql)
    res = tdw.execute(sql)

def TDW_PL(tdw, argv=[]):
    config(argv)
    print "hello TDW"
    get_article_click_uin(tdw)