#ifndef PARSE_H
#define PARSE_H

#include <stdint.h>
#include "Scan.h"
#include "RelayState.h"

struct ParseAnalysis {
  /*
  Note: Although DC voltage does not have a phase, we store it's position, in an
  input sequence, alongside the AC phases (A,B,C + DC) for convenience. */
  int phase_position[4] = {-1, -1, -1, -1};
  uint8_t phase_count[4] = {0};

  // references to improve code clarity
  int& position_phaseA = phase_position[0];
  int& position_phaseB = phase_position[1];
  int& position_phaseC = phase_position[2];
  int& position_DC = phase_position[3];
  uint8_t& count_phaseA = phase_count[0];
  uint8_t& count_phaseB = phase_count[1];
  uint8_t& count_phaseC = phase_count[2];
  uint8_t& count_DC = phase_count[3];

  int* numerical_values = nullptr;
  uint8_t numericals_size = 0;

  ParseAnalysis& operator=(const ParseAnalysis& rhs) {
    for (uint8_t i = 0; i < 4; i++) {
      phase_position[i] = rhs.phase_position[i];
      phase_count[i] = rhs.phase_count[i];
    }

    if (numericals_size != rhs.numericals_size) {
      delete[] numerical_values;
      numerical_values = new int[rhs.numericals_size];
    }

    numericals_size = rhs.numericals_size;
    for (uint8_t i = 0; i < numericals_size; i++)
      numerical_values[i] = rhs.numerical_values[i];

    return *this;
  }

  ~ParseAnalysis() {
    delete[] numerical_values;
    numerical_values = nullptr;
  }
};

namespace helper {
void initializeNumericals(int[], uint8_t);

void analyzeThisToken(const Token&, ParseAnalysis&, uint8_t&);

bool hasAtLeastOneNumber(const ParseAnalysis&);

bool endsWithNumber(const ParseAnalysis&);

bool numbersAreWithinRange(const ParseAnalysis&);

bool beginsWithPhase(const ParseAnalysis&);

bool phaseAppearsAtMostOnce(const ParseAnalysis&);

bool hasNoLeadingZeros(const ParseAnalysis&);

bool phasesInOrder(const ParseAnalysis&);

void condenseConsecutiveNumericals(ParseAnalysis&);

void assignNewRelayValues(const ParseAnalysis&, RelayState&);

uint8_t getValueAfterThisPosition(uint8_t, const ParseAnalysis&);
}  // namespace helper

ParseAnalysis analyzeTokens(const TokenSet& inputTokens) {
  ParseAnalysis result;
  result.numerical_values = new int[inputTokens.size()];
  result.numericals_size = inputTokens.size();

  helper::initializeNumericals(result.numerical_values, result.numericals_size);

  for (uint8_t i = 0; i < inputTokens.size(); i++)
    helper::analyzeThisToken(inputTokens[i], result, i);

  return result;
}

bool hasNoParseErrors(const ParseAnalysis& analysis) {
  return helper::hasAtLeastOneNumber(analysis) &&
         helper::endsWithNumber(analysis) &&
         helper::numbersAreWithinRange(analysis) &&
         helper::beginsWithPhase(analysis) &&
         helper::phaseAppearsAtMostOnce(analysis) &&
         helper::hasNoLeadingZeros(analysis) && helper::phasesInOrder(analysis);
}

void parseNewRelayState(ParseAnalysis& analysis, RelayState& currentState) {
  helper::condenseConsecutiveNumericals(analysis);

  helper::assignNewRelayValues(analysis, currentState);
}

void helper::initializeNumericals(int positions[], uint8_t size) {
  for (uint8_t i = 0; i < size; i++) positions[i] = -1;
}

void helper::analyzeThisToken(const Token& token, ParseAnalysis& analysis,
                              uint8_t& position) {
  switch (token) {
    case Token::NUM0:
      analysis.numerical_values[position] = 0;
      break;
    case Token::NUM1:
      analysis.numerical_values[position] = 1;
      break;
    case Token::NUM2:
      analysis.numerical_values[position] = 2;
      break;
    case Token::NUM3:
      analysis.numerical_values[position] = 3;
      break;
    case Token::NUM4:
      analysis.numerical_values[position] = 4;
      break;
    case Token::NUM5:
      analysis.numerical_values[position] = 5;
      break;
    case Token::NUM6:
      analysis.numerical_values[position] = 6;
      break;
    case Token::NUM7:
      analysis.numerical_values[position] = 7;
      break;
    case Token::NUM8:
      analysis.numerical_values[position] = 8;
      break;
    case Token::NUM9:
      analysis.numerical_values[position] = 9;
      break;
    case Token::AC_phaseA:
      analysis.position_phaseA = position;
      analysis.count_phaseA++;
      break;
    case Token::AC_phaseB:
      analysis.position_phaseB = position;
      analysis.count_phaseB++;
      break;
    case Token::AC_phaseC:
      analysis.position_phaseC = position;
      analysis.count_phaseC++;
      break;
    case Token::DC:
      analysis.position_DC = position;
      analysis.count_DC++;
      break;
    case Token::INVALID:
      break;  // do nothing, inputs containing INVALID won't be parsed
  }
}

