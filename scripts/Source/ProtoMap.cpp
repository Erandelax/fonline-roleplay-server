#include "StdAfx.h"
#include "ProtoMap.h"
#include "ItemManager.h"
#include "Crypt.h"
#include "ConstantsManager.h"
#include <strstream>
#include "IniParser.h"

#ifdef FONLINE_MAPPER
# include "ResourceManager.h"
#endif

#define FO_MAP_VERSION_TEXT1                     ( 1 )
#define FO_MAP_VERSION_TEXT2                     ( 2 )
#define FO_MAP_VERSION_TEXT3                     ( 3 )
#define FO_MAP_VERSION_TEXT4                     ( 4 )
#define FO_MAP_VERSION_TEXT5                     ( 5 )

#define APP_HEADER                               "Header"
#define APP_TILES                                "Tiles"
#define APP_OBJECTS                              "Objects"

bool ProtoMap::Init( ushort pid, const char* name, int path_type )
{
    Clear();
    if( !name || !name[ 0 ] )
        return false;

    pmapPid = pid;
    pathType = path_type;
    pmapName = name;
    LastObjectUID = 0;

    isInit = true;
    if( !Refresh() )
    {
        Clear();
        return false;
    }
    return true;
}

void ProtoMap::Clear()
{
    #ifdef FONLINE_SERVER
    MEMORY_PROCESS( MEMORY_PROTO_MAP, -(int) SceneriesToSend.capacity() * sizeof( SceneryCl ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, -(int) WallsToSend.capacity() * sizeof( SceneryCl ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, -(int) mapEntires.capacity() * sizeof( MapEntire ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, -(int) CrittersVec.size() * sizeof( MapObject ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, -(int) ItemsVec.size() * sizeof( MapObject ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, -(int) SceneryVec.size() * sizeof( MapObject ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, -(int) GridsVec.size() * sizeof( MapObject ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, -(int) Header.MaxHexX * Header.MaxHexY );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, -(int) Tiles.capacity() * sizeof( MapEntire ) );

    SAFEDELA( HexFlags );

    for( auto it = CrittersVec.begin(), end = CrittersVec.end(); it != end; ++it )
        SAFEREL( *it );
    CrittersVec.clear();
    for( auto it = ItemsVec.begin(), end = ItemsVec.end(); it != end; ++it )
        SAFEREL( *it );
    ItemsVec.clear();
    for( auto it = SceneryVec.begin(), end = SceneryVec.end(); it != end; ++it )
        SAFEREL( *it );
    SceneryVec.clear();
    for( auto it = GridsVec.begin(), end = GridsVec.end(); it != end; ++it )
        SAFEREL( *it );
    GridsVec.clear();
    #endif

    for( auto it = MObjects.begin(), end = MObjects.end(); it != end; ++it )
    {
        MapObject* mobj = *it;
        #ifdef FONLINE_MAPPER
        mobj->RunTime.FromMap = NULL;
        mobj->RunTime.MapObjId = 0;
        #endif
        SAFEREL( mobj );
    }
    MObjects.clear();
    Tiles.clear();
    #ifdef FONLINE_MAPPER
    TilesField.clear();
    RoofsField.clear();
    #endif
    memzero( &Header, sizeof( Header ) );
    pmapName = "";
    pathType = 0;
    isInit = false;
}

bool ProtoMap::LoadTextFormat( const char* buf )
{
    IniParser map_ini;
    map_ini.LoadFilePtr( buf, Str::Length( buf ) );

    // Header
    memzero( &Header, sizeof( Header ) );
    char* header_str = map_ini.GetApp( APP_HEADER );
    if( header_str )
    {
        istrstream istr( header_str );
        string     field, value;
        int        ivalue;
        while( !istr.eof() && !istr.fail() )
        {
            istr >> field >> value;
            if( !istr.fail() )
            {
                ivalue = atoi( value.c_str() );
                if( field == "Version" )
                {
                    Header.Version = ivalue;
                    uint old_version = ( ivalue << 20 );
                    #define FO_MAP_VERSION_V6    ( 0xFE000000 )
                    #define FO_MAP_VERSION_V7    ( 0xFF000000 )
                    #define FO_MAP_VERSION_V8    ( 0xFF100000 )
                    #define FO_MAP_VERSION_V9    ( 0xFF200000 )
                    if( old_version == FO_MAP_VERSION_V6 || old_version == FO_MAP_VERSION_V7 ||
                        old_version == FO_MAP_VERSION_V8 || old_version == FO_MAP_VERSION_V9 )
                    {
                        Header.Version = FO_MAP_VERSION_TEXT1;
                        Header.DayTime[ 0 ] = 300;
                        Header.DayTime[ 1 ] = 600;
                        Header.DayTime[ 2 ] = 1140;
                        Header.DayTime[ 3 ] = 1380;
                        Header.DayColor[ 0 ] = 18;
                        Header.DayColor[ 1 ] = 128;
                        Header.DayColor[ 2 ] = 103;
                        Header.DayColor[ 3 ] = 51;
                        Header.DayColor[ 4 ] = 18;
                        Header.DayColor[ 5 ] = 128;
                        Header.DayColor[ 6 ] = 95;
                        Header.DayColor[ 7 ] = 40;
                        Header.DayColor[ 8 ] = 53;
                        Header.DayColor[ 9 ] = 128;
                        Header.DayColor[ 10 ] = 86;
                        Header.DayColor[ 11 ] = 29;
                    }
                }
                if( field == "MaxHexX" )
                    Header.MaxHexX = ivalue;
                if( field == "MaxHexY" )
                    Header.MaxHexY = ivalue;
                if( field == "WorkHexX" || field == "CenterX" )
                    Header.WorkHexX = ivalue;
                if( field == "WorkHexY" || field == "CenterY" )
                    Header.WorkHexY = ivalue;
                if( field == "Time" )
                    Header.Time = ivalue;
                if( field == "NoLogOut" )
                    Header.NoLogOut = ( ivalue != 0 );
                if( field == "ScriptModule" && value != "-" )
                    Str::Copy( Header.ScriptModule, value.c_str() );
                if( field == "ScriptFunc" && value != "-" )
                    Str::Copy( Header.ScriptFunc, value.c_str() );
                if( field == "DayTime" )
                {
                    Header.DayTime[ 0 ] = ivalue;
                    istr >> Header.DayTime[ 1 ];
                    istr >> Header.DayTime[ 2 ];
                    istr >> Header.DayTime[ 3 ];
                }
                if( field == "DayColor0" )
                {
                    Header.DayColor[ 0 ] = ivalue;
                    istr >> ivalue;
                    Header.DayColor[ 4 ] = ivalue;
                    istr >> ivalue;
                    Header.DayColor[ 8 ] = ivalue;
                }
                if( field == "DayColor1" )
                {
                    Header.DayColor[ 1 ] = ivalue;
                    istr >> ivalue;
                    Header.DayColor[ 5 ] = ivalue;
                    istr >> ivalue;
                    Header.DayColor[ 9 ] = ivalue;
                }
                if( field == "DayColor2" )
                {
                    Header.DayColor[ 2 ] = ivalue;
                    istr >> ivalue;
                    Header.DayColor[ 6 ] = ivalue;
                    istr >> ivalue;
                    Header.DayColor[ 10 ] = ivalue;
                }
                if( field == "DayColor3" )
                {
                    Header.DayColor[ 3 ] = ivalue;
                    istr >> ivalue;
                    Header.DayColor[ 7 ] = ivalue;
                    istr >> ivalue;
                    Header.DayColor[ 11 ] = ivalue;
                }
            }
        }
        delete[] header_str;
    }
    if( ( Header.Version != FO_MAP_VERSION_TEXT1 && Header.Version != FO_MAP_VERSION_TEXT2 &&
          Header.Version != FO_MAP_VERSION_TEXT3 && Header.Version != FO_MAP_VERSION_TEXT4 && Header.Version != FO_MAP_VERSION_TEXT5 ) ||
        Header.MaxHexX < 1 || Header.MaxHexY < 1 )
        return false;

    // Tiles
    char* tiles_str = map_ini.GetApp( APP_TILES );
    if( tiles_str )
    {
        istrstream istr( tiles_str );
        string     type;
        if( Header.Version == FO_MAP_VERSION_TEXT1 )
        {
            string name;

            // Deprecated
            while( !istr.eof() && !istr.fail() )
            {
                int hx, hy;
                istr >> type >> hx >> hy >> name;
                if( !istr.fail() && hx >= 0 && hx < Header.MaxHexX && hy >= 0 && hy < Header.MaxHexY )
                {
                    hx *= 2;
                    hy *= 2;
                    if( type == "tile" )
                        Tiles.push_back( Tile( Str::GetHash( name.c_str() ), hx, hy, 0, 0, 0, false ) );
                    else if( type == "roof" )
                        Tiles.push_back( Tile( Str::GetHash( name.c_str() ), hx, hy, 0, 0, 0, true ) );
                    else if( type == "0" )
                        Tiles.push_back( Tile( Str::AtoUI( name.c_str() ), hx, hy, 0, 0, 0, false ) );
                    else if( type == "1" )
                        Tiles.push_back( Tile( Str::AtoUI( name.c_str() ), hx, hy, 0, 0, 0, true ) );
                }
            }
        }
        else
        {
            char   name[ MAX_FOTEXT ];
            int    hx, hy;
            int    ox, oy, layer;
            bool   is_roof;
            size_t len;
            bool   has_offs;
            bool   has_layer;
            while( !istr.eof() && !istr.fail() )
            {
                istr >> type >> hx >> hy;
                if( !istr.fail() && hx >= 0 && hx < Header.MaxHexX && hy >= 0 && hy < Header.MaxHexY )
                {
                    if( !type.compare( 0, 4, "tile" ) )
                        is_roof = false;
                    else if( !type.compare( 0, 4, "roof" ) )
                        is_roof = true;
                    else
                        continue;

                    len = type.length();
                    has_offs = false;
                    has_layer = false;
                    if( len > 5 )
                    {
                        while( --len >= 5 )
                        {
                            switch( type[ len ] )
                            {
                            case 'o':
                                has_offs = true;
                                break;
                            case 'l':
                                has_layer = true;
                                break;
                            default:
                                break;
                            }
                        }
                    }

                    if( has_offs )
                        istr >> ox >> oy;
                    else
                        ox = oy = 0;
                    if( has_layer )
                        istr >> layer;
                    else
                        layer = 0;

                    istr.getline( name, MAX_FOTEXT );
                    Str::EraseFrontBackSpecificChars( name );

                    Tiles.push_back( Tile( Str::GetHash( name ), hx, hy, ox, oy, layer, is_roof ) );
                }
            }
        }
        delete[] tiles_str;
    }

    // Objects
    char* objects_str = map_ini.GetApp( APP_OBJECTS );
    if( objects_str )
    {
        istrstream istr( objects_str );
        string     field;
        char       svalue[ MAX_FOTEXT ];
        int        ivalue;
        int        critter_param_index = -1;
        while( !istr.eof() && !istr.fail() )
        {
            istr >> field;
            istr.getline( svalue, MAX_FOTEXT );

            if( !istr.fail() )
            {
                ivalue = Str::AtoI( svalue );

                if( field == "MapObjType" )
                {
                    MapObject* mobj = new MapObject();

                    mobj->MapObjType = ivalue;
                    if( ivalue == MAP_OBJECT_CRITTER )
                        mobj->MCritter.Cond = COND_LIFE;
                    else if( ivalue != MAP_OBJECT_ITEM && ivalue != MAP_OBJECT_SCENERY )
                        continue;

                    MObjects.push_back( mobj );
                }
                else if( MObjects.size() )
                {
                    MapObject& mobj = *MObjects.back();
                    // Shared
                    if( field == "ProtoId" )
                        mobj.ProtoId = ivalue;
                    else if( field == "MapX" )
                        mobj.MapX = ivalue;
                    else if( field == "MapY" )
                        mobj.MapY = ivalue;
                    else if( field == "UID" )
                        mobj.UID = ivalue;
                    else if( field == "ContainerUID" )
                        mobj.ContainerUID = ivalue;
                    else if( field == "ParentUID" )
                        mobj.ParentUID = ivalue;
                    else if( field == "ParentChildIndex" )
                        mobj.ParentChildIndex = ivalue;
                    else if( field == "LightColor" )
                        mobj.LightColor = ivalue;
                    else if( field == "LightDay" )
                        mobj.LightDay = ivalue;
                    else if( field == "LightDirOff" )
                        mobj.LightDirOff = ivalue;
                    else if( field == "LightDistance" )
                        mobj.LightDistance = ivalue;
                    else if( field == "LightIntensity" )
                        mobj.LightIntensity = ivalue;
                    else if( field == "ScriptName" )
                        Str::Copy( mobj.ScriptName, Str::EraseFrontBackSpecificChars( svalue ) );
                    else if( field == "FuncName" )
                        Str::Copy( mobj.FuncName, Str::EraseFrontBackSpecificChars( svalue ) );
                    else if( field == "UserData0" )
                        mobj.UserData[ 0 ] = ivalue;
                    else if( field == "UserData1" )
                        mobj.UserData[ 1 ] = ivalue;
                    else if( field == "UserData2" )
                        mobj.UserData[ 2 ] = ivalue;
                    else if( field == "UserData3" )
                        mobj.UserData[ 3 ] = ivalue;
                    else if( field == "UserData4" )
                        mobj.UserData[ 4 ] = ivalue;
                    else if( field == "UserData5" )
                        mobj.UserData[ 5 ] = ivalue;
                    else if( field == "UserData6" )
                        mobj.UserData[ 6 ] = ivalue;
                    else if( field == "UserData7" )
                        mobj.UserData[ 7 ] = ivalue;
                    else if( field == "UserData8" )
                        mobj.UserData[ 8 ] = ivalue;
                    else if( field == "UserData9" )
                        mobj.UserData[ 9 ] = ivalue;
                    // Critter
                    else if( mobj.MapObjType == MAP_OBJECT_CRITTER )
                    {
                        if( field == "Critter_Dir" || field == "Dir" )
                            mobj.MCritter.Dir = ivalue;
                        else if( field == "Critter_Cond" )
                            mobj.MCritter.Cond = ivalue;
                        else if( field == "Critter_Anim1" )
                            mobj.MCritter.Anim1 = ivalue;
                        else if( field == "Critter_Anim2" )
                            mobj.MCritter.Anim2 = ivalue;
                        else if( field == "Critter_CondExt" )
                            Deprecated_CondExtToAnim2( mobj.MCritter.Cond, ivalue, mobj.MCritter.Anim2, mobj.MCritter.Anim2 );                                                  // Deprecated
                        else if( field.size() >= 18 /* "Critter_ParamIndex" or "Critter_ParamValue" */ && field.substr( 0, 13 ) == "Critter_Param" )
                        {
                            if( field.substr( 13, 5 ) == "Index" )
                            {
                                critter_param_index = ConstantsManager::GetParamId( Str::EraseFrontBackSpecificChars( svalue ) );
                            }
                            else if( critter_param_index != -1 )
                            {
                                if( !mobj.MCritter.Params )
                                    mobj.AllocateCritterParams();
                                mobj.MCritter.Params[ critter_param_index ] = ivalue;
                                critter_param_index = -1;
                            }
                        }
                    }
                    // Item/Scenery
                    else if( mobj.MapObjType == MAP_OBJECT_ITEM || mobj.MapObjType == MAP_OBJECT_SCENERY )
                    {
                        // Shared parameters
                        if( field == "OffsetX" )
                            mobj.MItem.OffsetX = ivalue;
                        else if( field == "OffsetY" )
                            mobj.MItem.OffsetY = ivalue;
                        else if( field == "AnimStayBegin" )
                            mobj.MItem.AnimStayBegin = ivalue;
                        else if( field == "AnimStayEnd" )
                            mobj.MItem.AnimStayEnd = ivalue;
                        else if( field == "AnimWait" )
                            mobj.MItem.AnimWait = ivalue;
                        else if( field == "PicMapName" )
                        {
                            #ifdef FONLINE_MAPPER
                            Str::Copy( mobj.RunTime.PicMapName, Str::EraseFrontBackSpecificChars( svalue ) );
                            #endif
                            mobj.MItem.PicMapHash = Str::GetHash( Str::EraseFrontBackSpecificChars( svalue ) );
                        }
                        else if( field == "PicInvName" )
                        {
                            #ifdef FONLINE_MAPPER
                            Str::Copy( mobj.RunTime.PicInvName, Str::EraseFrontBackSpecificChars( svalue ) );
                            #endif
                            mobj.MItem.PicInvHash = Str::GetHash( Str::EraseFrontBackSpecificChars( svalue ) );
                        }
                        else if( field == "InfoOffset" )
                            mobj.MItem.InfoOffset = ivalue;
                        // Item
                        else if( mobj.MapObjType == MAP_OBJECT_ITEM )
                        {
                            if( field == "Item_InfoOffset" )
                                mobj.MItem.InfoOffset = ivalue;
                            else if( field == "Item_Count" )
                                mobj.MItem.Count = ivalue;
                            else if( field == "Item_BrokenFlags" )
                                mobj.MItem.BrokenFlags = ivalue;
                            else if( field == "Item_BrokenCount" )
                                mobj.MItem.BrokenCount = ivalue;
                            else if( field == "Item_Deterioration" )
                                mobj.MItem.Deterioration = ivalue;
                            else if( field == "Item_ItemSlot" )
                                mobj.MItem.ItemSlot = ivalue;
                            else if( field == "Item_AmmoPid" )
                                mobj.MItem.AmmoPid = ivalue;
                            else if( field == "Item_AmmoCount" )
                                mobj.MItem.AmmoCount = ivalue;
                            else if( field == "Item_LockerDoorId" )
                                mobj.MItem.LockerDoorId = ivalue;
                            else if( field == "Item_LockerCondition" )
                                mobj.MItem.LockerCondition = ivalue;
                            else if( field == "Item_LockerComplexity" )
                                mobj.MItem.LockerComplexity = ivalue;
                            else if( field == "Item_TrapValue" )
                                mobj.MItem.TrapValue = ivalue;
                            else if( field == "Item_Val0" )
                                mobj.MItem.Val[ 0 ] = ivalue;
                            else if( field == "Item_Val1" )
                                mobj.MItem.Val[ 1 ] = ivalue;
                            else if( field == "Item_Val2" )
                                mobj.MItem.Val[ 2 ] = ivalue;
                            else if( field == "Item_Val3" )
                                mobj.MItem.Val[ 3 ] = ivalue;
                            else if( field == "Item_Val4" )
                                mobj.MItem.Val[ 4 ] = ivalue;
                            else if( field == "Item_Val5" )
                                mobj.MItem.Val[ 5 ] = ivalue;
                            else if( field == "Item_Val6" )
                                mobj.MItem.Val[ 6 ] = ivalue;
                            else if( field == "Item_Val7" )
                                mobj.MItem.Val[ 7 ] = ivalue;
                            else if( field == "Item_Val8" )
                                mobj.MItem.Val[ 8 ] = ivalue;
                            else if( field == "Item_Val9" )
                                mobj.MItem.Val[ 9 ] = ivalue;
                            // Deprecated
                            else if( field == "Item_DeteorationFlags" )
                                mobj.MItem.BrokenFlags = ivalue;
                            else if( field == "Item_DeteorationCount" )
                                mobj.MItem.BrokenCount = ivalue;
                            else if( field == "Item_DeteorationValue" )
                                mobj.MItem.Deterioration = ivalue;
                            else if( field == "Item_InContainer" )
                                mobj.ContainerUID = ivalue;
                        }
                        // Scenery
                        else if( mobj.MapObjType == MAP_OBJECT_SCENERY )
                        {
                            if( field == "Scenery_CanUse" )
                                mobj.MScenery.CanUse = ( ivalue != 0 );
                            else if( field == "Scenery_CanTalk" )
                                mobj.MScenery.CanTalk = ( ivalue != 0 );
                            else if( field == "Scenery_TriggerNum" )
                                mobj.MScenery.TriggerNum = ivalue;
                            else if( field == "Scenery_ParamsCount" )
                                mobj.MScenery.ParamsCount = ivalue;
                            else if( field == "Scenery_Param0" )
                                mobj.MScenery.Param[ 0 ] = ivalue;
                            else if( field == "Scenery_Param1" )
                                mobj.MScenery.Param[ 1 ] = ivalue;
                            else if( field == "Scenery_Param2" )
                                mobj.MScenery.Param[ 2 ] = ivalue;
                            else if( field == "Scenery_Param3" )
                                mobj.MScenery.Param[ 3 ] = ivalue;
                            else if( field == "Scenery_Param4" )
                                mobj.MScenery.Param[ 4 ] = ivalue;
                            else if( field == "Scenery_ToMapPid" )
                                mobj.MScenery.ToMapPid = ivalue;
                            else if( field == "Scenery_ToEntire" )
                                mobj.MScenery.ToEntire = ivalue;
                            else if( field == "Scenery_ToDir" )
                                mobj.MScenery.ToDir = ivalue;
                            else if( field == "Scenery_SpriteCut" )
                                mobj.MScenery.SpriteCut = ivalue;
                        }
                    }
                }
            }
        }
        delete[] objects_str;
    }

    return true;
}

#ifdef FONLINE_MAPPER
void ProtoMap::SaveTextFormat( FileManager& fm )
{
    // Header
    fm.SetStr( "[%s]\n", APP_HEADER );
    fm.SetStr( "%-20s %d\n", "Version", Header.Version );
    fm.SetStr( "%-20s %d\n", "MaxHexX", Header.MaxHexX );
    fm.SetStr( "%-20s %d\n", "MaxHexY", Header.MaxHexY );
    fm.SetStr( "%-20s %d\n", "WorkHexX", Header.WorkHexX );
    fm.SetStr( "%-20s %d\n", "WorkHexY", Header.WorkHexY );
    fm.SetStr( "%-20s %s\n", "ScriptModule", Header.ScriptModule[ 0 ] ? Header.ScriptModule : "-" );
    fm.SetStr( "%-20s %s\n", "ScriptFunc", Header.ScriptFunc[ 0 ] ? Header.ScriptFunc : "-" );
    fm.SetStr( "%-20s %d\n", "NoLogOut", Header.NoLogOut );
    fm.SetStr( "%-20s %d\n", "Time", Header.Time );
    fm.SetStr( "%-20s %-4d %-4d %-4d %-4d\n", "DayTime", Header.DayTime[ 0 ], Header.DayTime[ 1 ], Header.DayTime[ 2 ], Header.DayTime[ 3 ] );
    fm.SetStr( "%-20s %-3d %-3d %-3d\n", "DayColor0", Header.DayColor[ 0 ], Header.DayColor[ 4 ], Header.DayColor[ 8 ] );
    fm.SetStr( "%-20s %-3d %-3d %-3d\n", "DayColor1", Header.DayColor[ 1 ], Header.DayColor[ 5 ], Header.DayColor[ 9 ] );
    fm.SetStr( "%-20s %-3d %-3d %-3d\n", "DayColor2", Header.DayColor[ 2 ], Header.DayColor[ 6 ], Header.DayColor[ 10 ] );
    fm.SetStr( "%-20s %-3d %-3d %-3d\n", "DayColor3", Header.DayColor[ 3 ], Header.DayColor[ 7 ], Header.DayColor[ 11 ] );
    fm.SetStr( "\n" );

    // Tiles
    fm.SetStr( "[%s]\n", APP_TILES );
    char tile_str[ 256 ];
    for( uint i = 0, j = (uint) Tiles.size(); i < j; i++ )
    {
        Tile&       tile = Tiles[ i ];
        const char* name = Str::GetName( tile.NameHash );
        if( name )
        {
            bool has_offs = ( tile.OffsX || tile.OffsY );
            bool has_layer = ( tile.Layer != 0 );

            Str::Copy( tile_str, tile.IsRoof ? "roof" : "tile" );
            if( has_offs || has_layer )
                Str::Append( tile_str, "_" );
            if( has_offs )
                Str::Append( tile_str, "o" );
            if( has_layer )
                Str::Append( tile_str, "l" );

            fm.SetStr( "%-10s %-4d %-4d ", tile_str, tile.HexX, tile.HexY );

            if( has_offs )
                fm.SetStr( "%-3d %-3d ", tile.OffsX, tile.OffsY );
            else
                fm.SetStr( "        " );

            if( has_layer )
                fm.SetStr( "%d ", tile.Layer );
            else
                fm.SetStr( "  " );

            fm.SetStr( "%s\n", name );
        }
    }
    fm.SetStr( "\n" );

    // Objects
    fm.SetStr( "[%s]\n", APP_OBJECTS );
    for( uint k = 0; k < MObjects.size(); k++ )
    {
        MapObject& mobj = *MObjects[ k ];
        // Shared
        fm.SetStr( "%-20s %d\n", "MapObjType", mobj.MapObjType );
        fm.SetStr( "%-20s %d\n", "ProtoId", mobj.ProtoId );
        if( mobj.MapX )
            fm.SetStr( "%-20s %d\n", "MapX", mobj.MapX );
        if( mobj.MapY )
            fm.SetStr( "%-20s %d\n", "MapY", mobj.MapY );
        if( mobj.UID )
            fm.SetStr( "%-20s %d\n", "UID", mobj.UID );
        if( mobj.ContainerUID )
            fm.SetStr( "%-20s %d\n", "ContainerUID", mobj.ContainerUID );
        if( mobj.ParentUID )
            fm.SetStr( "%-20s %d\n", "ParentUID", mobj.ParentUID );
        if( mobj.ParentChildIndex )
            fm.SetStr( "%-20s %d\n", "ParentChildIndex", mobj.ParentChildIndex );
        if( mobj.LightColor )
            fm.SetStr( "%-20s %d\n", "LightColor", mobj.LightColor );
        if( mobj.LightDay )
            fm.SetStr( "%-20s %d\n", "LightDay", mobj.LightDay );
        if( mobj.LightDirOff )
            fm.SetStr( "%-20s %d\n", "LightDirOff", mobj.LightDirOff );
        if( mobj.LightDistance )
            fm.SetStr( "%-20s %d\n", "LightDistance", mobj.LightDistance );
        if( mobj.LightIntensity )
            fm.SetStr( "%-20s %d\n", "LightIntensity", mobj.LightIntensity );
        if( mobj.ScriptName[ 0 ] )
            fm.SetStr( "%-20s %s\n", "ScriptName", mobj.ScriptName );
        if( mobj.FuncName[ 0 ] )
            fm.SetStr( "%-20s %s\n", "FuncName", mobj.FuncName );
        if( mobj.UserData[ 0 ] )
            fm.SetStr( "%-20s %d\n", "UserData0", mobj.UserData[ 0 ] );
        if( mobj.UserData[ 1 ] )
            fm.SetStr( "%-20s %d\n", "UserData1", mobj.UserData[ 1 ] );
        if( mobj.UserData[ 2 ] )
            fm.SetStr( "%-20s %d\n", "UserData2", mobj.UserData[ 2 ] );
        if( mobj.UserData[ 3 ] )
            fm.SetStr( "%-20s %d\n", "UserData3", mobj.UserData[ 3 ] );
        if( mobj.UserData[ 4 ] )
            fm.SetStr( "%-20s %d\n", "UserData4", mobj.UserData[ 4 ] );
        if( mobj.UserData[ 5 ] )
            fm.SetStr( "%-20s %d\n", "UserData5", mobj.UserData[ 5 ] );
        if( mobj.UserData[ 6 ] )
            fm.SetStr( "%-20s %d\n", "UserData6", mobj.UserData[ 6 ] );
        if( mobj.UserData[ 7 ] )
            fm.SetStr( "%-20s %d\n", "UserData7", mobj.UserData[ 7 ] );
        if( mobj.UserData[ 8 ] )
            fm.SetStr( "%-20s %d\n", "UserData8", mobj.UserData[ 8 ] );
        if( mobj.UserData[ 9 ] )
            fm.SetStr( "%-20s %d\n", "UserData9", mobj.UserData[ 9 ] );
        // Critter
        if( mobj.MapObjType == MAP_OBJECT_CRITTER )
        {
            fm.SetStr( "%-20s %d\n", "Critter_Dir", mobj.MCritter.Dir );
            fm.SetStr( "%-20s %d\n", "Critter_Cond", mobj.MCritter.Cond );
            fm.SetStr( "%-20s %d\n", "Critter_Anim1", mobj.MCritter.Anim1 );
            fm.SetStr( "%-20s %d\n", "Critter_Anim2", mobj.MCritter.Anim2 );
            if( mobj.MCritter.Params )
            {
                for( int i = 0; i < MAX_PARAMS; i++ )
                {
                    if( mobj.MCritter.Params[ i ] )
                    {
                        const char* param_name = ConstantsManager::GetParamName( i );
                        if( param_name )
                        {
                            fm.SetStr( "%-20s %s\n", "Critter_ParamIndex", param_name );
                            fm.SetStr( "%-20s %d\n", "Critter_ParamValue", mobj.MCritter.Params[ i ] );
                        }
                    }
                }
            }
        }
        // Item
        else if( mobj.MapObjType == MAP_OBJECT_ITEM || mobj.MapObjType == MAP_OBJECT_SCENERY )
        {
            if( mobj.MItem.OffsetX )
                fm.SetStr( "%-20s %d\n", "OffsetX", mobj.MItem.OffsetX );
            if( mobj.MItem.OffsetY )
                fm.SetStr( "%-20s %d\n", "OffsetY", mobj.MItem.OffsetY );
            if( mobj.MItem.AnimStayBegin )
                fm.SetStr( "%-20s %d\n", "AnimStayBegin", mobj.MItem.AnimStayBegin );
            if( mobj.MItem.AnimStayEnd )
                fm.SetStr( "%-20s %d\n", "AnimStayEnd", mobj.MItem.AnimStayEnd );
            if( mobj.MItem.AnimWait )
                fm.SetStr( "%-20s %d\n", "AnimWait", mobj.MItem.AnimWait );
            if( mobj.RunTime.PicMapName[ 0 ] )
                fm.SetStr( "%-20s %s\n", "PicMapName", mobj.RunTime.PicMapName );
            if( mobj.RunTime.PicInvName[ 0 ] )
                fm.SetStr( "%-20s %s\n", "PicInvName", mobj.RunTime.PicInvName );
            if( mobj.MItem.InfoOffset )
                fm.SetStr( "%-20s %d\n", "InfoOffset", mobj.MItem.InfoOffset );
            if( mobj.MapObjType == MAP_OBJECT_ITEM )
            {
                if( mobj.MItem.Count )
                    fm.SetStr( "%-20s %d\n", "Item_Count", mobj.MItem.Count );
                if( mobj.MItem.BrokenFlags )
                    fm.SetStr( "%-20s %d\n", "Item_BrokenFlags", mobj.MItem.BrokenFlags );
                if( mobj.MItem.BrokenCount )
                    fm.SetStr( "%-20s %d\n", "Item_BrokenCount", mobj.MItem.BrokenCount );
                if( mobj.MItem.Deterioration )
                    fm.SetStr( "%-20s %d\n", "Item_Deterioration", mobj.MItem.Deterioration );
                if( mobj.MItem.ItemSlot )
                    fm.SetStr( "%-20s %d\n", "Item_ItemSlot", mobj.MItem.ItemSlot );
                if( mobj.MItem.AmmoPid )
                    fm.SetStr( "%-20s %d\n", "Item_AmmoPid", mobj.MItem.AmmoPid );
                if( mobj.MItem.AmmoCount )
                    fm.SetStr( "%-20s %d\n", "Item_AmmoCount", mobj.MItem.AmmoCount );
                if( mobj.MItem.LockerDoorId )
                    fm.SetStr( "%-20s %d\n", "Item_LockerDoorId", mobj.MItem.LockerDoorId );
                if( mobj.MItem.LockerCondition )
                    fm.SetStr( "%-20s %d\n", "Item_LockerCondition", mobj.MItem.LockerCondition );
                if( mobj.MItem.LockerComplexity )
                    fm.SetStr( "%-20s %d\n", "Item_LockerComplexity", mobj.MItem.LockerComplexity );
                if( mobj.MItem.TrapValue )
                    fm.SetStr( "%-20s %d\n", "Item_TrapValue", mobj.MItem.TrapValue );
                if( mobj.MItem.Val[ 0 ] )
                    fm.SetStr( "%-20s %d\n", "Item_Val0", mobj.MItem.Val[ 0 ] );
                if( mobj.MItem.Val[ 1 ] )
                    fm.SetStr( "%-20s %d\n", "Item_Val1", mobj.MItem.Val[ 1 ] );
                if( mobj.MItem.Val[ 2 ] )
                    fm.SetStr( "%-20s %d\n", "Item_Val2", mobj.MItem.Val[ 2 ] );
                if( mobj.MItem.Val[ 3 ] )
                    fm.SetStr( "%-20s %d\n", "Item_Val3", mobj.MItem.Val[ 3 ] );
                if( mobj.MItem.Val[ 4 ] )
                    fm.SetStr( "%-20s %d\n", "Item_Val4", mobj.MItem.Val[ 4 ] );
                if( mobj.MItem.Val[ 5 ] )
                    fm.SetStr( "%-20s %d\n", "Item_Val5", mobj.MItem.Val[ 5 ] );
                if( mobj.MItem.Val[ 6 ] )
                    fm.SetStr( "%-20s %d\n", "Item_Val6", mobj.MItem.Val[ 6 ] );
                if( mobj.MItem.Val[ 7 ] )
                    fm.SetStr( "%-20s %d\n", "Item_Val7", mobj.MItem.Val[ 7 ] );
                if( mobj.MItem.Val[ 8 ] )
                    fm.SetStr( "%-20s %d\n", "Item_Val8", mobj.MItem.Val[ 8 ] );
                if( mobj.MItem.Val[ 9 ] )
                    fm.SetStr( "%-20s %d\n", "Item_Val9", mobj.MItem.Val[ 9 ] );
            }
            // Scenery
            else if( mobj.MapObjType == MAP_OBJECT_SCENERY )
            {
                if( mobj.MScenery.CanUse )
                    fm.SetStr( "%-20s %d\n", "Scenery_CanUse", mobj.MScenery.CanUse );
                if( mobj.MScenery.CanTalk )
                    fm.SetStr( "%-20s %d\n", "Scenery_CanTalk", mobj.MScenery.CanTalk );
                if( mobj.MScenery.TriggerNum )
                    fm.SetStr( "%-20s %d\n", "Scenery_TriggerNum", mobj.MScenery.TriggerNum );
                if( mobj.MScenery.ParamsCount )
                    fm.SetStr( "%-20s %d\n", "Scenery_ParamsCount", mobj.MScenery.ParamsCount );
                if( mobj.MScenery.Param[ 0 ] )
                    fm.SetStr( "%-20s %d\n", "Scenery_Param0", mobj.MScenery.Param[ 0 ] );
                if( mobj.MScenery.Param[ 1 ] )
                    fm.SetStr( "%-20s %d\n", "Scenery_Param1", mobj.MScenery.Param[ 1 ] );
                if( mobj.MScenery.Param[ 2 ] )
                    fm.SetStr( "%-20s %d\n", "Scenery_Param2", mobj.MScenery.Param[ 2 ] );
                if( mobj.MScenery.Param[ 3 ] )
                    fm.SetStr( "%-20s %d\n", "Scenery_Param3", mobj.MScenery.Param[ 3 ] );
                if( mobj.MScenery.Param[ 4 ] )
                    fm.SetStr( "%-20s %d\n", "Scenery_Param4", mobj.MScenery.Param[ 4 ] );
                if( mobj.MScenery.ToMapPid )
                    fm.SetStr( "%-20s %d\n", "Scenery_ToMapPid", mobj.MScenery.ToMapPid );
                if( mobj.MScenery.ToEntire )
                    fm.SetStr( "%-20s %d\n", "Scenery_ToEntire", mobj.MScenery.ToEntire );
                if( mobj.MScenery.ToDir )
                    fm.SetStr( "%-20s %d\n", "Scenery_ToDir", mobj.MScenery.ToDir );
                if( mobj.MScenery.SpriteCut )
                    fm.SetStr( "%-20s %d\n", "Scenery_SpriteCut", mobj.MScenery.SpriteCut );
            }
        }
        fm.SetStr( "\n" );
    }
    fm.SetStr( "\n" );
}
#endif

#ifdef FONLINE_SERVER
bool ProtoMap::LoadCache( FileManager& fm )
{
    // Server version
    uint version = fm.GetBEUInt();
    if( version != FONLINE_VERSION )
        return false;
    fm.GetBEUInt();
    fm.GetBEUInt();
    fm.GetBEUInt();

    // Header
    if( !fm.CopyMem( &Header, sizeof( Header ) ) )
        return false;

    // Tiles
    uint tiles_count = fm.GetBEUInt();
    if( tiles_count )
    {
        Tiles.resize( tiles_count );
        fm.CopyMem( &Tiles[ 0 ], tiles_count * sizeof( Tile ) );
    }

    // Critters
    uint count = fm.GetBEUInt();
    CrittersVec.reserve( count );
    for( uint i = 0; i < count; i++ )
    {
        MapObject* mobj = new MapObject();
        fm.CopyMem( mobj, sizeof( MapObject ) - sizeof( MapObject::_RunTime ) );
        if( mobj->MCritter.Params )
        {
            mobj->AllocateCritterParams();
            fm.CopyMem( mobj->MCritter.Params, sizeof( int ) * MAX_PARAMS );
        }
        CrittersVec.push_back( mobj );
    }

    // Items
    count = fm.GetBEUInt();
    ItemsVec.reserve( count );
    for( uint i = 0; i < count; i++ )
    {
        MapObject* mobj = new MapObject();
        fm.CopyMem( mobj, sizeof( MapObject ) - sizeof( MapObject::_RunTime ) );
        ItemsVec.push_back( mobj );
    }

    // Scenery
    count = fm.GetBEUInt();
    SceneryVec.reserve( count );
    for( uint i = 0; i < count; i++ )
    {
        MapObject* mobj = new MapObject();
        fm.CopyMem( mobj, sizeof( MapObject ) );
        SceneryVec.push_back( mobj );
        mobj->RunTime.RefCounter = 1;
        mobj->RunTime.BindScriptId = 0;
        if( mobj->ScriptName[ 0 ] && mobj->FuncName[ 0 ] )
            BindSceneryScript( mobj );
    }

    // Grids
    count = fm.GetBEUInt();
    GridsVec.reserve( count );
    for( uint i = 0; i < count; i++ )
    {
        MapObject* mobj = new MapObject();
        fm.CopyMem( mobj, sizeof( MapObject ) );
        GridsVec.push_back( mobj );
        mobj->RunTime.RefCounter = 1;
        mobj->RunTime.BindScriptId = 0;
    }

    // To send
    count = fm.GetBEUInt();
    if( count )
    {
        WallsToSend.resize( count );
        if( !fm.CopyMem( &WallsToSend[ 0 ], count * sizeof( SceneryCl ) ) )
            return false;
    }
    count = fm.GetBEUInt();
    if( count )
    {
        SceneriesToSend.resize( count );
        if( !fm.CopyMem( &SceneriesToSend[ 0 ], count * sizeof( SceneryCl ) ) )
            return false;
    }

    // Hashes
    HashTiles = fm.GetBEUInt();
    HashWalls = fm.GetBEUInt();
    HashScen = fm.GetBEUInt();

    // Hex flags
    HexFlags = new uchar[ Header.MaxHexX * Header.MaxHexY ];
    if( !HexFlags )
        return false;
    if( !fm.CopyMem( HexFlags, Header.MaxHexX * Header.MaxHexY ) )
        return false;

    // Entires
    count = fm.GetBEUInt();
    if( count )
    {
        mapEntires.resize( count );
        if( !fm.CopyMem( &mapEntires[ 0 ], count * sizeof( MapEntire ) ) )
            return false;
    }

    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) SceneriesToSend.capacity() * sizeof( SceneryCl ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) WallsToSend.capacity() * sizeof( SceneryCl ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) mapEntires.capacity() * sizeof( MapEntire ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) CrittersVec.size() * sizeof( MapObject ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) ItemsVec.size() * sizeof( MapObject ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) SceneryVec.size() * sizeof( MapObject ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) GridsVec.size() * sizeof( MapObject ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) Header.MaxHexX * Header.MaxHexY );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) Tiles.capacity() * sizeof( Tile ) );
    return true;
}

void ProtoMap::SaveCache( FileManager& fm )
{
    // Server version
    fm.SetBEUInt( FONLINE_VERSION );
    fm.SetBEUInt( 0 );
    fm.SetBEUInt( 0 );
    fm.SetBEUInt( 0 );

    // Header
    fm.SetData( &Header, sizeof( Header ) );

    // Tiles
    fm.SetBEUInt( (uint) Tiles.size() );
    if( Tiles.size() )
        fm.SetData( &Tiles[ 0 ], (uint) Tiles.size() * sizeof( Tile ) );

    // Critters
    fm.SetBEUInt( (uint) CrittersVec.size() );
    for( auto it = CrittersVec.begin(), end = CrittersVec.end(); it != end; ++it )
    {
        fm.SetData( *it, (uint) sizeof( MapObject ) - sizeof( MapObject::_RunTime ) );
        if( ( *it )->MCritter.Params )
            fm.SetData( ( *it )->MCritter.Params, sizeof( int ) * MAX_PARAMS );
    }

    // Items
    fm.SetBEUInt( (uint) ItemsVec.size() );
    for( auto it = ItemsVec.begin(), end = ItemsVec.end(); it != end; ++it )
        fm.SetData( *it, (uint) sizeof( MapObject ) - sizeof( MapObject::_RunTime ) );

    // Scenery
    fm.SetBEUInt( (uint) SceneryVec.size() );
    for( auto it = SceneryVec.begin(), end = SceneryVec.end(); it != end; ++it )
        fm.SetData( *it, (uint) sizeof( MapObject ) );

    // Grids
    fm.SetBEUInt( (uint) GridsVec.size() );
    for( auto it = GridsVec.begin(), end = GridsVec.end(); it != end; ++it )
        fm.SetData( *it, (uint) sizeof( MapObject ) );

    // To send
    fm.SetBEUInt( (uint) WallsToSend.size() );
    fm.SetData( &WallsToSend[ 0 ], (uint) WallsToSend.size() * sizeof( SceneryCl ) );
    fm.SetBEUInt( (uint) SceneriesToSend.size() );
    fm.SetData( &SceneriesToSend[ 0 ], (uint) SceneriesToSend.size() * sizeof( SceneryCl ) );

    // Hashes
    fm.SetBEUInt( HashTiles );
    fm.SetBEUInt( HashWalls );
    fm.SetBEUInt( HashScen );

    // Hex flags
    fm.SetData( HexFlags, Header.MaxHexX * Header.MaxHexY );

    // Entires
    fm.SetBEUInt( (uint) mapEntires.size() );
    fm.SetData( &mapEntires[ 0 ], (uint) mapEntires.size() * sizeof( MapEntire ) );

    // Save
    char fname[ MAX_FOPATH ];
    Str::Format( fname, "%s%sb", pmapName.c_str(), MAP_PROTO_EXT );
    fm.SaveOutBufToFile( fname, pathType );
}

void ProtoMap::BindSceneryScript( MapObject* mobj )
{
// ============================================================
    # define BIND_SCENERY_FUNC( params )                                                                                                         \
        if( mobj->ProtoId != SP_SCEN_TRIGGER )                                                                                                   \
            mobj->RunTime.BindScriptId = Script::Bind( mobj->ScriptName, mobj->FuncName, "bool %s(Critter&,Scenery&,int,Item@" params, false );  \
        else                                                                                                                                     \
            mobj->RunTime.BindScriptId = Script::Bind( mobj->ScriptName, mobj->FuncName, "void %s(Critter&,Scenery&,bool,uint8" params, false )
// ============================================================

    switch( mobj->MScenery.ParamsCount )
    {
    case 1:
        BIND_SCENERY_FUNC( ",int)" );
        break;
    case 2:
        BIND_SCENERY_FUNC( ",int,int)" );
        break;
    case 3:
        BIND_SCENERY_FUNC( ",int,int,int)" );
        break;
    case 4:
        BIND_SCENERY_FUNC( ",int,int,int,int)" );
        break;
    case 5:
        BIND_SCENERY_FUNC( ",int,int,int,int,int)" );
        break;
    default:
        BIND_SCENERY_FUNC( ")" );
        break;
    }

    if( mobj->RunTime.BindScriptId <= 0 )
    {
        char map_info[ MAX_FOTEXT ];
        Str::Format( map_info, "pid<%u>, name<%s>", GetPid(), pmapName.c_str() );
        WriteLogF( _FUNC_, " - Map<%s>, Can't bind scenery function<%s> in module<%s>. Scenery hexX<%u>, hexY<%u>.\n", map_info,
                   mobj->FuncName, mobj->ScriptName, mobj->MapX, mobj->MapY );
        mobj->RunTime.BindScriptId = 0;
    }
}
#endif // FONLINE_SERVER

bool ProtoMap::Refresh()
{
    if( !IsInit() )
        return false;

    char map_info[ MAX_FOTEXT ];
    Str::Format( map_info, "pid<%u>, name<%s>", GetPid(), pmapName.c_str() );

    // Read
    string fname_txt = pmapName + MAP_PROTO_EXT;
    string fname_bin = pmapName + string( MAP_PROTO_EXT ) + "b";
    string fname_map = pmapName + ".map"; // Deprecated

    #ifdef FONLINE_SERVER
    // Cached binary
    FileManager cached;
    cached.LoadFile( fname_bin.c_str(), pathType );

    // Load text or binary
    FileManager fm;
    bool        text = true;
    if( !fm.LoadFile( fname_txt.c_str(), pathType ) )
    {
        text = false;
        if( !fm.LoadFile( fname_map.c_str(), pathType ) && !cached.IsLoaded() )
        {
            WriteLogF( _FUNC_, " - Load file<%s> fail.\n", FileManager::GetDataPath( pmapName.c_str(), pathType ) );
            return false;
        }
    }

    // Process cache
    if( cached.IsLoaded() )
    {
        bool load_cache = true;
        if( fm.IsLoaded() )
        {
            uint64 last_write = fm.GetWriteTime();
            uint64 last_write_cache = cached.GetWriteTime();
            if( last_write > last_write_cache )
                load_cache = false;
        }

        if( load_cache )
        {
            if( LoadCache( cached ) )
                return true;

            if( !fm.IsLoaded() )
            {
                WriteLogF( _FUNC_, " - Map<%s>. Can't read cached map file.\n", map_info );
                return false;
            }
        }
    }
    #endif // FONLINE_SERVER
    #ifdef FONLINE_MAPPER
    // Load binary or text
    FileManager fm;
    bool        text = true;
    if( !fm.LoadFile( fname_txt.c_str(), pathType ) )
    {
        text = false;
        if( !fm.LoadFile( fname_map.c_str(), pathType ) )
        {
            WriteLogF( _FUNC_, " - Load file<%s> fail.\n", FileManager::GetDataPath( pmapName.c_str(), pathType ) );
            return false;
        }
    }
    #endif // FONLINE_MAPPER

    // Load
    if( text )
    {
        if( !LoadTextFormat( (const char*) fm.GetBuf() ) )
        {
            WriteLogF( _FUNC_, " - Map<%s>. Can't read text map format.\n", map_info );
            return false;
        }
    }
    else
    {
        WriteLogF( _FUNC_, " - Map<%s>. Binary format is not supported anymore, resave map in earliest version.\n", map_info );
        return false;
    }
    fm.UnloadFile();

    // Deprecated, add UIDs
    if( Header.Version < FO_MAP_VERSION_TEXT4 )
    {
        uint uid = 0;
        for( uint i = 0, j = (uint) MObjects.size(); i < j; i++ )
        {
            MapObject* mobj = MObjects[ i ];

            // Find item in container
            if( mobj->MapObjType != MAP_OBJECT_ITEM || !mobj->ContainerUID )
                continue;

            // Find container
            for( uint k = 0, l = (uint) MObjects.size(); k < l; k++ )
            {
                MapObject* mobj_ = MObjects[ k ];
                if( mobj_->MapX != mobj->MapX || mobj_->MapY != mobj->MapY )
                    continue;
                if( mobj_->MapObjType != MAP_OBJECT_ITEM && mobj_->MapObjType != MAP_OBJECT_CRITTER )
                    continue;
                if( mobj_ == mobj )
                    continue;
                if( mobj_->MapObjType == MAP_OBJECT_ITEM )
                {
                    ProtoItem* proto_item = ItemMngr.GetProtoItem( mobj_->ProtoId );
                    if( !proto_item || proto_item->Type != ITEM_TYPE_CONTAINER )
                        continue;
                }
                if( !mobj_->UID )
                    mobj_->UID = ++uid;
                mobj->ContainerUID = mobj_->UID;
            }
        }
    }

    // Fix child objects positions
    for( uint i = 0, j = (uint) MObjects.size(); i < j;)
    {
        MapObject* mobj_child = MObjects[ i ];
        if( !mobj_child->ParentUID )
        {
            i++;
            continue;
        }

        bool delete_child = true;
        for( uint k = 0, l = (uint) MObjects.size(); k < l; k++ )
        {
            MapObject* mobj_parent = MObjects[ k ];
            if( !mobj_parent->UID || mobj_parent->UID != mobj_child->ParentUID || mobj_parent == mobj_child )
                continue;

            ProtoItem* proto_parent = ItemMngr.GetProtoItem( mobj_parent->ProtoId );
            if( !proto_parent || !proto_parent->ChildPid[ mobj_child->ParentChildIndex ] )
                break;

            ushort child_hx = mobj_parent->MapX, child_hy = mobj_parent->MapY;
            FOREACH_PROTO_ITEM_LINES( proto_parent->ChildLines[ mobj_child->ParentChildIndex ], child_hx, child_hy, Header.MaxHexX, Header.MaxHexY,;
                                      );

            mobj_child->MapX = child_hx;
            mobj_child->MapY = child_hy;
            mobj_child->ProtoId = proto_parent->ChildPid[ mobj_child->ParentChildIndex ];
            delete_child = false;
            break;
        }

        if( delete_child )
        {
            MObjects[ i ]->Release();
            MObjects.erase( MObjects.begin() + i );
            j = (uint) MObjects.size();
        }
        else
        {
            i++;
        }
    }

    #ifdef FONLINE_SERVER
    // Parse objects
    WallsToSend.clear();
    SceneriesToSend.clear();
    ushort maxhx = Header.MaxHexX;
    ushort maxhy = Header.MaxHexY;

    HexFlags = new uchar[ Header.MaxHexX * Header.MaxHexY ];
    if( !HexFlags )
        return false;
    memzero( HexFlags, Header.MaxHexX * Header.MaxHexY );

    for( auto it = MObjects.begin(), end = MObjects.end(); it != end; ++it )
    {
        MapObject& mobj = *( *it );

        if( mobj.MapObjType == MAP_OBJECT_CRITTER )
        {
            mobj.AddRef();
            CrittersVec.push_back( &mobj );
            continue;
        }
        else if( mobj.MapObjType == MAP_OBJECT_ITEM )
        {
            mobj.AddRef();
            ItemsVec.push_back( &mobj );
            continue;
        }

        ushort     pid = mobj.ProtoId;
        ushort     hx = mobj.MapX;
        ushort     hy = mobj.MapY;

        ProtoItem* proto_item = ItemMngr.GetProtoItem( pid );
        if( !proto_item )
        {
            WriteLogF( _FUNC_, " - Map<%s>, Unknown prototype<%u>, hexX<%u>, hexY<%u>.\n", map_info, pid, hx, hy );
            continue;
        }

        if( hx >= maxhx || hy >= maxhy )
        {
            WriteLogF( _FUNC_, " - Invalid object position on map<%s>, pid<%u>, hexX<%u>, hexY<%u>.\n", map_info, pid, hx, hy );
            continue;
        }

        int type = proto_item->Type;
        switch( type )
        {
        case ITEM_TYPE_WALL:
        {
            if( !FLAG( proto_item->Flags, ITEM_NO_BLOCK ) )
                SETFLAG( HexFlags[ hy * maxhx + hx ], FH_BLOCK );
            if( !FLAG( proto_item->Flags, ITEM_SHOOT_THRU ) )
            {
                SETFLAG( HexFlags[ hy * maxhx + hx ], FH_BLOCK );
                SETFLAG( HexFlags[ hy * maxhx + hx ], FH_NOTRAKE );
            }

            SETFLAG( HexFlags[ hy * maxhx + hx ], FH_WALL );

            // To client
            SceneryCl cur_wall;
            memzero( &cur_wall, sizeof( SceneryCl ) );

            cur_wall.ProtoId = mobj.ProtoId;
            cur_wall.MapX = mobj.MapX;
            cur_wall.MapY = mobj.MapY;
            cur_wall.OffsetX = mobj.MScenery.OffsetX;
            cur_wall.OffsetY = mobj.MScenery.OffsetY;
            cur_wall.LightColor = mobj.LightColor;
            cur_wall.LightDistance = mobj.LightDistance;
            cur_wall.LightFlags = mobj.LightDirOff | ( ( mobj.LightDay & 3 ) << 6 );
            cur_wall.LightIntensity = mobj.LightIntensity;
            cur_wall.InfoOffset = mobj.MScenery.InfoOffset;
            cur_wall.AnimStayBegin = mobj.MScenery.AnimStayBegin;
            cur_wall.AnimStayEnd = mobj.MScenery.AnimStayEnd;
            cur_wall.AnimWait = mobj.MScenery.AnimWait;
            cur_wall.PicMapHash = mobj.MScenery.PicMapHash;
            cur_wall.SpriteCut = mobj.MScenery.SpriteCut;

            WallsToSend.push_back( cur_wall );
        }
        break;
        case ITEM_TYPE_GENERIC:
        case ITEM_TYPE_GRID:
        {
            if( pid == SP_GRID_ENTIRE )
            {
                mapEntires.push_back( MapEntire( hx, hy, mobj.MScenery.ToDir, mobj.MScenery.ToEntire ) );
                continue;
            }

            if( type == ITEM_TYPE_GRID )
                SETFLAG( HexFlags[ hy * maxhx + hx ], FH_SCEN_GRID );
            if( !FLAG( proto_item->Flags, ITEM_NO_BLOCK ) )
                SETFLAG( HexFlags[ hy * maxhx + hx ], FH_BLOCK );
            if( !FLAG( proto_item->Flags, ITEM_SHOOT_THRU ) )
            {
                SETFLAG( HexFlags[ hy * maxhx + hx ], FH_BLOCK );
                SETFLAG( HexFlags[ hy * maxhx + hx ], FH_NOTRAKE );
            }
            SETFLAG( HexFlags[ hy * maxhx + hx ], FH_SCEN );

            // To server
            if( pid == SP_MISC_SCRBLOCK )
            {
                // Block around
                for( int k = 0; k < 6; k++ )
                {
                    ushort hx_ = hx, hy_ = hy;
                    MoveHexByDir( hx_, hy_, k, Header.MaxHexX, Header.MaxHexY );
                    SETFLAG( HexFlags[ hy_ * maxhx + hx_ ], FH_BLOCK );
                }
            }
            else if( type == ITEM_TYPE_GRID )
            {
                mobj.AddRef();
                GridsVec.push_back( &mobj );
            }
            else                     // ITEM_TYPE_GENERIC
            {
                // Bind script
                const char* script = ItemMngr.GetProtoScript( pid );
                if( script )
                {
                    char script_module[ MAX_SCRIPT_NAME + 1 ];
                    char script_func[ MAX_SCRIPT_NAME + 1 ];
                    if( Script::ReparseScriptName( script, script_module, script_func ) )
                    {
                        Str::Copy( mobj.ScriptName, script_module );
                        Str::Copy( mobj.FuncName, script_func );
                    }
                }

                mobj.RunTime.BindScriptId = 0;
                if( mobj.ScriptName[ 0 ] && mobj.FuncName[ 0 ] )
                    BindSceneryScript( &mobj );

                // Add to collection
                mobj.AddRef();
                SceneryVec.push_back( &mobj );
            }

            if( pid == SP_SCEN_TRIGGER )
            {
                if( mobj.RunTime.BindScriptId )
                    SETFLAG( HexFlags[ hy * maxhx + hx ], FH_TRIGGER );
                continue;
            }

            // To client
            SceneryCl cur_scen;
            memzero( &cur_scen, sizeof( SceneryCl ) );

            // Flags
            if( type == ITEM_TYPE_GENERIC && mobj.MScenery.CanUse )
                SETFLAG( cur_scen.Flags, SCEN_CAN_USE );
            if( type == ITEM_TYPE_GENERIC && mobj.MScenery.CanTalk )
                SETFLAG( cur_scen.Flags, SCEN_CAN_TALK );
            if( type == ITEM_TYPE_GRID && proto_item->Grid_Type != GRID_EXITGRID )
                SETFLAG( cur_scen.Flags, SCEN_CAN_USE );

            // Other
            cur_scen.ProtoId = mobj.ProtoId;
            cur_scen.MapX = mobj.MapX;
            cur_scen.MapY = mobj.MapY;
            cur_scen.OffsetX = mobj.MScenery.OffsetX;
            cur_scen.OffsetY = mobj.MScenery.OffsetY;
            cur_scen.LightColor = mobj.LightColor;
            cur_scen.LightDistance = mobj.LightDistance;
            cur_scen.LightFlags = mobj.LightDirOff | ( ( mobj.LightDay & 3 ) << 6 );
            cur_scen.LightIntensity = mobj.LightIntensity;
            cur_scen.InfoOffset = mobj.MScenery.InfoOffset;
            cur_scen.AnimStayBegin = mobj.MScenery.AnimStayBegin;
            cur_scen.AnimStayEnd = mobj.MScenery.AnimStayEnd;
            cur_scen.AnimWait = mobj.MScenery.AnimWait;
            cur_scen.PicMapHash = mobj.MScenery.PicMapHash;
            cur_scen.SpriteCut = mobj.MScenery.SpriteCut;

            SceneriesToSend.push_back( cur_scen );
        }
        break;
        default:
            break;
        }
    }

    for( auto it = MObjects.begin(), end = MObjects.end(); it != end; ++it )
        SAFEREL( *it );
    MapObjectPtrVec().swap( MObjects );

    // Generate hashes
    HashTiles = maxhx * maxhy;
    if( Tiles.size() )
        Crypt.Crc32( (uchar*) &Tiles[ 0 ], (uint) Tiles.size() * sizeof( Tile ), HashTiles );
    HashWalls = maxhx * maxhy;
    if( WallsToSend.size() )
        Crypt.Crc32( (uchar*) &WallsToSend[ 0 ], (uint) WallsToSend.size() * sizeof( SceneryCl ), HashWalls );
    HashScen = maxhx * maxhy;
    if( SceneriesToSend.size() )
        Crypt.Crc32( (uchar*) &SceneriesToSend[ 0 ], (uint) SceneriesToSend.size() * sizeof( SceneryCl ), HashScen );

    // Shrink the vector capacities to fit their contents and reduce memory use
    SceneryClVec( SceneriesToSend ).swap( SceneriesToSend );
    SceneryClVec( WallsToSend ).swap( WallsToSend );
    EntiresVec( mapEntires ).swap( mapEntires );
    MapObjectPtrVec( CrittersVec ).swap( CrittersVec );
    MapObjectPtrVec( ItemsVec ).swap( ItemsVec );
    MapObjectPtrVec( SceneryVec ).swap( SceneryVec );
    MapObjectPtrVec( GridsVec ).swap( GridsVec );
    TileVec( Tiles ).swap( Tiles );

    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) SceneriesToSend.capacity() * sizeof( SceneryCl ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) WallsToSend.capacity() * sizeof( SceneryCl ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) mapEntires.capacity() * sizeof( MapEntire ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) CrittersVec.size() * sizeof( MapObject ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) ItemsVec.size() * sizeof( MapObject ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) SceneryVec.size() * sizeof( MapObject ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) GridsVec.size() * sizeof( MapObject ) );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) Header.MaxHexX * Header.MaxHexY );
    MEMORY_PROCESS( MEMORY_PROTO_MAP, (int) Tiles.capacity() * sizeof( Tile ) );

    SaveCache( fm );
    #endif

    #ifdef FONLINE_MAPPER
    // Post process objects
    for( uint i = 0, j = (uint) MObjects.size(); i < j; i++ )
    {
        MapObject* mobj = MObjects[ i ];

        // Map link
        mobj->RunTime.FromMap = this;

        // Convert hashes to names
        if( mobj->MapObjType == MAP_OBJECT_ITEM || mobj->MapObjType == MAP_OBJECT_SCENERY )
        {
            if( mobj->MItem.PicMapHash && !mobj->RunTime.PicMapName[ 0 ] )
                Str::Copy( mobj->RunTime.PicMapName, Str::GetName( mobj->MItem.PicMapHash ) );
            if( mobj->MItem.PicInvHash && !mobj->RunTime.PicInvName[ 0 ] )
                Str::Copy( mobj->RunTime.PicInvName, Str::GetName( mobj->MItem.PicInvHash ) );
        }

        // Last UID
        if( mobj->UID > LastObjectUID )
            LastObjectUID = mobj->UID;
    }

    // Create cached fields
    TilesField.resize( Header.MaxHexX * Header.MaxHexY );
    RoofsField.resize( Header.MaxHexX * Header.MaxHexY );
    for( uint i = 0, j = (uint) Tiles.size(); i < j; i++ )
    {
        if( !Tiles[ i ].IsRoof )
        {
            TilesField[ Tiles[ i ].HexY * Header.MaxHexX + Tiles[ i ].HexX ].push_back( Tiles[ i ] );
            TilesField[ Tiles[ i ].HexY * Header.MaxHexX + Tiles[ i ].HexX ].back().IsSelected = false;
        }
        else
        {
            RoofsField[ Tiles[ i ].HexY * Header.MaxHexX + Tiles[ i ].HexX ].push_back( Tiles[ i ] );
            RoofsField[ Tiles[ i ].HexY * Header.MaxHexX + Tiles[ i ].HexX ].back().IsSelected = false;
        }
    }
    Tiles.clear();
    TileVec( Tiles ).swap( Tiles );
    #endif
    return true;
}

