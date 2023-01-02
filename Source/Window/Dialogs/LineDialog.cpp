#include "LineDialog.hpp"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <limits.h>

LineDialog::LineDialog(QWidget* parent)
    : QDialog(parent)
{
    QGridLayout* gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel("Line thickness"), 0, 0);
    auto* thickness = new QLineEdit();
    thickness->setText("5");
    thickness->setValidator(new QIntValidator(1, INT_MAX, this));
    gridLayout->addWidget(thickness, 0, 1);
    fields.append(thickness);

    gridLayout->addWidget(new QLabel("1(R)"), 1, 0);
    auto* red = new QLineEdit();
    red->setText("0");
    red->setValidator(new QIntValidator(0, 255, this));
    gridLayout->addWidget(red, 1, 1);
    fields.append(red);

    gridLayout->addWidget(new QLabel("2(G)"), 2, 0);
    auto* green = new QLineEdit();
    green->setText("0");
    green->setValidator(new QIntValidator(0, 255, this));
    gridLayout->addWidget(green, 2, 1);
    fields.append(green);

    gridLayout->addWidget(new QLabel("3(B)"), 4, 0);
    auto* blue = new QLineEdit();
    blue->setText("0");
    blue->setValidator(new QIntValidator(0, 255, this));
    gridLayout->addWidget(blue, 4, 1);
    fields.append(blue);

    gridLayout->addWidget(new QLabel("Alpha"), 5, 0);
    auto* alpha = new QLineEdit();
    alpha->setText("255");
    alpha->setValidator(new QIntValidator(0, 255, this));
    gridLayout->addWidget(alpha, 5, 1);
    fields.append(alpha);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                           Qt::Horizontal, this);

    gridLayout->addWidget(buttonBox, 6, 0, 1, 2);

    bool conn = connect(buttonBox, &QDialogButtonBox::accepted,
                        this, &LineDialog::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox, &QDialogButtonBox::rejected,
                   this, &LineDialog::reject);
    Q_ASSERT(conn);

    setLayout(gridLayout);
}

QList<int> LineDialog::getInts(QWidget* parent, bool* ok)
{
    LineDialog* dialog = new LineDialog(parent);
    *ok = dialog->exec() == QDialog::Accepted;

    QList<int> result;

    for (auto field : dialog->fields) {
        result.append(field->text().toInt());
    }

    return result;
}

void LineDialog::accept()
{
    for (auto field : fields) {
        auto text = field->text();
        int pos = field->cursorPosition();
        if (field->validator()->validate(text, pos) != QValidator::Acceptable) {
            QDialog::reject();
            return;
        }
    }
    QDialog::accept();
}
