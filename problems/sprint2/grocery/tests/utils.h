#ifndef UTILS_H
#define UTILS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLabel>
#include <QtTest>
#include "selector.h"
#include "additemsnotification.h"
#include "cartwindow.h"

#include <map>


template <class Widget>
Widget* FindElementByName(QWidget* window, const QString& name) {
    auto elements = window->findChildren<Widget*>();

    auto iter = std::find_if(elements.begin(), elements.end(), [&](Widget* elem) {
        return elem->objectName() == name;
    });
    return iter == elements.end() ? nullptr : *iter;
}

template <typename LayoutType>
std::map<void*, LayoutType*> get_layout_map(QWidget* parent) {
    auto layouts = parent->findChildren<LayoutType*>();

    std::map<void*, LayoutType*> layout_map;

    for (const auto& layout : layouts) {
        int item_count = layout->count();

        for (int i = 0; i < item_count; ++i) {
            auto object = layout->itemAt(i);

            auto widget = object->widget();
            if (widget) {
                layout_map[static_cast<void*>(widget)] = layout;
                continue;
            }

            auto child_layout = object->layout();
            if (child_layout) {
                layout_map[static_cast<void*>(child_layout)] = layout;
            }
        }
    }
    return layout_map;
}


void check_items_layout(Selector* window, const std::map<void*, QHBoxLayout*>& horizontal_map,
                        const std::map<void*, QGridLayout*>& grid_map);

void check_grid_layout(Selector* window, const std::map<void*, QVBoxLayout*>& vertical_map,
                       const std::map<void*, QHBoxLayout*>& horizontal_map);

void check_add_pbs(Selector* window, const std::map<void*, QVBoxLayout*>& vertical_map,
                   const std::map<void*, QHBoxLayout*>& horizontal_map);

void check_cart_buttons(Selector* window, const std::map<void*, QVBoxLayout*>& vertical_map,
                        const std::map<void*, QHBoxLayout*>& horizontal_map);
#endif  // UTILS_H
