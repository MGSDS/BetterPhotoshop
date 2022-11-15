#pragma once

#include <QDialog>

class QLineEdit;
class QLabel;

class LineDialog : public QDialog
{
Q_OBJECT
public:
    explicit LineDialog(QWidget *parent = nullptr);
    static QList<int> getInts(QWidget *parent, bool *ok = nullptr);

private:
    QList<QLineEdit*> fields;
    void accept() override;
};