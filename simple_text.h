/*
MIT License

Copyright (c) 2024 P. G. G. Costa

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef PGGK_SIMPLE_TEXT_H
#define PGGK_SIMPLE_TEXT_H
#include <stdio.h>
#include <stdint.h>

#include <string>
#include <vector>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * @brief Struct of a SDL_Texture that can handle a text draw.
 *
 * It just stores an SDL_Texture pointer and a bool to sinalize
 * if all the text has finished being drawn on the texture.
 */
struct CombinedTexture
{
    /// Indicates whether text drawing is complete.
    bool finished = false;

    /// Pointer to the target texture.
    SDL_Texture *texture = nullptr;

    ~CombinedTexture()
    {
        SDL_DestroyTexture(texture);
    }
};

/**
 * @brief Struct that stores settings of a Typewritter Effect.
 *
 * Store the counters, timers and duration of a ingle char typing.
 */
struct TypeStats
{
    std::vector<std::string> current_lines;
    std::string temp_text;
    std::vector<uint32_t> utf8_text;
    /// Indicates the current char index.
    int type_counter;

    /// Indicates the current horizontal position of the char.
    int current_x;

    /// Needs to be updated every frame (by adding delta time).
    float timer;

    /// Duration in seconds of a character typing.
    float duration;

    TypeStats(int _tc, int _cx, int _t, int _d)
    {
        type_counter = _tc;
        current_x = _cx;
        timer = _t;
        duration = _d;
        current_lines.clear();
        utf8_text.clear();
        temp_text = "";
    }
};

/**
 * @brief Struct that stores a font atlas info.
 *
 * Store the SDL_Texture of a font atlas and also
 * it's all characters.
 */
struct FontAtlas
{
    /// Pointer to the atlas texture.
    SDL_Texture *atlas_texture;

    /// Pointer to an array containing all the characters of the atlas ordered.
    char *characters;

    /**
     * Initialize the FontAtlas
     * @param filename path to the atlas png source file.
     * @param renderer pointer to the current renderer.
     * @param chars pointer to the characters array.
     */
    FontAtlas(const char *filename, SDL_Renderer *renderer, const char* chars)
    {
        SDL_Surface *image = IMG_Load(filename);
        atlas_texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_SetTextureBlendMode(atlas_texture, SDL_BLENDMODE_BLEND);
        characters = (char *) chars;
        SDL_FreeSurface(image);
    }

    ~FontAtlas()
    {
        if (characters)
        {
            free(characters);
        }
        if (atlas_texture)
        {
            SDL_DestroyTexture(atlas_texture);
        }
    }
};

/**
 * Get the index of an given character.
 * @param input_string reference to the original text.
 * @returns text without newline characters
 */
int get_char_index(uint32_t character, const char *atlas);

/**
 * Get the source rect of a character in a font atlas using the character index.
 * @param index current character index in a font atlas.
 * @param atlas_width width of the font atlas.
 * @param atlas_height height of the font atlas.
 * @param cell_width width of a font atlas cell.
 * @param cell_height height of a font atlas cell.
 * @returns source rect
 */
SDL_Rect get_atlas_rect_by_index(const int index, const int atlas_width, const int atlas_height, const int cell_width, const int cell_height);

/**
 * Remove all newline characters from a string.
 * @param input_string reference to the original text.
 * @returns text without newline characters
 */
std::string remove_new_lines(const std::string &input_string);

/**
 * Split a text into a vector of strings using newline character as separator.
 * @param input_string reference to the original text.
 * @returns splitted text
 */
std::vector<std::string> split_string_by_newline(const std::string &input_string);

/**
 * Split a text into a vector of strings when the horizontal draw size is too long.
 * @param text reference to the original text.
 * @param size size of the font when drawing.
 * @param h_offset horizontal offset between characters in percentage.
 * @param max_length max horizontal draw size per line.
 * @returns splitted text
 */
std::vector<std::string> split_text_by_size(const std::string &text, const int size, const int h_offset, const int max_length);

/**
 * Split a text into a vector of strings using newline as separator and breaking the
 * line when the horizontal draw sie is too long.
 * @param text reference to the original text.
 * @param size size of the font when drawing.
 * @param h_offset horizontal offset between characters in percentage.
 * @param max_length max horizontal draw size per line.
 * @returns splitted text
 */
std::vector<std::string> get_all_lines(const std::string &text, const int size, const int h_offset, const int max_length);

/**
 * Get the line of a splitted text by a current char index.
 *
 * @param lines splitted text, stored as a vector of strings.
 * @param current_char index of the current char.
 * @returns index of the current line.
 */
