#include "HistogramWidget.h"
#include "Core/Image/Histogram/Histogram.hpp"
#include <QBarSeries>
#include <QChartView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QtCharts/QBarSet>

HistogramWidget::HistogramWidget(QWidget* parent)
{
    auto layout = std::make_unique<QVBoxLayout>(this);
    m_Splitter = std::make_unique<QSplitter>(Qt::Orientation::Vertical, this);
    m_Splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_RedChartView = std::make_unique<QChartView>(this);
    m_GreenChartView = std::make_unique<QChartView>(this);
    m_BlueChartView = std::make_unique<QChartView>(this);
    m_GrayscaleChartView = std::make_unique<QChartView>(this);

    m_Splitter->addWidget(m_RedChartView.get());
    m_Splitter->addWidget(m_GreenChartView.get());
    m_Splitter->addWidget(m_BlueChartView.get());
    m_Splitter->addWidget(m_GrayscaleChartView.get());
    layout->addWidget(m_Splitter.get());
    setLayout(layout.release());
}

void HistogramWidget::SetImage(const Image* image, ActiveChannel activeChannel)
{
    if (image == nullptr)
    {
        return;
    }

    std::vector<std::vector<int>> histograms = Histogram::Calculate(*image);

    SetCharts(histograms);

    if (image->IsGrayscale())
    {
        SetGrayScaleMode();
        return;
    }

    SetRgbMode();


    if (activeChannel == ActiveChannel::ZEROTH)
    {
        m_BlueChartView->hide();
        m_GreenChartView->hide();
    }
    else if (activeChannel == ActiveChannel::FIRST)
    {
        m_BlueChartView->hide();
        m_RedChartView->hide();
    }
    else if (activeChannel == ActiveChannel::SECOND)
    {
        m_GreenChartView->hide();
        m_RedChartView->hide();
    }

}

void HistogramWidget::SetChart(std::vector<int>& histogram, QChartView* chartView, QColor color, std::string name)
{
    auto barSet = std::make_unique<QBarSet>(name.c_str());
    barSet->setColor(color);
    barSet->setBorderColor(color);
    int max = *std::max_element(histogram.begin() + 1, histogram.end());
    for (int i = 1; i < histogram.size(); i++)
    {
        *barSet << histogram[i] / (float)max;
    }

    auto barSeries = std::make_unique<QBarSeries>();
    barSeries->append(barSet.release());

    auto chart = std::make_unique<QChart>();
    chart->legend()->hide();
    chart->addSeries(barSeries.release());
    chart->createDefaultAxes();
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundBrush(QBrush(QColor("transparent")));
    chart->axes(Qt::Horizontal).back()->setRange(0, 255);
    chart->axes(Qt::Vertical).back()->setRange(0, 1);
    chart->axes(Qt::Horizontal).back()->setGridLineVisible(false);
    chart->axes(Qt::Vertical).back()->setGridLineVisible(false);
    chartView->setChart(chart.release());
    chartView->setRenderHint(QPainter::Antialiasing);
}

void HistogramWidget::SetGreenChart(std::vector<int>& histogram)
{
    SetChart(histogram, m_GreenChartView.get(), QColor(0, 255, 0), "Green");
}

void HistogramWidget::SetRedChart(std::vector<int>& histogram)
{
    SetChart(histogram, m_RedChartView.get(), QColor(255, 0, 0), "Red");
}

void HistogramWidget::SetBlueChart(std::vector<int>& histogram)
{
    SetChart(histogram, m_BlueChartView.get(), QColor(0, 0, 255), "Blue");
}

void HistogramWidget::SetGrayscaleChart(std::vector<int>& histogram)
{
    SetChart(histogram, m_GrayscaleChartView.get(), QColor(255, 255, 255), "Grayscale");
}

void HistogramWidget::SetCharts(std::vector<std::vector<int>>& histograms)
{
    if (m_IsGrayScale)
    {
        SetGrayScaleMode();
    }
    else
    {
        SetRgbMode();
    }
    SetRedChart(histograms[0]);
    SetGreenChart(histograms[1]);
    SetBlueChart(histograms[2]);
    SetGrayscaleChart(histograms[3]);
}

void HistogramWidget::SetGrayScaleMode()
{
    m_IsGrayScale = true;
    m_GrayscaleChartView->show();
    m_RedChartView->hide();
    m_GreenChartView->hide();
    m_BlueChartView->hide();
}

void HistogramWidget::SetRgbMode()
{
    m_IsGrayScale = false;
    m_GrayscaleChartView->show();
    m_RedChartView->show();
    m_GreenChartView->show();
    m_BlueChartView->show();
}
