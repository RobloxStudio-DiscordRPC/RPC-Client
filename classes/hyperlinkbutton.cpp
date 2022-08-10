#include "hyperlinkbutton.h"

HyperlinkButton::HyperlinkButton(QWidget* parent): QPushButton(parent) {
    connect(
        this, &HyperlinkButton::clicked,
        this, [this](){openUrl(destination);}
    );
}

void HyperlinkButton::setDestination(QUrl url) {
    destination = url;
    emit destinationChanged();
}
