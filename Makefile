# ****************************************************************************
#    Ledger App Concordium
#    (c) 2023 Ledger SAS.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
# ****************************************************************************

ifeq ($(BOLOS_SDK),)
$(error Environment variable BOLOS_SDK is not set)
endif

########################################
#        INCLUDES                      #
########################################

# Include ledger`s standart mandatory config
include config.mk

ifneq ($(filter $(MAKECMDGOALS),help clean all_bin),) 
    # Do not include for ledger-native makefiles if running custom maketargets
else
    # Include project/compiler definitions and SDK paths
    include $(BOLOS_SDK)/Makefile.defines
    
    # Include standard rules for building Ledger apps
    include $(BOLOS_SDK)/Makefile.standard_app
    
    # Include target-specific build rules (e.g., nanos, nanox, stax)
    include $(BOLOS_SDK)/Makefile.target
endif

#######################################
#       TARGETS                       #
#######################################


# List of suported ledger targets
LEDGER_TARGETS :=  nanox nanos2 stax flex apex_p apex_m

.PHONY: clean, all, help
help:
	@echo "Available targets:"
	@echo clean, debug, all, ${LEDGER_TARGETS}





# "all" собирает все таргеты из списка
all_bin: $(LEDGER_TARGETS)

$(LEDGER_TARGETS):
	@echo "Building for TARGET=$@"
	$(MAKE) TARGET=$@

.PHONY: clean debug $(LEDGER_TARGETS)

# ---- DEBUG MULTI-TARGET ----

# Если среди целей есть один из Ledger targets → это TARGET
TARGET := $(filter $(LEDGER_TARGETS),$(MAKECMDGOALS))

# Цель debug требует, чтобы TARGET был выбран
debug:
ifeq ($(TARGET),)
	$(error Please specify one of: $(LEDGER_TARGETS))
endif
	@echo "Debug build for TARGET=$(TARGET)"
	$(MAKE) DEBUG=1 TARGET=$(TARGET) app.elf