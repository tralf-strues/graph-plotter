//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file text_label.h
//! @date 2021-10-06
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef TEXT_LABEL_H
#define TEXT_LABEL_H

#include "gui_component.h"

class GUI_TextLabel : public GUI_Component
{
public:
    GUI_TextLabel(Renderer& renderer, const Vec2<int32_t>& pos, const Font& font, Color color);

    const Text& getText() const;

    void updateText(const char* str);

    /* GUI_Component */
    void render() override;

private:
    Text  m_Text;
    Font  m_Font;
    Color m_Color;
};

#endif // TEXT_LABEL_H