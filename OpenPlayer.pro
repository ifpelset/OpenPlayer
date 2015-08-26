SOURCES += \
    main.cpp \
    MainWindow.cpp \
    MyPlayer.cpp \
    MyPlayList.cpp \
    libs/GetAlbumPicture.cpp \
    PlayListDlg.cpp \
    LrcDisplay.cpp \
    LoginDlg.cpp

QT += widgets gui multimedia

CONFIG += C++11

HEADERS += \
    MainWindow.h \
    MyPlayer.h \
    MyPlayList.h \
    PlayListDlg.h \
    LrcDisplay.h \
    libs/getalbumpicture.h \
    LoginDlg.h

RESOURCES += \
    myrc.qrc

RC_ICONS = mainIcon.ico
