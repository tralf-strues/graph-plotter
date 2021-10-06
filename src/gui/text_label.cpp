//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file text_label.cpp
//! @date 2021-10-06
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "text_label.h"

GUI_TextLabel::GUI_TextLabel(const Vec2<int32_t>& pos, const Font& font, Color color)
    : GUI_Component(pos),
      m_Font(font),
      m_Color(color)
{
}

const Text& GUI_TextLabel::getText() const
{
    return m_Text;
}

void GUI_TextLabel::updateText(Renderer& renderer, const char* str)
{
    assert(str);

    m_Text.load(renderer, str, m_Font, m_Color);
}

void GUI_TextLabel::render(Renderer& renderer)
{
    m_Text.render(renderer, m_Pos);
}