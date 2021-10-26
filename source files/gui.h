#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <sstream>
#include <SFML/Graphics.hpp>;

sf::Color ChangeColorTone(const sf::Color& color_, int tone_);

class InfoSlot;

class GUIInput
{
public:
    enum {Pressed = 0, Hold, Released};

    //Character Input
    class CharInput
    {
    public:
        static bool input_shouldListenForInput;
        static bool input_wasUpdated;
        static std::string input_string;

        static void Format(std::string& string_, const std::string& formating_);
        static void Format_basic(std::string& string_, const unsigned long& startPos_,
                                 const bool& allowLetters_, const bool& allowNumbers_, const bool& allowSpace_, const bool& allowReturn_, const bool& allowOtherChar_);
        static void Format_toUpperCase(std::string& string_, const unsigned long& startPos_);
        static void Format_toLowerCase(std::string& string_, const unsigned long& startPos_);
        static void Format_forNumbers(std::string& string_, const bool& allowDecimal_);
        static void Format_truncateToLength(std::string& string_, const unsigned long& length_);
        //plus more...
    };

    //Mouse Input
    sf::Vector2f mouse_position;
    short mouse_leftButton = -1;
    float mouse_scrollDelta = 0.f;

    //others...
    short key_return = -1;
    short key_escape = -1;
};

class GUIObject
{
public:
    static const bool SNAP_TO_PIXEL = true;

    virtual ~GUIObject() {};

    virtual bool Update(const GUIInput& input_, std::string& message_, const sf::FloatRect& viewRect_ = sf::FloatRect()) = 0;
    virtual void Draw(sf::RenderTarget& target_) = 0;
    virtual void shouldIgnore(bool state_)
    {
        ignore = state_;
    }

protected:
    sf::FloatRect boundary;
    bool ignore = false;
};

class GUIButton : public GUIObject
{
public:
    const float OUTLINE_THICKNESS = 2.f;

    GUIButton(const sf::FloatRect& boundary_, const std::string& message_,
              sf::Font* textFont_, const std::string& text_, const int& textSize_, const sf::Vector2f& textOffset_, const short& textAlignment_, const sf::Color& textColor_,
              sf::Texture* iconTextr_, const float& iconScale_, const sf::IntRect& iconTexrRect_,
              const sf::Color& clrMain_, const sf::Color& clrOutline_, const sf::Color& clrHighlight_, const sf::Color& clrPressed_,
              const short& keyActivateType_ = GUIInput::Released);

    virtual bool Update(const GUIInput& input_, std::string& message_, const sf::FloatRect& viewRect_ = sf::FloatRect());
    virtual void Draw(sf::RenderTarget& target_);
    virtual void SetBoundary(const sf::FloatRect& boundary_);
    virtual void SetPosition(const sf::Vector2f& pos_);
    virtual void MovePosition(const sf::Vector2f& move_);
    virtual void SetSize(const sf::Vector2f& size_);
    virtual void SetTextString(const std::string& string_);

protected:
    std::string message;
    sf::RectangleShape rectangle;
    sf::Color clrMain, clrHighlight, clrPressed;
    sf::Text text;
    short textAlignment;
    sf::Sprite icon;
    sf::Texture* iconTextr;
    float iconScale;
    bool shouldDraw;
    short keyActivateType;
};

class GUIToggleButton : public GUIButton
{
public:
    GUIToggleButton(const sf::FloatRect& boundary_, const std::string& messageOff_, const std::string& messageOn_,
              sf::Font* textFont_, const std::string& textOff_, const std::string& textOn_, const int& textSize_, const sf::Vector2f& textOffset_, const short& textAlignment_,
              const sf::Color& textColorOff_, const sf::Color& textColorOn_,
              sf::Texture* iconTextrOff_, const float& iconScaleOff_, const sf::IntRect& iconTexrRectOff_,
              sf::Texture* iconTextrOn_, const float& iconScaleOn_, const sf::IntRect& iconTexrRectOn_,
              const sf::Color& clrMainOff_, const sf::Color& clrOutlineOff_, const sf::Color& clrHighlightOff_, const sf::Color& clrPressedOff_,
              const sf::Color& clrMainOn_, const sf::Color& clrOutlineOn_, const sf::Color& clrHighlightOn_, const sf::Color& clrPressedOn_);

    virtual bool Update(const GUIInput& input_, std::string& message_, const sf::FloatRect& viewRect_ = sf::FloatRect());
    virtual void SetState(const bool& state_);
    const bool& GetState()
    {
        return state;
    }

