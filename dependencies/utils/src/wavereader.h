//  ☀️
#ifndef WAVEREADER_H
#define WAVEREADER_H

#include <cstdint>
#include <iostream>
#include <string>
/*
https://habr.com/ru/articles/488744/
*/
std::int32_t convert_to_int(char *buffer, std::size_t len);
bool load_wav_file_header(std::ifstream &file, std::uint8_t &channels,
                          std::int32_t &sampleRate, std::uint8_t &bitsPerSample,
                          std::int32_t &size);

char *load_wav(const std::string &filename, std::uint8_t &channels,
               std::int32_t &sampleRate, std::uint8_t &bitsPerSample,
               std::int32_t &size);

#endif
