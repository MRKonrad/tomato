if(DOWNLOAD_DEPENDENCIES)

    # using 4.12 because 4.13 causes error:
    # /usr/bin/ld: ../thirdParty/itk/lib/libitksys-4.13.a(SystemTools.cxx.o): unrecognized relocation (0x2a) in section `.text'
    set(TOMATO_ITK_VERSION v4.12.0)

    if (WIN32)
        if(NOT DEFINED CMAKE_GENERATOR_TOOLSET)
            set(ITK_TOOLSET v140)
        elseif(NOT(CMAKE_GENERATOR_TOOLSET MATCHES v90 OR CMAKE_GENERATOR_TOOLSET MATCHES v140))
            set(ITK_TOOLSET v140)
        else()
            set(ITK_TOOLSET ${CMAKE_GENERATOR_TOOLSET})
        endif()
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            # windows 64 bit
            set(ITK_ZIP_NAME itk_${TOMATO_ITK_VERSION}_windows_x64_${ITK_TOOLSET}.zip)
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
            # windows 32 bit
            set(ITK_ZIP_NAME itk_${TOMATO_ITK_VERSION}_windows_Win32_${ITK_TOOLSET}.zip)
        endif()
    endif ()
    if (UNIX AND NOT APPLE)
        set(ITK_ZIP_NAME itk_${TOMATO_ITK_VERSION}_linux_g++_CXX98.zip)
    endif ()
    if (APPLE)
        set(ITK_ZIP_NAME itk_${TOMATO_ITK_VERSION}_osx_g++_CXX98.zip)
    endif ()

    set(DOWNLOAD_URL https://github.com/MRKonrad/build-itk/releases/download/itk_${TOMATO_ITK_VERSION}/${ITK_ZIP_NAME})
    MESSAGE(STATUS "Downloading ${DOWNLOAD_URL}")

    if (NOT EXISTS ${PROJECT_SOURCE_DIR}/thirdParty/itk)

        # DOWNLOAD
        file(DOWNLOAD
                ${DOWNLOAD_URL}
                "${PROJECT_SOURCE_DIR}/thirdParty/itk/itk.zip"
                STATUS status LOG log)
        list(GET status 0 status_code)
        list(GET status 1 status_string)

        # UNZIP
        execute_process( COMMAND cmake -E tar xzf "${PROJECT_SOURCE_DIR}/thirdParty/itk/itk.zip"
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/thirdParty/itk)

        # DELETE ZIP
        file(REMOVE "${PROJECT_SOURCE_DIR}/thirdParty/itk/itk.zip")

    endif()

    set(ITK_DIR_HINTS "${PROJECT_SOURCE_DIR}/thirdParty/itk" CACHE PATH "ITK_DIR_HINTS")


endif(DOWNLOAD_DEPENDENCIES)