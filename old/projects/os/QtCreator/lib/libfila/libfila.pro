########################################################################
# Copyright (c) 1988-2018 $organization$
#
# This software is provided by the author and contributors ``as is''
# and any express or implied warranties, including, but not limited to,
# the implied warranties of merchantability and fitness for a particular
# purpose are disclaimed. In no event shall the author or contributors
# be liable for any direct, indirect, incidental, special, exemplary,
# or consequential damages (including, but not limited to, procurement
# of substitute goods or services; loss of use, data, or profits; or
# business interruption) however caused and on any theory of liability,
# whether in contract, strict liability, or tort (including negligence
# or otherwise) arising in any way out of the use of this software,
# even if advised of the possibility of such damage.
#
#   File: libfila.pro
#
# Author: $author$
#   Date: 4/10/2018
#
# QtCreator .pro file for fila library libfila
########################################################################
include(../../../../../build/QtCreator/fila.pri)
include(../../../../QtCreator/fila.pri)
include(../../fila.pri)
include(../../../../QtCreator/lib/libfila/libfila.pri)

TARGET = $${libfila_TARGET}
TEMPLATE = $${libfila_TEMPLATE}
CONFIG += $${libfila_CONFIG}

########################################################################
# INCLUDEPATH
#
INCLUDEPATH += \
$${libfila_INCLUDEPATH} \

# DEFINES
# 
DEFINES += \
$${libfila_DEFINES} \

########################################################################
# OBJECTIVE_HEADERS
#
OBJECTIVE_HEADERS += \
$${libfila_OBJECTIVE_HEADERS} \

# OBJECTIVE_SOURCES
#
SOURCES += \
$${libfila_OBJECTIVE_SOURCES} \

########################################################################
# HEADERS
#
HEADERS += \
$${libfila_HEADERS} \

# SOURCES
#
SOURCES += \
$${libfila_SOURCES} \
$${OBJECTIVE_SOURCES} \

########################################################################


