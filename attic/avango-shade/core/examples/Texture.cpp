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

#include "Texture.h"

#include <fstream>
#include <GL/glew.h>

/* DDS Format definitions */
#define DDS_MAGIC 0x20534444

#define DDSD_CAPS 0x00000001
#define DDSD_HEIGHT 0x00000002
#define DDSD_WIDTH 0x00000004
#define DDSD_PITCH 0x00000008
#define DDSD_PIXELFORMAT 0x00001000
#define DDSD_MIPMAPCOUNT 0x00020000
#define DDSD_LINEARSIZE 0x00080000
#define DDSD_DEPTH 0x00800000

#define DDPF_ALPHAPIXELS 0x00000001
#define DDPF_FOURCC 0x00000004
#define DDPF_RGB 0x00000040

#define DDSCAPS_COMPLEX 0x00000008
#define DDSCAPS_TEXTURE 0x00001000
#define DDSCAPS_MIPMAP 0x00400000


Texture::Texture(const char* fname, int array) :
  m_border(0),
  m_array(array)
{
  std::ifstream hfile(fname);

  /* Check for header magic */
  if (read_dword(hfile) != DDS_MAGIC)
    throw FileFormatError();
  if (read_dword(hfile) != 124)
    throw FileFormatError();

  /* Read useful information */
  unsigned int dwFlags = read_dword(hfile);
  m_height = read_dword(hfile);
  m_width = read_dword(hfile);
  /*unsigned int dwPitchOrLinearSize =*/ read_dword(hfile);
  unsigned int dwDepth = read_dword(hfile);
  m_levels = read_dword(hfile);

  /* Validate data */
  if ( ((dwFlags & DDSD_CAPS) == 0) || ((dwFlags & DDSD_PIXELFORMAT) == 0)
      || ((dwFlags & DDSD_HEIGHT) == 0) || ((dwFlags & DDSD_WIDTH) == 0))
    throw FileFormatError();
  if ( ((dwFlags & DDSD_CAPS) != 0) && (dwDepth != 0) )
    throw FileFormatError();
  if ((dwFlags & DDSD_MIPMAPCOUNT) == 0)
    m_levels = 1;

  /* Reserved words */
  for (int i=0; i != 11; ++i)
    read_dword(hfile);

  /* Read DDPIXELFORMAT structure */
  if (read_dword(hfile) != 32)
    throw FileFormatError();
  unsigned int dwPFlags = read_dword(hfile);
  if ((dwPFlags & DDPF_RGB) == 0)
    throw FileFormatError();
  if ((dwPFlags & DDPF_ALPHAPIXELS) != 0)
    m_alpha = true;
  else
    m_alpha = false;

  read_dword(hfile); /* dwFourCC */
  unsigned int dwRGBBitCount = read_dword(hfile);
  unsigned int dwRBitMask = read_dword(hfile);
  unsigned int dwGBitMask = read_dword(hfile);
  unsigned int dwBBitMask = read_dword(hfile);
  unsigned int dwAlphaBitMask = read_dword(hfile);
  if ( (!m_alpha && (dwRGBBitCount != 24)) ||
      (m_alpha  && (dwRGBBitCount != 32)) )
    throw FileFormatError();
  int comp_swap[4];
  comp_swap[R] = byte_index(dwRBitMask);
  comp_swap[G] = byte_index(dwGBitMask);
  comp_swap[B] = byte_index(dwBBitMask);
  if (m_alpha)
    comp_swap[A] = byte_index(dwAlphaBitMask);

  /* Read DDCAPS2 structure */
  unsigned int dwCaps1 = read_dword(hfile);
  /*unsigned int dwCaps2 =*/ read_dword(hfile);

  /* Validate data */
  if ((dwCaps1 & DDSCAPS_TEXTURE) == 0)
    throw FileFormatError();
  if ((dwCaps1 & DDSCAPS_MIPMAP) == 0)
    m_levels = 1;

  /* Reserved words */
  for (int i=0; i != 2; ++i)
    read_dword(hfile);

  /* Reserved words */
  read_dword(hfile);

  /* Now read pixel data */
  //m_data = new unsigned char*[m_levels];
  m_data.resize(m_levels);
  int components = m_alpha?4:3;
  for (int i = 0; i != m_levels; ++i)
  {
    //m_data[i] = new unsigned char[get_width(i)*get_height(i)*4];
    m_data[i] = boost::shared_array<unsigned char>(new unsigned char[get_width(i)*get_height(i)*4]);

    for (int y = 0; y != get_height(i); ++y)
    {
      for (int x = 0; x != get_width(i); ++x)
        for (int k = 0; k != components; ++k)
        {
          unsigned char v = static_cast<unsigned char>(hfile.get());
          set_pixel(x, y, comp_swap[k], v, i);
        }
      //align_dword(hfile);
    }

    //align_dword(hfile);
  }

}

  Texture::Texture(int width, int height, bool alpha, int levels, int fill_r, int fill_g, int fill_b, int fill_a)
