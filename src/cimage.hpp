#ifndef CIMAGE_HPP
#define CIMAGE_HPP


#include <cstddef> // or cstdlib

namespace SamsungDForumIr {
    class CImage;
}

class SamsungDForumIr::CImage
{
public:
    static bool IsJPEG(const std::string &filePath);
    static bool IsPDF(const std::string &filePath);
    static bool IsPNG(const std::string &filePath);
    static bool IsImage(const std::string &filePath);
    static bool IsResumeFileSizeValid(const std::string &filePath);
    static bool IsScreenshotFileSizeValid(const std::string &filePath);
    static bool IsScreenshotDpiValid(const std::string &filePath);
    static bool IsScreenshotResolutionValid(const std::string &filePath);
    static bool SaveAsJPEG(const std::string &inputFilePath, const std::string &outputFilePath);
};


#endif /* CIMAGE_HPP */


