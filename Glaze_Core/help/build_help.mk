# ============================================================================
#  Name	 : build_help.mk
#  Part of  : Glaze_Core
# ============================================================================
#  Name	 : build_help.mk
#  Part of  : Glaze_Core
#
#  Description: This make file will build the application help file (.hlp)
# 
# ============================================================================

do_nothing :
	@rem do_nothing

# build the help from the MAKMAKE step so the header file generated
# will be found by cpp.exe when calculating the dependency information
# in the mmp makefiles.

MAKMAKE : Glaze_Core_0xE3243CFD.hlp
Glaze_Core_0xE3243CFD.hlp : Glaze_Core.xml Glaze_Core.cshlp Custom.xml
	cshlpcmp Glaze_Core.cshlp
ifeq (WINS,$(findstring WINS, $(PLATFORM)))
	copy Glaze_Core_0xE3243CFD.hlp $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
endif

BLD : do_nothing

CLEAN :
	del Glaze_Core_0xE3243CFD.hlp
	del Glaze_Core_0xE3243CFD.hlp.hrh

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing
		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo Glaze_Core_0xE3243CFD.hlp

FINAL : do_nothing
