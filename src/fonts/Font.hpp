#pragma once
#include <freetype/freetype.h>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <map>
#include <vector>
#include "fonts/BmpLoader.hpp"
#include "shaders/Shader.hpp"
#include "ui/Window.hpp"

#ifndef FONT_DIR
#define FONT_DIR "../../assets/fonts/"
#endif

using std::cerr;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;

struct Character {
    unsigned int textureID;
    glm::ivec2 Size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class FontBase {
public:
    map<char, Character> characters;
    Shader fontShader;
    unsigned int VAO, VBO;
    void genCharacters();
    void drawText(std::string text, float x, float y, float scale,
                  glm::vec3 color) {

        fontShader.use();
        fontShader.setVec3("textColor", color);
        glm::mat4 projection = glm::ortho(0.0f, (float) Window::width, 0.0f,
                                          (float) Window::height);
        fontShader.setMatrix4("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        string::const_iterator c;
        for (c = text.begin(); c != text.end(); ++c) {
            Character ch = characters[*c];
            float xpos = x + (float) ch.bearing.x * scale;
            float ypos = y - (float) (ch.Size.y - ch.bearing.y) * scale;
            float w = (float) ch.Size.x * scale;
            float h = (float) ch.Size.y * scale;
            float vertices[6][4] = {{xpos, ypos + h, 0.0f, 0.0f},
                                    {xpos, ypos, 0.0f, 1.0f},
                                    {xpos + w, ypos, 1.0f, 1.0f},
                                    {xpos, ypos + h, 0.0f, 0.0f},
                                    {xpos + w, ypos, 1.0f, 1.0f},
                                    {xpos + w, ypos + h, 1.0f, 0.0f}};
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            x += (float) (ch.advance >> 6) * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void setUpFontRender() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr,
                     GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};


class FontPixelStyle : public FontBase {
public:
    string font_vshader = GLSL_DIR "FontShader.vs";
    string font_fshader = GLSL_DIR "FontShader.fs";
    string font_path = FONT_DIR "pixel_font.bmp";
    int charWidth = 6;
    int charHeight = 8;
    int offsetX = 2;
    char start_char = '!';
    BmpFile bmpFile;
    FontPixelStyle() {
        bmpFile = BmpLoader::load(font_path);
        genCharacters();
        setUpFontRender();
        fontShader = Shader(font_vshader, font_fshader, ShaderParamType::PATH);
    }
    vector<uint8_t> getTexImage(const unsigned char ch) const {
        if (ch < start_char) {
            return vector<uint8_t>(charWidth * charHeight, 0);
        }
        int strideX = 8;
        int strideY = 8;
        auto res = vector<uint8_t>(charWidth * charHeight);
        int posY = (ch - start_char) / 8;
        int posX = (ch - start_char) % 8;
        auto baseX = strideX * posX;
        auto baseY = strideY * posY;

        for (int i = 0; i < charHeight; ++i) {
            for (int j = 0; j < charWidth; ++j) {
                res[i * charWidth + j] = *(
                        bmpFile.pixels.data() + (baseY + i) * bmpFile.rowSize +
                        (baseX + j + offsetX) * bmpFile.bytesPerPixel);
            }
        }
        return res;
    }

    void genCharacters() {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for (unsigned char c = 0; c < 128; ++c) {
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, charWidth, charHeight, 0,
                         GL_RED, GL_UNSIGNED_BYTE, getTexImage(c).data());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            const unsigned int bearing = c == '@' ? charWidth + 1 : charWidth;
            Character character = {texture, glm::ivec2(charWidth, charHeight),
                                   glm::ivec2(1, 6), bearing << 6};
            characters.insert(pair<char, Character>(c, character));
        }
    }
    void debugRenderChar(const unsigned char c) {
        auto img = getTexImage(c);
        for (int i = 0; i < charHeight; ++i) {
            for (int j = 0; j < charWidth; ++j) {
                if (img[i * charWidth + j] == 255) {
                    cout << "#";
                } else {
                    cout << " ";
                }
            }
            cout << endl;
        }
    }

    void debugTest() {
        for (char i = ' '; i <= '~'; ++i) {
            debugRenderChar(i);
            cout << "====================" << endl;
        }
    }
};

class Fonts : public FontBase {
public:
    FT_Library ft;
    FT_Face face;
    string font_vshader = GLSL_DIR "FontShader.vs";
    string font_fshader = GLSL_DIR "FontShader.fs";
    string font_path = FONT_DIR "arial.ttf";

    Fonts() {
        if (FT_Init_FreeType(&ft)) {
            cerr << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
        }
        if (FT_New_Face(ft, font_path.c_str(), 0, &face)) {
            cerr << "ERROR::FREETYPE: Failed to load font" << endl;
        }
        FT_Set_Pixel_Sizes(face, 0, 48);
        genCharacters();
        setUpFontRender();
        fontShader = Shader(font_vshader, font_fshader, ShaderParamType::PATH);
    }


    void genCharacters() {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for (unsigned int c = 0; c < 128; ++c) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                cout << "ERROR::FREETYPE: Failed to load Glyph of '"
                     << static_cast<char>(c) << "'" << endl;
                continue;
            }
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                         (signed int) face->glyph->bitmap.width,
                         (signed int) face->glyph->bitmap.rows, 0, GL_RED,
                         GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            // use GL_NEAREST to try out pixel art
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            Character character = {texture,
                                   glm::ivec2(face->glyph->bitmap.width,
                                              face->glyph->bitmap.rows),
                                   glm::ivec2(face->glyph->bitmap_left,
                                              face->glyph->bitmap_top),
                                   (unsigned int) face->glyph->advance.x};
            characters.insert(pair<char, Character>(c, character));
        }
    }


    ~Fonts() {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
};
