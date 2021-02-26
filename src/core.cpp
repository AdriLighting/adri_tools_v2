        // #define fsprintf(parm_a, ...)           {sprintf_P(adriTools_PrBuffer, (PGM_P)PSTR(parm_a), ##__VA_ARGS__); adriTools_print(String(adriTools_PrBuffer));if(adri_toolsv2_trace){ARDUINOTRACE_TRACEF();adriTools_print(ARDUINOTRACE_EXTERN_STRING);adriTools_printLn("");}}

#include "core.h"

    #if defined(ESP8266)
        #define FILESYSTEM LittleFS
    #elif defined(ESP32)
        #define FILESYSTEM SPIFFS
    #else
    #endif

namespace {
String  string_to_split(String name, String value){
    return name + ";" + value;
}    
}


// region ################################################ ARDUINOTRACE 
// (Edited frm : ArduinoTrace - github.com/bblanchon/ArduinoTrace)
// ########################################################################################
boolean adri_toolsv2_trace = false;
String ARDUINOTRACE_EXTERN_STRING;
 String ARDUINOTRACE_LINE ;
 String ARDUINOTRACE_FUNC ;
 String ARDUINOTRACE_VAR  ;
// endregion >>>> ARDUINOTRACE

const char PROGMEM adriTools_ch_cr[]  = "\n";
char * adriTools_PrBuffer;


// region ################################################ ADRI_TOOLSV2
// ########################################################################################
adri_toolsV2 * adri_toolsv2_ptr = nullptr;
adri_toolsV2 * adri_toolsv2Ptr_get(){
    if (adri_toolsv2_ptr==nullptr) adri_toolsv2_ptr = new adri_toolsV2();
    return adri_toolsv2_ptr;
}
void adri_toolsv2Ptr_chech(){
    if (adri_toolsv2_ptr==nullptr) adri_toolsv2_ptr = new adri_toolsV2();
}
void adri_toolsV2::loop(){
    if (heap_monitor) heap_monitorLoop();
    if (_telnetLoop != NULL) _telnetLoop ();
}
adri_toolsV2::adri_toolsV2(){
    adri_toolsv2_ptr = this;
    adriTools_PrBuffer = new char[255];
    heap_start = ESP.getFreeHeap();
    heap_last = ESP.getFreeHeap();
}
adri_toolsV2::adri_toolsV2(int size){
    adri_toolsv2_ptr = this;
    adriTools_PrBuffer = new char[size];
    heap_start = ESP.getFreeHeap();
    heap_last = ESP.getFreeHeap();
}  
// endregion >>>> ADRI_TOOLSV2
// 
// region ################################################ FSPRINTF
// ########################################################################################

 /**
 * @fn          { adriTools_print (String buf) }
 * @brief       { function_description }
 *
 * @param[in]   path  The path
 *
 * @return      { description_of_the_return_value }
 */

void adriTools_print (String buf) {
    if (adri_toolsv2_ptr!=nullptr) {
        if (adri_toolsv2_ptr->_telnetPut != NULL) {
            adri_toolsv2_ptr->_telnetPut(buf);
        }
    }

    Serial.print(buf);
}
void adriTools_printLn      (String buf) {Serial.println(buf);}
void adriTools_getbufferSier(const char *tag){
    int length = sizeof(tag)/sizeof(char);
    const char *arr_ptr = &tag[0];
    Serial.print(F("size 1: "));
    Serial.print(length);
    Serial.print(F(" - size 2: "));
    Serial.println(strlen(arr_ptr));

}
// endregion >>>> FSPRINTF

// region ################################################ LOG
// ########################################################################################


