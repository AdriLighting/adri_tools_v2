/**
 * @file    logger.cpp
 *
 * @brief   fournit des fonctionnalités utiles pour déboguer les croquis via "printf" ou "LITTLEFS"
 * @see     
 */

/*
    Utiliser la macro ADRI_LOGV ou ADRI_LOG

                region  lvl     trace   param   args
    ADRI_LOG(   parm_d, parm_c, parm_b, parm_a, ...)

    region  : attribuer un seemble de ADRI_LOG a la meme region, et de pouvoir gerer indivuduelement avec la classe "adriTools_logger"
    lvl     : atribbuer un lvl associer a la position ou se trouve ADRI_LOG au sein de la fonction a debuger
    trace   : permet d'afficher la MACRO __LINE__ et ARDUINOTRACE_FUNCTION_NAME
    param   : "%s   - %d  - etcc"
    args    :  char - int - etcc
    

    ADRI_LOG permet d"afficher avec Serial ou bien d'ecrire dans un fichier text
     

    adriTools_logger

    lvl
        0 start
        1 end
        2 random
    region
        -1 other region
        -2 dont print
        >=0 region via "adriTools_logger_variables"

        by mod 
            printer_displayMod_set
                set mos via printer_displayMod_pos
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

            printer_displayMod_set
                print et littfs  via printer_displayMod_pos

        all region
            printer_display_toggle
                activ/desactiv  print et littfs 
            spiff_toggle
                activ/desactiv  littfs

        by region
            activateByVariable_toggle    
                print et littfs 
            activateByVariable_toggleAddLine
                littfs
            activateByVariable_toggleSerial
                print

*/
    lvl from adriTools_logger

#include "logger.h"


char adriLogger_buffer1[1024];
// char adriLogger_buffer2[1024];


//################# adriTools_logger_variables
//################################################
//
// region ################################################  
/**
 * @brief      contructor
 *
 * @param[in]  name  nom de la region
 */
adriTools_logger_variables::adriTools_logger_variables(String name){
    _name = name;
    ADRI_LOGV(-1, 2, 2, _state, "%s", _name.c_str());
}
/**
*   @brief      active/desactive totalement la region
*/
void adriTools_logger_variables::toggle(){
    _state = !_state;
    ADRI_LOGV(-1, 2, 2, _state, "%s", _name.c_str());    
}   
/**
 * @brief       active/desactive "LITTLEFS"
 */
void adriTools_logger_variables::toggleAddLine(){
    _addLine = !_addLine;
    ADRI_LOGV(-1, 2, 2, _addLine, "%s", _name.c_str());    
} 
/**
 * @brief       active/desactive "printf"
 */
void adriTools_logger_variables::toggleSerialPrint(){
    _serial = !_serial;
    ADRI_LOGV(-1, 2, 2, _serial, "%s", _name.c_str());    
}   
/**
 * @brief      get the value of _addLine
 *
 * @return     _addLine
 */
boolean adriTools_logger_variables::statuAddLine(){
    return _addLine;
}  
/**
 * @brief     get the value of _state
 *
 * @return     _state
 */  
boolean adriTools_logger_variables::statu(){
    return _state;
}  
/**
 * @brief      get the value of _serial
 *
 * @return     _serial
 */
boolean adriTools_logger_variables::statuSerialPrint(){
    return _serial;
}  
// endregion >>>> 



//################# adriTools_logger
//################################################
//
// region ################################################ consctructor
adriTools_logger * adriTools_logger_ptr = nullptr;
/**
 * @brief      ptr ver la dernierre instance de la classe "adriTools_logger"
 *
 * @return     ptr
 */
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
/**
 * @brief      printf des regions et de leur statu
 */
void adriTools_logger::activateByVariable_print(){
    for (int i = 0; i < _activateByVariable_cnt; ++i)
    {
        ADRI_LOG(-1, 2, 0, "[%-5d][%-25S][state: %d][_addLine: %d][_serial: %d]", 
            i, 
            _activateByVariable[i]->_name.c_str(),
            _activateByVariable[i]->_state,
            _activateByVariable[i]->_addLine,
            _activateByVariable[i]->_serial
            );
    }
}
/**
 * @brief      ajoute une region, crée une instance "activateByVariable_add" 
 *
 * @param[in]  pos   The position
 */