#ifdef FONLINE_MAPPER
void ProtoMap::GenNew()
{
    Clear();
    Header.Version = FO_MAP_VERSION_TEXT5;
    Header.MaxHexX = MAXHEX_DEF;
    Header.MaxHexY = MAXHEX_DEF;
    pmapPid = 0xFFFF;
    pathType = PT_SERVER_MAPS;

    // Morning	 5.00 -  9.59	 300 - 599
    // Day		10.00 - 18.59	 600 - 1139
    // Evening	19.00 - 22.59	1140 - 1379
    // Nigh		23.00 -  4.59	1380
    Header.DayTime[ 0 ] = 300;
    Header.DayTime[ 1 ] = 600;
    Header.DayTime[ 2 ] = 1140;
    Header.DayTime[ 3 ] = 1380;
    Header.DayColor[ 0 ] = 18;
    Header.DayColor[ 1 ] = 128;
    Header.DayColor[ 2 ] = 103;
    Header.DayColor[ 3 ] = 51;
    Header.DayColor[ 4 ] = 18;
    Header.DayColor[ 5 ] = 128;
    Header.DayColor[ 6 ] = 95;
    Header.DayColor[ 7 ] = 40;
    Header.DayColor[ 8 ] = 53;
    Header.DayColor[ 9 ] = 128;
    Header.DayColor[ 10 ] = 86;
    Header.DayColor[ 11 ] = 29;

    # ifdef FONLINE_MAPPER
    TilesField.resize( Header.MaxHexX * Header.MaxHexY );
    RoofsField.resize( Header.MaxHexX * Header.MaxHexY );
    # endif

    isInit = true;
}