int get_current_line(const std::vector<std::string> &lines, const int current_char);

/**
 * Draw a text line into a single texture (if provided) or into the renderer.
 *
 * @param text string to be drawed.
 * @param font_atlas FontAtlas reference containing the font atlas texture and characters.
 * @param renderer SDL_Renderer pointer to the current renderer.
 * @param x position X inside the current renderer context.
 * @param y position Y inside the current renderer context.
 * @param size size of the font when drawing.
 * @param h_offset horizontal offset between characters in percentage.
 * @param target CombineTexture pointer to the texture that will store the text.
 * @param color Color of the text in RGB format.
 */
void draw_text(const std::string &text,
               const FontAtlas &font_atlas,
               SDL_Renderer *renderer,
               const int x,
               const int y,
               const int size,
               const int h_offset = 57,
               CombinedTexture *target = nullptr,
               SDL_Color color = {255, 255, 255});

/**
 * Draw a text line into a single texture (if provided) or into the renderer.
 *
 * @param utf8_text text as uint32_t utf8 char vector.
 * @param font_atlas FontAtlas reference containing the font atlas texture and characters.
 * @param renderer SDL_Renderer pointer to the current renderer.
 * @param x position X inside the current renderer context.
 * @param y position Y inside the current renderer context.
 * @param size size of the font when drawing.
 * @param h_offset horizontal offset between characters in percentage.
 * @param target CombineTexture pointer to the texture that will store the text.
 * @param color Color of the text in RGB format.
 */
void draw_utf8_text(const std::vector<uint32_t> utf8_text,
                    const FontAtlas &font_atlas,
                    SDL_Renderer *renderer,
                    const int x,
                    const int y,
                    const int size,
                    const int h_offset,
                    CombinedTexture *target,
                    SDL_Color color = {255, 255, 255});

/**
 * Draw multiline text with typewritter.
 *
 * @param text string to be drawed.
 * @param font_atlas FontAtlas reference containing the font atlas texture and characters.
 * @param renderer SDL_Renderer pointer to the current renderer.
 * @param rect SDL_Rect destiny reference.
 * @param target CombineTexture pointer to the texture that will store the text.
 * @param stats TypeStats reference to typewritter effect attributes.
 * @param size size of the font when drawing.
 * @param callback function that will be called when the text finishes drawing.
 * @param h_offset horizontal offset between characters in percentage.
 * @param v_offset vertical offset between characters in percentage.
 */
void draw_typewriter(std::string &text,
                     const FontAtlas &font_atlas,
                     SDL_Renderer *renderer,
                     const SDL_Rect &rect,
                     CombinedTexture *target,
                     TypeStats &stats,
                     const int size,
                     void (*callback)() = nullptr,
                     const int h_offset = 57,
                     const int v_offset = 70);

/**
 * Draw multiline text into a single texture (if provided) or into the renderer.
 *
 * @param text string to be drawed.
 * @param font_atlas FontAtlas reference containing the font atlas texture and characters.
 * @param renderer SDL_Renderer pointer to the current renderer.
 * @param rect SDL_Rect destiny reference.
 * @param size size of the font when drawing.
 * @param h_offset horizontal offset between characters in percentage.
 * @param v_offset vertical offset between characters in percentage.
 * @param target CombineTexture pointer to the texture that will store the text.
 */
void draw_text_multiline(const std::string &text,
                         const FontAtlas &font_atlas,
                         SDL_Renderer *renderer,
                         const SDL_Rect &rect,
                         const int size,
                         const int h_offset = 57,
                         const int v_offset = 70,
                         CombinedTexture *target = nullptr,
                         std::vector<std::string> *c_lines = nullptr);

bool is_utf8_start(char c)
{
    return ((c & 0xE0) == 0xC0 || (c & 0xF0) == 0xE0 || (c & 0xF8) == 0xF0);
}

bool is_utf_cont(char c)
{
    return (c & 0xC0) == 0x80;
}