void adri_toolsV2::log_filePath_set(String ret, int debug){/*ADRI_LOG(debug, 2, 2, "", "");*/log_filePath = ret; }
void adri_toolsV2::log_read(String & ret, boolean lineNbr, int debug){
    // ADRI_LOG(debug, 0, 2, "", "");
    if (lineNbr) fsprintf("\n[_log_read]\n"); 
    char buffer[255];
    ret = "";
    String print = "";
    int nbr = 0;
    File file = FILESYSTEM.open(log_filePath, "r");
    if (file) {
        while (file.position()<file.size()) {
            String xml = file.readStringUntil('\n');
            if (xml != "") {
                if (lineNbr)  {
                    sprintf(buffer, "[%15d] %s", nbr, xml.c_str());
                    print += String(buffer) + "\n";
                    fsprintf("%s\n", buffer);
                    delay(5);
                    nbr++;
                    ret += xml + "\n";
                }
                else ret += xml + "\n";
            }
        }
        file.close(); 
    }
    // ADRI_LOG(debug, 1, 2, "", "");
}
void adri_toolsV2::log_write(String & old, String timeStr, int debug){
    // ADRI_LOG(debug, 2, 2, "", "");
    int freeHeap = ESP.getFreeHeap();
    File file = FILESYSTEM.open(log_filePath, "w");
    if (file) {
        char buffer[1000];
        sprintf(buffer, "%15s | %15d", timeStr.c_str(), freeHeap);
        String result = old + "\n" + String(buffer);
        file.println(result);
        file.close(); 
        old = result;
    } 
}

void adri_toolsV2::log_write(String & old , String timeStr, String msg, int debug){
    // ADRI_LOG(debug, 2, 2, "", "");
    File file = FILESYSTEM.open(log_filePath, "w");
    if (file) {
        char buffer[1000];
        sprintf(buffer, "%15s | %s", timeStr.c_str(), msg.c_str());
        String result = old + "\n" + String(buffer);
        file.println(result);
        file.close(); 
        old = result;
    } 
}
// endregion >>>> LOG

// region ################################################ DISPLAY TEXT
// ########################################################################################
String adri_toolsV2::info_parm(String name, String value , int len, const char * tdb1, const char * sep,  const char * tdb2,  const char * last) {
    String s=name;
    int size = s.length();
    while (size<len){
        s+=" ";
        size = s.length();
    }
    return tdb1+s+sep+tdb2+value+last;
}
void adri_toolsV2::tempStr_print() {
    if (tempStrArraySize < 0) return;
    for (int i = 0; i < tempStrArraySize; ++i) {
        fsprintf("%s", tempStrArray[i].c_str());
        yield();
    }
}
String * adri_toolsV2::tempStr_get(int & size){
    size = tempStrArraySize;
    return tempStrArray;
}
String * adri_toolsV2::tempStr_get(){
    return tempStrArray;
}
void adri_toolsV2::tempStr_sizeSet(int size){
    tempStrArraySize = size;
}
// endregion >>>> DISPLAY TEXT

// region ################################################ CHAR
// ########################################################################################
String adri_toolsV2::ch_toString(char * c){
    // char result[255];
    // fssprintf(result, "%s", c);
    String result((char*)c);
    return result;
}
String adri_toolsV2::ch_toString(const char * c){
       return String((const __FlashStringHelper*) c);
}
// endregion >>>> CHAR

// region ################################################ TIME
// ########################################################################################
int* adri_toolsV2::splitTime(String Val, char sep) {
   String      aVal            = Val;
   byte        firstIndex      = aVal.indexOf(sep);
   byte        secondIndex     = aVal.indexOf(sep, firstIndex + 1);
   String      hr              = aVal.substring(0, firstIndex);
   String      minute          = aVal.substring(firstIndex + 1, secondIndex);
   String      sec             = aVal.substring(secondIndex + 1);
   int         _hr             = hr.toInt();
   int         _minute         = minute.toInt();
   int         _sec            = sec.toInt();

   int     *array          = new int[3];
           array[0]        = _hr;
           array[1]        = _minute;
           array[2]        = _sec;
   return array;    
}
void adri_toolsV2::seconds2time(unsigned long s, char * time) {
   // int milli      = (s                    ) % 1000;
   int seconds    = (s /   (1000)         ) % 60   ;
   int minutes    = (s /   (1000*60)      ) % 60   ;
   int hours      = (s /   (1000*3600)    ) % 24   ;
   int days    = (s /  (1000*3600*24)  )     ;
   sprintf(time,"%d-%02d:%02d:%02d", days, hours , minutes, seconds);
}
void adri_toolsV2::millis2time(unsigned long s, char * time) {
   int milli      = (s                    ) % 1000;
   int seconds    = (s /   (1000)         ) % 60   ;
   int minutes    = (s /   (1000*60)      ) % 60   ;
   int hours      = (s /   (1000*3600)    ) % 24   ;
   int days    = (s /  (1000*3600*24)  )     ;
   sprintf(time,"%d-%02d:%02d:%02d:%01d", days, hours , minutes, seconds, milli);
}
String adri_toolsV2::on_time() {
   char t[100];
   unsigned long offset = 0; //1000 * 60 * 60 * 24 * 3 ;
   unsigned long ms=millis()+offset;
   seconds2time(ms, t);
   return String(t);
}
String adri_toolsV2::om_time() {
   char t[100];
   unsigned long offset = 0; //1000 * 60 * 60 * 24 * 3 ;
   unsigned long ms=millis()+offset;
   millis2time(ms, t);
   return String(t);
}

