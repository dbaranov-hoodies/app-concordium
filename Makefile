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

# Include ledger`s standard mandatory config
include config.mk

ifdef TARGET
    include $(BOLOS_SDK)/Makefile.defines
    include $(BOLOS_SDK)/Makefile.standard_app
    include $(BOLOS_SDK)/Makefile.target
endif


#######################################
#       LOCAL VARIABLES               #
#######################################
# List of supported ledger targets
LEDGER_TARGETS :=  nanox nanos2 stax flex apex_p apex_m

#######################################
#       TARGETS                       #
#######################################
.PHONY: clean_local, all, help, debug $(LEDGER_TARGETS)

.PHONY: default
default:
ifdef TARGET
	@$(MAKE) TARGET=$(TARGET)
else
	@echo "No TARGET specified. Run 'make TARGET=(one of [$(LEDGER_TARGETS)])'."
endif

# Just print available targets
help:
	@echo "Available targets:"
	@echo clean_local, debug, all, ${LEDGER_TARGETS}

# Clean build dirs
clean_local:
	rm -rf build bin debug

# Build .elf for all all ledger targets
all_bin: $(LEDGER_TARGETS)

# Build certain ledger target (helper to "make nanosp" instead of "make TARGET=nanosp")
$(LEDGER_TARGETS):
	@echo "Building for TARGET=$@"
	$(MAKE) TARGET=$@

