#include "logger.h"


char adriLogger_buffer1[1024];
// char adriLogger_buffer2[1024];


//################# adriTools_logger_variables
//################################################
//
// region ################################################  
adriTools_logger_variables::adriTools_logger_variables(String name){
    _name = name;
    ADRI_LOGV(-1, 2, 2, _state, "%s", _name.c_str());
}
void adriTools_logger_variables::toggle(){
    _state = !_state;
    ADRI_LOGV(-1, 2, 2, _state, "%s", _name.c_str());    
}   
void adriTools_logger_variables::toggleAddLine(){
    _addLine = !_addLine;
    ADRI_LOGV(-1, 2, 2, _addLine, "%s", _name.c_str());    
}   
boolean adriTools_logger_variables::statuAddLine(){
    return _addLine;
}  
boolean adriTools_logger_variables::statu(){
    return _state;
}  
// endregion >>>> 



//################# adriTools_logger
//################################################
//
// region ################################################ consctructor
adriTools_logger * adriTools_logger_ptr = nullptr;
adriTools_logger * adriTools_logger_ptrGet(){
    if (adriTools_logger_ptr == nullptr) adriTools_logger_ptr = new adriTools_logger();
    return adriTools_logger_ptr;
}
adriTools_logger::adriTools_logger(){
    adriTools_logger_ptr = this;
    spiff_setup();
}    
// endregion >>>> consctructor

// region ################################################ activateByVariable
void adriTools_logger::activateByVariable_print(){
    for (int i = 0; i < _activateByVariable_cnt; ++i)
    {
        ADRI_LOG(-1, 2, 0, "[%-5d][%S]", i, _activateByVariable[i]->_name.c_str());
    }
}
void adriTools_logger::activateByVariable_add(String name){
    ADRI_LOG(-1, 2, 2, "pos:%d - name:%s", _activateByVariable_cnt, name.c_str());
    _activateByVariable[_activateByVariable_cnt] = new adriTools_logger_variables(name);
    _activateByVariable_cnt++;
}
void adriTools_logger::activateByVariable_toggle(int pos){
    _activateByVariable[pos]->toggle();    
} 
void adriTools_logger::activateByVariable_toggleAddLine(int pos){
    _activateByVariable[pos]->toggleAddLine();    
} 
boolean adriTools_logger::activateByVariable_stau(int pos){
    return _activateByVariable[pos]->statu();
} 
boolean adriTools_logger::activateByVariable_statuAddLine(int pos){
    return _activateByVariable[pos]->statuAddLine();
}     
// endregion >>>> activateByVariable

// region ################################################ tools
String adriTools_logger::timeStamp()
{
  char result[20];
  snprintf(result, 20, "%lu", millis());
  return String(result);
}
int adriTools_logger::strContains(char str, const char *sfind) {
    int result = 0;
    int len = strlen(sfind);
    for (int i = 0; i < len; ++i) {   
        if ( sfind[i] == str ) result++;

    }   
    return result; 
}    
// endregion >>>> tools

// region ################################################ display mod && lvl
/*
    lvl from ADRI_LOG
        0 start with end
        1 end
        2 random
        3 in 1
    lvl from adriTools_logger
        0   all
        1   all + only header bt       
        2   all + only header t       
        3   only region
        4   only region + only header bt
        5   only region + only header t
        6   no region
        7   no region + only header bt
        8   no region + only header t
        9   only bt
        10  only t

*/
boolean adriTools_logger::printer_displayMod(int actif, int lvl){
    if (!printer_display) return false;

    if (actif >= 0) {
        if (!adriTools_logger_ptrGet()->activateByVariable_stau(actif)) return false;

        if (printer_display_lvl == 6) return false;
        if (printer_display_lvl == 7) {
            if (lvl == -1) return true;
            if ((lvl != 0) && (lvl != 1))   return false;
        }
        if (printer_display_lvl == 8) {
            if (lvl == -1) return true;
            if (lvl!=0) return false;
        } 
    } else {
        if (printer_display_lvl == 3) return false;
        if (printer_display_lvl == 4) {
            if (lvl == -1) return true;
            if ((lvl != 0) && (lvl != 1)) return false;
        }
        if (printer_display_lvl == 5) {
            if (lvl == -1) return true;            
            if (lvl!=0) return false;
        }        
    }
    if (actif == -2) return false; 

    if (printer_display_lvl == 9) {
        if (lvl == -1) return true;  
        if ((lvl != 0) && (lvl != 1)) return false;
    }
    if (printer_display_lvl == 10) {
        if (lvl == -1) return true;  
        if (lvl!=0) return false;
    } 

    return true;
}    

