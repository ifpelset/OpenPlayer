SOURCES += \
    main.cpp \
    MainWindow.cpp \
    MyPlayer.cpp \
    MyPlayList.cpp \
    libs/GetAlbumPicture.cpp \
    PlayListDlg.cpp \
    LrcDisplay.cpp

QT += widgets gui multimedia

HEADERS += \
    MainWindow.h \
    MyPlayer.h \
    MyPlayList.h \
    PlayListDlg.h \
    LrcDisplay.h

RESOURCES += \
    myrc.qrc

RC_ICONS = mainIcon.ico
