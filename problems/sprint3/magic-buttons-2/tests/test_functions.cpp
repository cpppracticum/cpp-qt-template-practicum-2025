#include "functions.h"


void OpenYandex() {
}

void OpenPracticum() {
}

void OpenHomeDirectory() {
}

void OpenCmd() {
}

QString custom_url {};
QString custom_dir {};

void OpenCustomUrl(const QString & url) {
    if (url != QString{})
    {
        custom_url = url;
    }
}

void OpenCustomDirectory(const QString & directory) {
    if (directory != QString{})
    {
        custom_dir = directory;
    }
}
