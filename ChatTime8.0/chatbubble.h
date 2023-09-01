#ifndef CHATBUBBLE_H
#define CHATBUBBLE_H

#include <QApplication>
#include <QListWidget>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QTextOption>

class ChatBubbleDelegate : public QStyledItemDelegate {
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QString text = index.data(Qt::DisplayRole).toString();
        bool isSender = index.data(Qt::UserRole).toBool();

        painter->save();

        QPen pen;
        if (isSender) {
            pen.setColor(Qt::darkGreen);
            painter->setBrush(QColor("#DCF8C6"));
            painter->translate(option.rect.topRight());
        } else {
            pen.setColor(Qt::darkBlue);
            painter->setBrush(QColor("#DCE9FF"));
            painter->translate(option.rect.topLeft());
        }

        painter->setPen(pen);
        painter->setRenderHint(QPainter::Antialiasing);

        QRect bubbleRect(0, 0, option.rect.width() - 10, option.rect.height());
        painter->drawRoundedRect(bubbleRect, 10, 10);

        QTextOption textOption;
        textOption.setAlignment(isSender ? Qt::AlignRight : Qt::AlignLeft);
        painter->drawText(bubbleRect, text, textOption);

        painter->restore();
    }
};


#endif // CHATBUBBLE_H
