#_______________________Pmake___________________________________________________
#
#	Makefile : 	MLV Filter Attribute
# 	January 2014	Wayne MoggNageswara
#_______________________________________________________________________________

SET(OD_MODULE_DEPS MLVFilterAttrib uiODMain)
SET(OD_IS_PLUGIN yes)
SET(OD_MODULE_SOURCES
	uimlvfilterattribpi.cc
	uimlvfilterattrib.cc)
SET( OD_PLUGIN_ALO_EXEC ${OD_MAIN_EXEC} )
OD_INIT_MODULE()
