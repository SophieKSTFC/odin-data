/*
 *  OffsetCalculator.h
 *
 *  Created on: 13 June 2017
 *    Author: Gary Yendell
 */

#ifndef ODINDATA_OFFSETCALCULATOR_H
#define ODINDATA_OFFSETCALCULATOR_H


namespace FrameProcessor
{

/** Class to manage relative offset adjustments and calculate absolute offsets
 *
 */
class OffsetCalculator
{
public:
  OffsetCalculator();
  virtual ~OffsetCalculator();

  void rewind(size_t active_frame_no, size_t adjustment);
  /*
   * setFrameOffsetAdjustment(active_frame_no, adjustment)
   */
  void setInitialFrame(size_t frame_no);
  /*
   * assert offsets_.empty()? Or re-calculate? We should only get things in this order: (setInitialFrame), run, (rewind, rewind, ...). What would the use case be for allowing otherwise?
   * setFrameOffsetAdjustment(frame_no, frame_no)
   */
  size_t adjustFrameOffset(size_t frame_no);
  /*
   * Iterate through map backwards until value < frame_no (This will be the first (last) one unless we get frames out of order)
   *   return absolute_offset
   * else return frame_no (This should happen if the map is empty or only has future adjustments)
   */
  void configure(OdinData::IpcMessage& config, OdinData::IpcMessage& reply);
  /*
   * If CONFIG_REWIND_FRAMES && CONFIG_REWIND_ACTIVE_FRAME
   *   rewind(frame, offset)
   * else if CONFIG_INITIAL_FRAME_ID
   *   setInitialFrame(frame)
   */

private:
  /** Configuration constant for number of frames to rewind by */
  static const std::string CONFIG_REWIND_FRAMES;
  /** Configuration constant for the first frame after rewind*/
  static const std::string CONFIG_REWIND_ACTIVE_FRAME;
  /** Configuration constant for the initial frame ID */
  static const std::string CONFIG_INITIAL_FRAME_ID;

  void setFrameOffsetAdjustment(size_t active_frame_no, size_t adjustment);
  /*
   * assert offset >= active_frame_no. Otherwise we will get a negative absolute offset
   * assert active_frame_no > most recent complete frame. Otherwise frames that have already been written should have had this applied and so must be in the wrong place
   * assert active_frame_no > any currently in map? Otherwise the currently existing adjustment will never be used. Maybe a use case for this? Could remove current entry.
   * abs_offset = map.rbegin()->second + adjustment || adjustment if map.empty()
   * Add new entry frame_id -> abs_offset to offset_adjustments_
   */

  /** Pointer to logger */
  LoggerPtr logger_;
  /** Map of offset adjustments - frame id -> absolute offset */
  std::map<size_t, size_t> offset_adjustments_;
  /*
   * Need a map for each file name? Map of maps
   *   If we get a delayed frame from file 1 after we have restarted acquisition and start getting file 2 frames?
   */

};

} /* namespace FrameProcessor */
#endif //ODINDATA_OFFSETCALCULATOR_H
