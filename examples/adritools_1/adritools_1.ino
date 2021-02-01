#include <Arduino.h>
#include <adri_tools_v2.h>


adri_toolsV2 			* _tools;
adriToolsv2_serialRead 	* _serial;
adriTools_logger 		* _looger;


// region ################################################ USER FUNCTIONS PROTOTYPE
String _serial_menu(String cmd, String value);
String _serial_ESPreset(String cmd, String value);
String _serial_freeHeap(String cmd, String value);
String _serial_freeHeapToggle(String cmd, String value);
String _serial_print_log(String cmd, String value);
String _serial_adriTrace(String cmd, String value);
String _serial_value(String cmd, String value);
String _serial_parse(String cmd, String value);
// endregion >>>> USER FUNCTIONS PROTOTYPE

void setup()
{

	String log; 					// log result
	String logFile = "/log.txt";	// log filepath


	Serial.begin(115200);
  	for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  	fsprintf("\n##################################################\n\n");



	LittleFS.begin();

	_tools = new adri_toolsV2();
  	_tools->heap_print();

	_looger = new adriTools_logger();
	_looger->activateByVariable_add("example"); // exxemple for _serial_value

// region ################################################ ESP 
// PRINT ESP INFOS
// 
	_tools->ESP_core_info();
	_tools->tempStr_print();
	_tools->ESP_flash_info();
	_tools->tempStr_print();
	_tools->ESP_boot_info();
	_tools->tempStr_print();
// endregion >>>> ESP

// region ################################################ SPIFFS FILESYSTEM
// PRINT SPIFFS FILES
// 
	// _tools->SPIFFS_printFiles("");
// endregion >>>> SPIFFS FILESYSTEM

 
// region ################################################ FSPRINTF
// SERIAL PRINT W PROGMEM AND DEBGUGTRACE
// 	
	adri_toolsv2_trace = true; 	// active arduintrace into fsprintf

	fsprintf("\n[LINE 1]\n"); 	// serial print with progmem
	fsprintf("\n[LINE 2]\n");

	adri_toolsv2_trace = false;
// endregion >>>> FSPRINTF
	

	ARDUINOTRACE_TRACEF(); 	// inserer une trace "return as arduinotrace_extern_string" @ line 50 


// region ################################################ ADRILOG
// AJOUTER DES LOG DANS UN FICHIER TXT
// 
	LittleFS.remove(logFile); 			// remove log filepath
	_tools->log_filePath_set(logFile); 	// set log filepath
	// _tools->log_write(log, ARDUINOTRACE_EXTERN_STRING);
	_tools->log_write(log, "mydata");
	_tools->log_write(log, "mydata_2", "mydata_3");
	_tools->log_read(log, true);
	_tools->log_write(log, "mydata_4");
	_tools->log_read(log, true);
// endregion >>>> ADRILOG


// region ################################################ ADRISERIALMENU
// KEYBOARD-MENU VIE SERIAL
// 
    _serial = new adriToolsv2_serialRead();
// 							array 1/2	taille
    _serial->cmd_array(		1, 			7); 
//								display name		touche	return 	function
    _serial->cmd_item_add(	1, 	"menu",				"a",	"",		_serial_menu);
    _serial->cmd_item_add(	1, 	"ESPreset",			"z",	"", 	_serial_ESPreset);
    _serial->cmd_item_add(	1, 	"freeHeap",			"e",	"", 	_serial_freeHeap);
    _serial->cmd_item_add(	1, 	"freeHeapMonitor",	"r",	"", 	_serial_freeHeapToggle);
    _serial->cmd_item_add(	1, 	"print_log",		"t",	"", 	_serial_print_log);
    _serial->cmd_item_add(	1, 	"toggle trace",		"y",	"", 	_serial_adriTrace);
    _serial->cmd_item_add(	1, 	"print spiff",		"u",	"", 	_serial_print_Spiff);

// 							array 2/2	taille
    _serial->cmd_array(		2, 			2); 
//												!touche=value 	return 	function
    _serial->cmd_item_add(2, "serial_value",	"a", 			"", 	_serial_value);
    _serial->cmd_item_add(2, "serial_logger",	"q", 			"", 	_serial_logger);

// 							&cmd=value  display name	function  							
    _serial->cmd_3( (char*)"&", 		"parse", 		_serial_parse);
// 							²cmd=value    							
    _serial->cmd_4( (char*)"²", 		"parse", 		_serial_parse);

    _serial->menu(); // print menu
// endregion >>>> ADRISERIALMENU


// region ################################################ HEAP
    _tools->heap_print();
// endregion >>>> HEAP

}

void loop()
{
	_serial->loop();
	_tools->loop();
}


// region ################################################ USER FUNCTIONS
	
String _serial_menu(String cmd, String value) {
    adriToolsv2_serialReadPtr_get()->menu();
    return "";
}
String _serial_ESPreset(String cmd, String value){
    ESP.restart();
    return "";
}	
String _serial_freeHeap(String cmd, String value){
	_tools->heap_print();
    return "";
}	
String _serial_freeHeapToggle(String cmd, String value){
	_tools->heap_monitorToggle();
    return "";
}
String _serial_print_log(String cmd, String value){
	String log;
	_tools->log_read(log, true);
    return "";
}	
String _serial_print_Spiff(String cmd, String value){
	String log;
	// _tools->SPIFFS_printFiles("");
	// delete[] _tools;
	// _tools = nullptr;
	_tools->SPIFFS_printFiles("");
    return "";
}
String _serial_adriTrace(String cmd, String value){
	adri_toolsv2_trace =! adri_toolsv2_trace;
	fsprintfv(adri_toolsv2_trace, "\n[adri_toolsv2_trace]\n");
	return "";
}
String _serial_value(String cmd, String value){
	ADRI_LOG(0, 0, 2,"","");
	ADRI_LOG(0, 2, 2, "cmd: %s - value: %S", cmd.c_str(), value.c_str());	
	int pos = value.toInt();
	switch (pos) {
	    case 0:
	    	ADRI_LOG(0,3,2,"case 0","");
	    	_looger->activateByVariable_toggle(0);
			break;
		default: ADRI_LOG(0,3,2,"default",""); break;
	}
	ADRI_LOG(0, 1, 2,"","");
	return "";
}

String _serial_logger(String cmd, String value){
	ADRI_LOG(-1, 0, 2,"","");
	ADRI_LOG(-1, 2, 2, "cmd: %s - value: %S", cmd.c_str(), value.c_str());	
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
	} else {
		_looger->printer_display_lvl_set(pos);	
	}
	ADRI_LOG(-1, 1, 2,"","");
	return "";
}
String _serial_parse(String cmd, String value){
	fsprintf("\n[_serial_parse] cmd: %s - value: %S\n", cmd.c_str(), value.c_str());
	return "";
}	
// endregion >>>> USER FUNCTIONS
