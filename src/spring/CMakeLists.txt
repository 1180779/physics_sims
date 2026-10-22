file(GLOB_RECURSE SPRING_SOURCES
        src/*.cxx
        src/*.hxx
)
list(REMOVE_ITEM SPRING_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# AUTORCC compiles this; it must be listed as a target source 
# Registers the vendored JetBrains icons + Mono font
set(SPRING_RESOURCES resources/resources.qrc)

add_library(spring_core ${SPRING_SOURCES})

target_link_libraries(spring_core PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGLWidgets
        cad_math::cad_math
        cad_common::cad_common
        cad_graphics::cad_graphics
        TBB::tbb
)

target_include_directories(spring_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(spring src/main.cpp)

target_link_libraries(spring PRIVATE spring_core)

add_custom_target(SpringCopyShaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders
        $<TARGET_FILE_DIR:spring>/shaders
        COMMENT "Copying shaders to output directory"
)

add_dependencies(spring SpringCopyShaders)

if (WIN32)
    get_target_property(QMAKE_EXECUTABLE Qt::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QMAKE_EXECUTABLE}" DIRECTORY)

    add_custom_command(TARGET spring POST_BUILD
            COMMAND "${QT_BIN_DIR}/windeployqt.exe"
            --no-translations
            "$<TARGET_FILE:spring>"
            COMMENT "Running windeployqt"
    )
endif ()

if (SPRING_ENABLE_BUNDLE AND (MINGW OR MSVC OR (CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")))
    get_target_property(QMAKE_EXECUTABLE Qt::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QMAKE_EXECUTABLE}" DIRECTORY)
    if (MSVC)
        set(SPRING_BUNDLE_PLATFORM windows-msvc)
        set(SPRING_RUNTIME_NAME "MSVC runtime")
        set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
        set(CMAKE_INSTALL_DEBUG_LIBRARIES FALSE)
        include(InstallRequiredSystemLibraries)
        set(SPRING_MSVC_RUNTIME_LIBRARIES "${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}")
        get_filename_component(SPRING_DUMPBIN_HINT "${CMAKE_LINKER}" DIRECTORY)
        find_program(SPRING_DUMPBIN NAMES dumpbin HINTS "${SPRING_DUMPBIN_HINT}" REQUIRED)
        set(SPRING_DEPENDENCY_TOOL dumpbin)
        set(SPRING_DEPENDENCY_COMMAND "${SPRING_DUMPBIN}")
    elseif (MINGW)
        set(SPRING_BUNDLE_PLATFORM windows-mingw)
        set(SPRING_RUNTIME_NAME "MinGW runtime")
        set(SPRING_DEPENDENCY_TOOL objdump)
        set(SPRING_DEPENDENCY_COMMAND "${CMAKE_OBJDUMP}")
    else ()
        set(SPRING_BUNDLE_PLATFORM linux-gcc)
    endif ()
    get_filename_component(SPRING_COMPILER_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
    find_path(SPRING_QT_LICENSE_DIR NAMES LICENSE
            HINTS "${QT_BIN_DIR}/../../../Licenses" "${QT_BIN_DIR}/../LICENSES"
            DOC "Directory of Qt license texts to include in the bundle" NO_DEFAULT_PATH)
    if (WIN32)
        set(SPRING_BUNDLE_SCRIPT BundleWindows)
    else ()
        if (Qt6_VERSION VERSION_LESS 6.5)
            message(FATAL_ERROR "Linux bundles require Qt 6.5 or newer for runtime deployment.")
        endif ()
        set(SPRING_BUNDLE_SCRIPT BundleLinux)
        include(GNUInstallDirs)
        set_target_properties(spring PROPERTIES INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
        install(TARGETS spring RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT spring_bundle)
        install(TARGETS tbb
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT spring_bundle)
        install(DIRECTORY src/shaders format DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT spring_bundle)
        install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/cnc_ui/src/shaders/
                DESTINATION ${CMAKE_INSTALL_BINDIR}/shaders/cnc/ui COMPONENT spring_bundle)
        install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/cnc_gpu/shaders/
                DESTINATION ${CMAKE_INSTALL_BINDIR}/shaders/cnc/gpu COMPONENT spring_bundle)
        qt_generate_deploy_app_script(TARGET spring OUTPUT_SCRIPT SPRING_DEPLOY_SCRIPT NO_TRANSLATIONS)
        install(SCRIPT ${SPRING_DEPLOY_SCRIPT} COMPONENT spring_bundle)
    endif ()
    configure_file(${PROJECT_SOURCE_DIR}/cmake/${SPRING_BUNDLE_SCRIPT}.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/${SPRING_BUNDLE_SCRIPT}.cmake.in @ONLY)
    file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SPRING_BUNDLE_SCRIPT}-$<CONFIG>.cmake
            INPUT ${CMAKE_CURRENT_BINARY_DIR}/${SPRING_BUNDLE_SCRIPT}.cmake.in)
    add_custom_target(bundle
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/${SPRING_BUNDLE_SCRIPT}-$<CONFIG>.cmake
            DEPENDS spring
            COMMENT "Creating ${SPRING_BUNDLE_PLATFORM} bundle"
            VERBATIM)
endif ()
