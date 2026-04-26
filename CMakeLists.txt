cmake_minimum_required(VERSION 3.16)

project(HMS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)

# List all source files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    welcomescreen.cpp welcomescreen.h welcomescreen.ui
    doctorlogindialog.cpp doctorlogindialog.h doctorlogindialog.ui
    patientlogindialog.cpp patientlogindialog.h patientlogindialog.ui
    adminlogindialog.cpp adminlogindialog.h adminlogindialog.ui
    employeelogindialog.cpp employeelogindialog.h employeelogindialog.ui
    doctordashboard.cpp doctordashboard.h doctordashboard.ui
    patientdashboard.cpp patientdashboard.h patientdashboard.ui
    employeedashboard.cpp employeedashboard.h employeedashboard.ui
)

qt_add_executable(HMS
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    admindashboard.h admindashboard.cpp admindashboard.ui
    admindashboard.cpp
    admindashboard.ui
    admindashboard.ui
    admindashboard.ui

    doctordashboardtodayswork.ui
    doctordashboardtodayswork.ui
    doctordashboardtodayswork.h doctordashboardtodayswork.cpp doctordashboardtodayswork.ui
    AppointmentsWindow.ui
    appointmentswindow.h appointmentswindow.cpp appointmentswindow.ui
    patientdashboardlabreport.h patientdashboardlabreport.cpp patientdashboardlabreport.ui
    patientdashboardpayment.h patientdashboardpayment.cpp patientdashboardpayment.ui
    patientdashboarddocfeedback.h patientdashboarddocfeedback.cpp patientdashboarddocfeedback.ui
    patientdashboardfeedback.h patientdashboardfeedback.cpp patientdashboardfeedback.ui
    patientdashboardprofile.h patientdashboardprofile.cpp patientdashboardprofile.ui
    admindashboarddoctor.h admindashboarddoctor.cpp admindashboarddoctor.ui
    admindashboardpatient.h admindashboardpatient.cpp admindashboardpatient.ui
    doctorreportsdialog.h doctorreportsdialog.cpp doctorreportsdialog.ui
    doctordashboardsalary.h doctordashboardsalary.cpp doctordashboardsalary.ui

)

# Include directories (optional here, but good if you later separate headers)
target_include_directories(HMS PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against required Qt modules
target_link_libraries(HMS PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
)

# Platform-specific settings
if(APPLE)
    set_target_properties(HMS PROPERTIES
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.HMS"
    )
elseif(WIN32)
    set_target_properties(HMS PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

qt_finalize_executable(HMS)

# Installation rules
install(TARGETS HMS
    BUNDLE DESTINATION .
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
