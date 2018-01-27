#ifndef STRINGDOUBLE_H
#define STRINGDOUBLE_H

#include <string>
#include <algorithm>
#include <locale>
#include <utility>

class StringDouble {
 public:
  // constructors
  StringDouble(const StringDouble& i) : str(i.str) {}
  StringDouble(StringDouble&& i) : str(std::move(i.str)) {}
  explicit StringDouble(const char* cstr = "") : str(cstr) {}
  explicit StringDouble(char c) : str({c}) {}  // uses initilization list
  explicit StringDouble(unsigned char val) : str(std::to_string(val)) {}
  explicit StringDouble(int val) : str(std::to_string(val)) {}
  explicit StringDouble(unsigned int val) : str(std::to_string(val)) {}
  explicit StringDouble(long val) : str(std::to_string(val)) {}
  explicit StringDouble(unsigned long val) : str(std::to_string(val)) {}
  explicit StringDouble(float val) : str(std::to_string(val)) {}
  explicit StringDouble(double val) : str(std::to_string(val)) {}

  // memory management
  void reserve(unsigned int amount) { str.reserve(amount); }
  unsigned int length() const { return str.length(); }

  // copy/assignment
  StringDouble& operator=(const StringDouble& rhs) {
    this->str = rhs.str;
    return *this;
  }
  StringDouble& operator=(const char* rhs) {
    this->str = rhs;
    return *this;
  }
  StringDouble& operator=(StringDouble&& rhs) {
    this->str = std::move(rhs.str);
    return *this;
  }

  // concatenate
  unsigned char concat(const StringDouble& str) {
    this->str += str.str;
    return 1;
  }
  unsigned char concat(const char* cstr) {
    this->str += cstr;
    return 1;
  }
  unsigned char concat(char c) {
    this->str += c;
    return 1;
  }
  unsigned char concat(unsigned char c) {
    this->str += std::to_string(c);
    return 1;
  }
  unsigned char concat(int num) {
    this->str += std::to_string(num);
    return 1;
  }
  unsigned char concat(unsigned int num) {
    this->str += std::to_string(num);
    return 1;
  }
  unsigned char concat(long num) {
    this->str += std::to_string(num);
    return 1;
  }
  unsigned char concat(unsigned long num) {
    this->str += std::to_string(num);
    return 1;
  }
  unsigned char concat(float num) {
    this->str += std::to_string(num);
    return 1;
  }
  unsigned char concat(double num) {
    this->str += std::to_string(num);
    return 1;
  }

  StringDouble& operator+=(const StringDouble& rhs) {
    concat(rhs);
    return *this;
  }
  StringDouble& operator+=(const char* rhs) {
    concat(rhs);
    return *this;
  }
  StringDouble& operator+=(char rhs) {
    concat(rhs);
    return *this;
  }
  StringDouble& operator+=(unsigned char rhs) {
    concat(rhs);
    return *this;
  }
  StringDouble& operator+=(int rhs) {
    concat(rhs);
    return *this;
  }
  StringDouble& operator+=(unsigned int rhs) {
    concat(rhs);
    return *this;
  }
  StringDouble& operator+=(long rhs) {
    concat(rhs);
    return *this;
  }
  StringDouble& operator+=(unsigned long rhs) {
    concat(rhs);
    return *this;
  }
  StringDouble& operator+=(float rhs) {
    concat(rhs);
    return *this;
  }
  StringDouble& operator+=(double rhs) {
    concat(rhs);
    return *this;
  }
  StringDouble operator+(const StringDouble& rhs) const {
    return StringDouble((this->str + rhs.str).c_str());
  }

