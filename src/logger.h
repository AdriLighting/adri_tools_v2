#ifndef LOGGER_H
	#define LOGGER_H

	#include <arduino.h>
	#include "core.h"

/*	#define _LEVEL_ERROR 1
	#define _LEVEL_WARN 2
	#define _LEVEL_INFO 3
	#define _LEVEL_CONFIG 4
	#define _LEVEL_DEBUG 5
	#define _LEVEL_VERBOSE 6
	#define _LEVEL_VERY_VERBOSE 7
	static const char *LOG_LEVEL_LETTERS[] = {
	    "",    // NONE
	    "E",   // ERROR
	    "W",   // WARNING
	    "I",   // INFO
	    "C",   // CONFIG
	    "D",   // DEBUG
	    "V",   // VERBOSE
	    "VV",  // VERY_VERBOSE
	};	
*/
	class adriTools_logger_variables 
	{

		public:
			boolean _state = true;
			String 	_name = "0";
			
			adriTools_logger_variables(String name);
			void toggle();
			boolean statu();
	};

	class adriTools_logger
	{
		int 						_activateByVariable_cnt = 0;
		adriTools_logger_variables 	* _activateByVariable[10];

		boolean printer_display		= true;
		int 	printer_display_lvl = 0;

		boolean printer_timestamp 	= true;
		boolean printer_heap 		= true;		

		String spiff_folder = "/logger";
		String spiff_fileName = "";

		boolean spiff_sav = false;
	public:



		void serial_menu();
		void printer_serial(char * buffer);
		void printer_timestamp_toggle();
		void printer_heap_toggle();
		void printer_display_toggle();
		void printer_display_lvl_set(int lvl);

		void spiff_setup();
		void spiff_toggle();
		void spiff_readCurrent();
		void spiff_removeCurrent();
		void spiff_get(String & result);
		void spiff_addLine(String result);

		adriTools_logger();
		~adriTools_logger(){};

		boolean printer_displayMod(int actif, int lvl);

		String 	timeStamp();
		int 	strContains(char str, const char *sfind);
		
		void activateByVariable_add(String name);
		void activateByVariable_toggle(int pos);
		boolean activateByVariable_stau(int pos);
	};

	adriTools_logger * adriTools_logger_ptrGet();
#endif // LOGGER_H
