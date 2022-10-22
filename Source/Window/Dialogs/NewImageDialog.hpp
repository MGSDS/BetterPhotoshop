#pragma once

#include <QDialog>
#include <QSpinBox>

class NewImageDialog : public QDialog
{
    Q_OBJECT

public:
    NewImageDialog(QWidget* parent = nullptr);
    ~NewImageDialog() override = default;

    int Exec() { return exec(); }

    int GetWidth() const { return m_WidthSpinBox->value(); }

    int GetHeight() const { return m_HeightSpinBox->value(); }

private:
    QSpinBox* m_WidthSpinBox = nullptr;
    QSpinBox* m_HeightSpinBox = nullptr;
};
