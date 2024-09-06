#include <iostream>
#include <mysql.h>
#include <mysql_error.h>
#include <windows.h>

const char* USER="matteo";
const char* PW="password";
const char* DB ="myDB";
const char* HOST = "localhost";

using namespace std;

class Login(){
    public:
      Login():userId(""),password("");

      void setUserId(const newUserId){
        userId = newUserId;
      }

      void setPassword(const newPassword){
        password = newPassword;
      }

      std::string getUserId() const{
        return userId;
      }

      std::string getPassword() const{
        return password;
      }

    private:
      std::string userId;
      std::string password;
};

char encrypthCh(char ch, int shift){
    if(isalpha(ch)){
        char base = isupper(ch) ? 'A' :'a';
        char encrypted_ch = (ch-base+shift+26)%26+base;
    return encrypted_ch;
    }
    if(isdigit(ch)){
        char encrypted_ch = (ch-'0'+shift+10)%10+'0';
        return encrypted_ch;
    }
    return ch;

}


string encrypt(const string& password, int shift){
    string encrypted_password;
    for(char ch : password){
        encrypted_password += encrypthCh(ch, shift);
    }
    return encrypted_password;
}

char decryptCh(char ch, int shift){
    if(isalpha(ch)){
        char base = isupper(ch) ? 'A' :'a';
        char decrypted_ch = (ch-base-shift+26)%26+base;
        return decrypted_ch;
    }else if(isdigit(ch)){
        char decrypted_ch = (ch-'0'-shift+10)%10+'0';
        return decrypted_ch;
    }else{
        return ch;
    }
}

string decrypt(const string& encrypted, int shift){
     string decrypted = " ";
     for(int i=0; i<decrypted.length();i++){
        char ch = encrypted[i];
        char decryptedChar = decrypt(ch,shift);
        decrypted += decryptedChar;
     }
     return decrypted;
}

string DBpw(MYSQL* conn, const string& id){
    string encryptedPw;

    string query = "SELECT Password FROM password HWERE Id = '"+id"'";
    if(mysql_error(conn, query.c_str())){
        cout<<"error:"<<mysql_error(conn)<<endl;
    }else{
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        encryptedPw = row[0];
        mysql_free_result(res);
    }
}



int main(){

    Login login;
    MYSQL* conn;
    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn,HOST,USER,PW,DB,3306,NULL,0)){
        cout<<"error"<<mysql_error(conn)<<endl;
    }else{
        cout<<"logged in database"<<endl;
    }
    Sleep(3000);
    int shift = 3;
    bool exit = false;
    while(!exit){
        system("cls");
        cout<<"1. Signup"<<endl;
        cout<<"2.Login"<<endl;
        cout<<"0.Exit"<<endl;
        cout<<"Enter yout choice:";
        int val;
        cin>>val;

        if(val==1){
            system("cls");
            string id,pw;
            cout<<"Enter ID For Signup:";
            cin>>id;
            login.setId(id);
            cout<<"Enter Password For Signup:";
            cin>>pw;
            login.setPassword(pw);

            string encryptedPW = encrypt(login.getPassword(),shift);

            string query = "INSERT INTO password (Id, PW) VALUES ('"+login.getId()+"','"+login.getPassword()+"')";
            if(mysql_error(conn,query.c_str())){
                cout<<"Error"<<mysql_error(conn)<<endl;
            }else{
                cout<<"Signup Successful"<<endl;
            }
            Sleep(3000);
        }else if(val==2){
            system("cls");
            cout<<"Enter Id:";
            cin>>id;
            cout<<"Enter password";
            cin>>pw;
            string getDB = DBpw(conn,id);
            if(!getDB.empty()){
                string decryptedPW = decrypt(getDB, shift);
                if(decryptedPW==pw){
                cout<<"Login Successful"<<endl;
                
            }else{
                cout<<"Incorrect Password"<<endl;
            }
            }else{
                cout<<"User not found"<<endl;
            }
            Sleep(3000);
        }else if(val==0){
            exit = true;
            cout<<"Exiting..."<<endl;
            Sleep(3000);
        }else{
            cout<<"Invalid choice"<<endl;
            Sleep(3000);
        }
    }
    mysql_close(conn);
    return 0;
}
