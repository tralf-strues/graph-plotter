//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file text_label.cpp
//! @date 2021-10-06
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "text_label.h"

GUI_TextLabel::GUI_TextLabel(Renderer& renderer, const Vec2<int32_t>& pos, const Font& font, Color color)
    : GUI_Component(renderer, pos),
      m_Font(font),
      m_Color(color)
{
}

const Text& GUI_TextLabel::getText() const
{
    return m_Text;
}

void GUI_TextLabel::updateText(const char* str)
{
    assert(str);

    m_Text.load(m_Renderer, str, m_Font, m_Color);
}

void GUI_TextLabel::render()
{
    m_Text.render(m_Renderer, m_Pos);
}