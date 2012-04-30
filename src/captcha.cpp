/**********************************
*    = Header File Inclusion =
**********************************/

#include <list>
#include <boost/lexical_cast.hpp>
#include <Wt/WImage>
#include <Wt/WMemoryResource>
#include <Magick++.h>
#include "captcha.hpp"
#include "system.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace boost;
using namespace Wt;
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
*    = Constructos =
**********************************/

Captcha::Captcha()
{
    Result = 0;
}


/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

WImage *Captcha::GenCap()
{
    size_t n1 = (rand() % 10) + 1;
    size_t n2 = (rand() % 10) + 1;
    int rotate = (rand() % 7 - 3);
    int skew = (rand() % 9 - 4);

    Result = n1 * n2;

    string captcha(lexical_cast<string>(n1));
    captcha += " X ";
    captcha += lexical_cast<string>(n2);

    Image img(Geometry(115, 35), Color("white"));
    list<Drawable> drawList;

    drawList.push_back(DrawableTextAntialias(true));
    drawList.push_back(DrawableFont("../fnt/HAZELN.TTF"));
    drawList.push_back(DrawablePointSize(32));
    drawList.push_back(DrawableStrokeColor(Color("black")));
    drawList.push_back(DrawableFillColor(Color(0, 0, 0, MaxRGB)));
    drawList.push_back(DrawableTextDecoration(UnderlineDecoration));
    drawList.push_back(DrawableGravity(CenterGravity));


    drawList.push_back(DrawableRotation(rotate));
    drawList.push_back(DrawableRotation(skew));
    drawList.push_back(DrawableText(0, 0, captcha));

    img.draw(drawList);


    string capPath;

    do {
        capPath = "../tmp/captcha-";
        capPath += lexical_cast<string>(rand());
        capPath += ".png";
    } while (System::FileExists(capPath));


    img.write(capPath);

    WMemoryResource *capResource = new WMemoryResource("image/png");

    char *buffer = NULL;
    size_t size = 0;
    System::ReadFile(capPath, size, &buffer);

    capResource->setData(reinterpret_cast<const unsigned char*>(buffer),
                         static_cast<int>(size));

    delete buffer;

    System::EraseFile(capPath);

    WImage *capImage = new WImage(capResource, "Captcha");
    capImage->setStyleClass("captcha");

    return capImage;
}


/**********************************
*    = Event Handlers =
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


