#include <TGUI/HorizontalLayout.hpp>
#include <numeric>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    HorizontalLayout::HorizontalLayout()
    {
        m_callback.widgetType = "HorizontalLayout";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    HorizontalLayout::Ptr HorizontalLayout::copy(ConstPtr layout)
    {
        if (layout)
            return std::make_shared<HorizontalLayout>(*layout);
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void HorizontalLayout::updateWidgetPositions()
    {
        float sumRatio = 0;
        for (unsigned int i = 0; i < m_widgetsRatio.size(); ++i)
        {
            if (m_widgetsFixedSizes[i] == 0)
                sumRatio += m_widgetsRatio[i];
        }

        float currentRatio = 0;
        float currentOffset = 0;
        const float sumFixedSize = std::accumulate(m_widgetsFixedSizes.begin(), m_widgetsFixedSizes.end(), 0.f);
        for (unsigned int i = 0; i < m_layoutWidgets.size(); ++i)
        {
            m_layoutWidgets[i]->setPosition((m_size.x - sumFixedSize) * currentRatio + currentOffset, 0.f);
            if (m_widgetsFixedSizes[i])
            {
                m_layoutWidgets[i]->setSize(m_widgetsFixedSizes[i], m_size.y);
                currentOffset += m_widgetsFixedSizes[i];
            }
            else
            {
                m_layoutWidgets[i]->setSize((m_size.x - sumFixedSize) * m_widgetsRatio[i] / sumRatio, m_size.y);
                currentRatio += m_widgetsRatio[i] / sumRatio;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    tgui::Widget::Ptr HorizontalLayout::clone()
    {
        return std::make_shared<HorizontalLayout>(*this);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////