bool helper::hasAtLeastOneNumber(const ParseAnalysis& analysis) {
  for (uint8_t i = 0; i < analysis.numericals_size; i++)
    if (analysis.numerical_values[i] > -1) return true;
  return false;
}

bool helper::endsWithNumber(const ParseAnalysis& analysis) {
  return analysis.numerical_values[analysis.numericals_size - 1] != -1;
}

bool helper::numbersAreWithinRange(const ParseAnalysis& analysis) {
  // DC has max of 2 relays
  if (analysis.count_DC > 0 &&
      analysis.numerical_values[analysis.position_DC + 1] > 2)
    return false;

  unsigned value = 0;
  for (uint8_t i = 0; i < analysis.numericals_size; i++) {
    if (analysis.numerical_values[i] == -1)
      value = 0;
    else
      value = 10 * value + analysis.numerical_values[i];
    if (value > 16) return false;
  }
  return true;
}

bool helper::beginsWithPhase(const ParseAnalysis& analysis) {
  return analysis.position_phaseA == 0 || analysis.position_phaseB == 0 ||
         analysis.position_phaseC == 0 || analysis.position_DC == 0;
}

bool helper::phaseAppearsAtMostOnce(const ParseAnalysis& analysis) {
  return analysis.count_phaseA <= 1 && analysis.count_phaseB <= 1 &&
         analysis.count_phaseC <= 1 && analysis.count_DC <= 1;
}

bool helper::hasNoLeadingZeros(const ParseAnalysis& analysis) {
  for (uint8_t i = 0; i < analysis.numericals_size; i++)
    if (analysis.numerical_values[i] == 0 && i + 1 < analysis.numericals_size &&
        analysis.numerical_values[i + 1] > 0)
      return false;
  return true;
}

bool helper::phasesInOrder(const ParseAnalysis& analysis) {
  bool phaseA_exists = analysis.count_phaseA > 0;
  bool phaseB_exists = analysis.count_phaseB > 0;
  bool phaseC_exists = analysis.count_phaseC > 0;
  bool DC_exists = analysis.count_DC > 0;

  bool phaseA_is_good = !phaseA_exists;
  bool phaseB_is_good = !phaseB_exists;
  bool phaseC_is_good = !phaseC_exists;
  bool DC_is_good = !DC_exists;

  if (phaseA_exists) phaseA_is_good = analysis.position_phaseA == 0;
  if (phaseB_exists)
    phaseB_is_good = analysis.position_phaseB > analysis.position_phaseA;
  if (phaseC_exists)
    phaseC_is_good = analysis.position_phaseC > analysis.position_phaseB &&
                     analysis.position_phaseC > analysis.position_phaseA;
  if (DC_exists)
    DC_is_good = analysis.position_DC > analysis.position_phaseC &&
                 analysis.position_DC > analysis.position_phaseB &&
                 analysis.position_DC > analysis.position_phaseA;

  return phaseA_is_good && phaseB_is_good && phaseC_is_good && DC_is_good;
}

void helper::condenseConsecutiveNumericals(ParseAnalysis& analysis) {
  for (uint8_t i = 0; i < analysis.numericals_size; i++)
    if (analysis.numerical_values[i] == 1 && i + 1 < analysis.numericals_size &&
        analysis.numerical_values[i + 1] >= 0) {
      analysis.numerical_values[i] = 10 + analysis.numerical_values[i + 1];
      analysis.numerical_values[i + 1] = -1;
      i++;
    }
}

void helper::assignNewRelayValues(const ParseAnalysis& analysis,
                                  RelayState& currentState) {
  bool phaseA_exists = analysis.count_phaseA > 0;
  bool phaseB_exists = analysis.count_phaseB > 0;
  bool phaseC_exists = analysis.count_phaseC > 0;
  bool DC_exists = analysis.count_DC > 0;

  if (phaseA_exists)
    currentState.phaseA =
        helper::getValueAfterThisPosition(analysis.position_phaseA, analysis);
  if (phaseB_exists)
    currentState.phaseB =
        helper::getValueAfterThisPosition(analysis.position_phaseB, analysis);
  if (phaseC_exists)
    currentState.phaseC =
        helper::getValueAfterThisPosition(analysis.position_phaseC, analysis);
  if (DC_exists)
    currentState.DC =
        helper::getValueAfterThisPosition(analysis.position_DC, analysis);
}

uint8_t helper::getValueAfterThisPosition(uint8_t pos,
                                          const ParseAnalysis& analysis) {
  uint8_t index = pos + 1;
  while (analysis.numerical_values[index] == -1) index++;
  return analysis.numerical_values[index];
}

#endif  // PARSE_H
