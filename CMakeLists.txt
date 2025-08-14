cmake_minimum_required(VERSION 3.16)

project(GuitarPedal VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core Multimedia)

qt_standard_project_setup(REQUIRES 6.8)

# --- Uygulama hedefi: C++ dosyaları burada olmalı
qt_add_executable(appGuitarPedal
    main.cpp
    PedalController.h
    AudioEngine.h
    AudioEngine.cpp
)

# --- QML modülü: sadece QML ve assetler
qt_add_qml_module(appGuitarPedal
    URI GuitarPedal
    VERSION 1.0
    QML_FILES
        Main.qml
    RESOURCES
        assets/Button-Pedal-Checked.png
        assets/Button-Pedal-Checked@2x.png
        assets/Button-Pedal.png
        assets/Button-Pedal@2x.png
        assets/Guitar-Pedal-Background.png
        assets/Guitar-Pedal-Background@2x.png
        assets/Knob-Dial.png
        assets/Knob-Dial@2x.png
        assets/Knob-Markings.png
        assets/Knob-Markings@2x.png
        assets/LED-Checked.png
        assets/LED-Checked@2x.png
        assets/LED.png
        assets/LED@2x.png
        assets/Screw.png
        assets/Screw@2x.png
        assets/Switch-Checked.png
        assets/Switch-Checked@2x.png
        assets/Switch.png
        assets/Switch@2x.png
        fonts/OFL.txt
        fonts/Prisma.ttf
        fonts/RussoOne-Regular.ttf
        SOURCES AudioEngine.cpp
)

set_target_properties(appGuitarPedal PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

target_link_libraries(appGuitarPedal
    PRIVATE
        Qt6::Quick
        Qt6::Core
        Qt6::Multimedia
)

include(GNUInstallDirs)
install(TARGETS appGuitarPedal
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
