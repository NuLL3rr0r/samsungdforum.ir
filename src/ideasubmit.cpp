/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/lexical_cast.hpp>
#include <Wt/WApplication>
#include <Wt/WCheckBox>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WMessageBox>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WVBoxLayout>
#include <Wt/WWidget>
#include "ideasubmit.hpp"
#include "crypto.hpp"
#include "db.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace boost;
using namespace cppdb;
using namespace Wt;
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

IdeaSubmit::IdeaSubmit(CgiRoot *cgi, Wt::WDialog *parentDialog, bool editMode, size_t editRowId) : BaseWidget(cgi),
    m_dlgParent(parentDialog),
    m_editMode(editMode),
    m_editRowId(editRowId),
    m_editCompletedHandler(this)
{
    this->clear();
    this->addWidget(Layout());

    this->setOverflow(WContainerWidget::OverflowAuto);

    Div *dvButtonWrappers = new Div();
    Div *dvButtons = new Div(dvButtonWrappers, "dvDialogButtons");
    WPushButton *submitButton;
    if (!m_editMode)
        submitButton = new WPushButton(m_lang->GetString("HOME_SUBMIT_IDEA_DLG_OK_BUTTON"), dvButtons);
    else
        submitButton = new WPushButton(m_lang->GetString("HOME_SUBMIT_IDEA_DLG_OK_EDIT_BUTTON"), dvButtons);
    WPushButton *returnButton = new WPushButton(m_lang->GetString("HOME_SUBMIT_IDEA_DLG_RETURN_BUTTON"), dvButtons);

    submitButton->setStyleClass("dialogButton");
    returnButton->setStyleClass("dialogButton");

    WVBoxLayout *layout = new WVBoxLayout();

    layout->addWidget(this);
    layout->setStretchFactor(this, 1);

    layout->addWidget(dvButtonWrappers );
    layout->setStretchFactor(dvButtonWrappers , 0);


    m_dlgParent->contents()->setLayout(layout);

    if (m_editMode) {
        string title;
        int typegamesentertainment;
        int typehygienehealth;
        int typenewsinfo;
        int typelearning;
        int typenonepresenceservices;
        int typeonlineshopping;
        int typeothers;
        string typeotherstext;
        string description;
        int audienceteens;
        int audienceparents;
        int audienceboys;
        int audiencegirls;
        int audiencewomen;
        int audiencemen;
        int audiencechilds;
        int audienceallfamilymembers;
        string usage;
        string similar;
        string difference;

        result r = m_db->Sql() << "SELECT title, "
                                  "typegamesentertainment, typehygienehealth, typenewsinfo, typelearning, "
                                  "typenonepresenceservices, typeonlineshopping, typeothers, typeotherstext, "
                                  "description, "
                                  "audienceteens, audienceparents, audienceboys, audiencegirls, audiencewomen, "
                                  "audiencemen, audiencechilds, audienceallfamilymembers, "
                                  "usage, similar, difference "
                                  "FROM [" + m_dbTables->Table("IDEAS")
                                  + "] WHERE rowid=?;" << m_editRowId << row;
        if (!r.empty()) {
            r >> title >> typegamesentertainment >> typehygienehealth
              >> typenewsinfo >> typelearning >> typenonepresenceservices >> typeonlineshopping
              >> typeothers >> typeotherstext >> description >> audienceteens >> audienceparents >> audienceboys
              >> audiencegirls >> audiencewomen >> audiencemen >> audiencechilds >> audienceallfamilymembers
              >> usage >> similar >> difference;

            title = Crypto::Decrypt(title);
            typeotherstext = Crypto::Decrypt(typeotherstext);
            description = Crypto::Decrypt(description);
            usage = Crypto::Decrypt(usage);
            similar = Crypto::Decrypt(similar);
            difference = Crypto::Decrypt(difference);

            m_typeGamesEntertainmentCheckBox->setChecked(typegamesentertainment);
            m_typeHygieneHealthCheckBox->setChecked(typehygienehealth);
            m_typeNewsInfoCheckBox->setChecked(typenewsinfo);
            m_typeLearningCheckBox->setChecked(typelearning);
            m_typeNonePresenceServicesCheckBox->setChecked(typenonepresenceservices);
            m_typeOnlineShoppingCheckBox->setChecked(typeonlineshopping);
            m_typeOthersCheckBox->setChecked(typeothers);
            if (typeothers)
                m_typeOthersTextLineEdit->setText(WString::fromUTF8(typeotherstext));
            m_titleLineEdit->setText(WString::fromUTF8(title));
            m_descriptionTextArea->setText(WString::fromUTF8(description));
            m_audienceTeensCheckBox->setChecked(audienceteens);
            m_audienceParentsCheckBox->setChecked(audienceparents);
            m_audienceBoysCheckBox->setChecked(audienceboys);
            m_audienceGirlsCheckBox->setChecked(audiencegirls);
            m_audienceWomenCheckBox->setChecked(audiencewomen);
            m_audienceMenCheckBox->setChecked(audiencemen);
            m_audienceChildsCheckBox->setChecked(audiencechilds);
            m_audienceAllFamilyMembersCheckBox->setChecked(audienceallfamilymembers);
            m_usageTextArea->setText(WString::fromUTF8(usage));
            m_similarTextArea->setText(WString::fromUTF8(similar));
            m_differenceTextArea->setText(WString::fromUTF8(difference));
        }
    }

    submitButton->clicked().connect(this, &IdeaSubmit::OnSubmitButtonPressed);
    returnButton->clicked().connect(this, &IdeaSubmit::OnReturnButtonPressed);
}


