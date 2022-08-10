#ifndef HYPERLINKBUTTON_H
#define HYPERLINKBUTTON_H

#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>

class HyperlinkButton : public QPushButton
{
    Q_OBJECT

    Q_PROPERTY(
        QUrl destination
        WRITE setDestination
        NOTIFY destinationChanged
        MEMBER destination
    )

    public:
        HyperlinkButton(QWidget* parent = nullptr);

        void setDestination(QUrl url);
        QUrl destination;

        static inline void openUrl(const QUrl url) {
            QDesktopServices::openUrl(url);
        }

        signals:
            void destinationChanged();
};

#endif // HYPERLINKBUTTON_H