String adri_toolsV2::on_time(long ms) {
   char t[100];
   seconds2time(ms, t);
   return String(t);
}


// endregion >>>> TIME

// region ################################################ IP
// ########################################################################################
String adri_toolsV2::ip2string(IPAddress a) {
    char buf[18];
        fssprintf(buf,"%d.%d.%d.%d",a[0],a[1],a[2],a[3]);
        return String(buf);
    }

IPAddress adri_toolsV2::string2ip(String ip) {
    String a[10];
    explode(ip, '.', a);
    IPAddress adr = {
        static_cast<uint8_t>(a[0].toInt()),
        static_cast<uint8_t>(a[1].toInt()),
        static_cast<uint8_t>(a[2].toInt()),
        static_cast<uint8_t>(a[3].toInt())
    };
    return adr;
}
// endregion >>>> IP

// region ################################################ STRING SPLIT
// ########################################################################################
int adri_toolsV2::explode(String s, char sep, String * list) {
    // Serial.printf("\nexplode s: %s\n", s.c_str() );
    String  t           = s + sep;
    int     str_index   = 0;
    int     list_index  = 0;
    int     j;
    String  sub;
    int size = t.length();
    while (str_index < size ) {
        j=t.indexOf(sep,str_index);
        if (j!=-1) {
            sub=t.substring(str_index, j);
            // Serial.printf("explode index:%d %s\n", str_index,sub.c_str() );
            list[list_index++]=sub;
            str_index=j+1;
        }
    }
    list[list_index]="";
    return list_index;
}
// endregion >>>> STRING SPLIT

// region ################################################ XML
// ########################################################################################
String adri_toolsV2::literal_value(String name, String xml){
   String open,close;
   String value = "";
   int start, end;
   open="<"+name+">";
   close="</"+name+">";
   start=xml.indexOf(open);
   if (start!=-1) {
      start+=name.length()+2;
      end=xml.indexOf(close);
      value=xml.substring(start,end);
   }
   return value;
}
String adri_toolsV2::literal_item(String name, String value) {
   String x="<"+name+">"+value+"</"+name+">";
   return x;
}
// endregion >>>> XML

// region ################################################ JSON
// ########################################################################################
String adri_toolsV2::jsonAddStringsValue (boolean start, String label, String value) {
    String ret;
    if (start)  ret = "\""+label+"\":\"" +  value + "\"";
    else        ret = ", \""+label+"\":\"" +  value + "\"";
    return ret;
}
String adri_toolsV2::jsonAddIntValue (boolean start, String label, String value) {
    String ret;
    if (start)  ret =  "\""+label+"\":" +  value ;
    else        ret = ", \""+label+"\":" +  value ;
    return ret;
}
String adri_toolsV2::jsonAddIntValue (boolean start, char * c_label, String value) {
    String label = ch_toString(c_label);
    String ret;
    if (start)  ret =  "\""+label+"\":" +  value ;
    else        ret = ", \""+label+"\":" +  value ;
    return ret;
}
// endregion >>>> JSON

