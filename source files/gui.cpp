#include <sstream>
#include <iostream>
#include "gui.h"

//Public Functions
sf::Color ChangeColorTone(const sf::Color& color_, int tone_)
{
    int r = color_.r + tone_;
    int g = color_.g + tone_;
    int b = color_.b + tone_;

    if(r > 255) r = 255;
    if(r < 0) r = 0;

    if(g > 255) g = 255;
    if(g < 0) g = 0;

    if(b > 255) b = 255;
    if(b < 0) b = 0;

    return sf::Color(r, g, b);
}

//Character Input
bool GUIInput::CharInput::input_shouldListenForInput = false;
bool GUIInput::CharInput::input_wasUpdated = false;
std::string GUIInput::CharInput::input_string;

void GUIInput::CharInput::Format(std::string& string_, const std::string& formating_)
{
    std::stringstream ss(formating_);
    std::string communication;
    unsigned long startPos = 0;;

    while(!ss.eof())
    {
        //formatting instructions
        ss >> communication;

        if(communication == "startpos")
            ss >> startPos;
        else if(communication == "basic")
        {
            short step = 0;
            bool boolParam[5] = {};

            while(!ss.eof() && step < 5)
            {
                ss >> communication;
                if(communication == "end")
                    break;
                boolParam[step] = std::stoi(communication);
                step++;
            }

            Format_basic(string_, startPos,
                         boolParam[0], boolParam[1], boolParam[2], boolParam[3], boolParam[4]);
        }
        else if(communication == "touppercase")
            Format_toUpperCase(string_, startPos);
        else if(communication == "tolowercase")
            Format_toLowerCase(string_, startPos);
        else if(communication == "fornumbers")
        {
            bool boolParam = false;

            ss >> communication;
            if(communication != "end")
                boolParam = std::stoi(communication);

            Format_forNumbers(string_, boolParam);
        }
        else if(communication == "truncatetolength")
        {
            unsigned long length = 0;

            ss >> communication;
            if(communication != "end")
                length = std::stoul(communication);

            Format_truncateToLength(string_, length);
        }

        while(!ss.eof() && communication != "end")
            ss >> communication;
    }
}

void GUIInput::CharInput::Format_basic(std::string& string_, const unsigned long& startPos_,
                                       const bool& allowLetters_, const bool& allowNumbers_, const bool& allowSpace_, const bool& allowReturn_, const bool& allowOtherChar_)
{
    if(startPos_ < string_.size())
        for(auto itr = string_.begin() + startPos_; itr != string_.end();)
        {

            bool deleteCharacter = false;

            if(*itr >= 48 && *itr <= 57)
            {
                if(allowNumbers_) ;
                else deleteCharacter = true;
            }
            else if((*itr >= 65 && *itr <= 90) || (*itr >= 97 && *itr <= 122))
            {
                if(allowLetters_) ;
                else deleteCharacter = true;
            }
            else if(*itr == 32)
            {
                if(allowSpace_) ;
                else deleteCharacter = true;
            }
            else if(*itr == 13)
            {
                if(allowReturn_) ;
                else deleteCharacter = true;
            }
            else if(!allowOtherChar_)
                deleteCharacter = true;

            if(deleteCharacter)
            {
                itr = string_.erase(itr);
                continue;
            }
            itr++;
        }
}

void GUIInput::CharInput::Format_toUpperCase(std::string& string_, const unsigned long& startPos_)
{
    if(startPos_ < string_.size())
        for(auto itr = string_.begin() + startPos_; itr != string_.end(); itr++)
            if(*itr >= 97 && *itr <= 122)
                *itr -= 32;
}

void GUIInput::CharInput::Format_toLowerCase(std::string& string_, const unsigned long& startPos_)
{
    if(startPos_ < string_.size())
        for(auto itr = string_.begin() + startPos_; itr != string_.end(); itr++)
            if(*itr >= 65 && *itr <= 90)
                *itr += 32;
}

void GUIInput::CharInput::Format_forNumbers(std::string& string_, const bool& allowDecimal_)
{
    bool removeDecimalPoint = !allowDecimal_;
    for(auto itr = string_.begin(); itr != string_.end();)
    {
        if(*itr >= 48 && *itr <= 57)
        {
            if(itr != string_.begin() && string_.front() == 48 && (allowDecimal_ ? !removeDecimalPoint : true))
            {
                itr = string_.erase(string_.begin());
                continue;
            }
        }
        else
        {
            if(removeDecimalPoint == false && *itr == 46)
            {
                if(itr == string_.begin())
                {
                    itr = string_.insert(itr, '0');
                    itr++;
                }

                removeDecimalPoint = true;
                itr ++;
                continue;
            }
            itr = string_.erase(itr);
            continue;
        }
        itr++;
    }
}

