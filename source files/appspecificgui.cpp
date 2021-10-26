#include "appspecificgui.h"
#include <iostream>

std::string* InfoSlot::intermediaryRef[2] = {nullptr, nullptr};

InfoSlot::InfoSlot(const sf::Vector2f& position_, const sf::Color& color_, sf::Font* font_, sf::Texture* icons_) :
    deleteObj(false),
    shouldDraw(true),
    position(position_),
    values{},
    stringReference{},
    lastState(false),
    color(color_),
    inputField{GUIInputField("fornumbers 1 end", stringReference[0],
                             sf::FloatRect(position_.x + 112, position_.y, 66, 24), "update_value 0",
                             font_, 16, sf::Vector2f(-10, -10), 1, sf::Color::White, color_,
                             "buy", 90,
                             color_, sf::Color::Transparent, ChangeColorTone(color_, 15), ChangeColorTone(color_, -15),
                             sf::Color::Transparent, color_),
               GUIInputField("fornumbers 1 end", stringReference[1],
                             sf::FloatRect(position_.x + 200, position_.y, 66, 24), "update_value 1",
                             font_, 16, sf::Vector2f(-10, -10), 1, sf::Color::White, color_,
                             "sell", 90,
                             color_, sf::Color::Transparent, ChangeColorTone(color_, 15), ChangeColorTone(color_, -15),
                             sf::Color::Transparent, color_),
               GUIInputField("fornumbers 1 end", stringReference[2],
                             sf::FloatRect(position_.x + 448, position_.y, 66, 24), "update_value 2",
                             font_, 16, sf::Vector2f(-10, -10), 1, sf::Color::White, color_,
                             "buy", 90,
                             color_, sf::Color::Transparent, ChangeColorTone(color_, 15), ChangeColorTone(color_, -15),
                             sf::Color::Transparent, color_),
               GUIInputField("fornumbers 1 end", stringReference[3],
                             sf::FloatRect(position_.x + 536, position_.y, 66, 24), "update_value 3",
                             font_, 16, sf::Vector2f(-10, -10), 1, sf::Color::White, color_,
                             "sell", 90,
                             color_, sf::Color::Transparent, ChangeColorTone(color_, 15), ChangeColorTone(color_, -15),
                             sf::Color::Transparent, color_),
               GUIInputField("basic 1 0 0 0 0 end touppercase end truncatetolength 4 end", stringReference[4],
                             sf::FloatRect(position_.x, position_.y, 90, 24), "name",
                             font_, 16, sf::Vector2f(0, -10), 0, sf::Color(255, 255, 255, 0), sf::Color(color_.r, color_.g, color_.b, 0),
                             "name", 90,
                             color_, sf::Color::Transparent, ChangeColorTone(color_, 15), ChangeColorTone(color_, -15),
                             sf::Color::Transparent, color_),
                GUIInputField("basic 1 0 0 0 0 end touppercase end truncatetolength 4 end", stringReference[4],
                             sf::FloatRect(position_.x + 336, position_.y, 90, 24), "name",
                             font_, 16, sf::Vector2f(0, -10), 0, sf::Color(255, 255, 255, 0), sf::Color(color_.r, color_.g, color_.b, 0),
                             "name", 90,
                             color_, sf::Color::Transparent, ChangeColorTone(color_, 15), ChangeColorTone(color_, -15),
                             sf::Color::Transparent, color_)},
    nameText{GUIGraphicsText(position_, "", font_, 16, sf::Color::White, 1, sf::Vector2f(0, 2)),
            GUIGraphicsText(position_, "", font_, 12, sf::Color::White, 1, sf::Vector2f(3, 6)),
            GUIGraphicsText(position_, "", font_, 16, sf::Color::White, 1, sf::Vector2f(0, 2)),
            GUIGraphicsText(position_, "", font_, 12, sf::Color::White, 1, sf::Vector2f(3, 6))},
    toggleButton{GUIToggleButton(sf::FloatRect(position_.x + 278, position_.y, 24, 24), "bar_ex1", "bar_ex1",
                    nullptr, "", "", 0, sf::Vector2f(), 0.8f, sf::Color(), sf::Color(),
                    icons_, 0.5f, sf::IntRect(0, 0, 48, 48),
                    icons_, 0.5f, sf::IntRect(48, 0, 48, 48),
                    sf::Color::Transparent, uiOtherColors[1], sf::Color(240, 240, 240), sf::Color(225, 225, 225),
                    sf::Color::Transparent, uiOtherColors[1], sf::Color(240, 240, 240), sf::Color(225, 225, 225)),
                 GUIToggleButton(sf::FloatRect(position_.x + 614, position_.y, 24, 24), "bar_ex2", "bar_ex2",
                    nullptr, "", "", 0, sf::Vector2f(), 0.8f, sf::Color(), sf::Color(),
                    icons_, 0.5f, sf::IntRect(0, 0, 48, 48),
                    icons_, 0.5f, sf::IntRect(48, 0, 48, 48),
                    sf::Color::Transparent, uiOtherColors[1], sf::Color(240, 240, 240), sf::Color(225, 225, 225),
                    sf::Color::Transparent, uiOtherColors[1], sf::Color(240, 240, 240), sf::Color(225, 225, 225))},
    deleteButton(sf::FloatRect(position_.x + 640, position_.y, 24, 24), "delete",
                 nullptr, "", 0, sf::Vector2f(), 0, sf::Color(),
                 icons_, 0.5f, sf::IntRect(96, 0, 48, 48),
                 sf::Color::Transparent, sf::Color::Transparent, sf::Color(240, 240, 240), sf::Color(225, 225, 225))
{
    boundary = sf::FloatRect(position_.x - 28, position_.y, 694, 24);

    graphicsVA.Add(sf::Vertex(position + sf::Vector2f(318, -2), uiOtherColors[0]));
    graphicsVA.Add(sf::Vertex(position + sf::Vector2f(318 + 2, -2), uiOtherColors[0]));
    graphicsVA.Add(sf::Vertex(position + sf::Vector2f(318 + 2, 26), uiOtherColors[0]));
    graphicsVA.Add(sf::Vertex(position + sf::Vector2f(318, 26), uiOtherColors[0]));

    Refresh(true, true);
}