std::vector<uint32_t> get_utf8_char_vector(const char *text)
{
    std::vector<uint32_t> converted;
    uint32_t temp_char = 0x00000000;
    uint8_t current_offset = 0;
    uint32_t mask;
    for (int i = 0; i < strlen(text); i++)
    {
        if (is_utf8_start(text[i]))
        {
            current_offset = 0;
            temp_char = 0x00000000;
            if ((text[i] & 0xE0) == 0xC0)
            {
                temp_char = temp_char | (text[i] << 8);
                current_offset = 0;
                mask = 0xffff0000;
            }
            else if ((text[i] & 0xF0) == 0xE0)
            {
                temp_char = temp_char | (text[i] << 16);
                current_offset = 1;
                mask = 0xff000000;
            }
            else if ((text[i] & 0xF8) == 0xF0)
            {
                temp_char = temp_char | (text[i] << 24);
                current_offset = 2;
                mask = 0x00000000;
            }
        }
        else if (is_utf_cont(text[i]))
        {
            uint32_t num = 0x00000000;
            num = text[i] << (current_offset * 8);
            if (current_offset == 0)
                temp_char = (temp_char | (num & 0xff0000ff));
            if (current_offset == 1)
                temp_char = (temp_char | (num & 0xff00ffff));
            if (current_offset == 2)
                temp_char = (temp_char | (num & 0xffffffff));
            current_offset--;
            if ((i + 1 < strlen(text) && !is_utf_cont(text[i + 1])) || (i + 1 == strlen(text)))
            {
                temp_char = temp_char ^ mask;
                converted.push_back(temp_char);
            }
        }
        else
        {
            converted.push_back((uint32_t)(text[i]));
        }
    }
    return converted;
}

int get_char_index(uint32_t character, const char *atlas)
{
    std::vector<uint32_t> utf8_atlas = get_utf8_char_vector(atlas);
    for (int i = 0; i < utf8_atlas.size(); i++)
    {
        if (utf8_atlas[i] == character)
            return i; 
    }
    return -1;
}

SDL_Rect get_atlas_rect_by_index(const int index, const int atlas_width, const int atlas_height, const int cell_width, const int cell_height)
{
    SDL_Rect rect;
    int cells_per_row = atlas_width / cell_width;

    int row = index / cells_per_row;
    int col = index % cells_per_row;

    rect.x = col * cell_width;
    rect.y = row * cell_height;
    rect.w = cell_width;
    rect.h = cell_height;

    return rect;
}

std::string remove_new_lines(const std::string &input_string)
{
    std::string result = input_string;
    size_t found = result.find('\n');
    while (found != std::string::npos)
    {
        result.erase(found, 1);
        found = result.find('\n');
    }
    return result;
}

std::vector<std::string> split_string_by_newline(const std::string &input_string)
{
    std::vector<std::string> result;
    std::istringstream iss(input_string);

    std::string line;
    while (std::getline(iss, line, u8'\n'))
    {
        result.push_back(remove_new_lines(line));
    }

    return result;
}

std::vector<std::string> split_text_by_size(const std::string &text, const int size, const int h_offset, const int max_length)
{
    std::vector<std::string> result;

    std::istringstream iss(text);
    std::string word;

    int real_offset = size - (size * h_offset / 100);

    while (iss >> word)
    {
        if (result.empty())
        {
            result.push_back(word);
        }
        else
        {
            std::string &lastElement = result.back();
            if ((lastElement.length() * real_offset) + (word.length() * real_offset) + real_offset <= max_length)
            {
                lastElement += " " + word;
            }
            else
            {
                result.push_back(word);
            }
        }
    }

    return result;
}

std::vector<std::string> get_all_lines(const std::string &text, const int size, const int h_offset, const int max_length)
{
    std::vector<std::string> result;
    std::vector<std::string> temp = split_string_by_newline(text);
    for(auto t : temp)
    {
        std::vector<std::string> splt = split_text_by_size(t, size, h_offset, max_length);
        for(auto s : splt)
        {
            result.push_back(s);
        }
    }
    return result;
}

int get_current_line(const std::vector<std::string> &lines, const int current_char)
{
    int total_chars = 0;

    for (int i = 0; i < lines.size(); i++)
    {
        std::vector<uint32_t> utf8_line = get_utf8_char_vector(lines[i].c_str());
        total_chars += utf8_line.size() + 1;

        if (total_chars > current_char)
        {
            return i;
        }
    }
    return -1;
}

