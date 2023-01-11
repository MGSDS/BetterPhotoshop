#include "ResizeScaleDialog.hpp"
#include <QDialogButtonBox>
#include <QDoubleValidator>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <limits.h>

ResizeScaleDialog::ResizeScaleDialog(QWidget* parent)
    : QDialog(parent)
{
    QGridLayout* gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel("Scale X"), 0, 0);
    auto* scaleX = new QLineEdit();
    scaleX->setText("1");
    scaleX->setValidator(new QDoubleValidator(0.01, 10, 2, this));
    gridLayout->addWidget(scaleX, 0, 1);
    fields.append(scaleX);

    gridLayout->addWidget(new QLabel("Scale Y"), 1, 0);
    auto* scaleY = new QLineEdit();
    scaleY->setText("1");
    scaleY->setValidator(new QDoubleValidator(0.01, 10, 2, this));
    gridLayout->addWidget(scaleY, 1, 1);
    fields.append(scaleY);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                           Qt::Horizontal, this);

    gridLayout->addWidget(buttonBox, 6, 0, 1, 2);

    bool conn = connect(buttonBox, &QDialogButtonBox::accepted,
                        this, &ResizeScaleDialog::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox, &QDialogButtonBox::rejected,
                   this, &ResizeScaleDialog::reject);
    Q_ASSERT(conn);

    setLayout(gridLayout);
}

void ResizeScaleDialog::accept()
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

QList<double> ResizeScaleDialog::getDoubles(QWidget* parent, bool* ok)
{
    auto dialog = std::make_unique<ResizeScaleDialog>(parent);
    *ok = dialog->exec() == QDialog::Accepted;

    QList<double> result;

    for (auto field : dialog->fields) {
        result.append(field->text().toDouble());
    }

    return result;
}
