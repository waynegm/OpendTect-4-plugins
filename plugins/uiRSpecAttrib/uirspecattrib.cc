/*Copyright (C) 2014 Wayne Mogg. All rights reserved.
 * 
 * This file may be used either under the terms of:
 * 
 * 1. The GNU General Public License version 3 or higher, as published by
 * the Free Software Foundation, or
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*+
 * ________________________________________________________________________
 * 
 * Author:        Wayne Mogg
 * Date:          August 2014
 * ________________________________________________________________________
 * 
 * 
-*/
#include "uirspecattrib.h"
#include "rspecattrib.h"

#include "attribdesc.h"
#include "attribparam.h"
#include "survinfo.h"
#include "uiattribfactory.h"
#include "uiattrsel.h"
#include "uigeninput.h"
#include "uimsg.h"
#include "uispinbox.h"

using namespace Attrib;

mInitAttribUI(uiRSpecAttrib,RSpecAttrib,"Recursive spectral decomposition",sKeyFreqGrp())


uiRSpecAttrib::uiRSpecAttrib( uiParent* p, bool is2d )
: uiAttrDescEd(p,is2d)

{
	inpfld_ = createInpFld( is2d );
	
	BufferString label = "Effective window ";
	label += SI().getZUnitString( true );
	windowfld_ = new uiGenInput( this, label.buf(), FloatInpSpec(50) );
	windowfld_->attach( alignedBelow, inpfld_ );

	BufferString lbl( "Output frequency (" );
	lbl += zIsTime() ? "Hz" :
	(SI().zInMeter() ? "cycles/km" : "cycles/kft"); lbl += ")";
	freqfld_ = new uiLabeledSpinBox( this, lbl, 1 );
	freqfld_->attach( alignedBelow, windowfld_ );
	freqfld_->box()->doSnap( true );

	stepfld_ = new uiLabeledSpinBox( this, "step", 1 );
	stepfld_->attach( rightTo, freqfld_ );
	stepfld_->box()->valueChanged.notify(mCB(this,uiRSpecAttrib,stepChg));
	
	stepChg(0);
	
	setHAlignObj( inpfld_ );
}

void uiRSpecAttrib::inputSel( CallBacker* )
{
	if ( !*inpfld_->getInput() ) return;
	
	CubeSampling cs;
	if ( !inpfld_->getRanges(cs) )
		cs.init(true);
	
	const float nyqfreq = 0.5f / cs.zrg.step;
	
	const float freqscale = zIsTime() ? 1.f : 1000.f;
	const float scalednyqfreq = nyqfreq * freqscale;
	stepfld_->box()->setInterval( (float)0.5, scalednyqfreq/2 );
	stepfld_->box()->setStep( (float)0.5, true );
	freqfld_->box()->setMinValue( 0.0f );
	freqfld_->box()->setMaxValue( scalednyqfreq );
}

void uiRSpecAttrib::stepChg( CallBacker* )
{
	if ( mIsZero(stepfld_->box()->getFValue(),mDefEps) )
	{
		stepfld_->box()->setValue( 1 );
		stepfld_->box()->setStep( 1, true );
	}
	else
	{
		freqfld_->box()->setMinValue( stepfld_->box()->getFValue() );
		freqfld_->box()->setStep( stepfld_->box()->getFValue(), true );
	}
}

int uiRSpecAttrib::getOutputIdx( float outval ) const
{
	const float step = stepfld_->box()->getFValue();
	return mNINT32(outval/step)-1;
}


float uiRSpecAttrib::getOutputValue( int idx ) const
{
	const float step = stepfld_->box()->getFValue();
	return float((idx+1)*step);
}

bool uiRSpecAttrib::setParameters( const Attrib::Desc& desc )
{
	if ( strcmp(desc.attribName(), RSpecAttrib::attribName()) )
		return false;
	mIfGetFloat(RSpecAttrib::windowStr(), window, windowfld_->setValue(window));
	const float freqscale = zIsTime() ? 1.f : 1000.f;
//	mIfGetFloat(RSpecAttrib::freqStr(), freq, freqfld_->box()->setValue(freq*freqscale) )
	mIfGetFloat(RSpecAttrib::stepStr(), step, stepfld_->box()->setValue(step*freqscale) );
	
	stepChg(0);
	
	return true;
}


bool uiRSpecAttrib::setInput( const Attrib::Desc& desc )
{
	putInp( inpfld_, desc, 0 );
	inputSel(0);
	return true;
}

bool uiRSpecAttrib::setOutput( const Desc& desc )
{
	const float freq = getOutputValue( desc.selectedOutput() );
	freqfld_->box()->setValue( freq );
	return true;
}

bool uiRSpecAttrib::getParameters( Attrib::Desc& desc )
{
	if ( strcmp( desc.attribName(), RSpecAttrib::attribName()) )
		return false;
	
	mSetFloat( RSpecAttrib::windowStr(), windowfld_->getfValue() );
	
	const float freqscale = zIsTime() ? 1.f : 1000.f;
//	mSetFloat( RSpecAttrib::freqStr(), freqfld_->box()->getFValue()/freqscale );
	mSetFloat( RSpecAttrib::stepStr(), stepfld_->box()->getFValue()/freqscale );
	
	return true;
}


bool uiRSpecAttrib::getInput( Attrib::Desc& desc )
{
//	inpfld_->processInput();
	fillInp( inpfld_, desc, 0 );
	return true;
}

bool uiRSpecAttrib::getOutput( Desc& desc )
{
	checkOutValSnapped();
	const int freqidx = getOutputIdx( freqfld_->box()->getFValue() );
	fillOutput( desc, freqidx );
	return true;
}

void uiRSpecAttrib::getEvalParams( TypeSet<EvalParam>& params ) const
{
	EvalParam ep( "Frequency" ); ep.evaloutput_ = true;
	params += ep;
	params += EvalParam( "Window", RSpecAttrib::windowStr() );
}

void uiRSpecAttrib::checkOutValSnapped() const
{
	const float oldfreq = freqfld_->box()->getFValue();
	const int freqidx = getOutputIdx( oldfreq );
	const float freq = getOutputValue( freqidx );
	if ( oldfreq>0.5 && oldfreq!=freq )
	{
		BufferString wmsg = "Chosen output frequency \n";
		wmsg += "does not fit with frequency step \n";
		wmsg += "and will be snapped to nearest suitable frequency";
		uiMSG().warning( wmsg );
	}
}
