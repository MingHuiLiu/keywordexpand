#include "index.h"
namespace wordexpand{	
	bool Sql::InitMysql(){
		mysql_init(&myCont);
		if(!mysql_real_connect(&myCont,"10.234.148.106","qspace","forconnect","stat",3306,NULL,0)){
			commom::LOG_INFO("connect mysql error");
			return false;
		}
		commom::DEBUG_INFO("init mysql ok");
		return true;
	}

	bool Sql::ExeQuery(string& query){
		//commom::DEBUG_INFO(query);
		if(mysql_query(&myCont, query.c_str())){  
			commom::LOG_INFO("Query Error:" + string(mysql_error(&myCont)));
			return false;
		}else{
			//检查插入结果
			commom::DEBUG_INFO("ExeQuery OK");
			return true;
		}
	}

	bool Sql::AddTask(std::map<string, string>& taskinfo){
		string strsql = "";
		strsql += "insert into keyword_task set ";
		strsql += ("task_id= " + taskinfo["task_id"]);
		strsql += (", task_version= "+ taskinfo["task_version"]);
		strsql += (", task_name= " + taskinfo["task_name"]);
		strsql += (", task_cname= " + taskinfo["task_cname"]);
		strsql += (", input= " + taskinfo["input"]);
		strsql += (", step= " + taskinfo["step"]);
		strsql += (", status= " + taskinfo["status"]);
		strsql += (", pack_name= " + taskinfo["pack_name"]);;
		strsql += (", done_name= " + taskinfo["done_name"]);
		strsql += (", user_num= " + taskinfo["user_num"]);
		strsql += (", biz_url=" + taskinfo["biz_url"]);
		strsql += (", creator=" + taskinfo["creator"]);
		strsql += (", modify_user= " + taskinfo["modify_user"]);
		strsql += (", create_time= " + taskinfo["create_time"]);
		strsql += (", modify_time= " + taskinfo["modify_time"]);
		//commom::DEBUG_INFO(strsql);
		return ExeQuery(strsql);
	}
	bool Sql::UpdataKeywords(std::vector<std::string>& v, std::map<string, string>& taskinfo){
		for(int i = 0; i< v.size(); i++){
			string strsql = "";
			strsql += "insert into keyword_word_filter_result set ";
			strsql += (" task_id= " + taskinfo["task_id"]);
			strsql += (", task_version= " + taskinfo["task_version"]);
			strsql += (", keyword= \"" + v.at(i));
			strsql += ("\", source= \"\"");
			strsql += (", score= 1");
			strsql += (", iteration= 1");
			strsql += (", delete_status = 0");
			strsql += (", modify_user= " + taskinfo["modify_user"]);
			strsql += (", modify_time= " + taskinfo["modify_time"]);
			//commom::DEBUG_INFO(strsql);
			ExeQuery(strsql);
		}	
		return true;
	}
	bool Sql::UpdataBiz(std::vector<bizinfo>& results,std::map<string, string>& taskinfo){
		for(int i = 0; i< results.size(); i++){
			string strsql = "";
			strsql += "insert into keyword_biz_filter_result set ";
			strsql += (" task_id= " + taskinfo["task_id"]);
			strsql += (", task_version= " + taskinfo["task_version"]);
			strsql += (", biz= \"" + results.at(i).uin);
			strsql += ("\", biz_title=\" " + results.at(i).bizname);
			strsql += ("\", biz_info= \"" + results.at(i).bizdesc);
			strsql += ("\", biz_auth=\"\"");
			strsql += (", biz_sub= 1");
			strsql += (", match_list = \"\"");
			strsql += (", type= 0");
			strsql += (", source_word=\"\"");
			strsql += (", source = \"\"");
			strsql += (", score= " + f.ConvertToStr(results.at(i).score));
			strsql += (", iteration = 1");
			strsql += (", delete_status = 0");
			strsql += (", modify_user= " + taskinfo["modify_user"]);
			strsql += (", modify_time= " + taskinfo["modify_time"]);
			//commom::DEBUG_INFO(strsql);
			ExeQuery(strsql);
		}	
		return true;
	}

	int Sql::GetTaskId(){
		int id = 0;
		string sqlstr = "select max(task_id) as task_id from keyword_task";
		ExeQuery(sqlstr);
		MYSQL_RES *result = mysql_use_result(&myCont);
		MYSQL_ROW row;  
		if(mysql_field_count(&myCont) != 1){
			commom::LOG_INFO("GetTaskId Error");
		}else{
			row = mysql_fetch_row(result);  
		}
		if(mysql_num_fields(result) != 1){
			commom::DEBUG_INFO("GetTaskId Error");
		}else{
			id =  atoi(row[0]);
		}
		mysql_free_result(result);  
		return id + 1;
	}

	bool Sql::AddTask(string taskid, string lzid,string& uinnumber){
		string strsql = "";
		strsql += "insert into lz_task_staue set ";
		strsql += ("taskid= " + taskid);
		strsql += (", ds= " + f.GetTime());
		strsql += (", lzid= " + lzid);
		strsql += (", uinnumber= " + uinnumber);
		strsql += (", staue= \"0\"");
		commom::DEBUG_INFO(strsql);
		return ExeQuery(strsql);
	}

	bool Sql::SelectTask(std::vector<taskstaue>& tasklist){
		//commom::DEBUG_INFO("SelectTask");
		int id = 0;
		string sqlstr = "select * from lz_task_staue where staue != \"4\"";
		ExeQuery(sqlstr);
		MYSQL_RES *result = mysql_use_result(&myCont);
		MYSQL_ROW row;  
		int j = mysql_num_fields(result);
		if(j != 5){
			commom::LOG_INFO("sql SelectTask error");
			return false;
		}
		taskstaue tmp;
		while(row=mysql_fetch_row(result)){
			tmp.taskid = row[0];
			tmp.ds = row[1];
			tmp.lzid = row[2];
			tmp.num = row[3];
			tmp.staue = row[4];
			tasklist.push_back(tmp);
		}
		return true;
	}

	bool Sql::UpdataTask(string taskid, string staue){
		//update MyClass set name='Mary' where id=1;
		string strsql = "";
		strsql += "update  lz_task_staue set ";
		strsql += ("staue=" + staue);
		strsql += (" where taskid =" + taskid);
		commom::DEBUG_INFO(strsql);
		return ExeQuery(strsql);
	}
}