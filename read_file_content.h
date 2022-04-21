// Copyright (C) 2018-2020  Igara Studio S.A.
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

using buffer = std::vector<uint8_t> ;
const size_t kChunkSize = 1024*64; // 64k

buffer read_file_content(const std::string& filename)
{
  FILE* f = std::fopen(filename.c_str(), "rb");
  if (!f)
    throw std::runtime_error("file not found");

  buffer buf;
  size_t pos = 0;

  while (!std::feof(f)) {
    buf.resize(buf.size() + kChunkSize);
    size_t read_bytes = std::fread(&buf[pos], 1, kChunkSize, f);
    pos += read_bytes;
    if (read_bytes < kChunkSize) {
      buf.resize(pos);
      break;
    }
  }

  std::fclose(f);
  return buf;
}
