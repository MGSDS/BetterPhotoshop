#pragma once

#include <QDialog>

class QLineEdit;
class QLabel;

class ResizeScaleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ResizeScaleDialog(QWidget* parent = nullptr);
    static QList<double> getDoubles(QWidget* parent, bool* ok = nullptr);

private:
    QList<QLineEdit*> fields;
    void accept() override;
};