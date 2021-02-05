
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


	/**
	* @class	adriTools_logger_variables
	*
	* @brief    crée une region atribbuer a un ou plusieur "printf" ou "LITTLEFS"
	* @see		https://github.com/AdriLighting/adri_tools_v2
	*
	* @author	adrilighting
	* @date		05/02/2021 06:25:54
	*/
	class adriTools_logger_variables 
	{

		public:
			/** \brief active/desactive une totalement region */
			boolean _state 		= true;

			/** \brief active/desactive "LITTLEFS" */
			boolean _addLine 	= true;

			/** \brief active/desactive "printf" */
			boolean _serial 	= true;

			/** \brief nom d'affichage */
			String 	_name 		= "0";

			adriTools_logger_variables(String name);

			void toggle();
			void toggleAddLine();
			void toggleSerialPrint();
			boolean statu();
			boolean statuAddLine();
			boolean statuSerialPrint();
	};

	/**
	* @class	adriTools_logger
	*
	* @brief	fournit des fonctionnalités utiles pour déboguer les croquis via "printf" ou "LITTLEFS"
	* @see		https://github.com/AdriLighting/adri_tools_v2
	*
	* @author	adrilighting
	* @date		05/02/2021 06:25:54
	*/

	class adriTools_logger
	{
		/** \brief nbr de class "adriTools_logger_variables" ceer */
		int 						_activateByVariable_cnt = 0;
		/** \brief array cotenat les class "adriTools_logger_variables" */
		adriTools_logger_variables 	* _activateByVariable[10];

		/** \brief active/desactive le "printf" et "littfs" pour toute les region */
		boolean printer_display			= true;

		/** \brief active  le "printf" et "littfs" uniquement pour le lvl selectionner pour toute les region */
		int 	printer_display_lvl 	= 0;

		/** \brief active/desactive par region < 0 le "printf"  */
		boolean seriealPrint_otherRegion 	= true;
		/** \brief active/desactive par region < 0 le "littfs"  */
		boolean spiff_addLine_otherRegion = true;

		/** \brief active/desactive le "littfs" pour toutes les region */
		boolean spiff_sav = false;

		/** \brief acitve/desactive le timestamp */
		boolean printer_timestamp 	= true;

		/** \brief activer/desactive le freeheap */
		boolean printer_heap 		= true;		

		/** \brief enplacement du doosier des log situé da la mem SPIFF */
		String spiff_folder = "/logger";

		/** \brief nom du fichier log situé da le dossier "spiff_folder" */
		String spiff_fileName = "";
	public:



		void serial_menu();

		void printer_serial(boolean addLine, int lvl, char * buffer);

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

		void spiff_addLine_otherRegion_toggle();
		void seriealPrint_otherRegion_toggle();

		/**
		 * @brief      constructor
		 */
		adriTools_logger();
		~adriTools_logger(){};


		boolean printer_displayMod(int region, int lvl);
		String 	timeStamp();
		int 	strContains(char str, const char *sfind);


		void activateByVariable_print();

		void activateByVariable_add(String name);

		void activateByVariable_toggle(int pos);
		void activateByVariable_toggleAddLine(int pos);
		void activateByVariable_toggleSerial(int pos);

		boolean activateByVariable_stau(int pos);
		boolean activateByVariable_statuAddLine(int pos);
		boolean activateByVariable_statuSerialPrint(int pos);
	};

	adriTools_logger * adriTools_logger_ptrGet();
	String adriToolLogger_serialMenu_region(String cmd, String value);
	String adriToolLogger_serialMenu_regionAddLine(String cmd, String value);
	String adriToolLogger_serialMenu_regionSerialPrint(String cmd, String value);
	String adriToolLogger_serialMenu_cmd(String cmd, String value);	
#endif // LOGGER_H
