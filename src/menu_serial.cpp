#include "menu_serial.h"

void adriToolsv2_serialReadItem::item_add(   
        String      name,
        const char* key,
        String      ret,
        at_srFunc   f 
    )
{
    _name       = name;
    _key        = key;
    _ret        = ret;
    _function   = f;
}


adriToolsv2_serialRead * adriToolsv2_serialRead_ptr;
adriToolsv2_serialRead * adriToolsv2_serialReadPtr_get(){
    return adriToolsv2_serialRead_ptr;
}
adriToolsv2_serialRead::adriToolsv2_serialRead(){
    adriToolsv2_serialRead_ptr = this;
}
void adriToolsv2_serialRead::menu(){
    fsprintf("\n[adriToolsv2_serialRead::menu]\n");
    for (int i = 0; i < _cmd_1_cnt; ++i) {
        fsprintf("[%5d][%25s][%-15s]\n", 
                    i, 
                    _cmd_1_Array[i]._name.c_str(), 
                    _cmd_1_Array[i]._key
                );
    }
    fsprintf("\n[!]\n");
    for (int i = 0; i < _cmd_2_cnt; ++i) {
        fsprintf("[%5d][%25s][%-15s]\n", 
                    i, 
                    _cmd_2_Array[i]._name.c_str(), 
                    _cmd_2_Array[i]._key
                );
    } 
    if (_cmd_3_desc!="") { fsprintf("\n[%s][%s]\n", _cmd_3_sep, _cmd_3_desc.c_str()); } 
    if (_cmd_4_desc!="") { fsprintf("\n[%s][%s]\n", _cmd_4_sep, _cmd_4_desc.c_str()); }                  
}
void adriToolsv2_serialRead::cmd_array(int pos, int cnt) {
    switch (pos){
        case 1: 
            _cmd_1_Array = new adriToolsv2_serialReadItem[cnt];
        break;
        case 2: 
            _cmd_2_Array = new adriToolsv2_serialReadItem[cnt];
        break;
    }
}


void adriToolsv2_serialRead::cmd_item_add(int pos, String name, const char* key, String ret, at_srFunc f){
    switch (pos){
        case 1: 
            _cmd_1_Array[_cmd_1_cnt].item_add(name, key, ret, f);
            _cmd_1_cnt++;
        break;
        case 2: 
            _cmd_2_Array[_cmd_2_cnt].item_add(name, key, ret, f);
            _cmd_2_cnt++;
        break;
    }
}
void adriToolsv2_serialRead::cmd_3(char* sep, String desc, at_srFunc f){
    _cmd_3_sep  = sep;
    _cmd_3_desc = desc;
    _cmd_3      = f;
}
void adriToolsv2_serialRead::cmd_4(char* sep, String desc, at_srFunc f){
    _cmd_4_sep  = sep;
    _cmd_4_desc = desc;
    _cmd_4      = f;    
}

void adriToolsv2_serialRead::loop(){

    if(Serial.available()) { 
        boolean next = true;

        String a = Serial.readStringUntil('\n');

        String cmd = "";
        String value = "";  
        static String lastMsg = "";

        if (a.indexOf("^")>=0)a=lastMsg;
        lastMsg = a;

        if (a.indexOf("!")>=0){
            splitText(a, "!", cmd,  value) ; 
            for (int i = 0; i < _cmd_2_cnt; ++i) {
                if (cmd == _cmd_2_Array[i]._key) {
                    _cmd_2_Array[i]._function(cmd, value);
                    break;
                }
            } 
            next = false;       
        }       
        if (!next) return;
        if (_cmd_3_sep != (char *)"") {
            if (a.indexOf(_cmd_3_sep)>=0) {
                 splitText(a, _cmd_3_sep, cmd,  value) ; 
                _cmd_3(cmd, value);
                return;
            }   
        }   
        if (_cmd_4_sep != (char *)"") {
            if (a.indexOf(_cmd_4_sep)>=0) {
                 splitText(a, _cmd_4_sep, cmd,  value) ; 
                _cmd_4(cmd, value);
                return;
            }                            
        } 
        if (next) {
            for (int i = 0; i < _cmd_1_cnt; ++i) {
                if (a.indexOf(_cmd_1_Array[i]._key)>=0 ) {
                    _cmd_1_Array[i]._function("", "");
                }
            }                        
        }        
        
    }       
}
int adriToolsv2_serialRead::splitText(String A_readString, const char* sep, String & cmd, String & value) {

    String  s_command;
    String  s_valueCommand;
    String  s_readString = A_readString;
    char    inputChar[s_readString.length() + 1] ;
            s_readString.toCharArray(inputChar, s_readString.length() + 1);
    char    * Rcommand  = strtok(inputChar, sep);

    while (Rcommand != 0){          
        char* separator  = strchr(Rcommand, '=');
        if (separator != 0) {
            *separator  = 0;            
            ++separator ;
            s_command = String(Rcommand);
            s_valueCommand = String(separator); 
            
            if (s_command!=""){
                value = s_valueCommand;
                cmd = s_command;
                break;
            }
        }
        Rcommand = strtok(0, sep); 
    }

    return 0;
}
