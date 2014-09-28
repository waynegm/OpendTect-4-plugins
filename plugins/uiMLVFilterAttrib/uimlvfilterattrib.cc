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
 _______________________________________________________________________

-*/

#include "uimlvfilterattrib.h"
#include "mlvfilterattrib.h"

#include "attribdesc.h"
#include "attribparam.h"
#include "uiattribfactory.h"
#include "uiattrsel.h"
#include "uigeninput.h"
#include "uispinbox.h"

using namespace Attrib;

const int cMinVal = 3;
const int cMaxVal = 9;
const int cStepVal = 2;

static const char* outstr[] =
{
    "Average",
    "Median",
    "Element",
    0
};

mInitAttribUI(uiMLVFilterAttrib,MLVFilter,"MLV Filter",sKeyFilterGrp())


uiMLVFilterAttrib::uiMLVFilterAttrib( uiParent* p, bool is2d )
    : uiAttrDescEd(p,is2d,"mToDoHelpID")

{
    inpfld_ = createInpFld( is2d );

    sizefld_ = new uiLabeledSpinBox( this, "Filter size" );
    sizefld_->box()->setMinValue( cMinVal );
    sizefld_->box()->setStep( cStepVal, true );
    sizefld_->attach( alignedBelow, inpfld_ );
 
    outfld_ = new uiGenInput( this, "Output statistic",
	    			   StringListInpSpec(outstr) );
    outfld_->attach( alignedBelow, sizefld_ );
    setHAlignObj( outfld_ );
}




bool uiMLVFilterAttrib::setParameters( const Attrib::Desc& desc )
{
    if ( strcmp(desc.attribName(),MLVFilter::attribName()) )
	return false;

    mIfGetInt(MLVFilter::sizeStr(),size, sizefld_->box()->setValue(size) )
    mIfGetEnum(MLVFilter::outputStr(), output, outfld_->setValue(output))
    return true;
}


bool uiMLVFilterAttrib::setInput( const Attrib::Desc& desc )
{
    putInp( inpfld_, desc, 0 );
    return true;
}


bool uiMLVFilterAttrib::getParameters( Attrib::Desc& desc )
{
    if ( strcmp(desc.attribName(),MLVFilter::attribName()) )
	return false;

    mSetInt( MLVFilter::sizeStr(), sizefld_->box()->getValue() );
    mSetEnum(MLVFilter::outputStr(),outfld_->getIntValue());
    
    return true;
}


bool uiMLVFilterAttrib::getInput( Attrib::Desc& desc )
{
    inpfld_->processInput();
    fillInp( inpfld_, desc, 0 );
    return true;
}


void uiMLVFilterAttrib::getEvalParams( TypeSet<EvalParam>& params ) const
{
    params += EvalParam( "Filter size", MLVFilter::sizeStr() );
}