void adriTools_logger::activateByVariable_add(String name){
    ADRI_LOG(-1, 2, 2, "pos:%d - name:%s", _activateByVariable_cnt, name.c_str());
    _activateByVariable[_activateByVariable_cnt] = new adriTools_logger_variables(name);
    _activateByVariable_cnt++;
}
/**
*   @brief      active/desactive totalement la region voullu
*/
void adriTools_logger::activateByVariable_toggle(int pos){
    _activateByVariable[pos]->toggle();    
} 
/**
 * @brief      active/desactive "littlefs" pour la region voullue
 *
 * @param[in]  pos   poistion opur le region "_activateByVariable[pos]"
 */
void adriTools_logger::activateByVariable_toggleAddLine(int pos){
    _activateByVariable[pos]->toggleAddLine();    
} 
/**
 * @brief      active/desactive "printf" pour la region voullue
 *
 * @param[in]  pos   poistion opur le region "_activateByVariable[pos]"
 */ 
void adriTools_logger::activateByVariable_toggleSerial(int pos){
    _activateByVariable[pos]->toggleSerialPrint();    
} 
/**
 * @brief     obtenir si la region peut "printf" et "littlefs"
 *
 * @param[in]  pos   poistion opur le region "_activateByVariable[pos]"
 *
 * @return     return boolean
 */
boolean adriTools_logger::activateByVariable_stau(int pos){
    return _activateByVariable[pos]->statu();
} 
/**
 * @brief     obtenir si la region peut "littlefs"
 *
 * @param[in]  pos   poistion opur le region "_activateByVariable[pos]"
 *
 * @return     return boolean
 */ 
boolean adriTools_logger::activateByVariable_statuAddLine(int pos){
    return _activateByVariable[pos]->statuAddLine();
}
/**
 * @brief     obtenir si la region peut "printf"
 *
 * @param[in]  pos   poistion opur le region "_activateByVariable[pos]"
 *
 * @return     return boolean
 */    
boolean adriTools_logger::activateByVariable_statuSerialPrint(int pos){
    return _activateByVariable[pos]->statuSerialPrint();
}     
// endregion >>>> activateByVariable

// region ################################################ tools
/**
 * @brief      création du tempstamp
 *
 * @return     string
 */
String adriTools_logger::timeStamp()
{
  char result[20];
  snprintf(result, 20, "%lu", millis());
  return String(result);
}
/**
 * @brief      recherche des similitude dans une chaine de caractére
 *
 * @param[in]  str    le caractére a chercher
 * @param[in]  sfind  la chaine a analyser
 *
 * @return     le nombre de similitude
 */
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


*/
 
/**
 * @brief       active  le "printf" et "littfs" uniquement pour le level voullu pour toutes les region
 *
 * @param[in]   lvl   level voullu
 */
void adriTools_logger::printer_displayMod_set(int lvl){
    printer_displayMod_pos = lvl;
    ADRI_LOGV(-1, 2, 2, printer_displayMod_pos, "", "");  
}    
// endregion >>>> display mod

// region ################################################ toggle
/**
 * @brief      acitve/desactive le timestamp
 */
void adriTools_logger::printer_timestamp_toggle(){
    printer_timestamp = !printer_timestamp;
    ADRI_LOGV(-1, 2, 2, printer_timestamp, "", "");  
}
/**
 * @brief      activer/desactive le freeheap
 */
void adriTools_logger::printer_heap_toggle(){
    printer_heap = !printer_heap;
    ADRI_LOGV(-1, 2, 2, printer_heap, "", "");  
}
/**
 * @brief      active/desactive le "printf" et "littfs" pour toutes les region
 */
void adriTools_logger::printer_display_toggle(){
    printer_display = !printer_display;
    ADRI_LOGV(-1, 2, 2, printer_display, "", "");  
}
/**
 * @brief      active/desactive le "littfs" pour toutes les region
 */
void adriTools_logger::spiff_toggle(){
    spiff_sav = !spiff_sav;
    ADRI_LOGV(-1, 2, 2, spiff_sav, "", ""); 	
}
/**
 * @brief      active/desactive par region < 0 le "littlfs"
 */
