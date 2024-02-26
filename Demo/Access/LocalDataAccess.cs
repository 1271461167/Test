using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Security.Cryptography;
using System.Threading;
using System.Windows.Documents;
using Demo.Common;
using Demo.Model;
using MySql.Data.MySqlClient;
namespace Demo.Access
{
    public class LocalDataAccess
    {
        private LocalDataAccess() { }
        public static LocalDataAccess Instance;
        public static LocalDataAccess GetInstance() 
        {
            if (Instance == null)
                Instance = new LocalDataAccess();
            return Instance;
        }
        MySqlConnection conn;
        MySqlCommand cmd;
        MySqlDataAdapter adapter;
        private void Dispose()
        {
            if (conn != null)
            {
                conn.Close();
                conn.Dispose();
            }
            if (cmd != null)
            {
                cmd.Dispose();
                cmd = null;
            }
            if (adapter != null)
            {
                adapter.Dispose();
                adapter = null;
            }
        }

        private bool DBConnection() 
        {
            string connStr = ConfigurationManager.ConnectionStrings["db"].ConnectionString;
            if (conn == null)
                conn = new MySqlConnection(connStr);
            try
            {
                conn.Open();
                return true;
            }
            catch (Exception)
            {
                return false;
            }            
        }
        public UserRole CheckUserInfo(MySqlParameter[] parameters)
        {
            if(DBConnection())
            {
                try
                {
                    Thread.Sleep(2000);
                    DataSet dataSet = new DataSet();
                    string sql = "select * from user where name=@username and password=@password";
                    cmd = new MySqlCommand();
                    cmd.CommandText = sql;
                    cmd.Connection = conn;
                    cmd.CommandType = CommandType.Text;                   
                    foreach(var parameter in parameters)
                    {
                        if(parameter.Value==null||parameter.Value.ToString()=="")
                        {
                           parameter.Value = DBNull.Value;
                        }
                        cmd.Parameters.Add(parameter);
                    }
                    
                    adapter=new MySqlDataAdapter(cmd);
                    adapter.Fill(dataSet);
                    UserRole role = new UserRole();
                    role.Identity = dataSet.Tables[0].Rows[0]["permission"].ToString();
                    role.Name = dataSet.Tables[0].Rows[0]["name"].ToString();
                    role.Password = dataSet.Tables[0].Rows[0]["password"].ToString();
                    role.Gender = int.Parse(dataSet.Tables[0].Rows[0]["gender"].ToString());
                    role.Avatar = dataSet.Tables[0].Rows[0]["avatar"].ToString();
                    role.Age = int.Parse(dataSet.Tables[0].Rows[0]["age"].ToString()); 
                    return role;
                }
                catch(Exception)
                {
                    throw new Exception("账户或密码错误");
                }
                finally { conn.Close(); }
            }
            else
                throw new Exception("数据库打开失败");
        }
        public List<SeriesModel> GetCourseSeries()
        {
            List<SeriesModel> models = new List<SeriesModel>();
            if (DBConnection())
            {
                try
                {
                    DataSet dataSet = new DataSet();
                    string sql = @"select a.course_id,a.course_name,c.plant_name,b.play_count,b.isgrowing,b.growrate 
                                    from course a 
                                    left join record b on a.course_id=b.course_id 
                                    left join plantforms c on b.plant_id=c.plant_id 
                                    order by a.course_id,c.plant_id";
                    cmd = new MySqlCommand();
                    cmd.CommandText = sql;
                    cmd.Connection = conn;
                    cmd.CommandType = CommandType.Text;
                    adapter = new MySqlDataAdapter(cmd);
                    adapter.Fill(dataSet);
                    string courseId = "";
                    SeriesModel model = null;
                    foreach(DataRow row in dataSet.Tables[0].Rows)
                    {
                        string tempId = row.Field<string>("course_name");
                        if(courseId!= tempId)
                        {

                        }
                    }

                    return models;

                }
                catch (Exception)
                {
                    throw new Exception("数据库连接失败");
                }
                finally { conn.Close(); }
            }
            else
                throw new Exception("数据库打开失败");
        }
    }
}