/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

/**********************************
*    = Event Handlers =
**********************************/

void IdeaSubmit::OnTypeOthersCheckBoxChanged()
{
    m_typeOthersTextLineEdit->setEnabled(m_typeOthersCheckBox->isChecked());
    m_typeOthersTextLineEdit->validator()->setMandatory(m_typeOthersCheckBox->isChecked());
}

void IdeaSubmit::OnSubmitButtonPressed()
{
    if (!m_typeGamesEntertainmentCheckBox->isChecked()
            && !m_typeHygieneHealthCheckBox->isChecked()
            && !m_typeNewsInfoCheckBox->isChecked()
            && !m_typeLearningCheckBox->isChecked()
            && !m_typeNonePresenceServicesCheckBox->isChecked()
            && !m_typeOnlineShoppingCheckBox->isChecked()
            && !m_typeOthersCheckBox->isChecked()) {
        m_typeGamesEntertainmentCheckBox->setFocus();
        return;
    }

    if (!Validate(m_typeOthersTextLineEdit)
            || !Validate(m_titleLineEdit)
            || !Validate(m_descriptionTextArea)
            || !Validate(m_usageTextArea)) {
        return;
    }

    if (!m_editMode) {
        m_db->Insert(m_dbTables->Table("IDEAS"), "email, title, typegamesentertainment, typehygienehealth, typenewsinfo, typelearning, typenonepresenceservices, typeonlineshopping, typeothers, typeotherstext, description, audienceteens, audienceparents, audienceboys, audiencegirls, audiencewomen, audiencemen, audiencechilds, audienceallfamilymembers, usage, similar, difference",
                     22, m_cgiEnv->LoggedInUserName.c_str(),
                     Crypto::Encrypt(m_titleLineEdit->text().toUTF8()).c_str(),
                     lexical_cast<string>(m_typeGamesEntertainmentCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_typeHygieneHealthCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_typeNewsInfoCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_typeLearningCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_typeNonePresenceServicesCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_typeOnlineShoppingCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_typeOthersCheckBox->isChecked()).c_str(),
                     Crypto::Encrypt(m_typeOthersTextLineEdit->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_descriptionTextArea->text().toUTF8()).c_str(),
                     lexical_cast<string>(m_audienceTeensCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceParentsCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceBoysCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceGirlsCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceWomenCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceMenCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceChildsCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceAllFamilyMembersCheckBox->isChecked()).c_str(),
                     Crypto::Encrypt(m_usageTextArea->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_similarTextArea->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_differenceTextArea->text().toUTF8()).c_str()
                     );

        m_msg = new WMessageBox(L"ارسال شد",
                                L"با تشکر، ایده شما با موفقیت ارسال شد.", Warning, NoButton);
        m_msg->addButton(L"تائید", Ok);
        m_msg->buttonClicked().connect(this, &IdeaSubmit::OnSubmitSuccessOK);
        m_msg->show();
    } else {
        m_db->Update(m_dbTables->Table("IDEAS"), "rowid", lexical_cast<string>(m_editRowId),
                     "title=?, typegamesentertainment=?, typehygienehealth=?, typenewsinfo=?, typelearning=?, typenonepresenceservices=?, typeonlineshopping=?, typeothers=?, typeotherstext=?, description=?, audienceteens=?, audienceparents=?, audienceboys=?, audiencegirls=?, audiencewomen=?, audiencemen=?, audiencechilds=?, audienceallfamilymembers=?, usage=?, similar=?, difference=?",
                     21,
                     Crypto::Encrypt(m_titleLineEdit->text().toUTF8()).c_str(),
                     lexical_cast<string>(m_typeGamesEntertainmentCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_typeHygieneHealthCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_typeNewsInfoCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_typeLearningCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_typeNonePresenceServicesCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_typeOnlineShoppingCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_typeOthersCheckBox->isChecked()).c_str(),
                     Crypto::Encrypt(m_typeOthersTextLineEdit->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_descriptionTextArea->text().toUTF8()).c_str(),
                     lexical_cast<string>(m_audienceTeensCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceParentsCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceBoysCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceGirlsCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceWomenCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceMenCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceChildsCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_audienceAllFamilyMembersCheckBox->isChecked()).c_str(),
                     Crypto::Encrypt(m_usageTextArea->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_similarTextArea->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_differenceTextArea->text().toUTF8()).c_str()
                     );

        m_editCompletedHandler.emit();

        m_msg = new WMessageBox(L"ذخیره شد",
                                L"با تشکر، تغییرات شما با موفقیت ذخیره شد.", Warning, NoButton);
        m_msg->addButton(L"تائید", Ok);
        m_msg->buttonClicked().connect(this, &IdeaSubmit::OnSubmitSuccessOK);
        m_msg->show();
    }
}

