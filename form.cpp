#include "includes.h"

void Form::draw( ) {
    constexpr float frequency = 2.f;
    const float step = frequency * g_csgo.m_globals->m_frametime;
    m_open ? m_opacity += step : m_opacity -= step;
    math::clamp( m_opacity, 0.f, 1.f );
    m_alpha = static_cast<int>(255 * m_opacity);
    if( !m_alpha ) return;

    Color accent = g_gui.m_color;
    accent.a() = m_alpha;
    render::rect_filled(m_x, m_y, m_width, m_height, theme::canvas(m_alpha));
    render::rect(m_x, m_y, m_width, m_height, theme::border(m_alpha));
    theme::header(m_x + 1, m_y + 1, m_width - 2, m_alpha);

    const Rect tabs = GetTabsRect();
    render::rect_filled(tabs.x, tabs.y, tabs.w, tabs.h, theme::panel(m_alpha));
    for( size_t i = 0; i < m_tabs.size(); ++i ) {
        const auto tab = m_tabs[i];
        const Rect row = GetTabRect(i);
        if( tab == m_active_tab ) {
            render::rect_filled(row.x, row.y, row.w, row.h, theme::field(m_alpha));
            render::rect_filled(row.x, row.y + 3, 3, row.h - 6, accent);
        }
        render::menu_shade.string(row.x + 12, row.y + 8,
            tab == m_active_tab ? theme::ink(m_alpha) : theme::muted(m_alpha), scots::display(tab->m_title));
    }
    theme::rose(tabs.x + tabs.w / 2, tabs.y + tabs.h - 59, m_alpha);
    render::menu.string(tabs.x + tabs.w / 2, tabs.y + tabs.h - 24,
        theme::muted(m_alpha), scots::display("STAY IN BALANCE"), render::ALIGN_CENTER);

    const Rect elements = GetElementsRect();
    render::rect_filled(elements.x, elements.y, elements.w, elements.h, theme::panel(m_alpha));
    render::rect(elements.x, elements.y, elements.w, elements.h, theme::border(m_alpha));
    if( m_active_tab ) {
        for( const auto element : m_active_tab->m_elements ) {
            if( !element || element == m_active_element || !element->m_show || !(element->m_flags & ElementFlags::DRAW) ) continue;
            element->draw();
        }
        if( m_active_element && m_active_element->m_show ) m_active_element->draw();
    }
    theme::accent_line(m_x + 16, m_y + m_height - 22, m_width - 32, m_alpha);
    render::menu.string(m_x + m_width / 2, m_y + m_height - 17,
        theme::muted(m_alpha), "COCAINE", render::ALIGN_CENTER);
}
