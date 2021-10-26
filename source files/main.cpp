#include "appspecificgui.h"
#include <iostream>

const int SCREEN_WIDTH = 638 + 56;
const int SCREEN_HEIGHT = 490;

struct ResultInfo
{
    float roe[2] = {};
    std::string assetName[2] = {};
    sf::Color assetColor[2] = {};
    bool direction = false;
};

int main()
{
    sf::Texture textureIcon;
    textureIcon.loadFromFile("resources/images/icons.png");
    textureIcon.setSmooth(true);

    sf::Texture textureArrows;
    textureArrows.loadFromFile("resources/images/arrows.png");
    textureArrows.setSmooth(true);

    sf::Image icon;
    icon.loadFromFile("resources/images/window_icon.png");

    sf::Font font;
    font.loadFromFile("resources/fonts/adobe_gothic_std_bold_subsetted.otf");
    //font.loadFromFile("resources/fonts/adobe_gothic_std_bold.otf");

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Arbitrage Helper", sf::Style::Close);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setVerticalSyncEnabled(true);

    sf::Vector2f uiRefPos((SCREEN_WIDTH - 638) / 2, 16);
    float viewSlotPosY = 0.f;
    float viewSlotPosYEasing = 0.f;
    sf::Vector2f mousePosition;
    bool mouseClickReleased = false;
    float scrollBarTime = 0.f;
    float scrollBarHeightEasing = 40 * 5;
    float scrollBarYEasing = uiRefPos.y;
    unsigned int sleepFrameCount = 0;
    std::string message = "";

    sf::RenderTexture renderTexture;
    renderTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    renderTexture.setSmooth(true);

    std::vector<InfoSlot*> infoSlots;
    GUIButton addButton(sf::FloatRect(uiRefPos.x, uiRefPos.y, 638, 24), "add_slot",
                        nullptr, "", 0, sf::Vector2f(), 0, sf::Color(),
                        &textureIcon, 0.5f, sf::IntRect(288, 0, 48, 48),
                        sf::Color::Transparent, uiOtherColors[1], sf::Color(240, 240, 240), sf::Color(225, 225, 225));

    GUIGraphicsVertexArray graphicsVA;
    graphicsVA.Add(sf::FloatRect(SCREEN_WIDTH - 7, uiRefPos.y, 7, 40 * 5), sf::Color(uiOtherColors[0].r, uiOtherColors[0].b, uiOtherColors[0].g, 0));
    graphicsVA.Add(sf::FloatRect(0, 0, SCREEN_WIDTH, 2), sf::Color::White);
    graphicsVA.Add(sf::FloatRect(0, uiRefPos.y + 40 * 5 - 2, SCREEN_WIDTH, SCREEN_HEIGHT - (uiRefPos.y + 40 * 5 - 2)), sf::Color::White);
    graphicsVA.Add(sf::FloatRect((SCREEN_WIDTH - 638) / 2.f, uiRefPos.y + 40 * 5 - 2, 638, 2), uiOtherColors[0]);
    graphicsVA.Add(sf::FloatRect(uiRefPos.x + 62, SCREEN_HEIGHT - uiRefPos.y - 22, 2, 20), uiOtherColors[1]);
    graphicsVA.Add(sf::FloatRect(SCREEN_WIDTH - uiRefPos.x - 47, SCREEN_HEIGHT - uiRefPos.y - 22, 2, 20), uiOtherColors[1]);

    GUIGraphicsText graphicsText[] =
    {
        GUIGraphicsText(sf::Vector2f(uiRefPos.x + 63, SCREEN_HEIGHT - uiRefPos.y - 48), "Intermediary", &font, 16, uiOtherColors[1], 0, sf::Vector2f()),
        GUIGraphicsText(sf::Vector2f(SCREEN_WIDTH - uiRefPos.x - 46, SCREEN_HEIGHT - uiRefPos.y - 48), "Trade Fees(%)", &font, 16, uiOtherColors[1], 0, sf::Vector2f())
    };

    float tradeFeesFactor[2] = {(100.f - 0.3f) / 100.f, (100.f - 0.1f) / 100.f};
    std::string miscInputFieldRef[4] = {"USDT", "USDT", "0.25", "0.1"};
    GUIInputField miscInputField[] =
    {
        GUIInputField("basic 1 0 0 0 0 end touppercase end truncatetolength 4 end", miscInputFieldRef[0],
                      sf::FloatRect(uiRefPos.x, SCREEN_HEIGHT - uiRefPos.y - 24, 56, 24), "updated_intermediary",
                      &font, 16, sf::Vector2f(0, -10), 0, uiOtherColors[1], uiOtherColors[1],
                      "name", 120,
                      sf::Color::Transparent, uiOtherColors[1], sf::Color(240, 240, 240), sf::Color(225, 225, 225),
                      sf::Color::Transparent, sf::Color(225, 225, 225)),
        GUIInputField("basic 1 0 0 0 0 end touppercase end truncatetolength 4 end", miscInputFieldRef[1],
                      sf::FloatRect(uiRefPos.x + 70, SCREEN_HEIGHT - uiRefPos.y - 24, 56, 24), "updated_intermediary",
                      &font, 16, sf::Vector2f(0, -10), 0, uiOtherColors[1], uiOtherColors[1],
                      "name", 120,
                      sf::Color::Transparent, uiOtherColors[1], sf::Color(240, 240, 240), sf::Color(225, 225, 225),
                      sf::Color::Transparent, sf::Color(225, 225, 225)),
        GUIInputField("fornumbers 1 end", miscInputFieldRef[2],
                      sf::FloatRect(SCREEN_WIDTH - uiRefPos.x - 92, SCREEN_HEIGHT - uiRefPos.y - 24, 39, 24), "updated_tradefee",
                      &font, 16, sf::Vector2f(0, -10), 0, uiOtherColors[1], uiOtherColors[1],
                      "0.0", 120,
                      sf::Color::Transparent, uiOtherColors[1], sf::Color(240, 240, 240), sf::Color(225, 225, 225),
                      sf::Color::Transparent, sf::Color(225, 225, 225)),
        GUIInputField("fornumbers 1 end", miscInputFieldRef[3],
                      sf::FloatRect(SCREEN_WIDTH - uiRefPos.x - 39, SCREEN_HEIGHT - uiRefPos.y - 24, 39, 24), "updated_tradefee",
                      &font, 16, sf::Vector2f(0, -10), 0, uiOtherColors[1], uiOtherColors[1],
                      "0.0", 120,
                      sf::Color::Transparent, uiOtherColors[1], sf::Color(240, 240, 240), sf::Color(225, 225, 225),
                      sf::Color::Transparent, sf::Color(225, 225, 225))
    };

    InfoSlot::intermediaryRef[0] = &miscInputFieldRef[0];
    InfoSlot::intermediaryRef[1] = &miscInputFieldRef[1];

    std::vector<ResultInfo> results[2];
    short resultSorting = 0;
    short resultIndex = 0;
    bool resultChangeDirection = false;
    sf::Vector2f resultRefPos((SCREEN_WIDTH - 180) / 2.f,  uiRefPos.y + 40 * 5 + (SCREEN_HEIGHT - uiRefPos.y - 40 * 5 - 34 - 165) / 2.f);
    if(GUIObject::SNAP_TO_PIXEL)
        resultRefPos = sf::Vector2f(sf::Vector2i(resultRefPos));

    sf::Sprite resultSprite(textureArrows);
    resultSprite.setPosition(resultRefPos);

    GUIGraphicsVertexArray resultVA;
    resultVA.Add(sf::FloatRect(resultRefPos.x - 33, resultRefPos.y + 17, 77, 33), sf::Color::Transparent);
    resultVA.Add(sf::FloatRect(resultRefPos.x + 180 - 44, resultRefPos.y + 17, 77, 33), sf::Color::Transparent);
    resultVA.Add(sf::FloatRect(resultRefPos.x - 33, resultRefPos.y + 115, 77, 33), sf::Color::Transparent);
    resultVA.Add(sf::FloatRect(resultRefPos.x + 180 - 44, resultRefPos.y + 115, 77, 33), sf::Color::Transparent);
    resultVA.Add(sf::FloatRect(uiRefPos.x + 7 + 65, uiRefPos.y * 2 + 40 * 5 + 17, 7, 7), uiConditionColors[1]);

    GUIGraphicsText resultText[] =
    {
        GUIGraphicsText(resultRefPos + sf::Vector2f(5, 21), "XRP", &font, 20, sf::Color::White, 0, sf::Vector2f()),
        GUIGraphicsText(resultRefPos + sf::Vector2f(174, 21), "XRP", &font, 20, sf::Color::White, 0, sf::Vector2f()),
        GUIGraphicsText(resultRefPos + sf::Vector2f(5, 119), "LTC", &font, 20, sf::Color::White, 0, sf::Vector2f()),
        GUIGraphicsText(resultRefPos + sf::Vector2f(174, 119), "LTC", &font, 20, sf::Color::White, 0, sf::Vector2f()),
        GUIGraphicsText(resultRefPos + sf::Vector2f(16, 71), "NGN", &font, 16, uiOtherColors[2], 1, sf::Vector2f()),
        GUIGraphicsText(resultRefPos + sf::Vector2f(185, 71), "USDT", &font, 16, uiOtherColors[2], 1, sf::Vector2f()),
        GUIGraphicsText(sf::Vector2f(SCREEN_WIDTH / 2.f, resultRefPos.y + 178), "+2.4%", &font, 28, uiConditionColors[1], 0, sf::Vector2f()),
        GUIGraphicsText(sf::Vector2f(uiRefPos.x + 22 + 65, uiRefPos.y * 2 + 40 * 5 + 6), "1/18", &font, 24, uiOtherColors[1], 1, sf::Vector2f()),
        GUIGraphicsText(sf::Vector2f(SCREEN_WIDTH / 2.f, resultRefPos.y + 70), "Add more fields+", &font, 16, uiOtherColors[1], 0, sf::Vector2f())
    };

    GUIButton resultButton[] =
    {
        GUIButton(sf::FloatRect(uiRefPos.x - 10 + 65, uiRefPos.y + 40 * 5 + (SCREEN_HEIGHT - uiRefPos.y - 40 * 5 - 96) / 2.f, 48, 48), "result_back",
                  nullptr, "", 0, sf::Vector2f(), 1, uiOtherColors[1],
                  &textureIcon, 1, sf::IntRect(144, 0, 48, 48),
                  sf::Color::Transparent, sf::Color::Transparent, sf::Color(240, 240, 240), sf::Color(225, 225, 225)),
        GUIButton(sf::FloatRect(SCREEN_WIDTH - uiRefPos.x - 48 + 10 - 65, uiRefPos.y + 40 * 5 + (SCREEN_HEIGHT - uiRefPos.y - 40 * 5 - 96) / 2.f, 48, 48), "result_next",
                  nullptr, "", 0, sf::Vector2f(), 1, sf::Color(),
                  &textureIcon, 1, sf::IntRect(192, 0, 48, 48),
                  sf::Color::Transparent, sf::Color::Transparent, sf::Color(240, 240, 240), sf::Color(225, 225, 225)),
        GUIButton(sf::FloatRect(uiRefPos.x + 65, uiRefPos.y * 2 + 40 * 5 + 6, 38 + resultText[7].GetGloabalBounds().width, 37), "result_sort",
                  nullptr, "", 0, sf::Vector2f(), 0, sf::Color(),
                  nullptr, 1, sf::IntRect(),
                  sf::Color::Transparent, sf::Color::Transparent, sf::Color(240, 240, 240), sf::Color(225, 225, 225)),
        GUIButton(sf::FloatRect(resultRefPos.x + 242, resultRefPos.y - 21, 48, 48), "result_direction",
                  nullptr, "", 0, sf::Vector2f(), 1, sf::Color(),
                  &textureIcon, 1, sf::IntRect(240, 0, 48, 48),
                  sf::Color::Transparent, sf::Color::Transparent, sf::Color(240, 240, 240), sf::Color(225, 225, 225))
    };
    for(int i = 0; i < 4; i++)
        resultButton[i].shouldIgnore(true);


    sf::Clock timer;

    //program loop
    while(window.isOpen())
    {
        float deltaTime = timer.restart().asSeconds();
        sleepFrameCount++;

        message = "";
        GUIInput input;
        input.mouse_position = (sf::Vector2f)sf::Mouse::getPosition(window);

        //handle input
        sf::Event event;
        while(true)
        {
            if(sleepFrameCount > 1)
            {
                window.waitEvent(event);
                timer.restart();
            }
            else if(window.pollEvent(event))
                ;
            else break;

            sleepFrameCount = 0;

            //handle events
            if(event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                input.mouse_leftButton = GUIInput::Released;
            else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                input.mouse_leftButton = GUIInput::Pressed;
            else if(event.type == sf::Event::MouseWheelMoved)
            {
                input.mouse_scrollDelta = event.mouseWheel.delta;

                if(sf::FloatRect(0, 0, SCREEN_WIDTH, uiRefPos.y + 40 * 5).contains(mousePosition))
                {
                    viewSlotPosY -= event.mouseWheel.delta * 40;
                    float viewMoveRange = 0;
                    if(infoSlots.size() >= 5)
                    {
                        viewMoveRange = (infoSlots.size() - 5 + 1) * 40;

                        if(scrollBarTime <= 0.3f)
                            scrollBarTime = 1.f - scrollBarTime / 0.3f * 0.1f;
                        else if(scrollBarTime <= 0.9f)
                            scrollBarTime = 0.9f;
                    }

                    if(viewSlotPosY < 0)
                        viewSlotPosY = 0.f;
                    else if(viewSlotPosY > viewMoveRange)
                        viewSlotPosY = viewMoveRange;
                }
            }
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
                input.key_return = GUIInput::Released;
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                input.key_escape = GUIInput::Released;
            else if(event.type == sf::Event::TextEntered && GUIInput::CharInput::input_shouldListenForInput)
            {
                if(event.text.unicode == 8) //backspace
                {
                    if(GUIInput::CharInput::input_string.size() > 0)
                        GUIInput::CharInput::input_string.pop_back();
                }
                else GUIInput::CharInput::input_string += (char)event.text.unicode;

                GUIInput::CharInput::input_wasUpdated = true;
            }
        }
        if(input.mouse_leftButton == -1 && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            input.mouse_leftButton = GUIInput::Hold;

        //logic
        viewSlotPosYEasing += (viewSlotPosY - viewSlotPosYEasing) * deltaTime * 10.f;
        sf::FloatRect slotViewRect(0, viewSlotPosYEasing, SCREEN_WIDTH, 40 * 5 + uiRefPos.y);
        GUIInput slotInput = input;
        slotInput.mouse_position.y += viewSlotPosYEasing;

        for(int i = 0; i < infoSlots.size(); i++)
            infoSlots[i]->Update(slotInput, message, slotViewRect);
        addButton.Update(slotInput, message, slotViewRect);
        for(int i = 0; i < 4; i++)
            miscInputField[i].Update(input, message);
        for(int i = 0; i < 4; i++)
            resultButton[i].Update(input, message);

        if(scrollBarTime > 0)
        {
            scrollBarTime -= deltaTime;
            if(scrollBarTime < 0)
                scrollBarTime = 0;
            sleepFrameCount = 0;

            scrollBarHeightEasing += ((infoSlots.size() >= 5 ? 5.f / (infoSlots.size() + 1) : 1.f) * (40 * 5 - 16) - scrollBarHeightEasing) * deltaTime * 20.f;
            scrollBarYEasing += (uiRefPos.y + (infoSlots.size() >= 5 ? (viewSlotPosYEasing / ((infoSlots.size() + 1) * 40)) : 0.f) * (40 * 5 - 16) - scrollBarYEasing) * deltaTime * 20.f;

            graphicsVA.GetVertex(0).position.y = graphicsVA.GetVertex(1).position.y = scrollBarYEasing;
            graphicsVA.GetVertex(2).position.y = graphicsVA.GetVertex(3).position.y = graphicsVA.GetVertex(0).position.y + scrollBarHeightEasing;

            float fadeFactor = 1.f;
            if(scrollBarTime >= 0.9f)
                fadeFactor = (1.f - scrollBarTime) / 0.1f;
            else if(scrollBarTime <= 0.3f)
                fadeFactor = scrollBarTime / 0.3f;
            graphicsVA.GetVertex(0).color.a = graphicsVA.GetVertex(1).color.a = graphicsVA.GetVertex(2).color.a = graphicsVA.GetVertex(3).color.a = fadeFactor * fadeFactor * fadeFactor * 255.f;

        }

        //render
        renderTexture.setView(renderTexture.getDefaultView());
        renderTexture.clear(sf::Color::White);

        renderTexture.setView(sf::View(sf::FloatRect(0, viewSlotPosYEasing, SCREEN_WIDTH, SCREEN_HEIGHT)));
        for(int i = 0; i < infoSlots.size(); i++)
            infoSlots[i]->Draw(renderTexture);
        addButton.Draw(renderTexture);
        renderTexture.display();

        renderTexture.setView(renderTexture.getDefaultView());
        graphicsVA.Draw(renderTexture);
        for(int i = 0; i < 4; i++)
            miscInputField[i].Draw(renderTexture);
        for(int i = 0; i < 2; i++)
            graphicsText[i].Draw(renderTexture);

        if(results[0].size() != 0)
        {
            for(int i = 0; i < 4; i++)
                resultButton[i].Draw(renderTexture);
            if(results[0].size() != 0)
                resultText[7].Draw(renderTexture);
            resultVA.Draw(renderTexture);
            if(results[resultSorting].size() > 0)
            {
                renderTexture.draw(resultSprite);
                for(int i = 0; i < 7; i++)
                    resultText[i].Draw(renderTexture);
            }
            else resultText[8].Draw(renderTexture);
        }
        else resultText[8].Draw(renderTexture);

        window.clear(sf::Color::White);
        window.draw(sf::Sprite(renderTexture.getTexture()));
        window.display();

        //handle messages
        if(message != "")
        {
            //std::cout << message << std::endl;

            std::stringstream ss(message);
            while(!ss.eof())
            {
                ss >> message;

                if(message == "add_slot")
                {
                    unsigned short index = infoSlots.size();
                    infoSlots.push_back(new InfoSlot(uiRefPos + sf::Vector2f(0, (24 + 16) * index), uiColors[index % UI_COLOR_COUNT], &font, &textureIcon));
                    addButton.SetPosition(uiRefPos +  sf::Vector2f(0, (24 + 16) * (index + 1)));

                    if(infoSlots.size() >= 5)
                        viewSlotPosY += 40;

                    if(infoSlots.size() >= 5)
                    {
                        if(scrollBarTime <= 0.3f)
                            scrollBarTime = 1.f - scrollBarTime / 0.3f * 0.1f;
                        else if(scrollBarTime <= 0.9f)
                            scrollBarTime = 0.9f;
                    }
                }
                else if(message == "updated_tradefee")
                {
                    tradeFeesFactor[0] = (100.f - (miscInputFieldRef[2].size() != 0 ? std::stof(miscInputFieldRef[2]) : 0)) / 100.f;
                    tradeFeesFactor[1] = (100.f - (miscInputFieldRef[3].size() != 0 ? std::stof(miscInputFieldRef[3]) : 0)) / 100.f;

                    int ssPos = ss.eof() ? ss.str().size() : (int)ss.tellg();
                    ss.str(ss.str() + " calculate_results");
                    ss.seekg(ssPos);
                }
                else if(message == "updated_intermediary")
                {
                    for(int i = 0; i < infoSlots.size(); i++)
                        infoSlots[i]->RefreshName();

                    int ssPos = ss.eof() ? ss.str().size() : (int)ss.tellg();
                    ss.str(ss.str() + " prepare_results");
                    ss.seekg(ssPos);
                }
                else if(message == "deleted_slot")
                {
                    for(int i = 0; i < infoSlots.size(); i++)
                        if(infoSlots[i]->shouldDelete())
                        {
                            delete infoSlots[i];
                            infoSlots.erase(infoSlots.begin() + i);
                            i--;
                        }

                    for(int i = 0; i < infoSlots.size(); i++)
                        infoSlots[i]->ChangeParameters(uiRefPos + sf::Vector2f(0, (24 + 16) * i), uiColors[i % UI_COLOR_COUNT]);
                    addButton.SetPosition(uiRefPos + sf::Vector2f(0, (24 + 16) * infoSlots.size()));

                    float viewMoveRange = infoSlots.size() >= 5 ? (infoSlots.size() - 5 + 1) * 40: 0;
                    if(viewSlotPosY < 0)
                        viewSlotPosY = 0.f;
                    else if(viewSlotPosY > viewMoveRange)
                        viewSlotPosY = viewMoveRange;

                    if(std::abs(int(viewSlotPosY - viewSlotPosYEasing)) > 0)
                    {
                        if(scrollBarTime <= 0.3f)
                            scrollBarTime = 1.f - scrollBarTime / 0.3f * 0.1f;
                        else if(scrollBarTime <= 0.9f)
                            scrollBarTime = 0.9f;
                    }

                    int ssPos = ss.eof() ? ss.str().size() : (int)ss.tellg();
                    ss.str(ss.str() + " calculate_results");
                    ss.seekg(ssPos);
                }
                else if(message == "calculate_results")
                {
                    for(int i = 0; i < 2; i++)
                        results[i].clear();

                    for(int a = 0; a < infoSlots.size(); a++)
                    {
                        bool slot1Tabs[2] = {};
                        double slot1Values[4] = {};
                        sf::Color slot1Color;
                        std::string slot1Name;
                        if(!infoSlots[a]->GetValues(slot1Tabs[0], slot1Values[0], slot1Values[1], slot1Tabs[1], slot1Values[2], slot1Values[3], slot1Color, slot1Name))
                            continue;

                        for(int b = a + 1; b < infoSlots.size(); b++)
                        {
                            bool slot2Tabs[2] = {};
                            double slot2Values[4] = {};
                            sf::Color slot2Color;
                            std::string slot2Name;
                            if(!infoSlots[b]->GetValues(slot2Tabs[0], slot2Values[0], slot2Values[1], slot2Tabs[1], slot2Values[2], slot2Values[3], slot2Color, slot2Name))
                                continue;

                            ResultInfo result;
                            result.assetColor[0] = slot1Color;
                            result.assetColor[1] = slot2Color;

                            result.assetName[0] = slot1Name;
                            result.assetName[1] = slot2Name;

                            result.roe[0] = (slot1Name != miscInputFieldRef[0] ? slot1Values[0] * tradeFeesFactor[0] : 1);
                            result.roe[0] = (slot2Name != miscInputFieldRef[0] ? result.roe[0] / (slot2Tabs[0] ? slot2Values[1] : slot2Values[0]) * tradeFeesFactor[0] : result.roe[0]);
                            result.roe[0] = (slot2Name != miscInputFieldRef[1] ? result.roe[0] * slot2Values[2] * tradeFeesFactor[1] : result.roe[0]);
                            result.roe[0] = (slot1Name != miscInputFieldRef[1] ? result.roe[0] / (slot1Tabs[1] ? slot1Values[3] : slot1Values[2]) * tradeFeesFactor[1] : result.roe[0]);
                            result.roe[0] = (result.roe[0] - 1) * 100.f;

                            result.roe[1] = (slot2Name != miscInputFieldRef[0] ? slot2Values[0] * tradeFeesFactor[0] : 1);
                            result.roe[1] = (slot1Name != miscInputFieldRef[0] ? result.roe[1] / (slot1Tabs[0] ? slot1Values[1] : slot1Values[0]) * tradeFeesFactor[0] : result.roe[1]);
                            result.roe[1] = (slot1Name != miscInputFieldRef[1] ? result.roe[1] * slot1Values[2] * tradeFeesFactor[1] : result.roe[1]);
                            result.roe[1] = (slot2Name != miscInputFieldRef[1] ? result.roe[1] / (slot2Tabs[1] ? slot2Values[3] * tradeFeesFactor[1] : slot2Values[2]) : result.roe[1]);
                            result.roe[1] = (result.roe[1] - 1) * 100.f;

                            if(result.roe[1] > result.roe[0])
                                result.direction = 1;

                            results[0].push_back(result);
                        }
                    }

                    resultSorting = 1;
                    resultChangeDirection = false;
                    resultIndex = 0;
                    if(results[0].size() != 0)
                        for(int b = 0; b < results[0].size(); b++)
                            if(results[0][b].roe[results[0][b].direction] > 0)
                            {
                                if(results[1].size() == 0)
                                    results[1].push_back(results[0][b]);
                                else
                                {
                                    for(int a = 0; a < results[1].size(); a++)
                                    {
                                        if(results[0][b].roe[results[0][b].direction] > results[1][a].roe[results[1][a].direction])
                                        {
                                            results[1].insert(results[1].begin() + a, results[0][b]);
                                            break;
                                        }
                                        else if(a == results[1].size() - 1)
                                        {
                                            results[1].push_back(results[0][b]);
                                            break;
                                        }
                                    }
                                }
                            }

                    int ssPos = ss.eof() ? ss.str().size() : (int)ss.tellg();
                    ss.str(ss.str() + " prepare_results");
                    ss.seekg(ssPos);
                }
                else if(message == "result_sort")
                {
                    resultSorting = !resultSorting;
                    resultChangeDirection = false;
                    resultIndex = 0;

                    int ssPos = ss.eof() ? ss.str().size() : (int)ss.tellg();
                    ss.str(ss.str() + " prepare_results");
                    ss.seekg(ssPos);
                }
                else if(message == "result_next")
                {
                    resultIndex++;
                    resultChangeDirection = false;

                    int ssPos = ss.eof() ? ss.str().size() : (int)ss.tellg();
                    ss.str(ss.str() + " prepare_results");
                    ss.seekg(ssPos);
                }
                else if(message == "result_back")
                {
                    resultIndex--;
                    resultChangeDirection = false;

                    int ssPos = ss.eof() ? ss.str().size() : (int)ss.tellg();
                    ss.str(ss.str() + " prepare_results");
                    ss.seekg(ssPos);
                }
                else if(message == "result_direction")
                {
                    resultChangeDirection = !resultChangeDirection;

                    int ssPos = ss.eof() ? ss.str().size() : (int)ss.tellg();
                    ss.str(ss.str() + " prepare_results");
                    ss.seekg(ssPos);
                }
                else if(message == "prepare_results")
                {
                    if(results[0].size() != 0)
                    {
                        if(results[resultSorting].size() != 0)
                        {
                            if(resultIndex  >= results[resultSorting].size() || resultIndex < 0)
                                resultIndex = 0;

                            for(int a = 0; a < 8; a++)
                                resultVA.GetVertex(a).color = results[resultSorting][resultIndex].assetColor[0];
                            for(int a = 0; a < 8; a++)
                                resultVA.GetVertex(8 + a).color = results[resultSorting][resultIndex].assetColor[1];
                            for(int a = 0; a < 2; a++)
                                resultText[a].SetString(results[resultSorting][resultIndex].assetName[0]);
                            for(int a = 0; a < 2; a++)
                                resultText[2 + a].SetString(results[resultSorting][resultIndex].assetName[1]);
                            for(int a = 0; a < 2; a++)
                                resultText[4 + a].SetString(miscInputFieldRef[a]);

                            resultButton[0].shouldIgnore(resultIndex == 0);
                            resultButton[1].shouldIgnore(resultIndex == results[resultSorting].size() - 1);
                            resultButton[3].shouldIgnore(false);

                            bool direction = resultChangeDirection ? !results[resultSorting][resultIndex].direction : results[resultSorting][resultIndex].direction;
                            if(direction)
                                resultSprite.setTextureRect(sf::IntRect(180, 0, -180, 165));
                            else resultSprite.setTextureRect(sf::IntRect(0, 0, 180, 165));

                            float roe = results[resultSorting][resultIndex].roe[direction];
                            if(roe != 0)
                            {
                                bool roeSign = roe >= 0;
                                int roeInt = abs((int)roe);
                                std::string roeString = std::to_string(roe + roeInt * (roeSign ? -1 : 1));
                                GUIInput::CharInput::Format_truncateToLength(roeString, roeSign ? 4 : 5);
                                while(roeString.size() != 0)
                                {
                                    if(roeString[0] == '.')
                                        break;
                                    roeString.erase(roeString.begin());
                                }
                                roeString = (roeSign ? "+" : "-") + std::to_string(roeInt) + roeString + "%";
                                resultText[6].SetString(roeString);
                            }
                            else resultText[6].SetString("0.0%");
                            resultText[6].SetColor(roe > 0 ? uiConditionColors[1] : (roe < 0 ? uiConditionColors[0] : uiOtherColors[0]));
                        }
                        else
                        {
                            for(int a = 0; a < 16; a++)
                                resultVA.GetVertex(a).color = sf::Color::Transparent;
                            resultText[8].SetString("No positive outcome was found.");

                            resultButton[0].shouldIgnore(true);
                            resultButton[1].shouldIgnore(true);
                            resultButton[3].shouldIgnore(true);
                        }

                        resultText[7].SetString(std::to_string(results[resultSorting].size() != 0 ? resultIndex + 1 : 0) + '/' + std::to_string(results[resultSorting].size()));
                        resultButton[2].SetSize(sf::Vector2f(29 + resultText[7].GetGloabalBounds().width, 28));
                        for(int a = 0; a < 4; a++)
                            resultVA.GetVertex(16 + a).color = resultSorting ? uiConditionColors[1] : uiOtherColors[0];
                        resultButton[2].shouldIgnore(false);
                    }
                    else
                    {
                        for(int a = 0; a < 4; a++)
                            resultButton[a].shouldIgnore(true);
                        resultText[8].SetString("Add more fields+");
                    }
                }
            }
        }
    }
}
