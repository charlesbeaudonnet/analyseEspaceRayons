# Install script for directory: /home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/21303468/workspace/analyseEspaceRayons/exe/src/ext/openexr/IlmBase/Imath/libImath.a")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathBoxAlgo.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathBox.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathColorAlgo.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathColor.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathEuler.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathExc.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathExport.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathForward.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathFrame.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathFrustum.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathFrustumTest.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathFun.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathGL.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathGLU.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathHalfLimits.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathInt64.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathInterval.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathLimits.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathLineAlgo.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathLine.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathMath.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathMatrixAlgo.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathMatrix.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathNamespace.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathPlane.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathPlatform.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathQuat.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathRandom.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathRoots.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathShear.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathSphere.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathVecAlgo.h"
    "/home/21303468/workspace/analyseEspaceRayons/pbrt-v3/src/ext/openexr/IlmBase/Imath/ImathVec.h"
    )
endif()

