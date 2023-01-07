#pragma once

#include <QDialog>

class QLineEdit;

class BcResizerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BcResizerDialog(QWidget* parent = nullptr);
    static QList<double> getDoubles(QWidget* parent, bool* ok = nullptr);

private:
    QList<QLineEdit*> fields;
    void accept() override;
};
