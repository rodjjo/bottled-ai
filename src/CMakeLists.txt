file(GLOB Sources
    "${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/config/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/controls/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/windows/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/python/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/stable_diffusion/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/geometry/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/dialogs/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/panels/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/console/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/opengl_utils/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

file(GLOB SourcesEntry
    "${CMAKE_CURRENT_SOURCE_DIR}/entry-point/*.cpp")

if(WIN32)
add_executable(bottled-ai WIN32 ${Sources})
else(WIN32)
add_executable(bottled-ai ${Sources})
endif(WIN32)

if(WIN32)
add_executable(bottled-ai-win WIN32 ${SourcesEntry})
endif(WIN32)

target_link_libraries(
    bottled-ai 
    stdc++ 
    fltk
    fltk_forms 
    fltk_images 
    fltk_jpeg 
    fltk_png 
    fltk_z 
    fltk_gl 
#   python3.lib
    python310.lib
    opengl32
    user32
    comctl32
    Gdiplus
    Ws2_32
)

set_target_properties(bottled-ai
    PROPERTIES
    LINKER_LANGUAGE C
)

if(WIN32)
set(SUBSYSTEM_LINKER_OPTIONS "-mconsole")
endif(WIN32)

target_link_options(bottled-ai PRIVATE ${SUBSYSTEM_LINKER_OPTIONS})

install(TARGETS bottled-ai
        RUNTIME DESTINATION bin
        COMPONENT runtime)

if(WIN32)
install(TARGETS bottled-ai-win
        RUNTIME DESTINATION bin
        COMPONENT runtime)
endif()