: m_width(width), m_height(height), m_alpha(alpha), m_levels(levels), m_border(0)
{
  /* Now alloc pixel data */
  //m_data = new unsigned char*[m_levels];
  m_data.resize(m_levels);
  for (int i = 0; i != m_levels; ++i)
  {
    //m_data[i] = new unsigned char[get_width(i)*get_height(i)*4];
    m_data[i] = boost::shared_array<unsigned char>(new unsigned char[get_width(i)*get_height(i)*4]);
    if (fill_r != -1)
      for (int j = 0; j != get_width(i)*get_height(i); ++j)
      {
        m_data[i][4*j+0] = fill_r;
        m_data[i][4*j+1] = fill_g;
        m_data[i][4*j+2] = fill_b;
        m_data[i][4*j+3] = fill_a;
      }
  }
}

Texture::~Texture(void)
{
  //for (int i = 0; i != m_levels; ++i) {
  //  delete [] m_data[i];
  //}

  //delete[] m_data;
}

void Texture::save(const char* fname)
{
  std::ofstream hfile(fname, std::ios_base::out);

  /* Write head */
  write_dword(hfile, DDS_MAGIC);
  write_dword(hfile, 124);
  unsigned int dwFlags = DDSD_CAPS | DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PITCH;
  if (m_levels != 1)
    dwFlags |= DDSD_MIPMAPCOUNT;
  write_dword(hfile, dwFlags);
  write_dword(hfile, m_height);
  write_dword(hfile, m_width);
  write_dword(hfile, 4*m_width);
  write_dword(hfile, 0);
  write_dword(hfile, m_levels);

  /* Reserved words */
  for (int i=0; i != 11; ++i)
    write_dword(hfile, 0);

  /* Write DDPIXELFORMAT structure */
  write_dword(hfile, 32);
  if (m_alpha)
    write_dword(hfile, DDPF_RGB | DDPF_ALPHAPIXELS);
  else
    write_dword(hfile, DDPF_RGB);
  write_dword(hfile, 0);
  if (m_alpha)
    write_dword(hfile, 32);
  else
    write_dword(hfile, 24);
  write_dword(hfile, 0x000000ff);
  write_dword(hfile, 0x0000ff00);
  write_dword(hfile, 0x00ff0000);
  write_dword(hfile, 0xff000000);

  /* Write DDSCAPS structure */
  unsigned int dwCaps1 = DDSCAPS_TEXTURE;
  if (m_levels != 1)
    dwCaps1 |= DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
  write_dword(hfile, dwCaps1);
  for (int i=0; i != 3; ++i)
    write_dword(hfile, 0);

  /* Reserved word */
  write_dword(hfile, 0);

  /* Write pixel data */
  int components = m_alpha?4:3;
  for (int i = 0; i != m_levels; ++i)
  {
    for (int y = 0; y != get_height(i); ++y)
      for (int x = 0; x != get_width(i); ++x)
        for (int k = 0; k != components; ++k)
        {
          hfile.put(get_pixel(x, y, k, i));
        }
    //align_dword(hfile);
  }
}

int Texture::get_width(int level) const
{
  int ret_value = m_width >> level;
  if (ret_value == 0) ret_value = 1;
  return ret_value;
}

int Texture::get_height(int level) const
{
  int ret_value = m_height >> level;
  if (ret_value == 0) ret_value = 1;
  return ret_value;
}

void Texture::upload(void) const
{
  if (m_array)
    glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MAX_LEVEL, m_levels-1);
  else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_levels-1);

  for (int i = 0; i != m_levels; ++i)
  {
    if (m_array)
      glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT, i, GL_RGBA, get_width(i), get_height(i)/m_array, m_array, m_border, GL_RGBA, GL_UNSIGNED_BYTE, &m_data[i][0]);
    else if (m_alpha)
      glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, get_width(i), get_height(i), m_border, GL_RGBA, GL_UNSIGNED_BYTE, &m_data[i][0]);
    else
      glTexImage2D(GL_TEXTURE_2D, i, GL_RGB, get_width(i), get_height(i), m_border, GL_RGBA, GL_UNSIGNED_BYTE, &m_data[i][0]);
  }
}

void Texture::set_border(int border)
{
  m_border = border;
}

unsigned int Texture::read_dword(std::istream &hfile)
{
  return static_cast<unsigned int>(hfile.get())
    + 256 * ( static_cast<unsigned int>(hfile.get())
        + 256 * ( static_cast<unsigned int>(hfile.get())
          + 256 * ( static_cast<unsigned int>(hfile.get()) )));
}

void Texture::write_dword(std::ostream &hfile, unsigned int v)
{
  hfile.put((v >> 0) & 0xff);
  hfile.put((v >> 8) & 0xff);
  hfile.put((v >> 16) & 0xff);
  hfile.put((v >> 24) & 0xff);
}

void Texture::align_dword(std::istream &hfile)
{
  unsigned int fillin = (4 - (hfile.tellg() % 4)) % 4;
  for (unsigned int i = 0; i != fillin; ++i)
    hfile.get();
}

void Texture::align_dword(std::ostream &hfile)
{
  unsigned int fillin = (4 - (hfile.tellp() % 4)) % 4;
  for (unsigned int i = 0; i != fillin; ++i)
    hfile.put(0);
}

int Texture::byte_index(unsigned int mask)
{
  switch (mask)
  {
  case 0x000000ff:
    return 0;
  case 0x0000ff00:
    return 1;
  case 0x00ff0000:
    return 2;
  case 0xff000000:
    return 3;
  }

  throw FileFormatError();
}
