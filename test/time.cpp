#include <iostream>
#include <chrono>

int main() {
  auto now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
  auto end = now + 5;

  // 输出当前时间点和未来时间点
  std::cout << "Current time point: " << now << std::endl;
  std::cout << "Future time point (+5 seconds): " << end << std::endl;

  return 0;
}