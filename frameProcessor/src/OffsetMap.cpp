/*
 *  OffsetMap.h
 *
 *  Created on: 13 June 2017
 *    Author: Gary Yendell
 */

#include <assert.h>

#include <OffsetMap.h>

namespace FrameProcessor
{

OffsetMap::OffsetMap()
{
  this->logger_ = Logger::getLogger("FW.OffsetMap");
  this->logger_->setLevel(Level::getTrace());
  LOG4CXX_TRACE(logger_, "OffsetMap constructor.");
}

OffsetMap::~OffsetMap()
{
}

/** Add a global starting adjustment
 *
 * Should be called before any frames received. Offset map will be checked to be
 * empty.
 *
 */
void OffsetMap::setInitialOffset(size_t frame_no)
{
  assert(offsets_.empty());
  addFrameOffsetAdjustment(frame_no, frame_no);
}

/** Add an adjustment to the map
 *
 * Should only apply to future frames, otherwise previous frames that would
 * have had this offset applied will be in the wrong place.
 *
 */
void OffsetMap::addFrameOffsetAdjustment(size_t active_frame_no,
                                         size_t adjustment)
{
  LOG4CXX_DEBUG(logger_, "Frame number: " << active_frame_no);
  LOG4CXX_DEBUG(logger_, "Offset adjustment: " << adjustment);
  // Avoid negative offsets
  assert(active_frame_no >= adjustment);
  // Only add if there are no current adjustments with a later active frame
  assert(offsets_.empty() || active_frame_no > offsets_.rbegin()->first);

  size_t total_adjustment;
  if (offsets_.empty()) {
    // Simply apply this adjustment
    total_adjustment = adjustment;
  }
  else {
    // Apply this adjustment in addition to current adjustment
    total_adjustment = adjustment + offsets_.rbegin()->second;
  }
  offsets_[active_frame_no] = total_adjustment;
}

/** Calculate adjusted offset for given frame
 *
 * Search offset map for adjustments including this frame and apply, otherwise
 * just return frame number; there is no active offset adjustment for the frame.
 *
 */
size_t OffsetMap::adjustFrameOffset(size_t frame_no)
{
  LOG4CXX_DEBUG(logger_, "Raw frame number: " << frame_no);
  std::map<size_t, size_t>::reverse_iterator it;  // active_frame -> offset
  for (it = offsets_.rbegin(); it != offsets_.rend(); it++) {
    if (it->first <= frame_no) {
      LOG4CXX_DEBUG(logger_, "Frame offset adjustment: " << it->second);
      return frame_no - it->second;
    }
  }
  return frame_no;
}

} /* namespace FrameProcessor */