void adriTools_logger::spiffAddLine_otherRegion_toggle(){
    spiff_addLine_otherRegion = !spiff_addLine_otherRegion;
    ADRI_LOGV(-1, 2, 2, spiff_addLine_otherRegion, "", "");     
}
/**
 * @brief      active/desactive par region < 0 le "printf"
 */
void adriTools_logger::seriealPrint_otherRegion_toggle(){
    seriealPrint_otherRegion = !seriealPrint_otherRegion;
    ADRI_LOGV(-1, 2, 2, seriealPrint_otherRegion, "", "");     
}
// endregion >>>> toggle

// region ################################################ menu
/**
 * @brief      fonction pour le keyboard, active/desactive totalement la region voullu
 *
 * @param[in]  cmd    The command
 * @param[in]  value  The value
 *
 * @return     ""
 */
String adriToolLogger_serialMenu_region(String cmd, String value){
    adriTools_logger * _looger = adriTools_logger_ptrGet();
    ADRI_LOGP(false, -1, -1, 0, "<<<", "");
    ADRI_LOGP(false, -1, 0, 2,"","");
    ADRI_LOGP(false, -1, 2, 2, "cmd: %s - value: %S", cmd.c_str(), value.c_str());  
    int pos = value.toInt();
    _looger->activateByVariable_toggle(pos);
    ADRI_LOGP(false, -1, 1, 2,"","");
    ADRI_LOGP(false, -1, -1, 0, ">>>", "");
    return "";
}
/**
 * @brief      fonction pour le keyboard, active/desactive "littlefs" pour la region voullue
 *
 * @param[in]  cmd    The command
 * @param[in]  value  The value
 *
 * @return     ""
 */
String adriToolLogger_serialMenu_regionAddLine(String cmd, String value){
    adriTools_logger * _looger = adriTools_logger_ptrGet();
    ADRI_LOGP(false, -1, -1, 0, "<<<", "");
    ADRI_LOGP(false, -1, 0, 2,"","");
    ADRI_LOGP(false, -1, 2, 2, "cmd: %s - value: %S", cmd.c_str(), value.c_str());  
    int pos = value.toInt();
    _looger->activateByVariable_toggleAddLine(pos);
    ADRI_LOGP(false, -1, 1, 2,"","");
    ADRI_LOGP(false, -1, -1, 0, ">>>", "");
    return "";
}
/**
 * @brief      fonction pour le keyboard, active/desactive "printf" pour la region voullue
 *
 * @param[in]  cmd    The command
 * @param[in]  value  The value
 *
 * @return     ""
 */
String adriToolLogger_serialMenu_regionSerialPrint(String cmd, String value){
    adriTools_logger * _looger = adriTools_logger_ptrGet();
    ADRI_LOGP(false, -1, -1, 0, "<<<", "");
    ADRI_LOGP(false, -1, 0, 2,"","");
    ADRI_LOGP(false, -1, 2, 2, "cmd: %s - value: %S", cmd.c_str(), value.c_str());  
    int pos = value.toInt();
    _looger->activateByVariable_toggleSerial(pos);
    ADRI_LOGP(false, -1, 1, 2,"","");
    ADRI_LOGP(false, -1, -1, 0, ">>>", "");
    return "";
}
/**
 * @brief      fonction pour le keyboard, commnds menu
 *
 * @param[in]  cmd    The command
 * @param[in]  value  The value
 *
 * @return     ""
 */
Strin
String adriToolLogger_serialMenu_cmd(String cmd, String value){
    adriTools_logger * _looger = adriTools_logger_ptrGet();
    ADRI_LOGP(false, -1, -1, 0, "<<<", "");
    ADRI_LOGP(false, -1, 0, 2,"","");
    ADRI_LOGP(false, -1, 2, 2, "cmd: %s - value: %S", cmd.c_str(), value.c_str());  
    int pos = value.toInt();
    if (value == "a") {
        _looger->serial_menu();
    } else if (value == "z") {
        _looger->printer_display_toggle();
    } else if (value == "e") {
        _looger->printer_heap_toggle();
    } else if (value == "r") {
        _looger->printer_timestamp_toggle();
    } else if (value == "t") {
        _looger->spiff_toggle();
    } else if (value == "y") {
        _looger->spiff_readCurrent();
    } else if (value == "u") {
        _looger->spiff_removeCurrent();
    } else if (value == "i") {
        _looger->spiffAddLine_otherRegion_toggle();  
    } else if (value == "o") {
        _looger->seriealPrint_otherRegion_toggle();           
    } else if (value == "p") {
        _looger->activateByVariable_print();            
    } else {
        _looger->printer_displayMod_set(pos);  
    }
    ADRI_LOGP(false, -1, 1, 2,"","");
    ADRI_LOGP(false, -1, -1, 0, ">>>", "");
    return "";
}
/**
 * @brief      printf des comandes disponnible
 */