void InfoSlot::RefreshName()
{
    lastName = stringReference[4];
    lastState = inputField[4].GetState();

    nameText[0].SetString(stringReference[4]);
    nameText[2].SetString(stringReference[4]);
    nameText[1].SetString('/' + *intermediaryRef[0]);
    nameText[3].SetString('/' + *intermediaryRef[1]);

    sf::FloatRect nameRect = nameText[0].GetGloabalBounds();
    float refPosX[] = {(90 - 3 - nameRect.width - nameText[1].GetGloabalBounds().width) / 2.f, 336 + (90 - 3 - nameRect.width - nameText[3].GetGloabalBounds().width) / 2.f};
    nameText[0].SetPosition(position + sf::Vector2f(refPosX[0], 0));
    nameText[1].SetPosition(position + sf::Vector2f(refPosX[0] + nameRect.width, 0));
    nameText[2].SetPosition(position + sf::Vector2f(refPosX[1], 0));
    nameText[3].SetPosition(position + sf::Vector2f(refPosX[1] + nameRect.width, 0));

    sf::Color clr = sf::Color::White;
    if(inputField[4].GetState())
        clr = color;
    nameText[0].SetColor(clr);
    nameText[2].SetColor(clr);
    clr.a = 180;
    nameText[1].SetColor(clr);
    nameText[3].SetColor(clr);

    if(inputField[4].GetState())
        inputField[5].Refresh();
}

