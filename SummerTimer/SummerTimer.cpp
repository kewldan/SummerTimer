#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <Windows.h>
#include "Arial.h"
#include "icon.h"

const int WIDTH = 1280;
const int HEIGHT = 720;
const uint64_t summer = 1654030800000;
const uint64_t autumn = 1630443600000;

using namespace sf;

struct Row {
    Text title, current;
    RectangleShape rect, back;
    uint32_t dimension = 0;
};

Font font;
struct Row rows[7];

Text getText(String cons, Vector2f position) {
    Text text;
    text.setFont(font);

    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setString(cons);
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, 0);
    text.setPosition(position);
    return text;
}

int main()
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    RenderWindow window(VideoMode(WIDTH, HEIGHT), L"Таймер");
    window.setVerticalSyncEnabled(true);
    Image icon;
    icon.loadFromMemory(Icon, 2558);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    if (!font.loadFromMemory(ArialMT, 710044)) {
        printf("Cannot load font");
    }
    Text title = getText(L"До лета осталось:", Vector2f(WIDTH / 2, 50));

    for (int i = 0; i < sizeof(rows) / sizeof(*rows); i++) {
        rows[i].title.setFont(font);
        rows[i].title.setFillColor(Color::Black);
        rows[i].title.setCharacterSize(24);
        rows[i].title.setPosition(10, i * 60 + 150);

        rows[i].rect.setPosition(250, i * 60 + 150);
        rows[i].rect.setFillColor(Color(131, 131, 131));

        rows[i].back.setPosition(250, i * 60 + 150);
        rows[i].back.setFillColor(Color(211, 211, 211));
        rows[i].back.setSize(Vector2f(1000, 30));

        rows[i].current.setFont(font);
        rows[i].current.setFillColor(Color::Black);
        rows[i].current.setCharacterSize(24);
        FloatRect textRect = rows[i].current.getLocalBounds();
        rows[i].current.setOrigin(textRect.left + textRect.width / 2.0f, 0);
        rows[i].current.setPosition(700, i * 60 + 150);
    }

    rows[0].title.setString(L"Осталось секунд:");
    rows[0].dimension = 1000U;
    rows[1].title.setString(L"Осталось минут:");
    rows[1].dimension = 60000U;
    rows[2].title.setString(L"Осталось часов:");
    rows[2].dimension = 3600000U;
    rows[3].title.setString(L"Осталось дней:");
    rows[3].dimension = 86400000U;
    rows[4].title.setString(L"Осталось недель:");
    rows[4].dimension = 604800000U;
    rows[5].title.setString(L"Осталось месяцев:");
    rows[5].dimension = 2592000000U;

    rows[6].title.setString(L"Пройдено:");
    rows[6].back.setFillColor(Color(133, 141, 250));
    rows[6].rect.setFillColor(Color(124, 226, 128));

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        window.clear(Color(230, 230, 230));


        unsigned __int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        uint64_t s = summer - now;

        window.draw(title);

        for (int i = 0; i < sizeof(rows) / sizeof(*rows) - 1; i++) {
            rows[i].current.setString(std::to_string(s / rows[i].dimension));
            rows[i].rect.setSize(Vector2f((s % rows[i].dimension) * (1000.0 / rows[i].dimension), 30));

            window.draw(rows[i].title);
            window.draw(rows[i].back);
            window.draw(rows[i].rect);
            window.draw(rows[i].current);
        }

        double percent = (now - autumn) / ((double)summer - autumn) * 100.0;
        rows[6].current.setString(std::to_string(percent) + "%");
        rows[6].rect.setSize(Vector2f(percent*10,30));

        window.draw(rows[6].title);
        window.draw(rows[6].back);
        window.draw(rows[6].rect);
        window.draw(rows[6].current);


        window.display();
    }

    return 0;
}