void adriTools_logger::printer_display_lvl_set(int lvl){
    printer_display_lvl = lvl;
    ADRI_LOGV(-1, 2, 2, printer_display_lvl, "", "");  
}    
// endregion >>>> display mod

// region ################################################ toggle
void adriTools_logger::printer_timestamp_toggle(){
    printer_timestamp = !printer_timestamp;
    ADRI_LOGV(-1, 2, 2, printer_timestamp, "", "");  
}
void adriTools_logger::printer_heap_toggle(){
    printer_heap = !printer_heap;
    ADRI_LOGV(-1, 2, 2, printer_heap, "", "");  
}
void adriTools_logger::printer_display_toggle(){
    printer_display = !printer_display;
    ADRI_LOGV(-1, 2, 2, printer_display, "", "");  
}
void adriTools_logger::spiff_toggle(){
    spiff_sav = !spiff_sav;
    ADRI_LOGV(-1, 2, 2, spiff_sav, "", ""); 	
}
void adriTools_logger::spiff_addLine_otherLvl_toggle(){
    spiff_addLine_otherLvl = !spiff_addLine_otherLvl;
    ADRI_LOGV(-1, 2, 2, spiff_addLine_otherLvl, "", "");     
}
// endregion >>>> toggle

// region ################################################ menu
void adriTools_logger::serial_menu() {
    adri_toolsV2 * _ptr = adri_toolsv2Ptr_get();
    if (_ptr->tempStrArray != nullptr) delete[] _ptr->tempStrArray;
    int size = 23;
    _ptr->tempStr_sizeSet(size);
    _ptr->tempStrArray   	= new String[size];

    _ptr->tempStrArray[0]	= "\nMenu\n";
    _ptr->tempStrArray[1]	= _ptr->info_parm(F("a"),     F("menu"), 10);
    _ptr->tempStrArray[2]	= _ptr->info_parm(F("z"),     F("printer_display_toggle"), 10);
    _ptr->tempStrArray[3]	= _ptr->info_parm(F("e"),     F("printer_heap_toggle"), 10);
    _ptr->tempStrArray[4]	= _ptr->info_parm(F("r"),     F("printer_timestamp_toggle"), 10);
    _ptr->tempStrArray[5]	= _ptr->info_parm(F("t"),     F("spiff activate"), 10);
    _ptr->tempStrArray[6]	= _ptr->info_parm(F("y"),     F("spiff read"), 10);
    _ptr->tempStrArray[7] 	= _ptr->info_parm(F("u"),     F("spiff remove"), 10);

    _ptr->tempStrArray[8]	= "\nPrinter lvl\n";
    _ptr->tempStrArray[9]	= _ptr->info_parm(F("0"),    F("all"), 10);
    _ptr->tempStrArray[10]	= _ptr->info_parm(F("1"),    F("all + only header bt "), 10);
    _ptr->tempStrArray[11]	= _ptr->info_parm(F("2"),    F("all + only header t "), 10);
    _ptr->tempStrArray[12]	= _ptr->info_parm(F("3"),    F("only region"), 10);
    _ptr->tempStrArray[13]	= _ptr->info_parm(F("4"),    F("only region + only header bt"), 10);
    _ptr->tempStrArray[14]	= _ptr->info_parm(F("5"),    F("only region + only header t"), 10);
    _ptr->tempStrArray[15]	= _ptr->info_parm(F("6"),    F("no region"), 10);
    _ptr->tempStrArray[16]	= _ptr->info_parm(F("7"),    F("no region + only header bt"), 10);
    _ptr->tempStrArray[17]	= _ptr->info_parm(F("8"),    F("no region + only header t"), 10);
    _ptr->tempStrArray[18]	= _ptr->info_parm(F("9"),    F("only bt"), 10);
    _ptr->tempStrArray[19]	= _ptr->info_parm(F("10"),   F("only t"), 10);
    _ptr->tempStrArray[20]   = "\nMenu\n";
    _ptr->tempStrArray[21]  = _ptr->info_parm(F("i"),    F("spif toggle -1"), 10);
    _ptr->tempStrArray[22]  = _ptr->info_parm(F("o"),    F("print region"), 10);
    _ptr->tempStr_print();        
}    
// endregion >>>> menu