bool InfoSlot::Update(const GUIInput& input_, std::string& message_, const sf::FloatRect& viewRect_)
{
    deleteButton.shouldIgnore(!boundary.contains(input_.mouse_position));
    shouldDraw = viewRect_ != sf::FloatRect() ? boundary.intersects(viewRect_) : true;

    std::string intraMessage;
    for(int i = 0; i < 6; i++)
        inputField[i].Update(input_, intraMessage, viewRect_);
    for(int i = 0; i < 2; i++)
        toggleButton[i].Update(input_, intraMessage, viewRect_);
    deleteButton.Update(input_, intraMessage, viewRect_);

    if(inputField[4].GetState() and !inputField[5].GetState())
        inputField[5].SetState(true);
    else if(!inputField[4].GetState() and inputField[5].GetState())
        inputField[4].SetState(true);

    if(lastName != stringReference[4] || lastState != inputField[4].GetState())
        RefreshName();

    if(intraMessage != "")
    {
        std::stringstream ss(intraMessage);
        while(!ss.eof())
        {
            ss >> intraMessage;

            if(intraMessage == "update_value")
            {
                bool prevCompleteState = (toggleButton[0].GetState() ? values[0] > 0 && values[1] > 0 : values[0] > 0) && (toggleButton[1].GetState() ? values[2] > 0 && values[3] > 0 : values[2] > 0);

                short index = 0;
                ss >> index;
                if(stringReference[index].size() != 0)
                    values[index] = std::stod(stringReference[index]);
                else values[index] = 0;

                if(prevCompleteState || ((toggleButton[0].GetState() ? values[0] > 0 && values[1] > 0 : values[0] > 0) && (toggleButton[1].GetState() ? values[2] > 0 && values[3] > 0 : values[2] > 0)))
                {
                    if(message_.size() != 0)
                        message_ += ' ';
                    message_ += "calculate_results";
                }
            }
            else if(intraMessage == "name")
            {
                inputField[4].SetState(false);
                inputField[5].SetState(false);

                if((toggleButton[0].GetState() ? values[0] > 0 && values[1] > 0 : values[0] > 0) && (toggleButton[1].GetState() ? values[2] > 0 && values[3] > 0 : values[2] > 0))
                {
                    if(message_.size() != 0)
                        message_ += ' ';
                    message_ += "calculate_results";
                }
            }
            else if(intraMessage == "bar_ex1")
                Refresh(true, false);
            else if(intraMessage == "bar_ex2")
                Refresh(false, true);
            else if(intraMessage == "delete")
            {
                deleteObj = true;
                if(message_.size() != 0)
                    message_ += ' ';
                message_ += "deleted_slot";
            }
        }
    }
}

void InfoSlot::Draw(sf::RenderTarget& target_)
{
    if(!ignore && shouldDraw)
    {
        for(int i = 0; i < 6; i++)
            inputField[i].Draw(target_);
        for(int i = 0; stringReference[4].size() > 0 && i < 4; i++)
            nameText[i].Draw(target_);
        for(int i = 0; i < 2; i++)
            toggleButton[i].Draw(target_);
        graphicsVA.Draw(target_);
        deleteButton.Draw(target_);
    }
}

void InfoSlot::Refresh(const bool& resetEx1Values_, const bool& resetEx2Values_)
{
    graphicsVA.Resize(4);
    if(toggleButton[0].GetState())
    {
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(102, 3), uiConditionColors[1]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(102 + 7, 3), uiConditionColors[1]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(102 + 7, 21), uiConditionColors[1]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(102, 21), uiConditionColors[1]));

        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(190, 3), uiConditionColors[0]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(190 + 7, 3), uiConditionColors[0]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(190 + 7, 21), uiConditionColors[0]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(190, 21), uiConditionColors[0]));

        inputField[0].SetSize(sf::Vector2f(66, 24));
        inputField[1].SetSize(sf::Vector2f(66, 24));
        inputField[0].SetBlankText("buy", 90);
        inputField[1].shouldIgnore(false);
        if(resetEx1Values_)
        {
            stringReference[0] = ""; inputField[0].Refresh();
            stringReference[1] = ""; inputField[1].Refresh();
        }
    }
    else
    {
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(102, 3), uiOtherColors[0]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(102 + 7, 3), uiOtherColors[0]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(102 + 7, 21), uiOtherColors[0]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(102, 21), uiOtherColors[0]));

        inputField[0].SetSize(sf::Vector2f(154, 24));
        inputField[0].SetBlankText("price", 90);
        inputField[1].shouldIgnore(true);
        if(resetEx1Values_)
        {
            stringReference[0] = ""; inputField[0].Refresh();
        }
    }

    if(toggleButton[1].GetState())
    {
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(438, 3), uiConditionColors[1]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(438 + 7, 3), uiConditionColors[1]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(438 + 7, 21), uiConditionColors[1]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(438, 21), uiConditionColors[1]));

        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(526, 3), uiConditionColors[0]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(526 + 7, 3), uiConditionColors[0]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(526 + 7, 21), uiConditionColors[0]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(526, 21), uiConditionColors[0]));

        inputField[2].SetSize(sf::Vector2f(66, 24));
        inputField[3].SetSize(sf::Vector2f(66, 24));
        inputField[2].SetBlankText("buy", 90);
        inputField[3].shouldIgnore(false);
        if(resetEx2Values_)
        {
            stringReference[2] = ""; inputField[2].Refresh();
            stringReference[3] = ""; inputField[3].Refresh();
        }
    }
    else
    {
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(438, 3), uiOtherColors[0]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(438 + 7, 3), uiOtherColors[0]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(438 + 7, 21), uiOtherColors[0]));
        graphicsVA.Add(sf::Vertex(position + sf::Vector2f(438, 21), uiOtherColors[0]));

        inputField[2].SetSize(sf::Vector2f(154, 24));
        inputField[2].SetBlankText("price", 90);
        inputField[3].shouldIgnore(true);
        if(resetEx2Values_)
        {
            stringReference[2] = ""; inputField[2].Refresh();
        }
    }
}

