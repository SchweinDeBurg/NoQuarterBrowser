cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

get_filename_component(REAL_SOURCE_PATH "${CMAKE_SOURCE_DIR}" REALPATH)
message(STATUS "The path to the top level of the source tree is ${REAL_SOURCE_PATH}")
get_filename_component(REAL_BINARY_PATH "${CMAKE_BINARY_DIR}" REALPATH)
message(STATUS "The path to the top level of the build tree is ${REAL_BINARY_PATH}")
if("${REAL_SOURCE_PATH}" STREQUAL "${REAL_BINARY_PATH}")
	message(FATAL_ERROR "In-source build of this project is forbidden!")
endif()

project(NoQuarterBrowser LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_MINIMUM_VERSION "5.9.0")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 ${QT_MINIMUM_VERSION} COMPONENTS Core Gui WebEngine WebEngineWidgets Widgets REQUIRED)
message(STATUS "Found Qt version ${Qt5Core_VERSION}")

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)

# header files
list(APPEND ${PROJECT_NAME}_HEADERS
	Include/AbstractPageWidget.h
	Include/AddressLineEdit.h
	Include/BrowserApplication.h
	Include/BrowserPageWidget.h
	Include/BrowserTabWidget.h
	Include/BrowserWindow.h
	Include/CommonStuff.h
	Include/CustomWebPage.h
	Include/CustomWebView.h
	Include/DownloadsPageWidget.h
	Include/FileTransfer.h
	Include/HistoryPageWidget.h
)

# source files
list(APPEND ${PROJECT_NAME}_SOURCES
	Source/AbstractPageWidget.cpp
	Source/AddressLineEdit.cpp
	Source/BrowserApplication.cpp
	Source/BrowserPageWidget.cpp
	Source/BrowserTabWidget.cpp
	Source/BrowserWindow.cpp
	Source/CustomWebPage.cpp
	Source/CustomWebView.cpp
	Source/DownloadsPageWidget.cpp
	Source/EntryPoint.cpp
	Source/FileTransfer.cpp
	Source/HistoryPageWidget.cpp
)

# form files
list(APPEND ${PROJECT_NAME}_FORMS
)

# resource files
list(APPEND ${PROJECT_NAME}_RESOURCES
	Resources/MainResources.qrc
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_FORMS} ${${PROJECT_NAME}_RESOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}/Include
	${Qt5Core_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
	${Qt5WebEngine_INCLUDE_DIRS}
	${Qt5WebEngineWidgets_INCLUDE_DIRS}
	${Qt5Widgets_INCLUDE_DIRS}
)
target_link_directories(${PROJECT_NAME} PRIVATE
)
target_link_libraries(${PROJECT_NAME}
	Qt5::Core
	Qt5::Gui
	Qt5::WebEngine
	Qt5::WebEngineWidgets
	Qt5::Widgets
	Threads::Threads
)

install(TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
