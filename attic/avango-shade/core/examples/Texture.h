// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 2007 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#ifndef Texture_H
#define Texture_H Texture_H

#include <iostream>
#include <vector>
#include <boost/shared_array.hpp>

class FileFormatError
{
};

class Texture
{
  public:
    /* Create a new texture */
    Texture(int width, int height, bool alpha = false, int levels = 1, int fill_r = -1, int fill_g = 0, int fill_b = 0, int fill_a = 255);

    /* Load texture from file */
    Texture(const char* fname, int array = 0);

    /* Destructor */
    ~Texture(void);

    /* Save texture to file */
    void save(const char* fname);

    /* Returns width of given level or base if none */
    int get_width(int level = 0) const;

    /* Returns height of given level or base if none */
    int get_height(int level = 0) const;

    /* Returns number of level (from 1 to ...)  */
    int get_levels(void) const { return m_levels; }

    /* Upload to OpenGL */
    void upload(void) const;

    /* Sets border mode for OpenGL */
    void set_border(int border);

    /* Read and write pixels */
    enum component
    {
        R = 0,
        G,
        B,
        A
    };

    unsigned char get_pixel(int x, int y, int i, int level = 0) { return m_data[level][(y * get_width(level) + x) * 4 + i]; }

    void set_pixel(int x, int y, int i, unsigned char v, int level = 0) { m_data[level][(y * get_width(level) + x) * 4 + i] = v; }

  private:
    unsigned int read_dword(std::istream& hfile);
    void write_dword(std::ostream& hfile, unsigned int v);
    void align_dword(std::istream& hfile);
    void align_dword(std::ostream& hfile);

    int byte_index(unsigned int mask);

    int m_width, m_height;
    bool m_alpha;
    int m_levels;
    int m_border;
    int m_array;

    std::vector<boost::shared_array<unsigned char>> m_data;
};

#endif /* Texture_H */