void InfoSlot::ChangeParameters(const sf::Vector2f& position_, const sf::Color& color_)
{
    sf::Vector2f diff = position_ - position;
    position = position_;
    color = color_;

    for(int i = 0; i < 6; i++)
    {
        inputField[i].MovePosition(diff);

        inputField[i].clrMainOff = inputField[i].clrOutlineOn = inputField[i].textColorOn = color_;
        inputField[i].clrHighlightOff = ChangeColorTone(color_, 15);
        inputField[i].clrPressedOff = ChangeColorTone(color_, -15);

        inputField[i].clrMain = inputField[i].state ? inputField[i].clrMainOn : inputField[i].clrMainOff;
        inputField[i].clrHighlight = inputField[i].state ? inputField[i].clrHighlightOn : inputField[i].clrHighlightOff;
        inputField[i].clrPressed = inputField[i].state ? inputField[i].clrPressedOn : inputField[i].clrPressedOff;
        inputField[i].rectangle.setOutlineColor(inputField[i].state ? inputField[i].clrOutlineOn : inputField[i].clrOutlineOff);
    }
    inputField[4].textColorOn.a = inputField[5].textColorOn.a = 0;
    for(int i = 0; i < 2; i++)
        toggleButton[i].MovePosition(diff);
    graphicsVA.MovePosition(diff);
    deleteButton.MovePosition(diff);
    boundary.left = position_.x;
    boundary.top = position_.y;
    RefreshName();
}

bool InfoSlot::GetValues(bool& exch1_tabs_, double& exch1_value1_, double& exch1_value2_, bool& exch2_tabs_, double& exch2_value1_, double& exch2_value2_,  sf::Color& color_, std::string& asset_name_)
{
    if(toggleButton[0].GetState() && values[0] > 0 && values[1] > 0)
    {
        exch1_tabs_ = toggleButton[0].GetState();
        exch1_value1_ = values[0];
        exch1_value2_ = values[1];
    }
    else if(!toggleButton[0].GetState() && values[0] > 0)
    {
        exch1_tabs_ = toggleButton[0].GetState();
        exch1_value1_ = values[0];
    }
    else return false;

    if(toggleButton[1].GetState() && values[2] > 0 && values[3] > 0)
    {
        exch2_tabs_ = toggleButton[1].GetState();
        exch2_value1_ = values[2];
        exch2_value2_ = values[3];
        color_ = inputField[0].clrMainOff;
        asset_name_ = stringReference[4];
        return true;
    }
    else if(!toggleButton[1].GetState() && values[2] > 0)
    {
        exch2_tabs_ = toggleButton[1].GetState();
        exch2_value1_ = values[2];
        color_ = inputField[0].clrMainOff;
        asset_name_ = stringReference[4];
        return true;
    }
    return false;
}
