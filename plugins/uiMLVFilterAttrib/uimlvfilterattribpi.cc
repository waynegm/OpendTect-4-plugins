/*Copyright (C) 2014 Wayne Mogg. All rights reserved.

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

#include "uimenu.h"
#include "uiodmain.h"
#include "odplugin.h"

#include "uimlvfilterattrib.h"


mDefODPluginInfo(uiMLVFilterAttrib)
{
    static PluginInfo retpi = {
	"Mean of Least Variance Filter Attribute (UI)",
	"Wayne Mogg",
	"1.0",
   	"" };
    return &retpi;
}


mDefODInitPlugin(uiMLVFilterAttrib)
{
    uiMLVFilterAttrib::initClass();
   return 0;
}

