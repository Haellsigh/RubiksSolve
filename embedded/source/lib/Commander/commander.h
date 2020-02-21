#ifndef RUBIKSSOLVE_COMMANDER_H
#define RUBIKSSOLVE_COMMANDER_H

#include <cstdint>

#include <CircularBuffer.h>

namespace rubiks {

static constexpr size_t bufferSize         = 1024;
static constexpr size_t commandsBufferSize = 200;

struct Command {
  enum Movement { QuarterTurnPositive = 1, QuarterTurnNegative = -1, HalfTurn = 2 };

  // Invalid motor by default
  uint8_t  motor = 0;
  Movement movement;
};

class Commander {
 public:
  void receiveNewData(uint8_t* data, size_t length);
  void receiveNewData(uint8_t data);

  bool    hasNextCommand();
  Command getNextCommand();

 private:
  void parseReceivedData();
  bool parseCommand(uint8_t data);

 private:
  CircularBuffer<uint8_t, bufferSize>         mBuffer;
  CircularBuffer<Command, commandsBufferSize> mCommandBuffer;
};

}  // namespace rubiks

#endif