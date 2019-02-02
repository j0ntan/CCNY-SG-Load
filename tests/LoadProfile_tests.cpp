#include <gmock/gmock.h>
#include <TextFile.h>
#include <InputSequence.h>
#include <LoadProfile.h>
#include <string>
#include <limits.h>

using namespace testing;

class TextFileMock : public TextFile {
 public:
  ~TextFileMock() override = default;

  MOCK_METHOD3(readBytesUntil, size_t(char, char*, size_t));
  MOCK_METHOD0(available, int());
};

class TextFileMock_tests : public Test {
 public:
  TextFileMock* mock_ptr = new TextFileMock;
  TextFileMock& file_mock = *mock_ptr;

  LoadProfile profile{mock_ptr};
  InputSequence sequence;
  unsigned long duration = ULONG_MAX;
};

bool operator==(const InputSequence& lhs, const InputSequence& rhs) {
  bool retval = lhs.length() == rhs.length();
  if (retval) {
    const uint8_t len = lhs.length();
    for (uint8_t i = 0; i < len && retval; ++i) retval = lhs[i] == rhs[i];
  }
  return retval;
}

InputSequence initialized_Sequence(const char* str) {
  InputSequence retval;
  retval.addInput(str);
  return retval;
}

TEST_F(TextFileMock_tests, noLineAvailableForBlankFile) {
  EXPECT_CALL(file_mock, available()).WillOnce(Return(false));
  ASSERT_FALSE(profile.lineAvailable());
}

TEST_F(TextFileMock_tests, lineAvailableForNonBlankFile) {
  EXPECT_CALL(file_mock, available()).WillOnce(Return(true));
  ASSERT_TRUE(profile.lineAvailable());
}

TEST_F(TextFileMock_tests, readSingleSimpleLine) {
  std::string file_line1{"0 0 0 0"};
  EXPECT_CALL(file_mock, readBytesUntil('\n', NotNull(), _))
      .WillOnce(SetArrayArgument<1>(file_line1.begin(), file_line1.end()));
  profile.readLine(sequence, duration);

  ASSERT_EQ(sequence, initialized_Sequence("A0B0C0"));
  ASSERT_EQ(duration, 0);
}

TEST_F(TextFileMock_tests, readSingleLineWithLengthBelowRequired) {
  std::string file_line1{"0000"};
  EXPECT_CALL(file_mock, readBytesUntil('\n', NotNull(), _))
      .WillOnce(SetArrayArgument<1>(file_line1.begin(), file_line1.end()));
  profile.readLine(sequence, duration);

  ASSERT_EQ(sequence, initialized_Sequence("?"));
  ASSERT_EQ(duration, 0);
}

TEST_F(TextFileMock_tests, readSingleLineWithInvalidChar) {
  std::string file_line1{"0 0 0 a"};
  EXPECT_CALL(file_mock, readBytesUntil('\n', NotNull(), _))
      .WillOnce(SetArrayArgument<1>(file_line1.begin(), file_line1.end()));
  profile.readLine(sequence, duration);

  ASSERT_EQ(sequence, initialized_Sequence("?"));
  ASSERT_EQ(duration, 0);
}

TEST_F(TextFileMock_tests, readSingleLineWithTooManySpaces) {
  std::string file_line1{"0 0 0 0 0"};
  EXPECT_CALL(file_mock, readBytesUntil('\n', NotNull(), _))
      .WillOnce(SetArrayArgument<1>(file_line1.begin(), file_line1.end()));
  profile.readLine(sequence, duration);

  ASSERT_EQ(sequence, initialized_Sequence("?"));
  ASSERT_EQ(duration, 0);
}

TEST_F(TextFileMock_tests, readSingleLineWithValidCharsInWrongOrder) {
  std::string file_line1{"0 00  0"};
  EXPECT_CALL(file_mock, readBytesUntil('\n', NotNull(), _))
      .WillOnce(SetArrayArgument<1>(file_line1.begin(), file_line1.end()));
  profile.readLine(sequence, duration);

  ASSERT_EQ(sequence, initialized_Sequence("?"));
  ASSERT_EQ(duration, 0);
}

TEST_F(TextFileMock_tests, readSingleLineWithMultipleDigitNumbers) {
  std::string file_line1{"16 40 543 1500"};
  EXPECT_CALL(file_mock, readBytesUntil('\n', NotNull(), _))
      .WillOnce(SetArrayArgument<1>(file_line1.begin(), file_line1.end()));
  profile.readLine(sequence, duration);

  ASSERT_EQ(sequence, initialized_Sequence("A16B40C543"));
  ASSERT_EQ(duration, 1500);
}

TEST_F(TextFileMock_tests, readCommentLineFollowedByValidSequenceLine) {
  std::string file_line1{"// this line is a comment"};
  std::string file_line2{"1 2 3 4"};
  EXPECT_CALL(file_mock, readBytesUntil('\n', NotNull(), _))
      .WillOnce(SetArrayArgument<1>(file_line1.begin(), file_line1.end()))
      .WillOnce(SetArrayArgument<1>(file_line2.begin(), file_line2.end()));
  profile.readLine(sequence, duration);

  ASSERT_EQ(sequence, initialized_Sequence("A1B2C3"));
  ASSERT_EQ(duration, 4);
}