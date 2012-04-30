/**********************************
*    = Header File Inclusion =
**********************************/

#include <string>
#include <Magick++.h>
#include <poppler/cpp/poppler-document.h>
#include "cimage.hpp"
#include "base.hpp"
#include "log.hpp"
#include "math.hpp"
#include "system.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace Magick;
using namespace SamsungDForumIr;


/**********************************
*    = Constants =
**********************************/

/**********************************
*    = Enumerations =
**********************************/

/**********************************
*    = Properties =
**********************************/

/**********************************
*    = Fields =
**********************************/

/**********************************
*    = Constructors =
**********************************/

/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

bool CImage::IsJPEG(const string &filePath)
{
    try {
        Image image(filePath);
        if ((image.format().find("JFIF") != string::npos)) {
            return true;
        }
    }
    catch(...) {
    }

    return false;
}

bool CImage::IsPDF(const string &filePath)
{
    /*try {
        DBG(filePath);
        Image image(filePath);
        DBG(image.format());
        if ((image.format().find("PDF") != string::npos)) {
            return true;
        }
    }
    catch(Magick::WarningCoder &ex) {
        ERR("Magick::WarningCoder", ex.what());
    }
    catch(Magick::Warning &ex) {
        ERR("Magick::Warning", ex.what());
    }
    catch(Magick::ErrorFileOpen &ex) {
        ERR("Magick::ErrorFileOpen", ex.what());
    }
    catch(Magick::Exception &ex) {
        ERR("Magick::Exeption", ex.what());
    }
    catch(std::exception &ex) {
        ERR("std::exception", ex.what());
    }
    catch(...) {
        ERR("CImage / ...");
    }*/

    bool result = false;

    poppler::document *doc = poppler::document::load_from_file(filePath);

    if (doc != NULL)
        result = true;

    delete doc;
    doc = NULL;

    return result;
}

bool CImage::IsPNG(const string &filePath)
{
    try {
        Image image(filePath);
        if ((image.format().find("Portable Network Graphics") != string::npos)) {
            return true;
        }
    }
    catch(...) {
    }

    return false;
}

bool CImage::IsImage(const string &filePath)
{
    //if (IsPNG(filePath) || IsJPEG(filePath)) {
    if (IsJPEG(filePath)) {
        return true;
    } else {
        return false;
    }
}

bool CImage::IsResumeFileSizeValid(const string &filePath)
{
    if(System::FileSize(filePath) < Base::FILE_SIZE_MAX_RESUME)
        return true;
    else
        return false;
}

bool CImage::IsScreenshotFileSizeValid(const string &filePath)
{
    if(System::FileSize(filePath) < Base::FILE_SIZE_MAX_APP_SCR)
        return true;
    else
        return false;
}

bool CImage::IsScreenshotDpiValid(const string &filePath)
{
   try {
        Image image(filePath);
        if (Math::Round(image.xResolution()) == (int)Base::DPI_APP_SCR
                && Math::Round(image.yResolution()) == (int)Base::DPI_APP_SCR) {
            return true;
        }
    }
    catch(...) {
    }

    return false;
}

bool CImage::IsScreenshotResolutionValid(const string &filePath)
{
    try {
        Image image(filePath);
        if (image.columns() <= Base::RESOLUTION_MAX_X_APP_SCR
                && image.rows() <= Base::RESOLUTION_MAX_Y_APP_SCR) {
            return true;
        }
    }
    catch(...) {
    }

    return false;
}

bool CImage::SaveAsJPEG(const string &inputFilePath, const string &outputFilePath)
{
    try {
        Image image(inputFilePath);
        image.magick("JPEG");
        image.quality(80);
        image.write(outputFilePath);

        return true;
    }
    catch(...) {
    }

    return false;
}


/**********************************
*    = Event Handlers =
**********************************/

/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

/**********************************
*    = Base Class Overrides =
**********************************/

/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


