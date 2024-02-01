using System;
using System.Configuration;
using System.Data;
using System.Threading;
using Demo.Common;
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
    }
}