void IdeaSubmit::OnReturnButtonPressed()
{
    m_dlgParent->hide();
}

void IdeaSubmit::OnSubmitSuccessOK()
{
    delete m_msg;
    m_msg = NULL;

    m_dlgParent->hide();
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

Signal<> &IdeaSubmit::EditCompletedHandler()
{
    return m_editCompletedHandler;
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *IdeaSubmit::Layout()
{
    Div *root = new Div("IdeaSubmit");

    m_typeGamesEntertainmentCheckBox = new WCheckBox();
    m_typeHygieneHealthCheckBox = new WCheckBox();
    m_typeNewsInfoCheckBox = new WCheckBox();
    m_typeLearningCheckBox = new WCheckBox();
    m_typeNonePresenceServicesCheckBox = new WCheckBox();
    m_typeOnlineShoppingCheckBox = new WCheckBox();
    m_typeOthersCheckBox = new WCheckBox();
    m_typeOthersTextLineEdit = new WLineEdit();
    m_titleLineEdit = new WLineEdit();
    m_descriptionTextArea = new WTextArea();
    m_audienceTeensCheckBox = new WCheckBox();
    m_audienceParentsCheckBox = new WCheckBox();
    m_audienceBoysCheckBox = new WCheckBox();
    m_audienceGirlsCheckBox = new WCheckBox();
    m_audienceWomenCheckBox = new WCheckBox();
    m_audienceMenCheckBox = new WCheckBox();
    m_audienceChildsCheckBox = new WCheckBox();
    m_audienceAllFamilyMembersCheckBox = new WCheckBox();
    m_usageTextArea = new WTextArea();
    m_similarTextArea = new WTextArea();
    m_differenceTextArea = new WTextArea();

    m_typeOthersTextLineEdit->setDisabled(true);

    m_typeOthersTextLineEdit->resize(200, WLength::Auto);
    m_titleLineEdit->resize(200, WLength::Auto);
    m_descriptionTextArea->resize(350, 150);
    m_usageTextArea->resize(350, 150);
    m_similarTextArea->resize(350, 150);
    m_differenceTextArea->resize(350, 150);


    new WText(L"<h4>فرم مسابقه ایده نرم‌افزار تلویزیون هوشمند</h4>", root);

    new WText(L"* کاربر گرامی تکمیل اطلاعات قسمت ۱ تا ۵ ضروری است.", root);
    new WBreak(root);

    new WText(L"<hr />", root);
    new WBreak(root);

    new WText(L"۱- ایده نرم‌افزار شما مربوط به چه گروه نرم‌افزاری است؟", root);
    new WBreak(root);
    new WBreak(root);

    root->addWidget(m_typeGamesEntertainmentCheckBox);
    new WText(L"بازی و سرگرمی", root);
    new WBreak(root);

    root->addWidget(m_typeHygieneHealthCheckBox);
    new WText(L"بهداشت و سلامت", root);
    new WBreak(root);

    root->addWidget(m_typeNewsInfoCheckBox);
    new WText(L"اخبار و اطلاعات", root);
    new WBreak(root);

    root->addWidget(m_typeLearningCheckBox);
    new WText(L"آموزش", root);
    new WBreak(root);

    root->addWidget(m_typeNonePresenceServicesCheckBox);
    new WText(L"خدمات غیرحضوری", root);
    new WBreak(root);

    root->addWidget(m_typeOnlineShoppingCheckBox);
    new WText(L"خرید آنلاین", root);
    new WBreak(root);

    root->addWidget(m_typeOthersCheckBox);
    new WText(L"سایر", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_typeOthersTextLineEdit);
    new WBreak(root);
    new WBreak(root);

    new WText(L"راهنما: دسته‌بندی نرم‌افزارتان را بدقت انتخاب کنید. به جای کاربر فکر کنید. کاربر برای یافتن نرم‌افزاری با قابلیت نرم‌افزار شما، به کدام گزینه رجوع خواهد کرد؟", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"<hr />", root);
    new WBreak(root);
    new WText(L"ایده خاصی دارید؟ ذهن خلاق خود را پرواز دهید و برای تلویزیون هوشمند سامسنوگ خود، آرزوهای در دسترس خود را مکتوب کنید.", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"۲- عنوان ایده‌تان چیست؟", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_titleLineEdit);
    new WBreak(root);
    new WBreak(root);

    new WText(L"راهنما: در صورتی که ایده‌ خود را به نرم‌افزار تبدیل کرده‌اید نام نرم‌افزارتان را بنویسید.", root);
    new WBreak(root);

    new WBreak(root);
    new WText(L"<hr />", root);
    new WBreak(root);

    new WText(L"۳- ایده خود را شرح دهید", root);
    new WBreak(root);
    root->addWidget(m_descriptionTextArea);
    new WBreak(root);

    new WBreak(root);
    new WText(L"<hr />", root);
    new WBreak(root);

    new WText(L"۴- مخاطبان ایده شما در خانواده کدام دسته است؟", root);
    new WBreak(root);
    new WBreak(root);

    root->addWidget(m_audienceTeensCheckBox);
    new WText(L"نوجوانان", root);
    new WBreak(root);

    root->addWidget(m_audienceParentsCheckBox);
    new WText(L"والدین", root);
    new WBreak(root);

    root->addWidget(m_audienceBoysCheckBox);
    new WText(L"پسران", root);
    new WBreak(root);

    root->addWidget(m_audienceGirlsCheckBox);
    new WText(L"دختران", root);
    new WBreak(root);

    root->addWidget(m_audienceWomenCheckBox);
    new WText(L"زنان", root);
    new WBreak(root);

    root->addWidget(m_audienceMenCheckBox);
    new WText(L"مردان", root);
    new WBreak(root);

    root->addWidget(m_audienceChildsCheckBox);
    new WText(L"کودکان", root);
    new WBreak(root);

    root->addWidget(m_audienceAllFamilyMembersCheckBox);
    new WText(L"تمامی اعضای خانواده", root);
    new WBreak(root);

    new WBreak(root);
    new WText(L"<hr />", root);
    new WBreak(root);

    new WText(L"۵- ایده شما چه کاربردهایی دارد؟ و یا چه مساله ای را حل میکند؟", root);
    new WBreak(root);
    root->addWidget(m_usageTextArea);
    new WBreak(root);
    new WBreak(root);

    new WText(L"آیا مشابه ایده خودتان را در جایی دیده‌اید؟ توضیح دهید:", root);
    new WBreak(root);
    new WText(L"[لزومی به تکمیل این مورد نیست اما در داوری و امتیازدهی ایده بسیار موثر است]", root);
    new WBreak(root);
    root->addWidget(m_similarTextArea);
    new WBreak(root);
    new WBreak(root);

    new WText(L"تفاوت ایده شما با کار یا کارهای قبلی چیست؟", root);
    new WBreak(root);
    new WText(L"[لزومی به تکمیل این مورد نیست اما در داوری و امتیازدهی ایده بسیار موثر است]", root);
    new WBreak(root);
    root->addWidget(m_differenceTextArea);
    new WBreak(root);

    new WBreak(root);
    new WBreak(root);
    new WBreak(root);
    new WBreak(root);


    WLengthValidator *typeOthersValidator = new WLengthValidator(1, 128);
    WLengthValidator *titleValidator = new WLengthValidator(1, 128);
    WLengthValidator *descriptionValidator = new WLengthValidator(1, 65536);
    WLengthValidator *usageValidator = new WLengthValidator(1, 65536);

    typeOthersValidator->setMandatory(false);
    titleValidator->setMandatory(true);
    descriptionValidator->setMandatory(true);
    usageValidator->setMandatory(true);

    m_typeOthersTextLineEdit->setValidator(typeOthersValidator);
    m_titleLineEdit->setValidator(titleValidator);
    m_descriptionTextArea->setValidator(descriptionValidator);
    m_usageTextArea->setValidator(usageValidator);

    m_typeOthersCheckBox->changed().connect(this, &IdeaSubmit::OnTypeOthersCheckBoxChanged);

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


