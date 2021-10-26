#ifndef APPSPECIFICGUI_H_INCLUDED
#define APPSPECIFICGUI_H_INCLUDED

#include "gui.h"

static const short UI_COLOR_COUNT = 5;
static sf::Color uiColors[UI_COLOR_COUNT] = {sf::Color(249, 199, 79), sf::Color(67, 170, 139), sf::Color(155, 55, 189),
                                             sf::Color(79, 170, 249), sf::Color(243, 114, 44)};
static sf::Color uiConditionColors[] = {sf::Color(249, 65, 68), sf::Color(144, 190, 109)};
static sf::Color uiOtherColors[] = {sf::Color(204, 204, 204), sf::Color(179, 179, 179), sf::Color(153, 153, 153)};


class InfoSlot : public GUIObject
{
public:
    InfoSlot(const sf::Vector2f& position_, const sf::Color& color_, sf::Font* font_, sf::Texture* icons_);

    bool Update(const GUIInput& input_, std::string& message_, const sf::FloatRect& viewRect_ = sf::FloatRect());
    void Draw(sf::RenderTarget& target_);

    void ChangeParameters(const sf::Vector2f& position_, const sf::Color& color_);
    bool GetValues(bool& exch1_tabs_, double& exch1_value1, double& exch1_value2, bool& exch2_tabs_, double& exch2_value1, double& exch2_value2, sf::Color& color_, std::string& asset_name_);
    void Refresh(const bool& resetEx1Values_, const bool& resetEx2Values_);
    void RefreshName();
    const bool& shouldDelete()
    {
        return deleteObj;
    }

    static std::string* intermediaryRef[2];

protected:
    float values[4];
    std::string stringReference[5];
    GUIButton deleteButton;
    GUIToggleButton toggleButton[2];
    GUIInputField inputField[6];
    GUIGraphicsText nameText[4];
    GUIGraphicsVertexArray graphicsVA;
    sf::Vector2f position;
    sf::Color color;
    std::string lastName;
    bool lastState;
    bool shouldDraw;
    bool deleteObj;
};

#endif // APPSPECIFICGUI_H_INCLUDED
