#include "commander.h"

#include <Arduino.h>

namespace rubiks {

void Commander::receiveNewData(uint8_t* data, size_t length) {
  // Put all the data in, overwriting earlier data if necessary
  int i = 0;
  while (i < length) {
    mBuffer.put(data[i]);
    i++;
  }

  // Parse if we received all the movements
  if (data[length - 1] == 255)
    parseReceivedData();
}

void Commander::receiveNewData(uint8_t data) {
  if (!mBuffer.full()) {
    mBuffer.put(data);
  }

  // Parse if we received all the movements
  if (data == 255) {
    parseReceivedData();
  }
}

bool Commander::hasNextCommand() {
  return !mCommandBuffer.empty();
}

Command Commander::getNextCommand() {
  return mCommandBuffer.get();
}

void Commander::parseReceivedData() {
  // Make sure we start after the first 0.
  if (mBuffer.removeUntil(0)) {
    // Parse until command buffer is full, there was an error, or we read a 0 or 255.
    while (parseCommand(mBuffer.get()))
      ;
  }
}

/**
 * Returns false if:
 *  - data signifies the start or end of a new chain
 *  - buffer is full, we can't add any more commands
 */
bool Commander::parseCommand(uint8_t data) {
  if (data == 0 || data == 255)
    return false;

  Command command;

  command.motor = data & 0b00000111;
  if (command.motor == 0)
    return false;  // invalid motor

  switch ((data & 0b00011000) >> 3) {
    case 0:
      command.movement = Command::QuarterTurnPositive;
      break;
    case 1:
      command.movement = Command::QuarterTurnNegative;
      break;
    case 2:
      command.movement = Command::HalfTurn;
      break;
    default:
      return false;  // invalid movement
  }

  mCommandBuffer.put(command);

  if (mCommandBuffer.full()) {
    // Buffer is full, stop parsing
    return false;
  }

  return true;
}

}  // namespace rubiks