bool ProtoMap::Save( const char* fname, int path_type, bool keep_name /* = false */ )
{
    string pmap_name_old = pmapName;

    if( fname && *fname )
        pmapName = fname;
    if( path_type >= 0 )
        pathType = path_type;

    // Fill tiles from cached fields
    TilesField.resize( Header.MaxHexX * Header.MaxHexY );
    RoofsField.resize( Header.MaxHexX * Header.MaxHexY );
    for( ushort hy = 0; hy < Header.MaxHexY; hy++ )
    {
        for( ushort hx = 0; hx < Header.MaxHexX; hx++ )
        {
            TileVec& tiles = TilesField[ hy * Header.MaxHexX + hx ];
            for( uint i = 0, j = (uint) tiles.size(); i < j; i++ )
                Tiles.push_back( tiles[ i ] );
            TileVec& roofs = RoofsField[ hy * Header.MaxHexX + hx ];
            for( uint i = 0, j = (uint) roofs.size(); i < j; i++ )
                Tiles.push_back( roofs[ i ] );
        }
    }

    // Delete non used UIDs
    for( uint i = 0, j = (uint) MObjects.size(); i < j; i++ )
    {
        MapObject* mobj = MObjects[ i ];
        if( !mobj->UID )
            continue;

        bool founded = false;
        for( uint k = 0, l = (uint) MObjects.size(); k < l; k++ )
        {
            MapObject* mobj_ = MObjects[ k ];
            if( mobj_->ContainerUID == mobj->UID || mobj_->ParentUID == mobj->UID )
            {
                founded = true;
                break;
            }
        }
        if( !founded )
            mobj->UID = 0;
    }

    // Save
    FileManager fm;
    Header.Version = FO_MAP_VERSION_TEXT5;
    SaveTextFormat( fm );
    Tiles.clear();

    string save_fname = pmapName + MAP_PROTO_EXT;
    if( keep_name )
        pmapName = pmap_name_old;

    if( !fm.SaveOutBufToFile( save_fname.c_str(), pathType ) )
    {
        WriteLogF( _FUNC_, " - Unable write file.\n" );
        fm.ClearOutBuf();
        return false;
    }

    fm.ClearOutBuf();
    return true;
}

