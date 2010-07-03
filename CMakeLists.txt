        include_directories(
            ../
            ../../
            ../../core-impl/collections
            ../../servicebrowser
            ../../statusbar
            ${CMAKE_CURRENT_BINARY_DIR}/../../..
            ${KDE4_INCLUDE_DIR}
            ${STRIGI_INCLUDE_DIR}
            ${QT_INCLUDES}
            )

########### next target ###############

        set(amarok_service_remotecollection_PART_SRCS
            RemoteCollectionService.cpp
            RemoteCollection.cpp
            RemoteCollectionQueryMaker.cpp
			HttpDaemon.cpp
			HttpService.cpp
        )


        kde4_add_plugin(amarok_service_remotecollection ${amarok_service_remotecollection_PART_SRCS})
        target_link_libraries(amarok_service_remotecollection
            amarokcore
            amaroklib
            ${KDE4_KDECORE_LIBS}
            ${KDE4_KDEUI_LIBS}
            ${KDE4_KIO_LIBS}
            ${KDE4_THREADWEAVER_LIBRARIES}
        )
        if( LIBLASTFM_FOUND )
            target_link_libraries(amarok_service_remotecollection
                ${LIBLASTFM_LIBRARY}
            )
        endif( LIBLASTFM_FOUND )


        install(TARGETS amarok_service_remotecollection DESTINATION ${PLUGIN_INSTALL_DIR} )




########### install files ###############

        install( FILES amarok_service_remotecollection.desktop DESTINATION ${SERVICES_INSTALL_DIR})
