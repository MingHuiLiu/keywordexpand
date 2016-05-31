#!/usr/bin/python2.6
# encoding: utf-8

import os
import time
import sys
import httplib
import json
import traceback
import urllib
import time
from urllib import quote
import random
################################################################################
##调用洛子接口，创建依赖任务
###############################################################################

class APIWrapper_LZ:
    def __init__(self):
        #共用字段
        self.Host_Port = "t.gri.oa.com:8081"
        #http://10.222.106.18:80/LService/LhotseTask
        self.Host_Port_ex = "http://10.204.28.91:8081"
        self.API_URL = '/LService/LhotseTask'

    def CallAPIPost(self, pst_url, body_params,bJson=False,host_port=""):
        try:
            headers = {"Content-Type":"application/x-www-form-urlencoded", "Accept": "text/plain"}
            if host_port == "": host_port = "10.222.106.18:80" #host_port = "lhotse-yh-appcgi:8081" #"10.136.4.90:8081"
            conn = httplib.HTTPConnection(host_port)

            if bJson:
                params = json.dumps(body_params)
            else:
                params = urllib.urlencode(body_params)

            conn.request(method="POST", url=pst_url, body=params, headers=headers)
            r = conn.getresponse()
            if (r.status == 200):
                print "|call api ok"
                print "\n***********respone: ", r.status, "****************\n"
            else:
                #mylog.MyErrorLog("|call api failed.%s.status: %d, reason: %s" %(host_port + pst_url, r.status, r.reason))
                print "\n***********respone error: ", r.status, "****************\n"
                return None

            #5.response information
            res = r.read()
            #print "##########\n ", res, "###########\n"

            #6.json parse
            resp = json.loads(res)
            #mylog.MyDebugLog("|call api %s response: %s"%(host_port + pst_url, res))
            #print "\n***********respone info: ", len(resp["keyword"]), "****************\n"

            #7.return result
            #print resp
            return resp
        except BaseException:
            #mylog.MyErrorLog("|CallAPIPost error:" + host_port + pst_url + "\n"  + traceback.format_exc())
            print traceback.format_exc()
            return None

    @staticmethod
    def GetDefaultLZTaskParams():
        #通用参数
        taskParams={}
        taskParams["taskName"]=""
        taskParams["taskType"]="75"
        taskParams["cycleUnit"]="O"
        taskParams["brokerIp"]="any"
        taskParams["cycleNum"]="1"
        taskParams["startDate"]="2015-01-01 10:00:00"
        taskParams["endDate"]="2999-01-01 10:00:00"
        taskParams["selfDepend"]="2"
        taskParams["taskAction"]=""
        taskParams["startHour"]="08"
        taskParams["startMin"]="30"
        taskParams["retriable"]="1"
        taskParams["notes"]="xxxxx"
        taskParams["inCharge"]="seanxywang"
        taskParams["sourceServer"]="hdfs_tl-if-nn-tdw.tencent-distribute.com"
        taskParams["targetServer"]="tdw_tl"
        taskParams["creater"]="seanxywang"
        taskParams["tryLimit"]="3"
        taskParams["bgId"]="8"
        taskParams["productId"]="304"
        taskParams["tdwAppGroup"]="g_cdg_weixin"#"g_cdg_weixin 此处不能改"
        taskParams["taskExt"]="taskExt"
        return taskParams

    ##分为两步:-------------------------------------------------------------
    ## 1.先创建写BizUin到TDW的入库任务（HDFS到TDW表）
    ## 2.再创建select公众号粉丝uin的数据出库任务（TDW出库到HDFS）
    ## 3.脚本文件查询check文件，如2已经完成，则写入到face系统,并返回face的URL
    ## 4.提醒用户任务已经完成-----------------------------------------------
    def RequestBiz2Uin(self):
        try:

            #解析参数，写入本地文件文件
            #取公众号和文章的ID
            '''
            bizList = tRequest["bizs"]
            articleList = tRequest["articles"]
            taskId = tRequest["taskId"]
            wt = file2local()
            (dir,filename) = wt.getfilename(taskId)
            fullfilename = dir + filename
            '''
            #taskId = str(random.randint(0,100));
            #获取此任务的唯一ID
            #taskId = str(taskId) + "_" + str(time.strftime('%Y%m%d%m%s',time.localtime(time.time())))
            taskId = "201605311972"
            dir = "./"
            filename = "testinput"
            #print filename
            #wt.Writebizs2local(bizList,fullfilename,taskId)
            #wt.WriteArticles2local(articleList,fullfilename,taskId)
            #print bizList
            #执行脚本文件,创建分区 ,在此之前需要创建表（biz_article_id bizuin_partition）

            #测试
            #self.PutLocalFileToHFDS(dir,filename);
            #CreateHDFSToTDWId = self.CreateHDFSToTDW(filename,add_partitionID);
            #getuinsID = self.getuins(CreateHDFSToTDWId,taskId);
            #taskParams = self.CreateTDW2HDFS(taskId);
            #return taskParams;

            #测试
            if(self.PutLocalFileToHFDS(dir,filename) == -1):
                return None
            add_partitionID = self.add_partition(taskId)
            if(add_partitionID == -1):
                print "add_partition error"
                return None
                #上传到hdfs'''
            #self.PutLocalFileToHFDS(dir,filename)

            #创建入库任务：HDFS到TDW表,
            print "BEGIN"
            #add_partitionID = ""
            print add_partitionID
            CreateHDFSToTDWId = self.CreateHDFSToTDW(filename,add_partitionID,taskId)
            if(CreateHDFSToTDWId == -1):
                print "CreateHDFSToTDWId error"
                return None
            #输出到临时表daily_res_keyword_uinlis

            getuinsID = self.getuins(CreateHDFSToTDWId,taskId)
            if(getuinsID == -1):
                print "getuins error"
                return None
            #taskParams = self.CreateTDW2HDFS(getuinsID,taskId)
            #if(taskParams == None):
                #mylog.MyDebugLog("CreateTDW2HDFS error")
                #return None
            return taskId

        except BaseException:
            ##mylog.MyErrorLog("|RequestBiz2Uin " + traceback.format_exc())
            print traceback.format_exc()
            return None

    ##分两步进行：
    ## 1.创建分区
    ## 2.将任务1的taskID作为父任务，创建写HDFS的任务
    def CreateHDFSToTDW(self, filename,parentid,taskId):
        try:
            #paramsList = "["
            #paramsList += "{'propName':'charSet','propValue':'UTF-8'}"
            #paramsList += ",{'propName':'databaseName','propValue':'cdg_weixin'}"
            #paramsList += ",{'propName':'delimiter','propValue':'9'}"
            #paramsList += ",{'propName':'failedOnZeroWrited','propValue':'0'}"
            #paramsList += ",{'propName':'loadMode','propValue':'TRUNCATE'}"
            #paramsList += ",{'propName':'partitionType','propValue':'P_${YYYYMMDD}'}"
            #paramsList += ",{'propName':'sourceColumnNames','bizuin,ds,flag'}"
            #paramsList += ",{'propName':'sourceFilePath','propValue':'hdfs://tl-if-nn-tdw.tencent-distribute.com:54310/stage/outface/wxg/g_cdg_weixin/bizuin2tdw/'}"
            #paramsList += "]"

            paramsList = []
            dcitExtParam = {}
            dcitExtParam["propName"] = "charSet"
            dcitExtParam["propValue"] = 'UTF-8'
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            dcitExtParam["propName"] = "databaseName"
            dcitExtParam["propValue"] = 'wxg_data_valueless'
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            dcitExtParam["propName"] = "delimiter"
            dcitExtParam["propValue"] = '9'
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            dcitExtParam["propName"] = "failedOnZeroWrited"
            dcitExtParam["propValue"] = '1'
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            dcitExtParam["propName"] = "loadMode"
            dcitExtParam["propValue"] = 'TRUNCATE'
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            #dcitExtParam["propName"] = "flag_task_partitionType"
            #dcitExtParam["propValue"] = 'P_${YYYYMMDD}'
            dcitExtParam["propName"] = "partitionType"
            dcitExtParam["propValue"] = 'P_'+ str(taskId)
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            dcitExtParam["propName"] = "sourceColumnNames"
            #dcitExtParam["propValue"] = 'taskid,id,tag,flag,score'
            dcitExtParam["propValue"] = 'taskid,id,tag,flag,score'
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            dcitExtParam["propName"] = "sourceFileNames"
            dcitExtParam["propValue"] = filename
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            dcitExtParam["propName"] = "sourceFilePath"
            dcitExtParam["propValue"] = '/stage/outface/wxg/g_cdg_weixin/seanxy/tdw/'
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            dcitExtParam["propName"] = "tableName"
            dcitExtParam["propValue"] = 'wxy_sourceid_partition'
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            dcitExtParam["propName"] = "tdw"
            dcitExtParam["propValue"] = 'tdw_tl'
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            dcitExtParam["propName"] = "targetColumnNames"
            dcitExtParam["propValue"] = 'taskid,id,tag,flag,score'
            paramsList.append(dcitExtParam)

            taskParams = APIWrapper_LZ.GetDefaultLZTaskParams()
            taskParams["parentTaskId"] ='{'+str(parentid)+':1}'
            taskParams["taskType"] = "75"
            #taskParams["taskName"] = "put_bizarticles2tdw(" + str(parentid) + ")"
            taskParams["taskName"] = "testesdwa1231a" + str(random.randint(0,100));
            taskParams["taskExt"] = json.dumps(paramsList)

            tRespone = self.CallAPIPost(self.API_URL, taskParams)

            #获取父任务ID,未获取到则返回失败
            print tRespone
            if(tRespone == None):return -1
            taskId = tRespone[0]["taskId"]
            if taskId == 0: return  -1
            return taskId
        except:
            #mylog.MyErrorLog("|CreateHDFSToTDW " + traceback.format_exc())
            print traceback.format_exc()
            return  -1
    def add_partition(self,taskId):
        try:
            paramsList = []
            dcitExtParam = {}
            dcitExtParam["propName"] = "file_name"
            dcitExtParam["propValue"] = 'add_source2uin_partition.py'
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            dcitExtParam["propName"] = "params"
            dcitExtParam["propValue"] = str(taskId)
            paramsList.append(dcitExtParam)

            #paramsList = "["
            #paramsList += "{'propName':'file_name','propValue':'add_source2uin_partition.py'}"
            #paramsList += ",{'propName':'params','propValue':'"
            #paramsList += str(taskId)
            #paramsList += "'}]"

            taskParams = APIWrapper_LZ.GetDefaultLZTaskParams()
            taskParams["taskType"] = "121"
            taskParams["taskName"] = "add_partition:(" + str(taskId) + ")"
            taskParams["taskExt"] = json.dumps(paramsList)

            tRespone = self.CallAPIPost(self.API_URL, taskParams)
            #获取父任务ID,未获取到则返回失败
            print tRespone
            if(tRespone == None):return -1
            taskId = tRespone[0]["taskId"]
            if taskId == 0: return -1
            return taskId
        except:
            #mylog.MyErrorLog("|add_partition " + traceback.format_exc())
            print traceback.format_exc()
            return -1

    def getuins(self,parentid,taskId):
        try:
            paramsList = []
            dcitExtParam = {}
            dcitExtParam["propName"] = "file_name"
            dcitExtParam["propValue"] = 'source2uin.py'
            paramsList.append(dcitExtParam)
            dcitExtParam = {}
            dcitExtParam["propName"] = "params"
            dcitExtParam["propValue"] = str(taskId)
            paramsList.append(dcitExtParam)

            #paramsList = "["
            #paramsList += "{'propName':'file_name','propValue':'source2uin.py'}"
            #paramsList += ",{'propName':'params','propValue':'"
            #paramsList += str(taskId)
            #paramsList += "'}]"
            taskParams = APIWrapper_LZ.GetDefaultLZTaskParams()
            taskParams["parentTaskId"] ='{'+str(parentid)+':1}'
            taskParams["taskType"] = "121"
            taskParams["taskName"] = "getuins(" + str(parentid) + ")"
            taskParams["taskExt"] = json.dumps(paramsList)

            tRespone = self.CallAPIPost(self.API_URL, taskParams)
            #获取父任务ID,未获取到则返回失败
            print tRespone
            if(tRespone == None):return -1
            taskId = tRespone[0]["taskId"]
            if taskId == 0: return -1
            return taskId
        except:
            #mylog.MyErrorLog("|getuins " + traceback.format_exc())
            print traceback.format_exc()
            return -1

    def RequestPack2Url(self, sStartDate, sEndDate, iPageCnt):
        try:
            if iPageCnt is None: return []
            reqParams = {}
            reqParams["startdate"] = sStartDate
            reqParams["enddate"] = sEndDate
            reqParams["page"] = 0
            reqParams["size"] = 100000
            print "\n***********RequestPack2Url: request ", iPageCnt, "****************\n"
            tRespone = self.CallAPIPost(self.pack2url, reqParams)
            print "\n***********RequestPack2Url: response ", tRespone["num"], "****************\n"
            if(tRespone == None):return -1
            return tRespone
        except BaseException:
            #mylog.MyErrorLog("|RequestPack2Url " + traceback.format_exc())
            print traceback.format_exc()
            return None

    def PutLocalFileToHFDS(self,dir,fileName):
        try:
            strToolPath = "/usr/local/hadoop-0.20.1-tdw.0.2/bin/hadoop"
            #strLocalPath = "/tmp/word2vec/bizuin_"
            strAccPwd = "-Dfs.default.name=hdfs://tl-if-nn-tdw.tencent-distribute.com:54310 -Dhadoop.job.ugi=tdw_seanxywang:tdw_seanxywang,g_cdg_weixin"
            strRemoteFilePath = "hdfs://tl-if-nn-tdw.tencent-distribute.com:54310/stage/outface/wxg/g_cdg_weixin/seanxy/tdw/"
            #创建文件夹
            #strComMKDir = "%s fs %s -mkdir %s"%(strToolPath, strAccPwd, strRemoteFilePath)
            #PUT文件到指定文件夹
            strComPut = "%s fs %s -put %s%s %s"%(strToolPath, strAccPwd, dir, fileName, strRemoteFilePath)
            #执行
            ##mylog.MyDebugLog("|PutLocalFileToHFDS: " + strComMove)
            #os.system(strComMove)
            ##mylog.MyDebugLog("|PutLocalFileToHFDS: " + strComMKDir)
            #os.system(strComMKDir)
            ##mylog.MyDebugLog("|PutLocalFileToHFDS: " + strComPut)
            os.system(strComPut)
            return 1
        except:
            #mylog.MyErrorLog("|PutLocalFileToHFDS error: " + traceback.format_exc())
            print traceback.format_exc()
            return -1
if __name__ == "__main__":
    AWrapperBiz = APIWrapper_LZ()
    resp = AWrapperBiz.RequestBiz2Uin()