void adriTools_logger::serial_menu() {
    adri_toolsV2 * _ptr = adri_toolsv2Ptr_get();
    if (_ptr->tempStrArray != nullptr) delete[] _ptr->tempStrArray;
    int size = 23;
    _ptr->tempStr_sizeSet(size);
    _ptr->tempStrArray   	= new String[size];

    _ptr->tempStrArray[0]	= "\nPrinter lvl\n";
    _ptr->tempStrArray[1]	= _ptr->info_parm(F("0"),	F("all"), 10);
    _ptr->tempStrArray[2]	= _ptr->info_parm(F("1"),	F("all + only header bt "), 10);
    _ptr->tempStrArray[3]	= _ptr->info_parm(F("2"),	F("all + only header t "), 10);
    _ptr->tempStrArray[4]	= _ptr->info_parm(F("3"),	F("only region"), 10);
    _ptr->tempStrArray[5]	= _ptr->info_parm(F("4"),	F("only region + only header bt"), 10);
    _ptr->tempStrArray[6]	= _ptr->info_parm(F("5"),	F("only region + only header t"), 10);
    _ptr->tempStrArray[7]	= _ptr->info_parm(F("6"),	F("no region"), 10);
    _ptr->tempStrArray[8]	= _ptr->info_parm(F("7"),	F("no region + only header bt"), 10);
    _ptr->tempStrArray[9]	= _ptr->info_parm(F("8"),	F("no region + only header t"), 10);
    _ptr->tempStrArray[10]	= _ptr->info_parm(F("9"),	F("only bt"), 10);
    _ptr->tempStrArray[11]	= _ptr->info_parm(F("10"),	F("only t"), 10);
    _ptr->tempStrArray[12]	= "\nMenu\n";
    _ptr->tempStrArray[13]	= _ptr->info_parm(F("a"),	F("menu"), 10);
    _ptr->tempStrArray[14]	= _ptr->info_parm(F("z"),	F("printer_display_toggle"), 10);
    _ptr->tempStrArray[15]	= _ptr->info_parm(F("e"),	F("printer_heap_toggle"), 10);
    _ptr->tempStrArray[16]	= _ptr->info_parm(F("r"),	F("printer_timestamp_toggle"), 10);
    _ptr->tempStrArray[17]	= _ptr->info_parm(F("t"),	F("spiff activate"), 10);
    _ptr->tempStrArray[18]	= _ptr->info_parm(F("y"),	F("spiff read"), 10);
    _ptr->tempStrArray[19]	= _ptr->info_parm(F("u"),	F("spiff remove"), 10);    
    _ptr->tempStrArray[20] 	= _ptr->info_parm(F("i"),	F("toggle spifAddline lvl -1"), 10);
    _ptr->tempStrArray[21] 	= _ptr->info_parm(F("o"),	F("toggle serialPrint lvl -1"), 10);
    _ptr->tempStrArray[22] 	= _ptr->info_parm(F("p"),   F("print region"), 10);
    _ptr->tempStr_print();        
}    
// endregion >>>> menu

/**
 * @brief      fonction pour determiner si un "printf" ou "littlefs" est possible
 *
 * @param[in]  region  The region
 * @param[in]  lvl     The level
 *
 * @return     true si un "printf" ou "littlefs" est possible
 */