// region ################################################ printer line
void esp_log_print_lines(boolean addLine, int actif, int lvl, int mod, String pLine, String func, String var, const char *tag ){
    adriTools_logger * _looger = adriTools_logger_ptrGet();
    String  pTag        = String(tag);
    int     searchCR    = 0;
    searchCR = _looger->strContains('\n', tag);    
    String  varP        = var;
    String  title       = "";

    switch (lvl) {
        case 0:
            title = "[<<START>>]";
        break;
        case 1:
            title = "[<<END>>]";
        break;
        default:break;
    }    

    if (pLine != "") {
        if (pTag != "")     {
            String cr = "";
            int len = strlen(tag);
            if (searchCR > 0)   {
                if ((pTag.substring(len-1) == "\n") && (pTag.substring(0, 1) == "\n")  ) {
                    pTag.remove(len-1, 1);
                    pTag.remove(0, 1);
                    cr = "\n";
                } else {
                    if (pTag.substring(0,1)     == "\n") {pTag.remove(0, 1);}
                    if (pTag.substring(len-1)   == "\n") {pTag.remove(len-1, 1);}
                    cr = "\n";
                }
                if (varP != "") {
                    fssprintf(adriLogger_buffer1, "[%-97s][%-20s]%s%s\t[%s]", 
                            pLine.c_str(), 
                            varP.c_str(),
                            title.c_str(), 
                            cr.c_str(),
                            pTag.c_str()
                        );
                } else {
                    fssprintf(adriLogger_buffer1, "[%-97s]%s%s\t[%s]", 
                            pLine.c_str(), 
                            title.c_str(), 
                            cr.c_str(),
                            pTag.c_str()
                        );                    
                } 
            }  else {
                if (varP != "") {
                    fssprintf(adriLogger_buffer1, "[%-97s][%-20s]%s[%s]", 
                            pLine.c_str(), 
                            varP.c_str(),
                            title.c_str(), 
                            pTag.c_str()
                        ); 
                } else {
                    fssprintf(adriLogger_buffer1, "[%-97s]%s[%s]", 
                            pLine.c_str(), 
                            title.c_str(), 
                            pTag.c_str()
                        ); 
                }
            }
        } else {
            if (varP != "") {fssprintf(adriLogger_buffer1, "[%-97s][%-20s]%s", pLine.c_str(), varP.c_str(), title.c_str()); }
            else {fssprintf(adriLogger_buffer1, "[%-97s]%s", pLine.c_str(), title.c_str()); }
        }       
        
    }
    else {

        if (pTag != "")     {
            String cr = "";
            int len = strlen(tag);
            if (searchCR > 0)   {
                if ((pTag.substring(len-1) == "\n") && (pTag.substring(0, 1) == "\n")  ) {
                    pTag.remove(len-1, 1);
                    pTag.remove(0, 1);
                    cr = "\n";
                } else {
                    if (pTag.substring(0,1)     == "\n") {pTag.remove(0, 1);}
                    if (pTag.substring(len-1)   == "\n") {pTag.remove(len-1, 1);}
                    cr = "\n";
                }
                if (varP != "") {
                    fssprintf(adriLogger_buffer1, "[%-20s]%s%s\t[%s]", 
                            varP.c_str(),
                            title.c_str(), 
                            cr.c_str(),
                            pTag.c_str()
                        );
                } else {
                    fssprintf(adriLogger_buffer1, "%s%s\t[%s]", 
                            title.c_str(), 
                            cr.c_str(),
                            pTag.c_str()
                        );                    
                } 
            }  else {
                if (varP != "") {
                    fssprintf(adriLogger_buffer1, "[%-20s]%s[%s]", 
                            varP.c_str(),
                            title.c_str(), 
                            pTag.c_str()
                        ); 
                } else {
                    fssprintf(adriLogger_buffer1, "%s[%s]", 
                            title.c_str(), 
                            pTag.c_str()
                        ); 
                }
            }
        } else {
            if (varP != "") {fssprintf(adriLogger_buffer1, "[%-20s]%s", varP.c_str(), title.c_str()); }
            else {fssprintf(adriLogger_buffer1, "%s",  title.c_str()); }
        } 
    }

    _looger->printer_serial(addLine, actif, adriLogger_buffer1 );
}
// endregion >>>> printer line

void adriTools_logger::printer_serial(boolean addLine, int actif, char * buffer){
    String          result  = String(buffer);
    unsigned int    len     = result.length();
    char            bufferPrint[len + 255];
    int             heap    = ESP.getFreeHeap();
    String          cTtime  = timeStamp();

    if ((printer_timestamp) && (printer_heap)) {
        fssprintf(bufferPrint, "[%-8s][%-8d]%s", cTtime.c_str(), heap, buffer);
    }
    else {
        if (printer_timestamp)  fssprintf(bufferPrint, "[%-8s]%s", cTtime.c_str(), buffer);
        if (printer_heap)       fssprintf(bufferPrint, "[%-8d]%s", heap, buffer);
    } 

    if (spiff_sav && addLine) {
        if (actif >= 0) {
             if (adriTools_logger_ptrGet()->activateByVariable_statuAddLine(actif)) {spiff_addLine(String(bufferPrint));}
        } else {
            if (spiff_addLine_otherLvl) spiff_addLine(String(bufferPrint));
        }
    }

    fsprintf("%s\n", bufferPrint);
}

