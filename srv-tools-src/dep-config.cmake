SET ( Boost_FIND_REQUIRED TRUE )
SET ( Boost_USE_MULTITHREADED ON )
SET ( Boost_USE_STATIC_LIBS OFF )

SET ( KVM_FIND_REQUIRED TRUE )



### Boost ###

FIND_PACKAGE (Boost 1.48.0
    COMPONENTS
        
    )


IF ( Boost_FOUND )
    SET ( DEP_FOUND TRUE )
ELSE ( Boost_FOUND )
    SET ( DEP_FOUND FALSE )
    IF ( Boost_FIND_REQUIRED )
        MESSAGE ( FATAL_ERROR "Could not find Boost" )
    ENDIF ( Boost_FIND_REQUIRED )
ENDIF ( Boost_FOUND )



### kvm ###

FIND_LIBRARY ( KVM_LIBRARY NAMES kvm PATHS /usr/lib /usr/local/lib )

IF ( KVM_LIBRARY )
    SET ( KVM_FOUND TRUE )
ENDIF ( KVM_LIBRARY )


IF ( KVM_FOUND )
    SET ( DEP_FOUND TRUE )
    IF ( NOT KVM_FIND_QUIETLY )
        MESSAGE ( STATUS "Found kvm:" )
        MESSAGE ( STATUS "  (Library)       ${KVM_LIBRARY}" )
    ENDIF ( NOT KVM_FIND_QUIETLY )
ELSE ( KVM_FOUND )
    SET ( DEP_FOUND FALSE )
    IF ( KVM_FIND_REQUIRED )
        MESSAGE ( FATAL_ERROR "Could not find kvm" )
    ENDIF ( KVM_FIND_REQUIRED )
ENDIF ( KVM_FOUND )