void GUIInput::CharInput::Format_truncateToLength(std::string& string_, const unsigned long& length_)
{
    if(string_.size() > length_)
        string_.erase(string_.begin() + length_, string_.end());
}

//GUI Button
GUIButton::GUIButton(const sf::FloatRect& boundary_, const std::string& message_,
              sf::Font* textFont_, const std::string& text_, const int& textSize_, const sf::Vector2f& textOffset_, const short& textAlignment_, const sf::Color& textColor_,
              sf::Texture* iconTextr_, const float& iconScale_, const sf::IntRect& iconTexrRect_,
              const sf::Color& clrMain_, const sf::Color& clrOutline_, const sf::Color& clrHighlight_, const sf::Color& clrPressed_,
              const short& keyActivateType_) :
    clrMain(clrMain_), clrHighlight(clrHighlight_), clrPressed(clrPressed_),
    iconTextr(iconTextr_), iconScale(iconScale_), textAlignment(textAlignment_),
    message(message_), shouldDraw(true), keyActivateType(keyActivateType_)
{
    boundary = boundary_;
    if(SNAP_TO_PIXEL)
    {
        boundary.left = (int)boundary.left;
        boundary.top = (int)boundary.top;
    }

    rectangle.setSize(sf::Vector2f(boundary.width, boundary.height));
    rectangle.setPosition(boundary.left, boundary.top);
    rectangle.setFillColor(clrMain); rectangle.setOutlineColor(clrOutline_);
    rectangle.setOutlineThickness(-OUTLINE_THICKNESS);

    if(textFont_ != nullptr)
    {
        text.setFont(*textFont_);
        text.setCharacterSize(textSize_);
        text.setOrigin(-textOffset_);
        text.setFillColor(textColor_);
        text.setString(text_);
        text.setPosition(boundary_.left + boundary_.width * (textAlignment + 1.f) / 2.f - text.getGlobalBounds().width * (textAlignment + 1.f) / 2.f, boundary_.top + boundary_.height * 0.5f);
    }

    if(iconTextr_ != nullptr)
    {
        icon.setTexture(*iconTextr);
        if(iconTexrRect_ != sf::IntRect());
            icon.setTextureRect(iconTexrRect_);
        icon.setScale(sf::Vector2f(iconScale, iconScale));
        icon.setPosition(sf::Vector2f(boundary.left, boundary.top) + sf::Vector2f(boundary.width, boundary.height) * 0.5f
                         - sf::Vector2f(icon.getGlobalBounds().width, icon.getGlobalBounds().height) * 0.5f);
    }

    if(SNAP_TO_PIXEL)
    {
        text.setPosition((sf::Vector2f)(sf::Vector2i)text.getPosition());
        icon.setPosition((sf::Vector2f)(sf::Vector2i)icon.getPosition());
        rectangle.setPosition((sf::Vector2f)(sf::Vector2i)rectangle.getPosition());
    }
}

void GUIButton::SetBoundary(const sf::FloatRect& boundary_)
{
    boundary = boundary_;
    if(SNAP_TO_PIXEL)
    {
        boundary.left = (int)boundary.left;
        boundary.top = (int)boundary.top;
    }

    rectangle.setSize(sf::Vector2f(boundary.width, boundary.height));
    rectangle.setPosition(boundary.left, boundary.top);
    text.setPosition(boundary_.left + boundary_.width * (textAlignment + 1.f) / 2.f - text.getGlobalBounds().width * (textAlignment + 1.f) / 2.f, boundary_.top + boundary_.height * 0.5f);
    icon.setPosition(sf::Vector2f(boundary.left, boundary.top) + sf::Vector2f(boundary.width, boundary.height) * 0.5f
                         - sf::Vector2f(icon.getGlobalBounds().width, icon.getGlobalBounds().height) * 0.5f);
    if(SNAP_TO_PIXEL)
    {
        text.setPosition((sf::Vector2f)(sf::Vector2i)text.getPosition());
        icon.setPosition((sf::Vector2f)(sf::Vector2i)icon.getPosition());
        rectangle.setPosition((sf::Vector2f)(sf::Vector2i)rectangle.getPosition());
    }
}

