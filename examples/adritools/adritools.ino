#include <adri_tools_v2.h>

void setup()
{
	Serial.begin(115200);
	adri_toolsV2 * _ptr = new adri_toolsV2();
	_ptr->heap_print();
}

void loop()
{
	
}