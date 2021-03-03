#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <streambuf>
#include <vector>

std::set<char> delims{'\\'};

std::vector<std::string> splitpath(const std::string& str,
                                   const std::set<char> delimiters) {
  std::vector<std::string> result;

  char const* pch = str.c_str();
  char const* start = pch;
  for (; *pch; ++pch) {
    if (delimiters.find(*pch) != delimiters.end()) {
      if (start != pch) {
        std::string str(start, pch);
        result.push_back(str);
      } else {
        result.push_back("");
      }
      start = pch + 1;
    }
  }
  result.push_back(start);

  return result;
}

int find_substring(std::string str, std::string pattern, int index = 0) {
  // Step 0. Should not be empty string
  if (str.size() == 0 || pattern.size() == 0) return -1;

  // Step 1. Compute failure function
  int failure[pattern.size()];
  std::fill(failure, failure + pattern.size(), -1);

  for (int r = 1, l = -1; r < pattern.size(); r++) {
    while (l != -1 && pattern[l + 1] != pattern[r]) l = failure[l];

    // assert( l == -1 || pattern[l+1] == pattern[r]);
    if (pattern[l + 1] == pattern[r]) failure[r] = ++l;
  }

  // Step 2. Search pattern
  int tail = -1;
  for (index; index < str.size(); index++) {
    while (tail != -1 && str[index] != pattern[tail + 1]) tail = failure[tail];

    if (str[index] == pattern[tail + 1]) tail++;

    if (tail == pattern.size() - 1) return index - tail;
  }
  return -1;
}
bool endswith(std::string const& fullString, std::string const& ending) {
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare(fullString.length() - ending.length(),
                                    ending.length(), ending));
  } else {
    return false;
  }
}
bool mp4(std::string video) {
  std::vector<std::string> path = splitpath(video, delims);
  std::fstream binaryFile(video,
                          std::ios::in | std::ios::out | std::ios::binary);
  std::string str((std::istreambuf_iterator<char>(binaryFile)),
                  std::istreambuf_iterator<char>());
  // mvhd
  char target[5] = {char(0x6D), char(0x76), char(0x68), char(0x64), char(0x00)};
  int mvhd_position = find_substring(str, target);
  binaryFile.seekp(mvhd_position + 20);
  char new_value[3]{char(0xD6), char(0xD8), char(0x00)};
  binaryFile << char(0x00);
  binaryFile.seekp(mvhd_position + 21);
  binaryFile << char(0x00);
  binaryFile.seekp(mvhd_position + 22);
  binaryFile << new_value;
  std::cout << "+ " << path.back() << " is ready!\n";
  return 0;
}

bool webm(std::string video) {
  std::vector<std::string> path = splitpath(video, delims);
  std::fstream binaryFile(video,
                          std::ios::in | std::ios::out | std::ios::binary);
  std::string str((std::istreambuf_iterator<char>(binaryFile)),
                  std::istreambuf_iterator<char>());
  // 2AD7B1
  char time_stamp[4] = {char(0x2A), char(0xD7), char(0xB1)};
  char duration[3] = {char(0x44), char(0x89)};
  int duration_position =
      find_substring(str, duration, find_substring(str, time_stamp));
  binaryFile.seekp(duration_position + 4);
  binaryFile << char(0x70);
  binaryFile.seekp(duration_position + 5);
  binaryFile << char(0x00);
  std::cout << "+ " << path.back() << " is ready!\n";
  return 0;
}
void modify_video(std::string video) {
  std::vector<std::string> path = splitpath(video, delims);

  if (endswith(video, ".mp4")) {
    mp4(video);
  } else if (endswith(video, ".webm")) {
    webm(video);
  } else {
    std::cout << '-' << path.back()
              << " is not supported file by TikTok or me :3\n";
  }
}

int main(int argc, char* argv[]) {
  std::string socials =
      "\nSocials:\n\
      Github: @D-0M - https://github.com/D-0M/\n\
      Twitter: @Do00m - https://twitter.com/Do00M/\n\
      Instagram: @Bdaaa - https://www.instagram.com/bdaaa/\n\
      Tiktok: @Do00m - https://www.tiktok.com/@do00m/\n\
      Telegram: @OfficalDom - https://t.me/OfficalDom/\n\
      Discord: https://discord.gg/MPbkE2Q/\n\
      Website: D-0m.tk - simply https://d-0m.tk\n\n";

  if (argc <= 1) {
    std::cout << "\n###Please drug and drop some videos on the icon.###\n"
              << socials;
    system("pause");
    return 0;
  }
  std::cout << "Please wait while preparing your file/s";
  std::cout
      << "To use this tool just drag your video and drop it on the icon.\n";
  for (int index = 1; index < argc; index++) {
    modify_video(argv[index]);
  };
  std::cout << socials;
  system("pause");
  return 0;
}