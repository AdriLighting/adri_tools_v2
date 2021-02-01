#ifndef MENU_SERIAL_H
	#define MENU_SERIAL_H

	#include <arduino.h>
	#include "core.h"

	typedef String (*at_srFunc)(String cmd, String value);  
	class adriToolsv2_serialReadItem
	{

	public:
			String 			_name;
			const char*		_key;
			String 			_ret;
			at_srFunc 		_function;	

		adriToolsv2_serialReadItem(){}
		void item_add(
			String 		name,
			const char*	key,
			String 		ret,
			at_srFunc 	f		);

		~adriToolsv2_serialReadItem(){};
		
	};
	class adriToolsv2_serialRead
	{
			int _cmd_1_cnt = 0;
			int _cmd_2_cnt = 0;
			adriToolsv2_serialReadItem * _cmd_1_Array = nullptr; // simpl letter 	- split
			adriToolsv2_serialReadItem * _cmd_2_Array = nullptr; // ! 			- split
			at_srFunc 	_cmd_3 		= NULL;		
			char* 		_cmd_3_sep  = (char *)"";		
			String		_cmd_3_desc = "";		
			at_srFunc 	_cmd_4 		= NULL;		
			char*		_cmd_4_sep  = (char *)"";		
			String 		_cmd_4_desc = "";		
	public:

		adriToolsv2_serialRead();
		~adriToolsv2_serialRead(){};

		int splitText(String A_readString, const char* sep, String & cmd, String & value) ;
		void cmd_array(int pos, int cnt);

		void cmd_item_add (int pos, String name, const char* key, String ret, at_srFunc f);
		void cmd_3(char* sep, String desc, at_srFunc f);
		void cmd_4(char* sep, String desc, at_srFunc f);

		void loop();
		void menu();
	};
	adriToolsv2_serialRead * adriToolsv2_serialReadPtr_get();
	
#endif // MENU_SERIAL_H