// region ################################################ FILESYSTEM
// ########################################################################################
int adri_toolsV2::_SPIFFS_printFiles(String path){


    return 0;
}
void adri_toolsV2::SPIFFS_printFiles(String path){

}
// endregion >>>> FILESYSTEM

// region ################################################ ESP
// ########################################################################################

fs(sys_sys,     "\nSYSTEM\n");
fs(sys_core,    "Core/SDK");
fs(sys_heap,    "Heap");
fs(sys_boot,    "Boot Version");
fs(sys_cpu,     "CPU Freq");
fs(sys_sdk,     "SDK");
fs(sys_chip,    "ESP8266 Chip ID");
fs(sys_flash,   "Flash ID");
fs(sys_vcc,     "Vcc");
fs(ko,          "error");
fs(ok,          "ok");
fs(flash_conf,  "flash configuration");

void adri_toolsV2::ESP_boot_info() {
    if (tempStrArray != nullptr) delete[] tempStrArray;
    tempStrArraySize    = 1;
    tempStrArray        = new String[tempStrArraySize];

    // const rst_info  * resetInfo = system_get_rst_info();
    // boot_flags      f           = bootmode_detect();

    tempStrArray[0] = "\nBOOT\n";
    // tempStrArray[1] = string_to_split(F("reset reason"), String(RST_REASONS[resetInfo->reason]));     
    // tempStrArray[2] = string_to_split(F("reset cause"),  reset_cause(f));     
    // tempStrArray[3] = string_to_split(F("boot device"),  boot_device(f));     

}

void adri_toolsV2::ESP_core_info() {

    if (tempStrArray != nullptr) delete[] tempStrArray;
    tempStrArraySize    = 3;
    tempStrArray        = new String[tempStrArraySize];

    // char chip_id[10];
    // fssprintf(chip_id," (0x%06x)",ESP.getChipId());

    tempStrArray[0] = fsget(sys_sys);
    // tempStrArray[1] = string_to_split(fsget(sys_chip), ESP.getChipId()+String(chip_id));
    // tempStrArray[2] = string_to_split(fsget(sys_core), String(ESP.getCoreVersion())+"/"+ESP.getSdkVersion());
    // tempStrArray[3] = string_to_split(fsget(sys_boot), String(ESP.getBootVersion()));
    tempStrArray[1] = string_to_split(fsget(sys_heap), String(ESP.getFreeHeap())+"bytes");
    tempStrArray[2] = string_to_split(fsget(sys_cpu),  String(ESP.getCpuFreqMHz())+"MHz");
    // tempStrArray[6] = string_to_split(fsget(sys_vcc),  String(ESP.getVcc())+"mV") ;
}
void adri_toolsV2::ESP_shortFlash_info() {
    if (tempStrArray != nullptr) delete[] tempStrArray;
    tempStrArraySize    = 2;
    tempStrArray        = new String[tempStrArraySize];

    tempStrArray[0] = "\nFLASH\n";

    // FSInfo f;
    // FILESYSTEM.info(f);

    tempStrArray[1] = string_to_split(F("sketch size"),           String(ESP.getSketchSize())                         );
    // tempStrArray[2] = string_to_split(F("sketch freeSpace"),      String(ESP.getFreeSketchSpace())                    );
    // tempStrArray[3] = (ESP.getFlashChipRealSize()!=ESP.getFlashChipSize()) ? string_to_split(fsget(flash_conf), F("error")) : string_to_split(fsget(flash_conf), F("ok"));
    // tempStrArray[4] = string_to_split(F("SPIFFS"),    "____");
    // tempStrArray[5] = string_to_split(F("spiffs TotalBytes"),    String(f.totalBytes)                                );
    // tempStrArray[6] = string_to_split(F("spiffs UsedBytes"),     String(f.usedBytes)                                 );
    // tempStrArray[7] = string_to_split(F("spiffs FreeBytes"),     String(f.totalBytes-f.usedBytes)                    );

}
void adri_toolsV2::ESP_flash_info() {
    if (tempStrArray != nullptr) delete[] tempStrArray;
    tempStrArraySize    = 6;
    tempStrArray        = new String[tempStrArraySize];

    tempStrArray[0] = "\nFLASH\n";

    // char chip_id[20];
    // fssprintf(chip_id," (0x%06x)", ESP.getFlashChipId());

    // FSInfo f;
    // FILESYSTEM.info(f);

    String io;
    switch (ESP.getFlashChipMode()) {
        case FM_QIO :   io="QIO";   break;
        case FM_QOUT:   io="QOUT";  break;
        case FM_DIO :   io="DIO";   break;
        case FM_DOUT:   io="DOUT";  break;
        default     :   io="UNKNOWN";
    }
    // tempStrArray[1] = string_to_split(F("flash size map"),        String(FLASH_SIZE_MAP_NAMES[system_get_flash_size_map()]));
    // tempStrArray[1] = string_to_split(F("chip size"),             String(ESP.getFlashChipRealSize())                  );
    tempStrArray[1] = string_to_split(F("ide size"),              String(ESP.getFlashChipSize())                      );
    tempStrArray[2] = string_to_split(F("ide speed"),             String(ESP.getFlashChipSpeed())                     );
    tempStrArray[3] = string_to_split(F("ide mode"),              io                                                  );
    tempStrArray[4] = string_to_split(F("sketch size"),           String(ESP.getSketchSize())                         );
    tempStrArray[5] = string_to_split(F("sketch freeSpace"),      String(ESP.getFreeSketchSpace())                    );
    // tempStrArray[7] = (ESP.getFlashChipRealSize()!=ESP.getFlashChipSize()) ? string_to_split(fsget(flash_conf), F("error")) : string_to_split(fsget(flash_conf), F("ok"));
    // tempStrArray[6] = string_to_split(F("SPIFFS"),    "____");
    // tempStrArray[7] = string_to_split(F("spiffs TotalBytes"),    String(f.totalBytes)                                );
    // tempStrArray[8] = string_to_split(F("spiffs UsedBytes"),     String(f.usedBytes)                                 );
    // tempStrArray[9] = string_to_split(F("spiffs FreeBytes"),     String(f.totalBytes-f.usedBytes)                    );

}
// endregion >>>> ESP