// region ################################################ esp_log_printf_
void  esp_log_printf_(int actif, int lvl, int mod, String line, String func, String var, const char *tag) {
    if (!adriTools_logger_ptrGet()->printer_displayMod(actif, lvl)) return;
    String pLine = "";
    switch (mod) {
        case 1:
            pLine = line;
            break;
        case 2:
            pLine = line + " " + func;       
            break;
        default:
            break;
    }
    esp_log_print_lines(true, actif, lvl, mod, pLine, func, var, tag );
}
void  esp_log_printf_(int actif, int lvl, int mod, String line, String func, const char *tag) {
    if (!adriTools_logger_ptrGet()->printer_displayMod(actif, lvl)) return;
    String pLine = "";
    switch (mod) {
        case 1:
            pLine = line;
            break;
        case 2:
            pLine = line + " " + func;       
            break;
        default:
            break;
    }
    esp_log_print_lines(true, actif, lvl, mod, pLine, func, (char*)"", tag );
} 
void  esp_log_printf_(boolean addLine, int actif, int lvl, int mod, String line, String func, const char *tag) {
    if (!adriTools_logger_ptrGet()->printer_displayMod(actif, lvl)) return;
    String pLine = "";
    switch (mod) {
        case 1:
            pLine = line;
            break;
        case 2:
            pLine = line + " " + func;       
            break;
        default:
            break;
    }
    esp_log_print_lines(addLine, actif, lvl, mod, pLine, func, (char*)"", tag );
}     
// endregion >>>> 

// region ################################################ SPIFF

void adriTools_logger::spiff_setup(){


	ADRI_LOG(-1, 0, 2,"","");

    // Dir dir = LittleFS.openDir(spiff_folder);

    // int totalFile = 0;
    // while (dir.next()) {
    //     if (dir.isDirectory()) {

    //     } else  {
    //     	totalFile++;
    //     }
    // }

    // spiff_fileName = String(totalFile+1) + ".txt";

	spiff_fileName = "log.txt";

	spiff_addLine("\n");
	adri_toolsV2 * _tools = adri_toolsv2Ptr_get();
	_tools->ESP_boot_info();
	int size = _tools->tempStrArraySize;
	for (int i = 0; i < size; ++i)
	{
		spiff_addLine(_tools->tempStrArray[i]);
	}

	ADRI_LOG(-1, 1, 2,"","");
}   

void adriTools_logger::spiff_get(String & ret){
	// ADRI_LOG(-1, 0, 2,"","");

    String lines = "";

    File file = LittleFS.open(spiff_folder+"/"+spiff_fileName, "r");

    if (file) {
        while (file.position()<file.size()) {
            String xml = file.readStringUntil('\n');
            if (xml != "") {
				lines += xml + "\n";
            }
        }
        file.close(); 
    }

    ret = lines;

	// ADRI_LOG(-1, 1, 2,"","");
}  

void adriTools_logger::spiff_addLine(String result){
    // String old;
    // spiff_get(old);
    File file = LittleFS.open(spiff_folder+"/"+spiff_fileName, "a");
    file.println(result);
    file.close();

}
void adriTools_logger::spiff_readCurrent(){
    ADRI_LOG(-1, 0, 2, "", "");

    fsprintf("\n[spiff_readCurrent]\n"); 
    char buffer[512];
    String print = "";
    int nbr = 0;
    File file = LittleFS.open(spiff_folder+"/"+spiff_fileName, "r");
    if (file) {
        while (file.position()<file.size()) {
            String xml = file.readStringUntil('\n');
            if (xml != "") {
            	// Serial.println(xml);
                sprintf(buffer, "[%5d] %s", nbr, xml.c_str());
                print += String(buffer) + "\n";
                fsprintf("%s\n", buffer);
                delay(5);
                nbr++;
            }
        }
        file.close(); 
    }
    ADRI_LOG(-1, 1, 2, "", "");
}
void adriTools_logger::spiff_removeCurrent(){
	LittleFS.remove(spiff_folder+"/"+spiff_fileName);
}
   	
// endregion >>>> SPIFF