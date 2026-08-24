#pragma once

#include <PNGdec.h>
#include <esp_heap_caps.h>

#include <cstdlib>
#include <new>

// CHMI and RainViewer are mutually exclusive radar sources and all rendering
// runs on the single UI task. PNG carries a sizeable work buffer. Keep its
// lifetime shared, but place it in PSRAM so TLS handshakes retain roughly
// 48 kB more of the scarce contiguous internal heap.
inline PNG& RadarPngDecoder() {
  static PNG* decoder = []() -> PNG* {
    void* storage = heap_caps_malloc(sizeof(PNG),
                                     MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (storage == nullptr) {
      storage = heap_caps_malloc(sizeof(PNG),
                                 MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    }
    if (storage == nullptr) std::abort();
    return new (storage) PNG();
  }();
  return *decoder;
}
