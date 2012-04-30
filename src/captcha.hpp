#ifndef CAPTCHA_HPP
#define CAPTCHA_HPP


namespace Wt {
    class WImage;
}

namespace SamsungDForumIr {
    class Captcha;
}

class SamsungDForumIr::Captcha
{
public:
    std::size_t Result;

public:
    Captcha();

    Wt::WImage *GenCap();
};


#endif /* CAPTCHA_HPP */