  // comparison
  int compareTo(const StringDouble& str) const {
    return this->str.compare(str.str);
  }
  unsigned char equals(const StringDouble& str) const {
    return compareTo(str) == 0;
  }
  unsigned char equals(const char* cstr) const {
    return this->str.compare(cstr) == 0;
  }
  unsigned char operator==(const StringDouble& rhs) const {
    return equals(rhs);
  }
  unsigned char operator==(const char* rhs) const { return equals(rhs); }
  unsigned char operator!=(const StringDouble& rhs) const {
    return !equals(rhs);
  }
  unsigned char operator!=(const char* rhs) const { return !equals(rhs); }
  unsigned char operator<(const StringDouble& rhs) const {
    return compareTo(rhs) < 0;
  }
  unsigned char operator>(const StringDouble& rhs) const {
    return compareTo(rhs) > 0;
  }
  unsigned char operator<=(const StringDouble& rhs) const {
    return compareTo(rhs) < 0 || equals(rhs);
  }
  unsigned char operator>=(const StringDouble& rhs) const {
    return compareTo(rhs) > 0 || equals(rhs);
  }
  unsigned char equalsIgnoreCase(const StringDouble& str) {
    return length() == str.length() &&
           std::equal(this->str.begin(), this->str.end(), str.str.begin(),
                      [](const char& a, const char& b) {
                        return (std::tolower(a) == std::tolower(b));
                      });
  }
  unsigned char startsWith(const StringDouble& prefix) const {
    return this->str.find(prefix.str) == 0;
  }
  unsigned char startsWith(const StringDouble& prefix,
                           unsigned int offset) const {
    return this->str.find(prefix.str, offset) == 0;
  }
  unsigned char endsWith(const StringDouble& suffix) const {
    if (this->length() > suffix.length())
      return substring(this->length() - suffix.length(), suffix.length()) ==
             suffix;
    else
      return false;
  }

  // character access
  char charAt(unsigned int index) const { return this->str.at(index); }
  void setCharAt(unsigned int index, char c) { this->str[index] = c; }
  char operator[](unsigned int index) const { return this->str[index]; }
  char& operator[](unsigned int index) { return this->str[index]; }
  const char* c_str() const { return this->str.c_str(); }

  // search
  int indexOf(char ch) const { return indexOf(ch, 0); }
  int indexOf(char ch, unsigned int fromIndex) const {
    return indexOf(StringDouble(ch), fromIndex);
  }
  int indexOf(const StringDouble& str) const { return indexOf(str, 0); }
  int indexOf(const StringDouble& str, unsigned int fromIndex) const {
    unsigned int index = this->str.find(str.str, fromIndex);
    if (index == std::string::npos)
      return -1;
    else
      return index;
  }
  int lastIndexOf(char ch) const {
    return lastIndexOf(ch, this->str.length() - 1);
  }
  int lastIndexOf(char ch, unsigned int fromIndex) const {
    return lastIndexOf(StringDouble(ch), fromIndex);
  }
  int lastIndexOf(const StringDouble& str) const {
    return lastIndexOf(str, this->length() - 1);
  }
  int lastIndexOf(const StringDouble& str, unsigned int fromIndex) const {
    unsigned int index = this->str.rfind(str.str, fromIndex);
    if (index == std::string::npos)
      return -1;
    else
      return index;
  }
  StringDouble substring(unsigned int beginIndex) const {
    return substring(beginIndex, this->length());
  };
  StringDouble substring(unsigned int beginIndex, unsigned int endIndex) const {
    return StringDouble{
        this->str.substr(beginIndex, endIndex - beginIndex).c_str()};
  }

  // modification
  void replace(char find, char replace) {
    StringDouble::replace(StringDouble(find), StringDouble(replace));
  }
  void replace(const StringDouble& find, const StringDouble& replace) {
    std::string::size_type pos = 0;
    do {
      pos = this->str.find(find.str, pos);
      this->str.replace(pos, replace.str.length(), replace.str);
      pos += replace.length();
    } while (pos != std::string::npos);
  }
  void remove(unsigned int index) { this->str.erase(index); }
  void remove(unsigned int index, unsigned int count) {
    this->str.erase(index, count);
  }
  void toLowerCase() {
    std::locale loc;
    for (std::string::size_type i = 0; i < this->str.length(); ++i)
      this->str[i] = std::tolower(this->str[i], loc);
  }
  void toUpperCase() {
    std::locale loc;
    for (std::string::size_type i = 0; i < this->str.length(); ++i)
      this->str[i] = std::toupper(this->str[i], loc);
  }
  void trim() {
    std::size_t begin = this->str.find_first_not_of(' ');
    std::size_t length = this->str.find_last_not_of(' ') - begin + 1;
    this->str = this->str.substr(begin, length);
  }

  // parsing/conversion
  long toInt() const { return stol(this->str); }
  float toFloat() const { return stof(this->str); }
  double toDouble() const { return stod(this->str); }

 private:
  std::string str;
};

#endif  // STRINGDOUBLE_H
