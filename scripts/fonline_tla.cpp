#include "fonline_tla.h"

// Slot/parameters allowing
EXPORT bool allowSlot_Hand1( uint8, Item &, Critter &, Critter & toCr );

// Initialization

// In this functions (DllMain and DllLoad) all global variables is NOT initialized, use FONLINE_DLL_ENTRY instead
#if defined ( FO_WINDOWS )
int __stdcall DllMain( void* module, unsigned long reason, void* reserved ) { return 1; }
#elif defined ( FO_LINUX )
void __attribute__( ( constructor ) ) DllLoad()   {}
void __attribute__( ( destructor ) )  DllUnload() {}
#endif

void RegisterASExtensions();

void RegisterASExtensions()
{
	// ASEngine;
	#ifdef __SERVER
	#define SCRIPT_ITEM_STR    "Item"
	#endif
	#ifdef __CLIENT
	#define SCRIPT_ITEM_STR    "ItemCl"
	#endif
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val0_b0", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 0);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val0_b1", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 1);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val0_b2", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 2);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val0_b3", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 3);
	
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val1_b0", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 4);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val1_b1", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 5);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val1_b2", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 6);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val1_b3", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 7);
	
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val2_b0", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 8);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val2_b1", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 9);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val2_b2", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 10);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val2_b3", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 11);
	
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val3_b0", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 12);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val3_b1", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 13);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val3_b2", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 14);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val3_b3", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 15);
	
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val4_b0", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 16);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val4_b1", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 17);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val4_b2", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 18);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val4_b3", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 19);
		
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val5_b0", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 20);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val5_b1", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 21);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val5_b2", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 22);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val5_b3", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 23);
	
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val6_b0", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 24);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val6_b1", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 25);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val6_b2", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 26);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val6_b3", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 27);
	
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val7_b0", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 28);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val7_b1", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 29);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val7_b2", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 30);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val7_b3", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 31);
	
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val8_b0", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 32);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val8_b1", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 33);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val8_b2", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 34);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val8_b3", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 35);
	
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val9_b0", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 36);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val9_b1", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 37);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val9_b2", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 38);
		ASEngine->RegisterObjectProperty(SCRIPT_ITEM_STR, "int8 val9_b3", asOFFSET(Item, Data) + asOFFSET(Item::_Data, ScriptValues) + 39);
}

FONLINE_DLL_ENTRY( isCompiler )
{
	RegisterASExtensions();
}

// Slot/parameters allowing

EXPORT bool allowSlot_Hand1( uint8, Item&, Critter&, Critter& toCr )
{
    return toCr.Params[ PE_AWARENESS ] != 0;
}
