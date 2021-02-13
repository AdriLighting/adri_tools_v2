#include <Arduino.h>

	// #define ADRIOTOOLS_USETELNET

// ADRIOTOOLS_ADVLOGGER  [freeheap:  46368] [sart: 48448 - lost:  2080]
// ADRIOTOOLS_SOFTLOGGER [freeheap:  46872] [sart: 48512 - lost:  1640]

#define ADRIOTOOLS_SOFTLOGGER 
#include <adri_soft_logger.h>
#include <adri_tools_v2.h>
#include <adri_tools_v2_serialMenu.h>

#ifdef ADRIOTOOLS_USETELNET
	adri_telnet * _adri_telnet;

	#include <adri_wifiConnect.h>
	#include "secret.h"
	wifiConnect 		* myWifi;	// PTR pour unr instance statique "wifiConnect"
	wifi_credential_ap	* myWifiAp;	// PTR pour unr instance statique "wifi_credential_ap"

	const char 			* myWifiHostname = "MY_WIFI"; 	// AP AND DNS HOSTNAME 
														
											// 	AWC_LOOP; 		WIFI CONNECT STARTUP WITH STATIC 
											// 	AWC_SETUP; 		WIFI CONNECT STARTUP WITH STATIC 
	WIFICONNECT_MOD		myWifiConnectMod 	= 	AWC_SETUP;	

											// 	AWCS_MULTI;		STA CONNECT WITH MULTIPLE SSID
											// 	AWCS_NORMAL;	STA CONNECT WITH THE SELECTED SSID
	WIFICONNECTSSID_MOD myWifiSSIDMod 		= 	AWCS_NORMAL;	

	boolean 			myWifiOTA 			= false; 	// ENABLED OTA

#endif
adri_toolsV2 			* _tools;
adriToolsv2_serialRead 	* _serial;
adriToolsLogger 		* _looger;



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

	String log = ""; 					// log result
	String logFile = "/log.txt";	// log filepath


	Serial.begin(115200);
  	for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  	Serial.println(F("\n##################################################\n\n"));


	LittleFS.begin();

	_tools 	= new adri_toolsV2();
	_looger = new adriToolsLogger();

	#ifdef ADRIOTOOLS_ADVLOGGER
		_looger->activateByRegion_add("example"); // exxemple for _serial_value
	#endif

  	_tools->heap_print();

	#ifdef ADRIOTOOLS_USETELNET

// region ################################################ WIFICONNECT
		myWifi 		= new wifiConnect();
		myWifiAp 	= new wifi_credential_ap("");
		myWifiAp->hostname_set(adri_toolsv2Ptr_get()->ch_toString(myWifiHostname));
		wifi_credential_ap_register(myWifiAp);
		wifi_credential_sta_fromSPIFF();
		wifi_credential_set(
			0, 						// postion du ssid selectionner (0 to 2)
			SECRET_SSID,			// ssid
			SECRET_PASS, 			// pswd
			"",						// ip 		(vide pour ne pas cofigurer d'ip)
			"",						// subnet 	(vide pour ne pas cofigurer d'subnet)
			""						// gateway 	(vide pour ne pas cofigurer d'gateway)
		);
		wifi_credential_sta_toSpiff();	
		wifi_credential_sta_print();
		myWifi->load_fromSpiif 				();
		myWifi->credential_sta_pos_set 		(0);
		myWifi->connect_set 				(myWifiConnectMod);
		if (myWifiConnectMod == AWC_LOOP) myWifiSSIDMod = AWCS_NORMAL; 	// wifi connect loop ne supporte pas encor de multiple ssid
		myWifi->connectSSID_set 			(myWifiSSIDMod);
		myWifi->station_set 				(WIFI_STA);
		myWifi->hostName_set 				(myWifiHostname); 			// initialisation dns si ota desactiver
		myWifi->setup_id					();							// initialize les id STA
		//
		myWifiAp->psk_set 					("mywifiappsk");						// pswd AP
		myWifiAp->ip_set 					(myWifi->_credential_sta->ip_get());	// ip 	AP
		myWifiAp->print 					();	
		if (myWifiConnectMod == AWC_SETUP) {
			myWifi->setup 						();
			if(!myWifiOTA) 	myWifi->MDSN_begin	();
			// else 			arduinoOTA_setup	(myWifiHostname);
			wifi_connect_statu 					();
			fsprintf("\n[myWifiConnectDone] : %s\n", adri_toolsv2Ptr_get()->on_time().c_str());
		}	
// endregion >>>> WIFICONNECT

		_adri_telnet = new adri_telnet();
		_tools->_telnetSetup 	= _adri_telnet->telnet_setup;
		_tools->_telnetLoop 	= _adri_telnet->telnet_loop;
		_tools->_telnetPut 		= _adri_telnet->telnet_put;
		_tools->_telnetGet 		= _adri_telnet->telnet_get;

		_tools->_telnetSetup ();
	#endif





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
	fsprintf("\n[LINE 1]\n"); 	// serial print with progmem
	fsprintf("\n[LINE 2]\n");
// endregion >>>> FSPRINTF
	
// region ################################################ ADRILOG
// AJOUTER DES LOG DANS UN FICHIER TXT
// 
	LittleFS.remove(logFile); 			// remove log filepath
	_tools->log_filePath_set(logFile); 	// set log filepath
	_tools->log_write(log, "mydata");
	_tools->log_write(log, "mydata_2", "mydata_3");
	_tools->log_read(log, true);
	_tools->log_write(log, "mydata_4");
	_tools->log_read(log, true, -1);
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
    _serial->cmd_item_add(	1, 	"print spiff",		"u",	"", 	_serial_print_Spiff);

    #ifdef ADRIOTOOLS_ADVLOGGER
// 							array 2/2	taille
    _serial->cmd_array(		2, 			4); 
//													!touche=value 	return 	function
    _serial->cmd_item_add(2, "loggerRegion",		"q",			"", 	adriToolLogger_serialMenu_region);
    _serial->cmd_item_add(2, "logger_regionAddLine","s",			"", 	adriToolLogger_serialMenu_regionAddLine);
    _serial->cmd_item_add(2, "logger_regionSerial",	"d",			"", 	adriToolLogger_serialMenu_regionSerialPrint);
    _serial->cmd_item_add(2, "logger",				"e",			"", 	adriToolLogger_serialMenu_cmd);  
    #endif
    #ifdef ADRIOTOOLS_SOFTLOGGER
    _serial->cmd_array(		2, 			1); 
    _serial->cmd_item_add(2, "logger",				"e",			"", 	adriToolLogger_serialMenu_cmd);  
    #endif
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
		#ifdef ADRIOTOOLS_USETELNET
		myWifi->wifi_loop();	
		#endif	
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


String _serial_parse(String cmd, String value){
	fsprintf("\n[_serial_parse] cmd: %s - value: %S\n", cmd.c_str(), value.c_str());
	return "";
}	
// endregion >>>> USER FUNCTIONS
