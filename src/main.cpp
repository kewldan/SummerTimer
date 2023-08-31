#include "SFML/Graphics.hpp"
#include <string>
#include <chrono>
#include <cmath>
#include <Windows.h>
#include "Arial.h"
#include "icon.h"

using namespace sf;
using namespace std;

int WIDTH = 1280;
int HEIGHT = 720;
const unsigned long long GOAL_TIME = 1714521600000ULL;
const unsigned long long START_TIME = 1693526400000ULL;

const Color BACKGROUND_COLOR(230, 230, 230);

const Color PROGRESS_BACKGROUND(211, 211, 211);
const Color PROGRESS_FOREGROUND(131, 131, 131);

const Color MAIN_PROGRESS_BACKGROUND(133, 141, 250);
const Color MAIN_PROGRESS_FOREGROUND(124, 226, 128);


struct Row {
    Text title, current;
    RectangleShape rect, back;
    uint32_t dimension = 0;
};

Font font;
struct Row rows[7];
Image icon;
Text title;

void centerizeText(Text &text, Vector2f position) {
    const FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, 0);
    text.setPosition(position);
}

void centerizeText(Text &text, Vector2f position, Vector2f size) {
    centerizeText(text, Vector2f(position.x + size.x / 2, position.y));
}

int main() {
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    RenderWindow window(VideoMode(WIDTH, HEIGHT), L"Таймер");
    window.setVerticalSyncEnabled(true);
    icon.loadFromMemory(Icon, 2558);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    font.loadFromMemory(ArialMT, 710044);
    title = Text(L"До лета осталось:", font);
    title.setFillColor(Color::Black);

    for (int i = 0; i < sizeof(rows) / sizeof(*rows); i++) {
        rows[i].title.setFont(font);
        rows[i].title.setFillColor(Color::Black);
        rows[i].title.setCharacterSize(24);
        rows[i].title.setPosition(10.f, i * 60.f + 150.f);

        rows[i].rect.setPosition(250.f, i * 60.f + 150.f);
        rows[i].rect.setFillColor(PROGRESS_FOREGROUND);

        rows[i].back.setPosition(250.f, i * 60.f + 150.f);
        rows[i].back.setFillColor(PROGRESS_BACKGROUND);

        rows[i].current.setFont(font);
        rows[i].current.setFillColor(Color::Black);
        rows[i].current.setCharacterSize(24);
        rows[i].current.setPosition(750.f, i * 60.f + 150.f);
    }

    rows[5].title.setString(L"Осталось секунд:");
    rows[5].dimension = 1000U;
    rows[4].title.setString(L"Осталось минут:");
    rows[4].dimension = 1000U * 60U;
    rows[3].title.setString(L"Осталось часов:");
    rows[3].dimension = 1000U * 60U * 60U;
    rows[2].title.setString(L"Осталось дней:");
    rows[2].dimension = 1000U * 60U * 60U * 24U;
    rows[1].title.setString(L"Осталось недель:");
    rows[1].dimension = 1000U * 60U * 60U * 24U * 7U;
    rows[0].title.setString(L"Осталось месяцев:");
    rows[0].dimension = 1000U * 60U * 60U * 24U * 30U;

    rows[6].title.setString(L"Пройдено:");
    rows[6].back.setFillColor(MAIN_PROGRESS_BACKGROUND);
    rows[6].rect.setFillColor(MAIN_PROGRESS_FOREGROUND);

    while (window.isOpen()) {
        Event event{};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::Resized) {
                WIDTH = event.size.width;
                HEIGHT = event.size.height;

                static FloatRect visibleArea;
                visibleArea.width = WIDTH;
                visibleArea.height = HEIGHT;

                window.setView(View(visibleArea));
            }
        }

        window.clear(BACKGROUND_COLOR);


        uint64_t now = chrono::duration_cast<chrono::milliseconds>(
                chrono::system_clock::now().time_since_epoch()).count();
        uint64_t s = GOAL_TIME - now;


        centerizeText(title, Vector2f(WIDTH / 2.f, 50.f));
        window.draw(title);

        static char *buffer = new char[32];

        uint64_t left = s;
        for (int i = 0; i < sizeof(rows) / sizeof(*rows) - 1; i++) {
            rows[i].back.setSize(Vector2f(WIDTH - 300.f, 30.f));
            rows[i].rect.setSize(
                    Vector2f((s % rows[i].dimension) * (1000.f / rows[i].dimension) * ((WIDTH - 300) / 1000.f), 30.f));

            snprintf(buffer, 32, "%.0f", floor(left / rows[i].dimension));
            rows[i].current.setString(buffer);
            centerizeText(rows[i].current, rows[i].back.getPosition(), rows[i].back.getSize());

            left %= rows[i].dimension;

            window.draw(rows[i].title);
            window.draw(rows[i].back);
            window.draw(rows[i].rect);
            window.draw(rows[i].current);
        }

        double percent = (now - START_TIME) / ((double) GOAL_TIME - START_TIME) * 100.;

        rows[6].back.setSize(Vector2f(WIDTH - 300.f, 30.f));
        rows[6].rect.setSize(Vector2f((float) percent * 10.f * ((WIDTH - 300) / 1000.f), 30.f));

        snprintf(buffer, 32, "%.8f%%", percent);
        rows[6].current.setString(buffer);
        centerizeText(rows[6].current, rows[6].back.getPosition(), rows[6].back.getSize());

        window.draw(rows[6].title);
        window.draw(rows[6].back);
        window.draw(rows[6].rect);
        window.draw(rows[6].current);

        window.display();
    }

    return 0;
}