bool ProtoMap::IsMapFile( const char* fname )
{
    char* ext = (char*) FileManager::GetExtension( fname );
    if( !ext )
        return false;
    ext--;

    if( Str::CompareCase( ext, MAP_PROTO_EXT ) )
    {
        // Check text format
        IniParser txt;
        if( !txt.LoadFile( fname, PT_ROOT ) )
            return false;
        return txt.IsApp( APP_HEADER ) && txt.IsApp( APP_TILES ) && txt.IsApp( APP_OBJECTS );
    }

    return false;
}
#endif // FONLINE_MAPPER

#ifdef FONLINE_SERVER
uint ProtoMap::CountEntire( uint num )
{
    if( num == uint( -1 ) )
        return (uint) mapEntires.size();

    uint result = 0;
    for( uint i = 0, j = (uint) mapEntires.size(); i < j; i++ )
    {
        if( mapEntires[ i ].Number == num )
            result++;
    }
    return result;
}

ProtoMap::MapEntire* ProtoMap::GetEntire( uint num, uint skip )
{
    for( uint i = 0, j = (uint) mapEntires.size(); i < j; i++ )
    {
        if( num == uint( -1 ) || mapEntires[ i ].Number == num )
        {
            if( !skip )
                return &mapEntires[ i ];
            else
                skip--;
        }
    }

    return NULL;
}

