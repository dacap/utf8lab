// Copyright (c) 2022  David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "laf_decoder.h"
#include "read_file_content.h"

#include <benchmark/benchmark.h>

static void BM_LafDecoder(benchmark::State& state) {
  buffer fc = read_file_content("utf8.data");
  std::string c((const char*)&fc[0]);

  while (state.KeepRunning()) {
    utf8_decode decode(c);
    while (int chr = decode.next()) {
      benchmark::DoNotOptimize(chr);
    }
  }
}
BENCHMARK(BM_LafDecoder);

BENCHMARK_MAIN();
