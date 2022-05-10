#include "../plugin_sdk/plugin_sdk.hpp"
#include "Jax.h"

PLUGIN_NAME( "zBGAIO" );

SUPPORTED_CHAMPIONS(champion_id::Jax)

PLUGIN_API bool on_sdk_load( plugin_sdk_core* plugin_sdk_good )
{
    DECLARE_GLOBALS( plugin_sdk_good );

    switch (myhero->get_champion())
    {
    case champion_id::Jax:
        Jax::load();
        break;
    default:
        console->print("Champion not supported detected.");
        break;
    }

    return true;
}

PLUGIN_API void on_sdk_unload( )
{

}