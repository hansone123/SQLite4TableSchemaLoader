/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   newmain.cc
 * Author: hansone123
 *
 * Created on March 3, 2016, 8:37 PM
 */

#include <cstdlib>
#include <iostream>
#include <sqlite4.h>
#include <cstdio>
#include <vector>
#include <fstream>

using namespace std;

/*
 * 
 */
static void showArray(char* str, int length);
static int callback(void *data, int argc, sqlite4_value **argv,const  char **azColName);
static int callback2(void *data, int argc, sqlite4_value **argv,const  char **azColName);
static bool getTablesName(sqlite4 *db, vector<string> *tables_name);
static bool getTablesID(sqlite4 *db, vector<string> *tables_id);
static bool getTableSchema(sqlite4 *db, string tableName, vector<string> *tables_name);
static bool putSchemaToFile(string directoryPath, string id,string name,vector<string> *table_schema);

int main(int argc, char* argv[])
{
   sqlite4 *db;
   char *zErrMsg = 0;
   int rc;
   
   const char* databasePath;
   const char* directoryPath;
   if (argc == 3) {
       databasePath = argv[1];
       directoryPath = argv[2];
   }
   else {
       cout<<"Need two parameter: (first)databasePath  (second) output directoryPath"<<endl;
       return 0;
   }
   /* Open database */
   rc = sqlite4_open(0,databasePath, &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite4_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   
   vector<string> *tables_name = new vector<string>();
   getTablesName(db, tables_name);
   cout<<endl;
   
   
   /* Get tables ID*/
   vector<string> *tables_id = new vector<string>();
   getTablesID(db, tables_id);
   cout<<endl;
   
   /* Get table schema and put to file*/
   for (int i=0; i<tables_name->size(); i++) {
       
       vector<string> *table_schema = new vector<string>();
       string name = (*tables_name)[i];       
       getTableSchema(db, name, table_schema);
       cout<<name<<"'s schema:"<<endl;
        for (int j=0; j<table_schema->size(); j++) {
            cout<<(*table_schema)[j]<<endl;
        }
       putSchemaToFile(directoryPath, (*tables_id)[i], name, table_schema);
       delete table_schema;       
   }
   
   
   sqlite4_close(db, 0);
   
   
   
   return 0;
}
static bool getTablesName(sqlite4 *db, vector<string> *tables_name) {
    /*get tables name*/
   string SQL_getTablesName = "SELECT name FROM sqlite_master WHERE type = 'table'";
   
   int rc;
   char *zErrMsg = 0;
   rc = sqlite4_exec(db, SQL_getTablesName.c_str(), callback, tables_name);
   if( rc != SQLITE4_OK ){
      fprintf(stderr, "%s error: %s\n",SQL_getTablesName.c_str(), zErrMsg);
      return false;
   }
   cout<<"All tables name:"<<endl;
   for (int i=0; i<tables_name->size(); i++) {
        cout<<(*tables_name)[i]<<endl;
   }    
   return true;
}
static bool getTablesID(sqlite4 *db, vector<string> *tables_id) {
   string SQL_getTablesID = "SELECT rootpage FROM sqlite_master WHERE type = 'table'";
   int rc;
   char *zErrMsg = 0;
   rc = sqlite4_exec(db, SQL_getTablesID.c_str(), callback, tables_id);
   if( rc != SQLITE4_OK ){
      fprintf(stderr, "%s error: %s\n",SQL_getTablesID.c_str(), zErrMsg);
      return false;
   }
   cout<<"All tables ID:"<<endl;
   for (int i=0; i<tables_id->size(); i++) {
       cout<<(*tables_id)[i]<<endl;
   }
   return true;
}
static bool getTableSchema(sqlite4 *db, string tableName, vector<string> *tableSchema) {
    
   string SQL_getTablesSchema = "pragma table_info(" + tableName + ")";
   int rc;
   char *zErrMsg = 0;
   rc = sqlite4_exec(db, SQL_getTablesSchema.c_str(), callback2, tableSchema);
   if( rc != SQLITE4_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      return false;
   }
   return true;
}
static bool putSchemaToFile(string path, string id,string name,vector<string> *table_schema) {
    string filePath = path + "//" + id;
    fstream file;
    file.open(filePath.c_str(),ios::out);
    if(!file) {
        cout<<filePath<<" create failes"<<endl;
        return false;
    }
    file<<id<<endl;
    file<<name<<endl;
    for (int i=0; i<table_schema->size(); i++) {
        file<<(*table_schema)[i]<<endl;
    }
    
    return true;
}
static void showArray(char* str, int length) {
    for (int i=0; i<length; i++)
        printf("%c", str[i]);
    cout<<endl;
}

static int callback(void *data, int argc, sqlite4_value **argv,const  char **azColName) {
    
    vector<string> *output = (vector<string>*)data;
    for(int i=0; i<argc; i++){
                
        int byte;
        char *value = (char*)sqlite4_value_text(argv[i], &byte);
        string result = string(value);
        output->push_back(result);
    }
    
   return 0;
}

static int callback2(void *data, int argc, sqlite4_value **argv,const  char **azColName) {
    
    vector<string>* schema = (vector<string>*)data;
    string temp ="";
    for(int i=0; i<argc; i++){
                
        int byte;
        char *value = (char*)sqlite4_value_text(argv[i], &byte);
        if ( i == 1 ) {            
            temp.append(value);
        }
        if ( i == 2 ) {
            temp.append(",");
            temp.append(value);
        }
    }
    schema->push_back(temp);
   return 0;
}