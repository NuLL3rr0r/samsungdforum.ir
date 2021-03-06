CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 FATAL_ERROR )
PROJECT ( SAMSUNGDFORUMIR )

SET ( dep_DIR ${SAMSUNGDFORUMIR_SOURCE_DIR} )
SET ( CMAKE_INSTALL_PREFIX "${SAMSUNGDFORUMIR_SOURCE_DIR}/../www/" )


SET ( CXX_FLAGS_WARN_LEVEL "-Wall -Wextra -Wno-unused-parameter" )
SET ( CXX_FLAGS_ISO_CPP "-pedantic" )
SET ( CXX_FLAGS_CPP11 "-std=c++0x" )
SET ( CXX_FLAGS_USE_PIPE "-pipe" )
SET ( CXX_FLAGS_OPT "-O2 -march=native -mtune=native" )
SET ( CXX_FLAGS_OMIT_FP "-fomit-frame-pointer" )
SET ( CXX_FLAGS_SIZE_OPT "-ffunction-sections -fdata-sections" )
SET ( CXX_FLAGS_THREADS "-pthread" )
SET ( LD_FLAGS_SIZE_OPT "-Wl,--gc-sections" )

SET ( CMAKE_CXX_FLAGS "${CXX_FLAGS_WARN_LEVEL} ${CXX_FLAGS_ISO_CPP} ${CXX_FLAGS_CPP11} ${CXX_FLAGS_USE_PIPE} ${CXX_FLAGS_OPT} ${CXX_FLAGS_OMIT_FP} ${CXX_FLAGS_SIZE_OPT} ${CXX_FLAGS_THREADS}" )
SET ( CMAKE_EXE_LINKER_FLAGS "${LD_FLAGS_SIZE_OPT}" )

SET ( PREFERRED_MAGICKPP_LIB "GM" ) # GM or IM


FIND_PACKAGE ( dep )

IF ( DEP_FOUND )
    IF (${PREFERRED_MAGICKPP_LIB} MATCHES "GM")
        SET ( MAGICKPP_INCLUDE_DIR "${GM_MAGICKPP_INCLUDE_DIR}" )
        SET ( MAGICKPP_LIBRARY "${GM_MAGICKPP_LIBRARY}" )
        SET ( MAGICKPP_FLAGS "${GM_MAGICKPP_FLAGS}" )
    ELSE ()
        SET ( MAGICKPP_INCLUDE_DIR "${IM_MAGICKPP_INCLUDE_DIR}" )
        SET ( MAGICKPP_LIBRARY "${IM_MAGICKPP_LIBRARY}" )
        SET ( MAGICKPP_FLAGS "${IM_MAGICKPP_FLAGS}" )
    ENDIF ()

    MESSAGE ( STATUS "** Preferred Magick++ lib: ${PREFERRED_MAGICKPP_LIB}" )

    INCLUDE_DIRECTORIES ( ${Boost_INCLUDE_DIRS} )
    LINK_DIRECTORIES ( ${Boost_LIBRARY_DIRS} )

    INCLUDE_DIRECTORIES ( ${CPPDB_INCLUDE_DIR} )
    LINK_DIRECTORIES ( ${CPPDB_LIBRARY} )

    INCLUDE_DIRECTORIES ( ${CRYPTOPP_INCLUDE_DIR} )
    LINK_DIRECTORIES ( ${CRYPTOPP_LIBRARY} )

    INCLUDE_DIRECTORIES ( ${GEOIP_INCLUDE_DIR} )
    LINK_DIRECTORIES ( ${GEOIP_LIBRARY} )

    INCLUDE_DIRECTORIES ( ${MAGICKPP_INCLUDE_DIR} )
    LINK_DIRECTORIES ( ${MAGICKPP_LIBRARY} )

    INCLUDE_DIRECTORIES ( ${MAGICKPP_INCLUDE_DIR} )
    LINK_DIRECTORIES ( ${MAGICKPP_LIBRARY} )

    INCLUDE_DIRECTORIES ( ${POPPLER_CPP_INCLUDE_DIR} )
    LINK_DIRECTORIES ( ${POPPLER_CPP_LIBRARY} )

    INCLUDE_DIRECTORIES ( ${STATGRAB_INCLUDE_DIR} )
    LINK_DIRECTORIES ( ${DEVSTAT_LIBRARY} )
    LINK_DIRECTORIES ( ${STATGRAB_LIBRARY} )

    LINK_DIRECTORIES ( ${VMIME_INCLUDE_DIR} )
    LINK_DIRECTORIES ( ${VMIME_LIBRARY} )

    INCLUDE_DIRECTORIES ( ${WT_INCLUDE_DIR} )
    LINK_DIRECTORIES ( ${WT_LIBRARY} )
    LINK_DIRECTORIES ( ${WT_FCGI_LIBRARY} )

    INCLUDE ( _src.cmake )
    
    SET ( ROOT_BIN "root.fcgi" )

    ADD_EXECUTABLE ( ${ROOT_BIN} ${ROOT_SOURCE_FILES} )

    TARGET_LINK_LIBRARIES ( ${ROOT_BIN}
        ${Boost_LIBRARIES} ${CPPDB_LIBRARY} ${CRYPTOPP_LIBRARY}
        ${GEOIP_LIBRARY} ${MAGICKPP_FLAGS} ${POPPLER_CPP_LIBRARY}
        ${SQLITE_LIBRARY} ${DEVSTAT_LIBRARY} ${STATGRAB_LIBRARY}
        ${VMIME_LIBRARY} ${WT_LIBRARY} ${WT_FCGI_LIBRARY}
    )

    ADD_CUSTOM_COMMAND ( TARGET ${ROOT_BIN}
        POST_BUILD
        COMMAND strip ${ROOT_BIN}
        COMMAND strip -R.comment ${ROOT_BIN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    INSTALL ( FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${ROOT_BIN}"
        DESTINATION "${CMAKE_INSTALL_PREFIX}"
        PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
    )
    
ENDIF ( DEP_FOUND )


