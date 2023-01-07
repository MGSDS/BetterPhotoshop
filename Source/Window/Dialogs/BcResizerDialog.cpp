#include "BcResizerDialog.hpp"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDoubleValidator>
#include <QLabel>
#include <QLineEdit>

BcResizerDialog::BcResizerDialog(QWidget* parent)
{
    QGridLayout* gridLayout = new QGridLayout(this);

    gridLayout->addWidget(new QLabel("B"), 0, 0);
    auto* a = new QLineEdit();
    a->setText("0");
    a->setValidator(new QDoubleValidator(0, 1, 2, this));
    gridLayout->addWidget(a, 0, 1);
    fields.append(a);

    gridLayout->addWidget(new QLabel("C"), 1, 0);
    auto* b = new QLineEdit();
    b->setText("0.5");
    b->setValidator(new QDoubleValidator(0, 1, 2, this));
    gridLayout->addWidget(b, 1, 1);
    fields.append(b);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                           Qt::Horizontal, this);

    gridLayout->addWidget(buttonBox, 2, 0, 1, 2);

    bool conn = connect(buttonBox, &QDialogButtonBox::accepted,
                        this, &BcResizerDialog::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox, &QDialogButtonBox::rejected,
                   this, &BcResizerDialog::reject);
    Q_ASSERT(conn);
}

QList<double> BcResizerDialog::getDoubles(QWidget* parent, bool* ok)
{
    BcResizerDialog dialog(parent);
    if (dialog.exec() == QDialog::Accepted)
    {
        QList<double> result;
        for (auto* field : dialog.fields)
        {
            result.append(field->text().toDouble());
        }
        if (ok)
        {
            *ok = true;
        }
        return result;
    }

    if (ok)
    {
        *ok = false;
    }

    return QList<double>();
}

void BcResizerDialog::accept()
{
    for (auto* field : fields)
    {
        auto text = field->text();
        if (text.isEmpty())
        {
            return;
        }
    }
    QDialog::accept();
}