ProtoMap::MapEntire* ProtoMap::GetEntireRandom( uint num )
{
    vector< MapEntire* > entires;
    for( uint i = 0, j = (uint) mapEntires.size(); i < j; i++ )
    {
        if( num == uint( -1 ) || mapEntires[ i ].Number == num )
            entires.push_back( &mapEntires[ i ] );
    }

    if( entires.empty() )
        return NULL;
    return entires[ Random( 0, (uint) entires.size() - 1 ) ];
}

ProtoMap::MapEntire* ProtoMap::GetEntireNear( uint num, ushort hx, ushort hy )
{
    MapEntire* near_entire = NULL;
    uint       last_dist = 0;
    for( uint i = 0, j = (uint) mapEntires.size(); i < j; i++ )
    {
        MapEntire& entire = mapEntires[ i ];
        if( num == uint( -1 ) || entire.Number == num )
        {
            uint dist = DistGame( hx, hy, entire.HexX, entire.HexY );
            if( !near_entire || dist < last_dist )
            {
                near_entire = &entire;
                last_dist = dist;
            }
        }
    }
    return near_entire;
}

ProtoMap::MapEntire* ProtoMap::GetEntireNear( uint num, uint num_ext, ushort hx, ushort hy )
{
    MapEntire* near_entire = NULL;
    uint       last_dist = 0;
    for( uint i = 0, j = (uint) mapEntires.size(); i < j; i++ )
    {
        MapEntire& entire = mapEntires[ i ];
        if( num == uint( -1 ) || num_ext == uint( -1 ) || entire.Number == num || entire.Number == num_ext )
        {
            uint dist = DistGame( hx, hy, entire.HexX, entire.HexY );
            if( !near_entire || dist < last_dist )
            {
                near_entire = &entire;
                last_dist = dist;
            }
        }
    }
    return near_entire;
}

