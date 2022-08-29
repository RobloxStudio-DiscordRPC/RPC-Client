#ifndef FINDSTATUSDISPLAY_H
#define FINDSTATUSDISPLAY_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#ifndef safedelete
#define safedelete(x) { \
    if ((x)!=NULL) { \
        delete (x); \
        (x)=NULL; \
    } \
}
#endif

class FindStatusDisplay : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool found MEMBER found WRITE setFound NOTIFY searched)

    public:
        FindStatusDisplay(QWidget* parent = nullptr);
        ~FindStatusDisplay();

        void setFound(bool f);
        bool found;

        signals:
            void research();
            void searched();

    private:
        const inline static QString tick  = ":/icons/tick.png" ;
        const inline static QString cross = ":/icons/cross.png";

        QHBoxLayout* layout;
        QLabel* iconLabel;
        QLabel* label;
        QSpacerItem* spacer;
        QPushButton* researchButton;
};

#endif // FINDSTATUSDISPLAY_H