void GUIButton::MovePosition(const sf::Vector2f& move_)
{
    boundary.left += move_.x;
    boundary.top += move_.y;
    text.move(move_);
    icon.move(move_);
    rectangle.move(move_);

    if(SNAP_TO_PIXEL)
    {
        boundary.left = (int)boundary.left;
        boundary.top = (int)boundary.top;
        text.setPosition((sf::Vector2f)(sf::Vector2i)text.getPosition());
        icon.setPosition((sf::Vector2f)(sf::Vector2i)icon.getPosition());
        rectangle.setPosition((sf::Vector2f)(sf::Vector2i)rectangle.getPosition());
    }
}

void GUIButton::SetPosition(const sf::Vector2f& pos_)
{
    MovePosition(pos_ - sf::Vector2f(boundary.left, boundary.top));
}

void GUIButton::SetSize(const sf::Vector2f& size_)
{
    SetBoundary(sf::FloatRect(boundary.left, boundary.top, size_.x, size_.y));
}

void GUIButton::SetTextString(const std::string& string_)
{
    text.setString(string_);
    text.setPosition(boundary.left + boundary.width * (textAlignment + 1.f) / 2.f - text.getGlobalBounds().width * (textAlignment + 1.f) / 2.f, boundary.top + boundary.height * 0.5f);
}

bool GUIButton::Update(const GUIInput& input_, std::string& message_, const sf::FloatRect& viewRect_)
{
    if(!ignore)
    {
        shouldDraw = (viewRect_.width == 0 && viewRect_.height == 0) ? true : boundary.intersects(viewRect_);

        if(shouldDraw)
        {
            if(boundary.contains(input_.mouse_position))
            {
                rectangle.setFillColor(input_.mouse_leftButton == GUIInput::Hold ? clrPressed : clrHighlight);
                if(input_.mouse_leftButton >= 0 && input_.mouse_leftButton == keyActivateType)
                {
                    if(message_.size() != 0)
                        message_ += ' ';
                    message_ += message;
                    return true;
                }
            }
            else rectangle.setFillColor(clrMain);
        }
    }
    return false;
}

 void GUIButton::Draw(sf::RenderTarget& target_)
 {
     if(!ignore && shouldDraw)
     {
         target_.draw(rectangle);
         target_.draw(text);
         if(icon.getTextureRect() != sf::IntRect())
            target_.draw(icon);
     }
 }

 //Toggle Button
 GUIToggleButton::GUIToggleButton(const sf::FloatRect& boundary_, const std::string& messageOff_, const std::string& messageOn_,
                                  sf::Font* textFont_, const std::string& textOff_, const std::string& textOn_, const int& textSize_, const sf::Vector2f& textOffset_, const short& textAlignment_,
                                  const sf::Color& textColorOff_, const sf::Color& textColorOn_,
                                  sf::Texture* iconTextrOff_, const float& iconScaleOff_, const sf::IntRect& iconTexrRectOff_,
                                  sf::Texture* iconTextrOn_, const float& iconScaleOn_, const sf::IntRect& iconTexrRectOn_,
                                  const sf::Color& clrMainOff_, const sf::Color& clrOutlineOff_, const sf::Color& clrHighlightOff_, const sf::Color& clrPressedOff_,
                                  const sf::Color& clrMainOn_, const sf::Color& clrOutlineOn_, const sf::Color& clrHighlightOn_, const sf::Color& clrPressedOn_) :
    GUIButton(boundary_, messageOff_,
              textFont_, textOff_, textSize_, textOffset_, textAlignment_, textColorOff_,
              iconTextrOff_, iconScaleOff_, iconTexrRectOff_,
              clrMainOff_, clrOutlineOff_, clrHighlightOff_, clrPressedOff_),
    iconTextrOn(iconTextrOn_),iconScaleOn(iconScaleOn_),
    iconTexrRectOn(iconTexrRectOn_), iconTexrRectOff(iconTexrRectOff_),
    clrMainOn(clrMainOn_), clrOutlineOn(clrOutlineOn_), clrHighlightOn(clrHighlightOn_),clrPressedOn(clrPressedOn_),
    clrMainOff(clrMainOff_), clrOutlineOff(clrOutlineOff_), clrHighlightOff(clrHighlightOff_),clrPressedOff(clrPressedOff_),
    textOff(textOff_), textOn(textOn_),
    textColorOff(textColorOff_), textColorOn(textColorOn_),
    messageOff(messageOff_), messageOn(messageOn_),
    state(false)
{
}