void draw_text(const std::string &text,
               const FontAtlas &font_atlas,
               SDL_Renderer *renderer,
               const int x,
               const int y,
               const int size,
               const int h_offset,
               CombinedTexture *target,
               SDL_Color color)
{
    if (target && target->texture == nullptr)
    {
        int w, h;
        SDL_GetRendererOutputSize(renderer, &w, &h);
        target->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
        SDL_SetRenderTarget(renderer, target->texture);
        SDL_SetTextureBlendMode(target->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
    }
    if (target && target->finished)
    {
        if (SDL_GetRenderTarget(renderer) != nullptr)
        {
            SDL_SetRenderTarget(renderer, nullptr);
        }
        SDL_RenderCopy(renderer, target->texture, nullptr, nullptr);
        return;
    }
    int current_x = x;
    std::vector<uint32_t> utf8_text = get_utf8_char_vector(text.c_str());
    for (auto c : utf8_text)
    {
        if (c == ' ')
        {
            current_x += (size - (h_offset * size / 100));
            if (target)
            {
                if (SDL_GetRenderTarget(renderer) != nullptr)
                {
                    SDL_SetRenderTarget(renderer, nullptr);
                }
                SDL_RenderCopy(renderer, target->texture, nullptr, nullptr);
            }
            continue;
        }
        const int index = get_char_index(c, font_atlas.characters);
        if (index != -1)
        {
            SDL_Rect source;
            SDL_Rect src = get_atlas_rect_by_index(index, 512, 512, 32, 32);
            source.x = src.x;
            source.y = src.y;
            source.w = src.w;
            source.h = src.h;
            SDL_Rect destiny = {current_x, y, size, size};
            current_x += (size - (h_offset * size / 100));

            if (target)
            {
                int w, h;
                SDL_GetRendererOutputSize(renderer, &w, &h);
                SDL_Texture *tmp_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

                SDL_SetRenderTarget(renderer, tmp_target);
                SDL_SetTextureBlendMode(tmp_target, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderClear(renderer);
                SDL_SetTextureColorMod(tmp_target, color.r, color.g, color.b);
                SDL_RenderCopy(renderer, font_atlas.atlas_texture, &source, &destiny);

                SDL_SetRenderTarget(renderer, target->texture);
                SDL_RenderCopy(renderer, tmp_target, nullptr, nullptr);
                SDL_SetRenderTarget(renderer, nullptr);
                SDL_RenderCopy(renderer, target->texture, nullptr, nullptr);
                SDL_DestroyTexture(tmp_target);
                tmp_target = nullptr;
            }
            else
            {
                SDL_RenderCopy(renderer, font_atlas.atlas_texture, &source, &destiny);
            }
        }
        else
        {
            if (target)
            {
                if (SDL_GetRenderTarget(renderer) != nullptr)
                {
                    SDL_SetRenderTarget(renderer, nullptr);
                }
                SDL_RenderCopy(renderer, target->texture, nullptr, nullptr);
            }
        }
    }
    if (target)
    {
        if (SDL_GetRenderTarget(renderer) != nullptr)
        {
            SDL_SetRenderTarget(renderer, nullptr);
        }
        target->finished = true;
    }
}

void draw_utf8_text(const std::vector<uint32_t> utf8_text,
                    const FontAtlas &font_atlas,
                    SDL_Renderer *renderer,
                    const int x,
                    const int y,
                    const int size,
                    const int h_offset,
                    CombinedTexture *target,
                    SDL_Color color)
{
    if (target && target->texture == nullptr)
    {
        int w, h;
        SDL_GetRendererOutputSize(renderer, &w, &h);
        target->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
        SDL_SetRenderTarget(renderer, target->texture);
        SDL_SetTextureBlendMode(target->texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
    }
    if (target && target->finished)
    {
        if (SDL_GetRenderTarget(renderer) != nullptr)
        {
            SDL_SetRenderTarget(renderer, nullptr);
        }
        SDL_RenderCopy(renderer, target->texture, nullptr, nullptr);
        return;
    }
    int current_x = x;
    for (auto c : utf8_text)
    {
        if (c == ' ')
        {
            current_x += (size - (h_offset * size / 100));
            if (target)
            {
                if (SDL_GetRenderTarget(renderer) != nullptr)
                {
                    SDL_SetRenderTarget(renderer, nullptr);
                }
                SDL_RenderCopy(renderer, target->texture, nullptr, nullptr);
            }
            continue;
        }
        const int index = get_char_index(c, font_atlas.characters);
        if (index != -1)
        {
            SDL_Rect source;
            SDL_Rect src = get_atlas_rect_by_index(index, 512, 512, 32, 32);
            source.x = src.x;
            source.y = src.y;
            source.w = src.w;
            source.h = src.h;
            SDL_Rect destiny = {current_x, y, size, size};
            current_x += (size - (h_offset * size / 100));

            if (target)
            {
                int w, h;
                SDL_GetRendererOutputSize(renderer, &w, &h);
                SDL_Texture *tmp_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

                SDL_SetRenderTarget(renderer, tmp_target);
                SDL_SetTextureBlendMode(tmp_target, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderClear(renderer);
                SDL_SetTextureColorMod(tmp_target, color.r, color.g, color.b);
                SDL_RenderCopy(renderer, font_atlas.atlas_texture, &source, &destiny);

                SDL_SetRenderTarget(renderer, target->texture);
                SDL_RenderCopy(renderer, tmp_target, nullptr, nullptr);
                SDL_SetRenderTarget(renderer, nullptr);
                SDL_RenderCopy(renderer, target->texture, nullptr, nullptr);
                SDL_DestroyTexture(tmp_target);
                tmp_target = nullptr;
            }
            else
            {
                SDL_RenderCopy(renderer, font_atlas.atlas_texture, &source, &destiny);
            }
        }
        else
        {
            if (target)
            {
                if (SDL_GetRenderTarget(renderer) != nullptr)
                {
                    SDL_SetRenderTarget(renderer, nullptr);
                }
                SDL_RenderCopy(renderer, target->texture, nullptr, nullptr);
            }
        }
    }
    if (target)
    {
        if (SDL_GetRenderTarget(renderer) != nullptr)
        {
            SDL_SetRenderTarget(renderer, nullptr);
        }
        target->finished = true;
    }
}

void draw_typewriter(std::string &text,
                         const FontAtlas &font_atlas,
                         SDL_Renderer *renderer,
                         const SDL_Rect &rect,
                         CombinedTexture *target,
                         TypeStats &stats,
                         const int size,
                         void (*callback)(),
                         const int h_offset,
                         const int v_offset)
{
    std::vector<std::string> lines = get_all_lines(text, size, h_offset, rect.w);
    std::string tmptext = remove_new_lines(text);

    if(stats.utf8_text.size() == 0)
    {
        stats.utf8_text = get_utf8_char_vector(tmptext.c_str());
    }

    if (stats.type_counter > stats.utf8_text.size() - 1)
    {
        if (SDL_GetRenderTarget(renderer) != nullptr)
        {
            SDL_SetRenderTarget(renderer, nullptr);
        }
        SDL_RenderCopy(renderer, target->texture, nullptr, nullptr);
        return;
    }
    if(stats.timer > stats.duration)
    {
        int current_line = get_current_line(lines, stats.type_counter);
        if(stats.type_counter > 0 && current_line > get_current_line(lines, stats.type_counter - 1))
        {
            stats.current_x = rect.x;
        }
        if (get_char_index(stats.utf8_text[stats.type_counter], font_atlas.characters) == -1 && stats.utf8_text[stats.type_counter] != get_utf8_char_vector("  ")[0])
        {
            stats.type_counter++;
            if (SDL_GetRenderTarget(renderer) != nullptr)
            {
                SDL_SetRenderTarget(renderer, nullptr);
            }
            SDL_RenderCopy(renderer, target->texture, nullptr, nullptr);
            return;
        }
        target->finished = false;
        std::vector<uint32_t> u8t = {stats.utf8_text[stats.type_counter]};
        draw_utf8_text(u8t,
                       font_atlas,
                       renderer,
                       stats.current_x,
                       rect.y + (current_line * (size * v_offset / 100)),
                       size,
                       h_offset,
                       target);

        stats.current_x += (size - (h_offset * size / 100));
        stats.timer = 0;
        stats.type_counter++;
        if (stats.type_counter > tmptext.length() - 1 && callback != nullptr)
        {
            callback();
        }
    }
    else
    {
    if (SDL_GetRenderTarget(renderer) != nullptr)
    {
        SDL_SetRenderTarget(renderer, nullptr);
    }
    SDL_RenderCopy(renderer, target->texture, nullptr, nullptr);
    }
}

void draw_text_multiline(const std::string &text,
                         const FontAtlas &font_atlas,
                         SDL_Renderer *renderer,
                         const SDL_Rect &rect,
                         const int size,
                         const int h_offset,
                         const int v_offset,
                         CombinedTexture *target,
                         std::vector<std::string> *c_lines)
{
    if(target && target->finished)
    {
        SDL_Rect r;
        r.x = rect.x;
        r.y = rect.y;
        r.h = rect.h;
        r.w = rect.w;
        SDL_RenderCopy(renderer, target->texture, &r, &r);
        return;
    }

    std::vector<std::string> lines;
    if (c_lines)
        lines = *c_lines;
    else
        lines = split_text_by_size(text, size, h_offset, rect.w);
    int current_y = rect.y;
    for(auto l : lines)
    {
        if (target)
        {
            target->finished = false;
            draw_text(l, font_atlas, renderer, rect.x, current_y, size, h_offset, target);
        }
        else
        {
            draw_text(l, font_atlas, renderer, rect.x, current_y, size, h_offset);
        }
        current_y += (size * v_offset / 100);
    }
}
#endif