// region ################################################ HEAP
// ########################################################################################
String adri_toolsV2::heap_string() {
  char buf[50];
  int heap = system_get_free_heap_size();
  fssprintf(buf, "[freeheap: %6d] [sart: %5d - lost: %5d]", heap, heap_start, heap_start-heap) ;
  return String(buf);
}
void adri_toolsV2::heap_print() {
    int heap = system_get_free_heap_size();
    fsprintf("\n[freeheap: %6d] [sart: %5d - lost: %5d]\n", heap, heap_start, heap_start-heap) ;
}
void adri_toolsV2::heap_monitorLoop() {
    int heap = system_get_free_heap_size();
    if (heap_last != heap) {

        if      (heap > heap_last) {fsprintf("[Remaining heap: %5d - lost: %5d] [sart: %5d - lost: %5d]\n", heap, heap-heap_last, heap_start, heap_start-heap);}
        else if (heap < heap_last) {fsprintf("[Remaining heap: %5d - gain: %5d] [sart: %5d - lost: %5d]\n", heap, heap_last-heap, heap_start, heap_start-heap);}

        heap_last = system_get_free_heap_size();
    }
}
void adri_toolsV2::heap_monitorToggle() {
    heap_monitor = !heap_monitor;
    ADRI_LOGV(-1, 2, 2, heap_monitor, "", "");    
}
// >>> HEAP    
// endregion >>>> HEAP

    boolean adri_toolsV2::toBoolean(String value){
        if (value == "1") {
            return true;
        }        
        if (value == "true") {
            return true;
        }
        if (value ==  "TRUE") {
            return true;
        }
        if (value ==  "True") {
            return true;
        }
        if (value ==  "0") {
            return false;
        }        
        if (value ==  "false") {
            return false;
        }
        if (value ==  "FALSE") {
            return false;
        }
        if (value ==  "False") {
            return false;
        }
    }   