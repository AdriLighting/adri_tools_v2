# adri_tools_v2

Multiple utility tools for string managment and more.


<pre>
Librairies
<details>
adri_tools_v2                   = https://github.com/AdriLighting/adri_tools_v2

esp8266/arduino                 = https://github.com/esp8266/Arduino/tree/master/libraries
</details>
Dependency Graph
<details>
|-- [adri_tools_v2] 1.0.0
|   |-- [ESP8266WiFi] 1.0
|   |-- [LittleFS(esp8266)] 0.1.0</details>
</pre>

<hr>

#### examples

adritools_1
<details>
platformio.ini
<details>
<pre>
[env:nodemcuv2]
platform=espressif8266
board=nodemcuv2
framework=arduino
lib_extra_dirs= ${env.lib_extra_dirs}
[platformio]
src_dir= ${env.src_dir}

</pre>
</details>
<pre>
Librairies
<details>
adri_tools_v2                   = https://github.com/AdriLighting/adri_tools_v2
adri_softLogger                 = https://github.com/AdriLighting/adri_softLogger
adri_tools_v2_serialMenu        = https://github.com/AdriLighting/adri_tools_v2_serialMenu
adri_wifiConnect                = https://github.com/AdriLighting/adri_wifiConnect
adri_timer                      = https://github.com/AdriLighting/adri_timer

esp8266/arduino                 = https://github.com/esp8266/Arduino/tree/master/libraries
</details>
Dependency Graph
<details>
|-- [adri_tools_v2] 1.0.0
|   |-- [ESP8266WiFi] 1.0
|   |-- [LittleFS(esp8266)] 0.1.0
|-- [adri_softLogger] 1.0.0
|   |-- [adri_tools_v2] 1.0.0
|   |   |-- [ESP8266WiFi] 1.0
|   |   |-- [LittleFS(esp8266)] 0.1.0
|   |-- [ESP8266WiFi] 1.0
|   |-- [LittleFS(esp8266)] 0.1.0
|-- [adri_tools_v2_serialMenu] 1.0.0
|   |-- [adri_tools_v2] 1.0.0
|   |   |-- [ESP8266WiFi] 1.0
|   |   |-- [LittleFS(esp8266)] 0.1.0
|-- [adri_wifiConnect] 1.0.0
|   |-- [ESP8266WebServer] 1.0
|   |   |-- [ESP8266WiFi] 1.0
|   |-- [ESP8266WiFi] 1.0
|   |-- [ESP8266mDNS] 1.2
|   |   |-- [ESP8266WiFi] 1.0
|   |-- [adri_tools_v2] 1.0.0
|   |   |-- [ESP8266WiFi] 1.0
|   |   |-- [LittleFS(esp8266)] 0.1.0
|   |-- [LittleFS(esp8266)] 0.1.0
|   |-- [adri_timer] 1.0.0</details>
</pre>

</details>
<hr>