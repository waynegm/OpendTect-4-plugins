/*Copyright (C) 2014 Wayne Mogg All rights reserved.
  
 This file may be used either under the terms of:
 
 1. The GNU General Public License version 3 or higher, as published by
 the Free Software Foundation, or
 
 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/*+
 ________________________________________________________________________

 Author:        Wayne Mogg
 Date:          January 2014
 ________________________________________________________________________
 
 -*/

#include "avoattrib.h"
#include "odplugin.h"


mDefODPluginEarlyLoad(AVOAttrib)
mDefODPluginInfo(AVOAttrib)
{
    static PluginInfo retpi = {
	"AVO attribute v4 (base)",
	"Wayne Mogg",
	"4.0",
	"Simple AVO attributes for OpendTect v4." };
    return &retpi;
}


mDefODInitPlugin(AVOAttrib)
{
    Attrib::AVOAttrib::initClass();

    return 0;
}
