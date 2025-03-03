#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

#include "Tiny/tinyfiledialogs.h"

#include <iostream>
#include <stdio.h>
#include <pthread.h>

#include <stdio.h>
#include <unistd.h>

const int PATCH_IP = 0x09;
const int WINDOWX = 300;
const int WINDOWY = 300;

const int posX = 110;
const int posY = 180;

#define WHOLE_LOTTA_GREEN 255, 137, 137
#define WHOLE_LOTTA_RED   132, 172,  70

enum patch_errors
{
    P_SUCCESS,
    P_FOPEN_ERROR,
    P_FSIZE_ERROR,
    P_WRNG_VER,
    P_PATCHED,
    P_UNKNOWN

};

class Button {
public:
    Button(const std::string& text, const sf::Font& font, unsigned int characterSize, const sf::Vector2f& position, const sf::Color& color)
        : m_text(text, font, characterSize), m_position(position), m_color(color) {
        m_rect.setSize(sf::Vector2f(100, 50)); // Set button size
        m_rect.setFillColor(m_color);
        m_rect.setPosition(m_position);

        m_text.setPosition(m_position.x + 10, m_position.y + 10); // Adjust text position
    }

    void draw(sf::RenderWindow& window) {
        window.draw(m_rect);
        window.draw(m_text);
    }

    bool isMouseOver(const sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        return m_rect.getGlobalBounds().contains(mousePos.x, mousePos.y);
    }

    void setColor(const sf::Color& color) {
        m_rect.setFillColor(color);
    }

private:
    sf::RectangleShape m_rect;
    sf::Text m_text;
    sf::Vector2f m_position;
    sf::Color m_color;
};


int BinPatch(const char * filename);
void * Viz(void * arg);
void * Patcher(void * arg);

int main()
{
    pthread_t threads[2];

    pthread_create(&threads[0], NULL, Patcher, NULL);
    pthread_create(&threads[0], NULL, Viz, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}

void * Patcher(void * arg)
{
    sf::RenderWindow window(sf::VideoMode(300, 300), "whole lotta red patcher");

    sf::Font font;
    if (!font.loadFromFile("MDIO-Regular.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return NULL;
    }

    Button button("patch it!", font, 14, sf::Vector2f(110, 110), sf::Color(137, 172, 70));

    std::vector<sf::Texture> frames;
    int frameCount = 39;

    for (int i = 0; i <= frameCount; i++)
    {
        sf::Texture texture;
        std::string filename = "OldMoney/" + std::to_string(i) + ".gif";

        if (!texture.loadFromFile(filename))
        {
            printf("Failed to load frame %d\n", i);
        }

        frames.push_back(texture);
    }

    sf::Sprite sprite;
    sprite.setTexture(frames[0]);
    sprite.setScale(0.25f, 0.25f);
    sprite.setPosition(posX, posY);
    int currentFrame = 0;
    float frameTime = 0.1f;
    float elapsedTime = 0.0f;

    sf::Clock clock;

    while (window.isOpen()) {

        sf::Time deltaTime = clock.restart();
        elapsedTime += deltaTime.asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (button.isMouseOver(window))
                    {

                        const char * filePath = tinyfd_openFileDialog("Choose a file", ".", 0, nullptr, nullptr, 0);

                        if (!filePath) std::cout << "No file was selected!" << std::endl;
                        else BinPatch(filePath);

                    }
                }
            }

        }

        if (button.isMouseOver(window)) button.setColor(sf::Color(WHOLE_LOTTA_GREEN));
        else                            button.setColor(sf::Color(WHOLE_LOTTA_RED));

        if (elapsedTime >= frameTime)
        {
            currentFrame = (currentFrame + 1) % frames.size();
            sprite.setTexture(frames[currentFrame]);
            sprite.setScale(0.25f, 0.25f);
            sprite.setPosition(posX, posY);
            elapsedTime = 0.0f;
        }

        window.clear();
        button.draw(window);
        window.draw(sprite);
        window.display();
    }

    return NULL;
}

void * Viz(void * arg)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("PBC/playboi carti.mp3")) return NULL;

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    while (sound.getStatus() == sf::Sound::Playing) sf::sleep(sf::milliseconds(100));

    return NULL;
}

size_t GetFileSize(FILE * file)
{
    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    if (fsize == -1) {fclose(file); return P_FSIZE_ERROR;}
    rewind(file);

    return fsize;
}

char * ToBuf(FILE * file, size_t fsize)
{
    char * buf = (char*) calloc(fsize + 1, 1);
    if (!buf) return NULL;

    size_t what = 0;
    if (what = fread(buf, sizeof(char), fsize, file) != fsize)

    {
        fprintf(stderr, "fread = %lu, fsize = %lu", what, fsize);
        return NULL;
    }
    return buf;

}

int BinPatch(const char * filename)
{
    FILE * file = fopen(filename, "r+b");

    if (!file) return P_FOPEN_ERROR;

    size_t fsize = GetFileSize(file);

    char * binBuf = ToBuf(file, fsize);

    if (*binBuf != 0x33)
    {
        free(binBuf);

        printf("Wrong version of program!\n");
        return P_WRNG_VER;
    }
    if (binBuf[PATCH_IP] == 0xffffff8b && binBuf[PATCH_IP+1] == 0xffffffc1)
    {

        fclose(file);
        file = fopen(filename, "w+b");
        system("clear");

        for (int i = 0; i < 10; i++)
        {
            printf("choosing the bytes for patching.\n");
            usleep(100 * 1000);
            system("clear");
            printf("choosing the bytes for patching..\n");
            usleep(100 * 1000);
            system("clear");
            printf("choosing the bytes for patching...\n");
            usleep(100 * 1000);
            system("clear");
        }

        binBuf[PATCH_IP] = 0x33;
        binBuf[PATCH_IP + 1] = 0xc0;
        fwrite(binBuf, fsize, 1, file);

        free(binBuf);
        fclose(file);

        printf("File Successfully Patched!\n");
        return P_SUCCESS;
    }

    else if (binBuf[PATCH_IP] == 0x33 && binBuf[PATCH_IP+1] == 0xffffffc0)
    {
        free(binBuf);

        printf("File has been already patched!\n");
        return P_PATCHED;
    }

    free(binBuf);

    printf("Unknown error!\n");
    return P_UNKNOWN;

}