    friend InfoSlot;

protected:
    bool state;
    sf::Color clrMainOff, clrOutlineOff, clrHighlightOff, clrPressedOff;
    sf::Color clrMainOn, clrOutlineOn, clrHighlightOn, clrPressedOn;
    sf::Texture* iconTextrOn;
    sf::IntRect iconTexrRectOff, iconTexrRectOn;
    std::string textOff, textOn;
    sf::Color textColorOff, textColorOn;
    std::string messageOff, messageOn;
    float iconScaleOn;
};

class GUIInputField : public GUIToggleButton
{
public:
    GUIInputField(const std::string& formating_, std::string& stringReference_,
                  const sf::FloatRect& boundary_, const std::string& message_,
                  sf::Font* textFont_, const int& textSize_, const sf::Vector2f& textOffset_, const short& textAlignment_,
                  const sf::Color& textColorOff_, const sf::Color& textColorOn_,
                  const std::string& textBlank_, const sf::Uint8& textBlankOpacity_,
                  const sf::Color& clrMainOff_, const sf::Color& clrOutlineOff_, const sf::Color& clrHighlightOff_, const sf::Color& clrPressedOff_,
                  const sf::Color& clrMainOn_, const sf::Color& clrOutlineOn_);

    virtual bool Update(const GUIInput& input_, std::string& message_, const sf::FloatRect& viewRect_ = sf::FloatRect());
    void Refresh();
    virtual void SetState(const bool& state_);
    void SetBlankText(const std::string& textBlank_, const sf::Uint8& textBlankOpacity_)
    {
        textBlank = textBlank_;
        textBlankOpacity = textBlankOpacity_;
    }

    friend InfoSlot;

protected:
    std::string formating;
    std::string textBlank;
    sf::Uint8 textBlankOpacity;
    std::string& stringReference;
};

class GUIGraphicsVertexArray : public GUIObject
{
public:
    GUIGraphicsVertexArray(const sf::PrimitiveType& type_ = sf::Quads, sf::Texture* textr_ = nullptr);

    void Add(const sf::Vertex& vertex_);
    void Add(sf::Vertex* vertices_, const unsigned int& size_);
    void Add(const sf::FloatRect& rect_, const sf::Color& color_);
    void MovePosition(const sf::Vector2f& move_);
    void Clear()
    {
        vertexArray.clear();
    }
    void Resize(const unsigned int& size_)
    {
        vertexArray.resize(size_);
    }
    unsigned int GetSize()
    {
        return vertexArray.getVertexCount();
    }
    sf::Vertex& GetVertex(const unsigned int& index_)
    {
        return vertexArray[index_];
    }

    virtual void Draw(sf::RenderTarget& target_);
    virtual bool Update(const GUIInput& input_, std::string& message_, const sf::FloatRect& viewRect_ = sf::FloatRect())
    {
    }

protected:
    sf::VertexArray vertexArray;
    sf::Texture* texture;
    sf::Vector2f refPosition;
};

class GUIGraphicsText : public GUIObject
{
public:
    GUIGraphicsText(const sf::Vector2f& position_, const std::string& string_, sf::Font* font_, const int& size_, const sf::Color& color_,
                    const short& alignment_, const sf::Vector2f& offset_) :
    alignment(alignment_), position{position_}
    {
        text.setString(string_);
        text.setCharacterSize(size_);
        text.setFillColor(color_);
        text.setOrigin(-offset_);
        if(font_ != nullptr)
            text.setFont(*font_);
        text.setPosition(position + sf::Vector2f((alignment * 0.5f - 0.5f) * text.getGlobalBounds().width, 0));
        if(SNAP_TO_PIXEL)
            text.setPosition((sf::Vector2f)(sf::Vector2i)text.getPosition());
    }

    void SetString(const std::string& string_)
    {
        text.setString(string_);
        text.setPosition(position + sf::Vector2f((alignment * 0.5f - 0.5f) * text.getGlobalBounds().width, 0));
        if(SNAP_TO_PIXEL)
            text.setPosition((sf::Vector2f)(sf::Vector2i)text.getPosition());
    }
    void SetColor(const sf::Color& color_)
    {
        text.setFillColor(color_);
    }
    void SetPosition(const sf::Vector2f& position_)
    {
        position = position_;
        text.setPosition(position + sf::Vector2f((alignment * 0.5f - 0.5f) * text.getGlobalBounds().width, 0));
        if(SNAP_TO_PIXEL)
            text.setPosition((sf::Vector2f)(sf::Vector2i)text.getPosition());
    }
    const sf::FloatRect& GetGloabalBounds()
    {
        text.getGlobalBounds();
    }
    virtual void Draw(sf::RenderTarget& target_)
    {
        if(!ignore)
            target_.draw(text);
    }
    virtual bool Update(const GUIInput& input_, std::string& message_, const sf::FloatRect& viewRect_ = sf::FloatRect())
    {
    }

protected:
    short alignment;
    sf::Vector2f position;
    sf::Text text;
};

#endif // GUI_H_INCLUDED
