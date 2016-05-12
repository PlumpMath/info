if(NOT TARGET json)
  find_package(compat REQUIRED PATHS ../compat)
  add_library(json INTERFACE IMPORTED)
  get_filename_component(CURRENT_LIST_PATH "${CMAKE_CURRENT_LIST_FILE}" PATH)
  set_target_properties(json PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CURRENT_LIST_PATH}/include"
    INTERFACE_LINK_LIBRARIES "compat")
endif()
