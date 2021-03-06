#include <Windows.h>
#include <ShlObj.h>

#include <QString>
#include <QDir>
#include <QStandardPaths>

#include "../GetLastErrorString.h"
#include "../../profile/cpp/Profile/include/ambiesoft.profile.h"

#include "stdQt.h"

using namespace Ambiesoft;

namespace AmbiesoftQt {

    bool Move3Files(const QString& replaced, const QString& replacement, const QString& backup, QString* pError)
    {
        Q_ASSERT(!replaced.isEmpty());
        Q_ASSERT(!replacement.isEmpty());

        // maybe first save, original file not exist
        if(!QFile(replaced).exists())
        {
            if(!MoveFileW(replacement.toStdWString().c_str(),
                      replaced.toStdWString().c_str()))
            {
                if(pError)
                    *pError = QString::fromStdWString(GetLastErrorString(GetLastError()));
                return false;
            }
            return true;
        }
        if(!ReplaceFile(replaced.toStdWString().c_str(),
                    replacement.toStdWString().c_str(),
                    backup.isEmpty() ? nullptr : backup.toStdWString().c_str(),
                    0, // flag
                    nullptr,nullptr // reserved
                    ))
        {
            if(pError)
                *pError = QString::fromStdWString(GetLastErrorString(GetLastError()));
            return false;
        }
        return true;
    }

    static QString getSpecialFolder(int id,
                                    const QString& company,
                                    const QString& appname)
    {
        wchar_t path[MAX_PATH] = {0};
        if(S_OK != SHGetFolderPath(
                    nullptr,
                    id,
                    nullptr,
                    SHGFP_TYPE_CURRENT,
                    path))

        {
            Alert(nullptr, QObject::tr("Failed to get folder location."));
            return QString();
        }


        //    PWSTR pOut=NULL;
        //    if(S_OK != SHGetKnownFolderPath(id,0,NULL,&pOut))
        //    {
        //        Alert(nullptr, QObject::tr("Failed to get Roaming folder. Default folder will be used."));
        //        return QString();
        //    }

        QString dir = QString::fromUtf16((const ushort*)path);
        //    CoTaskMemFree(pOut);

        dir = QDir(dir).absolutePath();

        QDir(dir).mkdir(company);
        dir = pathCombine(dir, company);

        QDir(dir).mkdir(appname);
        dir = pathCombine(dir, appname);

        return dir;
    }
    static QString getSpecialFolder(int id)
    {
        const QString company = QApplication::organizationName();
        const QString appname = QApplication::applicationName();
        Q_ASSERT(!company.isEmpty());
        Q_ASSERT(!appname.isEmpty());

        return getSpecialFolder(id,company,appname);
    }


    QString getInifile(bool& bExit,
                       const QString& company,
                       const QString& appname)
    {
        QFileInfo trfi(QCoreApplication::applicationFilePath());
        std::string folini = pathCombine(trfi.absolutePath(), "folder.ini").toStdString();
        int intval = -1;
        Profile::GetInt("Main", "PathType", -1, intval, folini);

        QString dir;
        switch (intval)
        {
        case 0:
            dir = trfi.absolutePath();
            break;

        case 1:
            dir = getSpecialFolder(CSIDL_LOCAL_APPDATA,
                                   company,
                                   appname);
            break;

        default:
        case 2:
            dir = getSpecialFolder(CSIDL_APPDATA,
                                   company,
                                   appname);
            break;

        case 3:
        {
            std::string t;
            Profile::GetString("Main", "folder", "", t, folini);
            dir = t.c_str();
            if(dir.isEmpty())
            {
                Alert(nullptr, QObject::tr("Folder settings does not have sufficient data. Lanuch FolderConfig.exe and reconfigure."));
                bExit=true;
                return QString();
            }
            else
            {
                if(!QDir(dir).exists())
                {
                    if(!YesNo(nullptr, QObject::tr("\"%1\" does not exist. Do you want to create it?").arg(dir)))
                    {
                        bExit=true;
                        return QString();
                    }
                    QDir(dir).mkpath(".");
                    if(!QDir(dir).exists())
                    {
                        Alert(nullptr, QObject::tr("Failed to create directory."));
                        bExit=true;
                        return QString();
                    }
                }
            }
        }
            break;
        } // switch

        if (dir.isEmpty())
            return QString();

        return pathCombine(dir, appname + ".ini");
    }
    QString getInifile(bool& bExit)
    {
        const QString company = QApplication::organizationName();
        const QString appname = QApplication::applicationName();
        Q_ASSERT(!company.isEmpty());
        Q_ASSERT(!appname.isEmpty());

        return getInifile(bExit,company,appname);
    }


    QString GetUserDocumentDirectory()
    {
        QString result = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        if(!result.isEmpty())
            return result;

        result = getSpecialFolder(CSIDL_MYDOCUMENTS);
        if(!result.isEmpty())
            return result;

        return QDir::homePath();
    }


}
