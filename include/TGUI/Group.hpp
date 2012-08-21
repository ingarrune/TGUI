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


#ifndef _TGUI_GROUP_INCLUDED_
#define _TGUI_GROUP_INCLUDED_

#include <list>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Parent struct for objects that store multiple objects.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct TGUI_API Group
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Default constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Group();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Copy constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Group(const Group& copy);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Destructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual ~Group();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Overload of assignment operator
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Group& operator= (const Group& right);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Creates and adds an object to the group.
        ///
        /// \param objectName  If you want to access the object later then you must do this with this name.
        ///
        /// Usage example:
        /// \code tgui::Picture* pic = group.add<tgui::Picture>("picName"); \endcode
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        T* add(const sf::String objectName = "")
        {
            T* newObject = new T();
            newObject->m_Parent = this;
            m_EventManager.m_Objects.push_back(newObject);
            m_ObjName.push_back(objectName);

            newObject->initialize();

            return newObject;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns a pointer to an earlier created object.
        ///
        /// \param objectName The name that was given to the object when it was added to the group.
        ///
        /// \warning Make sure that the name belongs to the object you are trying to get.
        ///         Behaviour is undefined when e.g. calling this function to get a picture but
        ///         when passing a name that was given to a button.
        ///
        /// \warning This function will return NULL when an unknown object name was passed.
        ///
        /// \warning Don't pass an empty string to this function. Objects with empty names are sometimes used internally.
        ///
        /// Usage example:
        /// \code tgui::Picture* pic = group.get<tgui::Picture>("picName"); \endcode
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        T* get(const sf::String objectName)
        {
            for (unsigned int i=0; i<m_ObjName.size(); ++i)
            {
                if (m_ObjName[i].toWideString().compare(objectName) == 0)
                    return static_cast<T*>(m_EventManager.m_Objects[i]);
            }

            return NULL;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Makes a copy of any existing object and returns the pointer to the new object.
        ///
        /// \param oldObject     A pointer to the old object.
        /// \param newObjectName If you want to access the object later then you must do this with this name.
        ///
        /// Usage example:
        /// \code tgui::Picture* pic = group.copy(pictureToCopy, "NameOfNewPic"); \endcode
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        T* copy(T* oldObject, const sf::String newObjectName = "")
        {
            T* newObject = new T(*oldObject);

            m_EventManager.m_Objects.push_back(newObject);
            m_ObjName.push_back(newObjectName);

            return newObject;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Makes a copy of any existing object and returns the pointer to the new object.
        ///
        /// \param oldObjectName The name that was given to the object when it was added to the group.
        /// \param newObjectName If you want to access the copied object later then you must do this with this name.
        ///
        /// \warning This function will return NULL when an unknown object name was passed.
        ///
        /// Usage example:
        /// \code tgui::Picture* pic = group.copy<tgui::Picture>("PicToCopy", "NameOfNewPic"); \endcode
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        T* copy(const sf::String oldObjectName, const sf::String newObjectName = "")
        {
            for (unsigned int i=0; i<m_ObjName.size(); ++i)
            {
                if (m_ObjName[i].toWideString().compare(oldObjectName) == 0)
                {
                    T* newObject = new T(*static_cast<T*>(m_EventManager.m_Objects[i]));

                    m_EventManager.m_Objects.push_back(newObject);
                    m_ObjName.push_back(newObjectName);

                    return newObject;
                }
            }

            return NULL;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Loads a form from a file.
        ///
        /// The objects will be loaded and added to the group.
        /// Note that even when this function fails, some objects might have been loaded already.
        ///
        /// \return
        ///        - true on success
        ///        - false when the file could not be opened
        ///        - false when the file contains a mistake
        ///        - false when one of the objects couldn't be loaded
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual bool loadObjectsFromFile(const std::string filename);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns a list of all the objects.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual std::vector<OBJECT*>& getObjects();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Returns a list of the names of all the objects.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual std::vector<sf::String>& getObjectNames();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Removes a single object that was added to the group.
        ///
        /// If there are multiple objects with the same name then only the first matching object will be removed.
        ///
        /// \see remove(OBJECT*)
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void remove(const sf::String objectName);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Removes a single object that was added to the group.
        ///
        /// \see remove(sf::String)
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void remove(OBJECT* object);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Removes all objects that were added to the group.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void removeAllObjects();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Focuses an object.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void focus(OBJECT* object);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Unfocuses an object.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void unfocus(OBJECT* object);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Uncheck all the radio buttons.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void uncheckRadioButtons();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Adds a callback to the list in tgui::Window.
        ///
        /// This callback can be obtained by calling the getCallback function of the window.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void addCallback(Callback& callback) = 0;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Update the internal clock to make animation possible.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void updateTime(const sf::Time& elapsedTime);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Places an object before all other objects.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void moveObjectToFront(OBJECT* object);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Places an object behind all other objects.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void moveObjectToBack(OBJECT* object);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // This function will call the draw function from all the objects in the correct order.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void drawObjectGroup(sf::RenderTarget* target, const sf::RenderStates& states = sf::RenderStates::Default) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:

        /// The internal font, used by all objects by default. If not changed then this is the default SFML font.
        sf::Font globalFont;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        std::vector<sf::String>  m_ObjName;

        // The internal event manager
        EventManager m_EventManager;

        // Is the group focused? If so, then one of the objects inside the group may be focused
        bool m_GroupFocused;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //_TGUI_GROUP_INCLUDED_
