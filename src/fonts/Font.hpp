#pragma once
#include <freetype/freetype.h>
#include <ft2build.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::vector;

struct Character {
    unsigned int TextureId;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};


class FontsANSII {
    static inline vector<Character> Characters;
};

class Fonts {
    static inline map<char, Character> Characters;
    static inline bool hasInit = false;
    static inline FT_Library ft;
    static inline FT_Face face;
    static inline void init() {
        if (FT_Init_FreeType(&ft)) {
            cerr << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
        }
        if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face)) {
            cerr << "ERROR::FREETYPE: Failed to load font" << endl;
        }
        FT_Set_Pixel_Sizes(face, 0, 48);
    }

    static inline void genCharacters() {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for (unsigned int c = 0; c < 128; ++c) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                cout << "ERROR::FREETYPE: Failed to load Glyph of '"
                     << static_cast<char>('a' + c) << "'" << endl;
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
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            Character character = {texture,
                                   glm::ivec2(face->glyph->bitmap.width,
                                              face->glyph->bitmap.rows),
                                   glm::ivec2(face->glyph->bitmap_left,
                                              face->glyph->bitmap_top),
                                   (unsigned int) face->glyph->advance.x};
            Characters.insert(pair<char, Character>(c, character));
        }
    }
    ~Fonts() {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
};
