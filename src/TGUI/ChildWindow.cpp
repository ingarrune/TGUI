/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012 Bruno Van de Velde (VDV_B@hotmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/TGUI.hpp>

#include <SFML/OpenGL.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChildWindow::ChildWindow() :
    layout          (ChildWindow::LayoutRight),
    distanceToSide  (5),
    borderColor     (0, 0, 0),
    title           (""),
    m_TitleBarHeight(0),
    m_LoadedPathname(""),
    m_SplitImage    (false),
    m_Opacity       (255)
    {
        m_CloseButton = new tgui::Button();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChildWindow::ChildWindow(const ChildWindow& childWindowToCopy) :
    Panel             (childWindowToCopy),
    OBJECT_BORDERS    (childWindowToCopy),
    layout            (childWindowToCopy.layout),
    distanceToSide    (childWindowToCopy.distanceToSide),
    borderColor       (childWindowToCopy.borderColor),
    title             (childWindowToCopy.title),
    m_TitleBarHeight  (childWindowToCopy.m_TitleBarHeight),
    m_LoadedPathname  (childWindowToCopy.m_LoadedPathname),
    m_SplitImage      (childWindowToCopy.m_SplitImage),
    m_DraggingPosition(childWindowToCopy.m_DraggingPosition),
    m_Opacity         (childWindowToCopy.m_Opacity)
    {
        // Copy the textures
        if (TGUI_TextureManager.copyTexture(childWindowToCopy.m_TextureTitleBar_L, m_TextureTitleBar_L))   m_SpriteTitleBar_L.setTexture(*m_TextureTitleBar_L);
        if (TGUI_TextureManager.copyTexture(childWindowToCopy.m_TextureTitleBar_M, m_TextureTitleBar_M))   m_SpriteTitleBar_M.setTexture(*m_TextureTitleBar_M);
        if (TGUI_TextureManager.copyTexture(childWindowToCopy.m_TextureTitleBar_R, m_TextureTitleBar_R))   m_SpriteTitleBar_R.setTexture(*m_TextureTitleBar_R);

        // Copy the button
        m_CloseButton = new tgui::Button(*childWindowToCopy.m_CloseButton);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChildWindow::~ChildWindow()
    {
        if (m_TextureTitleBar_L != NULL)   TGUI_TextureManager.removeTexture(m_TextureTitleBar_L);
        if (m_TextureTitleBar_M != NULL)   TGUI_TextureManager.removeTexture(m_TextureTitleBar_M);
        if (m_TextureTitleBar_R != NULL)   TGUI_TextureManager.removeTexture(m_TextureTitleBar_R);

        delete m_CloseButton;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChildWindow* ChildWindow::clone()
    {
        return new ChildWindow(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChildWindow& ChildWindow::operator= (const ChildWindow& right)
    {
        // Make sure it is not the same object
        if (this != &right)
        {
            ChildWindow temp(right);
            this->Panel::operator=(right);
            this->OBJECT_BORDERS::operator=(right);

            // Delete the old close button
            delete m_CloseButton;

            std::swap(layout,              temp.layout);
            std::swap(distanceToSide,      temp.distanceToSide);
            std::swap(borderColor,         temp.borderColor);
            std::swap(title,               temp.title);
            std::swap(m_TitleBarHeight,    temp.m_TitleBarHeight);
            std::swap(m_LoadedPathname,    temp.m_LoadedPathname);
            std::swap(m_SplitImage,        temp.m_SplitImage);
            std::swap(m_DraggingPosition,  temp.m_DraggingPosition);
            std::swap(m_Opacity,           temp.m_Opacity);
            std::swap(m_TextureTitleBar_L, temp.m_TextureTitleBar_L);
            std::swap(m_TextureTitleBar_M, temp.m_TextureTitleBar_M);
            std::swap(m_TextureTitleBar_R, temp.m_TextureTitleBar_R);
            std::swap(m_SpriteTitleBar_L,  temp.m_SpriteTitleBar_L);
            std::swap(m_SpriteTitleBar_M,  temp.m_SpriteTitleBar_M);
            std::swap(m_SpriteTitleBar_R,  temp.m_SpriteTitleBar_R);
            std::swap(m_CloseButton,       temp.m_CloseButton);
        }

        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ChildWindow::load(unsigned int width, unsigned int height, const sf::Color& bkgColor, const std::string pathname)
    {
        // Until the loading succeeds, the child window will be marked as unloaded
        m_Loaded = false;

        // Make sure that the pathname isn't empty
        if (pathname.empty())
            return false;

        // Set the background color of the child window
        backgroundColor = bkgColor;

        // Store the filename
        m_LoadedPathname = pathname;
        m_LoadedBackgroundImageFilename = "";

        // Set the size of the child window
        m_Size.x = width;
        m_Size.y = height;

        // When the pathname does not end with a "/" then we will add it
        if (m_LoadedPathname[m_LoadedPathname.length()-1] != '/')
            m_LoadedPathname.push_back('/');

        // Open the info file
        InfoFileParser infoFile;
        if (infoFile.openFile(m_LoadedPathname + "info.txt") == false)
        {
            TGUI_OUTPUT((((std::string("TGUI: Failed to open ")).append(m_LoadedPathname)).append("info.txt")).c_str());
            return false;
        }

        std::string property;
        std::string value;

        // Set some default settings
        m_SplitImage = false;
        layout = LayoutRight;
        distanceToSide = 5;
        std::string imageExtension = "png";

        // Read untill the end of the file
        while (infoFile.readProperty(property, value))
        {
            // Check what the property is
            if (property.compare("splitimage") == 0)
            {
                // Find out if it is split or not
                if (value.compare("true") == 0)
                    m_SplitImage = true;
                else if (value.compare("false") == 0)
                    m_SplitImage = false;
            }
            else if (property.compare("phases") == 0)
            {
                // Get and store the different phases
                extractPhases(value);
            }
            else if (property.compare("extension") == 0)
            {
                imageExtension = value;
            }
            else if (property.compare("borders") == 0)
            {
                // Get the borders
                Vector4u borders;
                if (extractVector4u(value, borders))
                    setBorders(borders.x1, borders.x2, borders.x3, borders.x4);
            }
            else if (property.compare("backgroundimage") == 0)
            {
                // The value should contain at least 2 characters
                if (value.size() > 1)
                {
                    // The value must start and end with a quote
                    if ((value[0] == '"') && (value[value.length()-1] == '"'))
                    {
                        // Remove the quotes
                        value = value.substr(1, value.length()-2);

                        // Remember the filename
                        m_LoadedBackgroundImageFilename = value;

                        // Check if a filename was passed
                        if (value.empty() == false)
                        {
                            TGUI_OUTPUT("TGUI: FIXME: Spaces are removed from pathname and all characters were converted to lowercase.");

                            // Try to load the texture from the file
                            if (TGUI_TextureManager.getTexture(m_LoadedPathname + value, m_Texture))
                            {
                                // Set the texture for the sprite
                                m_Sprite.setTexture(*m_Texture, true);

                                // Set the size of the sprite
                                m_Sprite.setScale(static_cast<float>(width) / m_Texture->getSize().x, static_cast<float>(height) / m_Texture->getSize().y);
                            }
                            else // The texture was not loaded
                                return false;
                        }
                    }
                }
            }
            else if (property.compare("layout") == 0)
            {
                if (value.compare("left") == 0)
                    layout = LayoutLeft;
                else if (value.compare("right") == 0)
                    layout = LayoutRight;
            }
            else if (property.compare("distancetoside") == 0)
            {
                distanceToSide = atoi(value.c_str());
            }
        }

        // Close the info file
        infoFile.closeFile();

        // Remove the textures when they were loaded before
        if (m_TextureTitleBar_L != NULL)   TGUI_TextureManager.removeTexture(m_TextureTitleBar_L);
        if (m_TextureTitleBar_M != NULL)   TGUI_TextureManager.removeTexture(m_TextureTitleBar_M);
        if (m_TextureTitleBar_R != NULL)   TGUI_TextureManager.removeTexture(m_TextureTitleBar_R);

        bool error = false;

        // Check if the title bar image is split
        if (m_SplitImage)
        {
            TGUI_OUTPUT("TGUI: FIXME: SplitImage is not supported yet.");
            return false;
        }
        else // The title bar image isn't split
        {
            // Load the required texture
            if ((TGUI_TextureManager.getTexture(m_LoadedPathname + "TitleBar." + imageExtension, m_TextureTitleBar_M)))
            {
                 m_SpriteTitleBar_M.setTexture(*m_TextureTitleBar_M, true);
                 m_TitleBarHeight = m_TextureTitleBar_M->getSize().y;
            }
            else
                return false;

            // Load the close button
            if (m_CloseButton->load(m_LoadedPathname + "/Close") == false)
                return false;
        }

        // When there is no error we will return true
        m_Loaded = !error;
        return !error;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string ChildWindow::getLoadedPathname()
    {
        return m_LoadedPathname;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setTitlebarHeight(unsigned int height)
    {
        // Don't continue when the child window has not been loaded yet
        if (m_Loaded == false)
            return;

        // Remember the new title bar height
        m_TitleBarHeight = height;

        // Set the scale of the close button
        m_CloseButton->setScale(static_cast<float>(height) / m_TextureTitleBar_M->getSize().y, static_cast<float>(height) / m_TextureTitleBar_M->getSize().y);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int ChildWindow::getTitleBarHeight() const
    {
        return m_TitleBarHeight;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setTransparency(unsigned char transparency)
    {
        // Store the new transparency
        m_Opacity = transparency;

        m_SpriteTitleBar_L.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_SpriteTitleBar_M.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_SpriteTitleBar_R.setColor(sf::Color(255, 255, 255, m_Opacity));

        m_CloseButton->m_SpriteNormal_L.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_CloseButton->m_SpriteNormal_M.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_CloseButton->m_SpriteNormal_R.setColor(sf::Color(255, 255, 255, m_Opacity));

        m_CloseButton->m_SpriteMouseHover_L.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_CloseButton->m_SpriteMouseHover_M.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_CloseButton->m_SpriteMouseHover_R.setColor(sf::Color(255, 255, 255, m_Opacity));

        m_CloseButton->m_SpriteMouseDown_L.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_CloseButton->m_SpriteMouseDown_M.setColor(sf::Color(255, 255, 255, m_Opacity));
        m_CloseButton->m_SpriteMouseDown_R.setColor(sf::Color(255, 255, 255, m_Opacity));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned char ChildWindow::getTransparency() const
    {
        return m_Opacity;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::handleEvent(sf::Event& event, const float mouseX, const float mouseY)
    {
        // Don't continue when the child window has not been loaded yet
        if (m_Loaded == false)
            return;

        // Check if the mouse on top of the close button
        if (event.type == sf::Event::MouseMoved)
        {
            // Check if you are dragging the child window
            if (m_MouseDown == true)
            {
                // Move the child window
                Vector2f position = getPosition();
                setPosition(position.x + (mouseX - position.x - m_DraggingPosition.x), position.y + (mouseY - position.y - m_DraggingPosition.y));
            }
            else // The child window is not being dragged
            {
                // Check if the mouse is on top of the title bar
                if (getTransform().transformRect(sf::FloatRect(0, 0, static_cast<float>(getSize().x), static_cast<float>(m_TitleBarHeight))).contains(mouseX, mouseY))
                {
                    // Temporary set the close button to the correct position
                    if (layout == LayoutRight)
                        m_CloseButton->setPosition(getPosition().x + m_Size.x - distanceToSide - m_CloseButton->getScaledSize().x, getPosition().y + (m_TitleBarHeight / 2.f) - (m_CloseButton->getScaledSize().x / 2.f));
                    else
                        m_CloseButton->setPosition(getPosition().x + distanceToSide, getPosition().y + (m_TitleBarHeight / 2.f) - (m_CloseButton->getScaledSize().x / 2.f));

                    // Send the hover event to the close button
                    if (m_CloseButton->mouseOnObject(mouseX, mouseY))
                        m_CloseButton->mouseMoved(mouseX, mouseY);

                    // Reset the position of the button
                    m_CloseButton->setPosition(0, 0);

                    // The mouse is on top of the title bar, so the objects don't need to know abot it
                    return;
                }
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            // Check if the mouse is on top of the title bar
            if (getTransform().transformRect(sf::FloatRect(0, 0, static_cast<float>(getSize().x), static_cast<float>(m_TitleBarHeight))).contains(mouseX, mouseY))
            {
                // Get the current position
                Vector2f position = getPosition();

                // Temporary set the close button to the correct position
                if (layout == LayoutRight)
                    m_CloseButton->setPosition(position.x + m_Size.x - distanceToSide - m_CloseButton->getScaledSize().x, position.y + (m_TitleBarHeight / 2.f) - (m_CloseButton->getScaledSize().x / 2.f));
                else
                    m_CloseButton->setPosition(position.x + distanceToSide, position.y + (m_TitleBarHeight / 2.f) - (m_CloseButton->getScaledSize().x / 2.f));

                // Send the mouse press event to the close button
                if (m_CloseButton->mouseOnObject(mouseX, mouseY))
                    m_CloseButton->leftMousePressed(mouseX, mouseY);
                else
                {
                    // The mouse went down on the title bar
                    m_MouseDown = true;

                    // Remember where we are dragging the title bar
                    m_DraggingPosition.x = mouseX - position.x;
                    m_DraggingPosition.y = mouseY - position.y;
                }

                // Reset the position of the button
                m_CloseButton->setPosition(0, 0);

                // The mouse is on top of the title bar, so the objects don't need to know abot it
                return;
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            // The mouse is no longer down
            m_MouseDown = false;

            // Check if the mouse is on top of the title bar
            if (getTransform().transformRect(sf::FloatRect(0, 0, static_cast<float>(getSize().x), static_cast<float>(m_TitleBarHeight))).contains(mouseX, mouseY))
            {
                // Temporary set the close button to the correct position
                if (layout == LayoutRight)
                    m_CloseButton->setPosition(getPosition().x + m_Size.x - distanceToSide - m_CloseButton->getScaledSize().x, getPosition().y + (m_TitleBarHeight / 2.f) - (m_CloseButton->getScaledSize().x / 2.f));
                else
                    m_CloseButton->setPosition(getPosition().x + distanceToSide, getPosition().y + (m_TitleBarHeight / 2.f) - (m_CloseButton->getScaledSize().x / 2.f));

                // Send the mouse release event to the close button
                if (m_CloseButton->mouseOnObject(mouseX, mouseY))
                {
                    // Check if the close button was clicked
                    if (m_CloseButton->m_MouseDown == true)
                    {
                        // If a callback was requested then send it
                        if (callbackID > 0)
                        {
                            Callback callback;
                            callback.trigger = Callback::closed;
                            m_Parent->addCallback(callback);
                        }

                        // Remove the objects in the child window
                        removeAllObjects();

                        // Remove the child window itself
                        m_Parent->remove(this);

                        // Get out of here
                        return;
                    }
                }

                // Reset the position of the button
                m_CloseButton->setPosition(0, 0);

                // The mouse is on top of the title bar, so the objects don't need to know abot it
                return;
            }
        }

        // Let the child window handle the rest
        Panel::handleEvent(event, mouseX, mouseY - m_TitleBarHeight);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ChildWindow::mouseOnObject(float x, float y)
    {
        // Don't continue when the child window has not been loaded yet
        if (m_Loaded == false)
            return false;

        // Check if the mouse is on top of the title bar
        if (getTransform().transformRect(sf::FloatRect(0, 0, static_cast<float>(getSize().x), static_cast<float>(m_TitleBarHeight))).contains(x, y))
        {
            m_EventManager.mouseNotOnObject();
            return true;
        }
        else
        {
            // Check if the mouse is on top of the window
            return Panel::mouseOnObject(x, y - m_TitleBarHeight);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setBorders(unsigned int leftBorder, unsigned int topBorder, unsigned int rightBorder, unsigned int bottomBorder)
    {
        // Set the new border size
        m_LeftBorder   = leftBorder;
        m_TopBorder    = topBorder;
        m_RightBorder  = rightBorder;
        m_BottomBorder = bottomBorder;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // Don't draw when the child window wasn't created
        if (m_Loaded == false)
            return;

        // Adjust the transformation
        states.transform *= getTransform();

        sf::Transform oldTransform = states.transform;

        // Check if the title bar image is split
        if (m_SplitImage)
        {
            // Split image is not supported yet
            return;
        }
        else // The title bar image isn't split
        {
            // Scale the title bar
            states.transform.scale(static_cast<float>(m_Size.x) / m_TextureTitleBar_M->getSize().x, static_cast<float>(m_TitleBarHeight) / m_TextureTitleBar_M->getSize().y);

            // Draw the title bar
            target.draw(m_SpriteTitleBar_M, states);

            // Undo the scaling
            states.transform.scale(static_cast<float>(m_TextureTitleBar_M->getSize().x) / m_Size.x, static_cast<float>(m_TextureTitleBar_M->getSize().y) / m_TitleBarHeight);
        }

        // Calculate the scale factor of the view
        float scaleViewX = target.getSize().x / target.getView().getSize().x;
        float scaleViewY = target.getSize().y / target.getView().getSize().y;

        // Get the global translation
        sf::Vector2f globalTranslation = states.transform.transformPoint(0, 0);

        // Get the old clipping area
        GLint scissor[4];
        glGetIntegerv(GL_SCISSOR_BOX, scissor);

        // Check if there is a title
        if (title.empty() == false)
        {
            // Create the text object
            sf::Text text(title);
            text.setCharacterSize(m_TitleBarHeight * 8 / 10);

            // Check the layout
            if (layout == LayoutRight)
            {
                // Set the clipping area
                glScissor((globalTranslation.x + distanceToSide - target.getView().getCenter().x + (target.getView().getSize().x / 2.f)) * scaleViewX,
                          target.getSize().y - ((globalTranslation.y + (m_TitleBarHeight - target.getView().getCenter().y + (target.getView().getSize().y / 2.f))) * scaleViewY),
                          (m_Size.x - (3*distanceToSide) - m_CloseButton->getScaledSize().x) * scaleViewX, m_TitleBarHeight * scaleViewY);

                // Draw the text
                states.transform.translate(static_cast<float>(distanceToSide), 0);
                target.draw(text, states);
                states.transform.translate(-static_cast<float>(distanceToSide), 0);
            }
            else // if (layout == LayoutLeft)
            {
                // Set the clipping area
                glScissor((globalTranslation.x + (2*distanceToSide) + m_CloseButton->getScaledSize().x - target.getView().getCenter().x + (target.getView().getSize().x / 2.f)) * scaleViewX,
                          target.getSize().y - ((globalTranslation.y + (m_TitleBarHeight - target.getView().getCenter().y + (target.getView().getSize().y / 2.f))) * scaleViewY),
                          (m_Size.x - (3*distanceToSide) - m_CloseButton->getScaledSize().x) * scaleViewX, m_TitleBarHeight * scaleViewY);

                // Draw the text
                states.transform.translate(m_CloseButton->getScaledSize().x + (2*distanceToSide), 0);
                target.draw(text, states);
                states.transform.translate(-m_CloseButton->getScaledSize().x - (2*distanceToSide), 0);
            }

            // Reset the old clipping area
            glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
        }

        // Move the close button to the correct position
        if (layout == LayoutRight)
            states.transform.translate(m_Size.x - distanceToSide - m_CloseButton->getScaledSize().x, (m_TitleBarHeight / 2.f) - (m_CloseButton->getScaledSize().x / 2.f));
        else //if (layout == LayoutLeft)
            states.transform.translate(static_cast<float>(distanceToSide), (m_TitleBarHeight / 2.f) - (m_CloseButton->getScaledSize().x / 2.f));

        // Draw the close button
        target.draw(*m_CloseButton, states);

        // Set the correct transformation
        states.transform = oldTransform.translate(0, m_TitleBarHeight);

        // Draw the borders
        sf::RectangleShape borders(Vector2f(m_Size.x, m_Size.y));
        borders.setFillColor(borderColor);
        target.draw(borders, states);

        // Make room for the borders
        states.transform.translate(m_LeftBorder, m_TopBorder);

        // Draw the background
        sf::RectangleShape background(Vector2f(m_Size.x - m_LeftBorder - m_RightBorder, m_Size.y - m_TopBorder - m_BottomBorder));
        background.setFillColor(backgroundColor);
        target.draw(background, states);

        // Set the clipping area
        glScissor((globalTranslation.x + m_LeftBorder - target.getView().getCenter().x + (target.getView().getSize().x / 2.f)) * scaleViewX,
                  target.getSize().y - ((globalTranslation.y + m_TitleBarHeight + m_TopBorder + (m_Size.y - m_TopBorder - m_BottomBorder - target.getView().getCenter().y + (target.getView().getSize().y / 2.f))) * scaleViewY),
                  (m_Size.x - m_LeftBorder - m_RightBorder) * scaleViewX, (m_Size.y - m_TopBorder - m_BottomBorder) * scaleViewY);

        // Draw the objects in the child window
        drawObjectGroup(&target, states);

        // Reset the old clipping area
        glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

