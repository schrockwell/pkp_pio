
#define TEST(desc, x)                      \
  std::cout << desc << "...";              \
  if (x)                                   \
  {                                        \
    std::cout << " [OK]" << std::endl;     \
  }                                        \
  else                                     \
  {                                        \
    std::cout << " [FAILED]" << std::endl; \
    return 1;                              \
  }

#define ASSERT(x, message) \
  if (!(x))                \
  {                        \
    std::cout << message;  \
    return false;          \
  }
