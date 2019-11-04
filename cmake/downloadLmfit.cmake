if(DOWNLOAD_DEPENDENCIES)

    if (WIN32)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            # windows 64 bit
            set(LMFIT_ZIP_NAME lmfit_master_windows_x64_v120.zip)
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
            # windows 32 bit
            set(LMFIT_ZIP_NAME lmfit_master_windows_Win32_v120.zip)
        endif()
    endif ()
    if (UNIX AND NOT APPLE)
        set(LMFIT_ZIP_NAME lmfit_master_linux_g++_CXX98.zip)
    endif ()
    if (APPLE)
        set(LMFIT_ZIP_NAME lmfit_master_osx_g++_CXX98.zip)
    endif ()

    set(DOWNLOAD_URL https://github.com/MRKonrad/build-lmfit/releases/download/lmfit_master/${LMFIT_ZIP_NAME})
    MESSAGE(STATUS "Downloading ${DOWNLOAD_URL}")

    if (NOT EXISTS ${PROJECT_SOURCE_DIR}/thirdParty/lmfit)

        # DOWNLOAD
        file(DOWNLOAD
                ${DOWNLOAD_URL}
                "${PROJECT_SOURCE_DIR}/thirdParty/lmfit/lmfit.zip"
                STATUS status LOG log)
        list(GET status 0 status_code)
        list(GET status 1 status_string)

        # UNZIP
        execute_process( COMMAND cmake -E tar xzf "${PROJECT_SOURCE_DIR}/thirdParty/lmfit/lmfit.zip"
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/thirdParty/lmfit)

        # DELETE ZIP
        file(REMOVE "${PROJECT_SOURCE_DIR}/thirdParty/lmfit/lmfit.zip")

    endif()

#    set(LMFIT_DIR_HINTS "${PROJECT_SOURCE_DIR}/thirdParty/lmfit")

endif(DOWNLOAD_DEPENDENCIES)