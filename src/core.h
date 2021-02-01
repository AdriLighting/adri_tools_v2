#ifndef CORE_H
	#define CORE_H

	#include <arduino.h>

	extern String ARDUINOTRACE_EXTERN_STRING;

	extern char ARDUINOTRACE_LINE[];
	extern char ARDUINOTRACE_FUNC[];
	extern char ARDUINOTRACE_VAR[];

	extern char adriTools_PrBuffer[];

	#include "ArduinoTrace.h"

	#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

	#if  defined(ARDUINO_ARCH_ESP32)
		#include <WiFi.h>
		#include <SPIFFS.h>
	#elif defined(ARDUINO_ARCH_ESP8266)
		#include <ESP8266WiFi.h>
		#include <LittleFS.h>
	#endif

	void adriTools_print(String buf);
	void adriTools_printLn(String buf);

	#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
		
		

		#define fs(parm_a,parm_b) 		static const char parm_a[] PROGMEM = parm_b
		#define fsget(parm_a) 			String(FPSTR(parm_a))

		extern boolean adri_toolsv2_trace;

		#define fsprintf(parm_a, ...) 			{sprintf_P(adriTools_PrBuffer, (PGM_P)PSTR(parm_a), ##__VA_ARGS__); adriTools_print(String(adriTools_PrBuffer));if(adri_toolsv2_trace){ARDUINOTRACE_TRACEF();adriTools_print(ARDUINOTRACE_EXTERN_STRING);adriTools_printLn("");}}
		#define fsprintfv(parm_b, parm_a, ...) 	{sprintf_P(adriTools_PrBuffer, (PGM_P)PSTR(parm_a), ##__VA_ARGS__); adriTools_print(String(adriTools_PrBuffer));if(adri_toolsv2_trace){ARDUINOTRACE_TRACEF();adriTools_print(ARDUINOTRACE_EXTERN_STRING+"\n");ARDUINOTRACE_DUMP(parm_b);}}
		#define fsprintfs(parm_a) 				{sprintf_P(adriTools_PrBuffer, PSTR(parm_a)); adriTools_print(String(adriTools_PrBuffer));}
		#define fssprintf(parm_b, parm_a,...) 	sprintf_P(parm_b, PSTR(parm_a), __VA_ARGS__)
	#endif


	#define ADRITOOLS_LOGP_LVL_LINE		0
	#define ADRITOOLS_LOGP_LVL_LINEFUNC	1
	#define ADRITOOLS_LOGP_LVL_VAR		2
	#define ADRITOOLS_LOGP_LVL_ALL 		3

	void esp_log_printf_(int actf, int lvl, int mod, const char *line, const char *func, const char *var, const char *tag);
	void esp_log_printf_(int actf, int lvl, int mod, const char *line, const char *func, const char *tag);

	#define adri_log_v(parm_e, parm_d, parm_c, parm_b, parm_a, ...) 							\
		ARDUINOTRACE_TRACEL()															\
		ARDUINOTRACE_TRACEF()															\
		ARDUINOTRACE_DUMPL(parm_b)														\
		sprintf_P(adriTools_PrBuffer, (PGM_P)PSTR(parm_a), ##__VA_ARGS__) ;				\
	  	esp_log_printf_(parm_e, parm_d, parm_c, ARDUINOTRACE_LINE, ARDUINOTRACE_FUNC, ARDUINOTRACE_VAR, adriTools_PrBuffer) 

	#define adri_log_(parm_d, parm_c, parm_b, parm_a, ...) 										\
		ARDUINOTRACE_TRACEL()															\
		ARDUINOTRACE_TRACEF()															\
		sprintf_P(adriTools_PrBuffer, (PGM_P)PSTR(parm_a), ##__VA_ARGS__) ;				\
	  	esp_log_printf_(parm_d, parm_c, parm_b, ARDUINOTRACE_LINE, ARDUINOTRACE_FUNC, adriTools_PrBuffer) 

	#define ADRI_LOGV(parm_e, parm_d, parm_c, parm_b, parm_a, ...) adri_log_v(parm_e, parm_d, parm_c, parm_b, parm_a, __VA_ARGS__)
	#define ADRI_LOG(parm_d, parm_c, parm_b, parm_a, ...) adri_log_(parm_d, parm_c, parm_b, parm_a, __VA_ARGS__)


	struct boot_flags {
	    unsigned char raw_rst_cause : 4;
	    unsigned char raw_bootdevice : 4;
	    unsigned char raw_bootmode : 4;

	    unsigned char rst_normal_boot : 1;
	    unsigned char rst_reset_pin : 1;
	    unsigned char rst_watchdog : 1;

	    unsigned char bootdevice_ram : 1;
	    unsigned char bootdevice_flash : 1;
	};


	class adri_toolsV2
	{
	private:
		String log_filePath = "/log.txt";


		int 	heap_last 		= 0;	
		int 	heap_start		= 0;	
		boolean heap_monitor 	= false;	

		void seconds2time(unsigned long s, char * time);
		
		int _SPIFFS_printFiles(String path);

		String boot_device(boot_flags flags);	
		String reset_cause(struct boot_flags flags);
	public:
		String 	* tempStrArray 		= nullptr;
		int 	tempStrArraySize 	= -1;

		adri_toolsV2();
		~adri_toolsV2(){};

		void log_filePath_set(String path);
		void log_read(String & ret, boolean lineNbr);
		void log_write(String & old, String timeStr);
		void log_write(String & old, String timeStr, String msg);

		String ch_toString(char * c);
		String ch_toString(const char * c);		

		String literal_value(String name, String xml);
		String literal_item(String name, String value);

		String on_time();
		String on_time(long ms);

		IPAddress 	string2ip(String ip);
		String 		ip2string(IPAddress a);

		int explode(String s, char sep, String * list);

		String jsonAddStringsValue (boolean start, String label, String value) ;
		String jsonAddIntValue (boolean start, String label, String value);
		String jsonAddIntValue (boolean start, char * c_label, String value);

		void SPIFFS_printFiles(String path);

		String info_parm(String name, String value , int len = 25, const char * tdb1 = "\t", const char * sep = " : ",  const char * tdb2 = "",  const char * last = "\n");
		
		void 	tempStr_print();
		String 	* tempStr_get(int & size);
		String 	* tempStr_get();
		void 	tempStr_sizeSet(int size);
		
		void ESP_core_info();
		void ESP_flash_info();
		void ESP_boot_info();

		String 	heap_string();
		void 	heap_print();
		void 	heap_monitorLoop();
		void 	heap_monitorToggle();

		void loop();
	};
	adri_toolsV2 * adri_toolsv2Ptr_get();

#endif // CORE_H
