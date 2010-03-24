

#if defined(_WIN32)
#define _USE_MATH_DEFINES
#endif

#include <cmath>

#if defined(_WIN32)
#include <math.h>
#endif

#include "FingerInfo.h"
#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::devices::FingerInfo"));
}


FingerInfo::FingerInfo(int id, Vec2 position) :
  m_iFingerId(id),
  m_iHistorySize(0)
  {
  // first position
  addPosition(position);
  }


int
FingerInfo::getFingerId()
{
  return m_iFingerId;
}


void
FingerInfo::addPosition(Vec2 position)
{
  m_PositionHistory.push_front(position);
  if (m_PositionHistory.size() > m_iHistorySize + 1)
    m_PositionHistory.pop_back();
}


Vec2
FingerInfo::getPosition(int index)
{
  if ((index < 0) || ((unsigned int)index >= m_PositionHistory.size()))
  {
    logger.fatal() << "getPosition( " << index << " ): index out of bounds!";
    return Vec2(0.0f, 0.0f);
  }
  return m_PositionHistory[index];
}


Vec2
FingerInfo::getFilteredPosition()
{
  const unsigned int count = m_PositionHistory.size();
  if (count == 0)
    return getPosition();

  Vec2 position(0.0f, 0.0f);
  PositionHistory::iterator iter = m_PositionHistory.begin();
  PositionHistory::iterator iter_end = m_PositionHistory.end();
  float weight_sum = 0.0f;
  const float step = 4.0f / count;
  for (float t = 0.0f; iter != iter_end; iter++, t += step)
  {
    const float weight = std::exp(-t * t / 2.0f) / std::sqrt(2.0f * M_PI);
    weight_sum += weight;
    position = Vec2(position.x + iter->x * weight, position.y + iter->y * weight);
  }
  if (weight_sum != 0.0f)
  {
    return Vec2(position.x / weight_sum, position.y / weight_sum);
  }
  else
  {
    return Vec2(position.x, position.y);
  }
}


void
FingerInfo::setPositionHistorySize(unsigned int size)
{
  m_iHistorySize = size;
}
