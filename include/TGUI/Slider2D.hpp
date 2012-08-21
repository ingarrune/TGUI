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

// Special thanks to Dmitry for the Slider2D object !


#ifndef _TGUI_SLIDER2D_INCLUDED_
#define _TGUI_SLIDER2D_INCLUDED_

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct Slider2D : public OBJECT
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Default constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Slider2D();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Copy constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Slider2D(const Slider2D& copy);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Destructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual ~Slider2D();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Overload of assignment operator
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Slider2D& operator= (const Slider2D& right);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Slider2D* clone();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Loads the slider images.
        ///
        /// The \a verticalScroll member might be changed in this function. If you want to change it then do it afterwards.
        ///
        /// \param pathname  The path to the folder that contains the images.
        ///                  The folder must also contain an info.txt file, which will give more information about the slider.
        ///
        /// \return
        ///        - true on success
        ///        - false when the pathname is empty
        ///        - false when the info.txt file was not found
        ///        - false when the images couldn't be loaded
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual bool load(const std::string pathname);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the size of the slider.
        ///
        /// This function will undo all scaling, as it just calculates the correct scale factors for you.
        ///
        /// \param width   The new width of the slider
        /// \param height  The new height of the slider
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setSize(float width, float height);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the size of the slider, unaffected by scaling.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Vector2u getSize() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the size of the slider, after the scaling transformation.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Vector2f getScaledSize() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the pathname that was used to load the slider.
        ///
        /// When the slider has not been loaded yet then this function will return an empty string.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual std::string getLoadedPathname();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Sets a minimum value.
        ///
        /// When the value is too small then it will be changed to this minimum.
        /// The default minimum value is (-1, -1).
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setMinimum(Vector2f minimum);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Sets a maximum value.
        ///
        /// When the value is too big then it will be changed to this maximum.
        /// The default maximum value is (1,1).
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setMaximum(Vector2f maximum);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Changes the current value.
        ///
        /// It can't be smaller than the minimum or bigger than the maximum.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setValue(Vector2f value);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the minimum value.
        ///
        /// The default minimum value -1.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Vector2f getMinimum();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the maximum value.
        ///
        /// The default maximum value 1.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Vector2f getMaximum();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns the current value.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Vector2f getValue();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Places the thumb back in the center.
        ///
        /// When \a returnToCenter is true then this will happen automatically when the mouse is released.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void centerThumb();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // These functions are used to receive callback from the EventManager.
        // You normally don't need them, but you can use them to simulate an event.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual bool mouseOnObject(float x, float y);
        virtual void leftMousePressed(float x, float y);
        virtual void leftMouseReleased(float x, float y);
        virtual void mouseMoved(float x, float y);
        virtual void objectFocused();
        virtual void mouseNoLongerDown();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Because this struct is derived from sf::Drawable, you can just call the draw function from your sf::RenderTarget.
        // This function will be called and it will draw the slider on the render target.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:

        /// Should we return thumb to center after mouse up?
        /// This bool is true by default.
        bool returnToCenter;

        /// Should the thumb be scaled together with the slider or not?
        /// This bool is true by default, which means that the thumb always has the exact size as the loaded image.
        bool fixedThumbSize;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        Vector2f m_Minimum;
        Vector2f m_Maximum;
        Vector2f m_Value;

        sf::Texture* m_TextureThumbNormal;
        sf::Texture* m_TextureThumbHover;
        sf::Texture* m_TextureTrackNormal;
        sf::Texture* m_TextureTrackHover;

        sf::Sprite   m_SpriteThumbNormal;
        sf::Sprite   m_SpriteThumbHover;
        sf::Sprite   m_SpriteTrackNormal;
        sf::Sprite   m_SpriteTrackHover;

        // The pathname used to load the slider
        std::string m_LoadedPathname;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //_TGUI_SLIDER2D_INCLUDED_
