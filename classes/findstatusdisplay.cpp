#include "findstatusdisplay.h"

FindStatusDisplay::FindStatusDisplay(QWidget* parent): QWidget(parent) {
    layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    iconLabel = new QLabel(this);
    iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(iconLabel);

    label = new QLabel(this);
    layout->addWidget(label);

    spacer = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Fixed);
    layout->addSpacerItem(spacer);

    researchButton = new QPushButton("Search again", this);
    researchButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(
        researchButton, &QPushButton::pressed,
        this, [this](){emit research();}
    );
    layout->addWidget(researchButton);

    setFound(false);
}

void FindStatusDisplay::setFound(bool f) {
    found = f;
    emit searched();

    label->setText(found ? "found" : "not found");

    QPixmap icon(found ? tick : cross);
    iconLabel->setPixmap(icon);
}

FindStatusDisplay::~FindStatusDisplay() {
    safedelete(layout);
    safedelete(iconLabel);
    safedelete(label);
    safedelete(spacer);
    safedelete(researchButton);
}
