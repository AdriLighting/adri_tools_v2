# adri_tools_v2
adri_tools_v2
<pre>

core.h    
                region  lvl     trace   param   args
    ADRI_LOG(   parm_d, parm_c, parm_b, parm_a, ...)
    region  : attribuer un seemble de ADRI_LOG a la meme region, et de pouvoir gerer indivuduelement avec la classe "adriToolsLogger"
    lvl     : atribbuer un lvl associer a la position ou se trouve ADRI_LOG au sein de la fonction a debuger
    trace   : permet d'afficher la MACRO __LINE__ et ARDUINOTRACE_FUNCTION_NAME
    param   : "%s   - %d  - etcc"
    args    :  char - int - etcc
    
logger.h
    ADRI_LOG permet d"afficher avec Serial ou bien d'ecrire dans un fichier text 
    
    adriToolsLogger

    lvl
        0 start     t
        1 end       b
        2 random
    region
        -1 other region
        -2 dont print
        >=0 region via "adriToolsLogger_region"

        by mod 
            printer_displayMod_set
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

        by all region
            printer_display_toggle
                activ/desactiv  print et littfs 
            spiff_toggle
                activ/desactiv  littfs

        by region
            activateByRegion_toggle    
                print et littfs 
            activateByRegion_toggleAddLine
                littfs
            activateByRegion_toggleSerial
                print
    
</pre>