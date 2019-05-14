/*
** $Id: lapi.h,v 1.1 2005/07/30 20:07:42 gliptic Exp $
** Auxiliary functions from Lua API
** See Copyright Notice in lua.h
*/

#ifndef lapi_h
#define lapi_h


#include "lobject.h"


void luaA_pushobject (lua_State *L, const TObject *o);

#endif
