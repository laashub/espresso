# Copyright (C) 2009-2019 The ESPResSo project
# Copyright (C) 2009,2010
#   Max-Planck-Institute for Polymer Research, Theory Group
#
# This file is part of ESPResSo.
#
# ESPResSo is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ESPResSo is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Register a new tutorial target that depends on a list of files that need to be
# copied from the source directory to the build directory. Make the tutorial
# target a dependency of target `tutorials`.
function(configure_tutorial_target)
  cmake_parse_arguments(CONFIGURE "" "TARGET" "DEPENDS" ${ARGN})
  set(TARGET_DEPENDENCIES "")
  foreach(filepath ${CONFIGURE_DEPENDS})
    list(APPEND TARGET_DEPENDENCIES "${CMAKE_CURRENT_BINARY_DIR}/${filepath}")
    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${filepath}"
      DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${filepath}"
      COMMAND
        ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${filepath}"
        "${CMAKE_CURRENT_BINARY_DIR}/${filepath}")
  endforeach(filepath)
  add_custom_target(${CONFIGURE_TARGET} DEPENDS ${TARGET_DEPENDENCIES})
  add_dependencies(tutorials ${CONFIGURE_TARGET})
endfunction(configure_tutorial_target)

# Convert a tutorial to the Python and HTML formats. Make these files
# dependencies of targets `tutorials_html` and `tutorials_python`.
function(NB_EXPORT)
  cmake_parse_arguments(NB_EXPORT "HTML_RUN" "FILE;TARGET;SUFFIX"
                        "VAR_SUBST;ADD_SCRIPTS" ${ARGN})

  set(NB_FILE "${CMAKE_CURRENT_BINARY_DIR}/${NB_EXPORT_FILE}")
  set(DEPENDENCY_OF_TARGET "${NB_EXPORT_TARGET}")
  if(NOT "${NB_EXPORT_SUFFIX}" STREQUAL "")
    set(NB_EXPORT_TARGET "${NB_EXPORT_TARGET}_${NB_EXPORT_SUFFIX}")
  endif()
  get_filename_component(NB_FILE_BASE ${NB_FILE} NAME_WE)
  get_filename_component(NB_FILE_EXT ${NB_FILE} EXT)
  set(HTML_FILE "${NB_FILE_BASE}.html")
  set(PY_FILE "${NB_FILE_BASE}.py")

  if(${NB_EXPORT_HTML_RUN})
    set(HTML_FILE_DEPENDS "${NB_FILE_BASE}.run${NB_FILE_EXT}")
    add_custom_command(
      OUTPUT "${HTML_FILE_DEPENDS}"
      DEPENDS
        "${NB_FILE};${NB_EXPORT_ADD_SCRIPTS};${CMAKE_BINARY_DIR}/doc/tutorials/html_runner.py;${CMAKE_BINARY_DIR}/testsuite/scripts/importlib_wrapper.py"
      COMMAND
        "${CMAKE_BINARY_DIR}/pypresso"
        "${CMAKE_BINARY_DIR}/doc/tutorials/html_runner.py" "--input"
        "${NB_FILE}" "--output" "${HTML_FILE_DEPENDS}" "--substitutions"
        ${NB_EXPORT_VAR_SUBST} "--scripts" ${NB_EXPORT_ADD_SCRIPTS})
  else()
    set(HTML_FILE_DEPENDS "${NB_FILE}")
  endif()

  add_custom_command(
    OUTPUT ${HTML_FILE} DEPENDS ${HTML_FILE_DEPENDS};${NB_EXPORT_ADD_SCRIPTS}
    COMMAND ${IPYTHON_EXECUTABLE} nbconvert --to "html" --output ${HTML_FILE}
            ${HTML_FILE_DEPENDS})

  add_custom_command(
    OUTPUT ${PY_FILE} DEPENDS ${NB_FILE}
    COMMAND ${IPYTHON_EXECUTABLE} nbconvert --to "python" --output ${PY_FILE}
            ${NB_FILE})

  add_custom_target("${NB_EXPORT_TARGET}_html" DEPENDS ${HTML_FILE}
                                                       ${DEPENDENCY_OF_TARGET})
  add_custom_target("${NB_EXPORT_TARGET}_python"
                    DEPENDS ${PY_FILE} ${DEPENDENCY_OF_TARGET})
  add_dependencies(tutorials_html "${NB_EXPORT_TARGET}_html")
  add_dependencies(tutorials_python "${NB_EXPORT_TARGET}_python")
endfunction(NB_EXPORT)

add_custom_target(tutorials)
add_custom_target(tutorials_html)
add_custom_target(tutorials_python)

# Here: add new directory
add_subdirectory(01-lennard_jones)
add_subdirectory(02-charged_system)
add_subdirectory(04-lattice_boltzmann)
add_subdirectory(05-raspberry_electrophoresis)
add_subdirectory(06-active_matter)
add_subdirectory(07-electrokinetics)
add_subdirectory(08-visualization)
add_subdirectory(11-ferrofluid)
add_subdirectory(12-constant_pH)

configure_file(Readme.rst ${CMAKE_CURRENT_BINARY_DIR}/Readme.rst COPYONLY)
configure_file(html_runner.py ${CMAKE_CURRENT_BINARY_DIR}/html_runner.py)
