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

include $(BOLOS_SDK)/Makefile.defines
include $(BOLOS_SDK)/Makefile.standard_app
include $(BOLOS_SDK)/Makefile.target