void ProtoMap::GetEntires( uint num, EntiresVec& entires )
{
    for( uint i = 0, j = (uint) mapEntires.size(); i < j; i++ )
    {
        MapEntire& entire = mapEntires[ i ];
        if( num == uint( -1 ) || entire.Number == num )
            entires.push_back( entire );
    }
}

MapObject* ProtoMap::GetMapScenery( ushort hx, ushort hy, ushort pid )
{
    for( auto it = SceneryVec.begin(), end = SceneryVec.end(); it != end; ++it )
    {
        MapObject* mobj = *it;
        if( ( !pid || mobj->ProtoId == pid ) && mobj->MapX == hx && mobj->MapY == hy )
            return mobj;
    }
    return NULL;
}

void ProtoMap::GetMapSceneriesHex( ushort hx, ushort hy, MapObjectPtrVec& mobjs )
{
    for( auto it = SceneryVec.begin(), end = SceneryVec.end(); it != end; ++it )
    {
        MapObject* mobj = *it;
        if( mobj->MapX == hx && mobj->MapY == hy )
            mobjs.push_back( mobj );
    }
}

void ProtoMap::GetMapSceneriesHexEx( ushort hx, ushort hy, uint radius, ushort pid, MapObjectPtrVec& mobjs )
{
    for( auto it = SceneryVec.begin(), end = SceneryVec.end(); it != end; ++it )
    {
        MapObject* mobj = *it;
        if( ( !pid || mobj->ProtoId == pid ) && DistGame( mobj->MapX, mobj->MapY, hx, hy ) <= radius )
            mobjs.push_back( mobj );
    }
}

void ProtoMap::GetMapSceneriesByPid( ushort pid, MapObjectPtrVec& mobjs )
{
    for( auto it = SceneryVec.begin(), end = SceneryVec.end(); it != end; ++it )
    {
        MapObject* mobj = *it;
        if( !pid || mobj->ProtoId == pid )
            mobjs.push_back( mobj );
    }
}

MapObject* ProtoMap::GetMapGrid( ushort hx, ushort hy )
{
    for( auto it = GridsVec.begin(), end = GridsVec.end(); it != end; ++it )
    {
        MapObject* mobj = *it;
        if( mobj->MapX == hx && mobj->MapY == hy )
            return mobj;
    }
    return NULL;
}

#endif // FONLINE_SERVER