boolean adriTools_logger::printer_displayMod(int region, int lvl){
    if (!printer_display) return false;

    if (region >= 0) {
        if (!adriTools_logger_ptrGet()->activateByVariable_stau(region)) return false;

        if (printer_displayMod_pos == 6) return false;
        if (printer_displayMod_pos == 7) {
            if (lvl == -1) return true;
            if ((lvl != 0) && (lvl != 1))   return false;
        }
        if (printer_displayMod_pos == 8) {
            if (lvl == -1) return true;
            if (lvl!=0) return false;
        } 
    } else {
        if (printer_displayMod_pos == 3) return false;
        if (printer_displayMod_pos == 4) {
            if (lvl == -1) return true;
            if ((lvl != 0) && (lvl != 1)) return false;
        }
        if (printer_displayMod_pos == 5) {
            if (lvl == -1) return true;            
            if (lvl!=0) return false;
        }        
    }
    if (region == -2) return false; 

    if (printer_displayMod_pos == 9) {
        if (lvl == -1) return true;  
        if ((lvl != 0) && (lvl != 1)) return false;
    }
    if (printer_displayMod_pos == 10) {
        if (lvl == -1) return true;  
        if (lvl!=0) return false;
    } 

    return true;
}   

// region ################################################ printer line
void esp_log_print_lines(boolean addLine, int region, int lvl, int mod, String pLine, String func, String var, const char *tag ){
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

    _looger->printer_serial(addLine, region, adriLogger_buffer1 );
}
// endregion >>>> printer line

/**
 * @brief      fonction final pour le "printf" et "littfs"
 * @details    ajouter timestamp et freeheap
 * @details    check si la region est activer
 *
 * @param[in]  addLine  check if can print with "littfls"
 * @param[in]  region   The region
 * @param      buffer   The buffer
 */
void adriTools_logger::printer_serial(boolean addLine, int region, char * buffer){
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
        if (region >= 0) {
             if (adriTools_logger_ptrGet()->activateByVariable_statuAddLine(region)) {spiff_addLine(String(bufferPrint));}
        } else {
            if (spiff_addLine_otherRegion) spiff_addLine(String(bufferPrint));
        }
    }
    if (region >= 0) {
         if (adriTools_logger_ptrGet()->activateByVariable_statuSerialPrint(region)) {fsprintf("%s\n", bufferPrint);}
    } else {
        if (seriealPrint_otherRegion) fsprintf("%s\n", bufferPrint);
    }
    
}

// region ################################################ esp_log_printf_
void  esp_log_printf_(int region, int lvl, int mod, String line, String func, String var, const char *tag) {
    if (!adriTools_logger_ptrGet()->printer_displayMod(region, lvl)) return;
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
    esp_log_print_lines(true, region, lvl, mod, pLine, func, var, tag );
}
void  esp_log_printf_(int region, int lvl, int mod, String line, String func, const char *tag) {
    if (!adriTools_logger_ptrGet()->printer_displayMod(region, lvl)) return;
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
    esp_log_print_lines(true, region, lvl, mod, pLine, func, (char*)"", tag );
} 
void  esp_log_printf_(boolean addLine, int region, int lvl, int mod, String line, String func, const char *tag) {
    if (!adriTools_logger_ptrGet()->printer_displayMod(region, lvl)) return;
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
    esp_log_print_lines(addLine, region, lvl, mod, pLine, func, (char*)"", tag );
}     
// endregion >>>> 

// region ################################################ SPIFF
/**
 * @brief      initialize le fichier log
 * @details    utiliser pour chaque demmarage de l'esp, inscript dans le fichier log la raisson du dernier reset de l'esp 
 */
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
/**
 * @brief      stock le fichier log dans une string
 *
 * @param[int/out]  result  totalitée du fichier log
 */
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
/**
 * @brief      ajoute un ligne au fichier log
 *
 * @param[in]  result  la ligne a ajouter
 */
void adriTools_logger::spiff_addLine(String result){
    // String old;
    // spiff_get(old);
    File file = LittleFS.open(spiff_folder+"/"+spiff_fileName, "a");
    file.println(result);
    file.close();

}
/**
 * @brief      printf du fichier log
 */
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
/**
 * @brief      supprime le fichier log de la mem spiff
 */
void adriTools_logger::spiff_removeCurrent(){
	LittleFS.remove(spiff_folder+"/"+spiff_fileName);
}
   	
// endregion >>>> SPIFF