/*
 *  OffsetMap.h
 *
 *  Created on: 13 June 2017
 *    Author: Gary Yendell
 */

#include <log4cxx/logger.h>
using namespace log4cxx;

#ifndef ODINDATA_OFFSETMAP_H
#define ODINDATA_OFFSETMAP_H


namespace FrameProcessor
{

/** Class to manage relative offset adjustments and calculate absolute offsets
 *
 */
class OffsetMap
{
public:
  OffsetMap();
  virtual ~OffsetMap();

  void addFrameOffsetAdjustment(size_t active_frame_no, size_t adjustment);
  void setInitialOffset(size_t frame_no);
  size_t adjustFrameOffset(size_t frame_no);

private:
  /** Configuration constant for number of frames to rewind by */
  static const std::string CONFIG_REWIND_FRAMES;
  /** Configuration constant for the first frame after rewind*/
  static const std::string CONFIG_REWIND_ACTIVE_FRAME;
  /** Configuration constant for the initial frame ID */
  static const std::string CONFIG_INITIAL_FRAME_ID;

  /** Pointer to logger */
  LoggerPtr logger_;

  /** Map of offset adjustments - frame id -> absolute offset */
  std::map<size_t, size_t> offsets_;

};

} /* namespace FrameProcessor */
#endif //ODINDATA_OFFSETMAP_H
