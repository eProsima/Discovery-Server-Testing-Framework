# Copyright 2025 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

###############################################################################
# Project information
###############################################################################

# This directory is normally added by the top level CMakeLists.txt when BUILD_DOCS is ON, but it can also be
# configured on its own to work on the documentation without building the framework and its dependencies:
#     cmake -S docs -B build-docs && cmake --build build-docs
if(NOT PROJECT_NAME)
    cmake_minimum_required(VERSION 3.22)
    project("discovery-server" LANGUAGES NONE)
    set(PROJECT_NAME_LARGE "Discovery Server")
    include(CTest)
endif()

###############################################################################
# Sphinx documentation
###############################################################################

# Look for the Sphinx builder. It is usually installed within the same virtual environment where the requirements
# listed in docs/requirements.txt were installed, so give priority to the interpreter CMake already found.
if(Python3_EXECUTABLE)
    get_filename_component(SPHINX_HINT_DIR ${Python3_EXECUTABLE} DIRECTORY)
elseif(PYTHON_EXECUTABLE)
    get_filename_component(SPHINX_HINT_DIR ${PYTHON_EXECUTABLE} DIRECTORY)
endif()

find_program(SPHINX_BUILD_EXECUTABLE
    NAMES sphinx-build sphinx-build.exe
    HINTS ${SPHINX_HINT_DIR}
    DOC "Path to the sphinx-build executable used to render the documentation")

if(NOT SPHINX_BUILD_EXECUTABLE)
    message(FATAL_ERROR
        "BUILD_DOCS is ON but sphinx-build was not found. Install the documentation requirements with "
        "'pip3 install -r ${CMAKE_CURRENT_SOURCE_DIR}/requirements.txt', or set SPHINX_BUILD_EXECUTABLE to the "
        "sphinx-build of the environment where they are installed.")
endif()

set(DOCS_BUILDER "html" CACHE STRING
    "Sphinx builder used to render the documentation (html, dirhtml, singlehtml, latex, man, text, ...)")

set(DOCS_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${DOCS_BUILDER})
set(DOCS_DOCTREES_DIR ${CMAKE_CURRENT_BINARY_DIR}/doctrees)

# Sphinx keeps track of its own dependencies through the doctrees cache, so the target is always executed and the
# builder decides what has to be regenerated.
add_custom_target(docs ALL
    COMMAND ${SPHINX_BUILD_EXECUTABLE}
        -b ${DOCS_BUILDER}
        -d ${DOCS_DOCTREES_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${DOCS_OUTPUT_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building the ${PROJECT_NAME_LARGE} testing framework documentation (${DOCS_BUILDER})"
    VERBATIM)

###############################################################################
# Documentation tests
###############################################################################

# The RST sources must follow the style guidelines and the documentation must be free of spelling errors.
if(BUILD_TESTING)

    find_program(DOC8_EXECUTABLE
        NAMES doc8 doc8.exe
        HINTS ${SPHINX_HINT_DIR}
        DOC "Path to the doc8 executable used to check the RST style")

    if(DOC8_EXECUTABLE)
        # D001 is the line length check, which is not enforced on these sources.
        add_test(NAME documentation.doc8
            COMMAND ${DOC8_EXECUTABLE}
                --ignore D001
                --ignore-path ${CMAKE_CURRENT_BINARY_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    else()
        message(STATUS "doc8 not found, skipping the documentation.doc8 test")
    endif()

    # sphinxcontrib.spelling is an optional dependency: conf.py only registers the extension when it is importable.
    execute_process(
        COMMAND ${SPHINX_BUILD_EXECUTABLE} -b spelling --help
        RESULT_VARIABLE SPHINX_SPELLING_AVAILABLE
        OUTPUT_QUIET ERROR_QUIET)

    if(SPHINX_SPELLING_AVAILABLE EQUAL 0)
        # The extension scans the git history of the working directory to build the list of contributor names it must
        # accept, so the test has to be run from within the repository.
        add_test(NAME documentation.spelling
            COMMAND ${SPHINX_BUILD_EXECUTABLE}
                -W --keep-going
                -b spelling
                -d ${CMAKE_CURRENT_BINARY_DIR}/spelling_doctrees
                ${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_BINARY_DIR}/spelling
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    else()
        message(STATUS "sphinxcontrib.spelling not found, skipping the documentation.spelling test")
    endif()

endif()

###############################################################################
# Installation
###############################################################################

set(DOC_INSTALL_DIR share/doc/${PROJECT_NAME} CACHE PATH "Installation directory for the documentation")

install(DIRECTORY ${DOCS_OUTPUT_DIR}
    COMPONENT docs
    DESTINATION ${DOC_INSTALL_DIR}
    PATTERN ".doctrees" EXCLUDE
    PATTERN ".buildinfo" EXCLUDE)