void GUIToggleButton::SetState(const bool& state_)
{
    if(state != state_)
    {
        state = state_;

        clrMain = state ? clrMainOn : clrMainOff;
        clrHighlight = state ? clrHighlightOn : clrHighlightOff;
        clrPressed = state ? clrPressedOn : clrPressedOff;

        rectangle.setOutlineColor(state ? clrOutlineOn : clrOutlineOff);

        sf::Texture* tempTexr = state ? iconTextrOn : iconTextr;
        if(tempTexr != nullptr)
        {
            icon.setTexture(*tempTexr, true);

            sf::IntRect tempRect = state ? iconTexrRectOn : iconTexrRectOff;
            if(tempRect != sf::IntRect());
                icon.setTextureRect(tempRect);

            icon.setScale(sf::Vector2f(1, 1) * (state ? iconScaleOn : iconScale));
            icon.setPosition(sf::Vector2f(boundary.left, boundary.top) + sf::Vector2f(boundary.width, boundary.height) * 0.5f
                             - sf::Vector2f(icon.getGlobalBounds().width, icon.getGlobalBounds().height) * 0.5f);
        }
        else icon.setTextureRect(sf::IntRect());

        text.setFillColor(state ? textColorOn : textColorOff);
        text.setString(state ? textOn : textOff);
        text.setPosition(boundary.left + boundary.width * (textAlignment + 1.f) / 2.f - text.getGlobalBounds().width * (textAlignment + 1.f) / 2.f, boundary.top + boundary.height * 0.5f);

        if(SNAP_TO_PIXEL)
        {
            icon.setPosition((sf::Vector2f)(sf::Vector2i)icon.getPosition());
            text.setPosition((sf::Vector2f)(sf::Vector2i)text.getPosition());
        }
    }
}

bool GUIToggleButton::Update(const GUIInput& input_, std::string& message_, const sf::FloatRect& viewRect_)
{
    if(GUIButton::Update(input_, message_, viewRect_))
    {
        SetState(!state);
        message = state ? messageOn : messageOff;
        return true;
    }
    return false;
}

//GUI Field
GUIInputField::GUIInputField(const std::string& formating_, std::string& stringReference_,
                  const sf::FloatRect& boundary_, const std::string& message_,
                  sf::Font* textFont_, const int& textSize_, const sf::Vector2f& textOffset_, const short& textAlignment_,
                  const sf::Color& textColorOff_, const sf::Color& textColorOn_,
                  const std::string& textBlank_, const sf::Uint8& textBlankOpacity_,
                  const sf::Color& clrMainOff_, const sf::Color& clrOutlineOff_, const sf::Color& clrHighlightOff_, const sf::Color& clrPressedOff_,
                  const sf::Color& clrMainOn_, const sf::Color& clrOutlineOn_) :
    GUIToggleButton(boundary_, "", message_,
                    textFont_, "", "", textSize_, textOffset_, textAlignment_,
                    textColorOff_, textColorOn_,
                    nullptr, 1, sf::IntRect(),
                    nullptr,1, sf::IntRect(),
                    clrMainOff_, clrOutlineOff_, clrHighlightOff_, clrPressedOff_,
                    clrMainOn_, clrOutlineOn_, clrMainOn_, clrMainOn_),
    formating(formating_), stringReference(stringReference_), textBlank(textBlank_), textBlankOpacity(textBlankOpacity_)
{
    if(stringReference.size() != 0)
    {
        text.setString(stringReference);
        text.setFillColor(textColorOff);
    }
    else
    {
        sf::Color color = textColorOff;
        color.a = textBlankOpacity;

        text.setString(textBlank);
        text.setFillColor(color);
    }
    text.setPosition(boundary.left + boundary.width * (textAlignment + 1.f) / 2.f - text.getGlobalBounds().width * (textAlignment + 1.f) / 2.f, boundary.top + boundary.height * 0.5f);
    if(SNAP_TO_PIXEL)
        text.setPosition((sf::Vector2f)(sf::Vector2i)text.getPosition());
}

void GUIInputField::SetState(const bool& state_)
{
    if(state != state_)
    {
        state = state_;

        clrMain = state ? clrMainOn : clrMainOff;
        clrHighlight = state ? clrHighlightOn : clrHighlightOff;
        clrPressed = state ? clrPressedOn : clrPressedOff;

        rectangle.setOutlineColor(state ? clrOutlineOn : clrOutlineOff);

        sf::Texture* tempTexr = state ? iconTextrOn : iconTextr;
        if(tempTexr != nullptr)
        {
            icon.setTexture(*tempTexr, true);
            icon.setScale(sf::Vector2f(1, 1) * (state ? iconScaleOn : iconScale));
            icon.setPosition(sf::Vector2f(boundary.left, boundary.top) + sf::Vector2f(boundary.width, boundary.height) * 0.5f
                             - sf::Vector2f(icon.getGlobalBounds().width, icon.getGlobalBounds().height) * 0.5f);
        }
        else icon.setTextureRect(sf::IntRect());

        if(SNAP_TO_PIXEL)
            icon.setPosition((sf::Vector2f)(sf::Vector2i)icon.getPosition());

        if(state == true)
        {
            GUIInput::CharInput::input_string = stringReference;
            GUIInput::CharInput::input_shouldListenForInput = true;
        }
        else
        {
            GUIInput::CharInput::input_string.clear();
            GUIInput::CharInput::input_shouldListenForInput = false;
        }
        Refresh();
    }
}

bool GUIInputField::Update(const GUIInput& input_, std::string& message_, const sf::FloatRect& viewRect_)
{
    std::string prvMessage = message_;
    bool buttonReturn = GUIButton::Update(input_, message_, viewRect_);
    message_ = prvMessage;

    bool changeState = false;
    if((buttonReturn && !state) || (ignore && state) ||
       (state && ((input_.mouse_leftButton == GUIInput::Released && !boundary.contains(input_.mouse_position)) || input_.key_return == GUIInput::Released || input_.key_escape == GUIInput::Released)))
    {
        SetState(!state);

        if(!state)
        {
            if(message_.size() != 0)
                message_ += ' ';
            message_ += messageOn;
            return true;
        }
    }

    //Listen for character input
    if(state)
    {
        GUIInput::CharInput::input_shouldListenForInput = true;
        if(GUIInput::CharInput::input_wasUpdated)
        {
            GUIInput::CharInput::input_wasUpdated = false;
            GUIInput::CharInput::Format(GUIInput::CharInput::input_string, formating);
            Refresh();
        }
    }

    return false;
}

void GUIInputField::Refresh()
{
    if(state)
        stringReference = GUIInput::CharInput::input_string;

    if(stringReference.size() > 0)
    {
        text.setString(stringReference);
        text.setFillColor(state ? textColorOn : textColorOff);
    }
    else
    {
        sf::Color color = state ? textColorOn : textColorOff;
        color.a = textBlankOpacity;

        text.setString(textBlank);
        text.setFillColor(color);
    }
    text.setPosition(boundary.left + boundary.width * (textAlignment + 1.f) / 2.f - text.getGlobalBounds().width * (textAlignment + 1.f) / 2.f, boundary.top + boundary.height * 0.5f);

    if(SNAP_TO_PIXEL)
        text.setPosition((sf::Vector2f)(sf::Vector2i)text.getPosition());
}

//GUI Graphics Vertex Array
GUIGraphicsVertexArray::GUIGraphicsVertexArray(const sf::PrimitiveType& type_, sf::Texture* textr_) :
    vertexArray(type_), texture(textr_)
{
}

void GUIGraphicsVertexArray::Add(const sf::Vertex& vertex_)
{
    vertexArray.append(vertex_);
}

void GUIGraphicsVertexArray::Add(sf::Vertex* vertices_, const unsigned int& size_)
{
    for(int i = 0; i < size_; i++)
        vertexArray.append(vertices_[i]);
}

void GUIGraphicsVertexArray::Add(const sf::FloatRect& rect_, const sf::Color& color_)
{
    vertexArray.append(sf::Vertex(sf::Vector2f(rect_.left, rect_.top) , color_));
    vertexArray.append(sf::Vertex(sf::Vector2f(rect_.left + rect_.width, rect_.top) , color_));
    vertexArray.append(sf::Vertex(sf::Vector2f(rect_.left + rect_.width, rect_.top + rect_.height) , color_));
    vertexArray.append(sf::Vertex(sf::Vector2f(rect_.left, rect_.top + rect_.height) , color_));
}

void GUIGraphicsVertexArray::Draw(sf::RenderTarget& target_)
{
    if(!ignore)
        target_.draw(vertexArray, texture);
}

void GUIGraphicsVertexArray::MovePosition(const sf::Vector2f& move_)
{
    for(int i = 0; i < vertexArray.getVertexCount(); i++)
        vertexArray[i].position += move_;
}
